//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

#include <sstream>

#include <xmscore/misc/StringUtil.h>
#include <xmscore/python/misc/PyUtils.h>
#include <xmsinterp/interpolate/InterpLinear.h>
#include <xmsinterp/interpolate/InterpIdw.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------

void initInterpolate(py::module &m) {
    initInterpAnisotropic(m);
    initInterpBase(m);
    initInterpLinear(m);
    initInterpIdw(m);
}

namespace {
// ---------------------------------------------------------------------------
/// \brief appens to a python __repr__ string for an InterpLinear class
// ---------------------------------------------------------------------------
void AppendInterpLinearToPyReprString(const xms::InterpBase& a_interpBase,
                                      std::stringstream& a_ss)
{
  using namespace xms;
  try
  {
    const xms::InterpLinear& il(dynamic_cast<const xms::InterpLinear&>(a_interpBase));

    std::string offOn[2] = {"False", "True"};
    bool trunc = il.GetTruncateInterpolatedValues();
    a_ss << "use truncation: " << offOn[(int)trunc] << "\n";
    if (trunc)
    {
      a_ss << "truncation min: " << STRstd(il.GetTruncMin()) << "\n";
      a_ss << "truncation max: " << STRstd(il.GetTruncMax()) << "\n";
    }
    a_ss << "extrapolation value: " << STRstd(il.GetExtrapVal()) << "\n";
    bool useCt = il.GetUseCloughTocher();
    a_ss << "use clough tocher: " << offOn[(int)useCt] << "\n";
    bool useNn = il.GetUseNatNeigh();
    a_ss << "use natural neighbor: " << offOn[(int)useNn] << "\n";
    if (useNn)
    {
      int nfType = il.GetNatNeighNodalFunc();
      std::string nodalFunction[3] = {"constant", "gradient_plane", "quadratic"};
      a_ss << "natural neighbor nodal function type: " << nodalFunction[nfType] << "\n";
      if (nfType != 0)
      {
        int nearPtsOpt = il.GetNatNeighNodalFuncNearestPtsOption();
        std::string nearestOpt[2] = {"natural_neighbors", "nearest_points"};
        a_ss << "natural neighbor nodal function nearest points option: "
             << nearestOpt[nearPtsOpt] << "\n";
        int nPts = il.GetNatNeighNodalFuncNumNearestPts();
        a_ss << "natural neighbor nodal function number of nearest points: "
             << nPts << "\n";
        bool blend = il.GetNatNeighBlendWeights();
        a_ss << "natural neighbor nodal function blend weights option: "
             << offOn[(int)blend] << "\n";
      }
    }
  }
  catch (std::bad_cast &)
  {
    return;
  }
} // AppendInterpLinearToPyReprString
// ---------------------------------------------------------------------------
/// \brief appens to a python __repr__ string for an InterpLinear class
// ---------------------------------------------------------------------------
void AppendInterpIdwToPyReprString(const xms::InterpBase& a_interpBase,
                                   std::stringstream& a_ss)
{
  using namespace xms;
  try
  {
    const xms::InterpIdw& il(dynamic_cast<const xms::InterpIdw&>(a_interpBase));

    std::string offOn[2] = {"OFF", "ON"};
    bool trunc = il.GetTruncateInterpolatedValues();
    a_ss << "use truncation: " << offOn[(int)trunc] << "\n";
    if (trunc)
    {
      a_ss << "truncation min: " << STRstd(il.GetTruncMin()) << "\n";
      a_ss << "truncation max: " << STRstd(il.GetTruncMax()) << "\n";
    }
    a_ss << "power: " << STRstd(il.GetPower()) << "\n";
    std::string wMethod[2] = {"classic", "modified"};
    int opt = static_cast<int>(il.GetWeightCalcMethod());
    a_ss << "weight calculation method: " << wMethod[opt] << "\n";
    int nfType = static_cast<int>(il.GetNodalFunctionType());
    std::string nfStr[3] = {"constant", "gradient_plane", "quadratic"};
    a_ss << "nodal function type: " << nfStr[nfType] << "\n";
    a_ss << "nodal function number of nearest points: "
         << il.GetNodalFunctionNumNearestPts() << "\n";
    bool quad = il.GetNodalFunctionUseQuadrantSearch();
    a_ss << "nodal function use quadrant/octant search: " << offOn[(int)quad] << "\n";
  }
  catch (std::bad_cast &)
  {
    return;
  }
} // AppendInterpLinearToPyReprString

} // unnamed namespace

// ---------------------------------------------------------------------------
/// \brief creates a python __repr__ string from an InterpBase class
/// \return string for the __repr__ python member
// ---------------------------------------------------------------------------
std::string PyReprStringFromInterpBase(const xms::InterpBase& a_interpBase)
{
   using namespace xms;
   const InterpBase& il(a_interpBase);
   std::stringstream ss;
   const BSHP<VecPt3d> vec_pts = il.GetPts();
   const BSHP<VecInt> vec_tris = il.GetTris();
   const BSHP<VecFlt> vec_scalars = il.GetScalars();
   const VecPt3d& pts(*vec_pts);
   ss << "pts " << StringFromVecPt3d(pts);
   const VecInt& tris(*vec_tris);
   ss << "tris " << StringFromVecInt(tris);
   const VecFlt& scalars(*vec_scalars);
   ss << "scalars " << StringFromVecFlt(scalars);
   DynBitset ptActivity = il.GetPtActivity();
   ss << "pt activity " << StringFromDynBitset(ptActivity);
   DynBitset triActivity = il.GetTriActivity();
   ss << "tri activity " << StringFromDynBitset(triActivity);
   AppendInterpLinearToPyReprString(a_interpBase, ss);
   AppendInterpIdwToPyReprString(a_interpBase, ss);
   return ss.str();
} // PyReprStringFromInterpBase

