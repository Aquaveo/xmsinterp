//------------------------------------------------------------------------------
/// \file
/// \brief Linear interpolation class as well as other triangle based
/// interpolation such as natural neighbor and clough-toucher.
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My header
#include <xmsinterp/interpolate/InterpLinear.h>

// 3. Standard Library Headers
#include <cfloat>
#include <sstream>

// 4. External Library Headers

// 5. Shared Headers
#include <xmscore/math/math.h>
#include <xmsinterp/geometry/GmPtSearch.h>
#include <xmsinterp/geometry/GmTriSearch.h>
#include <xmsinterp/interpolate/detail/InterpCt.h>
#include <xmsinterp/interpolate/detail/InterpNatNeigh.h>
#include <xmsinterp/interpolate/detail/NodalFunc.h>
#include <xmsinterp/interpolate/InterpUtil.h>
#include <xmscore/stl/utility.h>
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

////////////////////////////////////////////////////////////////////////////////
/// \brief Implementation of InterpLinear
class InterpLinearImpl : public InterpLinear
{
public:
  InterpLinearImpl();
  virtual ~InterpLinearImpl();

  virtual void SetPtsTris(BSHP<VecPt3d> a_, BSHP<VecInt> a_tris) override;

  virtual void SetScalars(const float* a_scalar, size_t a_n) override;
  virtual void SetScalars(BSHP<VecFlt> a_scalar) override;
  virtual void SetPtActivity(DynBitset& a_activity) override;
  // bitset is number of triangles in length not numtri*3 like the tris array
  virtual void SetTriActivity(DynBitset& a_activity) override;
  virtual const BSHP<VecPt3d> GetPts() const override;
  virtual const BSHP<VecInt> GetTris() const override;
  virtual const BSHP<VecFlt> GetScalars() const override;
  virtual DynBitset GetPtActivity() const override;
  virtual DynBitset GetTriActivity() const override;
  virtual VecInt GetExtrapolationPointIndexes() const override;

  virtual float InterpToPt(const Pt3d& a_pt) override;
          float InterpToPtImpl(const Pt3d& a_pt, int a_ptIdx);
  virtual void InterpToPts(const VecPt3d& a_pts, VecFlt& a_scalars) override;

  // find triangle
  virtual int TriContainingPt(const Pt3d& a_pt) override;
  virtual void TriEnvelopsContainingPt(const Pt3d& a_pt, VecInt& a_tris) override;
  // interpolation
  virtual bool InterpWeights(const Pt3d& a_pt, VecInt& a_idxs, VecDbl& a_wts) override;

  float CloughTocherInterp(VecInt& a_idxs, const Pt3d& a_pt);
  float NatNeighInterp(const Pt3d& a_pt);
  // interpolation options
  virtual void SetExtrapVal(double a_val) override;
  virtual void SetTrunc(double a_sMax, double a_sMin) override;
  virtual void SetUseCloughTocher(bool a_, BSHP<Observer> a_prog) override;
  virtual void SetUseNatNeigh(bool a_,
                              int a_nodalFunc,
                              int a_ndFuncOpt,
                              int a_ndFuncNumNearestPts,
                              bool a_blendWeights,
                              BSHP<Observer> a_prog) override;

  /// \brief get extrapolation value
  /// \return the extrapolation value
  virtual double GetExtrapVal() const override { return m_extrap; }
  /// \brief get the option to truncate interpolated values
  /// \return the option to truncate interpolated values
  virtual bool GetTruncateInterpolatedValues() const override { return m_trunc; }
  /// \brief get minimum truncation value
  /// \return the minimum truncation value
  virtual double GetTruncMin() const override { return m_truncMin; }
  /// \brief get the maximum truncation value
  /// \return the maximum truncation value
  virtual double GetTruncMax() const override { return m_truncMax; }
  /// \brief get the option for using Clough Tocher interpolation
  /// \return the option for using Clough Tocher interpolation
  virtual bool GetUseCloughTocher() const override { return m_cloughTocher; }
  /// \brief get the option for using Natural Neighbor interpolation
  /// \return the option for using Natural Neighbor interpolation
  virtual bool GetUseNatNeigh() const override { return m_natNeigh; }
  /// \brief get the value for the Natural Neighbor nodal function
  /// \return the value for the Natural Neighbor nodal function
  virtual int GetNatNeighNodalFunc() const override
  {
    if (!m_nodal)
      return 0;
    return m_nodal->GetType();
  }
  /// \brief get the option for the Natural Neighbor nodal function nearest points.
  /// Nearest points or nearest natural neighbors.
  /// \return the option for the Natural Neighbor nodal function nearest points.
  virtual int GetNatNeighNodalFuncNearestPtsOption() const override
  {
    if (!m_nodal)
      return 0;
    return m_nodal->GetNearestPointsOption();
  }
  /// \brief get the value for the number of nearest points to use when calculating
  /// the nodal function
  /// \return the value for the number of nearest points
  virtual int GetNatNeighNodalFuncNumNearestPts() const override
  {
    if (!m_nodal)
      return 0;
    return m_nodal->GetNumNearestPoints();
  }
  /// \brief get the option for blending weights when using Natural Neighbor
  /// \return the option for blending weights when using Natural Neighbor
  virtual bool GetNatNeighBlendWeights() const override
  {
    if (!m_nodal)
      return false;
    return m_nodal->GetUseModifiedShepardWeights();
  }


