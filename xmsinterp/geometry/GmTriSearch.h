#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup geometry
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <boost/dynamic_bitset.hpp>
#include <xmscore/misc/base_macros.h>
#include <xmscore/points/ptsfwd.h>
#include <xmscore/stl/vector.h>
#include <xmscore/misc/boost_defines.h>
#include <xmscore/misc/DynBitset.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
class Observer;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
/// \brief Spatial index for searching triangles
/// \see GmTriSearchImpl
class GmTriSearch
{
public:
  static BSHP<GmTriSearch> New();
  virtual ~GmTriSearch();

  /// \cond
  // setup
  virtual void TrisToSearch(BSHP<std::vector<Pt3d>> a_pts, BSHP<std::vector<int>> a_tris) = 0;
  virtual void SetPtActivity(DynBitset& a_activity) = 0;
  // bitset is number of triangles in length not numtri*3 like the tris array
  virtual void SetTriActivity(DynBitset& a_activity) = 0;
  virtual DynBitset GetPtActivity() = 0;
  // find triangle
  virtual int TriContainingPt(const Pt3d& a_pt) = 0;
  virtual void TriEnvelopsContainingPt(const Pt3d& a_pt, std::vector<int>& a_tris) = 0;
  virtual void TriEnvelopesOverlap(const Pt3d& a_pMin,
                                   const Pt3d& a_pMax,
                                   std::vector<int>& a_tris) = 0;
  // interpolation
  virtual bool InterpWeights(const Pt3d& a_pt,
                             std::vector<int>& a_idxs,
                             std::vector<double>& a_wts) = 0;
  virtual bool InterpWeightsTriangleIdx(const Pt3d& a_pt,
                                        int& a_triangleIdx,
                                        std::vector<int>& a_idxs,
                                        std::vector<double>& a_wts) = 0;

  virtual std::string ToString() const = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(GmTriSearch);
  /// \endcond

protected:
  GmTriSearch();
};
//----- Function prototypes ----------------------------------------------------
}