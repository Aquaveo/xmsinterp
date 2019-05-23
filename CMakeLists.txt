set(CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.1.2)
cmake_policy(SET CMP0015 NEW) # Link Directory Pathing
set(CMAKE_DEBUG_POSTFIX _d)
if (APPLE)
	set(CMAKE_POSITION_INDEPENDENT_CODE False)
else()
	set(CMAKE_POSITION_INDEPENDENT_CODE True)
endif()

project(xmsinterplib C CXX)

set(BUILD_TESTING NO CACHE BOOL "Enable/Disable testing")
set(IS_CONDA_BUILD NO CACHE BOOL "Set this if you want to make a conda package.")
set(PYTHON_TARGET_VERSION 3.6 CACHE STRING "Version of python to link to for python wrapping.")
set(CONDA_PREFIX "" CACHE PATH "Path to the conda environment used to build.")
set(IS_PYTHON_BUILD NO CACHE BOOL "Set this if you want to build the python bindings.")
set(XMS_TEST_PATH ${PROJECT_SOURCE_DIR}/test_files/ CACHE PATH "Path to test files for testing")
set(XMS_VERSION "\"99.99.99\"" CACHE STRING "Library Version")

add_definitions(-DXMS_VERSION=\"${XMS_VERSION}\")

if(IS_PYTHON_BUILD AND BUILD_TESTING)
  message(FATAL_ERROR "Cannot build python module when testing is enabled")
endif()

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

if(IS_PYTHON_BUILD)
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
    
    find_package(PythonLibs ${PYTHON_TARGET_VERSION} EXACT REQUIRED)

    message("PYTHON_INCLUDE: ${PYTHON_INCLUDE_DIRS}")
    message("PYTHON_LIBS: ${PYTHON_LIBRARIES}")

    list(APPEND EXT_INCLUDE_DIRS
      ${PYTHON_INCLUDE_DIRS}
    )
endif()

message("External Include Dirs: ${EXT_INCLUDE_DIRS}")
message("External Lib Dirs: ${EXT_LIB_DIRS}")
message("Extneral Libs: ${EXT_LIBS}")

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${EXT_INCLUDE_DIRS})
link_directories(${EXT_LIB_DIRS})

# Static library sources
set(xmsinterp_sources
  # Interpolate
  xmsinterp/interpolate/InterpAnisotropic.cpp
  xmsinterp/interpolate/InterpIdw.cpp
  xmsinterp/interpolate/InterpLinear.cpp
  xmsinterp/interpolate/InterpSignals.cpp
  xmsinterp/interpolate/InterpUtil.cpp
  xmsinterp/interpolate/detail/AnisotropicInterpolator.cpp
  xmsinterp/interpolate/detail/InterpCt.cpp
  xmsinterp/interpolate/detail/InterpNatNeigh.cpp
  xmsinterp/interpolate/detail/NodalFunc.cpp
  # Thread
  xmsinterp/thread/ThreadLoop.cpp
  xmsinterp/thread/ThreadMgr.cpp
  # Tutorial
  xmsinterp/tutorial/TutInterpolation.cpp
)

set(xmsinterp_headers
  # Interpolate
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
  # Thread
  xmsinterp/thread/ThreadLoop.h
  xmsinterp/thread/ThreadMgr.h
)

# Pybind11 sources
set(xmsinterp_py
  xmsinterp/python/xmsinterp_py.cpp
  # Interpolate
  xmsinterp/python/interpolate/interpolate_py.cpp
  xmsinterp/python/interpolate/InterpAnisotropic_py.cpp
  xmsinterp/python/interpolate/InterpBase_py.cpp
  xmsinterp/python/interpolate/InterpIdw_py.cpp
  xmsinterp/python/interpolate/InterpLinear_py.cpp
)
set(xmsinterp_py_headers
  xmsinterp/python/interpolate/interpolate_py.h
)

# Tests
if (BUILD_TESTING)
 add_definitions(-DXMS_TEST_PATH="${XMS_TEST_PATH}/")
 add_definitions(-DCXX_TEST -DCXXTEST4)

 list(APPEND xmsinterp_sources
   xmsinterp/interpolate/InterpAnisotropic.t.h
   xmsinterp/interpolate/InterpIdw.t.h
   xmsinterp/interpolate/InterpLinear.t.h
   xmsinterp/interpolate/detail/AnisotropicInterpolator.t.h
   xmsinterp/interpolate/detail/InterpNatNeigh.t.h
   xmsinterp/interpolate/detail/NodalFunc.t.h
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
    pybind11_add_module(_xmsinterp
      ${xmsinterp_py} ${xmsinterp_py_headers}
    )
    target_include_directories(_xmsinterp
        PRIVATE
	      ${EXT_LIBS}
          ${PYTHON_INCLUDE_DIRS}
    )
    target_link_libraries(_xmsinterp
      PRIVATE
        ${EXT_LIBS}
        ${PROJECT_NAME}
    )
    set_target_properties(_xmsinterp PROPERTIES
      LINKER_LANGUAGE CXX
    )

    # Install recipe
    install(
      TARGETS _xmsinterp
      ARCHIVE DESTINATION "_package/xms/interp"
      LIBRARY DESTINATION "_package/xms/interp"
    )

  install(DIRECTORY "_package/" DESTINATION "_package"
           FILES_MATCHING PATTERN "*.py"
           PATTERN "_package/tests" EXCLUDE)
endif()

# Disable Optimization for GCC6 for now... Report issue to gcc and pybind
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND IS_PYTHON_BUILD)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 6 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7)
    Message("Disabling Optimization for GCC 6...")
    STRING(REPLACE "-O3" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
    STRING(REPLACE "-O3" "" CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
  endif()
endif()

# Install recipe
install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
)
foreach (header IN LISTS xmsinterp_headers xmsinterp_py_headers)
  get_filename_component(subdir "${header}" DIRECTORY)
  install(
    FILES "${header}"
    DESTINATION "include/${subdir}"
  )
endforeach ()