  BSHP<GmPtSearch> CreatePtSearch();
  void RecalcNodalFunc();
  virtual std::string ToString() const override;

protected:
  BSHP<GmTriSearch> m_triSearch; ///< spatial index for searching triangles
  BSHP<VecPt3d> m_pts;           ///< pt locations that make up triangles
  BSHP<VecInt> m_tris;           ///< triangles referencing indexes of pts
  BSHP<VecFlt> m_scalar;         ///< Scalars we're interpolating from
  double m_extrap;               ///< Extrapolation value
  bool m_trunc;                  ///< flag to indicate if truncation is on
  double m_truncMax;             ///< Maximum truncation value. All interpolated values will be <=
  double m_truncMin;             ///< Minimum truncation value. All interpolated values will be >=
  bool m_cloughTocher;           ///< flag indicating clough tocher interpolation will be used
  bool m_natNeigh;           ///< flag indicating that natural neighbor interpolation will be used
  BSHP<NodalFunc> m_nodal;   ///< Nodal function (constant, gradient plane, quadratic)
  BSHP<InterpCt> m_ct;       ///< Clough Tocher interpolation class
  BSHP<InterpNatNeigh> m_nn; ///< Natural neighbor interpolation class
  VecInt m_extrapolationPointIndexes; ///< indexes of points that are outside of all triangles
};

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

//------------------------------------------------------------------------------
/// \brief Creates an TriSearch class
/// \return Shared pointer to InterpLinear.
//------------------------------------------------------------------------------
BSHP<InterpLinear> InterpLinear::New()
{
  BSHP<InterpLinear> ptr(new InterpLinearImpl());
  return ptr;
} // InterpLinear::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
InterpLinear::InterpLinear()
{
} // InterpLinear::InterpLinear
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
InterpLinear::~InterpLinear()
{
} // InterpLinear::~InterpLinear

