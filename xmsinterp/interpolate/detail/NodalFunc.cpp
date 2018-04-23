//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/interpolate/detail/NodalFunc.h>

// 3. Standard library headers

// 4. External library headers
#include <boost/thread/mutex.hpp>
#include <boost/weak_ptr.hpp>

// 5. Shared code headers
//#include <shared1/containers/ContinuousArrays.h>
#include <xmscore/misc/XmError.h>
#include <xmscore/misc/XmLog.h>
#include <xmscore/stl/vector.h>
#include <xmscore/stl/utility.h>
#include <xmsinterp/geometry/GmPtSearch.h>
#include <xmsinterp/interpolate/InterpUtil.h>
#include <xmsinterp/interpolate/detail/InterpNatNeigh.h>
#include <xmsinterp/matrices/matrix.h>
#include <xmsinterp/thread/ThreadLoop.h>
#include <xmsinterp/thread/ThreadMgr.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
// using namespace xms;
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------
/// \brief Implementation of NodalFunc
class NodalFuncImpl : public NodalFunc
{
public:
  /// Nodal function type enumeration
  enum { NF_CONSTANT, NF_GRAD_PLANE, NF_QUADRATIC };

  /// \brief Threading class to compute nodal functions in parallel
  class NfThread : public ThreadLoop
  {
  public:
    /// \brief constructor
    /// \param[in] a_nf Parent class
    /// \param[in] a_ptSearch Spatial index for searching points
    NfThread(NodalFuncImpl* a_nf, BSHP<GmPtSearch> a_ptSearch)
    : m_nf(a_nf)
    , m_ptSearch(a_ptSearch)
    {
    }

    /// \brief Creates an instance of this class for the thread manager
    /// \return ThreadLoop.
    BSHP<ThreadLoop> CreateForNewThread()
    {
      BSHP<NfThread> r(new NfThread(m_nf, m_ptSearch));
      BSHP<ThreadLoop> q = BDPC<ThreadLoop>(r);
      return q;
    }

    NodalFuncImpl* m_nf;         ///< pointer to parent class
    BSHP<GmPtSearch> m_ptSearch; ///< spatial index for searching points
    VecInt m_nearestPts;         ///< indexes of nearest points to location
    std::vector<InterpPtInfo>
      m_matrixPts; ///< information about each point for nodal function calculation
    VecDbl m_d2;   ///< distance squared from location to each point
    VecDbl m_w;    ///< weight for each point
  private:
    /// \brief Calculates the nodal function in this thread
    //------------------------------------------------------------------------------
    /// \brief
    //------------------------------------------------------------------------------
    void Worker() override
    {
      if (m_nf->m_errorReport)
        return;
      m_nf->NfForPt((int)CurrIdx(), m_ptSearch, m_nearestPts, m_matrixPts, m_d2, m_w);
    }
  };

  NodalFuncImpl(const std::vector<Pt3d>& a_pts, const std::vector<float>& a_scalar);
  NodalFuncImpl(int a_type,
                bool a_2d,
                BSHP<GmPtSearch> a_ptSearch,
                const std::vector<Pt3d>& a_pts,
                const std::vector<float>& a_scalar,
                int a_nNearest,
                bool a_quad_oct,
                double a_power,
                bool a_modifiedShepardWeights,
                BSHP<Observer> a_p,
                InterpNatNeigh* a_natNeigh);

  virtual double ScalarFromPtIdx(int a_ptIdx, const Pt3d& a_loc) const override;
  virtual void GradientFromPtIdx(int a_ptIdx, Pt3d& a_grad) const override;
  virtual void ComputeNodalFuncs() override;
  virtual std::string ToString() const override;

  void NfForPt(int a_ptIdx,
               BSHP<GmPtSearch> a_s,
               std::vector<int>& nearestPts,
               std::vector<InterpPtInfo>& matrixPts,
               std::vector<double>& d2,
               std::vector<double>& w);
  void NodalFuncForPtFromMatrixPts(int a_ptIdx, const std::vector<InterpPtInfo>& a_pts);

