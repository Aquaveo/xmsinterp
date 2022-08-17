"""Initialize the module."""
from . import interpolate  # NOQA: F401
from .api.interpolator import interpolate_to_grid  # NOQA: F401
from .api.interpolator import interpolate_to_points  # NOQA: F401

__version__ = '6.1.2'