////////////////////////////////////////////////////////////////////////////////
/// \class InterpLinearImpl
/// \brief Linear interpolation from a set of triangles
/// Uses the boost RTree for queries.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
InterpLinearImpl::InterpLinearImpl()
: m_triSearch(GmTriSearch::New())
, m_pts(new VecPt3d())
, m_tris(new VecInt())
, m_scalar(new VecFlt())
, m_extrap(XM_NODATA)
, m_trunc(false)
, m_truncMax(0.0)
, m_truncMin(0.0)
, m_cloughTocher(false)
, m_natNeigh(false)
{
} // InterpLinearImpl::InterpLinearImpl
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
InterpLinearImpl::~InterpLinearImpl()
{
} // InterpLinearImpl::~InterpLinearImpl
//------------------------------------------------------------------------------
/// \brief Adds the triangles to the class
/// \param a_pts Array of point locations.
/// \param a_tris Array of triangles that references the a_pts array. This
/// array will have a size that is a multiple of 3. The first 3 locations in
/// array represent the first triangle and will have indices that correspond to
/// locations in the a_pts array.
//------------------------------------------------------------------------------
void InterpLinearImpl::SetPtsTris(BSHP<VecPt3d> a_pts, BSHP<VecInt> a_tris)
{
  m_pts = a_pts;
  m_tris = a_tris;
  m_scalar->assign(m_pts->size(), 0);
  for (size_t i = 0; i < m_pts->size(); ++i)
    (*m_scalar)[i] = (float)(*m_pts)[i].z;
  m_triSearch->TrisToSearch(a_pts, a_tris);
} // InterpLinearImpl::SetPtsTris
//------------------------------------------------------------------------------
/// \brief Set the scalars that will be used to interpolate from.
/// \param a_scalar Array of interpolation scalar values.
/// \param a_n Size of the array a_scalar.
//------------------------------------------------------------------------------
void InterpLinearImpl::SetScalars(const float* a_scalar, size_t a_n)
{
  XM_ENSURE_TRUE(a_n == m_pts->size());
  m_scalar->assign(&a_scalar[0], &a_scalar[a_n]);
  RecalcNodalFunc();
} // InterpLinearImpl::SetScalars
//------------------------------------------------------------------------------
/// \brief Set the scalars that will be used to interpolate from.
/// \param a_scalar: Array of interpolation scalar values.
//------------------------------------------------------------------------------
void InterpLinearImpl::SetScalars(BSHP<VecFlt> a_scalar)
{
  XM_ENSURE_TRUE(a_scalar->size() == m_pts->size());
  m_scalar = a_scalar;
  RecalcNodalFunc();
} // InterpLinear::SetScalars
//------------------------------------------------------------------------------
/// \brief Modifies the activity bitset of the class.
/// \param a_activity bitset of the activity of the points
//------------------------------------------------------------------------------
void InterpLinearImpl::SetPtActivity(DynBitset& a_activity)
{
  m_triSearch->SetPtActivity(a_activity);
  RecalcNodalFunc();
} // InterpLinearImpl::SetPtActivity
//------------------------------------------------------------------------------
/// \brief Modifies the activity bitset of the class.
/// \param a_activity bitset of the activity of the triangles
//------------------------------------------------------------------------------
void InterpLinearImpl::SetTriActivity(DynBitset& a_activity)
{
  m_triSearch->SetTriActivity(a_activity);
  RecalcNodalFunc();
} // InterpLinearImpl::SetTriActivity
//------------------------------------------------------------------------------
/// \brief Returns shared pointer to points vector.
/// \return Shared pointer to points vector.
//------------------------------------------------------------------------------
const BSHP<VecPt3d> InterpLinearImpl::GetPts() const
{
  return m_pts;
} // InterpLinearImpl::GetPts
//------------------------------------------------------------------------------
/// \brief Returns shared pointer to triangles vector.
/// \return Shared pointer to triangles vector.
//------------------------------------------------------------------------------
const BSHP<VecInt> InterpLinearImpl::GetTris() const
{
  return m_tris;
} // InterpLinearImpl::GetTris
//------------------------------------------------------------------------------
/// \brief Returns shared pointer to scalars vector.
/// \return Shared pointer to scalars vector.
//------------------------------------------------------------------------------
const BSHP<VecFlt> InterpLinearImpl::GetScalars() const
{
  return m_scalar;
} // InterpLinearImpl::GetScalars
//------------------------------------------------------------------------------
/// \brief Returns bitset of point activity
/// \return Returns bitset of point activity
//------------------------------------------------------------------------------
DynBitset InterpLinearImpl::GetPtActivity() const
{
  return m_triSearch->GetPtActivity();
} // InterpLinearImpl::GetPtActivity
//------------------------------------------------------------------------------
/// \brief Returns bitset of triangle activity
/// \return Returns bitset of triangle activity
//------------------------------------------------------------------------------
DynBitset InterpLinearImpl::GetTriActivity() const
{
  return m_triSearch->GetTriActivity();
} // InterpLinearImpl::GetTriActivity
//------------------------------------------------------------------------------
/// \brief Returns vector of point indexes for points that were outside of
/// all triangles
/// \return Returns vector of points indexes
//------------------------------------------------------------------------------
VecInt InterpLinearImpl::GetExtrapolationPointIndexes() const
{
  return m_extrapolationPointIndexes;
} // InterpLinearImpl::GetExtrapolationPointIndexes
//------------------------------------------------------------------------------
/// \brief Use the stored triangles to interpolate to a point. Returns
/// extrapolation value if the point is outside the triangles.
/// \param a_pt Location that is interpolated to.
/// \return Interpolated value at a_pt.
//------------------------------------------------------------------------------
float InterpLinearImpl::InterpToPt(const Pt3d& a_pt)
{
  return InterpToPtImpl(a_pt, 0);
} // InterpLinearImpl::InterpToPt
//------------------------------------------------------------------------------
/// \brief Use the stored triangles to interpolate to a point. Returns
/// extrapolation value if the point is outside the triangles.
/// \param a_pt Location that is interpolated to.
/// \param a_ptIdx point index
/// \return Interpolated value at a_pt.
//------------------------------------------------------------------------------
float InterpLinearImpl::InterpToPtImpl(const Pt3d& a_pt, int a_ptIdx)
{
  // return m_triSearch->InterpToPt(a_pt);
  float rval((float)m_extrap);
  VecInt idxs(3, 0);
  VecDbl wts(3, 0);
  if (m_triSearch->InterpWeights(a_pt, idxs, wts))
  {
    float* s = &(*m_scalar)[0];
    rval = (float)((s[idxs[0]] * wts[0]) + (s[idxs[1]] * wts[1]) + (s[idxs[2]] * wts[2]));
    if (m_cloughTocher || m_natNeigh)
    {
      if (EQ_TOL(wts[0], 1, FLT_EPSILON) || EQ_TOL(wts[1], 1, FLT_EPSILON) ||
          EQ_TOL(wts[2], 1, FLT_EPSILON))
      {
      }
      else
      {
        if (m_cloughTocher)
        {
          rval = CloughTocherInterp(idxs, a_pt);
        }
        else
          rval = NatNeighInterp(a_pt);
      }
    }
  }
  else
  {
    m_extrapolationPointIndexes.push_back(a_ptIdx);
  }
  if (m_trunc)
  {
    if (rval < m_truncMin)
      rval = (float)m_truncMin;
    if (rval > m_truncMax)
      rval = (float)m_truncMax;
  }
  return rval;
} // InterpLinearImpl::InterpToPt
//-----------------------------------------------------------------------------
/// \brief   Calls InterpToPt in a loop.
/// \param a_pts Locations of points.
/// \param a_scalars Interpolated scalar values at a_pts.
//-----------------------------------------------------------------------------
void InterpLinearImpl::InterpToPts(const VecPt3d& a_pts, VecFlt& a_scalars)
{
  // m_triSearch->InterpToPts(a_pts, a_scalars);
  m_extrapolationPointIndexes.clear();
  a_scalars.assign(a_pts.size(), 0);
  for (size_t i = 0; i < a_pts.size(); i++)
  {
    a_scalars[i] = InterpToPtImpl(a_pts[i], static_cast<int>(i));
  }
} // InterpLinearImpl::InterpToPts
//------------------------------------------------------------------------------
/// \brief Find the triangle containing the point
/// \param a_pt Location used to find a triangle.
/// \return Index of triangle containing a_pt. If XM_NONE is returned then no
/// triangle contained the point.
//------------------------------------------------------------------------------
int InterpLinearImpl::TriContainingPt(const Pt3d& a_pt)
{
  return m_triSearch->TriContainingPt(a_pt);
} // InterpLinearImpl::TriContainingPt
//------------------------------------------------------------------------------
/// \brief Find all triangles whose envelop contains the point.
/// \param a_pt Location used to find a triangle.
/// \param a_tris The indices to triangles whose envelop contains the point
//------------------------------------------------------------------------------
void InterpLinearImpl::TriEnvelopsContainingPt(const Pt3d& a_pt, VecInt& a_tris)
{
  m_triSearch->TriEnvelopsContainingPt(a_pt, a_tris);
} // InterpLinearImpl::TriContainingPt
//------------------------------------------------------------------------------
/// \brief Use the stored triangles to get interpolation weights for a point.
/// Returns false if the point is outside the triangles.
/// \param a_pt Location that is interpolated to.
/// \param a_idxs Vector of triangle point indices filled in by this method.
/// \param a_wts Vector of triangle point weights filled in by this method.
/// \return false if a_pt is not inside of any triangle
//------------------------------------------------------------------------------
bool InterpLinearImpl::InterpWeights(const Pt3d& a_pt, VecInt& a_idxs, VecDbl& a_wts)
{
  return m_triSearch->InterpWeights(a_pt, a_idxs, a_wts);
} // InterpLinearImpl::InterpWeights
//------------------------------------------------------------------------------
/// \brief uses Clough Tocher interpolation method to get the interpolated val
/// \param a_idxs Indices to the points that form the triangle that a_pt is
/// inside of.
/// \param a_pt Location that is interpolated to.
/// \return Interpolated value at a_pt.
//------------------------------------------------------------------------------
float InterpLinearImpl::CloughTocherInterp(VecInt& a_idxs, const Pt3d& a_pt)
{
  m_ct->ComputeCtCoeff(&a_idxs[0]);
  return (float)(m_ct->InterpToPt(a_pt));
} // InterpLinearImpl::CloughTocherInterp
//------------------------------------------------------------------------------
/// \brief uses Natural Neighbor interpolation method to get the interpolated
/// val
/// \param a_pt Location that is interpolated to.
/// \return Interpolated value at a_pt.
//------------------------------------------------------------------------------
float InterpLinearImpl::NatNeighInterp(const Pt3d& a_pt)
{
  return m_nn->InterpToPt(a_pt);
} // InterpLinearImpl::NatNeighInterp
//------------------------------------------------------------------------------
/// \brief Set the constant extrapolation value
/// \param a_val The value assigned to extrapolated points
//------------------------------------------------------------------------------
void InterpLinearImpl::SetExtrapVal(double a_val)
{
  m_extrap = a_val;
} // InterpLinearImpl::SetExtrapVal
//------------------------------------------------------------------------------
/// \brief Set the truncation values for the interpolation and turn on
/// truncation.
/// \param a_sMax The maximum value for truncation.
/// \param a_sMin The minimum value for truncation.
//------------------------------------------------------------------------------
void InterpLinearImpl::SetTrunc(double a_sMax, double a_sMin)
{
  m_trunc = true;
  m_truncMax = a_sMax;
  m_truncMin = a_sMin;
} // InterpLinearImpl::SetTrunc
//------------------------------------------------------------------------------
/// \brief Set the class to use the Clough Tocher interpolation method. This is
/// a legacy feature from GMS. Compare to linear.
/// \param a_ True/False to indicate if CT should be used.
/// \param a_prog Progress bar to give users feed back on the set up process
/// of CT. If you have a really large set of triangles this may take some time.
//------------------------------------------------------------------------------
void InterpLinearImpl::SetUseCloughTocher(bool a_, BSHP<Observer> a_prog)
{
  // m_triSearch->SetUseCloughTocher(a_, a_prog);
  m_cloughTocher = a_;
  if (!m_cloughTocher)
    return;

  BSHP<GmPtSearch> pts = CreatePtSearch();
  int nodalfunc(1), nearest(16);
  bool twod(true), quad_oct(false), modifiedShepardWeights(true);
  double dPower(2);
  m_nodal.reset();
  if (!inAllScalarsEqual(*m_scalar, m_triSearch->GetPtActivity()) && m_pts->size() > 6)
  {
    m_nodal = NodalFunc::New(nodalfunc, twod, pts, *m_pts, *m_scalar, nearest, quad_oct, dPower,
                             modifiedShepardWeights, a_prog, nullptr);
  }
  m_ct.reset(new InterpCt(*m_pts, m_nodal));
} // InterpLinearImpl::SetUseCloughTocher
//------------------------------------------------------------------------------
/// \brief Set the class to use natural neighbor (NN) interpolation.
/// \param a_ True/False to indicate if NN should be used.
/// \param a_nodalFunc Indicates which nodal function to use.
/// \param a_ndFuncOpt Indicates options for the nearest points when computing
/// the nodal functions.
/// \param a_ndFuncNumNearestPts The number of nearest points for nodal
/// function computation.
/// \param a_blendWeights Option to use a blending function on the calculated
/// weights.
/// \param a_prog Progress bar to give user feedback for generation of the
/// nodal functions.
//------------------------------------------------------------------------------
void InterpLinearImpl::SetUseNatNeigh(bool a_,
                                      int a_nodalFunc,
                                      int a_ndFuncOpt,
                                      int a_ndFuncNumNearestPts,
                                      bool a_blendWeights,
                                      BSHP<Observer> a_prog)
{
  m_natNeigh = a_;
  if (!m_natNeigh)
    return;

  m_nn = InterpNatNeigh::New(*m_pts, *m_tris, *m_scalar, m_triSearch.get());
  m_nn->SetBlendWeights(a_blendWeights);

  if (0 != a_nodalFunc && !inAllScalarsEqual(*m_scalar, m_triSearch->GetPtActivity()))
  {
    int nearest(a_ndFuncNumNearestPts);
    bool twod(true), quad_oct(false), modifiedShepardWeights(true);
    double dPower(2);
    InterpNatNeigh* nnPtr(nullptr);
    BSHP<GmPtSearch> pts;
    if (0 != a_ndFuncOpt)
      pts = CreatePtSearch();
    else
      nnPtr = m_nn.get();

    m_nodal = NodalFunc::New(a_nodalFunc, twod, pts, *m_pts, *m_scalar, nearest, quad_oct, dPower,
                             modifiedShepardWeights, a_prog, nnPtr);
    m_nn->SetNodalFunc(m_nodal);
  }
} // InterpLinearImpl::SetUseNatNeigh
//------------------------------------------------------------------------------
/// \brief Creates a GmPtSearch class to be used for nodal function generation
/// \return GmPtSearch.
//------------------------------------------------------------------------------
BSHP<GmPtSearch> InterpLinearImpl::CreatePtSearch()
{
  BSHP<GmPtSearch> pts = GmPtSearch::New(true);
  pts->PtsToSearch(m_pts);
  DynBitset b = m_triSearch->GetPtActivity();
  pts->SetActivity(b);
  return pts;
} // InterpLinearImpl::CreatePtSearch
//------------------------------------------------------------------------------
/// \brief Resets some members of the class
//------------------------------------------------------------------------------
void InterpLinearImpl::RecalcNodalFunc()
{
  if (m_nn)
    m_nn->RecalcNodalFunc();
  else if (m_ct)
    m_ct->RecalcNodalFunc();
} // InterpLinearImpl::RecalcNodalFunc
//------------------------------------------------------------------------------
/// \brief Write the internals to a string.
/// \return The string.
//------------------------------------------------------------------------------
std::string InterpLinearImpl::ToString() const
{
  std::stringstream ss;
  ss << m_cloughTocher << "=cloughTocher " << m_natNeigh << "=natNeigh " << m_trunc << "=trunc "
     << m_truncMax << "=truncMax " << m_truncMin << "=truncMin " << m_extrap << "=extrap "
     << "\n";
  if (m_triSearch)
  {
    ss << m_triSearch->ToString();
    ss << "=triSearch "
       << "\n";
  }
  if (m_pts)
    VecToStream(ss, *m_pts, "pts");
  if (m_tris)
    VecToStream(ss, *m_tris, "tris");
  if (m_scalar)
    VecToStream(ss, *m_scalar, "scalar");
  if (m_nodal)
  {
    ss << m_nodal->ToString();
    ss << "=nodal "
       << "\n";
  }
  if (m_ct)
  {
    ss << m_ct->ToString();
    ss << "=ct "
       << "\n";
  }
  if (m_nn)
  {
    ss << m_nn->ToString();
    ss << "=nn "
       << "\n";
  }
  ss << "\n";
  return ss.str();
} // InterpLinearImpl::ToString

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/interpolate/InterpLinear.t.h>

