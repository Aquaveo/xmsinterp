//------------------------------------------------------------------------------
/// \file
/// \brief Linear interpolation class with idw extrapolation
/// \copyright (C) Copyright Aquaveo 2020. Distributed under the xmsng
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My header
#include <xmsinterp/interpolate/InterpLinearExtrapIdw.h>

// 3. Standard Library Headers

// 4. External Library Headers
#include <xmscore/misc/xmstype.h>

// 5. Shared Headers

// 6. Non-shared Headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------


//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class InterpLinearExtrapIdw
/// \brief Linear interpolation from a set of triangles
/// Uses the boost RTree for queries.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
InterpLinearExtrapIdw::InterpLinearExtrapIdw()
  : m_linear(InterpLinear::New())
  , m_idw(InterpIdw::New())
{
} // InterpLinearExtrapIdw::InterpLinearExtrapIdw
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
//InterpLinearExtrapIdw::~InterpLinearExtrapIdw()
//{
//} // InterpLinearExtrapIdw::~InterpLinearExtrapIdw
//------------------------------------------------------------------------------
/// \brief Creates an TriSearch class
/// \return Shared pointer to InterpLinear.
//------------------------------------------------------------------------------
BSHP<InterpLinearExtrapIdw> InterpLinearExtrapIdw::New()
{
  BSHP<InterpLinearExtrapIdw> ptr(new InterpLinearExtrapIdw());
  return ptr;
} // InterpLinear::New
//------------------------------------------------------------------------------
/// \brief Adds the triangles to the class
/// \param a_pts Array of point locations.
/// \param a_tris Array of triangles that references the a_pts array. This
/// array will have a size that is a multiple of 3. The first 3 locations in
/// array represent the first triangle and will have indices that correspond to
/// locations in the a_pts array.
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::SetPtsTris(BSHP<VecPt3d> a_pts, BSHP<VecInt> a_tris)
{
  m_linear->SetPtsTris(a_pts, a_tris);
  m_idw->SetPtsTris(a_pts, a_tris);
} // InterpLinearExtrapIdw::SetPtsTris
//------------------------------------------------------------------------------
/// \brief Set the scalars that will be used to interpolate from.
/// \param a_scalar Array of interpolation scalar values.
/// \param a_n Size of the array a_scalar.
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::SetScalars(const float* a_scalar, size_t a_n)
{
  m_linear->SetScalars(a_scalar, a_n);
  m_idw->SetScalars(a_scalar, a_n);
} // InterpLinearExtrapIdw::SetScalars
//------------------------------------------------------------------------------
/// \brief Set the scalars that will be used to interpolate from.
/// \param a_scalar: Array of interpolation scalar values.
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::SetScalars(BSHP<VecFlt> a_scalar)
{
  m_linear->SetScalars(a_scalar);
  m_idw->SetScalars(a_scalar);
} // InterpLinearExtrapIdw::SetScalars
//------------------------------------------------------------------------------
/// \brief Modifies the activity bitset of the class.
/// \param a_activity bitset of the activity of the points
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::SetPtActivity(DynBitset& a_activity)
{
  m_linear->SetPtActivity(a_activity);
  m_idw->SetPtActivity(a_activity);
} // InterpLinearExtrapIdw::SetPtActivity
//------------------------------------------------------------------------------
/// \brief Modifies the activity bitset of the class.
/// \param a_activity bitset of the activity of the points
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::SetTriActivity(DynBitset& a_activity)
{
  m_linear->SetTriActivity(a_activity);
} // InterpLinearExtrapIdw::SetTriActivity
//------------------------------------------------------------------------------
/// \brief Returns shared pointer to points vector.
/// \return Shared pointer to points vector.
//------------------------------------------------------------------------------
const BSHP<VecPt3d> InterpLinearExtrapIdw::GetPts() const
{
  return m_linear->GetPts();
} // InterpLinearExtrapIdw::GetPts
//------------------------------------------------------------------------------
/// \brief get point activity
/// \return bitset of point activity
//------------------------------------------------------------------------------
DynBitset InterpLinearExtrapIdw::GetPtActivity() const
{
  return m_linear->GetPtActivity();
} // InterpLinearExtrapIdw::GetPtActivity
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
double InterpLinearExtrapIdw::GetExtrapVal() const
{
  return m_linear->GetExtrapVal();
} // InterpLinearExtrapIdw::GetExtrapVal
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
bool InterpLinearExtrapIdw::GetTruncateInterpolatedValues() const
{
  return m_linear->GetTruncateInterpolatedValues();
} // InterpLinearExtrapIdw::GetTruncateInterpolatedValues
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
double InterpLinearExtrapIdw::GetTruncMin() const
{
  return m_linear->GetTruncMin();
} // InterpLinearExtrapIdw::GetTruncMin
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
double InterpLinearExtrapIdw::GetTruncMax() const
{
  return m_linear->GetTruncMax();
} // InterpLinearExtrapIdw::GetTruncMax
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
bool InterpLinearExtrapIdw::GetUseCloughTocher() const
{
  return m_linear->GetUseCloughTocher();
} // InterpLinearExtrapIdw::GetUseCloughTocher
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
bool InterpLinearExtrapIdw::GetUseNatNeigh() const
{
  return m_linear->GetUseNatNeigh();
} // InterpLinearExtrapIdw::GetUseNatNeigh
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
int InterpLinearExtrapIdw::GetNatNeighNodalFunc() const
{
  return m_linear->GetNatNeighNodalFunc();
} // InterpLinearExtrapIdw::GetNatNeighNodalFunc
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
int InterpLinearExtrapIdw::GetNatNeighNodalFuncNearestPtsOption() const
{
  return m_linear->GetNatNeighNodalFuncNearestPtsOption();
} // InterpLinearExtrapIdw::GetNatNeighNodalFuncNearestPtsOption
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
int InterpLinearExtrapIdw::GetNatNeighNodalFuncNumNearestPts() const
{
  return m_linear->GetNatNeighNodalFuncNumNearestPts();
} // InterpLinearExtrapIdw::GetNatNeighNodalFuncNumNearestPts
//------------------------------------------------------------------------------
/// \brief 
/// \return 
//------------------------------------------------------------------------------
bool InterpLinearExtrapIdw::GetNatNeighBlendWeights() const
{
  return m_linear->GetNatNeighBlendWeights();
} // InterpLinearExtrapIdw::GetNatNeighBlendWeights
//------------------------------------------------------------------------------
/// \brief get triangle activity
/// \return bitset of triangle activity
//------------------------------------------------------------------------------
DynBitset InterpLinearExtrapIdw::GetTriActivity() const
{
  return m_linear->GetTriActivity();
} // InterpLinearExtrapIdw::GetTriActivity
//------------------------------------------------------------------------------
/// \brief Returns shared pointer to triangles vector.
/// \return Shared pointer to triangles vector.
//------------------------------------------------------------------------------
const BSHP<VecInt> InterpLinearExtrapIdw::GetTris() const
{
  return m_linear->GetTris();
} // InterpLinearExtrapIdw::GetTris
//------------------------------------------------------------------------------
/// \brief Returns shared pointer to scalars vector.
/// \return Shared pointer to scalars vector.
//------------------------------------------------------------------------------
const BSHP<VecFlt> InterpLinearExtrapIdw::GetScalars() const
{
  return m_linear->GetScalars();
} // InterpLinearExtrapIdw::GetScalars
//------------------------------------------------------------------------------
/// \brief Returns vector of point indexes for points that were outside of
/// all triangles
/// \return Returns vector of points indexes
//------------------------------------------------------------------------------
VecInt InterpLinearExtrapIdw::GetExtrapolationPointIndexes() const
{
  return m_linear->GetExtrapolationPointIndexes();
} // InterpLinearExtrapIdw::GetExtrapolationPointIndexes
//------------------------------------------------------------------------------
/// \brief Use the stored triangles to interpolate to a point. Returns
/// extrapolation value if the point is outside the triangles.
/// \param a_pt Location that is interpolated to.
/// \return Interpolated value at a_pt.
//------------------------------------------------------------------------------
float InterpLinearExtrapIdw::InterpToPt(const Pt3d& a_pt)
{
  float rval = XM_NODATA;
  if (m_linear->TriContainingPt(a_pt) > -1)
    rval = m_linear->InterpToPt(a_pt);
  else
    rval = m_idw->InterpToPt(a_pt);
  return rval;
} // InterpLinearExtrapIdw::InterpToPt
//------------------------------------------------------------------------------
/// \brief   Calls InterpToPt in a loop.
/// \param a_pts Locations of points.
/// \param a_scalars Interpolated scalar values at a_pts.
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::InterpToPts(const VecPt3d& a_pts, VecFlt& a_scalars)
{
  m_linear->InterpToPts(a_pts, a_scalars);
  VecInt extrap = m_linear->GetExtrapolationPointIndexes();
  for (size_t i = 0; i < extrap.size(); ++i)
  {
    a_scalars[extrap[i]] = m_idw->InterpToPt(a_pts[extrap[i]]);
  }
} // InterpLinearExtrapIdw::InterpToPts
//------------------------------------------------------------------------------
/// \brief Find the triangle containing the point
/// \param a_pt Location used to find a triangle.
/// \return Index of triangle containing a_pt. If XM_NONE is returned then no
/// triangle contained the point.
//------------------------------------------------------------------------------
int InterpLinearExtrapIdw::TriContainingPt(const Pt3d& a_pt)
{
  return m_linear->TriContainingPt(a_pt);
} // InterpLinearExtrapIdw::TriContainingPt
//------------------------------------------------------------------------------
/// \brief Find all triangle whose envelop contains the point.
/// \param a_pt Location used to find a triangle.
/// \param a_tris The indices to triangles whose envelop contains the point
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::TriEnvelopsContainingPt(const Pt3d& a_pt, VecInt& a_tris)
{
  m_linear->TriEnvelopsContainingPt(a_pt, a_tris);
} // InterpLinearExtrapIdw::TriContainingPt
//------------------------------------------------------------------------------
/// \brief Use the stored triangles to get interpolation weights for a point.
/// Returns false if the point is outside the triangles.
/// \param a_pt Location that is interpolated to.
/// \param a_idxs Vector of triangle point indices filled in by this method.
/// \param a_wts Vector of triangle point weights filled in by this method.
/// \return false if a_pt is not inside of any triangle
//------------------------------------------------------------------------------
bool InterpLinearExtrapIdw::InterpWeights(const Pt3d& a_pt, VecInt& a_idxs, VecDbl& a_wts)
{
  return m_linear->InterpWeights(a_pt, a_idxs, a_wts);
} // InterpLinearExtrapIdw::InterpWeights
//------------------------------------------------------------------------------
/// \brief Set the constant extrapolation value
/// \param a_val The value assigned to extrapolated points
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::SetExtrapVal(double a_val)
{
  m_linear->SetExtrapVal(a_val);
} // InterpLinearExtrapIdw::SetExtrapVal
//------------------------------------------------------------------------------
/// \brief Set the truncation values for the interpolation and turn on
/// truncation.
/// \param a_sMax The maximum value for truncation.
/// \param a_sMin The minimum value for truncation.
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::SetTrunc(double a_sMax, double a_sMin)
{
  m_linear->SetTrunc(a_sMax, a_sMin);
} // InterpLinearExtrapIdw::SetTrunc
//------------------------------------------------------------------------------
/// \brief Set the class to use the Clough Tocher interpolation method. This is
/// a legacy feature from GMS. Frankly it is so similar to linear that there is
/// really no reason to use it but it is here because users complain if features
/// are removed.
/// \param a_ True/False to indicate if CT should be used.
/// \param a_prog Progress bar to give users feed back on the set up process
/// of CT. If you have a really large set of triangles this may take some time.
//------------------------------------------------------------------------------
void InterpLinearExtrapIdw::SetUseCloughTocher(bool a_, BSHP<Observer> a_prog)
{
  m_linear->SetUseCloughTocher(a_, a_prog);
} // InterpLinearExtrapIdw::SetUseCloughTocher
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
void InterpLinearExtrapIdw::SetUseNatNeigh(bool a_,
                                      int a_nodalFunc,
                                      int a_ndFuncOpt,
                                      int a_ndFuncNumNearestPts,
                                      bool a_blendWeights,
                                      BSHP<Observer> a_prog)
{
  m_linear->SetUseNatNeigh(a_, a_nodalFunc, a_ndFuncOpt, a_ndFuncNumNearestPts,
                           a_blendWeights, a_prog);
} // InterpLinearExtrapIdw::SetUseNatNeigh
//------------------------------------------------------------------------------
/// \brief Write the internals to a string.
/// \return The string.
//------------------------------------------------------------------------------
std::string InterpLinearExtrapIdw::ToString() const
{
  return m_linear->ToString();
} // InterpLinearExtrapIdw::ToString

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/interpolate/InterpLinearExtrapIdw.t.h>


