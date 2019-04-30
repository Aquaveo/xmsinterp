//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under FreeBSD License
/// (See accompanying file LICENSE or https://aqaveo.com/bsd/license.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <boost/shared_ptr.hpp>
#include <xmscore/misc/DynBitset.h>
#include <xmscore/misc/StringUtil.h>
#include <xmscore/stl/vector.h>
#include <xmscore/python/misc/PyUtils.h>
#include <xmscore/python/misc/PyObserver.h>
#include <xmsgrid/triangulate/TrTriangulatorPoints.h>
#include <xmsinterp/interpolate/InterpIdw.h>
#include <xmsinterp/interpolate/InterpLinear.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Internal functions -----------------------------------------------------
//namespace {
//
//// -----------------------------------------------------------------------------
///// \brief converts string to an int. This will throw if the string is not
///// recognized.
///// \param[in] a_: string to be converted to an enum
///// \return int representing the nodal function 0-constant, 1-gradient_plane,
///// 2-quadratic
//// -----------------------------------------------------------------------------
//int NodalFuncTypeFromString(const std::string& a_nodal_func_type)
//{
//  int nodal_func(0);
//  if (a_nodal_func_type == "constant")
//    nodal_func = static_cast<int>(xms::InterpIdw::CONSTANT);
//  else if (a_nodal_func_type == "gradient_plane")
//    nodal_func = static_cast<int>(xms::InterpIdw::GRAD_PLANE);
//  else if (a_nodal_func_type == "quadratic")
//    nodal_func = static_cast<int>(xms::InterpIdw::QUADRATIC);
//  else
//  {
//    std::string msg = "nodal_func_type string must be one of 'constant', 'gradient_plane', "
//                      "'quadratic' not " + a_nodal_func_type;
//    throw py::value_error(msg);
//  }
//  return nodal_func;
//} // NodalFuncTypeFromString
//// -----------------------------------------------------------------------------
///// \brief converts string to an int. This will throw if the string is not
///// recognized.
///// \param[in] a_: string to be converted to an enum
///// \return int representing the search option 0-nearest_pts, 1-natural_neighbors
//// -----------------------------------------------------------------------------
//int NodalFuncPtSearchOptFromString(const std::string& a_nodal_func_pt_search_opt)
//{
//  int searchOpt(0);
//  if (a_nodal_func_pt_search_opt == "natural_neighbors")
//    searchOpt = 0;
//  else if (a_nodal_func_pt_search_opt == "nearest_pts")
//    searchOpt = 1;
//  else
//  {
//    std::string msg = "a_nodal_func_pt_search_opt string must be one of 'natural_neighbors',"
//                      " 'nearest_pts' not " + a_nodal_func_pt_search_opt;
//    throw py::value_error(msg);
//  }
//  return searchOpt;
//} // NodalFuncPtSearchOptFromString
//
//} // unnamed namespace

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpLinear(py::module &m) {

    py::class_<xms::InterpLinear, xms::InterpBase,
        boost::shared_ptr<xms::InterpLinear>> iLin(m, "InterpLinear");

	// ---------------------------------------------------------------------------
	// function: __init__
	// ---------------------------------------------------------------------------
  iLin.def(py::init([](py::iterable pts, py::iterable tris, py::iterable scalars) {
    boost::shared_ptr<xms::InterpLinear> rval(xms::InterpLinear::New());
    boost::shared_ptr<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
    boost::shared_ptr<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);

    if (!vec_tris || vec_tris->empty())
    {
      if (!vec_tris)
        vec_tris.reset(new xms::VecInt());
      xms::TrTriangulatorPoints t(*vec_pts, *vec_tris);
      t.Triangulate();
      if (vec_tris->size() < 3)
      {
        throw std::domain_error("Unable to triangulate points. Aborting.");
      }
    }

    rval->SetPtsTris(vec_pts, vec_tris);

    if (py::len(scalars) > 0)
    {
      BSHP<xms::VecFlt> vec_scalars = xms::VecFltFromPyIter(scalars);
      rval->SetScalars(vec_scalars);
    }

    rval->SetExtrapVal(std::numeric_limits<float>::quiet_NaN());

    return rval;
  }), py::arg("pts"), py::arg("tris"), py::arg("scalars"));
  // ---------------------------------------------------------------------------
  // function: SetPtsTris
  // ---------------------------------------------------------------------------
  iLin.def("SetPtsTris", [](xms::InterpLinear &self, py::iterable pts, py::iterable tris) {
    BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
    BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
    self.SetPtsTris(vec_pts, vec_tris);
  }, py::arg("pts"),py::arg("tris"));
  // ---------------------------------------------------------------------------
  // function: SetScalars
  // ---------------------------------------------------------------------------
  iLin.def("SetScalars", [](xms::InterpLinear &self, py::iterable scalar) {
    BSHP<xms::VecFlt> vec_scalars = xms::VecFltFromPyIter(scalar);
    self.SetScalars(vec_scalars);
  }, py::arg("scalar"));
  // ---------------------------------------------------------------------------
  // function: InterpToPt
  // ---------------------------------------------------------------------------
  iLin.def("InterpToPt",[](xms::InterpLinear &self, py::tuple pt) -> float {
    return self.InterpToPt(xms::Pt3dFromPyIter(pt));
  }, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: InterpToPts
  // ---------------------------------------------------------------------------
  iLin.def("InterpToPts", [](xms::InterpLinear &self, py::iterable pts) -> py::iterable {
    BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
    xms::VecFlt vec_scalars;
    self.InterpToPts(*vec_pts, vec_scalars);
    return xms::PyIterFromVecFlt(vec_scalars, py::isinstance<py::array>(pts));
  }, py::arg("pts"));
  // ---------------------------------------------------------------------------
  // function: SetPtActivity
  // ---------------------------------------------------------------------------
  iLin.def("SetPtActivity", [](xms::InterpLinear &self, py::iterable activity) {
    xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
    self.SetPtActivity(bitset);
  },py::arg("activity"));
  // ---------------------------------------------------------------------------
  // function: SetTriActivity
  // ---------------------------------------------------------------------------
  iLin.def("SetTriActivity", [](xms::InterpLinear &self, py::iterable activity) {
	  xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
	  self.SetTriActivity(bitset);
  },py::arg("activity"));
	// ---------------------------------------------------------------------------
	// function: GetPts
	// ---------------------------------------------------------------------------
	iLin.def_property_readonly("GetPts", [](xms::InterpLinear &self) -> py::iterable {
		BSHP<xms::VecPt3d> pts = self.GetPts();
		return xms::PyIterFromVecPt3d(*pts);
	});
	// ---------------------------------------------------------------------------
	// function: GetTris
	// ---------------------------------------------------------------------------
	iLin.def_property_readonly("GetTris", [](xms::InterpLinear &self) -> py::iterable {
		BSHP<std::vector<int>> tris = self.GetTris();
		return xms::PyIterFromVecInt(*tris, true);
	});
  // ---------------------------------------------------------------------------
  // function: GetScalars
  // ---------------------------------------------------------------------------
  iLin.def_property_readonly("GetScalars", [](xms::InterpLinear &self) -> py::iterable {
    BSHP<std::vector<float>> scalars = self.GetScalars();
    return xms::PyIterFromVecFlt(*scalars, true);
  });
  // ---------------------------------------------------------------------------
  // function: GetPtActivity
  // ---------------------------------------------------------------------------
  iLin.def_property_readonly("GetPtActivity", [](xms::InterpLinear &self) -> py::iterable {
    xms::DynBitset ptActivity = self.GetPtActivity();
    return xms::PyIterFromDynamicBitset(ptActivity, true);
  });
  // ---------------------------------------------------------------------------
  // function: GetTriActivity
  // ---------------------------------------------------------------------------
  iLin.def_property_readonly("GetTriActivity", [](xms::InterpLinear &self) -> py::iterable {
    xms::DynBitset triActivity = self.GetTriActivity();
    return xms::PyIterFromDynamicBitset(triActivity, true);
  });
  // ---------------------------------------------------------------------------
  // function: GetExtrapolationPointIndexes
  // ---------------------------------------------------------------------------
  iLin.def_property_readonly("GetExtrapolationPointIndexes", [](xms::InterpLinear &self) -> py::iterable {
    xms::VecInt extrapPtIndexes = self.GetExtrapolationPointIndexes();
    return xms::PyIterFromVecInt(extrapPtIndexes, true);
  });
  // ---------------------------------------------------------------------------
  // function: TriContainingPt
  // ---------------------------------------------------------------------------
  iLin.def("TriContainingPt", [](xms::InterpLinear &self, py::tuple pt) -> int {
    xms::Pt3d point = xms::Pt3dFromPyIter(pt);
    return self.TriContainingPt(point);
  }, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: TriEnvelopsContainingPt
  // ---------------------------------------------------------------------------
  iLin.def("TriEnvelopsContainingPt", [](xms::InterpLinear &self, py::tuple pt) -> py::array {
    xms::VecInt tris;
    xms::Pt3d point = xms::Pt3dFromPyIter(pt);
    self.TriEnvelopsContainingPt(point, tris);
    return xms::PyIterFromVecInt(tris);
  }, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: InterpWeights
  // ---------------------------------------------------------------------------
  iLin.def("InterpWeights", [](xms::InterpLinear &self, py::tuple pt) -> py::iterable {
    xms::VecInt idxs;
    xms::VecDbl wts;
    xms::Pt3d point = xms::Pt3dFromPyIter(pt);
    bool pt_inside = self.InterpWeights(point, idxs, wts);
    py::array ret_idxs = xms::PyIterFromVecInt(idxs, true);
    py::array ret_wts = xms::PyIterFromVecDbl(wts, true);
    return py::make_tuple(pt_inside, ret_idxs, ret_wts);
  }, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: SetExtrapVal
  // ---------------------------------------------------------------------------
  iLin.def("SetExtrapVal", &xms::InterpLinear::SetExtrapVal, py::arg("value"));
  // ---------------------------------------------------------------------------
  // function: SetTrunc
  // ---------------------------------------------------------------------------
  iLin.def("SetTrunc", &xms::InterpLinear::SetTrunc, py::arg("smax"), py::arg("smin"));
  // ---------------------------------------------------------------------------
  // function: SetUseCloughTocher
  // ---------------------------------------------------------------------------
  iLin.def("SetUseCloughTocher", [](xms::InterpLinear &self, bool on, py::object observer) {
    BSHP<xms::Observer> obs;
    if (!observer.is_none())
    {
      if (!py::isinstance<PyObserver>(observer))
        throw std::invalid_argument("observer must be of type xmscore.misc.Observer");
      BSHP<PyObserver> po = observer.cast<BSHP<PyObserver>>();
      obs = BDPC<xms::Observer>(po);
    }
    self.SetUseCloughTocher(on, obs);
  }, py::arg("on") = true, py::arg("observer"));
  // ---------------------------------------------------------------------------
  // function: SetUseNatNeigh
  // ---------------------------------------------------------------------------
  iLin.def("SetUseNatNeigh", [](xms::InterpLinear &self, bool on,
    int nodal_func_type, int nd_func_pt_search_opt,
    int nd_func_num_nearest_pts,
    bool nd_func_blend_weights,
    py::object observer)
    {
      BSHP<xms::Observer> obs;
      if (!observer.is_none())
      {
        if (!py::isinstance<PyObserver>(observer))
          throw std::invalid_argument("observer must be of type xmscore.misc.Observer");
        BSHP<PyObserver> po = observer.cast<BSHP<PyObserver>>();
        obs = BDPC<xms::Observer>(po);
      }
      self.SetUseNatNeigh(on, nodal_func_type, nd_func_pt_search_opt,
                          nd_func_num_nearest_pts, nd_func_blend_weights, obs);
    },  py::arg("on"), py::arg("nodal_func_type"), py::arg("nd_func_pt_search_opt"),
        py::arg("nd_func_num_nearest_pts"), py::arg("nd_func_blend_weights"),  py::arg("observer"));
    // ---------------------------------------------------------------------------
    // function: GetExtrapVal
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetExtrapVal", &xms::InterpLinear::SetExtrapVal);
    // ---------------------------------------------------------------------------
    // function: GetTruncateInterpolatedValues
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetTruncateInterpolatedValues", &xms::InterpLinear::GetTruncateInterpolatedValues);
    // ---------------------------------------------------------------------------
    // function: GetTruncMin
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetTruncMin", &xms::InterpLinear::GetTruncMin);
    // ---------------------------------------------------------------------------
    // function: GetTruncMax
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetTruncMax", &xms::InterpLinear::GetTruncMax);
    // ---------------------------------------------------------------------------
    // function: GetUseCloughTocher
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetUseCloughTocher", &xms::InterpLinear::GetUseCloughTocher);
    // ---------------------------------------------------------------------------
    // function: GetUseNatNeigh
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetUseNatNeigh", &xms::InterpLinear::GetUseNatNeigh);
    // ---------------------------------------------------------------------------
    // function: GetNatNeighNodalFunc
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetNatNeighNodalFunc", &xms::InterpLinear::GetNatNeighNodalFunc);
    // ---------------------------------------------------------------------------
    // function: GetNatNeighNodalFuncNearestPtsOption
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetNatNeighNodalFuncNearestPtsOption", &xms::InterpLinear::GetNatNeighNodalFuncNearestPtsOption);
    // ---------------------------------------------------------------------------
    // function: GetNatNeighNodalFuncNumNearestPts
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetNatNeighNodalFuncNumNearestPts", &xms::InterpLinear::GetNatNeighNodalFuncNumNearestPts);
    // ---------------------------------------------------------------------------
    // function: GetNatNeighBlendWeights
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetNatNeighBlendWeights", &xms::InterpLinear::GetNatNeighBlendWeights);

}
