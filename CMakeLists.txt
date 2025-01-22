# This is a generated file.
# To regenerate file, use generators in the aquaveo/dev_tools repository.
# Any changes needed to be made to the file should be made in the template file.

cmake_minimum_required(VERSION 3.11.0)
cmake_policy(SET CMP0015 NEW) # Link Directory Pathing
cmake_policy(SET CMP0091 NEW)
set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_CXX_STANDARD 17)

project(xmsinterplib C CXX)

# Cache variables
set(BUILD_TESTING NO CACHE BOOL "Enable/Disable testing")
set(IS_CONDA_BUILD NO CACHE BOOL "Set this if you want to make a conda package.")
set(IS_EMSCRIPTEN_BUILD NO CACHE BOOL "Set this to build with emscripten using cmake.")
set(PYTHON_TARGET_VERSION 3.10 CACHE STRING "Version of python to link to for python wrapping.")
set(CONDA_PREFIX "" CACHE PATH "Path to the conda environment used to build.")
set(IS_PYTHON_BUILD NO CACHE BOOL "Set this if you want to build the python bindings.")
set(XMS_TEST_PATH ${PROJECT_SOURCE_DIR}/test_files/ CACHE PATH "Path to test files for testing")
set(XMS_VERSION "99.99.99" CACHE STRING "Library Version")

# Static library sources
set(xmsinterp_sources
    xmsinterp/interpolate/InterpAnisotropic.cpp
    xmsinterp/interpolate/InterpIdw.cpp
    xmsinterp/interpolate/InterpLinear.cpp
    xmsinterp/interpolate/InterpLinearExtrapIdw.cpp
    xmsinterp/interpolate/InterpSignals.cpp
    xmsinterp/interpolate/InterpUtil.cpp
    xmsinterp/interpolate/detail/AnisotropicInterpolator.cpp
    xmsinterp/interpolate/detail/InterpCt.cpp
    xmsinterp/interpolate/detail/InterpNatNeigh.cpp
    xmsinterp/interpolate/detail/NodalFunc.cpp
    xmsinterp/thread/ThreadLoop.cpp
    xmsinterp/thread/ThreadMgr.cpp
    xmsinterp/tutorial/TutInterpolation.cpp
)

# Static library headers
set(xmsinterp_headers
    xmsinterp/interpolate/InterpAnisotropic.h
    xmsinterp/interpolate/InterpBase.h
    xmsinterp/interpolate/InterpIdw.h
    xmsinterp/interpolate/InterpLinear.h
    xmsinterp/interpolate/InterpLinearExtrapIdw.h
    xmsinterp/interpolate/InterpSignals.h
    xmsinterp/interpolate/InterpUtil.h
    xmsinterp/interpolate/detail/AnisotropicInterpolator.h
    xmsinterp/interpolate/detail/InterpCt.h
    xmsinterp/interpolate/detail/InterpNatNeigh.h
    xmsinterp/interpolate/detail/NodalFunc.h
    xmsinterp/thread/ThreadLoop.h
    xmsinterp/thread/ThreadMgr.h
)

if (APPLE OR WIN32 OR IS_EMSCRIPTEN_BUILD)
    # Target C++17 on MacOS and Windows. If building with version of Visual Studio without
    # support, has no effect.
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 11)
endif ()

if (APPLE)
    set(CMAKE_POSITION_INDEPENDENT_CODE False)
else ()
    set(CMAKE_POSITION_INDEPENDENT_CODE True)
endif ()

if (IS_PYTHON_BUILD AND BUILD_TESTING)
    message(FATAL_ERROR "Cannot build python module when testing is enabled")
endif ()

