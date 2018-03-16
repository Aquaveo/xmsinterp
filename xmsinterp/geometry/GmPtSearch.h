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
#include <xmscore/misc/boost_defines.h>
#include <xmscore/points/ptsfwd.h>
#include <xmscore/stl/vector.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
/// \brief Spatial index for searching points
/// \see GmPtSearchImpl
class GmPtSearch
{
public:
  static BSHP<GmPtSearch> New(bool a_2dSearch);
  virtual ~GmPtSearch();

  /// \cond
  virtual void PtsToSearch(BSHP<VecPt3d> a_pts) = 0;

  virtual void VectorThatGrowsToSearch(BSHP<VecPt3d> a) = 0;
  virtual bool AddPtToVectorIfUnique(const Pt3d& a_, double a_tol, int& a_ptIdx) = 0;

  virtual void NearestPtsToPt(const Pt3d& a_pt,
                              int a_numPtsToFind,
                              bool a_quad_oct_Search,
                              VecInt& a_nearest) const = 0;

  virtual void NearestPtsToPtInRtree(int a_ptIdx,
                                     const Pt3d& a_pt,
                                     int a_numPtsToFind,
                                     bool a_quad_oct_Search,
                                     VecInt& a_nearest) const = 0;

  virtual bool PtInRTree(const Pt3d& a_pt, const double a_tol) = 0;

  virtual void PtsWithinDistanceToPtInRtree(int a_ptIdx,
                                            const Pt3d& a_pt,
                                            double a_dist,
                                            VecInt& a_nearest) const = 0;

  virtual void SetActivity(boost::dynamic_bitset<size_t>& a_activity) = 0;
  virtual boost::dynamic_bitset<size_t> GetActivity() = 0;

  virtual const BSHP<VecPt3d> GetPointsPtr() const = 0;
  virtual bool Is2D() const = 0;
  virtual std::string ToString() const = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(GmPtSearch);
  /// \endcond
protected:
  GmPtSearch();
};
//----- Function prototypes ----------------------------------------------------
}