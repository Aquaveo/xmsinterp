//------------------------------------------------------------------------------
/// \file
/// \brief
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <boost/shared_ptr.hpp>
#include <xmsinterp/interpolate/InterpBase.h>
#include <xmsinterp/python/interpolate/interpolate_py.h>

//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;


//----- Python Interface -------------------------------------------------------
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>);


////////////////////////////////////////////////////////////////////////////////
/// \class InterpBase trampoline class
/// \brief Base class for interpolation methods.
template <class InterpBase_ = xms::InterpBase> class PyInterpBase : public InterpBase_ {
public:
  using InterpBase_::InterpBase_; // Inherit constructors
  void SetPtsTris(boost::shared_ptr<std::vector<xms::Pt3d>> a_pts, boost::shared_ptr<std::vector<int>> a_tris) override {
    PYBIND11_OVERLOAD_PURE(void, InterpBase_, SetPtsTris, a_pts, a_tris);
  }
  void SetScalars(const float* a_scalar, size_t a_n) override {
    PYBIND11_OVERLOAD_PURE(void, InterpBase_, SetScalars, a_scalar, a_n);
  }
  void SetScalars(boost::shared_ptr<std::vector<float>> a_scalar) override {
    PYBIND11_OVERLOAD_PURE(void, InterpBase_, SetScalars, a_scalar);
  }
  float InterpToPt(const xms::Pt3d& a_pt) override {
    PYBIND11_OVERLOAD_PURE(float, InterpBase_, InterpToPt, a_pt);
  }
  void InterpToPts(const std::vector<xms::Pt3d>& a_pts, std::vector<float>& a_scalars) override {
    PYBIND11_OVERLOAD_PURE(void, InterpBase_, InterpToPts, a_pts, a_scalars);
  }
  void SetPtActivity(xms::DynBitset& a_activity) override {
    PYBIND11_OVERLOAD_PURE(void, InterpBase_, SetActivity, a_activity);
  }
  // bitset is number of triangles in length not numtri*3 like the tris array
  void SetTriActivity(xms::DynBitset& a_activity) override {
    PYBIND11_OVERLOAD_PURE(void, InterpBase_, SetTriActivity, a_activity);
  }
  void SetTrunc(double a_sMax, double a_sMin) override {
    PYBIND11_OVERLOAD_PURE(void, InterpBase_, SetTrunc, a_sMax, a_sMin);
  }
  boost::shared_ptr<std::vector<xms::Pt3d>> GetPts() override {
    PYBIND11_OVERLOAD_PURE(boost::shared_ptr<std::vector<xms::Pt3d>>, InterpBase_, GetPts, );
  }
  boost::shared_ptr<std::vector<int>> GetTris() override {
    PYBIND11_OVERLOAD_PURE(boost::shared_ptr<std::vector<int>>, InterpBase_, GetTris, );
  }
  std::string ToString() const override {
    PYBIND11_OVERLOAD_PURE(std::string, InterpBase_, ToString, );
  }
  void SetIdString(const std::string& a_id) override {
    PYBIND11_OVERLOAD_PURE(void, InterpBase_, SetIdString, a_id);
  }
  std::string GetIdString() const override {
    PYBIND11_OVERLOAD_PURE(std::string, InterpBase_, GetIdString, );
  }
};

void initInterpBase(py::module &m) {
    py::class_<xms::InterpBase, PyInterpBase<>, std::shared_ptr<xms::InterpBase>>
      interpBase(m, "InterpBase");
      interpBase
        .def("__str__", &xms::InterpBase::ToString)
        .def("to_string", &xms::InterpBase::ToString)
        ;
}

