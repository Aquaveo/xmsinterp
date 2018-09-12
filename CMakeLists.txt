set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.1.2)
cmake_policy(SET CMP0015 NEW) # Link Directory Pathing
set(CMAKE_DEBUG_POSTFIX _d)
if (APPLE)
	set(CMAKE_POSITION_INDEPENDENT_CODE False)
else()
	set(CMAKE_POSITION_INDEPENDENT_CODE True)
endif()

set(BUILD_TESTING NO CACHE BOOL "Enable/Disable testing")
set(IS_CONDA_BUILD NO CACHE BOOL "Set this if you want to make a conda package.")
set(PYTHON_TARGET_VERSION 3.6 CACHE STRING "Version of python to link to for python wrapping.")
set(CONDA_PREFIX "" CACHE PATH "Path to the conda environment used to build.")
set(IS_PYTHON_BUILD NO CACHE BOOL "Set this if you want to build the python bindings.")

if(IS_PYTHON_BUILD AND BUILD_TESTING)
  message(FATAL_ERROR "Cannot build python module when testing is enabled")
endif()

project(xmsinterp C CXX)

if(WIN32)
    if(XMS_BUILD)
        add_definitions(/D _WIN32_WINNT=0x0501)  # Windows XP and higher
        add_definitions(/Zc:wchar_t-)  # Treat wchar_t as built-in type
    else(NOT XMS_BUILD)
        add_definitions(/D BOOST_ALL_NO_LIB)
    endif()
endif()

if(IS_CONDA_BUILD)
  include(${CMAKE_CURRENT_LIST_DIR}/condabuildinfo.cmake)
else() # If we are not using conda, we are using conan
  # Conan setup
  include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
  conan_basic_setup(TARGETS)
  set(EXT_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS})
  set(EXT_LIB_DIRS ${CONAN_LIB_DIRS})
  set(EXT_LIBS ${CONAN_LIBS})
endif(IS_CONDA_BUILD)

message("External Include Dirs: ${EXT_INCLUDE_DIRS}")
message("External Lib Dirs: ${EXT_LIB_DIRS}")
message("Extneral Libs: ${EXT_LIBS}")

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${EXT_INCLUDE_DIRS})
link_directories(${EXT_LIB_DIRS})

# Static library sources
set(xmsinterp_sources
  xmsinterp/geometry/geoms.cpp
  xmsinterp/geometry/GmExtents.cpp
  xmsinterp/geometry/GmMultiPolyIntersectionSorterTerse.cpp
  xmsinterp/geometry/GmMultiPolyIntersector.cpp
  xmsinterp/geometry/GmPolygon.cpp
  xmsinterp/geometry/GmPolyLinePtRedistributer.cpp
  xmsinterp/geometry/GmPtSearch.cpp
  xmsinterp/geometry/GmTriSearch.cpp
  xmsinterp/interpolate/InterpAnisotropic.cpp
  xmsinterp/interpolate/InterpIdw.cpp
  xmsinterp/interpolate/InterpLinear.cpp
  xmsinterp/interpolate/InterpSignals.cpp
  xmsinterp/interpolate/InterpUtil.cpp
  xmsinterp/interpolate/detail/AnisotropicInterpolator.cpp
  xmsinterp/interpolate/detail/InterpCt.cpp
  xmsinterp/interpolate/detail/InterpNatNeigh.cpp
  xmsinterp/interpolate/detail/NodalFunc.cpp
  xmsinterp/matrices/matrix.cpp
  xmsinterp/thread/ThreadLoop.cpp
  xmsinterp/thread/ThreadMgr.cpp
  xmsinterp/triangulate/TrBreaklineAdder.cpp
  xmsinterp/triangulate/triangles.cpp
  xmsinterp/triangulate/TrTin.cpp
  xmsinterp/triangulate/TrTriangulator.cpp
  xmsinterp/triangulate/TrTriangulatorPoints.cpp
  xmsinterp/triangulate/detail/TrAutoFixFourTrianglePts.cpp
  xmsinterp/triangulate/detail/triangulate.cpp
  xmsinterp/triangulate/detail/TrOuterTriangleDeleter.cpp
  xmsinterp/tutorial/TutInterpolation.cpp
)