add_definitions(-DXMS_VERSION=\"${XMS_VERSION}\")

if (IS_CONDA_BUILD)
    include(${CMAKE_CURRENT_LIST_DIR}/condabuildinfo.cmake)
elseif (IS_EMSCRIPTEN_BUILD)
    include(${CMAKE_CURRENT_LIST_DIR}/emscriptenbuildinfo.cmake)
else () # If we are not using conda or emscripten, we are using conan
    # Conan 2 setup
    find_package(xmscore REQUIRED)
    find_package(xmsgrid REQUIRED)
    find_package(Boost REQUIRED)
    find_package(ZLIB REQUIRED)
    list(APPEND EXT_INCLUDE_DIRS ${xmscore_INCLUDE_DIRS})
    list(APPEND EXT_INCLUDE_DIRS ${xmsgrid_INCLUDE_DIRS})
    list(APPEND EXT_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
    list(APPEND EXT_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS})
    list(APPEND EXT_LIB_DIRS ${xmscore_LIBRARY_DIRS})
    list(APPEND EXT_LIB_DIRS ${xmsgrid_LIBRARY_DIRS})
    list(APPEND EXT_LIB_DIRS ${Boost_LIBRARY_DIRS})
    list(APPEND EXT_LIB_DIRS ${ZLIB_LIBRARY_DIRS})
    list(APPEND EXT_LIBS ${xmscore_LIBRARIES})
    list(APPEND EXT_LIBS ${xmsgrid_LIBRARIES})
    list(APPEND EXT_LIBS ${Boost_LIBRARIES})
    list(APPEND EXT_LIBS ${ZLIB_LIBRARIES})
endif ()

if (WIN32)
    string(COMPARE EQUAL "${CONAN_SETTINGS_COMPILER_RUNTIME}" "MT" USES_MT)
    if (NOT USES_MT)
        string(COMPARE EQUAL "${CONAN_SETTINGS_COMPILER_RUNTIME}" "MTd" USES_MT)
    endif ()

    if (USE_TYPEDEF_WCHAR_T)
        add_definitions(/Zc:wchar_t-)  # Treat wchar_t as a typedef
    endif ()

    add_definitions(/D BOOST_ALL_NO_LIB)

    if (USES_MT)
        set(CompilerFlags
                CMAKE_CXX_FLAGS
                CMAKE_CXX_FLAGS_DEBUG
                CMAKE_CXX_FLAGS_RELEASE
                CMAKE_C_FLAGS
                CMAKE_C_FLAGS_DEBUG
                CMAKE_C_FLAGS_RELEASE
        )
        foreach (CompilerFlag ${CompilerFlags})
            string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
        endforeach ()
    endif ()
endif ()

if (IS_PYTHON_BUILD)

    find_package(pybind11 REQUIRED)
    list(APPEND EXT_INCLUDE_DIRS
            ${pybind11_INCLUDE_DIR}
    )

    find_package(PythonLibsNew ${PYTHON_TARGET_VERSION} EXACT REQUIRED)

    # Have to add this after conda because it doesn't get the path for pybind if we don't.
    list(APPEND EXT_INCLUDE_DIRS
            ${PYTHON_INCLUDE_DIRS}
    )
endif ()

if (BUILD_TESTING)
    message(STATUS "Adding test dependencies...")
    add_definitions(-DXMS_TEST_PATH="${XMS_TEST_PATH}/")
    add_definitions(-DCXX_TEST -DCXXTEST4)

    list(APPEND xmsinterp_headers
         xmsinterp/interpolate/InterpAnisotropic.t.h
         xmsinterp/interpolate/InterpIdw.t.h
         xmsinterp/interpolate/InterpLinear.t.h
         xmsinterp/interpolate/InterpLinearExtrapIdw.t.h
         xmsinterp/interpolate/InterpUtil.t.h
         xmsinterp/interpolate/detail/AnisotropicInterpolator.t.h
         xmsinterp/interpolate/detail/InterpNatNeigh.t.h
         xmsinterp/interpolate/detail/NodalFunc.t.h
         xmsinterp/tutorial/TutInterpolation.t.h
    )

    find_package(cxxtest REQUIRED)
    list(APPEND EXT_INCLUDE_DIRS ${cxxtest_INCLUDE_DIRS})
    list(APPEND EXT_LIB_DIRS ${cxxtest_LIBRARY_DIRS})
    list(APPEND EXT_LIBS ${cxxtest_LIBRARIES})

    if (cxxtest_FOUND)
        include("FindCxxTest")
        include_directories(${CXXTEST_INCLUDE_DIR})
        enable_testing()

        set(cxxtest_TESTGEN_ARGS --xunit-printer --have-eh)
        file(GLOB_RECURSE test_headers ${CMAKE_CURRENT_LIST_DIR}/xmsinterp/*.t.h)
        message(STATUS "Prefix Path: ${CMAKE_PREFIX_PATH}")
        message(STATUS "Module Path: ${CMAKE_MODULE_PATH}")
        CXXTEST_ADD_TEST(
                runner runner.cpp ${test_headers}
        )
        target_link_libraries(runner ${PROJECT_NAME})

        if (WIN32)
            set(RUNNERNAME "runner.exe")
        else ()
            set(RUNNERNAME "runner")
        endif ()
    endif ()
endif ()


message(STATUS "External Include Dirs: ${EXT_INCLUDE_DIRS}")
message(STATUS "External Lib Dirs: ${EXT_LIB_DIRS}")
message(STATUS "External Libs: ${EXT_LIBS}")


include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${EXT_INCLUDE_DIRS})
link_directories(${EXT_LIB_DIRS})


message("External Include Dirs: ${EXT_INCLUDE_DIRS}")
message("External Lib Dirs: ${EXT_LIB_DIRS}")
message("Extneral Libs: ${EXT_LIBS}")

# Static library
add_library(${PROJECT_NAME} STATIC
        ${xmsinterp_sources} ${xmsinterp_headers}
)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME}
        ${EXT_LIBS}
)
target_link_libraries(${PROJECT_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
)
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} rt)
endif ()


# Pybind11
if (IS_PYTHON_BUILD)
    # Pybind11 sources
    set(xmsinterp_py
        xmsinterp/python/xmsinterp_py.cpp
        xmsinterp/python/interpolate/interpolate_py.cpp
        xmsinterp/python/interpolate/InterpAnisotropic_py.cpp
        xmsinterp/python/interpolate/InterpBase_py.cpp
        xmsinterp/python/interpolate/InterpIdw_py.cpp
        xmsinterp/python/interpolate/InterpLinear_py.cpp
        xmsinterp/python/interpolate/InterpLinearExtrapIdw_py.cpp
        xmsinterp/python/interpolate/InterpUtil_py.cpp
    )

    set(xmsinterp_py_headers
        xmsinterp/python/interpolate/interpolate_py.h
    )

    message(STATUS "PYTHON_INCLUDE: ${PYTHON_INCLUDE_DIRS}")
    message(STATUS "PYTHON_LIBS: ${PYTHON_LIBRARIES}")

    pybind11_add_module(_xmsinterp
            ${xmsinterp_py} ${xmsinterp_py_headers}
    )
    target_include_directories(_xmsinterp
            PRIVATE
            ${EXT_INCLUDE_DIRS}
    )
    target_link_libraries(_xmsinterp
            PRIVATE
            ${EXT_LIBS}
            ${PROJECT_NAME}
    )
    set_target_properties(_xmsinterp PROPERTIES
            LINKER_LANGUAGE CXX
    )

    install(
            TARGETS _xmsinterp
            ARCHIVE DESTINATION "_package/xms/interp"
            LIBRARY DESTINATION "_package/xms/interp"
    )

    install(DIRECTORY "_package/" DESTINATION "_package"
            FILES_MATCHING PATTERN "*.py"
            PATTERN "_package/tests" EXCLUDE)
endif ()


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
