//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 1. Precompiled header

// 2. My own header
#include <xmsinterp/geometry/GmPolyLinePtRedistributer.h>

// 3. Standard library headers
#include <map>

// 4. External library headers

// 5. Shared code headers
#include <xmscore/math/math.h>
#include <xmscore/misc/XmError.h>

// 6. Non-shared code headers

//----- Forward declarations ---------------------------------------------------

//----- External globals -------------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------

//----- Internal functions -----------------------------------------------------

//----- Class / Function definitions -------------------------------------------
/// \brief Implementation of GmPolyLinePtRedistributer
class GmPolyLinePtRedistributerImpl : public GmPolyLinePtRedistributer
{
public:
  GmPolyLinePtRedistributerImpl() {}

  virtual VecPt3d Redistribute(const VecPt3d& a_input, double a_size) override;

  double PolyLineLengths(const VecPt3d& a_pts, VecDbl& a_lengths);
  VecPt3d RedistPolyLineWithNumSeg(const VecPt3d& a_pts,
                                   double a_totalLength,
                                   const VecDbl& a_lengths,
                                   int a_nSeg);
};

//------------------------------------------------------------------------------
/// \brief Creates an instance of this class
/// \return GmPolyLinePtRedistributer.
//------------------------------------------------------------------------------
BSHP<GmPolyLinePtRedistributer> GmPolyLinePtRedistributer::New()
{
  BSHP<GmPolyLinePtRedistributer> ret(new GmPolyLinePtRedistributerImpl);
  return ret;
} // MePolyRedistributePts::New
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
GmPolyLinePtRedistributer::GmPolyLinePtRedistributer()
{
}
//------------------------------------------------------------------------------
/// \brief
//------------------------------------------------------------------------------
GmPolyLinePtRedistributer::~GmPolyLinePtRedistributer()
{
}

//------------------------------------------------------------------------------
/// \brief Redistributes the distance between points on a polyline based on the
/// a_size value provided to the method
/// \param[in] a_polyLines Point locations defining the polylines.
/// \param[in] a_size Target distance between points on the polyline.
/// \return 2d vector of redistributed point locations.
//------------------------------------------------------------------------------
VecPt3d GmPolyLinePtRedistributerImpl::Redistribute(const VecPt3d& a_polyLines, double a_size)
{
  VecPt3d ret;
  XM_ENSURE_TRUE(!a_polyLines.empty(), ret);

  VecDbl lengths;
  double length = PolyLineLengths(a_polyLines, lengths);
  int nSeg = (int)((length / a_size) + .5);
  return RedistPolyLineWithNumSeg(a_polyLines, length, lengths, nSeg);
} // GmPolyLinePtRedistributerImpl::Redistribute
//------------------------------------------------------------------------------
/// \brief Calculates the total length (xy) of a polyline as well as the length
/// of each segment.
/// \param[in] a_pts The point locations of the polyline.
/// \param[out] a_lengths The length of each segment of the polyline.
/// \return The length
//------------------------------------------------------------------------------
double GmPolyLinePtRedistributerImpl::PolyLineLengths(const VecPt3d& a_pts, VecDbl& a_lengths)
{
  double l(0);
  a_lengths.resize(a_pts.size());
  for (size_t i = 1; i < a_pts.size(); ++i)
  {
    const Pt3d &p0(a_pts[i - 1]), &p1(a_pts[i]);
    a_lengths[i] = Mdist(p0.x, p0.y, p1.x, p1.y);
    l += a_lengths[i];
  }
  return l;
} // GmPolyLinePtRedistributerImpl::PolyLineLengths
//------------------------------------------------------------------------------
/// \brief Calculates the total length (xy) of a polyline as well as the length
/// of each segment.
/// \param[in] a_pts The point locations of the polyline.
/// \param[in] a_totalLength The total length of the polyline.
/// \param[in] a_lengths The length of each segment of the polyline.
/// \param[in] a_nSeg The number of segments for the new polyline.
/// \return The new polyline
//------------------------------------------------------------------------------
VecPt3d GmPolyLinePtRedistributerImpl::RedistPolyLineWithNumSeg(const VecPt3d& a_pts,
                                                                double a_totalLength,
                                                                const VecDbl& a_lengths,
                                                                int a_nSeg)
{
  VecPt3d ret;
  std::map<double, size_t> tVals;
  double runningLen(0), t;
  for (size_t i = 0; i < a_lengths.size(); ++i)
  {
    runningLen += a_lengths[i];
    t = runningLen / a_totalLength;
    tVals.insert(std::make_pair(t, i));
  }
  VecDbl targetTvals(a_nSeg);
  for (size_t i = 0; i < targetTvals.size(); ++i)
  {
    targetTvals[i] = (i + 1) * (1.0 / a_nSeg);
  }

  ret.reserve(a_nSeg + 1);
  ret.push_back(a_pts.front());
  for (size_t i = 0; i < targetTvals.size(); ++i)
  {
    auto it = tVals.lower_bound(targetTvals[i]);
    if (it != tVals.end())
    {
      size_t seg1 = it->second, seg0(0);
      double t1 = it->first, t0(0);
      if (it != tVals.begin())
      {
        it--;
        t0 = it->first;
        seg0 = it->second;
      }
      // convert polyline tval to segment tval
      double t = (targetTvals[i] - t0) / (t1 - t0);
      // calculate the point
      const Pt3d &p0(a_pts[seg0]), &p1(a_pts[seg1]);
      Pt3d pt;
      pt.x = p0.x + t * (p1.x - p0.x);
      pt.y = p0.y + t * (p1.y - p0.y);
      ret.push_back(pt);
    }
  }
  if (ret.size() < 2)
    ret.push_back(a_pts.back());
  return ret;
} // GmPolyLinePtRedistributerImpl::RedistPolyLineWithNumSeg

} // namespace xms

