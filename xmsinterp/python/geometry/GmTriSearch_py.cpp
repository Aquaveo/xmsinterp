//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
// #include <pybind11/numpy.h>
#include <boost/shared_ptr.hpp>
// #include <xmscore/misc/boost_defines.h>
// #include <xmscore/misc/DynBitset.h>
// #include <xmscore/python/misc/PublicObserver.h>
// #include <xmscore/python/misc/PyUtils.h>
// #include <xmsinterp/interpolate/InterpIdw.h>
// #include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initGmTriSearch(py::module &m) {

  // Class
  py::class_<xms::GmTriSearch, 
    boost::shared_ptr<xms::GmTriSearch>> iGmTriSearch(m, "GmTriSearch");
  iGmTriSearch.def(py::init(&xms::GmTriSearch::New));
  // ---------------------------------------------------------------------------
  // function: tris_to_search
  // ---------------------------------------------------------------------------
    const char* tris_to_search_doc = R"pydoc(
        Sets the points and triangles that are used to create an rtree.

        Args:
            pts (iterable):  array of the point locations
            tris (iterable): triangles
    )pydoc";

    iGmTriSearch.def("tris_to_search", [](xms::GmTriSearch &self,
                                py::iterable pts, py::iterable tris) {
              BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
              BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
              self.TrisToSearch(vec_pts, vec_tris);
          },tris_to_search_doc,py::arg("pts"),py::arg("tris"));
  // ---------------------------------------------------------------------------
  // property: pt_activity
  // ---------------------------------------------------------------------------
    const char* pt_activity_doc = R"pydoc(
        The activity bitset of the points
    )pydoc";

    iGmTriSearch.def_property("pt_activity",
    [](xms::GmTriSearch &self) -> py::iterable
    {
      return xms::PyIterFromDynamicBitset(self.GetPtActivity());
    }
    [](xms::GmTriSearch &self, py::iterable activity)
    {
      xms::DynBitset bs = xms::DynamicBitsetFromPyIter(activity);
      self.SetPtActivity(bs);
    },
    pt_activity_doc);
  // ---------------------------------------------------------------------------
  // function: set_tri_activity
  // ---------------------------------------------------------------------------
    const char* set_tri_activity_doc = R"pydoc(
        Modifies the activity bitset of the triangles

        Args:
            activity (iterable):  bitset of the activity of the triangles
    )pydoc";

    iGmTriSearch.def("set_tri_activity",
    [](xms::GmTriSearch &self, py::iterable activity)
    {
      xms::DynBitset bs = xms::DynamicBitsetFromPyIter(activity);
      self.SetTriActivity(bs);
    },
    set_tri_activity_doc,py::arg("activity"));
  // ---------------------------------------------------------------------------
  // function: tri_containing_pt
  // ---------------------------------------------------------------------------
    const char* tri_containing_pt_doc = R"pydoc(
        Find the triangle containing the point

        Args:
            pt (iterable):  Location used to find a triangle.

        Returns:
            iterable: Index of triangle containing pt.
    )pydoc";

    iGmTriSearch.def("tri_containing_pt",
    [](xms::GmTriSearch &self, py::iterable pt) -> int
    {
      xms::Pt3d p = xms::Pt3dFromPyIter(pt);
      return self.TriContainingPt(p);
    },
    tri_containing_pt_doc,py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: tri_envelopes_containing_pt
  // ---------------------------------------------------------------------------
    const char* tri_envelopes_containing_pt_doc = R"pydoc(
        Find all triangles whose envelope contains the point.

        Args:
            pt (iterable):  Location used to find a triangle.

        Returns:
            iterable: The indices to triangles whose envelope contains 
                the point

    )pydoc";

    iGmTriSearch.def("tri_envelopes_containing_pt",
    [](xms::GmTriSearch &self, py::iterable pt) -> py::iterable
    {
      xms::Pt3d p = xms::Pt3dFromPyIter(pt);
      xms::VecInt tris;
      self.TriEnvelopsContainingPt(p, tris);
      return xms::PyIterFromVecInt(tris);
    },
    tri_envelopes_containing_pt_doc,py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: tri_envelopes_overlap
  // ---------------------------------------------------------------------------
    const char* tri_envelopes_overlap_doc = R"pydoc(
        Find all triangles whose envelope overlaps the envelope defined by
        a_pMin and a_pMax

        Args:
            pt_min (iterable):  min x,y location of the box
            pt_max (iterable):  max x,y location of the box

        Returns:
            iterable: The indices to triangles whose envelope intersects with
                the input envelope.

    )pydoc";

    iGmTriSearch.def("tri_envelopes_overlap",
    [](xms::GmTriSearch &self, py::iterable pt_min, 
                            py::iterable pt_max) -> py::iterable
    {
      xms::Pt3d p_min = xms::Pt3dFromPyIter(pt_min);
      xms::Pt3d p_max = xms::Pt3dFromPyIter(pt_max);
      xms::VecInt tris;
      self.TriEnvelopesOverlap(p_min, p_max, tris);
      return xms::PyIterFromVecInt(tris);
    },
    tri_envelopes_overlap_doc, py::arg("pt_min"), py::arg("pt_min"));
  // ---------------------------------------------------------------------------
  // function: interp_weights
  // ---------------------------------------------------------------------------
    const char* interp_weights_doc = R"pydoc(
        Use the stored triangles to get interpolation weights for a point.

        Args:
            pt(iterable):  Location that is interpolated to.

        Returns:
            iterable: Contains a bool false if point is outside of the 
                triangle, an iterable of triangle point indices, and 
                an iterable of triangle point weights

    )pydoc";

    iGmTriSearch.def("interp_weights",
    [](xms::GmTriSearch &self, py::iterable pt) -> py::iterable
    {
      xms::Pt3d p = xms::Pt3dFromPyIter(pt);
      xms::VecInt idxs;
      xms::VecDbl wts;
      bool result = self.InterpWeights(p, idxs, wts);
      return py::make_tuple(
          result,
          xms::PyIterFromVecInt(tris),
          xms::PyIterFromVecDbl(wts)
      );
    },
    interp_weights_doc, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: interp_weights_triangle_idx
  // ---------------------------------------------------------------------------
    const char* interp_weights_triangle_idx_doc = R"pydoc(
        Use the stored triangles to get interpolation weights for a point.

        Args:
            pt(iterable):  Location that is interpolated to.

        Returns:
            iterable: Contains a bool false if point is outside of the 
                triangle, the found triangle index in the triangle array, 
                an iterable of triangle point indices, and an iterable 
                of triangle point weights

    )pydoc";

    iGmTriSearch.def("interp_weights",
    [](xms::GmTriSearch &self, py::iterable pt) -> py::iterable
    {
      xms::Pt3d p = xms::Pt3dFromPyIter(pt);
      int tri_idx;
      xms::VecInt idxs;
      xms::VecDbl wts;
      bool result = self.InterpWeightsTriangleIdx(p, tri_idx, idxs, wts);
      return py::make_tuple(
          result,
          tri_idx,
          xms::PyIterFromVecInt(tris),
          xms::PyIterFromVecDbl(wts)
      );
    },
    interp_weights_triangle_idx_doc, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: __str__
  // ---------------------------------------------------------------------------
    const char* str_doc = R"pydoc(
        Get the GmTriSearch as a string

        Returns:
            A string representing the GmTriSearch class.
    )pydoc";

    iGmTriSearch.def("__str__",
    [](xms::GmTriSearch &self) -> py::iterable
    {
      return self.ToString();
    },
    str_doc;
