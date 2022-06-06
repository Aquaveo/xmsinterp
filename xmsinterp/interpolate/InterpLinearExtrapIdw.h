#pragma once
//------------------------------------------------------------------------------
/// \file
/// \copyright (C) Copyright Aquaveo 2020. Distributed under the xmsng
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------

// 3. Standard Library Headers

// 4. External Library Headers
#include <xmsinterp/interpolate/InterpLinear.h>
#include <xmsinterp/interpolate/InterpIdw.h>

// 5. Shared Headers

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
class InterpLinearExtrapIdw : public InterpLinear
{
public:
  static BSHP<InterpLinearExtrapIdw> New();

  /// \cond
  virtual void SetPtsTris(BSHP<std::vector<Pt3d>> a_, BSHP<std::vector<int>> a_tris) override;
  virtual void SetScalars(const float* a_scalar, size_t a_n) override;
  virtual void SetScalars(BSHP<VecFlt> a_scalar) override;
  virtual float InterpToPt(const Pt3d& a_pt) override;
  virtual void InterpToPts(const std::vector<Pt3d>& a_pts, 
                           VecFlt& a_scalars) override;
  virtual void SetPtActivity(DynBitset& a_activity) override;
  // bitset is number of triangles in length not numtri*3 like the tris array
  virtual void SetTriActivity(DynBitset& a_activity) override;
  virtual const BSHP<VecPt3d> GetPts() const override;
  virtual const BSHP<VecInt> GetTris() const override;
  virtual const BSHP<VecFlt> GetScalars() const override;
  virtual DynBitset GetPtActivity() const override;
  virtual DynBitset GetTriActivity() const override;
  virtual VecInt GetExtrapolationPointIndexes() const override;

  // find triangle
  virtual int TriContainingPt(const Pt3d& a_pt) override;
  virtual void TriEnvelopsContainingPt(const Pt3d& a_pt, std::vector<int>& a_tris) override;
  // interpolation
  virtual bool InterpWeights(const Pt3d& a_pt,
                             std::vector<int>& a_idxs,
                             std::vector<double>& a_wts) override;

  // interpolation options
  virtual void SetExtrapVal(double a_val) override;
  virtual void SetTrunc(double a_sMax, double a_sMin) override;
  virtual void SetUseCloughTocher(bool a_, BSHP<Observer> a_prog) override;
  virtual void SetUseNatNeigh(bool a_,
                              int a_nodalFunc,
                              int a_ndFuncOpt,
                              int a_ndFuncNumNearestPts,
                              bool a_blendWeights,
                              BSHP<Observer> a_prog) override;

  virtual double GetExtrapVal() const;
  virtual bool GetTruncateInterpolatedValues() const;
  virtual double GetTruncMin() const;
  virtual double GetTruncMax() const;
  virtual bool GetUseCloughTocher() const;
  virtual bool GetUseNatNeigh() const;
  virtual int GetNatNeighNodalFunc() const;
  virtual int GetNatNeighNodalFuncNearestPtsOption() const;
  virtual int GetNatNeighNodalFuncNumNearestPts() const;
  virtual bool GetNatNeighBlendWeights() const;

  virtual std::string ToString() const override;

private:
  XM_DISALLOW_COPY_AND_ASSIGN(InterpLinearExtrapIdw);
  /// \endcond

protected:
  InterpLinearExtrapIdw();
  // virtual ~InterpLinearExtrapIdw();
  BSHP<InterpLinear> m_linear;
  BSHP<InterpIdw> m_idw;
}; // class InterpLinear

//----- Function prototypes ----------------------------------------------------
}