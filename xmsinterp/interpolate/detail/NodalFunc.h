#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate_detail
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <vector>
#include <xmscore/points/ptsfwd.h>    // for Pt3d
#include <xmscore/misc/base_macros.h> // XM_DISALLOW_COPY_AND_ASSIGN
#include <xmscore/misc/boost_defines.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
class GmPtSearch;
class InterpNatNeigh;
class Observer;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------
/// \brief Class to compute gradient plane and quadratic nodal functions for
/// interpolation.
/// \see NodalFuncImpl
class NodalFunc
{
public:
  static BSHP<NodalFunc> New(int a_type,
                             bool a_2d,
                             boost::shared_ptr<GmPtSearch> a_ptSearch,
                             const std::vector<Pt3d>& a_pts,
                             const std::vector<float>& a_scalar,
                             int a_nNearest,
                             bool a_quad_oct,
                             double a_power,
                             bool a_modifiedShepardWeights,
                             boost::shared_ptr<Observer> a_p,
                             InterpNatNeigh* a_natNeigh);
  virtual ~NodalFunc();

  /// \cond
  virtual double ScalarFromPtIdx(int a_ptIdx, const Pt3d& a_loc) const = 0;
  virtual void GradientFromPtIdx(int a_ptIdx, Pt3d& a_grad) const = 0;
  virtual void ComputeNodalFuncs() = 0;
  virtual std::string ToString() const = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(NodalFunc);
  /// \endcond

protected:
  NodalFunc();
};
//----- Function prototypes ----------------------------------------------------

} // namespace xms
