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
#include <xmscore/misc/boost_defines.h>
#include <xmscore/misc/DynBitset.h>
#include <xmscore/python/misc/PyObserver.h>
#include <xmscore/python/misc/PyUtils.h>
#include <xmsinterp/interpolate/InterpIdw.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Internal functions -----------------------------------------------------

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpIdw(py::module &m) {

    // Class
    py::class_<xms::InterpIdw, xms::InterpBase, 
        boost::shared_ptr<xms::InterpIdw>> iIdw(m, "InterpIdw");

  iIdw.def(py::init([](py::iterable pts, py::iterable tris, py::iterable scalars) {
    BSHP<xms::InterpIdw> idw(xms::InterpIdw::New());
    BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
    BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);

    idw->SetPtsTris(vec_pts, vec_tris);

    if (py::len(scalars) > 0)
    {
      BSHP<xms::VecFlt> vec_scalars = xms::VecFltFromPyIter(scalars);
      if (vec_scalars->size() != vec_pts->size())
        throw std::length_error("scalars length != pts length.");
      idw->SetScalars(vec_scalars);
    }

    return idw;
  }), py::arg("pts"), py::arg("tris"), py::arg("scalars"));
  // ---------------------------------------------------------------------------
  // function: SetPtsTris
  // ---------------------------------------------------------------------------
  iIdw.def("SetPtsTris", [](xms::InterpIdw &self, py::iterable pts, py::iterable tris) {
    BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
    BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
    self.SetPtsTris(vec_pts, vec_tris);
  },py::arg("pts"),py::arg("tris"));
  // ---------------------------------------------------------------------------
  // function: SetScalars
  // ---------------------------------------------------------------------------
  iIdw.def("SetScalars", [](xms::InterpIdw &self, py::iterable scalar) {
    BSHP<xms::VecFlt> vec_scalars = xms::VecFltFromPyIter(scalar);
    self.SetScalars(vec_scalars);
  }, py::arg("scalar"));
  // ---------------------------------------------------------------------------
  // function: SetPts
  // ---------------------------------------------------------------------------
  iIdw.def("SetPts", [](xms::InterpIdw &self, py::iterable pts, bool a2d) {
    BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
    self.SetPts(vec_pts, a2d);
  },py::arg("pts"), py::arg("a2d"));
  // ---------------------------------------------------------------------------
  // function: InterpToPt
  // ---------------------------------------------------------------------------
  iIdw.def("InterpToPt",[](xms::InterpIdw &self, py::tuple pt) -> float {
    xms::Pt3d point = xms::Pt3dFromPyIter(pt);
    return self.InterpToPt(point);
  },py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: InterpToPts
  // ---------------------------------------------------------------------------
  iIdw.def("InterpToPts", [](xms::InterpIdw &self, py::iterable pts) -> py::iterable {
    boost::shared_ptr<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
    boost::shared_ptr<xms::VecFlt> vec_scalars(new xms::VecFlt());
    self.InterpToPts(*vec_pts, *vec_scalars);
    return xms::PyIterFromVecFlt(*vec_scalars, py::isinstance<py::array>(pts));
  }, py::arg("pts"));
  // ---------------------------------------------------------------------------
  // function: SetPtActivity
  // ---------------------------------------------------------------------------
  iIdw.def("SetPtActivity", [](xms::InterpIdw &self, py::iterable activity) {
    xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
    self.SetPtActivity(bitset);
  },py::arg("activity"));
  // ---------------------------------------------------------------------------
  // function: SetTriActivity
  // ---------------------------------------------------------------------------
  iIdw.def("SetTriActivity", [](xms::InterpIdw &self, py::iterable activity) {
      xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
      self.SetTriActivity(bitset);
  },py::arg("activity"));
  // ---------------------------------------------------------------------------
  // function: GetPts
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetPts", [](xms::InterpIdw &self) -> py::iterable {
    BSHP<xms::VecPt3d> pts = self.GetPts();
    return xms::PyIterFromVecPt3d(*pts);
  });
  // ---------------------------------------------------------------------------
  // function: GetTris
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetTris", [](xms::InterpIdw &self) -> py::iterable {
      BSHP<xms::VecInt> tris = self.GetTris();
      return xms::PyIterFromVecInt(*tris);
  });
  // ---------------------------------------------------------------------------
  // function: GetScalars
  // ---------------------------------------------------------------------------
  iIdw.def("GetScalars", &xms::InterpIdw::GetScalars);
  // ---------------------------------------------------------------------------
  // function: GetPtActivity
  // ---------------------------------------------------------------------------
  iIdw.def("GetPtActivity", &xms::InterpIdw::GetPtActivity);
  // ---------------------------------------------------------------------------
  // function: GetTriActivity
  // ---------------------------------------------------------------------------
  iIdw.def("GetTriActivity", &xms::InterpIdw::GetTriActivity);
  // ---------------------------------------------------------------------------
  // function: SetTrunc
  // ---------------------------------------------------------------------------
    iIdw.def("SetTrunc", &xms::InterpIdw::SetTrunc, py::arg("smax"), py::arg("smin"));
  // ---------------------------------------------------------------------------
  // function: GetTruncateInterpolatedValues
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetTruncateInterpolatedValues", &xms::InterpIdw::GetTruncateInterpolatedValues);
  // ---------------------------------------------------------------------------
  // function: GetTruncMin
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetTruncMin", &xms::InterpIdw::GetTruncMin);
  // ---------------------------------------------------------------------------
  // function: GetTruncMax
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetTruncMax", &xms::InterpIdw::GetTruncMax);
  // ---------------------------------------------------------------------------
  // function: GetPower
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetPower", &xms::InterpIdw::GetPower);
  // ---------------------------------------------------------------------------
  // function: GetSearchOptsNumNearestPts
  // ---------------------------------------------------------------------------
  iIdw.def("GetSearchOptsNumNearestPts", &xms::InterpIdw::GetSearchOptsNumNearestPts);
  // ---------------------------------------------------------------------------
  // function: GetSearchOptsUseQuadrantSearch
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetSearchOptsUseQuadrantSearch", &xms::InterpIdw::GetSearchOptsUseQuadrantSearch);
  // ---------------------------------------------------------------------------
  // function: GetWeightCalcMethod
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetWeightCalcMethod", &xms::InterpIdw::GetWeightCalcMethod);
  // ---------------------------------------------------------------------------
  // function: GetNodalFunctionType
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetNodalFunctionType", &xms::InterpIdw::GetNodalFunctionType);
  // ---------------------------------------------------------------------------
  // function: GetNodalFunctionNumNearestPts
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetNodalFunctionNumNearestPts", &xms::InterpIdw::GetNodalFunctionNumNearestPts);
  // ---------------------------------------------------------------------------
  // function: GetNodalFunctionUseQuadrantSearch
  // ---------------------------------------------------------------------------
  iIdw.def_property_readonly("GetNodalFunctionUseQuadrantSearch", &xms::InterpIdw::GetNodalFunctionUseQuadrantSearch);
  // ---------------------------------------------------------------------------
  // function: SetObserver
  // ---------------------------------------------------------------------------
  iIdw.def("SetObserver", [](xms::InterpIdw &self, boost::shared_ptr<xms::PublicObserver> obs) {
    self.SetObserver(obs);
  }, py::arg("obs"));
  // ---------------------------------------------------------------------------
  // function: SetPower
  // ---------------------------------------------------------------------------
    iIdw.def("SetPower", &xms::InterpIdw::SetPower, py::arg("power"));
  // ---------------------------------------------------------------------------
  // function: SetSearchOpts
  // ---------------------------------------------------------------------------
    iIdw.def("SetSearchOpts", &xms::InterpIdw::SetSearchOpts, py::arg("number_nearest_points"),
                py::arg("use_quadrant_octant_search"));
  // ---------------------------------------------------------------------------
  // function: SetWeightCalcMethod
  // ---------------------------------------------------------------------------
  iIdw.def("SetWeightCalcMethod", [](xms::InterpIdw& self, int method) {
    xms::InterpIdw::WeightEnum we = static_cast<xms::InterpIdw::WeightEnum>(method);
    self.SetWeightCalcMethod(we);
  },py::arg("method"));
  // ---------------------------------------------------------------------------
  // function: SetNodalFunction
  // ---------------------------------------------------------------------------
    iIdw.def("SetNodalFunction", [](xms::InterpIdw &self, 
                                      int nodal_func_type,
                                      int n_nearest, bool quad_oct,
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
      xms::InterpIdw::NodalFuncEnum w = static_cast<xms::InterpIdw::NodalFuncEnum>(nodal_func_type);
      self.SetNodalFunction(w, n_nearest, quad_oct, obs);
    },
      py::arg("nodal_func_type") = "constant",
      py::arg("num_nearest_points") = 16,
      py::arg("use_quadrant_octant_search") = false,
      py::arg("observer") = py::none()
  );
  // ---------------------------------------------------------------------------
  // function: SetSaveWeights
  // ---------------------------------------------------------------------------
  iIdw.def("SetSaveWeights", &xms::InterpIdw::SetSaveWeights,
    py::arg("save"));
  // ---------------------------------------------------------------------------
  // function: InterpWeights
  // ---------------------------------------------------------------------------
    iIdw.def("InterpWeights", [](xms::InterpIdw &self, py::tuple pt) -> 
          py::iterable {
              xms::VecInt idxs;
              xms::VecDbl wts;
              if (py::len(pt) != 3) {
                throw py::type_error("Input points must be 3-tuples");
              } else {
                xms::Pt3d point = xms::Pt3dFromPyIter(pt);
                self.InterpWeights(point, idxs, wts);
                py::array ret_idxs = xms::PyIterFromVecInt(idxs, true);
                py::array ret_wts = xms::PyIterFromVecDbl(wts, true);
                return py::make_tuple(ret_idxs, ret_wts);
              }
          }, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: SetMultiThreading
  // ---------------------------------------------------------------------------
    iIdw.def("SetMultiThreading", &xms::InterpIdw::SetMultiThreading,
            py::arg("multithreading"));

}
