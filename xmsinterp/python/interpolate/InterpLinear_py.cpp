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
#include <xmsinterp/python/misc/PublicObserver.h>
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
            BSHP<xms::VecPt3d> vec_pts(new xms::VecPt3d());
            BSHP<xms::VecInt> vec_tris(new xms::VecInt());
            for (auto item : pts) {
              if(!py::isinstance<py::iterable>(item)) {
                throw py::type_error("First arg must be a n-tuple of 3-tuples");
              }

              py::tuple tuple = item.cast<py::iterable>();
              if (py::len(tuple) != 3) {
                throw py::type_error("Input points must be 3-tuples");
              } else {
                xms::Pt3d point(tuple[0].cast<double>(), tuple[1].cast<double>(), tuple[2].cast<double>());
                vec_pts->push_back(point);
              }
            }
            for (auto item : tris) {
              vec_tris->push_back(item.cast<int>());
            }
            self.SetPtsTris(vec_pts, vec_tris);
        })
        .def("set_scalars", [](xms::InterpLinear &self, py::iterable scalar) {
           BSHP<std::vector<float>> vec_scalars;
           for (auto item : scalar) {
             vec_scalars->push_back(item.cast<float>());
           }
           return self.SetScalars(vec_scalars);
        })
        .def("interp_to_pt",[](xms::InterpLinear &self, py::tuple pt) -> float {
            if(py::len(pt) != 3) {
              throw py::type_error("Input point should be a 3-tuple");
            } else {
              xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
              return self.InterpToPt(point);
            }
        })
        .def("interp_to_pts", [](xms::InterpLinear &self, py::iterable pts) -> py::iterable {
           xms::VecPt3d vec_pts;
           xms::VecFlt vec_scalars;

            for (auto item : pts) {
              if(!py::isinstance<py::iterable>(item)) {
                throw py::type_error("First arg must be a n-tuple of 3-tuples");
              }

              py::tuple tuple = item.cast<py::tuple>();

              if (py::len(tuple) != 3) {
                throw py::type_error("Input points must be 3-tuples");
              } else {
                xms::Pt3d point(tuple[0].cast<double>(), tuple[1].cast<double>(), tuple[2].cast<double>());
                vec_pts.push_back(point);
              }
            }

            self.InterpToPts(vec_pts, vec_scalars);
            if (py::isinstance<py::array>(pts)) {
              // NOTE: This is a copy operation
              return py::array(vec_scalars.size(), vec_scalars.data());
            } else {
              // NOTE: This is a copy operation
              auto tuple_ret = py::tuple(vec_scalars.size());
              for (size_t i = 0; i < tuple_ret.size(); ++i) {
                tuple_ret[i] = vec_scalars.at(i);
              }
              return tuple_ret;
            }
        })
        .def("set_pt_activity", [](xms::InterpLinear &self, py::iterable activity) {
          xms::DynBitset bitset;
          std::vector<unsigned char> bitvals;
          for (auto item : activity) {
            py::bool_ flag = item.cast<py::bool_>();
            if (flag) {
              bitvals.push_back(1);
            } else {
              bitvals.push_back(0);
            }
          }
          xms::VecBooleanToDynBitset(bitvals, bitset);
          self.SetPtActivity(bitset);
        })
        .def("set_tri_activity", [](xms::InterpLinear &self, py::iterable activity) {
          xms::DynBitset bitset;
          std::vector<unsigned char> bitvals;
          for (auto item : activity) {
            py::bool_ flag = item.cast<py::bool_>();
            if (flag) {
              bitvals.push_back(1);
            } else {
              bitvals.push_back(0);
            }
          }
          xms::VecBooleanToDynBitset(bitvals, bitset);
          self.SetTriActivity(bitset);
        })
        .def("tri_containing_pt", [](xms::InterpLinear &self, py::tuple pt) -> int {
           if (py::len(pt) != 3) {
             throw py::type_error("Input points must be 3-tuples");
           } else {
             xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
             return self.TriContainingPt(point);
           }
        })
        .def("tri_envelops_containing_pt", [](xms::InterpLinear &self, py::tuple pt) -> py::array {
           std::vector<int> tris;
           if (py::len(pt) != 3) {
             throw py::type_error("Input points must be 3-tuples");
           } else {
             xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
             self.TriEnvelopsContainingPt(point, tris);
             return py::array(tris.size(), tris.data());
           }
        })
        .def("interp_weights", [](xms::InterpLinear &self, py::tuple pt) -> py::iterable {
          xms::VecInt idxs;
          xms::VecDbl wts;
          if (py::len(pt) != 3) {
            throw py::type_error("Input points must be 3-tuples");
          } else {
            xms::Pt3d point(pt[0].cast<double>(), pt[1].cast<double>(), pt[2].cast<double>());
            bool pt_inside = self.InterpWeights(point, idxs, wts);
            py::array ret_idxs(idxs.size(), idxs.data());
            py::array ret_wts(wts.size(), wts.data());
            return py::make_tuple(pt_inside, ret_idxs, ret_wts);
          }
        })
        .def("set_extrap_val", &xms::InterpLinear::SetExtrapVal)
        .def("set_trunc", &xms::InterpLinear::SetTrunc)
        .def_property_readonly("pts", [](xms::InterpLinear &self) -> py::iterable {
          BSHP<std::vector<xms::Pt3d>> pts = self.GetPts();
          py::array_t<double, py::array::c_style> a({(int)(*pts).size(), 3});
          auto r = a.mutable_unchecked<2>();
          int i = 0;
          for (ssize_t i = 0; i < r.shape(0); i++) {
           r(i, 0) = (*pts)[i].x;
           r(i, 1) = (*pts)[i].y;
           r(i, 2) = (*pts)[i].z;
          }
          return a;
        })
        .def_property_readonly("tris", [](xms::InterpLinear &self) -> py::iterable {
          BSHP<std::vector<int>> tris = self.GetTris();
          return py::array(tris->size(), tris->data());
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
