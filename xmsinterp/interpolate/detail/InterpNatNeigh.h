#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <vector>
#include <xmscore/misc/base_macros.h>
#include <xmscore/misc/boost_defines.h>
#include <xmscore/points/ptsfwd.h>
#include <xmscore/stl/vector.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
class NodalFunc;
class GmTriSearch;
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
/// \brief Class that performs natural neighbor interpolation
/// \see InterpNatNeighImpl
class InterpNatNeigh
{
public:
  static boost::shared_ptr<InterpNatNeigh> New(const std::vector<Pt3d>& a_pts,
                                               const std::vector<int>& a_tris,
                                               const std::vector<float>& a_scalar,
                                               GmTriSearch* a_triSearch);
  virtual ~InterpNatNeigh();

  /// \cond
  virtual float InterpToPt(const Pt3d& a_pt) = 0;
  virtual void GetNeighbors(int a_ptIdx, std::vector<int>& a_neigh) = 0;
  virtual void SetNodalFunc(boost::shared_ptr<NodalFunc> a_) = 0;
  virtual void RecalcNodalFunc() = 0;
  virtual void SetBlendWeights(bool a_) = 0;
  virtual std::string ToString() = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(InterpNatNeigh);
  /// \endcond

protected:
  InterpNatNeigh();
};
//----- Function prototypes ----------------------------------------------------
}