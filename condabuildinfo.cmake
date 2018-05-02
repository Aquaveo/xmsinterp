set(BOOST_ROOT ${CMAKE_INSTALL_PREFIX})
set(Boost_USE_STATIC_LIBS TRUE)
find_package(Boost REQUIRED system filesystem timer serialization log_setup log)
set(EXT_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
set(EXT_LIB_DIRS ${Boost_LIBRARY_DIRS})
set(EXT_LIBS ${Boost_LIBRARIES})

list(APPEND EXT_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include/xmscore)
list(APPEND EXT_LIBS xmscore)