  void NodalFunc2d(int a_ptIdx, const std::vector<InterpPtInfo>& a_pts, double* a_A);
  void NodalFunc3d(int a_ptIdx, const std::vector<InterpPtInfo>& a_pts, double* a_A);
  Pt3d ComputeGradientForPoint(int a_ptIdx, const std::vector<InterpPtInfo>& a_pts);

  bool m_2d;      ///< flag for computing distances in xy instead of xyz
  int m_type;     ///< 1 gradient plane, 2 quadratic
  int m_nNearest; ///< number of nearest points to consider in calculations
  bool m_quadOct; ///< flag for performing quadrant(2d) or octant (3d) searching for nearest pts
  const std::vector<Pt3d>& m_pts;     ///< Points used to interpolate
  const std::vector<float>& m_scalar; ///< Scalars at the points used to interpolate
  BSHP<GmPtSearch> m_ptSearch;        ///< Class used to find nearest points
  std::vector<Pt3d> m_gradient;       ///< Calculated gradient coefficients at each point
  VecDbl2d m_quadratic;               ///< Calculated quadratic coefficients at each point
  double m_power;                     ///< exponent used with inverse distance (1/d^m_power)
  bool m_modifiedShepardWeights; ///< flag for calculating weights using a the modified shepard's
                                 ///< approach
  std::vector<int> m_nearestAll; ///< convenience variable when all points are used in calculation
  BSHP<Observer> m_prog;         ///< Observer that reports status of interpolation process
  bool m_errorReport;            ///< flag for reporting errors
  bool m_debugTest;              ///< flag used for testing error reporting
  /// Pointer to a Natural neighbor interpolator. Used if nearest points are
  /// found using natural neighbors.
  InterpNatNeigh* m_natNeigh;
};

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

//------------------------------------------------------------------------------
/// \brief Creates a NodalFunc class
/// \param[in] a_type The type of nodal function (gradient/quadratic)
/// \param[in] a_2d Flag to indicate that only xy and not xyz is considered
/// \param[in] a_ptSearch Spatial index for searching points
/// \param[in] a_pts The points at which nodal functions are calculated
/// \param[in] a_scalar The scalar at the points
/// \param[in] a_nNearest The number of nearest points to consider when
/// calculating nodal functions
/// \param[in] a_quad_oct Flag to do searching for nearest points based on
/// quadrants (2d) or octants (3d)
/// \param[in] a_power Exponent used when calculating weights for nearest
/// points (1/d^a_power)
/// \param[in] a_modifiedShepardWeights Flag for calculating weights using
/// the modified shepard's approach
/// \param[in] a_p Observer that reports status of interpolation process
/// \param[in] a_natNeigh Pointer to a Natural neighbor interpolator. Used if
/// nearest points are found using natural neighbors.
/// \return Shared pointer to an instance of NodalFunc.
//------------------------------------------------------------------------------
BSHP<NodalFunc> NodalFunc::New(int a_type,
                               bool a_2d,
                               BSHP<GmPtSearch> a_ptSearch,
                               const std::vector<Pt3d>& a_pts,
                               const std::vector<float>& a_scalar,
                               int a_nNearest,
                               bool a_quad_oct,
                               double a_power,
                               bool a_modifiedShepardWeights,
                               BSHP<Observer> a_p,
                               InterpNatNeigh* a_natNeigh)
{
  BSHP<NodalFunc> ptr(new NodalFuncImpl(a_type, a_2d, a_ptSearch, a_pts, a_scalar, a_nNearest,
                                        a_quad_oct, a_power, a_modifiedShepardWeights, a_p,
                                        a_natNeigh));
  return ptr;
} // NodalFunc::New
//------------------------------------------------------------------------------
/// \brief Constructor
//------------------------------------------------------------------------------
NodalFunc::NodalFunc()
{
} // NodalFunc::NodalFunc
//------------------------------------------------------------------------------
/// \brief Destructor
//------------------------------------------------------------------------------
NodalFunc::~NodalFunc()
{
} // NodalFunc::~NodalFunc

