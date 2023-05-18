//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header
#include <boost/thread.hpp>
// 2. My header
#include <xmsinterp/interpolate/InterpIdw.h>

// 3. Standard Library Headers

// 4. External Library Headers

// 5. Shared Headers
#include <xmscore/math/math.h>
#include <xmscore/stl/utility.h>
#include <xmscore/stl/vector.h>
#include <xmsgrid/geometry/GmPtSearch.h>
#include <xmsinterp/interpolate/InterpSignals.h>
#include <xmsinterp/interpolate/InterpUtil.h>
#include <xmsinterp/interpolate/detail/NodalFunc.h>
#include <xmsgrid/geometry/geoms.h>
#include <xmsinterp/thread/ThreadLoop.h>
#include <xmsinterp/thread/ThreadMgr.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/misc/xmstype.h>

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------
/// \brief Implementation of InterpIdw class
class InterpIdwImpl : public InterpIdw
{
public:
  /// \brief Threading class to compute idw interpolation in parallel
  class InterpThread : public ThreadLoop
  {
  public:
    /// \brief constructor
    /// \param[in] a_ Idw interpolation class
    /// \param[out] a_s Scalars filled by this class
    /// \param[in] a_p Pts being interpolated to
    InterpThread(InterpIdwImpl& a_, VecFlt& a_s, const VecPt3d& a_p)
    : m_interp(a_)
    , m_scalarTo(a_s)
    , m_pts(a_p)
    {
    } // InterpThread

    InterpIdwImpl& m_interp;        ///< Idw interpolation class
    VecFlt& m_scalarTo; ///< Scalars filled by this class
    const VecPt3d& m_pts; ///< Pts being interpolated to

    /// \brief creates an instance of this class for a new thread
    /// \return Shared pointer to ThreadLoop.
    BSHP<ThreadLoop> CreateForNewThread() override
    {
      BSHP<InterpThread> r(new InterpThread(m_interp, m_scalarTo, m_pts));
      BSHP<ThreadLoop> q = BDPC<ThreadLoop>(r);
      return q;
    }

  private:
    /// \brief Actually does the interpolation in a thread
    void Worker() override
    {
      int i = (int)CurrIdx();
      m_scalarTo[i] = m_interp.InterpToPt(m_pts[i], i);
    }
  };

  InterpIdwImpl();
  virtual ~InterpIdwImpl();

  virtual void SetPtsTris(BSHP<VecPt3d> a_pts, BSHP<VecInt> a_tris) override;
  void SetScalars(const float* a_scalar, size_t a_n) override;
  void SetScalars(BSHP<VecFlt> a_scalar) override;
  virtual void SetPts(BSHP<VecPt3d> a_pts, bool a_2d) override;
  // this method will run parallel
  virtual float InterpToPt(const Pt3d& a_pt) override;
  virtual void InterpToPts(const VecPt3d& a_pts, VecFlt& a_scalars) override;
  virtual void SetPtActivity(DynBitset& a_activity) override;
  /// \brief Sets triangle activity. Ignored by IDW.
  virtual void SetTriActivity(DynBitset&) override {}
  virtual const BSHP<VecPt3d> GetPts() const override;
  virtual const BSHP<VecInt> GetTris() const override;
  virtual const BSHP<VecFlt> GetScalars() const override;
  virtual DynBitset GetPtActivity() const override;
  virtual DynBitset GetTriActivity() const override;
  virtual void SetTrunc(double a_sMax, double a_sMin) override;

  /// \brief get the option to truncate interpolated values
  /// \return the option to truncate interpolated values
  virtual bool GetTruncateInterpolatedValues() const override { return m_trunc; }
  /// \brief get minimum truncation value
  /// \return the minimum truncation value
  virtual double GetTruncMin() const override { return m_truncMin; }
  /// \brief get the maximum truncation value
  /// \return the maximum truncation value
  virtual double GetTruncMax() const override { return m_truncMax; }
  /// \brief get the idw power (1/d or 1/d^2 or 1/d^3)
  /// \return the power
  virtual double GetPower() const override { return m_power; }
  /// \brief get the search options number of nearest points
  /// \return the number of nearest points
  virtual int GetSearchOptsNumNearestPts() const override { return m_nNearestPts; }
  /// \brief get the search options option to do a quadrant or octant search
  /// \return the option to do a quadrant or octant search
  virtual bool GetSearchOptsUseQuadrantSearch() const override { return m_quadOctSearch; }
  /// \brief get the weight calculation method
  /// \return the weight calculation method
  virtual WeightEnum GetWeightCalcMethod() const override
  {
    if (!m_modifiedShepardWeights)
      return InterpIdw::CLASSIC;
    return InterpIdw::MODIFIED;
  }
  /// \brief get the type of nodal function (constant, gradient plane, quadratic)
  /// \return the type of nodal function
  virtual NodalFuncEnum GetNodalFunctionType() const override
  {
    if (!m_nodalFunc)
      return InterpIdw::CONSTANT;
    return static_cast<InterpIdw::NodalFuncEnum>(m_nodalFunc->GetType());
  }
  /// \brief get the nodal function number of nearest points
  /// \return the nodal function number of nearest points
  virtual int GetNodalFunctionNumNearestPts() const override
  {
    if (!m_nodalFunc)
      return 0;
    return m_nodalFunc->GetNumNearestPoints();
  }
  /// \brief get the nodal function option to use a quadrant search
  /// \return the nodal function option to use a quadrant search
  virtual bool GetNodalFunctionUseQuadrantSearch() const override
  {
    if (!m_nodalFunc)
      return false;
    return m_nodalFunc->GetUseQuadrantSearch();
  }


