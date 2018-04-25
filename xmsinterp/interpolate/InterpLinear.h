#pragma once
//------------------------------------------------------------------------------
/// \file
/// \ingroup interpolate
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
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
  virtual void SetPtsTris(BSHP<std::vector<Pt3d>> a_, BSHP<std::vector<int>> a_tris) override = 0;
  virtual void SetScalars(const float* a_scalar, size_t a_n) override = 0;
  virtual void SetScalars(BSHP<std::vector<float>> a_scalar) override = 0;
  virtual float InterpToPt(const Pt3d& a_pt) override = 0;
  virtual void InterpToPts(const std::vector<Pt3d>& a_pts,
                           std::vector<float>& a_scalars) override = 0;
  virtual void SetPtActivity(DynBitset& a_activity) override = 0;
  // bitset is number of triangles in length not numtri*3 like the tris array
  virtual void SetTriActivity(DynBitset& a_activity) override = 0;
  virtual BSHP<std::vector<Pt3d>> GetPts() override = 0;
  virtual BSHP<std::vector<int>> GetTris() override = 0;

  // find triangle
  virtual int TriContainingPt(const Pt3d& a_pt) = 0;
  virtual void TriEnvelopsContainingPt(const Pt3d& a_pt, std::vector<int>& a_tris) = 0;
  // interpolation
  virtual bool InterpWeights(const Pt3d& a_pt,
                             std::vector<int>& a_idxs,
                             std::vector<double>& a_wts) = 0;

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
  virtual std::string ToString() const override = 0;
  virtual void SetIdString(const std::string& a_id) override = 0;
  virtual std::string GetIdString() const override = 0;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(InterpLinear);
  /// \endcond

protected:
  InterpLinear();
}; // class InterpLinear

//----- Function prototypes ----------------------------------------------------
}