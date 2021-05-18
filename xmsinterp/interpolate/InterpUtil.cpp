//------------------------------------------------------------------------------
/// \file
/// \brief Utility functions called by interpolation code
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/interpolate/InterpUtil.h>

// 3. Standard library headers
#include <thread>

// 4. External library headers
#include <xmscore/math/math.h>
#include <xmscore/misc/boost_defines.h> // BSHP
#include <xmscore/misc/XmError.h> // XM_ENSURE_TRUE
#include <xmscore/misc/xmstype.h> // XM_NODATA
#include <xmscore/stl/vector.h> // VecInt, VecPt3d, etc.

// 5. Shared code headers

// 6. Non-shared code headers
#include <xmsinterp/interpolate/InterpIdw.h>

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------

//------------------------------------------------------------------------------
/// \brief Sets up matrices for nodal function calculations. Refactored out of
/// GMS.
/// \param xk x value of point
/// \param yk y value of point
/// \param fk scalar value of point
/// \param closest information on closest points
/// \param M matrix filled by function
/// \param VV matrix filled by fucntion
//------------------------------------------------------------------------------
void inNodalFuncSetUpMatrixAndVector(double xk,
                                     double yk,
                                     double fk,
                                     const std::vector<InterpPtInfo>& closest,
                                     double** M,
                                     double* VV)
{
  int i, j, n;
  double v1[6], v2[6]; // fg_vv is made up of two summations
  double wi, xi, yi, fi;

  n = (int)closest.size();
  // initialize matrix and vector.
  for (i = 1; i <= 5; i++)
  {
    v1[i] = 0.0;
    v2[i] = 0.0;
    VV[i - 1] = 0.0;
    for (j = 1; j <= 5; j++)
      M[i - 1][j - 1] = 0.0;
  }

  for (i = 0; i < n; i++)
  { // weights will all be zero when i=n-1
    wi = closest[i].m_weight;
    xi = closest[i].m_loc.x - xk;
    yi = closest[i].m_loc.y - yk;
    fi = closest[i].m_scalar;

    // set up matrix in banded form.
    M[0][0] = M[0][0] + wi * xi * xi;
    M[0][1] = M[0][1] + wi * xi * yi;
    M[0][2] = M[0][2] + wi * xi * xi * xi;
    M[0][3] = M[0][3] + wi * xi * xi * yi;
    M[0][4] = M[0][4] + wi * xi * yi * yi;
    v1[1] = v1[1] + wi * xi * fi;
    v2[1] = v2[1] + wi * xi;

    M[1][0] = M[1][0] + wi * yi * yi;
    M[1][1] = M[1][1] + wi * xi * xi * yi;
    M[1][2] = M[1][2] + wi * xi * yi * yi;
    M[1][3] = M[1][3] + wi * yi * yi * yi;
    v1[2] = v1[2] + wi * yi * fi;
    v2[2] = v2[2] + wi * yi;

    M[2][0] = M[2][0] + wi * xi * xi * xi * xi;
    M[2][1] = M[2][1] + wi * xi * xi * xi * yi;
    M[2][2] = M[2][2] + wi * xi * xi * yi * yi;
    v1[3] = v1[3] + wi * xi * xi * fi;
    v2[3] = v2[3] + wi * xi * xi;

    M[3][0] = M[3][0] + wi * xi * xi * yi * yi;
    M[3][1] = M[3][1] + wi * xi * yi * yi * yi;
    v1[4] = v1[4] + wi * xi * yi * fi;
    v2[4] = v2[4] + wi * xi * yi;

    M[4][0] = M[4][0] + wi * yi * yi * yi * yi;
    v1[5] = v1[5] + wi * yi * yi * fi;
    v2[5] = v2[5] + wi * yi * yi;
  }

  for (i = 1; i <= 5; i++)
    VV[i - 1] = v1[i] - fk * v2[i];

} // inNodalFuncSetUpMatrixAndVector
//------------------------------------------------------------------------------
/// \brief Sets up matrices for nodal function calculations. Refactored out of
/// GMS.
/// \param xk x value of point
/// \param yk y value of point
/// \param zk z value of point
/// \param fk scalar value of point
/// \param closest information on closest points
/// \param M matrix filled by function
/// \param vv matrix filled by function
//------------------------------------------------------------------------------
void inNodalFuncSetUpMatrixAndVector3d(double xk,
                                       double yk,
                                       double zk,
                                       double fk,
                                       const std::vector<InterpPtInfo>& closest,
                                       double** M,
                                       double* vv)
{
  int i, j;
  double v1[10], v2[10]; // vv is made up of two summations
  double wi, xi, yi, zi, fi;
  double xyi, xzi, yzi, x2i, y2i, z2i, wXi;
  double** m = M;

  // initialize to matrix and vector.
  // NOTE: arrays are defined from 1,not 0: this is so I'm compatible with
  // the function solve_banded_matrix in matrices.c
  for (i = 1; i < 10; i++)
  {
    v1[i] = 0.0;
    v2[i] = 0.0;
    vv[i - 1] = 0.0;
    for (j = 1; j < 10; j++)
      m[i - 1][j - 1] = 0.0;
  }

  for (i = 0; i < (int)closest.size(); ++i)
  {
    wi = closest[i].m_weight;
    xi = closest[i].m_loc.x - xk;
    yi = closest[i].m_loc.y - yk;
    zi = closest[i].m_loc.z - zk;
    fi = closest[i].m_scalar;

    // set up matrix in banded form.
    xyi = xi * yi;
    xzi = xi * zi;
    yzi = yi * zi;
    x2i = xi * xi;
    y2i = yi * yi;
    z2i = zi * zi;

    wXi = wi * xi;
    m[0][0] += (wXi * xi);
    m[0][1] += (wXi * yi);
    m[0][2] += (wXi * zi);
    m[0][3] += (wXi * xyi);
    m[0][4] += (wXi * xzi);
    m[0][5] += (wXi * yzi);
    m[0][6] += (wXi * x2i);
    m[0][7] += (wXi * y2i);
    m[0][8] += (wXi * z2i);
    v1[1] += (wXi * fi);
    v2[1] += wXi;

    wXi = wi * yi;
    m[1][0] += (wXi * yi);
    m[1][1] += (wXi * zi);
    m[1][2] += (wXi * xyi);
    m[1][3] += (wXi * xzi);
    m[1][4] += (wXi * yzi);
    m[1][5] += (wXi * x2i);
    m[1][6] += (wXi * y2i);
    m[1][7] += (wXi * z2i);
    v1[2] += (wXi * fi);
    v2[2] += wXi;

    wXi = wi * zi;
    m[2][0] += (wXi * zi);
    m[2][1] += (wXi * xyi);
    m[2][2] += (wXi * xzi);
    m[2][3] += (wXi * yzi);
    m[2][4] += (wXi * x2i);
    m[2][5] += (wXi * y2i);
    m[2][6] += (wXi * z2i);
    v1[3] += (wXi * fi);
    v2[3] += wXi;

    wXi = wi * xyi;
    m[3][0] += (wXi * xyi);
    m[3][1] += (wXi * xzi);
    m[3][2] += (wXi * yzi);
    m[3][3] += (wXi * x2i);
    m[3][4] += (wXi * y2i);
    m[3][5] += (wXi * z2i);
    v1[4] += (wXi * fi);
    v2[4] += wXi;

    wXi = wi * xzi;
    m[4][0] += (wXi * xzi);
    m[4][1] += (wXi * yzi);
    m[4][2] += (wXi * x2i);
    m[4][3] += (wXi * y2i);
    m[4][4] += (wXi * z2i);
    v1[5] += (wXi * fi);
    v2[5] += wXi;

    wXi = wi * yzi;
    m[5][0] += (wXi * yzi);
    m[5][1] += (wXi * x2i);
    m[5][2] += (wXi * y2i);
    m[5][3] += (wXi * z2i);
    v1[6] += (wXi * fi);
    v2[6] += wXi;

    wXi = wi * x2i;
    m[6][0] += (wXi * x2i);
    m[6][1] += (wXi * y2i);
    m[6][2] += (wXi * z2i);
    v1[7] += (wXi * fi);
    v2[7] += wXi;

    wXi = wi * y2i;
    m[7][0] += (wXi * y2i);
    m[7][1] += (wXi * z2i);
    v1[8] += (wXi * fi);
    v2[8] += wXi;

    wXi = wi * z2i;
    m[8][0] += (wXi * z2i);
    v1[9] += (wXi * fi);
    v2[9] += wXi;
  }

  for (i = 1; i <= 9; i++)
    vv[i - 1] = v1[i] - fk * v2[i];
} // inNodalFuncSetUpMatrixAndVector3d
//------------------------------------------------------------------------------
/// \brief Computes the distance squared between the point "a_pt" and the other
/// points. The other points are defined by looking up the location in "a_ptLoc"
/// using the index stored in "a_ptIdxs". The "a_d2" vector is then filled
/// with the distance squared. If "a_2d" is true then we only do the x,y
/// distance.
/// \param a_pt Location of the interpolation point.
/// \param a_ptIdxs Indices of the points in the a_ptLoc vector. If
/// a_ptIdxs.empty(), then use all a_ptLoc.
/// \param a_ptLoc Vector of point locations
/// \param a_2d Calculate 2D distance squared verses 3D distance squared
/// \param a_d2 Vector of distances squared that is filled in by this function
//------------------------------------------------------------------------------
void inDistanceSquared(const Pt3d& a_pt,
                       const std::vector<int>& a_ptIdxs,
                       const std::vector<Pt3d>& a_ptLoc,
                       bool a_2d,
                       std::vector<double>& a_d2)
{
  bool useAll = a_ptIdxs.empty();
  size_t nPtIdxs = useAll ? a_ptLoc.size() : a_ptIdxs.size();
  a_d2.resize(0);
  a_d2.reserve(nPtIdxs);
  size_t j;
  double d2;
  for (size_t i = 0; i < nPtIdxs; ++i)
  {
    j = useAll ? i : a_ptIdxs[i];
    d2 = xms::sqr(a_pt.x - a_ptLoc[j].x) + xms::sqr(a_pt.y - a_ptLoc[j].y);
    if (!a_2d)
      d2 += xms::sqr(a_pt.z - a_ptLoc[j].z);
    a_d2.push_back(d2);
  }
} // inDistanceSquared
//------------------------------------------------------------------------------
/// \brief Computes the idw weights that would be assigned to points associated
/// with the distance squared that is passed in: "a_d2". The function can handle
/// any power ("a_power") associated with the idw calculation and there is an
/// option for the Modified Shepard Weights as well (see the reference in the
/// code).
/// \param a_distSquare Vectors of distances squared.
/// \param a_power The exponent used in the inverse distance weighting
/// function. Normally this would be 2 and thus the distances squared could be
/// used. However if the power is not 2 then the distance must be calculated
/// and the exponent applied.
/// \param a_modifiedShepardWeights Specifies a method for calculating the
/// normalized weights from the distances squared. See the code below for the
/// reference to the journal article.
/// \param a_w Vector of weights filled in by this function. This will be the
/// same size as a_distSquare and each value will correspond to the respective
/// distance squared.
//------------------------------------------------------------------------------
void inIdwWeights(const std::vector<double>& a_distSquare,
                  double a_power,
                  bool a_modifiedShepardWeights,
                  std::vector<double>& a_w)
{
  std::vector<double> d2(a_distSquare);
  double distSum(0), dExp(a_power / 2), r, d;
  a_w.resize(0);
  a_w.assign(d2.size(), 0);
  r = sqrt(*std::max_element(d2.begin(), d2.end()));
  for (size_t i = 0; i < d2.size(); ++i)
  { // this is basically at the same location as our point
    if (d2[i] < 1e-20)
      d2[i] = 1e10;
  }
  for (size_t i = 0; i < d2.size(); ++i)
  {
    if (a_modifiedShepardWeights)
    { // Franke, R., and Neilson, G. 1980. Smooth interpolation of large sets
      // of scattered data. International Journal of Numerical Methods
      // in Engineering 15, 1691-1704.

      // r is the distance to the furthest point, d is the distance to the
      // current point
      d = sqrt(d2[i]);
      d2[i] = xms::sqr((r - d) / (r * d));
    }
    else
    { // classic weighting function
      if (dExp == 1)
      { // 1 over distance squared
        d2[i] = 1 / d2[i];
      }
      else
      { // if the m_power variable is not 2 then we have to use the "pow" function
        d2[i] = 1 / pow(d2[i], dExp);
      }
    }
    distSum += d2[i];
  }
  if (0.0 == distSum)
  {
    d2 = a_distSquare;
    for (size_t i = 0; i < d2.size(); ++i)
      distSum += d2[i];
  }
  for (size_t i = 0; i < d2.size(); ++i)
  {
    a_w[i] = d2[i] / distSum;
  }
} // inIdwWeights
//------------------------------------------------------------------------------
/// \brief Check to see if the all of the values in the scalars array are the
/// same. It will also take into account the activity that is passed in.
/// \param a_scalars vector of scalar values
/// \param a_act Activity array associated with the scalars
/// \return true if all values are equal.
//------------------------------------------------------------------------------
bool inAllScalarsEqual(const std::vector<float>& a_scalars, const DynBitset& a_act)
{
  bool allScalarsSame(true);
  if (a_scalars.empty())
    return allScalarsSame;

  if (a_act.empty() || a_act.size() != a_scalars.size())
  {
    float f = a_scalars[0];
    for (size_t i = 1; allScalarsSame && i < a_scalars.size(); ++i)
    {
      if (f != a_scalars[i])
        allScalarsSame = false;
    }
    return allScalarsSame;
  }

  // get first active value
  size_t idx(0);
  if (!a_act[0])
  {
    for (size_t i = 1; idx == 0 && i < a_act.size(); ++i)
    {
      if (a_act[i])
        idx = i;
    }
  }

  float f = a_scalars[idx];
  for (size_t i = idx + 1; allScalarsSame && i < a_scalars.size(); ++i)
  {
    if (a_act[i] && f != a_scalars[i])
      allScalarsSame = false;
  }
  return allScalarsSame;
} // inAllScalarsEqual
//------------------------------------------------------------------------------
/// \brief Returns an array of values given mesh information.
/// \param a_pts[in] vector of mesh points
/// \param a_minPt[in] Lower left corner of the lower left raster cell
/// \param a_size[in] Size of the raster cells in the X and Y directions
/// \param a_numRows[in] Number of rows in the raster
/// \param a_numCols[in] Number of cols in the raster
/// \param a_activity[in] This is the size of the raster return values and is
///  used to determine whether to set raster cells to active and interp values to each cell
/// \return The interpolated values at the given points
//------------------------------------------------------------------------------
std::vector<float> inGenerateRasterIdw(const std::vector<Pt3d>& a_pts,
                                       const Pt3d& a_minPt,
                                       const Pt3d& a_size,
                                       const int a_numRows,
                                       const int a_numCols,
                                       const float a_noDataVal,
                                       const std::vector<int>& a_activity)
{
  int numThreads = (int)std::thread::hardware_concurrency();
  if (numThreads == 0)
    numThreads = 8;
  if (a_numRows < numThreads)
    numThreads = 1;
  BSHP<InterpIdw> idw = InterpIdw::New();
  BSHP<VecInt> idwTris(new xms::VecInt()); // not used by idw
  // Create the IDW interpolator
  BSHP<VecPt3d> idwPts(new xms::VecPt3d(a_pts));
  idw->SetPtsTris(idwPts, idwTris);
  static int numPts = 16;
  static bool useQuadrants = false;
  idw->SetSearchOpts(numPts, useQuadrants);
  // Determine the vector of points for interpolation
  VecFlt vals, retVals;
  VecPt3d rasterPts;
  rasterPts.reserve(a_numRows * a_numCols);
  auto halfSize = a_size / 2.0;
  const bool useActivity = !a_activity.empty();
  if (useActivity)
  {
    XM_ENSURE_TRUE(a_activity.size() == a_numRows * a_numCols, retVals);
  }
  int count = 0;
  for (int y = 0; y < a_numRows; ++y)
  {
    for (int x = 0; x < a_numCols; ++x)
    {
      if (!useActivity || (useActivity && a_activity[count]))
      {
        const Pt2d xy(x, y);
        rasterPts.push_back(a_minPt + xy * a_size + halfSize);
      }
      ++count;
    }
  }
  idw->InterpToPts(rasterPts, vals);
  if (useActivity)
  {
    count = 0;
    int activeCount = 0;
    retVals.reserve(a_numRows * a_numCols);
    for (int y = 0; y < a_numRows; ++y)
    {
      for (int x = 0; x < a_numCols; ++x)
      {
        if (a_activity[count])
        {
          retVals.push_back(vals[activeCount]);
          ++activeCount;
        }
        else
          retVals.push_back(a_noDataVal);
        ++count;
      }
    }
  }
  else
  {
    retVals = vals;
  }
  return retVals;
} // inGenerateRasterIdw

} // namespace xms
#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/interpolate/InterpUtil.t.h>

