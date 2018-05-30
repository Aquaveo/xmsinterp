#pragma once
//------------------------------------------------------------------------------
/// \file
/// \brief initializer functions for members of interpolate python module.
/// \copyright (C) Copyright Aquaveo 2018. Distributed under the xmsng
///  Software License, Version 1.0. (See accompanying file
///  LICENSE_1_0.txt or copy at http://www.aquaveo.com/xmsng/LICENSE_1_0.txt)
//------------------------------------------------------------------------------

//----- Included files ---------------------------------------------------------
#include <pybind11/pybind11.h>
#include <xmscore/misc/Observer.h>


//----- Namespace declaration --------------------------------------------------
namespace py = pybind11;

//----- Function declarations --------------------------------------------------
void initMisc(py::module &);

void initObserver(py::module &);


class PyObserver : public xms::Observer {

public:
  /* Inherit the constructors */
  using xms::Observer::Observer;

private:
  /* Trampoline (need one for each virtual function) */
  void OnProgressStatus(double a_percentComplete) override {
    PYBIND11_OVERLOAD_PURE_NAME(
      void,                 /* Return type */
      xms::Observer,        /* Parent class */
      "on_progress_status", /* Python name */
      OnProgressStatus,     /* Name of function in C++ */
      a_percentComplete     /* Argument(s) */
    );
  }
  void OnBeginOperationString(const std::string& a_operation) override {
    PYBIND11_OVERLOAD_NAME(
      void,                 /* Return type */
      xms::Observer,        /* Parent class */
      "on_begin_operation_string", /* Python name */
      OnBeginOperationString,      /* Name of function in C++ */
      a_operation           /* Argument(s) */
    );
  }
  void OnEndOperation() override {
    PYBIND11_OVERLOAD_NAME(
      void,                 /* Return type */
      xms::Observer,        /* Parent class */
      "on_end_operation",   /* Python name */
      OnEndOperation,       /* Name of function in C++ */
    );
  }
  void OnUpdateMessage(const std::string& a_message) override {
    PYBIND11_OVERLOAD_NAME(
      void,                 /* Return type */
      xms::Observer,        /* Parent class */
      "on_update_message",  /* Python name */
      OnUpdateMessage,      /* Name of function in C++ */
      a_message             /* Argument(s) */
    );
  }
  void TimeRemainingInSeconds(double a_remainingSeconds) override {
    PYBIND11_OVERLOAD_NAME(
      void,                 /* Return type */
      xms::Observer,        /* Parent class */
      "time_remaining_in_seconds",  /* Python name */
      TimeRemainingInSeconds,       /* Name of function in C++ */
      a_remainingSeconds    /* Argument(s) */
    );
  }

  void TimeElapsedInSeconds(double a_elapsedSeconds) override {
    PYBIND11_OVERLOAD_NAME(
      void,                 /* Return type */
      xms::Observer,        /* Parent class */
      "time_elapsed_in_seconds",  /* Python name */
      TimeElapsedInSeconds,       /* Name of function in C++ */
      a_elapsedSeconds      /* Argument(s) */
    );
  }
};
