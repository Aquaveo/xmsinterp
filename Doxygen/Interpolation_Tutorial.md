\tableofcontents
# Interpolation Tutorial {#Interpolation_Tutorial}

## Introduction {#Intro_Interpolation}
The purpose of this tutorial is to provide explanation on how to use the classes defined in xmsinterp to perform spatial interpolation. The examples provided in this tutorial refer to test cases that are in the xmsinterp/tutorial/TutInterpolation.cpp source file.

The xmsinterp library contains classes for performing 2d (x,y) linear, natural neighbor, and idw interpolation: xms::InterpLinear and xms::InterpIdw. Natural neighbor interpolation is performed using the linear interpolation class. 3d (x,y,z) interpolation can be performed using the idw class. 

Both interpolators are derived off of xms::InterpBase. This class has methods for setting the points and triangles used to interpolate as well as methods to set scalar values and activity on points and triangles (you can imagine activity as points or cells that may have a “dry” value when the scalars are generated from a numerical model). At a minimum, you must specify the points to set up an interpolation class. The InterpLinear class requires triangles to perform the interpolation. If none are specified then the points are triangulated using xms::TrTriangulator;

## Example - Inverse Distance Weighted (IDW) Interpolation {#Example_IDW}
The following example shows how to use the IDW interpolator. The testing code for this example is TutInterpolationTests::test_Example_IDW.

\snippet xmsinterp/tutorial/TutInterpolation.cpp snip_test_Example_Idw

## Example - Linear Interpolation {#Example_Linear}
The following example shows how to use the Linear interpolator. The testing code for this example is TutInterpolationTests::test_Example_Linear.

\snippet xmsinterp/tutorial/TutInterpolation.cpp snip_test_Example_Linear

## Example - Natural Neighbor Interpolation {#Example_NatNeigh}
The following example shows how to use the Natural Neighbor interpolator. This is an option that can be set on the xms::InterpLinear class. The testing code for this example is TutInterpolationTests::test_Example_NaturalNeighbor.

\snippet xmsinterp/tutorial/TutInterpolation.cpp snip_test_Example_NatNeigh
