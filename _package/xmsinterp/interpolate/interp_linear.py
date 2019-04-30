"""
********************************************************************************
* Name: interp_linear.py
* Author: Gage Larsen, Matt LeBaron
* Created On: April 29th, 2019
* Copyright: (c)
* License: BSD 2-Clause
********************************************************************************
"""

from .._xmsinterp.interpolate import InterpLinear as iLin


class InterpLinear:

    nodal_function_types = {
        'constant': 0,
        'gradient_plane': 1,
        'quadratic': 2
    }

    nodal_function_point_search_options = {
        'natural_neighbors': 0,
        'nearest_pts': 1,
    }



    def __init__(self, points=None, triangles=None, scalars=None, **kwargs):
        if 'instance' in kwargs:
            self._instance = kwargs['instance']
            return

        if points is None:
            raise ValueError('"points" is a required argument.')
        self._check_points(points)

        if triangles is None or len(triangles) == 0:
            triangles = []
        else:
            self._check_triangles(triangles, len(points))

        if scalars is None or len(scalars) == 0:
            scalars = []
        else:
            self._check_scalars(scalars, len(points))

        self._instance = iLin(points, triangles, scalars)

    def __repr__(self):
        return '<InterpLinear - Point Count: {}, Triangle Count: {}>'.format(
            len(self.points),
            int(len(self.triangles) / 3),
        )

    def __str__(self):
        return '<InterpLinear - Point Count: {}, Triangle Count: {}>'.format(
            len(self.points),
            int(len(self.triangles) / 3),
        )

    @staticmethod
    def _check_points(points):
        if len(points) < 3:
            raise ValueError('"points" must be a list of 3 or more points')

    @staticmethod
    def _check_triangles(triangles, point_length):
        if len(triangles) % 3 != 0:
            raise ValueError('"triangles" must be a list of point indexes divisible by 3 defining each triangle.')
        max_triangles = max(triangles)
        if max_triangles > point_length - 1:
            raise ValueError('point in triangles is out of range: {}'.format(max_triangles))
        min_triangles = min(triangles)
        if min_triangles < 0:
            raise ValueError('point in triangles is out of range: {}'.format(min_triangles))

    @staticmethod
    def _check_scalars(scalars, point_length):
        if len(scalars) != point_length:
            raise ValueError("Length of scalars must be equal to the length of points.")

    @staticmethod
    def _check_activity(activity, _length, _type):
        if len(activity) != _length:
            raise ValueError("Length of activity must be equal to the length of {}.".format(_type))

    @staticmethod
    def _get_nodal_function_type(_str):
        nf_type = InterpLinear.nodal_function_types.get(_str, None)
        if nf_type is None:
            raise ValueError('"nodal_function_type" must be one of {}, not {}'.format(
                ", ".join(InterpLinear.nodal_function_types.keys()), _str))
        return nf_type

    @staticmethod
    def _get_nodal_function_point_search_options(_str):
        nf_type = InterpLinear.nodal_function_point_search_options.get(_str, None)
        if nf_type is None:
            raise ValueError('"nodal_function_point_search_option" must be one of {}, not {}'.format(
                ", ".join(InterpLinear.nodal_function_point_search_options.keys()), _str))
        return nf_type

    def set_points_and_triangles(self, points, triangles):
        self._instance.SetPtsTris(points, triangles)

    @property
    def points(self):
        return self._instance.GetPts

    @points.setter
    def points(self, value):
        self._check_points(value)
        self._instance.SetPts(value)

    @property
    def triangles(self):
        return self._instance.GetTris

    @triangles.setter
    def triangles(self, value):
        self._check_triangles(value, len(self.points))
        self._instance.SetTris(value)

    @property
    def scalars(self):
        return self._instance.GetScalars

    @scalars.setter
    def scalars(self, value):
        self._check_scalars(value, len(self.points))
        self._instance.SetScalars(value)

    def interpolate_to_point(self, points):
        return self._instance.InterpToPt(points)

    def interpolate_to_points(self, points):
        return self._instance.InterpToPts(points)

    @property
    def point_activity(self):
        return self._instance.GetPtActivity

    @point_activity.setter
    def point_activity(self, value):
        self._check_activity(value, len(self.points), "points")
        self._instance.SetPtActivity(value)

    @property
    def triangle_activity(self):
        return self._instance.GetTriActivity

    @triangle_activity.setter
    def triangle_activity(self, value):
        self._check_activity(value, len(self.triangles) / 3, "triangles")
        self._instance.SetTriActivity(value)

    @property
    def extrapolation_point_indexes(self):
        return self._instance.GetExtrapolationPointIndexes

    def triangle_containing_point(self, point):
        return self._instance.TriContainingPt(point)

    def triangle_envelopes_containing_point(self, point):
        return self._instance.TriEnvelopsContainingPt(point)

    def interpolate_weights(self, point):
        return self._instance.InterpWeights(point)

    @property
    def extrapolation_value(self):
        return self._instance.GetExtrapVal

    @extrapolation_value.setter
    def extrapolation_value(self, value):
        self._instance.SetExtrapVal(value)

    def set_truncation(self, maximum, minimum):
        if maximum < minimum:
            raise ValueError('maximum must be greater than minimum')
        self._instance.SetTrunc(maximum, minimum)

    @property
    def use_clough_tocher(self):
        return self._instance.GetUseCloughTocher()

    def set_use_clough_tocher(self, on, progress=None):
        self._instance.SetUseCloughTocher(on, progress)

    @property
    def use_natural_neighbor(self):
        return self._instance.GetUseNatNeigh()

    def set_use_natural_neighbor(self, on, nodal_function_type="constant",
                                 nodal_function_point_search_option="nearest_pts",
                                 nodal_function_number_nearest_points=16, nodal_function_blend_weights=True,
                                 progress=None):
        nft = self._get_nodal_function_type(nodal_function_type)
        nfpso = self._get_nodal_function_point_search_options(nodal_function_point_search_option)
        self._instance.SetUseNatNeigh(on, nft, nfpso, nodal_function_number_nearest_points,
                                      nodal_function_blend_weights, progress)

    @property
    def truncate_interpolated_values(self):
        return self._instance.GetTruncateInterpolatedValues

    @property
    def truncate_min(self):
        return self._instance.GetTruncMin

    @property
    def truncate_max(self):
        return self._instance.GetTruncMax

    @property
    def native_neighbor_nodal_func(self):
        return self._instance.GetNatNeighNodalFunc

    @property
    def native_neighbor_nodal_func_nearest_points_option(self):
        return self._instance.GetNatNeighNodalFuncNearestPtsOption

    @property
    def native_neighbor_nodal_func_number_nearest_points(self):
        return self._instance.GetNatNeighNodalFuncNumNearestPts

    @property
    def native_neighbor_blend_weights(self):
        return self._instance.GetNatNeighBlendWeights
