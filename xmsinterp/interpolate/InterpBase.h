#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <xmscore/misc/base_macros.h>
#include <xmscore/misc/boost_defines.h>
#include <xmscore/misc/DynBitset.h>
#include <xmscore/points/ptsfwd.h>
#include <xmscore/stl/vector.h>

//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \class InterpBase
/// \brief Base class for interpolation methods.
class InterpBase
{
public:
  /// \cond These are all documented in the implementations
  virtual void SetPtsTris(BSHP<VecPt3d> a_, BSHP<VecInt> a_tris) = 0;
  virtual void SetScalars(const float* a_scalar, size_t a_n) = 0;
  virtual void SetScalars(BSHP<VecFlt> a_scalar) = 0;
  virtual float InterpToPt(const Pt3d& a_pt) = 0;
  virtual void InterpToPts(const VecPt3d& a_pts, VecFlt& a_scalars) = 0;
  virtual void SetPtActivity(DynBitset& a_activity) = 0;
  // bitset is number of triangles in length not numtri*3 like the tris array
  virtual void SetTriActivity(DynBitset& a_activity) = 0;
  virtual void SetTrunc(double a_sMax, double a_sMin) = 0;

  virtual const BSHP<VecPt3d> GetPts() const = 0;
  virtual const BSHP<VecInt> GetTris() const = 0;
  virtual const BSHP<VecFlt> GetScalars() const = 0;
  virtual DynBitset GetPtActivity() const = 0;
  virtual DynBitset GetTriActivity() const = 0;

  virtual std::string ToString() const = 0;
  virtual ~InterpBase() {}

protected:
  InterpBase() {}

private:
  XM_DISALLOW_COPY_AND_ASSIGN(InterpBase);
  /// \endcond
};
//----- Function prototypes ----------------------------------------------------
}