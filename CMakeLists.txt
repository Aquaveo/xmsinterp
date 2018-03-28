set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 3.1.2)
cmake_policy(SET CMP0015 NEW) # Link Directory Pathing
set(CMAKE_DEBUG_POSTFIX _d)

project(xmsinterp)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${CONAN_INCLUDE_DIRS})



set(xmsinterp_sources
  xmsinterp/geometry/geoms.cpp
  xmsinterp/geometry/GmExtents.cpp
  xmsinterp/geometry/GmMultiPolyIntersectionSorterTerse.cpp
  xmsinterp/geometry/GmMultiPolyIntersector.cpp
  xmsinterp/geometry/GmPolygon.cpp
  xmsinterp/geometry/GmPolyLinePtRedistributer.cpp
  xmsinterp/geometry/GmPtSearch.cpp
  xmsinterp/geometry/GmTriSearch.cpp
  xmsinterp/interpolate/InterpIdw.cpp
  xmsinterp/interpolate/InterpLinear.cpp
  xmsinterp/interpolate/InterpSignals.cpp
  xmsinterp/interpolate/InterpUtil.cpp
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
  xmsinterp/interpolate/InterpBase.h
  xmsinterp/interpolate/InterpIdw.h
  xmsinterp/interpolate/InterpLinear.h
  xmsinterp/interpolate/InterpSignals.h
  xmsinterp/interpolate/InterpUtil.h
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
    xmsinterp/interpolate/InterpIdw.t.h
    xmsinterp/interpolate/InterpLinear.t.h
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

    set(CXXTEST_TESTGEN_ARGS --xunit-printer)
    file(GLOB_RECURSE test_headers ${CMAKE_CURRENT_LIST_DIR}/xmsinterp/*.t.h)
    CXXTEST_ADD_TEST(
      runner runner.cpp ${test_headers}
    )
    target_link_libraries(runner ${PROJECT_NAME})
  endif()
endif ()

add_library(${PROJECT_NAME} STATIC
  ${xmsinterp_sources} ${xmsinterp_headers})
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/>
    ${Boost_INCLUDE_DIR})
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
target_link_libraries(${PROJECT_NAME}
  ${CMAKE_THREAD_LIBS_INIT}
)

install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
foreach (header IN LISTS xmsinterp_headers)
  get_filename_component(subdir "${header}" DIRECTORY)
  install(
    FILES "${header}"
    DESTINATION "include/xmsinterp/${subdir}"
  )
endforeach ()
