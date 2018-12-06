#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers

// 5. Shared Headers
#include <xmscore/points/ptsfwd.h>
#include <xmscore/stl/vector.h>
#include <xmsinterp/interpolate/InterpBase.h>
#include <xmscore/misc/base_macros.h>
#include <xmscore/misc/boost_defines.h>


//----- Forward declarations ---------------------------------------------------

//----- Namespace declaration --------------------------------------------------
namespace xms
{
class Observer;

//----- Constants / Enumerations -----------------------------------------------

//----- Structs / Classes ------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// \brief Class that performs linear interpolation
/// \see InterpLinearImpl
class InterpLinear : public InterpBase
{
public:
  static BSHP<InterpLinear> New();
  virtual ~InterpLinear();

  /// \cond
  virtual void SetPtsTris(BSHP<VecPt3d> a_, BSHP<VecInt> a_tris) override = 0;
  virtual void SetScalars(const float* a_scalar, size_t a_n) override = 0;
  virtual void SetScalars(BSHP<VecFlt> a_scalar) override = 0;
  virtual float InterpToPt(const Pt3d& a_pt) override = 0;
  virtual void InterpToPts(const VecPt3d& a_pts,
                           VecFlt& a_scalars) override = 0;
  virtual void SetPtActivity(DynBitset& a_activity) override = 0;
  // bitset is number of triangles in length not numtri*3 like the tris array
  virtual void SetTriActivity(DynBitset& a_activity) override = 0;
  virtual const BSHP<VecPt3d> GetPts() const override = 0;
  virtual const BSHP<VecInt> GetTris() const override = 0;
  virtual const BSHP<VecFlt> GetScalars() const override = 0;
  virtual DynBitset GetPtActivity() const override = 0;
  virtual DynBitset GetTriActivity() const override = 0;

  // find triangle
  virtual int TriContainingPt(const Pt3d& a_pt) = 0;
  virtual void TriEnvelopsContainingPt(const Pt3d& a_pt, std::vector<int>& a_tris) = 0;
  // interpolation
  virtual bool InterpWeights(const Pt3d& a_pt,
                             VecInt& a_idxs,
                             VecDbl& a_wts) = 0;

  // interpolation options
  virtual void SetExtrapVal(double a_val) = 0;
  virtual void SetTrunc(double a_sMax, double a_sMin) override = 0;
  virtual void SetUseCloughTocher(bool a_, BSHP<Observer> a_prog) = 0;
  virtual void SetUseNatNeigh(bool a_,
                              int a_nodalFunc,
                              int a_ndFuncOpt,
                              int a_ndFuncNumNearestPts,
                              bool a_blendWeights,
                              BSHP<Observer> a_prog) = 0;

  virtual double GetExtrapVal() const = 0;
  virtual bool GetTruncateInterpolatedValues() const = 0;
  virtual double GetTruncMin() const = 0;
  virtual double GetTruncMax() const = 0;
  virtual bool GetUseCloughTocher() const = 0;
  virtual bool GetUseNatNeigh() const = 0;
  virtual int GetNatNeighNodalFunc() const = 0;
  virtual int GetNatNeighNodalFuncNearestPtsOption() const = 0;
  virtual int GetNatNeighNodalFuncNumNearestPts() const = 0;
  virtual bool GetNatNeighBlendWeights() const = 0;


  virtual std::string ToString() const override = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(InterpLinear);
  /// \endcond

protected:
  InterpLinear();
}; // class InterpLinear

//----- Function prototypes ----------------------------------------------------
}
