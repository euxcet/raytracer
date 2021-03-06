CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

PROJECT( raytracer )

SET( CORE_SOURCE
	src/core/api.cpp
	src/core/camera.cpp
	src/core/geometry.cpp
	src/core/light.cpp
	src/core/primitive.cpp
	src/core/scene.cpp
	src/core/shape.cpp
	src/core/transform.cpp
	)

set ( CORE_HEADERS
	src/core/api.h
	src/core/camera.h
	src/core/engine.h
	src/core/geometry.h
	src/core/intersection.h
	src/core/light.h
	src/core/material.h
	src/core/primitive.h
	src/core/raytracer.h
	src/core/scene.h
	src/core/shape.h
	src/core/texture.h
	src/core/transform.h
	)

FILE ( GLOB SOURCE
	src/shape/bezier.cpp
	src/shape/plane.cpp
	src/shape/sphere.cpp
	src/shape/triangle.cpp
	src/accelerator/bfaccel.cpp
	src/accelerator/kdtreeaccel.cpp
	src/engine/rt.cpp
	src/engine/ppm.cpp
 	)

INCLUDE_DIRECTORIES ( src )
INCLUDE_DIRECTORIES ( src/core )
INCLUDE_DIRECTORIES ( texture )
include_directories( /usr/local/Cellar/eigen/3.3.4/include/eigen3/ )

ADD_LIBRARY ( raytracer STATIC
	${CORE_SOURCE}
	${CORE_HEADERS}
	${SOURCE}
	)

SET( ALL_LIBS
	raytracer
	${CMAKE_THREAD_LIBS_INIT}
	)

set (CMAKE_CXX_STANDARD 11)

SET ( CMAKE_CXX_FLAGS "-std=c++11 -fopenmp ${CMAKE_CXX_FLAGS}" )

ADD_EXECUTABLE ( raytracer_exe src/main/main.cpp )

SET_TARGET_PROPERTIES ( raytracer_exe PROPERTIES OUTPUT_NAME main )
TARGET_LINK_LIBRARIES( raytracer_exe ${ALL_LIBS} )

INSTALL ( TARGETS
	raytracer_exe
	DESTINATION
	bin
	)

INSTALL ( TARGETS
	raytracer
	DESTINATION
	lib
	)
