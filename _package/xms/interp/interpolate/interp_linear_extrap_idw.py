"""Pure Python wrapping for InterpLinearExtrapIdw class."""
from .interpolator import Interpolator
from .._xmsinterp.interpolate import InterpLinearExtrapIdw as iLin


class InterpLinearExtrapIdw(Interpolator):
    """Class for performing linear interpolation."""
    nodal_function_types = {
        'constant': 0,
        'gradient_plane': 1,
        'quadratic': 2
    }

    nodal_function_point_search_options = {
        'natural_neighbor': 0,
        'nearest_points': 1,
    }

    def __init__(self, points=None, triangles=None, scalars=None, nodal_function=None,
                 point_search_option="natural_neighbor", number_nearest_points=16, blend_weights=True,
                 progress=None, quadrant_oct=False, weight_quadrant_oct=False,
                 weight_number_nearest_points=16, **kwargs):
        """Constructor.

        Args:
            points (list): The points of the source geometry as x,y,z tuples
            triangles (list): The triangles of the source geometry as point index tuples
            scalars (list): The scalar values at the points
            nodal_function (str): The nodal function to use One of: 'constant', 'gradient_plane', 'quadratic'
            point_search_option (str): The point search option. One of: 'natural_neighbor', 'nearest_points'
            number_nearest_points (int): The number of nearest points to consider
            blend_weights (bool): True if weights should be blended
            progress (Observer): Observer object for providing feedback
            weight_number_nearest_points (int): number of points to include in interpolation weight calculation
            quadrant_oct (bool): True if quadrant search should be used
            weight_quadrant_oct (bool): get nearest points from quadrants for weight calculation
            **kwargs (dict): Generic keyword arguments
        """
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

        if nodal_function is not None:
            self.set_use_natural_neighbor(True, nodal_function, point_search_option, number_nearest_points,
                                          blend_weights, progress)
            self.set_idw_nodal_function(nodal_function, number_nearest_points, quadrant_oct, progress)
        self.set_idw_search_options(nearest_point=weight_number_nearest_points, quadrant_oct_search=weight_quadrant_oct)

        super().__init__(**kwargs)

    def __eq__(self, other):
        """Equality operator.

        Args:
            other (InterpLinearExtrapIdw): InterpLinearExtrapIdw to compare

        Returns:
            bool: True if InterpLinearExtrapIdws are equal
        """
        other_instance = getattr(other, '_instance', None)
        if not other_instance or not isinstance(other_instance, iLin):
            return False
        return other_instance == self._instance

    def __ne__(self, other):
        """Equality operator.

        Args:
            other (InterpLinearExtrapIdw): InterpLinearExtrapIdw to compare

        Returns:
            bool: True if InterpLinearExtrapIdws are not equal
        """
        result = self.__eq__(other)
        return not result

    def __repr__(self):
        """Returns a string representation of the InterpLinearExtrapIdw."""
        return '<InterpLinearExtrapIdw - Point Count: {}, Triangle Count: {}>'.format(
            len(self.points),
            int(len(self.triangles) / 3),
        )

    def __str__(self):
        """Returns a string representation of the InterpLinearExtrapIdw."""
        return '<InterpLinearExtrapIdw - Point Count: {}, Triangle Count: {}>'.format(
            len(self.points),
            int(len(self.triangles) / 3),
        )

    @staticmethod
    def _check_points(points):
        """Verifies that there are at least three points.

        Args:
            points (iterable): Array of points
        """
        if len(points) < 3:
            raise ValueError('"points" must be a list of 3 or more points')

    @staticmethod
    def _check_triangles(triangles, point_length):
        """Verifies that the triangles are valid.

        Args:
            triangles (iterable): Array of the triangles
            point_length (int): Count of all points
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
        """Verifies that the scalars are valid.

        Args:
            scalars (iterable): Array of scalars
            point_length (int): Count of all points
        """
        if len(scalars) != point_length:
            raise ValueError("Length of scalars must be equal to the length of points.")

    @staticmethod
    def _check_activity(activity, _length, _type):
        """Verifies that the activity is valid.

        Args:
            activity (iterable): Array of activity
            _length (int): Count of all points
            _type (string): 'points' or 'triangles'
        """
        if len(activity) != _length:
            raise ValueError("Length of activity must be equal to the length of {}.".format(_type))

    @staticmethod
    def _get_nodal_function_type(_str):
        """Gets the nodal function type based on the string.

        Args:
            _str (string): Nodal function type: 'constant', 'gradient_plane' or 'quadratic'

        Returns:
            The int value based on the nodal function type
        """
        nf_type = InterpLinearExtrapIdw.nodal_function_types.get(_str, None)
        if nf_type is None:
            raise ValueError('"nodal_function_type" must be one of {}, not {}'.format(
                ", ".join(InterpLinearExtrapIdw.nodal_function_types.keys()), _str))
        return nf_type

    @staticmethod
    def _get_nodal_function_point_search_options(_str):
        """Gets the nodal function point search option.

        Args:
            _str (string): Search option: 'natural_neighbors' or 'nearest_points'

        Returns:
            The int value based on the nodal function point search option
        """
        nf_type = InterpLinearExtrapIdw.nodal_function_point_search_options.get(_str, None)
        if nf_type is None:
            raise ValueError('"nodal_function_point_search_option" must be one of {}, not {}'.format(
                ", ".join(InterpLinearExtrapIdw.nodal_function_point_search_options.keys()), _str))
        return nf_type

    def set_points_and_triangles(self, points, triangles):
        """Adds the triangles to the class.

        Args:
            points (iterable): Array of point locations.
            triangles (iterable): Array of triangles that references the points array. This array will have a size that
                                  is a multiple of 3. The first 3 locations in array represent the first triangle and
                                  will have indices that correspond to locations in the points array.
        """
        self._instance.SetPtsTris(points, triangles)

    @property
    def points(self):
        """Gets the points."""
        return self._instance.GetPts

    @property
    def triangles(self):
        """Gets the triangles."""
        return self._instance.GetTris

    @property
    def scalars(self):
        """Gets the scalars."""
        return self._instance.GetScalars

    @scalars.setter
    def scalars(self, value):
        """Set the scalars that will be used to interpolate from.

        Args:
            value (iterable): Array of interpolation scalar values.
        """
        self._check_scalars(value, len(self.points))
        self._instance.SetScalars(value)

    def interpolate_to_point(self, point):
        """Use the stored triangles to interpolate to a point.

        Returns extrapolation value if the point is outside the triangles.

        Args:
            point (tuple): Location that is interpolated to.

        Returns:
            Interpolated value at point.
        """
        return self._instance.InterpToPt(point)

    def interpolate_to_points(self, points):
        """Calls InterpToPt in a loop.

        Args:
            points (iterable): Locations of points.

        Returns:
            Interpolated values at points.
        """
        return self._instance.InterpToPts(points)

    @property
    def point_activity(self):
        """Gets the point activity."""
        return self._instance.GetPtActivity

    @point_activity.setter
    def point_activity(self, value):
        """Modifies the activity bitset of the class.

        Args:
            value (iterable): Bitset of the activity of the points
        """
        self._check_activity(value, len(self.points), "points")
        self._instance.SetPtActivity(value)

    @property
    def triangle_activity(self):
        """Gets the triangle activity."""
        return self._instance.GetTriActivity

    @triangle_activity.setter
    def triangle_activity(self, value):
        """Modifies the activity bitset of the class.

        Args:
            value (iterable): Bitset of the activity of the triangles
        """
        self._check_activity(value, len(self.triangles) / 3, "triangles")
        self._instance.SetTriActivity(value)

    @property
    def extrapolation_point_indexes(self):
        """Returns vector of point indexes for points that were outside of all triangles."""
        return self._instance.GetExtrapolationPointIndexes

    def triangle_containing_point(self, point):
        """Find the triangle containing the point.

        Args:
            point (tuple): Location used to find a triangle.

        Returns:
            Index of triangle containing point. If None is returned then no triangle contained the point.
        """
        return self._instance.TriContainingPt(point)

    def triangle_envelopes_containing_point(self, point):
        """Find all triangles whose envelop contains the point.

        Args:
            point (tuple): Location used to find a triangle.

        Returns:
            Iterable of triangle indexes containing the point
        """
        return self._instance.TriEnvelopsContainingPt(point)

    def interpolate_weights(self, point):
        """Use the stored triangles to get interpolation weights for a point.

        Returns False if the point is outside the triangles.

        Args:
            point (tuple): Location that is interpolated to.

        Returns:
            tuple of a bool, array of indexes, and array of weights
        """
        return self._instance.InterpWeights(point)

    @property
    def extrapolation_value(self):
        """Set the constant extrapolation value."""
        return self._instance.GetExtrapVal

    @extrapolation_value.setter
    def extrapolation_value(self, value):
        """Get extrapolation value."""
        self._instance.SetExtrapVal(value)

    def set_truncation(self, maximum, minimum):
        """Set the truncation values for the interpolation and turn on truncation.

        Args:
            maximum (float): The maximum value for truncation.
            minimum (float): The minimum value for truncation.
        """
        if maximum < minimum:
            raise ValueError('maximum must be greater than minimum')
        self._instance.SetTrunc(maximum, minimum)

    @property
    def use_clough_tocher(self):
        """Get the option for using Clough Tocher interpolation."""
        return self._instance.GetUseCloughTocher()

    def set_use_clough_tocher(self, on, progress=None):
        """Set the class to use the Clough Tocher interpolation method.

        This is a legacy feature from GMS. Compare to linear.

        Args:
            on (bool): TIndicate if CT should be used.
            progress (observer): Progress bar to give users feed back on the set up process of CT.
                                 If you have a really large set of triangles this may take some time.
        """
        self._instance.SetUseCloughTocher(on, progress)

    @property
    def use_natural_neighbor(self):
        """Get the option for using Natural Neighbor interpolation."""
        return self._instance.GetUseNatNeigh()

    def set_use_natural_neighbor(self, on=True, nodal_function="constant",
                                 point_search_option="natural_neighbor",
                                 number_nearest_points=16, blend_weights=True,
                                 progress=None):
        """Set the class to use natural neighbor (NN) interpolation.

        Args:
            on (bool): Indicate if NN should be used.
            nodal_function (str): Indicates which nodal function to use.
            point_search_option (str): Indicates options for the nearest points when computing
                                                      the nodal functions.
            number_nearest_points (int): The number of nearest points for nodal function computation.
            blend_weights (bool): Option to use a blending function on the calculated weights.
            progress (observer): Progress bar to give user feedback for generation of the nodal functions.
        """
        nft = self._get_nodal_function_type(nodal_function)
        nfpso = self._get_nodal_function_point_search_options(point_search_option)
        self._instance.SetUseNatNeigh(on, nft, nfpso, number_nearest_points,
                                      blend_weights, progress)

    def set_idw_nodal_function(self, nodal_function="constant", number_nearest_points=16, quadrant_oct=False,
                               progress=None):
        """Sets the type of nodal function as well as options for computing nodal functions.

        Args:
            nodal_function (string): The nodal function methodology:
                                          constant (0), gradient plane (1), quadratic (2).
            number_nearest_points (int): The nearest number of points to use when calculating the nodal functions.
            quadrant_oct (bool): Find the nearest number of points in each quadrant (2d) or octant (3d) when computing
                                 nodal functions.
            progress (float): Progress bar to give user feedback.
        """
        nft = self._get_nodal_function_type(nodal_function)
        self._instance.SetIdwNodalFunction(nft, number_nearest_points, quadrant_oct, progress)

    def set_idw_search_options(self, nearest_point, quadrant_oct_search):
        """Sets the search options for how to find the nearest points to the interpolation point.

        The number of nearest points can be specified as well as whether to find the nearest points in each
        quadrant or octant.

        Args:
            nearest_point (int): The number of nearest points to the interpolation point. These points are used to do
                                 the interpolation.
            quadrant_oct_search (bool): Specifies if the search criterion should find the nearest points in each
                                 quadrant (2d) or octant (3d)
        """
        self._instance.SetIdwSearchOpts(nearest_point, quadrant_oct_search)

    @property
    def truncate_interpolated_values(self):
        """Get the option to truncate interpolated values."""
        return self._instance.GetTruncateInterpolatedValues

    @property
    def truncate_min(self):
        """Get minimum truncation value."""
        return self._instance.GetTruncMin

    @property
    def truncate_max(self):
        """Get maximum truncation value."""
        return self._instance.GetTruncMax

    @property
    def native_neighbor_nodal_function(self):
        """Get the value for the Natural Neighbor nodal function."""
        return self._instance.GetNatNeighNodalFunc

    @property
    def native_neighbor_nodal_function_nearest_points_option(self):
        """Get the option for the Natural Neighbor nodal function nearest points."""
        return self._instance.GetNatNeighNodalFuncNearestPtsOption

    @property
    def native_neighbor_nodal_function_number_nearest_points(self):
        """Get the value for the number of nearest points to use when calculating the nodal function."""
        return self._instance.GetNatNeighNodalFuncNumNearestPts

    @property
    def native_neighbor_blend_weights(self):
        """Get the option for blending weights when using Natural Neighbor."""
        return self._instance.GetNatNeighBlendWeights