// namespace xms {
using namespace xms;

////////////////////////////////////////////////////////////////////////////////
/// \class InterpLinearUnitTests
/// \brief tester for the InterpLinearUnitTests class
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief tests serializing class to a string
//------------------------------------------------------------------------------
void InterpLinearUnitTests::testToString()
{
#if 0
  BSHP<InterpLinear> interpolator = InterpLinear::New();
  std::string s = interpolator->ToString();
  std::string expected = "0=cloughTocher 0=natNeigh 0=trunc 0=truncMax 0=truncMin 3.40282e+038,3.40282e+038,3.40282e+038=min -3.40282e+038,-3.40282e+038,-3.40282e+038=max -1e+007=extrap ";
  TS_ASSERT_EQUALS(expected, s);
#endif
} // InterpLinearUnitTests::testToString
//------------------------------------------------------------------------------
/// \brief tests interpolating from 1 triangle
//------------------------------------------------------------------------------
void InterpLinearUnitTests::testOneTriangle()
{
  InterpLinearImpl linear;
  BSHP<VecPt3d> p(new VecPt3d());
  *p = {{0, 0, 1}, {1, 1, 1}, {1, 0, 1}};
  BSHP<VecInt> t(new VecInt());
  *t = {0, 2, 1};
  linear.SetPtsTris(p, t);
  const float EXTRAPVAL = (float)-3.12345678;
  linear.SetExtrapVal((double)EXTRAPVAL);

  Pt3d pt(-.1, 0, 0);
  // double val(0);
  // outside the triangle
  TS_ASSERT_DELTA(EXTRAPVAL, linear.InterpToPt(pt), FLT_EPSILON);
  pt = Pt3d(-1e-6, 0, 0);
  TS_ASSERT_DELTA(EXTRAPVAL, linear.InterpToPt(pt), FLT_EPSILON);
  // inside the triangle
  pt = Pt3d(.5, .25, 0);
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);
  pt = Pt3d(-1e-13, 0, 0);
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);
  // on each edge of the triangle
  pt = Pt3d(.5, .5, 0);
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);
  pt = Pt3d(.5, 0, 0);
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);
  pt = Pt3d(1, .5, 0);
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);
  // at the points of the triangle
  pt = Pt3d();
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);
  pt = Pt3d(1, 1, 0);
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);
  pt = Pt3d(1, 0, 0);
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);

  VecInt idxes = linear.GetExtrapolationPointIndexes();
  VecInt baseIdxes = { 0, 0 };
  TS_ASSERT_EQUALS(baseIdxes, idxes);

  VecPt3d vPts = {{ .5, .5, 0 }, { -1, 0, 0 }, { -.5, 0, 0 }};
  VecFlt scalar;
  linear.InterpToPts(vPts, scalar);
  idxes = linear.GetExtrapolationPointIndexes();
  baseIdxes = { 1, 2 };
  TS_ASSERT_EQUALS(baseIdxes, idxes);
} // InterpLinearUnitTests::testOneTriangle
//------------------------------------------------------------------------------
/// \brief test interpolating from 3 triangles
//------------------------------------------------------------------------------
void InterpLinearUnitTests::testInterp()
{
  InterpLinearImpl linear;
  BSHP<VecPt3d> p(new VecPt3d());
  *p = {{0, 0, 0}, {1, 1, 1}, {1, 0, 2}};
  BSHP<VecInt> t(new VecInt());
  *t = {0, 2, 1};
  linear.SetPtsTris(p, t);

  Pt3d pt(.5, .5, 0);
  // double val(0);
  TS_ASSERT_DELTA(.5, linear.InterpToPt(pt), FLT_EPSILON);
  pt = Pt3d(.5, 0, 0);
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);
  pt = Pt3d(1, .5, 0);
  TS_ASSERT_DELTA(1.5, linear.InterpToPt(pt), FLT_EPSILON);
  pt = Pt3d(.66666666666666666, .3333333333333333333, 0);
  TS_ASSERT_DELTA(1, linear.InterpToPt(pt), FLT_EPSILON);
} // InterpLinearUnitTests::testInterp
//------------------------------------------------------------------------------
/// \brief test clough tocher interpolation
//------------------------------------------------------------------------------
void InterpLinearUnitTests::testCt()
{
  InterpLinearImpl linear;
  BSHP<VecPt3d> p(new VecPt3d());
  *p = {{0, 0, 0}, {1, 1, 1}, {1, 0, 2}};
  BSHP<VecInt> t(new VecInt());
  *t = {0, 2, 1};
  linear.SetPtsTris(p, t);

  Pt3d pt(.5, .5, 0);
  linear.SetUseCloughTocher(true, BSHP<Observer>());
  TS_ASSERT_DELTA(.5f, linear.InterpToPt(pt), 1e-7);
  linear.SetTrunc(-1, 0);
  TS_ASSERT_DELTA(-1.0f, linear.InterpToPt(pt), FLT_EPSILON);
  pt.x = .5;
  pt.y = .1;
  TS_ASSERT_DELTA(-1.0f, linear.InterpToPt(pt), FLT_EPSILON);
  pt.x = .2;
  pt.y = .15;
  TS_ASSERT_DELTA(-1.0f, linear.InterpToPt(pt), FLT_EPSILON);
  pt.x = .9;
  pt.y = .5;
  TS_ASSERT_DELTA(-1.0f, linear.InterpToPt(pt), FLT_EPSILON);
} // InterpLinearUnitTests::testCt

//} // namespace xms
#endif // CXX_TEST
