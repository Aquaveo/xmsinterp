//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <boost/shared_ptr.hpp>
#include <xmscore/misc/boost_defines.h>
#include <xmscore/misc/DynBitset.h>
#include <xmscore/python/misc/PublicObserver.h>
#include <xmscore/python/misc/PyUtils.h>
#include <xmsinterp/interpolate/InterpIdw.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpIdw(py::module &m) {

      // Class
      py::class_<xms::InterpIdw, xms::InterpBase, boost::shared_ptr<xms::InterpIdw>> iIdw(m, "InterpIdw");
      iIdw.def(py::init(&xms::InterpIdw::New))
          .def("set_pts_tris", [](xms::InterpIdw &self, py::iterable pts, py::iterable tris) {
              BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
              BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
              self.SetPtsTris(vec_pts, vec_tris);
          },"Sets the points that will be used to do the interpolation.",
            py::arg("vec_pts"),py::arg("vec_tris")
          )
          .def("set_pts", [](xms::InterpIdw &self, py::iterable pts, bool a2d) {
              BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
              self.SetPts(vec_pts, a2d);
          },"Sets the points that will be used to do the interpolation.",
            py::arg("vec_pts"), py::arg("a2d")
          )
          .def("interp_to_pt",[](xms::InterpIdw &self, py::tuple pt) -> float {
              xms::Pt3d point = xms::Pt3dFromPyIter(pt);
              return self.InterpToPt(point);
          },"Interpolates to the location specified by pt and returns the value.",
            py::arg("pt")
          )
          .def("interp_to_pts", [](xms::InterpIdw &self, py::iterable pts) -> py::iterable {
              boost::shared_ptr<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
              boost::shared_ptr<xms::VecFlt> vec_scalars(new xms::VecFlt());
              self.InterpToPts(*vec_pts, *vec_scalars);
              return xms::PyIterFromVecFlt(*vec_scalars, py::isinstance<py::array>(pts));
          },"Interpolates to an array of points and fills in an array of scalars." 
            "This method will run in parallel using multiple threads.",
            py::arg("vec_pts")
          )
          .def("set_pt_activity", [](xms::InterpIdw &self, py::iterable activity) {
              xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
              self.SetPtActivity(bitset);
          },"Sets the activity on the point being used to interpolate",
            py::arg("bitset")
          )
          .def("set_tri_activity", [](xms::InterpIdw &self, py::iterable activity) {
              xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
              self.SetTriActivity(bitset);
          },"Sets triangle activity. Ignored by IDW.",
            py::arg("bitset")
          )
          .def_property_readonly("pts", [](xms::InterpIdw &self) -> py::iterable {
              BSHP<xms::VecPt3d> pts = self.GetPts();
              return xms::PyIterFromVecPt3d(*pts);
          },"Returns shared pointer to points vector.")
          .def_property_readonly("tris", [](xms::InterpIdw &self) -> py::iterable {
              BSHP<xms::VecInt> tris = self.GetTris();
              return xms::PyIterFromVecInt(*tris);
          },"Returns shared pointer to triangles vector.")
          .def("set_trunc", &xms::InterpIdw::SetTrunc,
            "Set the truncation values for the interpolation and turn on truncation.",
            py::arg("smax"), py::arg("smin")
          )
          .def("set_observer", [](xms::InterpIdw &self,
                                  boost::shared_ptr<xms::PublicObserver> obs) {
              self.SetObserver(obs);
          },"Set the observer class so that feedback on the interpolation process can be received.",
            py::arg("obs")
          )
          .def("set_power", &xms::InterpIdw::SetPower,
            "Sets the exponent for the interpolation. By default the class does inverse distance"
            " squared weighting but the exponent can be changed to any value.",
            py::arg("power")  
          )
          .def("set_search_opts", &xms::InterpIdw::SetSearchOpts,
            "Sets the search options for how to find the nearest points to the interpolation point."
            " The number of nearest points can be specified as well as whether to find the nearest"
            " points in each quadrant or octant.",
            py::arg("n_nearest_points"), py::arg("quad_oct_search")
          )
          .def("set_weight_calc_method", &xms::InterpIdw::SetWeightCalcMethod,
            "Sets the method for calculating the weights. The classic just uses 1/distance^exponent."
            " The modified method uses another formulation based on the distance of the furtherest"
            " location from the interpolation pt.",
            py::arg("method")
          )
          .def("set_nodal_function", [](xms::InterpIdw &self, xms::InterpIdw::NodalFuncEnum a,
                                        int n_nearest, bool quad_oct,
                                        boost::shared_ptr<xms::PublicObserver> obs) {
              self.SetNodalFunction(a, n_nearest, quad_oct, obs);
          },"Sets the type of nodal function as well as options for computing nodal functions.",
            py::arg("a"), py::arg("n_nearest"), py::arg("quad_oct"), py::arg("obs")
          )
          .def("set_save_weights", &xms::InterpIdw::SetSaveWeights,
            "sets a flag to save the weights computed by the interpolation",
            py::arg("save_weight"))
          .def("interp_weights", [](xms::InterpIdw &self, py::tuple pt) -> py::iterable {
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
          },"Given a location and an array of points the weights associated with array of points"
            " are calculated.",
            py::arg("point")
          )
          .def("set_multi_threading", &xms::InterpIdw::SetMultiThreading,
            "sets a flag to use (or not) multi-threading when interpolating",
            py::arg("multithreading")  
          )
          .def("__str__", &xms::InterpIdw::ToString,"Write the internals to a string.")
          .def("to_string", &xms::InterpIdw::ToString,"Write the internals to a string.")
          ;

      py::enum_<xms::InterpIdw::WeightEnum>(iIdw, "weight_enum", "weight_enum for InterpIdw class")
          .value("CLASSIC", xms::InterpIdw::WeightEnum::CLASSIC)
          .value("MODIFIED", xms::InterpIdw::WeightEnum::MODIFIED)
          .export_values();

      py::enum_<xms::InterpIdw::NodalFuncEnum>(iIdw, "nodal_func_enum", "nodal_func_enum for InterpIdw class")
          .value("CONSTANT", xms::InterpIdw::NodalFuncEnum::CONSTANT)
          .value("GRAD_PLANE", xms::InterpIdw::NodalFuncEnum::GRAD_PLANE)
          .value("QUADRATIC", xms::InterpIdw::NodalFuncEnum::QUADRATIC)
          .export_values();
}
