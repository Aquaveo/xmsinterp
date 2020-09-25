"""Base class for interpolator class."""
from abc import ABC, abstractmethod
import logging

from xms.interp import interpolate

logger = logging.getLogger('xms.interp')


class Interpolator(ABC):
    """Abstract interpolate base class."""

    def __init__(self, **kwargs):
        """Constructor.

        Args:
            **kwargs (dict): Generic keyword arguments
        """
        self.truncation_max = kwargs.pop('truncation_max', None)
        self.truncation_min = kwargs.pop('truncation_min', None)

        if len(kwargs) > 0:
            logger.warning('The following kwargs are being ignored: {}.'.format(kwargs))

    @abstractmethod
    def __str__(self):
        """Returns a string representation of the interpolator."""
        pass

    @abstractmethod
    def __repr__(self):
        """Returns a string representation of the interpolator."""
        pass

    def __eq__(self, other):
        """Equality operator.

        Args:
            other (Interpolator): Interpolator to compare

        Returns:
            bool: True if Interpolators are equal
        """
        other_instance = getattr(other, '_instance', None)
        if not other_instance or not isinstance(other_instance, self.__class__):
            return False
        return other_instance == self._instance

    def __ne__(self, other):
        """Equality operator.

        Args:
            other (Interpolator): Interpolator to compare

        Returns:
            bool: True if Interpolators are not equal
        """
        result = self.__eq__(other)
        return not result

    @abstractmethod
    def set_truncation(self, **kwargs):
        """Set the truncation values for the interpolation and turn on truncation.

        Args:
            kwargs (dict): Generic keyword arguments
        """
        pass

    @abstractmethod
    def interpolate_to_points(self, **kwargs):
        """Calls InterpToPt in a loop.

        Args:
            kwargs (dict): Generic keyword arguments

        Returns:
            Interpolated values at points.
        """
        pass

    def _configure_truncation(self):
        """Configure the truncation options."""
        if self.truncation_max is not None and self.truncation_min is not None:
            self.set_truncation(maximum=self.truncation_max, minimum=self.truncation_min)

    def interpolate(self, pts, x='x', y='y', z='z', in_place=True):
        """Use the stored triangles to interpolate to points.

        Returns extrapolation value if the point is outside the triangles.

        Args:
            pts (pandas.DataFrame): Location that is interpolated to.
            x (str): Name of the x column
            y (str): Name of the y column
            z (str): Name of the z column
            in_place (bool): True if interpolated values should be updated in pts

        Returns:
            Interpolated value at point.
        """
        pts_is_dataframe = False
        try:
            # Assuming pts is a pandas dataframe
            assert x in pts.columns
            assert y in pts.columns
            values = pts[[x, y]].values
            pts_is_dataframe = True
        except AttributeError:
            # assume pts is numpy array
            values = pts

        self._configure_truncation()
        z_values = self.interpolate_to_points(values)

        if z and pts_is_dataframe and in_place:
            pts[z] = z_values
            return pts

        return z_values

    @classmethod
    def get_interpolator_types(cls, as_dict=False):
        """Get the strings associated with the interpolator types enum.

        Args:
            as_dict (bool): True if return value should have mapping between strings and enum values

        Returns:
            dict or list: The interpolator types
        """
        types = {
            'linear': interpolate.InterpLinear,
            'idw': interpolate.InterpIdw,
            'natural_neighbor': interpolate.InterpLinear,
        }
        if as_dict:
            return types
        return list(types.keys())

    @classmethod
    def get_interpolator(cls, interpolator_type, values, **kwargs):
        """Create a new interpolator.

        Args:
            interpolator_type:
            values (pandas.DataFrame): The points
            **kwargs (dict): Generic keyword arguments

        Returns:
            Interpolator: see description
        """
        types = cls.get_interpolator_types(as_dict=True)

        try:
            # assume values is a pandas dataframe
            values = values.values
        except AttributeError:
            pass

        try:
            return types[interpolator_type](points=values, **kwargs)
        except IndexError:
            raise ValueError('Interpolator kind "{}" is not recognized.'.format(interpolator_type))
