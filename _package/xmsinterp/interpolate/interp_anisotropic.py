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


class InterpAnisotropic:
    def __init__(self, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return
        self._instance = iAni()

    def set_points(self, center_line_points, interpolation_points, pick_closest):
        self._instance.SetPoints(center_line_points, interpolation_points, pick_closest)

    def interp_to_point(self, point):
        return self._instance.InterpToPt(point)

    def interp_to_points(self, points):
        return self._instance.InterpToPts(points)

    def get_interpolation_points(self):
        return self._instance.GetInterpolationPts()

    def get_transformed_points(self, points, get_closest):
        return self._instance.GetTransformedPts(points, get_closest)

    def set_power(self, power):
        self._instance.SetPower(power)

    def set_x_scale(self, scale):
        self._instance.SetXScale(scale)