////////////////////////////////////////////////////////////////////////////////
/// \class NodalFuncImpl
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief this constructor is only used with testing
/// \param[in] a_pts The points at which nodal functions are calculated
/// \param[in] a_scalar The scalar at the points
//------------------------------------------------------------------------------
NodalFuncImpl::NodalFuncImpl(const std::vector<Pt3d>& a_pts, const std::vector<float>& a_scalar)
: m_2d(true)
, m_type(0)
, m_nNearest(8)
, m_quadOct(false)
, m_pts(a_pts)
, m_scalar(a_scalar)
, m_ptSearch()
, m_gradient()
, m_quadratic()
, m_power(2.0)
, m_modifiedShepardWeights(true)
, m_prog()
, m_errorReport(false)
, m_debugTest(true)
, m_natNeigh(nullptr)
{
} // NodalFuncImpl::NodalFuncImpl
//------------------------------------------------------------------------------
/// \brief Constructor
/// \param[in] a_type The type of nodal function (gradient/quadratic)
/// \param[in] a_2d Flag to indicate that only xy and not xyz is considered
/// \param[in] a_ptSearch Spatial index for searching points
/// \param[in] a_pts The points at which nodal functions are calculated
/// \param[in] a_scalar The scalar at the points
/// \param[in] a_nNearest The number of nearest points to consider when
/// calculating nodal functions
/// \param[in] a_quad_oct Flag to do searching for nearest points based on
/// quadrants (2d) or octants (3d)
/// \param[in] a_power Exponent used when calculating weights for nearest
/// points (1/d^a_power)
/// \param[in] a_modifiedShepardWeights Flag for calculating weights using
/// the modified shepard's approach
/// \param[in] a_p Observer that reports status of interpolation process
/// \param[in] a_natNeigh Pointer to a Natural neighbor interpolator. Used if
/// nearest points are found using natural neighbors.
//------------------------------------------------------------------------------
NodalFuncImpl::NodalFuncImpl(int a_type,
                             bool a_2d,
                             BSHP<GmPtSearch> a_ptSearch,
                             const std::vector<Pt3d>& a_pts,
                             const std::vector<float>& a_scalar,
                             int a_nNearest,
                             bool a_quad_oct,
                             double a_power,
                             bool a_modifiedShepardWeights,
                             BSHP<Observer> a_p,
                             InterpNatNeigh* a_natNeigh)
