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
    py::class_<xms::InterpLinear, boost::shared_ptr<xms::InterpLinear>>(m, "InterpLinear")
        .def(py::init(&xms::InterpLinear::New))
        .def("__str__", &xms::InterpLinear::ToString)
        .def("to_string", &xms::InterpLinear::ToString)
        .def("set_pts_tris", [](xms::InterpLinear &self, py::iterable pts, py::iterable tris) {
            BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
            BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
            self.SetPtsTris(vec_pts, vec_tris);
        })
        .def("set_scalars", [](xms::InterpLinear &self, py::iterable scalar) {
            BSHP<xms::VecFlt> vec_scalars = xms::VecFltFromPyIter(scalar);
            return self.SetScalars(vec_scalars);
        })
        .def("interp_to_pt",[](xms::InterpLinear &self, py::tuple pt) -> float {
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            return self.InterpToPt(point);

        })
        .def("interp_to_pts", [](xms::InterpLinear &self, py::iterable pts) -> py::iterable {
            BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
            BSHP<xms::VecFlt> vec_scalars(new xms::VecFlt());
            self.InterpToPts(*vec_pts, *vec_scalars);
            return xms::PyIterFromVecFlt(*vec_scalars, py::isinstance<py::array>(pts));
        })
        .def("set_pt_activity", [](xms::InterpLinear &self, py::iterable activity) {
            xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
            self.SetPtActivity(bitset);
        })
        .def("set_tri_activity", [](xms::InterpLinear &self, py::iterable activity) {
            xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
            self.SetTriActivity(bitset);
        })
        .def("tri_containing_pt", [](xms::InterpLinear &self, py::tuple pt) -> int {
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            return self.TriContainingPt(point);
        })
        .def("tri_envelops_containing_pt", [](xms::InterpLinear &self, py::tuple pt) -> py::array {
            xms::VecInt tris;
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            self.TriEnvelopsContainingPt(point, tris);
            return xms::PyIterFromVecInt(tris);
        })
        .def("interp_weights", [](xms::InterpLinear &self, py::tuple pt) -> py::iterable {
            xms::VecInt idxs;
            xms::VecDbl wts;
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            bool pt_inside = self.InterpWeights(point, idxs, wts);
            py::array ret_idxs = xms::PyIterFromVecInt(idxs, true);
            py::array ret_wts = xms::PyIterFromVecDbl(wts, true);
            return py::make_tuple(pt_inside, ret_idxs, ret_wts);
        })
        .def("set_extrap_val", &xms::InterpLinear::SetExtrapVal)
        .def("set_trunc", &xms::InterpLinear::SetTrunc)
        .def_property_readonly("pts", [](xms::InterpLinear &self) -> py::iterable {
            BSHP<xms::VecPt3d> pts = self.GetPts();
            return xms::PyIterFromVecPt3d(*pts);
        })
        .def_property_readonly("tris", [](xms::InterpLinear &self) -> py::iterable {
          BSHP<std::vector<int>> tris = self.GetTris();
          return xms::PyIterFromVecInt(*tris, true);
        })
        .def("set_use_clough_tocher", [](xms::InterpLinear &self, bool on,
                                         boost::shared_ptr<xms::PublicObserver> observer) {
          self.SetUseCloughTocher(on, observer);
        })
        .def("set_use_nat_neigh", [](xms::InterpLinear &self, bool on, int nodal_func, int nd_func_opt,
                                     int nd_func_num_nearest_pts, bool blend_weights,
                                     boost::shared_ptr<xms::PublicObserver> observer) {
          self.SetUseNatNeigh(on, nodal_func, nd_func_opt, nd_func_num_nearest_pts, blend_weights, observer);
        });
}
