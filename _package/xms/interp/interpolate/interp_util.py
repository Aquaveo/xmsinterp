"""InterpUtil methods."""
from .._xmsinterp.interpolate import InterpUtil

__all__ = ['generate_raster_idw']


def generate_raster_idw(pts, min_pt, size, num_rows, num_cols, no_data_val, activity):
    """Returns an array of raster values given mesh point data and raster information.

    Args:
        pts (iterable): The measured depths at point locations.
        min_pt (tuple): The minimum element edge size.
        size (tuple): Size of the raster cells in the X and Y directions.
        num_rows (int): Number of rows in the raster.
        num_cols (int): Number of cols in the raster.
        no_data_val (float): The NODATA value to assign to inactive raster pixels in the result.
        activity (iterable): This is the size of the raster return values and is used to determine whether to
         set raster cells to active and interp values to each cell.
    Returns:
        The interpolated values at the given points.
    """
    return InterpUtil.generate_raster_idw(pts, min_pt, size, num_rows, num_cols, no_data_val, activity)