set(xmsinterp_headers
  xmsinterp/geometry/geoms.h
  xmsinterp/geometry/GmBoostTypes.h
  xmsinterp/geometry/GmExtents.h
  xmsinterp/geometry/GmMultiPolyIntersectionSorter.h
  xmsinterp/geometry/GmMultiPolyIntersectionSorterTerse.h
  xmsinterp/geometry/GmMultiPolyIntersector.h
  xmsinterp/geometry/GmMultiPolyIntersectorData.h
  xmsinterp/geometry/GmPolygon.h
  xmsinterp/geometry/GmPolyLinePtRedistributer.h
  xmsinterp/geometry/GmPtSearch.h
  xmsinterp/geometry/GmTriSearch.h
  xmsinterp/interpolate/InterpAnisotropic.h
  xmsinterp/interpolate/InterpBase.h
  xmsinterp/interpolate/InterpIdw.h
  xmsinterp/interpolate/InterpLinear.h
  xmsinterp/interpolate/InterpSignals.h
  xmsinterp/interpolate/InterpUtil.h
  xmsinterp/interpolate/detail/AnisotropicInterpolator.h
  xmsinterp/interpolate/detail/InterpCt.h
  xmsinterp/interpolate/detail/InterpNatNeigh.h
  xmsinterp/interpolate/detail/NodalFunc.h
  xmsinterp/matrices/matrix.h
  xmsinterp/thread/ThreadLoop.h
  xmsinterp/thread/ThreadMgr.h
  xmsinterp/triangulate/TrBreaklineAdder.h
  xmsinterp/triangulate/triangles.h
  xmsinterp/triangulate/TrTin.h
  xmsinterp/triangulate/TrTriangulator.h
  xmsinterp/triangulate/TrTriangulatorPoints.h
  xmsinterp/triangulate/detail/TrAutoFixFourTrianglePts.h
  xmsinterp/triangulate/detail/triangulate.h
  xmsinterp/triangulate/detail/TrOuterTriangleDeleter.h
)

# Pybind11 sources
set(xmsinterp_py
  xmsinterp/python/xmsinterp_py.cpp
  # Interpolate
  xmsinterp/python/interpolate/interpolate_py.h
  xmsinterp/python/interpolate/interpolate_py.cpp
  xmsinterp/python/interpolate/InterpAnisotropic_py.cpp
  xmsinterp/python/interpolate/InterpBase_py.cpp
  xmsinterp/python/interpolate/InterpIdw_py.cpp
  xmsinterp/python/interpolate/InterpLinear_py.cpp
  # Triangulate
  xmsinterp/python/triangulate/triangulate_py.h
  xmsinterp/python/triangulate/triangulate_py.cpp
  xmsinterp/python/triangulate/TrTin_py.cpp
  xmsinterp/python/triangulate/TrTriangulatorPoints_py.cpp
)

