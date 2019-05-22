import logging
from abc import ABC, abstractmethod

from xms.interp import interpolate

logger = logging.getLogger('xms.interp')


class Interpolator(ABC):
    """Abstract interpolate base class"""

    def __init__(self, **kwargs):
        self.truncation_max = kwargs.pop('truncation_max', None)
        self.truncation_min = kwargs.pop('truncation_min', None)

        if len(kwargs) > 0:
            logger.warning('The following kwargs are being ignored: {}.'.format(kwargs))

    @abstractmethod
    def __str__(self):
        pass

    @abstractmethod
    def __repr__(self):
        pass

    @abstractmethod
    def set_truncation(self, **kwargs):
        pass

    @abstractmethod
    def interpolate_to_points(self, **kwargs):
        pass

    def _configure_truncation(self):
        if self.truncation_max is not None and self.truncation_min is not None:
            self.set_truncation(maximum=self.truncation_max, minimum=self.truncation_min)

    def interpolate(self, pts, x='x', y='y', z='z', in_place=True):
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
        types = cls.get_interpolator_types(as_dict=True)

        try:
            #assume values is a pandas dataframe
            values = values.values
        except AttributeError:
            pass

        try:
            return types[interpolator_type](points=values, **kwargs)
        except IndexError:
            raise ValueError('Interpolator kind "{}" is not recognized.'.format(interpolator_type))