  /// \brief sets a flag to save the weights computed by the interpolation
  /// \param[in] a_ flag: true will save weights and false will not
  virtual void SetSaveWeights(bool a_) override { m_saveWeights = a_; }
  virtual void InterpWeights(const Pt3d& a_pt,
                             VecInt& a_idx,
                             VecDbl& a_wt) const override;

  /// \brief sets a flag to use (or not) multi-threading when interpolating
  /// \param[in] a_ flag: true will use multi-threading and false will not.
  /// The default setting for the class is to use multi-threading.
  virtual void SetMultiThreading(bool a_) override { m_multiThread = a_; }

  virtual std::string ToString() const override;

  void SetPower(double a_power) override;
  void RecalcNodalFunc();
  void SetSearchOpts(int a_nNearestPoints, bool a_quad_oct_Search) override;
  float InterpToPt(const Pt3d& a_pt, int a_idx) const;
  /// \brief Set the observer class so that feedback on the interpolation
  /// process can be received.
  /// \param a_prog: The observer.
  void SetObserver(BSHP<Observer> a_prog) override { m_prog = a_prog; }

  void ValFromWeights(VecDbl& a_w,
                      VecInt& a_nPts,
                      float& a_val,
                      int a_ptIdx,
                      const Pt3d& a_pt) const;
  void SetWeightCalcMethod(InterpIdw::WeightEnum a_) override;
  void SetNodalFunction(NodalFuncEnum a_,
                        int a_nNearest,
                        bool a_quad_oct,
                        BSHP<Observer> a_p) override;
  void CreateNodalFunctionPtr(NodalFuncEnum a_,
                              int a_nNearest,
                              bool a_quad_oct,
                              BSHP<Observer> a_p);
  double ScalarFromNodalFunc(int a_ptIdx, const Pt3d& a_loc) const;

  bool m_2d; ///< flag for computing distances in xy instead of xyz
  /// flag for performing quadrant(2d) or octant (3d) searching for nearest ps
  bool m_quadOctSearch;
  /// flag for calculating weights using a the modified shepard's approach
  bool m_modifiedShepardWeights;
  int m_nNearestPts;             ///< number of nearest points to use in the interpolation
  /// The exponent to be used with the inverse of the distance. By default this
  /// is 2 so we use 1/d^2. The exponent can be changed so that the calculation
  /// is 1/d or 1/d^3...
  double m_power;
  BSHP<GmPtSearch> m_ptSearch; ///< Class used to find nearest points
  BSHP<VecPt3d> m_pts;         ///< Points used to interpolate
  BSHP<VecInt> m_tris;         ///< triangles ignored by idw
  BSHP<VecFlt> m_scalarFrom;   ///< Scalars at the points used to interpolate
  BSHP<NodalFunc> m_nodalFunc; ///< Nodal function (constant, gradient plane, quadratic)
  BSHP<Observer> m_prog;       ///< Observer that reports status of interpolation process
  bool m_saveWeights;          ///< flag for saving the interpolation weights
  VecInt2d m_ptIdx;            ///< pt indexes for saving weights
  VecDbl2d m_weights;          ///< calculated weights for saving weights
  bool m_trunc;                ///< flag to indicate if truncation is on
  double m_truncMax;      ///< Maximum truncation value. All interpolated values will be <=
  double m_truncMin;      ///< Minimum truncation value. All interpolated values will be >=
  bool m_multiThread;     ///< flag to indicate if multithreading should be used when interpolating
  std::string m_idString; ///< identification for comparison with other Interp classes
};

//----- Internal functions -----------------------------------------------------
namespace
{
//------------------------------------------------------------------------------
/// \brief Static variable to store point indices
//------------------------------------------------------------------------------
static VecInt2d& iPtIdx()
{
  static VecInt2d m_;
  return m_;
}
//------------------------------------------------------------------------------
/// \brief Static variable to store point weights
//------------------------------------------------------------------------------
static VecDbl2d& iWeights()
{
  static VecDbl2d m_weights;
  return m_weights;
}
//------------------------------------------------------------------------------
/// \brief Saves the weight associated with a_pt that is used to interpolate
/// to a particular location indicated by a_ptIdx. Only used with GMS pilot
/// point stuff.
//------------------------------------------------------------------------------
static void iSaveWeight(int a_ptIdx, int a_pt, double a_val)
{
  XM_ENSURE_TRUE_VOID_NO_ASSERT(a_ptIdx > XM_NONE);
  iPtIdx()[a_ptIdx].push_back(a_pt);
  iWeights()[a_ptIdx].push_back(a_val);
} // iSaveWeight
//------------------------------------------------------------------------------
/// \brief calls signals with information about the points and weights that
/// are used to interpolate to a particular location. This gets used by
/// pilot point stuff in GMS. Not really used any other time.
//------------------------------------------------------------------------------
static void iCallSignals(VecInt2d& a_ptIdx,
                         VecDbl2d& a_weights)
{
  for (size_t i = 0; i < a_ptIdx.size(); ++i)
  {
    inSetDataIndex((int)i);
    for (size_t j = 0; j < a_ptIdx[i].size(); ++j)
    {
      inFillWtArray(a_ptIdx[i][j] + 1, a_weights[i][j]);
    }
  }
  a_ptIdx.clear();
  a_weights.clear();
} // iCallSignals
} // unnamed namespace
//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class InterpIdw
/// \see InterpIdwImpl
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Creates an InterpIdw class
/// \return Shared pointer to InterpIdw.
//------------------------------------------------------------------------------
BSHP<InterpIdw> InterpIdw::New()
{
  BSHP<InterpIdw> ptr(new InterpIdwImpl());
  return ptr;
} // InterpIdw::New
//------------------------------------------------------------------------------
/// \brief Constructor
//------------------------------------------------------------------------------
InterpIdw::InterpIdw()
{
}
//------------------------------------------------------------------------------
/// \brief Destructor
//------------------------------------------------------------------------------
InterpIdw::~InterpIdw()
{
}

