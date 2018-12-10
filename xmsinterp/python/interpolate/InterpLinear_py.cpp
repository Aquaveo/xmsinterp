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
#include <xmsinterp/interpolate/InterpIdw.h>
#include <xmsinterp/interpolate/InterpLinear.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>
#include <xmsinterp/triangulate/TrTriangulatorPoints.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Internal functions -----------------------------------------------------
namespace {

// -----------------------------------------------------------------------------
/// \brief converts string to an int. This will throw if the string is not
/// recognized.
/// \param[in] a_: string to be converted to an enum
/// \return int representing the nodal function 0-constant, 1-gradient_plane,
/// 2-quadratic
// -----------------------------------------------------------------------------
int NodalFuncTypeFromString(const std::string& a_nodal_func_type)
{
  int nodal_func(0);
  if (a_nodal_func_type == "constant")
    nodal_func = static_cast<int>(xms::InterpIdw::CONSTANT);
  else if (a_nodal_func_type == "gradient_plane")
    nodal_func = static_cast<int>(xms::InterpIdw::GRAD_PLANE);
  else if (a_nodal_func_type == "quadratic")
    nodal_func = static_cast<int>(xms::InterpIdw::QUADRATIC);
  else
  {
    std::string msg = "nodal_func_type string must be one of 'constant', 'gradient_plane', "
                      "'quadratic' not " + a_nodal_func_type;
    throw py::value_error(msg);
  }
  return nodal_func;
} // NodalFuncTypeFromString
// -----------------------------------------------------------------------------
/// \brief converts string to an int. This will throw if the string is not
/// recognized.
/// \param[in] a_: string to be converted to an enum
/// \return int representing the search option 0-nearest_pts, 1-natural_neighbors
// -----------------------------------------------------------------------------
int NodalFuncPtSearchOptFromString(const std::string& a_nodal_func_pt_search_opt)
{
  int searchOpt(0);
  if (a_nodal_func_pt_search_opt == "natural_neighbors")
    searchOpt = 0;
  else if (a_nodal_func_pt_search_opt == "nearest_pts")
    searchOpt = 1;
  else
  {
    std::string msg = "a_nodal_func_pt_search_opt string must be one of 'natural_neighbors',"
                      " 'nearest_pts' not " + a_nodal_func_pt_search_opt;
    throw py::value_error(msg);
  }
  return searchOpt;
} // NodalFuncPtSearchOptFromString

} // unnamed namespace

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpLinear(py::module &m) {
    const char* interp_linear_doc = R"pydoc(
        Class that performs linear interpolation

        Args:
            pts (iterable):  Array of the point locations.
            tris (:obj:`iterable`, optional): Triangles.
            scalar (:obj:`iterable`, optional): Array of interpolation scalar values.

    )pydoc";
    py::class_<xms::InterpLinear, xms::InterpBase,
        boost::shared_ptr<xms::InterpLinear>> iLin(m, "InterpLinear",
        interp_linear_doc);
    iLin.def(py::init([](py::iterable pts, py::iterable tris,
                          py::iterable scalars) {
      boost::shared_ptr<xms::InterpLinear> rval(xms::InterpLinear::New());
      boost::shared_ptr<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
      boost::shared_ptr<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
      rval->SetPtsTris(vec_pts, vec_tris);

      if (py::len(scalars) > 0)
      {
        BSHP<xms::VecFlt> vec_scalars = xms::VecFltFromPyIter(scalars);
        if (vec_scalars->size() != vec_pts->size())
          throw std::length_error("scalars length != pts length.");
        rval->SetScalars(vec_scalars);
      }

      return rval;
    }), py::arg("pts"), py::arg("tris") = py::make_tuple(), py::arg("scalars") = py::make_tuple());
  // ---------------------------------------------------------------------------
  // function: __str__
  // ---------------------------------------------------------------------------
    const char* __str__doc = R"pydoc(
        Write the internals to a string.

        Returns:
            str: The internals as a string
    )pydoc";

    iLin.def("__str__", &xms::InterpLinear::ToString,__str__doc);
  // ---------------------------------------------------------------------------
  // attribute: __repr__
  // ---------------------------------------------------------------------------
    iLin.def("__repr__", [](const xms::InterpLinear &il)
           {
             return PyReprStringFromInterpBase(il);
           }
    );
  // ---------------------------------------------------------------------------
  // function: to_string
  // ---------------------------------------------------------------------------
    const char* to_string_doc = R"pydoc(
        Write the internals to a string.

        Returns:
            str: The internals as a string
    )pydoc";

    iLin.def("to_string", &xms::InterpLinear::ToString, to_string_doc);
  // ---------------------------------------------------------------------------
  // function: set_pts_tris
  // ---------------------------------------------------------------------------
    const char* set_pts_tris_doc = R"pydoc(
        Adds the triangles to the class.

        Args:
            pts (iterable): Array of point locations.

            tris (iterable): Array of triangles that references the pts array. This array will have a size that is a multiple of 3. The first 3 locations in array represent the first triangle and will have indices that correspond to locations in the pts array.
    )pydoc";

    iLin.def("set_pts_tris", 
        [](xms::InterpLinear &self, py::iterable pts, py::iterable tris) {
            BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
            BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
            self.SetPtsTris(vec_pts, vec_tris);
        },set_pts_tris_doc, py::arg("pts"),py::arg("tris"));
  // ---------------------------------------------------------------------------
  // function: set_scalars
  // ---------------------------------------------------------------------------
    const char* set_scalars_doc = R"pydoc(
        Set the scalars that will be used to interpolate from.

        Args:
            scalar (iterable): Array of interpolation scalar values.
    )pydoc";

    iLin.def("set_scalars", [](xms::InterpLinear &self, py::iterable scalar) {
            BSHP<xms::VecFlt> vec_scalars = xms::VecFltFromPyIter(scalar);
            return self.SetScalars(vec_scalars);
        },set_scalars_doc, py::arg("scalar"));
  // ---------------------------------------------------------------------------
  // function: interp_to_pt
  // ---------------------------------------------------------------------------
    const char* interp_to_pt_doc = R"pydoc(
        Use the stored triangles to interpolate to a point. Returns 
        extrapolation value if the point is outside the triangles.

        Args:
            pt (tuple): Location that is interpolated to.

        Returns:
            float: Interpolated value at a_pt.
    )pydoc";

    iLin.def("interp_to_pt",[](xms::InterpLinear &self, py::tuple pt) -> float {
            if (!self.GetTris() || self.GetTris()->empty())
            {
              const BSHP<xms::VecPt3d> vec_pts = self.GetPts();
              if (vec_pts && !vec_pts->empty())
              {
                boost::shared_ptr<xms::VecInt> vec_tris(new xms::VecInt());
                xms::TrTriangulatorPoints t(*vec_pts, *vec_tris);
                t.Triangulate();
                self.SetPtsTris(vec_pts, vec_tris);
              }
            }
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            float s = self.InterpToPt(point);
            if (self.GetExtrapVal() == s)
              s = std::numeric_limits<float>::quiet_NaN();
            return s;
        },interp_to_pt_doc, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: interp_to_pts
  // ---------------------------------------------------------------------------
    const char* interp_to_pts_doc = R"pydoc(
        Calls interp_to_pt in a loop.

        Args:
            pts (iterable): Locations of points.

        Returns:
            iterable: Interpolated scalar values at pts.
    )pydoc";

    iLin.def("interp_to_pts", 
        [](xms::InterpLinear &self, py::iterable pts) -> py::iterable {
            if (!self.GetTris() || self.GetTris()->empty())
            {
              const BSHP<xms::VecPt3d> vec_pts = self.GetPts();
              if (vec_pts && !vec_pts->empty())
              {
                boost::shared_ptr<xms::VecInt> vec_tris(new xms::VecInt());
                xms::TrTriangulatorPoints t(*vec_pts, *vec_tris);
                t.Triangulate();
                self.SetPtsTris(vec_pts, vec_tris);
              }
            }
            BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
            xms::VecFlt vec_scalars;
            self.InterpToPts(*vec_pts, vec_scalars);
            double extrap = self.GetExtrapVal();
            for (auto& s : vec_scalars)
            {
              if (extrap == s)
                s = std::numeric_limits<float>::quiet_NaN();
            }
            return xms::PyIterFromVecFlt(vec_scalars,
                                         py::isinstance<py::array>(pts));
        },interp_to_pts_doc, py::arg("pts"));
  // ---------------------------------------------------------------------------
  // function: set_pt_activity
  // ---------------------------------------------------------------------------
    const char* set_pt_activity_doc = R"pydoc(
        Modifies the activity bitset of the class.

        Args:
            activity (iterable): Bitset of the activity of the points.
    )pydoc";

    iLin.def("set_pt_activity", 
        [](xms::InterpLinear &self, py::iterable activity) {
            xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
            self.SetPtActivity(bitset);
        },set_pt_activity_doc,py::arg("activity"));
  // ---------------------------------------------------------------------------
  // function: set_tri_activity
  // ---------------------------------------------------------------------------
    const char* set_tri_activity_doc = R"pydoc(
        Modifies the activity bitset of the class.

        Args:
            activity (iterable): Bitset of the activity of the triangles.
    )pydoc";

    iLin.def("set_tri_activity", 
        [](xms::InterpLinear &self, py::iterable activity) {
            xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
            self.SetTriActivity(bitset);
        },set_tri_activity_doc,py::arg("activity"));
  // ---------------------------------------------------------------------------
  // function: tri_containing_pt
  // ---------------------------------------------------------------------------
    const char* tri_containing_pt_doc = R"pydoc(
        Find the triangle containing the point

        Args:
            pt (tuple): Location used to find a triangle.

        Returns:
            int: Index of triangle containing pt. If XM_NONE is returned then no triangle contained the point.
    )pydoc";

    iLin.def("tri_containing_pt", 
        [](xms::InterpLinear &self, py::tuple pt) -> int {
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            return self.TriContainingPt(point);
        },tri_containing_pt_doc, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: tri_envelops_containing_pt
  // ---------------------------------------------------------------------------
    const char* tri_envelops_containing_pt_doc = R"pydoc(
        Find all triangles whose envelop contains the point.

        Args:
            pt (tuple): Location used to find a triangle.

        Returns:
            array: The indices to triangles whose envelop contains the point.
    )pydoc";

    iLin.def("tri_envelopes_containing_pt",
        [](xms::InterpLinear &self, py::tuple pt) -> py::array {
            xms::VecInt tris;
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            self.TriEnvelopsContainingPt(point, tris);
            return xms::PyIterFromVecInt(tris);
        },tri_envelops_containing_pt_doc, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: interp_weights
  // ---------------------------------------------------------------------------
    const char* interp_weights_doc = R"pydoc(
        Use the stored triangles to get interpolation weights for a point. 
        Returns false if the point is outside the triangles.

        Args:
            pt (tuple): Location that is interpolated to.

        Returns:
            iterable: Contains a boolean indicating if pt was inside of any of the triangles, an iterable of triangle point indices found in this method, and an iterable of triangle point weights found in this method.
    )pydoc";

    iLin.def("interp_weights", 
        [](xms::InterpLinear &self, py::tuple pt) -> py::iterable {
            xms::VecInt idxs;
            xms::VecDbl wts;
            xms::Pt3d point = xms::Pt3dFromPyIter(pt);
            bool pt_inside = self.InterpWeights(point, idxs, wts);
            py::array ret_idxs = xms::PyIterFromVecInt(idxs, true);
            py::array ret_wts = xms::PyIterFromVecDbl(wts, true);
            return py::make_tuple(pt_inside, ret_idxs, ret_wts);
        },interp_weights_doc, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: set_extrap_val
  // ---------------------------------------------------------------------------
    const char* set_extrap_val_doc = R"pydoc(
        Set the constant extrapolation value.

        Args:
            value (float): The value assigned to extrapolated points.
    )pydoc";

    iLin.def("set_extrapolation_value", &xms::InterpLinear::SetExtrapVal,
            set_extrap_val_doc, py::arg("value"));
  // ---------------------------------------------------------------------------
  // function: set_trunc
  // ---------------------------------------------------------------------------
    const char* set_trunc_doc = R"pydoc(
        Set the truncation values for the interpolation and turn on truncation.

        Args:
            max (float): The maximum value for truncation.

            min (float): The minimum value for truncation.
    )pydoc";

    iLin.def("set_truncation_max_min", &xms::InterpLinear::SetTrunc,set_trunc_doc,
            py::arg("max"), py::arg("min"));
  // ---------------------------------------------------------------------------
  // function: pts
  // ---------------------------------------------------------------------------
    const char* pts_doc = R"pydoc(
        Returns an iterable containing the points.

        Returns:
            iterable: An iterable containing the points.
    )pydoc";

    iLin.def_property_readonly("pts", []
        (xms::InterpLinear &self) -> py::iterable {
            BSHP<xms::VecPt3d> pts = self.GetPts();
            return xms::PyIterFromVecPt3d(*pts);
        },pts_doc);
  // ---------------------------------------------------------------------------
  // function: tris
  // ---------------------------------------------------------------------------
    const char* tris_doc = R"pydoc(
        Returns an iterable containing the triangles.

        Returns:
            iterable: An iterable containing the triangles.
    )pydoc";

    iLin.def_property_readonly("tris", []
        (xms::InterpLinear &self) -> py::iterable {
          BSHP<std::vector<int>> tris = self.GetTris();
          return xms::PyIterFromVecInt(*tris, true);
        },tris_doc);
  // ---------------------------------------------------------------------------
  // function: set_use_clough_tocher
  // ---------------------------------------------------------------------------
    const char* set_use_clough_tocher_doc1 = R"pydoc(
        Set the class to use the Clough Tocher interpolation method. This is a
        legacy feature from GMS. Compare to linear.

        Args:
            on (bool): True/False to indicate if CT should be used.

            observer (Observer): Progress bar to give users feed back on the set up process of CT. If you have a really large set of triangles this may take some time.
    )pydoc";

    iLin.def("set_use_clough_tocher", [](xms::InterpLinear &self, bool on) {
          BSHP<xms::PublicObserver> obs;
          self.SetUseCloughTocher(on, obs);
        },set_use_clough_tocher_doc1, py::arg("on"));
  // ---------------------------------------------------------------------------
  // function: set_use_clough_tocher
  // ---------------------------------------------------------------------------
    const char* set_use_clough_tocher_doc = R"pydoc(
        Set the class to use the Clough Tocher interpolation method. This is a 
        legacy feature from GMS. Compare to linear.

        Args:
            on (bool): True/False to indicate if CT should be used.

            observer (Observer): Progress bar to give users feed back on the set up process of CT. If you have a really large set of triangles this may take some time.
    )pydoc";

    iLin.def("set_use_clough_tocher", [](xms::InterpLinear &self, bool on,
                              boost::shared_ptr<xms::PublicObserver> observer) {
          self.SetUseCloughTocher(on, observer);
        },set_use_clough_tocher_doc, py::arg("on"), py::arg("observer"));
  // ---------------------------------------------------------------------------
  // function: set_use_nat_neigh
  // ---------------------------------------------------------------------------
    const char* set_use_nat_neigh_doc = R"pydoc(
        Set the class to use natural neighbor (NN) interpolation.

        Args:
            on (:obj:`bool`, optional): True/False to indicate if NN should be used.

            nodal_func_type (:obj:`string`, optional): Indicates which type of nodal function to use: "constant", "gradient_plane", or "quadratic".

            nd_func_pt_search_opt (:obj:`string`, optional): Indicates options for the nearest points when computing the nodal functions: "natural_neighbors" or "nearest_pts".

            nd_func_num_nearest_pts (:obj:`int`, optional): The number of nearest points for nodal function computation.

            nd_func_blend_weights (:obj:`bool`, optional): Option to use a blending function on the calculated weights.

            observer (:obj:`Observer`, optional): Progress bar to give user feedback for generation of the nodal functions.
    )pydoc";

    iLin.def("set_use_natural_neighbor",
     [](xms::InterpLinear &self, bool on,
        std::string nodal_func_type, std::string nd_func_pt_search_opt,
        int nd_func_num_nearest_pts,
        bool nd_func_blend_weights,
        py::object observer)
        {
          BSHP<xms::Observer> obs;
          if (!observer.is_none())
            obs = observer.cast<BSHP<xms::Observer>>();
          int nodalFuncType = NodalFuncTypeFromString(nodal_func_type);
          int nodalFuncPtSearchOpt = NodalFuncPtSearchOptFromString(nd_func_pt_search_opt);
          self.SetUseNatNeigh(on, nodalFuncType, nodalFuncPtSearchOpt,
                              nd_func_num_nearest_pts, nd_func_blend_weights, obs);
        },set_use_nat_neigh_doc,
            py::arg("on") = true, py::arg("nodal_func_type") = "constant",
            py::arg("nd_func_pt_search_opt") = "natural_neighbors",
            py::arg("nd_func_num_nearest_pts") = 16,
            py::arg("nd_func_blend_weights") = true,
            py::arg("observer") = py::none()
        );
}
