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
#include <xmscore/misc/DynBitset.h>
#include <xmscore/python/misc/PyUtils.h>
#include <xmscore/python/misc/PublicObserver.h>
#include <xmsinterp/interpolate/InterpLinear.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpLinear(py::module &m) {
    py::class_<xms::InterpLinear, xms::InterpBase, boost::shared_ptr<xms::InterpLinear>>(m, "InterpLinear")
        .def(py::init(&xms::InterpLinear::New))
        .def("__str__", &xms::InterpLinear::ToString,"Write the internals to a string.")
        .def("to_string", &xms::InterpLinear::ToString,"Write the internals to a string.")
        .def("set_pts_tris", [](xms::InterpLinear &self, py::iterable pts, py::iterable tris) {
            BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
            BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
            self.SetPtsTris(vec_pts, vec_tris);
        },"Adds the triangles to the class",
            py::arg("vec_pts"),py::arg("vec_tris")
        )
        .def("set_scalars", [](xms::InterpLinear &self, py::iterable scalar) {
            BSHP<xms::VecFlt> vec_scalars = xms::VecFltFromPyIter(scalar);
            return self.SetScalars(vec_scalars);
        },"Set the scalars that will be used to interpolate from.",
            py::arg("vec_scalars")
        )
        .def("interp_to_pt",[](xms::InterpLinear &self, py::tuple pt) -> float {
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            return self.InterpToPt(point);
        },"Use the stored triangles to interpolate to a point. Returns extrapolation value if"
            " the point is outside the triangles.",
            py::arg("point")    
        )
        .def("interp_to_pts", [](xms::InterpLinear &self, py::iterable pts) -> py::iterable {
            BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
            BSHP<xms::VecFlt> vec_scalars(new xms::VecFlt());
            self.InterpToPts(*vec_pts, *vec_scalars);
            return xms::PyIterFromVecFlt(*vec_scalars, py::isinstance<py::array>(pts));
        },"Calls InterpToPt in a loop.",
            py::arg("vec_pts")
        )
        .def("set_pt_activity", [](xms::InterpLinear &self, py::iterable activity) {
            xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
            self.SetPtActivity(bitset);
        },"Modifies the activity bitset of the class.",
            py::arg("bitset")
        )
        .def("set_tri_activity", [](xms::InterpLinear &self, py::iterable activity) {
            xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
            self.SetTriActivity(bitset);
        },"Modifies the activity bitset of the class.",
            py::arg("bitset")
        )
        .def("tri_containing_pt", [](xms::InterpLinear &self, py::tuple pt) -> int {
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            return self.TriContainingPt(point);
        },"Find the triangle containing the point",
            py::arg("point")
        )
        .def("tri_envelops_containing_pt", [](xms::InterpLinear &self, py::tuple pt) -> py::array {
            xms::VecInt tris;
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            self.TriEnvelopsContainingPt(point, tris);
            return xms::PyIterFromVecInt(tris);
        },"Find all triangle whose envelop contains the point.",
            py::arg("point")
        )
        .def("interp_weights", [](xms::InterpLinear &self, py::tuple pt) -> py::iterable {
            xms::VecInt idxs;
            xms::VecDbl wts;
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            bool pt_inside = self.InterpWeights(point, idxs, wts);
            py::array ret_idxs = xms::PyIterFromVecInt(idxs, true);
            py::array ret_wts = xms::PyIterFromVecDbl(wts, true);
            return py::make_tuple(pt_inside, ret_idxs, ret_wts);
        },"Use the stored triangles to get interpolation weights for a point. Returns false if"
            " the point is outside the triangles.",
            py::arg("point")    
        )
        .def("set_extrap_val", &xms::InterpLinear::SetExtrapVal,"Set the constant extrapolation value",
            py::arg("value")
        )
        .def("set_trunc", &xms::InterpLinear::SetTrunc,"Set the truncation values for the interpolation and turn on truncation.",
            py::arg("max"), py::arg("min")
        )
        .def_property_readonly("pts", [](xms::InterpLinear &self) -> py::iterable {
            BSHP<xms::VecPt3d> pts = self.GetPts();
            return xms::PyIterFromVecPt3d(*pts);
        },"Returns shared pointer to points vector.")
        .def_property_readonly("tris", [](xms::InterpLinear &self) -> py::iterable {
          BSHP<std::vector<int>> tris = self.GetTris();
          return xms::PyIterFromVecInt(*tris, true);
        },"Returns shared pointer to triangles vector.")
        .def("set_use_clough_tocher", [](xms::InterpLinear &self, bool on,
                                         boost::shared_ptr<xms::PublicObserver> observer) {
          self.SetUseCloughTocher(on, observer);
        },"Set the class to use the Clough Tocher interpolation method. This is a legacy feature"
            " from GMS. Frankly it is so similar to linear that there is really no reason to use"
            " it but it is here because users complain if features are removed.",
            py::arg("on"), py::arg("observer")
        )
        .def("set_use_nat_neigh", [](xms::InterpLinear &self, bool on, int nodal_func, int nd_func_opt,
                                     int nd_func_num_nearest_pts, bool blend_weights,
                                     boost::shared_ptr<xms::PublicObserver> observer) {
          self.SetUseNatNeigh(on, nodal_func, nd_func_opt, nd_func_num_nearest_pts, blend_weights, observer);
        },"Set the class to use natural neighbor (NN) interpolation.",
            py::arg("on"), py::arg("nodal_func"), py::arg("nd_func_opt"), py::arg("nd_func_num_neareest_pts"), 
			py::arg("blend_weights"), py::arg("observer")
        );
}