////////////////////////////////////////////////////////////////////////////////
/// \class InterpIdwImpl
/// \brief Inverse distance weighted interpolation class.
/// \image html Doxygen/html/images/idw_const.png
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Creates an InterpIdw class
//------------------------------------------------------------------------------
InterpIdwImpl::InterpIdwImpl()
: m_2d(true)
, m_quadOctSearch(false)
, m_modifiedShepardWeights(true)
, m_nNearestPts(16)
, m_power(2)
, m_ptSearch()
, m_pts(new VecPt3d())
, m_tris(new VecInt())
, m_scalarFrom(new VecFlt())
, m_nodalFunc()
, m_prog()
, m_saveWeights(false)
, m_trunc(false)
, m_truncMax(0.0)
, m_truncMin(0.0)
, m_multiThread(true)
{
} // InterpIdwImpl::InterpIdwImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
InterpIdwImpl::~InterpIdwImpl()
{
} // InterpIdwImpl::~InterpIdwImpl
//------------------------------------------------------------------------------
/// \brief Sets the points that will be used to do the interpolation.
/// \param a_pts array of the point locations
/// \param a_tris triangles
//------------------------------------------------------------------------------
void InterpIdwImpl::SetPtsTris(BSHP<VecPt3d> a_pts, BSHP<VecInt> a_tris)
{
  SetPts(a_pts, true);
  m_tris = a_tris;
} // InterpIdwImpl::SetPtsTris
//------------------------------------------------------------------------------
/// \brief Sets the points that will be used to do the interpolation.
/// \param a_pts array of the point locations
/// \param a_2d  indicates if the class will do 2D or 3D interpolation
//------------------------------------------------------------------------------
void InterpIdwImpl::SetPts(BSHP<VecPt3d> a_pts, bool a_2d)
{
  m_2d = a_2d;
  m_pts = a_pts;
  m_ptSearch = GmPtSearch::New(m_2d);
  m_ptSearch->PtsToSearch(m_pts);
  if (m_scalarFrom->empty())
  {
    m_scalarFrom->resize(0);
    m_scalarFrom->reserve(m_pts->size());
    VecFlt& s(*m_scalarFrom);
    VecPt3d& pts(*m_pts);
    for (Pt3d& p : pts)
      s.push_back((float)p.z);
  }
} // InterpIdwImpl::SetPts
//------------------------------------------------------------------------------
/// \brief Sets the scalar values that will be used to do the interpolation.
/// \param a_scalar array of scalar values
/// \param a_n      size of the a_scalar array
//------------------------------------------------------------------------------
void InterpIdwImpl::SetScalars(const float* a_scalar, size_t a_n)
{
  XM_ENSURE_TRUE(m_scalarFrom->size() == a_n);
  for (size_t i = 0; i < a_n; ++i)
    (*m_scalarFrom)[i] = a_scalar[i];
  RecalcNodalFunc();
} // InterpIdwImpl::SetScalars
//------------------------------------------------------------------------------
/// \brief Sets the scalar values that will be used to do the interpolation.
/// \param a_scalar array of scalar values
//------------------------------------------------------------------------------
void InterpIdwImpl::SetScalars(BSHP<VecFlt> a_scalar)
{
  XM_ENSURE_TRUE(a_scalar);
  SetScalars(&(*a_scalar)[0], a_scalar->size());
} // InterpIdwImpl::SetScalars
//------------------------------------------------------------------------------
/// \brief Sets the exponent for the interpolation. By default the class does
/// inverse distance squared weighting but the exponent can be changed to any
/// value.
/// \param a_power the exponent used to compute the point weights
/// 1 / distance^a_power
//------------------------------------------------------------------------------
void InterpIdwImpl::SetPower(double a_power)
{
  m_power = a_power;
  XM_ENSURE_TRUE(!m_nodalFunc); // Not handled: SetPower after creating nodal func
} // InterpIdwImpl::SetPower
//------------------------------------------------------------------------------
/// \brief Recalculates the nodal function. This happens when the scalars change
//------------------------------------------------------------------------------
void InterpIdwImpl::RecalcNodalFunc()
{
  if (m_nodalFunc)
  {
    m_nodalFunc->ComputeNodalFuncs();
  }
} // InterpIdwImpl::RecalcNodalFunc
//------------------------------------------------------------------------------
/// \brief Sets the search options for how to find the nearest points to the
/// interpolation point. The number of nearest points can be specified as well
/// as whether to find the nearest points in each quadrant or octant.
/// \param a_nNearestPoints the number of nearest points to the interpolation
/// point. These points are used to do the interpolation.
/// \param a_quad_oct_Search specifies if the search criterion should find the
/// nearest points in each quadrant (2d) or octant (3d)
//------------------------------------------------------------------------------
void InterpIdwImpl::SetSearchOpts(int a_nNearestPoints, bool a_quad_oct_Search)
{
  m_nNearestPts = a_nNearestPoints;
  m_quadOctSearch = a_quad_oct_Search;
} // InterpIdwImpl::SetPower
//------------------------------------------------------------------------------
/// \brief Interpolates to the location specified by a_pt and returns the value.
/// \param a_pt The location of the interpolation point.
/// \return The interpolated value.
//------------------------------------------------------------------------------
float InterpIdwImpl::InterpToPt(const Pt3d& a_pt)
{
  return InterpToPt(a_pt, XM_NONE);
} // InterpIdwImpl::InterpToPt
//------------------------------------------------------------------------------
/// \brief overload to interpolate to the location specified by a_pt. The a_idx
/// variable is used if the interpolation weights are being saved for later
/// processing by the calling program.
/// \param a_pt The location of the interpolation point.
/// \param a_idx Index related to the location a_pt used to later process
/// interpolation weights related to a_pt.
/// \return The interpolated value.
//------------------------------------------------------------------------------
float InterpIdwImpl::InterpToPt(const Pt3d& a_pt, int a_idx) const
{
  float rval(0);
  XM_ENSURE_FALSE(m_scalarFrom->empty(), rval);
  VecInt nPts;
  VecDbl w;
  InterpWeights(a_pt, nPts, w);
  if (1 == nPts.size())
  {
    rval = (*m_scalarFrom)[nPts[0]];
    if (m_saveWeights)
      iSaveWeight(a_idx, nPts[0], 1.0);
    return rval;
  }
  ValFromWeights(w, nPts, rval, a_idx, a_pt);
  if (m_trunc)
  {
    if (rval < m_truncMin)
      rval = (float)m_truncMin;
    if (rval > m_truncMax)
      rval = (float)m_truncMax;
  }
  return rval;
} // InterpIdwImpl::InterpToPt
//------------------------------------------------------------------------------
/// \brief Interpolates to an array of points and fills in an array of scalars.
/// This method will run in parallel using multiple threads.
/// \param a_pts     Array of points to interpolate to.
/// \param a_scalars Array of scalar values that is filled in by this method.
/// a_scalars will be the same size as a_pts and each value corresponds to the
/// interpolated value at the respective location in the a_pts array.
//------------------------------------------------------------------------------
void InterpIdwImpl::InterpToPts(const VecPt3d& a_pts, VecFlt& a_scalars)
{
  a_scalars.assign(a_pts.size(), 0);
  if (m_saveWeights)
  {
    iPtIdx().assign(a_scalars.size(), VecInt());
    iWeights().assign(a_scalars.size(), VecDbl());
  }

  if (!m_multiThread)
  {
    for (size_t i = 0; i < a_pts.size(); ++i)
    {
      a_scalars[i] = InterpToPt(a_pts[i], (int)i);
    }
  }
  else
  {
    BSHP<ThreadMgr> threadMgr = ThreadMgr::New();
    threadMgr->SetThreadLoopClass(InterpThread(*this, a_scalars, a_pts).CreateForNewThread());
    threadMgr->SetObserver(m_prog);
    if (a_pts.size() < 10000)
      threadMgr->ExplicitlySetNumThreads(2);
    threadMgr->RunThreads((int)a_scalars.size());
  }

  if (m_saveWeights)
    iCallSignals(iPtIdx(), iWeights());
} // InterpIdwImpl::InterpToPts
//------------------------------------------------------------------------------
/// \brief Sets the activity on the point being used to interpolate
/// \param a_activity Bitset of point activity.
//------------------------------------------------------------------------------
void InterpIdwImpl::SetPtActivity(DynBitset& a_activity)
{
  m_ptSearch->SetActivity(a_activity);
} // InterpIdwImpl::SetPtActivity
//------------------------------------------------------------------------------
/// \brief Returns shared pointer to points vector.
/// \return Shared pointer to points vector.
//------------------------------------------------------------------------------
const BSHP<VecPt3d> InterpIdwImpl::GetPts() const
{
  return m_pts;
} // InterpIdwImpl::GetPts
//------------------------------------------------------------------------------
/// \brief Returns shared pointer to triangles vector.
/// \return Shared pointer to triangles vector.
//------------------------------------------------------------------------------
const BSHP<VecInt> InterpIdwImpl::GetTris() const
{
  return m_tris;
} // InterpIdwImpl::GetTris
//------------------------------------------------------------------------------
/// \brief Returns shared pointer to scalars vector.
/// \return Shared pointer to scalars vector.
//------------------------------------------------------------------------------
const BSHP<VecFlt> InterpIdwImpl::GetScalars() const
{
  return m_scalarFrom;
} // InterpIdwImpl::GetScalars
//------------------------------------------------------------------------------
/// \brief Returns bitset of point activity
/// \return Returns bitset of point activity
//------------------------------------------------------------------------------
DynBitset InterpIdwImpl::GetPtActivity() const
{
  if (!m_ptSearch)
    return DynBitset();
  return m_ptSearch->GetActivity();
} // InterpIdwImpl::GetPtActivity
//------------------------------------------------------------------------------
/// \brief Returns empty bitset. No triangles used for idw
/// \return Returns empty bitset
//------------------------------------------------------------------------------
DynBitset InterpIdwImpl::GetTriActivity() const
{
  return DynBitset();
} // InterpIdwImpl::GetTriActivity
//------------------------------------------------------------------------------
/// \brief Set the truncation values for the interpolation and turn on
/// truncation.
/// \param a_sMax The maximum value for truncation.
/// \param a_sMin The minimum value for truncation.
//------------------------------------------------------------------------------
void InterpIdwImpl::SetTrunc(double a_sMax, double a_sMin)
{
  m_trunc = true;
  m_truncMax = a_sMax;
  m_truncMin = a_sMin;
} // InterpIdwImpl::SetTrunc
//------------------------------------------------------------------------------
/// \brief Given a location and an array of points the weights associated with
/// array of points are calculated.
/// \param a_pt    Location of the interpolation point
/// \param a_nPts  Vector of indices indicating the location in the m_pts
/// vector of the nearest points to a_pt.
/// \param a_w     Vector weights associated with the nearest points to a_pt.
//------------------------------------------------------------------------------
void InterpIdwImpl::InterpWeights(const Pt3d& a_pt,
                                  VecInt& a_nPts,
                                  VecDbl& a_w) const
{
  VecDbl d2;
  // first see if we are at this location
  m_ptSearch->NearestPtsToPt(a_pt, 1, false, a_nPts);
  // get the distance between our location and the nearest point
  inDistanceSquared(a_pt, a_nPts, *m_pts, m_2d, d2);
  if (m_nNearestPts == 1 || d2[0] < gmXyTol())
  {
    a_w.resize(1);
    a_w[0] = 1;
    return;
  }
  // see if we are doing all points
  if (m_nNearestPts < 0)
  {
    a_nPts.assign(m_pts->size(), 0);
    for (size_t i = 0; i < m_pts->size(); ++i)
      a_nPts[i] = (int)i;
  }
  else
  {
    m_ptSearch->NearestPtsToPt(a_pt, m_nNearestPts, m_quadOctSearch, a_nPts);
  }
  // compute weights for each point
  inDistanceSquared(a_pt, a_nPts, *m_pts, m_2d, d2);
  inIdwWeights(d2, m_power, m_modifiedShepardWeights, a_w);
} // InterpIdwImpl::InterpWeights
//------------------------------------------------------------------------------
/// \brief Write the internals to a string.
/// \return The string.
//------------------------------------------------------------------------------
std::string InterpIdwImpl::ToString() const
{
  std::stringstream ss;
  ss << m_2d << "=2d " << m_quadOctSearch << "=quadOctSearch " << m_modifiedShepardWeights
     << "=modifiedShepardWeights " << m_nNearestPts << "=nNearestPts " << m_power << "=power "
     << m_saveWeights << "=saveWeights " << m_multiThread << "=multiThread "
     << "\n";

  if (m_ptSearch)
    ss << m_ptSearch->ToString() << "=ptSearch "
       << "\n";

  if (m_pts)
    VecToStream(ss, *m_pts, "pts");
  if (m_tris)
    VecToStream(ss, *m_tris, "tris");
  if (m_scalarFrom)
    VecToStream(ss, *m_scalarFrom, "scalarFrom");

  if (m_nodalFunc)
    ss << m_nodalFunc->ToString() << "=nodalFunc "
       << "\n";

  for (size_t i = 0; i < m_ptIdx.size(); ++i)
    VecToStream(ss, m_ptIdx[i], "ptIdx");
  ss << "=ptIdx "
     << "\n";

  for (size_t i = 0; i < m_weights.size(); ++i)
    VecToStream(ss, m_weights[i], "weights");
  ss << "=weights "
     << "\n";

  return ss.str();
} // InterpIdwImpl::ToString
//------------------------------------------------------------------------------
/// \brief Given a vector of weights, the interpolated value a_val is computed
/// at the point a_pt.
/// \param a_w Vector of weights associated with locations indicated in the
/// a_nPts vector.
/// \param a_nPts Vector of indices into the m_pts vector for the nearest
/// points to a_pt.
/// \param a_val Interpolated value calculated in this method.
/// \param a_ptIdx Index associated with a_pt that is used to later process
/// interpolation weights.
/// \param a_pt The location of the interpolation point.
//------------------------------------------------------------------------------
void InterpIdwImpl::ValFromWeights(VecDbl& a_w,
                                   VecInt& a_nPts,
                                   float& a_val,
                                   int a_ptIdx,
                                   const Pt3d& a_pt) const
{
  a_val = 0;
  double val(0);
  for (size_t i = 0; i < a_w.size(); ++i)
  {
    int j = a_nPts[i];
    // val += a_w[i] * m_scalarFrom[j];
    val += a_w[i] * ScalarFromNodalFunc(j, a_pt);
    if (m_saveWeights)
      iSaveWeight(a_ptIdx, j, a_w[i]);
  }
  a_val = (float)val;
} // InterpIdwImpl::ValFromWeights
//------------------------------------------------------------------------------
/// \brief Sets the method for calculating the weights. The classic just uses
/// 1/distance^exponent. The modified method uses another formulation based
/// on the distance of the furtherest location from the interpolation pt.
/// \param a_ The weight calculation method.
//------------------------------------------------------------------------------
void InterpIdwImpl::SetWeightCalcMethod(InterpIdw::WeightEnum a_)
{
  if (a_ == InterpIdw::CLASSIC)
    m_modifiedShepardWeights = false;
  else if (a_ == InterpIdw::MODIFIED)
    m_modifiedShepardWeights = true;
  else
    m_modifiedShepardWeights = true;
} // InterpIdwImpl::SetWeightCalcMethod
//------------------------------------------------------------------------------
/// \brief Sets the type of nodal function as well as options for computing
/// nodal functions.
/// \param a_ The nodal function methodology: constant (0), gradient plane (1),
/// quadratic (2).
/// \param a_nNearest The nearest number of points to use when calculating the
/// nodal functions.
/// \param a_quad_oct Find the nearest number of points in each quadrant (2d)
/// or octant (3d) when computing nodal functions.
/// \param a_p Progress bar to give user feedback.
//------------------------------------------------------------------------------
void InterpIdwImpl::SetNodalFunction(NodalFuncEnum a_,
                                     int a_nNearest,
                                     bool a_quad_oct,
                                     BSHP<Observer> a_p)
{
  XM_ENSURE_TRUE_VOID_NO_ASSERT(a_ != CONSTANT);
  XM_ENSURE_TRUE_VOID_NO_ASSERT(!inAllScalarsEqual(*m_scalarFrom, m_ptSearch->GetActivity()));
  XM_ENSURE_TRUE(!m_pts->empty());

  CreateNodalFunctionPtr(a_, a_nNearest, a_quad_oct, a_p);
} // InterpIdwImpl::SetNodalFunction
//------------------------------------------------------------------------------
/// \brief Creates the Nodal function class
/// \param a_ Type of nodal function: gradient plane, quadratic
/// \param a_nNearest Number of nearest points to use when calculating nodal
/// functions.
/// \param a_quad_oct flag to turn on/off quadrant or octant searching for
/// nearest number of points.
/// \param a_p Observer for reporting progress of nodal function calculations.
//------------------------------------------------------------------------------
void InterpIdwImpl::CreateNodalFunctionPtr(NodalFuncEnum a_,
                                           int a_nNearest,
                                           bool a_quad_oct,
                                           BSHP<Observer> a_p)
{
  m_nodalFunc = NodalFunc::New((int)a_, m_2d, m_ptSearch, *m_pts, *m_scalarFrom, a_nNearest,
                               a_quad_oct, m_power, m_modifiedShepardWeights, a_p, nullptr);
} // InterpIdwImpl::CreateNodalFunctionPtr
//------------------------------------------------------------------------------
/// \brief Returns a scalar value based on the nodal function. When nodal
/// functions are used then the scalar value used at each point depends on the
/// location of the you are interpolating to.
/// \param a_ptIdx The index of one of the points being used to interpolate
/// (index into m_pts).
/// \param a_loc The location of the point we are interpolating to.
/// \return Scalar value at a_ptIdx adjusted by the nodal function.
//------------------------------------------------------------------------------
double InterpIdwImpl::ScalarFromNodalFunc(int a_ptIdx, const Pt3d& a_loc) const
{
  double rval((double)(*m_scalarFrom)[a_ptIdx]);
  if (m_nodalFunc)
  {
    rval = m_nodalFunc->ScalarFromPtIdx(a_ptIdx, a_loc);
  }
  return rval;
} // InterpIdwImpl::ScalarFromNodalFunc

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/interpolate/InterpIdw.t.h>

