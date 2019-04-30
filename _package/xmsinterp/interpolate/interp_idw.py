"""
********************************************************************************
* Name: interp_idw.py
* Author: Gage Larsen, Matt LeBaron
* Created On: April 29th, 2019
* Copyright: (c)
* License: BSD 2-Clause
********************************************************************************
"""

from .._xmsinterp.interpolate import InterpIdw as iIdw


class InterpIdw(object):

    weights = {
        'classic': 0,
        'modified': 1
    }

    nodal_function_types = {
        'constant': 0,
        'grad_plane': 1,
        'quadratic': 2
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

        self._instance = iIdw(points, triangles, scalars)

    @staticmethod
    def _check_points(points):
        """
        Verifies that there are at least three points.

        Args:
            points (iterable): All of the points
        """
        if len(points) < 3:
            raise ValueError('"points" must be a list of 3 or more points')

    @staticmethod
    def _check_triangles(triangles, point_length):
        """
        Verifies that the triangles are correct.

        Args:
            triangles (iterable): All of the triangles
            point_length (int): Count of all of the points
        """
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
        """
        Verifies that there are the same number of points and scalars.

        Args:
            scalars (iterable): All of the scalars
            point_length (int): Count of all of the points
        """
        if len(scalars) != point_length:
            raise ValueError("Length of scalars must be equal to the length of points.")

    @staticmethod
    def _check_activity(activity, _length, _type):
        """
        Verifies that the activity length and the length of the points or triangles are the same.

        Args:
            activity (iterable): Activity at a given location
            _length (int): Count of all of the points or triangles
            _type (string): 'points' or 'triangles'
        """
        if len(activity) != _length:
            raise ValueError("Length of activity must be equal to the length of {}.".format(_type))

    @staticmethod
    def _get_weight(_str):
        """
        Args:
            _str (string): Key for weights

        Returns (int): Int value from weights

        """
        weight_type = InterpIdw.weights.get(_str, None)
        if weight_type is None:
            raise ValueError('"weights" must be one of {}, not {}'.format(
                ", ".join(InterpIdw.weights.keys()), _str))
        return weight_type

    @staticmethod
    def _get_nodal_function_type(_str):
        """
        Gets the nodal_function_types int value from key string.

        Args:
            _str (string): Key for nodal_function_types

        Returns:
            Int value from nodal_function_types
        """
        nodal_function_type = InterpIdw.nodal_function_types.get(_str, None)
        if nodal_function_type is None:
            raise ValueError('"nodal_function_type" must be one of {}, not {}'.format(
                ", ".join(InterpIdw.nodal_function_types.keys()), _str))
        return nodal_function_type

    def set_points_and_triangles(self, points, triangles):
        """
        Sets the points that will be used to do the interpolation.

        Args:
            points (iterable): Array of the point locations
            triangles (iterable): All of the triangles
        """
        self._instance.SetPtsTris(points, triangles)

    @property
    def scalars(self):
        """Gets all of the scalars"""
        return self._instance.GetScalars

    @scalars.setter
    def scalars(self, value):
        """
        Sets the scalar values that will be used to do the interpolation.

        Args:
            value (iterable): Array of scalars
        """
        self._check_scalars(value, len(self.points))
        self._instance.SetScalars(value)

    @property
    def points(self):
        """Gets all of the points"""
        return self._instance.GetPts

    def set_points(self, points, is_2d):
        """
        Sets the points that will be used to do the interpolation.

        Args:
            points (iterable): Array of the point locations
            is_2d (bool): Flag if this is 2D
        """
        self._check_points(points)
        self._instance.SetPts(points, is_2d)

    @property
    def triangles(self):
        """Gets all of the triangles"""
        return self._instance.GetTris

    def interpolate_to_point(self, point):
        """
        Interpolates to the location specified by a_pt and returns the value.

        Args:
            point (tuple): The location of the interpolation point.

        Returns:
            The interpolated value.
        """
        return self._instance.InterpToPt(point)

    def interpolate_to_points(self, points):
        """
        Interpolates to an array of points and fills in an array of scalars.

        Args:
            points (iterable): Array of points to interpolate to.
        """
        return self._instance.InterpToPts(points)

    @property
    def point_activity(self):
        """Gets the point activity"""
        return self._instance.GetPtActivity

    @point_activity.setter
    def point_activity(self, value):
        """
        Sets the activity on the point being used to interpolate

        Args:
            value (iterable): Bitset of point activity.
        """
        self._check_activity(value, len(self.points), "points")
        self._instance.SetPtActivity(value)

    @property
    def triangle_activity(self):
        """Gets the triangle activity"""
        return self._instance.GetTriActivity

    @triangle_activity.setter
    def triangle_activity(self, value):
        """
        Sets the triangle activity.

        Args:
            value (iterable): Bitset of point activity.
        """
        self._check_activity(value, len(self.triangles) / 3, "triangles")
        self._instance.SetTriActivity(value)

    def set_truncation(self, maximum, minimum):
        """
        Set the truncation values for the interpolation and turn on truncation.

        Args:
            maximum (float): The maximum value for truncation.
            minimum (float): The minimum value for truncation.
        """
        if maximum < minimum:
            raise ValueError('maximum must be greater than minimum')
        self._instance.SetTrunc(maximum, minimum)

    @property
    def truncate_interpolation_values(self):
        """Gets the truncation interpolation values"""
        return self._instance.GetTruncateInterpolatedValues

    @property
    def truncate_min(self):
        """Gets the truncation minimum"""
        return self._instance.GetTruncMin

    @property
    def truncate_max(self):
        """Gets the truncation maximum"""
        return self._instance.GetTruncMax

    @property
    def power(self):
        """Gets the power"""
        return self._instance.GetPower

    @power.setter
    def power(self, value):
        """
        Sets the exponent for the interpolation. By default the class does inverse distance squared weighting but the
        exponent can be changed to any value.

        Args:
            value (float): The exponent used to compute the point weights
        """
        self._instance.SetPower(value)

    @property
    def search_options_number_nearest_points(self):
        """Gets the search options number nearest points"""
        return self._instance.GetSearchOptsNumNearestPts

    @property
    def search_options_use_quadrant_search(self):
        """Gets the search options using quadrant search"""
        return self._instance.GetSearchOptsUseQuadrantSearch

    @property
    def weight_calculation_method(self):
        """Gets the weight calculation method"""
        return self._instance.GetWeightCalcMethod

    @weight_calculation_method.setter
    def weight_calculation_method(self, weight):
        """
        Sets the method for calculating the weights. The classic just uses 1/distance^exponent. The modified method
        uses another formulation based on the distance of the furtherest location from the interpolation pt.

        Args:
            weight (string): 'modified' or 'classic'
        """
        weight_type = self._get_weight(weight)
        self._instance.SetWeightCalcMethod(weight_type)

    @property
    def nodal_function_type(self):
        """Gets the nodal function type"""
        return self._instance.GetNodalFunctionType

    @property
    def nodal_function_number_nearest_points(self):
        """Gets the nodal function number nearest the points"""
        return self._instance.GetNodalFunctionNumNearestPts

    @property
    def nodal_function_use_quadrant_search(self):
        """Gets the nodal function using quadrant search"""
        return self._instance.GetNodalFunctionUseQuadrantSearch

    def set_observer(self, progress=None):
        """
        Set the observer class so that feedback on the interpolation process can be received.

        Args:
            progress (observer): The observer
        """
        self._instance.SetObserver(progress)

    def set_search_options(self, nearest_point, quadrant_oct_search):
        """
        Sets the search options for how to find the nearest points to the interpolation point. The number of nearest
        points can be specified as well as whether to find the nearest points in each quadrant or octant.

        Args:
            nearest_point (int): The number of nearest points to the interpolation point. These points are used to do
                                 the interpolation.
            quadrant_oct_search (bool): Specifies if the search criterion should find the nearest points in each
                                 quadrant (2d) or octant (3d)
        """
        self._instance.SetSearchOpts(nearest_point, quadrant_oct_search)

    def set_nodal_function(self, nodal_function_type="constant", number_of_nearest_points=16, quadrant_oct=False, progress=None):
        """
        Sets the type of nodal function as well as options for computing nodal functions.

        Args:
            nodal_function_type (string): The nodal function methodology:
                                          constant (0), gradient plane (1), quadratic (2).
            number_of_nearest_points (int): The nearest number of points to use when calculating the nodal functions.
            quadrant_oct (bool): Find the nearest number of points in each quadrant (2d) or octant (3d) when computing
                                 nodal functions.
            progress (float): Progress bar to give user feedback.
        """
        nft = self._get_nodal_function_type(nodal_function_type)
        self._instance.SetNodalFunction(nft, number_of_nearest_points, quadrant_oct, progress)

    def set_save_weights(self, save):
        """
        Sets the flag for saving weights.

        Args:
            save (bool): Specifies if the weights should be saved
        """
        self._instance.SetSaveWeights(save)

    def interpolate_weights(self, point):
        """
        Given a location and an array of points the weights associated with array of points are calculated.

        Args:
            point (tuple): Location of the interpolation point
        """
        return self._instance.InterpWeights(point)

    def set_multithreading(self, use_multithreading):
        """
        Sets the flag for multi-threading.

        Args:
            use_multithreading (bool): Specifies if multi-threading will be used
        """
        self._instance.SetMultiThreading(use_multithreading)
