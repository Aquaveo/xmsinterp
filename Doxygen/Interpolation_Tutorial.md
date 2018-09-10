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

## Example - Anisotropic Interpolation {#Example_Anisotropic}
The following example shows how to use the Anisotropic Interpolator. The testing code for this example is TutInterpolationIntermediateTests::test_Example_Anisotropic.

\snippet xmsinterp/tutorial/TutInterpolation.cpp snip_test_Example_Anisotropic

The data for this example is shown in the plot below.  The upper figure shows the original centerline, the interpolation points, and the points to be interpolated to (with the interpolation values that would result from a normal Inverse Distance Weighted interpolation).  The lower figure shows the same points transformed into the space where all points are relative to their distance along and from the centerline and with the interpolation values that result from the additional scaling in the x direction.

![Anisotropic (scale = 0.5) vs IDW interpolation (power = 3 on both)](anisotropic_interp.png)

There are a number of additional changes which could be explored to make this interpolation more effective and more efficient.
First, there can be ambiguity regarding which station along the centerline a particular point is supposed to have, particularly if that point (or a cross-section) containing it intersects the centerline at or near a point on the centerline or if a channel has significant S or horse-shoe type bends.  That ambiguity can be partially resolved by defining the outer boundaries of the channel.  Then if a normal through that point intersects a segment, but also intersects a boundary, the transformed point relative to that segment can be eliminated.  Second, if indeed the interpolation points all come from cross sections, it would be useful if the interface had a way to group the points into related units.  We could then identify the station associated with the point nearest the centerline and then use that or the nearest station for all of the other points in that group.  That would help eliminate the ambiguity.  Also, then on the inner bend of a cross section associated with a point on the centerline, the interpolation point could be explictly bound to the station of that point, rather than projecting onto the adjacent segments.  There might also be a specific input function for cross sections that explicitly identifies the station and then just defines the distance above/below the station and the elevation.  That would avoid the transformation completely.  It might also be easier to capture, since the initial data may be acquired in that format.  Third, if a point to be interpolated to transforms into more than one station, it would probably be better to somehow average the transformed points than to just pick the closest one.  Finally, the current implementation interpolates from every interpolation point, even though those far away have almost no influence.  The boundary would help identify which cross sections a point to be interpolated falls between.  Then only the points from those cross-sections could be used to interpolate from.  For a long channel with many cross-sections, this could significantly reduce the number of points used in the interpolation (particularly since those points have little contribution).