#include <xmscore/testing/TestTools.h>
#include <xmscore/misc/XmLog.h>

// namespace xms {
using namespace xms;
//------------------------------------------------------------------------------
/// \brief Get points for testing.
/// \param a_pts: Points.
/// \param a_scalar: Scalars.
/// \param a_iPts: Interpolation points.
//------------------------------------------------------------------------------
static void iGetPoints(VecPt3d& a_pts,
                       VecFlt& a_scalar,
                       VecPt3d& a_iPts)
{
  a_pts = {{-75.0, -16.0, 0.0}, {-60.0, 32.0, 0.0},   {-34.0, 50.0, 0.0},   {-34.0, 27.0, 0.0},
           {-8.0, 40.0, 0.0},   {16.0, 38.0, 0.0},    {-25.0, 14.0, 43.64}, {10.0, 18.0, 44.16},
           {27.0, 26.0, 0.0},   {63.0, 35.0, 0.0},    {-32.0, 0.0, 59.04},  {-7.0, 7.0, 90.2},
           {26.0, 6.0, 67.2},   {75.0, 7.0, 0.0},     {-37.0, -15.0, 9.24}, {-7.0, -13.0, 71.0},
           {2.0, -3.0, 98.4},   {31.0, -15.0, 25.56}, {60.0, -13.0, 0.0},   {-50.0, -30.0, 0.0},
           {-30.0, -28.0, 0.0}, {43.0, -22.0, 0.0},   {-32.0, -50.0, 0.0},  {27.0, -37.0, 0.0},
           {60.0, -33.0, 0.0}};
  a_scalar.resize(a_pts.size());
  for (size_t i = 0; i < a_pts.size(); ++i)
    a_scalar[i] = (float)a_pts[i].z;

  a_iPts = {{-90, 60, 0}, {90, 60, 0}, {-90, -60, 0}, {90, -60, 0}, {60, -33, 0}};
} // iGetPoints