# Tests
if (BUILD_TESTING)
 add_definitions(-DCXX_TEST -DCXXTEST4)

 list(APPEND xmsinterp_sources
   xmsinterp/geometry/geoms.t.h
   xmsinterp/geometry/GmExtents.t.h
   xmsinterp/geometry/GmMultiPolyIntersector.t.h
   xmsinterp/geometry/GmPolygon.t.h
   xmsinterp/geometry/GmPolyLinePtRedistributer.t.h
   xmsinterp/geometry/GmPtSearch.t.h
   xmsinterp/geometry/GmTriSearch.t.h
   xmsinterp/interpolate/InterpAnisotropic.t.h
   xmsinterp/interpolate/InterpIdw.t.h
   xmsinterp/interpolate/InterpLinear.t.h
   xmsinterp/interpolate/detail/AnisotropicInterpolator.t.h
   xmsinterp/interpolate/detail/InterpNatNeigh.t.h
   xmsinterp/interpolate/detail/NodalFunc.t.h
   xmsinterp/triangulate/TrBreaklineAdder.t.h
   xmsinterp/triangulate/TrTin.t.h
   xmsinterp/triangulate/TrTriangulatorPoints.t.h
   xmsinterp/triangulate/detail/TrAutoFixFourTrianglePts.t.h
   xmsinterp/triangulate/detail/TrOuterTriangleDeleter.t.h
   xmsinterp/tutorial/TutInterpolation.t.h
 )

 find_package(CxxTest)
 if(CXXTEST_FOUND)
   include_directories(${CXXTEST_INCLUDE_DIR})
   enable_testing()

   set(CXXTEST_TESTGEN_ARGS --xunit-printer --have-eh)
   file(GLOB_RECURSE test_headers ${CMAKE_CURRENT_LIST_DIR}/xmsinterp/*.t.h)
   CXXTEST_ADD_TEST(
     runner runner.cpp ${test_headers}
   )
   target_link_libraries(runner ${PROJECT_NAME})
 endif()
endif ()

# Static library
add_library(${PROJECT_NAME} STATIC
  ${xmsinterp_sources} ${xmsinterp_headers}
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/>
    ${Boost_INCLUDE_DIR}
 )
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}
  ${EXT_LIBS}
)
target_link_libraries(${PROJECT_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
)
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} rt)
endif()

if(IS_PYTHON_BUILD) 
    # linux and mac builds for conan (on TRAVISCI) use a docker that has python
    # 2.7 as system python. We do not have control over that docker image so we
    # can't change this and it is fine for building conan packages and checking
    # that we don't have errors in the python wrapping. We have conda recipes
    # for building python packages that target other versions of python.
    find_package(PythonLibs ${PYTHON_TARGET_VERSION} EXACT REQUIRED)

    message("PYTHON_INCLUDE: ${PYTHON_INCLUDE_DIRS}")
    message("PYTHON_LIBS: ${PYTHON_LIBRARIES}")

    # Pybind11 module
    if(IS_CONDA_BUILD)
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11Targets.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/FindPythonLibsNew.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11Config.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11ConfigVersion.cmake")
        include("${CONDA_PREFIX}/share/cmake/pybind11/pybind11Tools.cmake")
    else()
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11Targets.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/FindPythonLibsNew.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11Config.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11ConfigVersion.cmake")
        include("${CONAN_PYBIND11_ROOT}/share/cmake/pybind11/pybind11Tools.cmake")
    endif()

    pybind11_add_module(xmsinterp_py
      ${xmsinterp_py} 
    )
    target_include_directories(xmsinterp_py
        PRIVATE
	  ${EXT_LIBS}
          ${PYTHON_INCLUDE_DIRS}
    )
    target_link_libraries(xmsinterp_py
      PRIVATE
        ${EXT_LIBS}
        ${PROJECT_NAME}
    )
    set_target_properties(xmsinterp_py PROPERTIES
      LINKER_LANGUAGE CXX
    )

    execute_process(COMMAND python -c "import os; print('${PYTHON_SITE_PACKAGES}'.replace('${CONDA_PREFIX}' + os.sep, ''))" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES_TRIMMED OUTPUT_STRIP_TRAILING_WHITESPACE)
    message("Site Packages: ${PYTHON_SITE_PACKAGES_TRIMMED}")

    # Install recipe
    install(
      TARGETS xmsinterp_py
      ARCHIVE DESTINATION ${PYTHON_SITE_PACKAGES_TRIMMED}
      LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES_TRIMMED}
    )

endif()


# Install recipe
install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)
foreach (header IN LISTS xmsinterp_headers)
  get_filename_component(subdir "${header}" DIRECTORY)
  install(
    FILES "${header}"
    DESTINATION "include/${subdir}"
  )
endforeach ()