#ifdef CXX_TEST
////////////////////////////////////////////////////////////////////////////////

#include <xmsinterp/geometry/GmPolyLinePtRedistributer.t.h>

#include <xmscore/testing/TestTools.h>
#include <xmsinterp/geometry/geoms.h>

//------------------------------------------------------------------------------
/// \brief tests creating the class
//------------------------------------------------------------------------------
void GmPolyLinePtRedistributerUnitTests::testCreateClass()
{
  BSHP<xms::GmPolyLinePtRedistributer> b = xms::GmPolyLinePtRedistributer::New();
  TS_ASSERT(b);
} // GmPolyLinePtRedistributerUnitTests::testCreateClass
//------------------------------------------------------------------------------
/// \brief tests redistributing 1 line segment
//------------------------------------------------------------------------------
void GmPolyLinePtRedistributerUnitTests::testRedistribute()
{
  xms::VecPt3d pts{{0, 0, 0}, {10, 0, 0}};
  double l(2);
  xms::VecPt3d basePts{{0, 0, 0}, {2, 0, 0}, {4, 0, 0}, {6, 0, 0}, {8, 0, 0}, {10, 0, 0}};
  xms::GmPolyLinePtRedistributerImpl r;
  xms::VecPt3d outPts = r.Redistribute(pts, l);
  TS_ASSERT_DELTA_VECPT3D(basePts, outPts, 1e-9);
} // GmPolyLinePtRedistributerUnitTests::testRedistribute
//------------------------------------------------------------------------------
/// \brief tests redistributing 2 line segments
//------------------------------------------------------------------------------
void GmPolyLinePtRedistributerUnitTests::testRedistribute1()
{
  xms::VecPt3d pts{{0, 0, 0}, {10, 0, 0}, {10, 10, 0}};
  double l(5);
  xms::VecPt3d basePts = {{0, 0, 0}, {5, 0, 0}, {10, 0, 0}, {10, 5, 0}, {10, 10, 0}};
  xms::GmPolyLinePtRedistributerImpl r;
  xms::VecPt3d outPts = r.Redistribute(pts, l);
  TS_ASSERT_DELTA_VECPT3D(basePts, outPts, 1e-9);
  l = 3.0;
  outPts = r.Redistribute(pts, l);
  basePts = {{0, 0, 0},     {2.85, 0, 0},  {5.71, 0, 0},  {8.57, 0, 0},
             {10, 1.43, 0}, {10, 4.29, 0}, {10, 7.14, 0}, {10, 10, 0}};
  TS_ASSERT_DELTA_VECPT3D(basePts, outPts, 1e-2);
  l = 20.0;
  outPts = r.Redistribute(pts, l);
  basePts = {{0, 0, 0}, {10, 10, 0}};
  TS_ASSERT_DELTA_VECPT3D(basePts, outPts, 1e-9);
  l = 41.0;
  outPts = r.Redistribute(pts, l);
  TS_ASSERT_DELTA_VECPT3D(basePts, outPts, 1e-9);
} // GmPolyLinePtRedistributerUnitTests::testRedistribute1
#endif
