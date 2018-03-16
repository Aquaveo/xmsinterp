#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup triangulate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

#include <xmscore/stl/vector.h> // for VecInt
#include <xmsinterp/triangulate/TrTriangulator.h>
#include <xmscore/misc/boost_defines.h> // BSHP

//----- Namespace declaration --------------------------------------------------

namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Classes / Structs ------------------------------------------------------
class Observer;

////////////////////////////////////////////////////////////////////////////////
class TrTriangulatorPoints : public TrTriangulator
{
public:
  TrTriangulatorPoints(const VecPt3d& a_pts, VecInt& a_tris, VecInt2d* a_trisAdjToPts = nullptr);
  virtual ~TrTriangulatorPoints();

  virtual void SetObserver(BSHP<Observer> a_);

  virtual int GetID() const override;
  virtual Pt3d GetLocation() const override;
  virtual int GetNPoints() const override;
  virtual void IncrementPoint() override;

  virtual void PrepareToReceiveTriangles() override;
  virtual void FinalizeTriangulation() override;
  virtual void ReceiveTriangle(int a_id1, int a_id2, int a_id3) override;

private:
  void UpdateAreaTolerance();

  const VecPt3d& m_pts;      ///< The points
  int m_idx;                 ///< Current point index
  VecInt& m_tris;            ///< The triangles as 0-based indices into m_pts
  VecInt2d* m_trisAdjToPts;  ///< Triangles adjacent to points
  double m_areaTol;          ///< tolerance for area to be greater than 0.0
  BSHP<Observer> m_observer; ///< Observer
};                           // class TrTriangulatorPoints

} // namespace