: m_2d(a_2d)
, m_type(a_type)
, m_nNearest(a_nNearest)
, m_quadOct(a_quad_oct)
, m_pts(a_pts)
, m_scalar(a_scalar)
, m_ptSearch(a_ptSearch)
, m_gradient()
, m_quadratic()
, m_power(a_power)
, m_modifiedShepardWeights(a_modifiedShepardWeights)
, m_prog(a_p)
, m_errorReport(false)
, m_debugTest(false)
, m_natNeigh(a_natNeigh)
{
  if (NF_GRAD_PLANE == m_type)
    m_gradient.assign(m_pts.size(), Pt3d());
  else if (NF_QUADRATIC == m_type)
  {
    // m_quadratic.SetSize((int)m_scalar.size(), 9, 0);
    m_quadratic.assign((int)m_scalar.size(), VecDbl(9, 0.0));
  }
  if (m_nNearest < 1)
  {
    for (size_t i = 0; i < m_pts.size(); ++i)
      m_nearestAll.push_back((int)i);
  }
  ComputeNodalFuncs();
} // NodalFuncImpl::NodalFuncImpl
//------------------------------------------------------------------------------
/// \brief Computes gradients at each point for use in the interpolation
/// process.
//------------------------------------------------------------------------------
void NodalFuncImpl::ComputeNodalFuncs()
{
  if (m_pts.empty())
    return;
  BSHP<GmPtSearch> s = m_ptSearch;

  if (m_pts.size() > 500)
  {
    BSHP<ThreadMgr> mgr = ThreadMgr::New();
    mgr->SetThreadLoopClass(NfThread(this, s).CreateForNewThread());
    if (m_pts.size() < 10000)
      mgr->ExplicitlySetNumThreads(2);
    mgr->SetObserver(m_prog);
    mgr->RunThreads((int)m_pts.size());
  }
  else
  {
    BSHP<GmPtSearch> ps = m_ptSearch;
    std::vector<int> nearestPts;
    std::vector<InterpPtInfo> matrixPts;
    std::vector<double> d2, w;
    for (size_t i = 0; i < m_pts.size(); ++i)
    {
      NfForPt((int)i, ps, nearestPts, matrixPts, d2, w);
    }
  }
} // NodalFuncImpl::ComputeNodalFuncs
//------------------------------------------------------------------------------
/// \brief Computes the nodal function for this point
/// \param[in] a_ptIdx Index of the point where calculation occurs
/// \param[in] s Spatial index for searching points
/// \param[out] nearestPts Vector of point indexes nearest to a_ptIdx
/// \param[out] matrixPts Vector of information about the nearest points to a_ptIdx
/// \param[out] d2 Vector of distance squared for each nearest point to a_ptIdx
/// \param[out] w Vector of weights for each nearest point to a_ptIdx
//------------------------------------------------------------------------------
void NodalFuncImpl::NfForPt(int a_ptIdx,
                            BSHP<GmPtSearch> s,
                            std::vector<int>& nearestPts,
                            std::vector<InterpPtInfo>& matrixPts,
                            std::vector<double>& d2,
                            std::vector<double>& w)
{
  const Pt3d& p = m_pts[a_ptIdx];
  // get the nNearest points to this point
  if (m_nNearest < 1)
  {
    nearestPts = m_nearestAll;
    // remove my index from nearest vector
    nearestPts.erase(nearestPts.begin() + a_ptIdx);
  }
  else if (m_natNeigh)
    m_natNeigh->GetNeighbors(a_ptIdx, nearestPts);
  else
    s->NearestPtsToPtInRtree(a_ptIdx, p, m_nNearest, m_quadOct, nearestPts);
  // get the distance squared between our point and the other points
  inDistanceSquared(p, nearestPts, m_pts, m_2d, d2);
  inIdwWeights(d2, m_power, m_modifiedShepardWeights, w);

  matrixPts.resize(nearestPts.size());
  int idx;
  for (size_t j = 0; j < nearestPts.size(); ++j)
  {
    idx = nearestPts[j];
    matrixPts[j].m_loc = m_pts[idx];
    matrixPts[j].m_weight = w[j];
    matrixPts[j].m_scalar = m_scalar[idx];
  }
  NodalFuncForPtFromMatrixPts(a_ptIdx, matrixPts);
} // NodalFuncImpl::NfForPt
//------------------------------------------------------------------------------
/// \brief Computes the nodal function for this point
/// \param[in] a_ptIdx Index of the point where calculation occurs
/// \param[in] a_pts Vector of information of the nearest n points to a_ptIdx
//------------------------------------------------------------------------------
void NodalFuncImpl::NodalFuncForPtFromMatrixPts(int a_ptIdx, const std::vector<InterpPtInfo>& a_pts)
{
  if (1 == m_type)
  {
    m_gradient[a_ptIdx] = ComputeGradientForPoint(a_ptIdx, a_pts);
  }
  else
  {
    double A[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    if (m_2d)
      NodalFunc2d(a_ptIdx, a_pts, A);
    else
      NodalFunc3d(a_ptIdx, a_pts, A);
    for (int i = 0; i < 9; ++i)
      m_quadratic[a_ptIdx][i] = A[i];
  }
} // NodalFuncImpl::ComputeNodalFuncForPt
//------------------------------------------------------------------------------
/// \brief Computes the matrix for the passed in point
/// \param[in] a_ptIdx Index of the point where calculation occurs
/// \param[in] a_pts Vector of information of the nearest n points to a_ptIdx
/// \param[out] a_A Matrix that is calculated by this method
//------------------------------------------------------------------------------
void NodalFuncImpl::NodalFunc2d(int a_ptIdx, const std::vector<InterpPtInfo>& a_pts, double* a_A)
{
  const Pt3d& p = m_pts[a_ptIdx];
  double *M[9], VV[9], *A(a_A), m[9][9];
  for (int i = 0; i < 9; ++i)
    M[i] = &m[i][0];
  // set up matrix for computing gradients
  inNodalFuncSetUpMatrixAndVector(p.x, p.y, m_scalar[a_ptIdx], a_pts, M, VV);
  if (!mxSolveBandedEquations(M, A, VV, 5, 5))
  {
    boost::mutex mtx;
    mtx.lock();
    m_errorReport = true;
    std::string msg =
      "Error computing nodal functions. Increase the number "
      "of points used for nodal function calculation.";
    XM_LOG(xmlog::error, msg);
    mtx.unlock();
  }
} // NodalFuncImpl::NodalFunc2d
//------------------------------------------------------------------------------
/// \brief Computes the matrix for the passed in point
/// \param[in] a_ptIdx Index of the point where calculation occurs
/// \param[in] a_pts Vector of information of the nearest n points to a_ptIdx
/// \param[out] a_A Matrix that is calculated by this method
//------------------------------------------------------------------------------
void NodalFuncImpl::NodalFunc3d(int a_ptIdx, const std::vector<InterpPtInfo>& a_pts, double* a_A)
{
  const Pt3d& p = m_pts[a_ptIdx];
  double *M[10], VV[10], *A(a_A), m[10][10];
  for (int i = 0; i < 10; ++i)
    M[i] = &m[i][0];
  // set up matrix for computing gradients
  inNodalFuncSetUpMatrixAndVector3d(p.x, p.y, p.z, m_scalar[a_ptIdx], a_pts, M, VV);
  if (!mxSolveBandedEquations(M, A, VV, 9, 9))
  {
    boost::mutex mtx;
    mtx.lock();
    m_errorReport = true;
    std::string msg =
      "Error computing nodal functions. Increase the number "
      "of points used for nodal function calculation.";
    XM_LOG(xmlog::error, msg);
    mtx.unlock();
  }
} // NodalFuncImpl::NodalFunc2d
//------------------------------------------------------------------------------
/// \brief Computes the gradient for this point
/// \param[in] a_ptIdx Index of the point where calculation occurs
/// \param[in] a_pts Vector of information of the nearest n points to a_ptIdx
/// \return The gradient at this location.
//------------------------------------------------------------------------------
Pt3d NodalFuncImpl::ComputeGradientForPoint(int a_ptIdx, const std::vector<InterpPtInfo>& a_pts)
{
  Pt3d grad;
  if (m_2d)
  {
    double A[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    NodalFunc2d(a_ptIdx, a_pts, A);
    grad.x = A[0];
    grad.y = A[1];
    return grad;
  }

  // 3D
  double A[3][3], x[3], B[3], *Aptr[3];
  for (int i = 0; i < 3; ++i)
  {
    Aptr[i] = &A[i][0];
    x[i] = B[i] = 0;
    for (int j = 0; j < 3; ++j)
      A[i][j] = 0;
  }

  const Pt3d& p = m_pts[a_ptIdx];
  double dx, dy, dz(0), df, weight;
  for (size_t i = 0; i < a_pts.size(); ++i)
  {
    weight = a_pts[i].m_weight;
    dx = a_pts[i].m_loc.x - p.x;
    dy = a_pts[i].m_loc.y - p.y;
    if (!m_2d)
      dz = a_pts[i].m_loc.z - p.z;
    df = a_pts[i].m_scalar - m_scalar[a_ptIdx];

    A[0][0] += weight * dx * dx;
    A[0][1] += weight * dx * dy;
    A[0][2] += weight * dx * dz;
    A[1][1] += weight * dy * dy;
    A[1][2] += weight * dy * dz;
    A[2][2] += weight * dz * dz;

    B[0] += weight * df * dx;
    B[1] += weight * df * dy;
    B[2] += weight * df * dz;
  }
  A[1][0] = A[0][1];
  A[2][0] = A[0][2];
  A[2][1] = A[1][2];

  bool err = false;
  if (m_2d)
    err = !mxSolveNxN(&Aptr[0], x, B, 2);
  else
    err = !mxSolve3x3(A, x, B);
  if (err || m_debugTest)
  {
    if (B[0] == 0.0 && B[1] == 0.0 && B[2] == 0.0)
    { // ds was probably zero which is OK. Apply the zero or trivial solution
      x[0] = x[1] = x[2] = 0.0;
    }
    else if (!m_errorReport)
    {
      boost::mutex mtx;
      mtx.lock();
      m_errorReport = true;
      std::string msg =
        "Error computing nodal functions. Increase the number "
        "of points used for nodal function calculation.";
      XM_LOG(xmlog::error, msg);
      mtx.unlock();
    }
  }
  grad.x = x[0];
  grad.y = x[1];
  grad.z = x[2];
  return grad;
} // NodalFuncImpl::ComputeGradientForPoint
//------------------------------------------------------------------------------
/// \brief Computes gradients at each point for use in the interpolation
/// process.
/// \param[in] a_ptIdx Index of the point where nodal function coefficients and
/// scalar is used to return a modified scalar based on nodal function.
/// \param[in] a_loc The location being interpolated to
/// \return Modified scalar value at a_ptIdx based on the nodal function.
//------------------------------------------------------------------------------
double NodalFuncImpl::ScalarFromPtIdx(int a_ptIdx, const Pt3d& a_loc) const
{
  if (1 == m_type)
  {
    if (m_2d)
    {
      double dc, qi;
      dc = m_gradient[a_ptIdx].x * m_pts[a_ptIdx].x + m_gradient[a_ptIdx].y * m_pts[a_ptIdx].y -
           m_scalar[a_ptIdx];
      qi = m_gradient[a_ptIdx].x * a_loc.x + m_gradient[a_ptIdx].y * a_loc.y - dc;
      return qi;
    }

    double f;
    f = m_scalar[a_ptIdx] + m_gradient[a_ptIdx].x * (a_loc.x - m_pts[a_ptIdx].x) +
        m_gradient[a_ptIdx].y * (a_loc.y - m_pts[a_ptIdx].y) +
        m_gradient[a_ptIdx].z * (a_loc.z - m_pts[a_ptIdx].z);
    return f;
  }
  else
  {
    double fi = (double)m_scalar[a_ptIdx];
    double qi, dx, dy, dz;
    dx = a_loc.x - m_pts[a_ptIdx].x;
    dy = a_loc.y - m_pts[a_ptIdx].y;
    if (m_2d)
    {
      qi = fi + m_quadratic[a_ptIdx][0] * dx + m_quadratic[a_ptIdx][1] * dy +
           m_quadratic[a_ptIdx][2] * dx * dx + m_quadratic[a_ptIdx][3] * dx * dy +
           m_quadratic[a_ptIdx][4] * dy * dy;
      return qi;
    }

    dz = a_loc.z - m_pts[a_ptIdx].z;
    qi = fi + m_quadratic[a_ptIdx][0] * dx + m_quadratic[a_ptIdx][1] * dy +
         m_quadratic[a_ptIdx][2] * dz + m_quadratic[a_ptIdx][3] * dx * dy +
         m_quadratic[a_ptIdx][4] * dx * dz + m_quadratic[a_ptIdx][5] * dy * dz +
         m_quadratic[a_ptIdx][6] * dx * dx + m_quadratic[a_ptIdx][7] * dy * dy +
         m_quadratic[a_ptIdx][8] * dz * dz;
    return qi;
  }
} // NodalFuncImpl::ScalarFromPtIdx
//------------------------------------------------------------------------------
/// \brief Fills in the gradient
/// \param[in] a_ptIdx Point index to find the calculated gradient
/// \param[out] a_grad Gradient value filled by this method.
//------------------------------------------------------------------------------
void NodalFuncImpl::GradientFromPtIdx(int a_ptIdx, Pt3d& a_grad) const
{
  if (a_ptIdx > 0 && a_ptIdx < (int)m_gradient.size())
    a_grad = m_gradient[a_ptIdx];
} // NodalFuncImpl::GradientFromPtIdx
//------------------------------------------------------------------------------
/// \brief Write the internals to a string.
/// \return The string.
//------------------------------------------------------------------------------
std::string NodalFuncImpl::ToString() const
{
  std::stringstream ss;
  ss << m_2d << "=2d " << m_type << "=type " << m_nNearest << "=nNearest " << m_quadOct
     << "=quadOct " << m_power << "=power " << m_modifiedShepardWeights
     << "=modifiedShepardWeights " << m_errorReport << "=errorReport " << m_debugTest
     << "=debugTest "
     << "\n";

  VecToStream(ss, m_pts, "pts");
  VecToStream(ss, m_scalar, "scalar");
  VecToStream(ss, m_gradient, "gradient");
  for (size_t j = 0; j < m_quadratic.size(); ++j)
  {
    VecToStream(ss, m_quadratic[j], "quadratic");
  }
  ss << "=quadratic "
     << "\n";

  VecToStream(ss, m_nearestAll, "nearestAll");

  return ss.str();
} // NodalFuncImpl::ToString

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/interpolate/detail/NodalFunc.t.h>
#include <xmscore/testing/TestTools.h>

// namespace xms {
using namespace xms;
////////////////////////////////////////////////////////////////////////////////
/// \class NodalFuncUnitTests
/// \brief tester for the NodalFunc class
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief test class creation
//------------------------------------------------------------------------------
void NodalFuncUnitTests::testCreateClass()
{
  BSHP<GmPtSearch> pts;
  std::vector<Pt3d> vPts;
  std::vector<float> vScalar;
  BSHP<NodalFunc> nf =
    NodalFunc::New(0, true, pts, vPts, vScalar, -1, false, 2, true, nullptr, nullptr);
  TS_ASSERT(nf);
} // NodalFuncUnitTests::testCreateClass
//------------------------------------------------------------------------------
/// \brief test computing a gradient
//------------------------------------------------------------------------------
void NodalFuncUnitTests::testComputeGradientForPoint()
{
  std::vector<Pt3d> p(1, Pt3d(0, 0, 0));
  std::vector<float> s(1, 0);
  NodalFuncImpl nf(p, s);
  nf.m_2d = false;
  std::vector<InterpPtInfo> pInfo(2);
  pInfo[0].m_loc = Pt3d(1, 0, 0);
  pInfo[0].m_scalar = 0;
  pInfo[0].m_weight = .5;
  pInfo[1].m_loc = Pt3d(2, 0, 0);
  pInfo[1].m_scalar = 0;
  pInfo[1].m_weight = .5;
  Pt3d grad = nf.ComputeGradientForPoint(0, pInfo);
  TS_ASSERT_EQUALS(Pt3d(0, 0, 0), grad);
  pInfo[0].m_scalar = 1;
  pInfo[1].m_scalar = 2;
  XmLog::Instance().GetAndClearStackStr();
  grad = nf.ComputeGradientForPoint(0, pInfo);
  TS_ASSERT_EQUALS(true, (XmLog::Instance().ErrCount() != 0));
  XmLog::Instance().GetAndClearStackStr();
} // NodalFuncUnitTests::testComputeGradientForPoint

//} // namespace xms
#endif // CXX_TEST
