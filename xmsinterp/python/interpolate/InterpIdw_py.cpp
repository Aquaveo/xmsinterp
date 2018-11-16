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
    const char* interp_idw_doc = R"pydoc(
        Class that performs inverse distance weighted interpolation
    )pydoc";
    py::class_<xms::InterpIdw, xms::InterpBase, 
        boost::shared_ptr<xms::InterpIdw>> iIdw(m, "InterpIdw",interp_idw_doc);
    iIdw.def(py::init(&xms::InterpIdw::New));
  // ---------------------------------------------------------------------------
  // function: set_pts_tris
  // ---------------------------------------------------------------------------
    const char* set_pts_tris_doc = R"pydoc(
        Sets the points that will be used to do the interpolation.

        Args:
            pts (iterable):  Array of the point locations.
            tris (iterable): Triangles.
    )pydoc";

    iIdw.def("set_pts_tris", [](xms::InterpIdw &self, py::iterable pts, 
                                py::iterable tris) {
              BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
              BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
              self.SetPtsTris(vec_pts, vec_tris);
          },set_pts_tris_doc,py::arg("pts"),py::arg("tris"));
  // ---------------------------------------------------------------------------
  // function: set_pts
  // ---------------------------------------------------------------------------
    const char* set_pts_doc = R"pydoc(
        Sets the points that will be used to do the interpolation.

        Args:
            pts (iterable):  array of the point locations
            a2d (bool): indicates if the class will do 2D or 3D interpolation
    )pydoc";

    iIdw.def("set_pts", [](xms::InterpIdw &self, py::iterable pts, bool a2d) {
              BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
              self.SetPts(vec_pts, a2d);
          }, set_pts_doc,py::arg("pts"), py::arg("a2d"));
  // ---------------------------------------------------------------------------
  // function: interp_to_pt
  // ---------------------------------------------------------------------------
    const char* interp_to_pt_doc = R"pydoc(
        Interpolates to the location specified by pt and returns the value.

        Args:
            pt (tuple): The location of the interpolation point.

        Returns:
          float: The interpolated value.
    )pydoc";

    iIdw.def("interp_to_pt",[](xms::InterpIdw &self, py::tuple pt) -> float {
              xms::Pt3d point = xms::Pt3dFromPyIter(pt);
              return self.InterpToPt(point);
          },interp_to_pt_doc,py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: interp_to_pts
  // ---------------------------------------------------------------------------
    const char* interp_to_pts_doc = R"pydoc(
        Interpolates to an array of points and fills in an array of scalars. 
        This method will run in parallel using multiple threads.

        Args:
            pts (iterable): Array of points to interpolate to.

        Returns:
          iterable: Array of scalar values. It will be the same size as a_pts and each value corresponds to the interpolated value at the respective location in the a_pts array.
    )pydoc";

    iIdw.def("interp_to_pts", [](xms::InterpIdw &self, py::iterable pts) -> 
                                py::iterable {
              boost::shared_ptr<xms::VecPt3d> vec_pts = 
                  xms::VecPt3dFromPyIter(pts);
              boost::shared_ptr<xms::VecFlt> vec_scalars(new xms::VecFlt());
              self.InterpToPts(*vec_pts, *vec_scalars);
              return xms::PyIterFromVecFlt(*vec_scalars, 
                  py::isinstance<py::array>(pts));
          },interp_to_pts_doc, py::arg("pts"));
  // ---------------------------------------------------------------------------
  // function: set_pt_activity
  // ---------------------------------------------------------------------------
    const char* set_pt_activity_doc = R"pydoc(
        Sets the activity on the point being used to interpolate

        Args:
            activity (iterable): Bitset of point activity.
    )pydoc";

    iIdw.def("set_pt_activity", [](xms::InterpIdw &self, 
                                  py::iterable activity) {
              xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
              self.SetPtActivity(bitset);
          },set_pt_activity_doc,py::arg("activity"));
  // ---------------------------------------------------------------------------
  // function: set_tri_activity
  // ---------------------------------------------------------------------------
    const char* set_tri_activity_doc = R"pydoc(
        Sets triangle activity. Ignored by IDW.

        Args:
            activity (iterable): Bitset of point activity.
    )pydoc";

    iIdw.def("set_tri_activity", [](xms::InterpIdw &self, 
                                    py::iterable activity) {
              xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
              self.SetTriActivity(bitset);
          },set_tri_activity_doc,py::arg("activity"));
  // ---------------------------------------------------------------------------
  // function: pts
  // ---------------------------------------------------------------------------
    const char* pts_doc = R"pydoc(
        Returns points vector.

        Returns:
            iterable: points vector
    )pydoc";

    iIdw.def_property_readonly("pts", [](xms::InterpIdw &self) -> py::iterable {
              BSHP<xms::VecPt3d> pts = self.GetPts();
              return xms::PyIterFromVecPt3d(*pts);
          },pts_doc);
  // ---------------------------------------------------------------------------
  // function: tris
  // ---------------------------------------------------------------------------
    const char* tris_doc = R"pydoc(
        Returns triangles vector.

        Returns:
            iterable: triangles vector
    )pydoc";

    iIdw.def_property_readonly("tris", [](xms::InterpIdw &self) -> 
                                          py::iterable {
              BSHP<xms::VecInt> tris = self.GetTris();
              return xms::PyIterFromVecInt(*tris);
          },tris_doc);
  // ---------------------------------------------------------------------------
  // function: set_trunc
  // ---------------------------------------------------------------------------
    const char* set_trunc_doc = R"pydoc(
        Set the truncation values for the interpolation and turn on truncation.

        Args:
            smax (float): The maximum value for truncation.
            smin (float): The minimum value for truncation.
    )pydoc";

    iIdw.def("set_trunc", &xms::InterpIdw::SetTrunc,
            set_trunc_doc,py::arg("smax"), py::arg("smin"));
  // ---------------------------------------------------------------------------
  // function: set_observer
  // ---------------------------------------------------------------------------
    const char* set_observer_doc = R"pydoc(
        Set the observer class so that feedback on the interpolation process 
        can be received.

        Args:
            obs (Observer): The observer
    )pydoc";

    iIdw.def("set_observer", [](xms::InterpIdw &self,
                                  boost::shared_ptr<xms::PublicObserver> obs) {
              self.SetObserver(obs);
          },set_observer_doc, py::arg("obs"));
  // ---------------------------------------------------------------------------
  // function: set_power
  // ---------------------------------------------------------------------------
    const char* set_power_doc = R"pydoc(
        Sets the exponent for the interpolation. By default the class does 
        inverse distance squared weighting but the exponent can be changed to 
        any value.

        Args:
            power (float): the exponent used to compute the point weights 
                1 / distance^a_power
    )pydoc";

    iIdw.def("set_power", &xms::InterpIdw::SetPower,
            set_power_doc, py::arg("power"));
  // ---------------------------------------------------------------------------
  // function: set_search_opts
  // ---------------------------------------------------------------------------
    const char* set_search_opts_doc = R"pydoc(
        Sets the search options for how to find the nearest points to the 
        interpolation point. The number of nearest points can be specified as 
        well as whether to find the nearest points in each quadrant or octant.

        Args:
            n_nearest_points (int): the number of nearest points to the interpolation point. These points are used to do the interpolation.

            quad_oct_search (bool): specifies if the search criterion should find the nearest points in each quadrant (2d) or octant (3d)
    )pydoc";

    iIdw.def("set_search_opts", &xms::InterpIdw::SetSearchOpts,
            set_search_opts_doc, py::arg("n_nearest_points"), 
            py::arg("quad_oct_search"));
  // ---------------------------------------------------------------------------
  // function: set_weight_calc_method
  // ---------------------------------------------------------------------------
    const char* set_weight_calc_method_doc = R"pydoc(
        Sets the method for calculating the weights. The classic just uses 
        1/distance^exponent. The modified method uses another formulation based 
        on the distance of the furtherest location from the interpolation pt.

        Args:
            method (weight_enum): The weight calculation method.
    )pydoc";

    iIdw.def("set_weight_calc_method", &xms::InterpIdw::SetWeightCalcMethod,
            set_weight_calc_method_doc,py::arg("method"));
  // ---------------------------------------------------------------------------
  // function: set_nodal_function
  // ---------------------------------------------------------------------------
    const char* set_nodal_function_doc = R"pydoc(
        Sets the type of nodal function as well as options for computing nodal 
        functions.

        Args:
            a (nodal_func_enum): The nodal function methodology: constant (0), gradient plane (1), quadratic (2).

            n_nearest (int): The nearest number of points to use when calculating the nodal functions.

            quad_oct (bool): Find the nearest number of points in each quadrant (2d) or octant (3d) when computing nodal functions.

            obs (Observer): Progress bar to give user feedback.
    )pydoc";

    iIdw.def("set_nodal_function", [](xms::InterpIdw &self, 
                                        xms::InterpIdw::NodalFuncEnum a,
                                        int n_nearest, bool quad_oct,
                                  boost::shared_ptr<xms::PublicObserver> obs) {
              self.SetNodalFunction(a, n_nearest, quad_oct, obs);
          },set_nodal_function_doc, py::arg("a"), py::arg("n_nearest"), 
          py::arg("quad_oct"), py::arg("obs"));
  // ---------------------------------------------------------------------------
  // function: set_save_weights
  // ---------------------------------------------------------------------------
    const char* set_save_weights_doc = R"pydoc(
        sets a flag to save the weights computed by the interpolation

        Args:
            save_weight (bool): true will save weights and false will not
    )pydoc";

    iIdw.def("set_save_weights", &xms::InterpIdw::SetSaveWeights,
            set_save_weights_doc, py::arg("save_weight"));
  // ---------------------------------------------------------------------------
  // function: interp_weights
  // ---------------------------------------------------------------------------
    const char* interp_weights_doc = R"pydoc(
        Given a location and an array of points the weights associated with 
        array of points are calculated.

        Args:
            pt (tuple): Location of the interpolation point

        Returns:
            iterable: Contains an iterable  of indices indicating the location in the m_pts vector of the nearest points to pt and an interable of the weights associated with the nearest points to pt.
    )pydoc";

    iIdw.def("interp_weights", [](xms::InterpIdw &self, py::tuple pt) -> 
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
          },interp_weights_doc, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: set_multi_threading
  // ---------------------------------------------------------------------------
    const char* set_multi_threading_doc = R"pydoc(
        Sets a flag to use (or not) multi-threading when interpolating.

        Args:
            multithreading (bool): True will use multi-threading and false will not. The default setting for the class is to use multi-threading.
    )pydoc";

    iIdw.def("set_multi_threading", &xms::InterpIdw::SetMultiThreading,
            set_multi_threading_doc, py::arg("multithreading"));
  // ---------------------------------------------------------------------------
  // function: __str__
  // ---------------------------------------------------------------------------
    const char* __str__doc = R"pydoc(
        Write the internals to a string.

        Returns:
            str: The internals as a string
    )pydoc";

    iIdw.def("__str__", &xms::InterpIdw::ToString,__str__doc);
  // ---------------------------------------------------------------------------
  // function: to_string
  // ---------------------------------------------------------------------------
    const char* to_string_doc = R"pydoc(
        Write the internals to a string.

        Returns:
            str: The internals as a string
    )pydoc";

    iIdw.def("to_string", &xms::InterpIdw::ToString,to_string_doc);

      py::enum_<xms::InterpIdw::WeightEnum>(iIdw, "weight_enum", 
                                            "weight_enum for InterpIdw class")
          .value("CLASSIC", xms::InterpIdw::WeightEnum::CLASSIC)
          .value("MODIFIED", xms::InterpIdw::WeightEnum::MODIFIED)
          .export_values();

      py::enum_<xms::InterpIdw::NodalFuncEnum>(iIdw, "nodal_func_enum", 
                                          "nodal_func_enum for InterpIdw class")
          .value("CONSTANT", xms::InterpIdw::NodalFuncEnum::CONSTANT)
          .value("GRAD_PLANE", xms::InterpIdw::NodalFuncEnum::GRAD_PLANE)
          .value("QUADRATIC", xms::InterpIdw::NodalFuncEnum::QUADRATIC)
          .export_values();
}
