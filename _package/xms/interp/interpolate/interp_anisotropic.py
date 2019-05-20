"""
********************************************************************************
* Name: interp_anisotropic.py
* Author: Gage Larsen, Matt LeBaron
* Created On: April 29th, 2019
* Copyright: (c)
* License: BSD 2-Clause
********************************************************************************
"""

from .._xmsinterp.interpolate import InterpAnisotropic as iAni


class InterpAnisotropic(object):
    def __init__(self, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return
        self._instance = iAni()

    def set_points(self, center_line_points, interpolation_points, pick_closest):
        """
        Sets points that wil be used for the centerline and interpolation.

        Args:
            center_line_points (iterable): Points that make the centerline
            interpolation_points (iterable): Points that will be used to interpolate from
            pick_closest (bool): If true, only keep one transformed interpolation point (that is closest to the
                                 centerline)
        """
        self._instance.SetPoints(center_line_points, interpolation_points, pick_closest)

    def interp_to_point(self, point):
        """
        Interpolates to the location specified by a_pt and returns the value.

        Args:
            point (tuple): The location of the interpolation point

        Returns:
            The interpolated value
        """
        return self._instance.InterpToPt(point)

    def interp_to_points(self, points):
        """
        Interpolates to the locations specified by a_pts and returns the values.

        Args:
            points (iterable): The points to interpolate to

        Returns:
            The interpolated values
        """
        return self._instance.InterpToPts(points)

    def get_interpolation_points(self):
        """
        Get the interpolation points transformed into (s,n,z) space and scaled.

        Returns:
            The transformed and scaled interpolation points (passed to SetPoints).
        """
        return self._instance.GetInterpolationPts()

    def get_transformed_points(self, points, get_closest):
        """
        Transform points into (s,n,z) space and then scale.

        Args:
            points (iterable): The points to transform into (s,n,z) space.
            get_closest (bool): True to pick only the closest transformed point.

        Returns:
            The points transformed into (s,n,z) space.
        """
        return self._instance.GetTransformedPts(points, get_closest)

    def set_power(self, power):
        """
        Sets the exponent for the interpolation. By default the class does inverse distance squared weighting but the exponent can be changed to any value.

        Args:
            power (float): The exponent used to compute the point weights
        """
        self._instance.SetPower(power)

    def set_x_scale(self, scale):
        """
        Set the X Scale.

        Args:
            scale (float): X scale
        """
        self._instance.SetXScale(scale)
