"""
The __init__ file for the interp module.
"""
from . import interpolate  # NOQA: F401
from .api.interpolator import interpolate_to_grid  # NOQA: F401
from .api.interpolator import interpolate_to_points  # NOQA: F401

__version__ = '3.1.1'