// namespace xms {
using namespace xms;


////////////////////////////////////////////////////////////////////////////////
/// \class InterpLinearUnitTests
/// \brief tester for the InterpLinearUnitTests class
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief tests doing interpolation to point outside the triangles
//------------------------------------------------------------------------------
void InterpLinearExtrapIdwUnitTests::testOutsideInterpolatePoint()
{
  BSHP<InterpLinear> interpolator = InterpLinearExtrapIdw::New();
  BSHP<VecPt3d> pts(new VecPt3d());
  *pts = {{0, 0, 0}, {10, 0, 1}, {10, 10, 2}, {0, 10, 3}};
  BSHP<VecInt> t(new VecInt());
  *t = {0, 1, 3, 1, 2, 3};

  interpolator->SetPtsTris(pts, t);

  Pt3d pt(0, 25, 0);

  float val = interpolator->InterpToPt(pt);
  TS_ASSERT_DELTA(2.70349288, val, FLT_EPSILON);
} // InterpLinearExtrapIdwUnitTests::testOutsideInterpolatePoint
//------------------------------------------------------------------------------
/// \brief tests doing interpolation to point inside the triangles
//------------------------------------------------------------------------------
void InterpLinearExtrapIdwUnitTests::testInsideInterpolatePoint()
{
  BSHP<InterpLinear> interpolator = InterpLinearExtrapIdw::New();
  BSHP<VecPt3d> pts(new VecPt3d());
  *pts = {{0, 0, 0}, {10, 0, 1}, {10, 10, 2}, {0, 10, 3}};
  BSHP<VecInt> t(new VecInt());
  *t = {0, 1, 3, 1, 2, 3};

  interpolator->SetPtsTris(pts, t);

  Pt3d pt(5, 5, 0);

  float val = interpolator->InterpToPt(pt);
  TS_ASSERT_DELTA(2.0, val, FLT_EPSILON);
} // InterpLinearExtrapIdwUnitTests::testInsideInterpolatePoint
//------------------------------------------------------------------------------
/// \brief tests doing interpolation to point inside the triangles
//------------------------------------------------------------------------------
void InterpLinearExtrapIdwUnitTests::testOneEdgeInterpolatePoint()
{
  BSHP<InterpLinear> interpolator = InterpLinearExtrapIdw::New();
  BSHP<VecPt3d> pts(new VecPt3d());
  *pts = {{0, 0, 0}, {10, 0, 1}, {10, 10, 2}, {0, 10, 3}};
  BSHP<VecInt> t(new VecInt());
  *t = {0, 1, 3, 1, 2, 3};

  interpolator->SetPtsTris(pts, t);

  Pt3d pt(10, 10, 0);

  float val = interpolator->InterpToPt(pt);
  TS_ASSERT_DELTA(2.0, val, FLT_EPSILON);
} // InterpLinearExtrapIdwUnitTests::testOneEdgeInterpolatePoint
//} // namespace xms
#endif // CXX_TEST