////////////////////////////////////////////////////////////////////////////////
/// \class InterpIdwUnitTests
/// \brief tester for the InterpIdw class
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief Creates a class
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testCreateClass()
{
  BSHP<InterpIdw> pts = InterpIdw::New();
  TS_ASSERT(pts);
}
//------------------------------------------------------------------------------
/// \brief tests interpolating to 1 location from 1 location
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testInterpToPts()
{
  BSHP<InterpIdw> idw = InterpIdw::New();
  BSHP<VecPt3d> pts(new VecPt3d());
  pts->push_back(Pt3d());
  VecFlt scalar(1, 2);
  idw->SetPts(pts, true);
  idw->SetScalars(&scalar[0], scalar.size());

  pts->resize(0);
  pts->push_back(Pt3d(1, 1, 1));
  scalar.resize(0);
  idw->SetMultiThreading(false);
  idw->InterpToPts(*pts, scalar);
  TS_ASSERT_EQUALS(2.0f, scalar[0]);
} // InterpIdwUnitTests::testInterpToPts
//------------------------------------------------------------------------------
/// \brief tests 2d interpolation
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testInterp2d_a()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  VecPt3d ipts;
  BSHP<VecFlt> scalar(new VecFlt());
  VecFlt s, vBase;
  iGetPoints(*pts, *scalar, ipts);

  BSHP<InterpIdw> idw = InterpIdw::New();
  idw->SetPts(pts, true);
  idw->SetScalars(scalar);
  idw->SetMultiThreading(false);
  idw->InterpToPts(ipts, s);
  vBase = {4.5606198f, 2.9842966f, 4.8022637f, 2.8352659f, 0.0f};
  TS_ASSERT_DELTA_VEC(vBase, s, 1e-7);
} // InterpIdwUnitTests::testInterp2d_a
//------------------------------------------------------------------------------
/// \brief tests 2d interpolation using all idw points
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testInterp2d_b()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  VecPt3d ipts;
  VecFlt scalar, s(5), vBase;
  iGetPoints(*pts, scalar, ipts);
  BSHP<InterpIdw> idw = InterpIdw::New();
  idw->SetPts(pts, true);
  idw->SetScalars(&scalar[0], scalar.size());
  idw->SetSearchOpts(-1, false);
  for (int i = 0; i < 5; ++i)
    s[i] = idw->InterpToPt(ipts[i]);
  vBase = {10.172515f, 7.9736009f, 9.5391436f, 7.2670202f, 0.0f};
  TS_ASSERT_DELTA_VEC(vBase, s, 1e-7);
} // InterpIdwUnitTests::testInterp2d_b
//------------------------------------------------------------------------------
/// \brief tests 2d interpolation using nearest 5 points
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testInterp2d_c()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  VecPt3d ipts;
  VecFlt scalar;
  iGetPoints(*pts, scalar, ipts);
  BSHP<InterpIdw> idw = InterpIdw::New();
  idw->SetPts(pts, true);
  idw->SetScalars(&scalar[0], scalar.size());
  idw->SetSearchOpts(5, false);
  VecInt idx;
  VecDbl wt, wtBase;
  Pt3d myPt;
  idw->InterpWeights(myPt, idx, wt);
  wtBase = {0.0019633969662346436, 0.064476394365326525, 0.0, 0.014616102731534851,
            0.91894410593690401};
  TS_ASSERT_DELTA_VEC(wtBase, wt, 1e-7);
} // InterpIdwUnitTests::testInterp2d_c
//------------------------------------------------------------------------------
/// \brief tests 2d interpolation changing the exponent to 3 and nearest 6 pts
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testInterp2d_d()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  VecPt3d ipts;
  VecFlt scalar, s(5), vBase;
  iGetPoints(*pts, scalar, ipts);
  BSHP<InterpIdw> idw = InterpIdw::New();
  idw->SetPts(pts, true);
  idw->SetScalars(&scalar[0], scalar.size());
  idw->SetSearchOpts(6, true);
  idw->SetPower(3);
  idw->SetNodalFunction(InterpIdw::GRAD_PLANE, 6, true, NULL);
  for (int i = 0; i < 5; ++i)
    s[i] = idw->InterpToPt(ipts[i]);
  vBase = {6.19763422f, 0.60451090f, -0.801453113f, 13.252287f, 0.0f};
  TS_ASSERT_DELTA_VEC(vBase, s, 1e-7);
}
//------------------------------------------------------------------------------
/// \brief tests 2d interpolation using classic weighting, exponent set to 1
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testInterp2d_e()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  VecPt3d ipts;
  VecFlt scalar, s(5), vBase;
  iGetPoints(*pts, scalar, ipts);
  BSHP<InterpIdw> idw = InterpIdw::New();
  idw->SetPts(pts, true);
  idw->SetScalars(&scalar[0], scalar.size());
  idw->SetWeightCalcMethod(InterpIdw::CLASSIC);
  for (int i = 0; i < 5; ++i)
    s[i] = idw->InterpToPt(ipts[i]);
  vBase = {16.884138f, 15.516511f, 16.895596f, 14.160349f, 0.0f};
  TS_ASSERT_DELTA_VEC(vBase, s, 1e-7);
  idw->SetPower(1);
  for (int i = 0; i < 5; ++i)
    s[i] = idw->InterpToPt(ipts[i]);
  vBase = {21.907907f, 21.933628f, 22.670679f, 19.839111f, 0.0f};
  TS_ASSERT_DELTA_VEC(vBase, s, 1e-7);
} // InterpIdwUnitTests::testInterp2d_e
//------------------------------------------------------------------------------
/// \brief test 2d interpolation with a quadratic nodal function
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testInterp2d_f()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  VecPt3d ipts;
  VecFlt scalar, s(5), vBase;
  iGetPoints(*pts, scalar, ipts);
  BSHP<InterpIdw> idw = InterpIdw::New();
  idw->SetPts(pts, true);
  idw->SetScalars(&scalar[0], scalar.size());
  idw->SetNodalFunction(InterpIdw::QUADRATIC, XM_NONE, true, NULL);
  for (int i = 0; i < 5; ++i)
    s[i] = idw->InterpToPt(ipts[i]);
  vBase = {-46.129936f, -65.856499f, -71.997749f, -47.643417f, 0.0f};
  TS_ASSERT_DELTA_VEC(vBase, s, 1e-7);
} // InterpIdwUnitTests::testInterp2d_f
//------------------------------------------------------------------------------
/// \brief test 3d interpolation
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testInterp3d()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  VecPt3d ipts;
  VecFlt scalar, s(5), vBase;
  iGetPoints(*pts, scalar, ipts);
  BSHP<InterpIdw> idw = InterpIdw::New();
  idw->SetPts(pts, false);
  idw->SetScalars(&scalar[0], scalar.size());
  idw->SetNodalFunction(InterpIdw::GRAD_PLANE, XM_NONE, true, NULL);
  for (int i = 0; i < 5; ++i)
    s[i] = idw->InterpToPt(ipts[i]);
  vBase = {1.5025957e-008f, -7.7562149e-008f, 1.4442284e-007f, -6.1540938e-008f, 0.0f};
  TS_ASSERT_DELTA_VEC(vBase, s, 1e-7);
  idw->SetNodalFunction(InterpIdw::QUADRATIC, XM_NONE, true, NULL);
  for (int i = 0; i < 5; ++i)
    s[i] = idw->InterpToPt(ipts[i]);
  vBase = {5.2535050e-008f, -1.2138121e-007f, 4.1972626e-007f, -1.1271275e-007f, 0.0f};
  TS_ASSERT_DELTA_VEC(vBase, s, 1e-7);
} // InterpIdwUnitTests::testInterp2d_f
//------------------------------------------------------------------------------
/// \brief test error reporting. Errors can be generated by the nodal function
/// setup process.
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testErrors()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  VecPt3d ipts;
  VecFlt scalar, s(5), vBase;
  *pts = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}};
  scalar.assign(pts->size(), 0);
  ipts.assign(5, Pt3d(-1, 0, 0));

  XmLog::Instance().GetAndClearStackStr();
  InterpIdwImpl idw;
  idw.SetPts(pts, false);
  idw.SetScalars(&scalar[0], scalar.size());
  idw.SetNodalFunction(InterpIdw::QUADRATIC, 1, true, NULL);
  // no nodal function created on previous line because all scalars are equal

  idw.CreateNodalFunctionPtr(InterpIdw::QUADRATIC, 1, true, NULL);
  TS_ASSERT_EQUALS(true, (XmLog::Instance().ErrCount() != 0));
  XmLog::Instance().GetAndClearStackStr();
} // InterpIdwUnitTests::testErrors
//------------------------------------------------------------------------------
/// \brief test error reporting.
//------------------------------------------------------------------------------
void InterpIdwUnitTests::testErrors2()
{
  BSHP<VecPt3d> pts(new VecPt3d());
  VecPt3d ipts;
  VecFlt scalar, s(5), vBase;
  *pts = {{0, 0, 0}, {1, 0, 0}, {2, 0, 0}, {3, 0, 0}};
  scalar.assign(pts->size(), 0);
  ipts.assign(5, Pt3d(-1, 0, 0));

  XmLog::Instance().GetAndClearStackStr();
  InterpIdwImpl idw;
  idw.SetPts(pts, true);
  idw.SetScalars(&scalar[0], scalar.size());
  idw.SetNodalFunction(InterpIdw::GRAD_PLANE, 2, true, NULL);
  // no nodal function created on previous line because all scalars are equal

  idw.CreateNodalFunctionPtr(InterpIdw::GRAD_PLANE, 2, true, NULL);
  for (int i = 0; i < 5; ++i)
    s[i] = idw.InterpToPt(ipts[i]);
  TS_ASSERT_EQUALS(true, (XmLog::Instance().ErrCount() != 0));
  XmLog::Instance().GetAndClearStackStr();
} // InterpIdwUnitTests::testErrors

//} // namespace xms
#endif // CXX_TEST
