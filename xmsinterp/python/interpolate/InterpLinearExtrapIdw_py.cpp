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
#include <xmsinterp/interpolate/InterpLinearExtrapIdw.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);

void initInterpLinearExtrapIdw(py::module &m) {

    py::class_<xms::InterpLinearExtrapIdw, xms::InterpBase,
        boost::shared_ptr<xms::InterpLinearExtrapIdw>> iLin(m, "InterpLinearExtrapIdw");

	// ---------------------------------------------------------------------------
	// function: __init__
	// ---------------------------------------------------------------------------
  iLin.def(py::init([](py::iterable pts, py::iterable tris, py::iterable scalars) {
    boost::shared_ptr<xms::InterpLinearExtrapIdw> rval(xms::InterpLinearExtrapIdw::New());
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
  iLin.def("SetPtsTris", [](xms::InterpLinearExtrapIdw &self, py::iterable pts, py::iterable tris) {
    BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
    BSHP<xms::VecInt> vec_tris = xms::VecIntFromPyIter(tris);
    self.SetPtsTris(vec_pts, vec_tris);
  }, py::arg("pts"),py::arg("tris"));
  // ---------------------------------------------------------------------------
  // function: SetScalars
  // ---------------------------------------------------------------------------
  iLin.def("SetScalars", [](xms::InterpLinearExtrapIdw &self, py::iterable scalar) {
    BSHP<xms::VecFlt> vec_scalars = xms::VecFltFromPyIter(scalar);
    self.SetScalars(vec_scalars);
  }, py::arg("scalar"));
  // ---------------------------------------------------------------------------
  // function: InterpToPt
  // ---------------------------------------------------------------------------
  iLin.def("InterpToPt",[](xms::InterpLinearExtrapIdw &self, py::tuple pt) -> float {
    return self.InterpToPt(xms::Pt3dFromPyIter(pt));
  }, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: InterpToPts
  // ---------------------------------------------------------------------------
  iLin.def("InterpToPts", [](xms::InterpLinearExtrapIdw &self, py::iterable pts) -> py::iterable {
    BSHP<xms::VecPt3d> vec_pts = xms::VecPt3dFromPyIter(pts);
    xms::VecFlt vec_scalars;
    self.InterpToPts(*vec_pts, vec_scalars);
    return xms::PyIterFromVecFlt(vec_scalars, py::isinstance<py::array>(pts));
  }, py::arg("pts"));
  // ---------------------------------------------------------------------------
  // function: SetPtActivity
  // ---------------------------------------------------------------------------
  iLin.def("SetPtActivity", [](xms::InterpLinearExtrapIdw &self, py::iterable activity) {
    xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
    self.SetPtActivity(bitset);
  },py::arg("activity"));
  // ---------------------------------------------------------------------------
  // function: SetTriActivity
  // ---------------------------------------------------------------------------
  iLin.def("SetTriActivity", [](xms::InterpLinearExtrapIdw &self, py::iterable activity) {
	  xms::DynBitset bitset = xms::DynamicBitsetFromPyIter(activity);
	  self.SetTriActivity(bitset);
  },py::arg("activity"));
	// ---------------------------------------------------------------------------
	// function: GetPts
	// ---------------------------------------------------------------------------
	iLin.def_property_readonly("GetPts", [](xms::InterpLinearExtrapIdw &self) -> py::iterable {
		BSHP<xms::VecPt3d> pts = self.GetPts();
		return xms::PyIterFromVecPt3d(*pts);
	});
	// ---------------------------------------------------------------------------
	// function: GetTris
	// ---------------------------------------------------------------------------
	iLin.def_property_readonly("GetTris", [](xms::InterpLinearExtrapIdw &self) -> py::iterable {
		BSHP<std::vector<int>> tris = self.GetTris();
		return xms::PyIterFromVecInt(*tris, true);
	});
  // ---------------------------------------------------------------------------
  // function: GetScalars
  // ---------------------------------------------------------------------------
  iLin.def_property_readonly("GetScalars", [](xms::InterpLinearExtrapIdw &self) -> py::iterable {
    BSHP<std::vector<float>> scalars = self.GetScalars();
    return xms::PyIterFromVecFlt(*scalars, true);
  });
  // ---------------------------------------------------------------------------
  // function: GetPtActivity
  // ---------------------------------------------------------------------------
  iLin.def_property_readonly("GetPtActivity", [](xms::InterpLinearExtrapIdw &self) -> py::iterable {
    xms::DynBitset ptActivity = self.GetPtActivity();
    return xms::PyIterFromDynamicBitset(ptActivity, true);
  });
  // ---------------------------------------------------------------------------
  // function: GetTriActivity
  // ---------------------------------------------------------------------------
  iLin.def_property_readonly("GetTriActivity", [](xms::InterpLinearExtrapIdw &self) -> py::iterable {
    xms::DynBitset triActivity = self.GetTriActivity();
    return xms::PyIterFromDynamicBitset(triActivity, true);
  });
  // ---------------------------------------------------------------------------
  // function: GetExtrapolationPointIndexes
  // ---------------------------------------------------------------------------
  iLin.def_property_readonly("GetExtrapolationPointIndexes", [](xms::InterpLinearExtrapIdw &self) -> py::iterable {
    xms::VecInt extrapPtIndexes = self.GetExtrapolationPointIndexes();
    return xms::PyIterFromVecInt(extrapPtIndexes, true);
  });
  // ---------------------------------------------------------------------------
  // function: TriContainingPt
  // ---------------------------------------------------------------------------
  iLin.def("TriContainingPt", [](xms::InterpLinearExtrapIdw &self, py::tuple pt) -> int {
    xms::Pt3d point = xms::Pt3dFromPyIter(pt);
    return self.TriContainingPt(point);
  }, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: TriEnvelopsContainingPt
  // ---------------------------------------------------------------------------
  iLin.def("TriEnvelopsContainingPt", [](xms::InterpLinearExtrapIdw &self, py::tuple pt) -> py::array {
    xms::VecInt tris;
    xms::Pt3d point = xms::Pt3dFromPyIter(pt);
    self.TriEnvelopsContainingPt(point, tris);
    return xms::PyIterFromVecInt(tris);
  }, py::arg("pt"));
  // ---------------------------------------------------------------------------
  // function: InterpWeights
  // ---------------------------------------------------------------------------
  iLin.def("InterpWeights", [](xms::InterpLinearExtrapIdw &self, py::tuple pt) -> py::iterable {
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
  iLin.def("SetExtrapVal", &xms::InterpLinearExtrapIdw::SetExtrapVal, py::arg("value"));
  // ---------------------------------------------------------------------------
  // function: SetTrunc
  // ---------------------------------------------------------------------------
  iLin.def("SetTrunc", &xms::InterpLinearExtrapIdw::SetTrunc, py::arg("smax"), py::arg("smin"));
  // ---------------------------------------------------------------------------
  // function: SetUseCloughTocher
  // ---------------------------------------------------------------------------
  iLin.def("SetUseCloughTocher", [](xms::InterpLinearExtrapIdw &self, bool on, py::object observer) {
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
  iLin.def("SetUseNatNeigh", [](xms::InterpLinearExtrapIdw &self, bool on,
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
    // function: SetIdwNodalFunction
    // ---------------------------------------------------------------------------
    iLin.def("SetIdwNodalFunction", [](xms::InterpLinearExtrapIdw &self, 
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
        self.SetIdwNodalFunc(w, n_nearest, quad_oct, obs);
      },
        py::arg("nodal_func_type") = 0,
        py::arg("num_nearest_points") = 16,
        py::arg("use_quadrant_octant_search") = false,
        py::arg("observer") = py::none()
    );
    // ---------------------------------------------------------------------------
    // function: SetIdwSearchOpts
    // ---------------------------------------------------------------------------
    iLin.def("SetIdwSearchOpts", &xms::InterpLinearExtrapIdw::SetIdwSearchOpts, py::arg("number_nearest_points"),
                py::arg("use_quadrant_octant_search"));
    // ---------------------------------------------------------------------------
    // function: GetExtrapVal
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetExtrapVal", &xms::InterpLinearExtrapIdw::SetExtrapVal);
    // ---------------------------------------------------------------------------
    // function: GetTruncateInterpolatedValues
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetTruncateInterpolatedValues", &xms::InterpLinearExtrapIdw::GetTruncateInterpolatedValues);
    // ---------------------------------------------------------------------------
    // function: GetTruncMin
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetTruncMin", &xms::InterpLinearExtrapIdw::GetTruncMin);
    // ---------------------------------------------------------------------------
    // function: GetTruncMax
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetTruncMax", &xms::InterpLinearExtrapIdw::GetTruncMax);
    // ---------------------------------------------------------------------------
    // function: GetUseCloughTocher
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetUseCloughTocher", &xms::InterpLinearExtrapIdw::GetUseCloughTocher);
    // ---------------------------------------------------------------------------
    // function: GetUseNatNeigh
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetUseNatNeigh", &xms::InterpLinearExtrapIdw::GetUseNatNeigh);
    // ---------------------------------------------------------------------------
    // function: GetNatNeighNodalFunc
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetNatNeighNodalFunc", &xms::InterpLinearExtrapIdw::GetNatNeighNodalFunc);
    // ---------------------------------------------------------------------------
    // function: GetNatNeighNodalFuncNearestPtsOption
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetNatNeighNodalFuncNearestPtsOption", &xms::InterpLinearExtrapIdw::GetNatNeighNodalFuncNearestPtsOption);
    // ---------------------------------------------------------------------------
    // function: GetNatNeighNodalFuncNumNearestPts
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetNatNeighNodalFuncNumNearestPts", &xms::InterpLinearExtrapIdw::GetNatNeighNodalFuncNumNearestPts);
    // ---------------------------------------------------------------------------
    // function: GetNatNeighBlendWeights
    // ---------------------------------------------------------------------------
    iLin.def_property_readonly("GetNatNeighBlendWeights", &xms::InterpLinearExtrapIdw::GetNatNeighBlendWeights);

}