#include <xmscore/testing/TestTools.h>

// namespace xms {
using namespace xms;

////////////////////////////////////////////////////////////////////////////////
/// \class InterpUtilUnitTests
/// \brief tester for the InterpUtil functions
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// \brief tests inGenerateRasterIdw
//------------------------------------------------------------------------------
void InterpUtilUnitTests::testGenerateRasterIdw()
{
  VecPt3d pts = {{0.0, 0.0, 1.0}, {0.0, 1.0, 2.0}, {1.0, 0.0, 3.0}};
  auto vals = inGenerateRasterIdw(pts, Pt3d(-1.0, -1.0, 0.0), Pt3d(1.0, 1.0, 0.0), 4, 4, XM_NODATA,
    VecInt());
  VecFlt expectedVals = {1.00f, 2.00f, 2.94f, 2.94f, 1.50f, 2.00f, 3.00f, 3.00f,
                         1.97f, 2.00f, 2.50f, 2.87f, 1.97f, 2.00f, 2.13f, 2.50f};
  TS_ASSERT_DELTA_VEC(vals, expectedVals, 0.01);
  VecInt activity = {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0};
  vals = inGenerateRasterIdw(pts, Pt3d(-1.0, -1.0, 0.0), Pt3d(1.0, 1.0, 0.0), 4, 4, XM_NODATA,
    activity);
  expectedVals = {XM_NODATA, XM_NODATA, XM_NODATA, XM_NODATA, 1.50, 2.00,
                  3.00,      3.00,      1.97f,      2.00,      2.50, 2.87f,
                  XM_NODATA, XM_NODATA, XM_NODATA, XM_NODATA};
  TS_ASSERT_DELTA_VEC(vals, expectedVals, 0.01);
} // InterpUtilUnitTests::testGenerateRasterIdw
#endif // CXX_TEST