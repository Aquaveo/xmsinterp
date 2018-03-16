#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard library headers
#include <iosfwd> // forward declaration of ostream

// 4. External library headers
#include <boost/serialization/assume_abstract.hpp>

// 5. Shared code headers
#include <xmscore/points/ptsfwd.h>
#include <xmscore/misc/base_macros.h>   // for XM_DISALLOW_COPY_AND_ASSIGN
#include <xmscore/misc/boost_defines.h> // for BSHP
#include <xmscore/stl/vector.h>         // for VecPt3d
#include <xmscore/stl/set.h>            // for SetInt

namespace boost
{
namespace serialization
{
class access;
}
}
namespace xms
{
////////////////////////////////////////////////////////////////////////////////
class TrTin
{
  /// \cond
  friend class boost::serialization::access;
  /// \endcond
public:
  static BSHP<TrTin> New();
  /// \cond

  // Setters
  virtual void SetPoints(BSHP<VecPt3d> a_pts) = 0;
  virtual void SetTriangles(BSHP<VecInt> a_tris) = 0;
  virtual void SetTrianglesAdjacentToPoints(BSHP<VecInt2d> a_trisAdjToPts) = 0;
  virtual void SetGeometry(BSHP<VecPt3d> a_pts,
                           BSHP<VecInt> a_tris,
                           BSHP<VecInt2d> a_trisAdjToPts) = 0;

  // Getters
  virtual VecPt3d& Points() = 0;
  virtual VecInt& Triangles() = 0;
  virtual VecInt2d& TrisAdjToPts() = 0; // Triangles adjacent to points

  virtual const VecPt3d& Points() const = 0;
  virtual const VecInt& Triangles() const = 0;
  virtual const VecInt2d& TrisAdjToPts() const = 0; // Triangles adjacent to points

  virtual BSHP<VecPt3d> PointsPtr() = 0;
  virtual BSHP<VecInt> TrianglesPtr() = 0;

  virtual int NumPoints() const = 0;
  virtual int NumTriangles() const = 0;

  // Non modifiers
  virtual bool TriangleFromEdge(int a_pt1,
                                int a_pt2,
                                int& a_tri,
                                int& a_localPt1,
                                int& a_localPt2) const = 0;
  virtual int TriangleAdjacentToEdge(int a_pt1, int a_pt2) const = 0;
  virtual int LocalIndex(int a_tri, int a_pt) const = 0;
  virtual int GlobalIndex(int a_triIdx, int a_localPt) const = 0;
  virtual bool VerticesAreAdjacent(int a_pt1, int a_pt2) const = 0;
  virtual int CommonEdgeIndex(int a_tri, int a_adjTri) const = 0;
  virtual int AdjacentTriangle(int a_triIdx, int a_edgeIdx) const = 0;
  virtual Pt3d TriangleCentroid(int a_tri) const = 0;
  virtual double TriangleArea(int a_tri) const = 0;
  // virtual int FirstBoundaryPoint () const = 0;
  virtual int NextBoundaryPoint(int a_point) const = 0;
  virtual int PreviousBoundaryPoint(int a_point) const = 0;
  virtual void GetBoundaryPoints(VecInt& a_boundaryPoints) const = 0;
  virtual void GetBoundaryPolys(VecInt2d& a_polys) const = 0;
  virtual bool GetExtents(Pt3d& a_mn, Pt3d& a_mx) const = 0;
  virtual void ExportTinFile(std::ostream& a_os) const = 0;

  // Modifiers
  virtual bool SwapEdge(int a_triA, int a_triB, bool a_checkAngle = true) = 0;
  virtual void DeleteTriangles(const SetInt& a_triangles) = 0;
  virtual void DeletePoints(const SetInt& a_points) = 0;
  virtual void OptimizeTriangulation() = 0;
  virtual void BuildTrisAdjToPts() = 0; // Triangles adjacent to points
  virtual void Clear() = 0;

  virtual std::string ToString() const = 0;
  virtual void FromString(const std::string&) = 0;
  template <typename Archive>
  void serialize(Archive& /*archive*/, const unsigned int /*version*/)
  {
  }
  /// \endcond

  virtual ~TrTin(); // moved from protected for boost serialize
protected:
  TrTin();

private:
  XM_DISALLOW_COPY_AND_ASSIGN(TrTin)

}; // class TrTin

//----- Global methods ---------------------------------------------------------

void trRenumberOnDelete(const SetInt& a_deleting, VecInt& a_vec);

} // namespace xms

BOOST_SERIALIZATION_ASSUME_ABSTRACT(xms::TrTin)
