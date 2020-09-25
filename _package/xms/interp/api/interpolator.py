"""Convenience methods for using interpolator classes."""
import numpy as np

from xms.interp.interpolate import Interpolator

try:
    import xarray as xr
    xr_enabled = True
except ImportError:
    xr_enabled = False

try:
    import pandas as pd
    pandas_enabled = True
except ImportError:
    pandas_enabled = False


def interpolate_to_points(interpolation_points, points, method, x='x', y='y', z='z', **kwargs):
    """Interpolate values from one set of points to another.

    Args:
        interpolation_points: The points to be used for interpolation
        points: The points to interpolate to
        method: The interpolation method to be used
        x: The x dataset
        y: The y dataset
        z: The z dataset

    Returns:
        The interpolated points
    """
    interp_object = Interpolator.get_interpolator(interpolator_type=method, values=interpolation_points, **kwargs)
    return interp_object.interpolate(points, x=x, y=y, z=z)


def _buffer_range(range_min, range_max, buf_by=0.1):
    """Get the (x, y) range of the data and expand by a percentage.

    Args:
        range_min: The minimum
        range_max: The maximum
        buf_by (float, optional): Percentage to expand, default to 10%

    Returns:
        the minimum expanded by the buffer, and the maximum expanded by the buffer
    """
    buf = (range_max - range_min) * buf_by
    return range_min - buf, range_max + buf


def _generate_x_y_coords(interpolation_points, size, buffer_by=0.1):
    """Generate the x and y coordinates of a grid.

    Args:
        interpolation_points: The interpolation points
        size: The size of the grid
        buffer_by: The buffer percentage

    Returns:
        The generated x and y coordinates

    Warning:
        Currently assuming interp_points is a pandas.DataFrame

    Todo:
        Handle other cases (e.g numpy array, xarray, list of tuples, etc.)
    """
    x_range = _buffer_range(interpolation_points.x.min(), interpolation_points.x.max(), buf_by=buffer_by)
    y_range = _buffer_range(interpolation_points.y.min(), interpolation_points.y.max(), buf_by=buffer_by)

    number_of_x_points, number_of_y_points = size
    x = np.linspace(x_range[0], x_range[1], number_of_x_points)
    y = np.linspace(y_range[0], y_range[1], number_of_y_points)
    return x, y


def interpolate_to_grid(interpolation_points, method, grid=None, size=(100, 100), x='x', y='y', z='z',
                        in_place=False, as_xr_dataarray=False, as_xr_dataset=False, **kwargs):
    """Interpolate values from a set of points to the points of a grid.

    Args:
        interpolation_points: The points to be used for interpolation
        method: Interpolation method to be used
        grid: The pandas dataframe to be used
        size: The size of the grid if no grid is provided
        x: The dataset to be used as x on the grid
        y: The dataset to be used as y on the grid
        z: The dataset to be used as z on the grid
        in_place: Make the changes in place
        as_xr_dataarray: return as xarray data array
        as_xr_dataset: return as xarray dataset

    Returns:
        The new dataset as an xarray data array or data set

    Warning:
        This api call requires pandas
    """
    if grid is not None:
        default_x, default_y = list(grid.coords.items())[:2]
        x_coord, y_coord = grid.coords.get(x, default_x), grid.coords.get(y, default_y)
        size = x_coord.size, y_coord.size
    else:
        x_coord, y_coord = _generate_x_y_coords(interpolation_points, size)

    if not pandas_enabled:
        raise RuntimeError("This api function requires pandas to be installed")

    pts = np.stack([i.flatten() for i in np.meshgrid(x_coord, y_coord)], axis=1)
    pts = pd.DataFrame(columns=['x', 'y'], data=pts)
    interpolate_to_points(interpolation_points=interpolation_points, points=pts, method=method, **kwargs)

    z_values = pts.z.values.reshape((size[1], size[0]))
    data = None

    if xr_enabled:
        data = xr.DataArray(coords={'x': x_coord, 'y': y_coord}, dims=['x', 'y'], data=z_values.T, name=z)

    if in_place and grid is not None:
        if xr_enabled and type(grid) == xr.DataArray or hasattr(grid, 'to_dataset'):
            grid.name = z
            grid.data = z_values

        if xr_enabled and type(grid) == xr.Dataset or hasattr(grid, 'to_array'):
            if data is not None:
                grid[z] = data
            else:
                arr = list(grid.data_vars.values())[0].copy()
                # TODO: not handling if default_x, _y are used or if first array has different coords
                assert x in arr.coords
                assert y in arr.coords
                arr.data = z_values
                grid[z] = arr

        return grid
    elif data is not None:  # i.e. xarray is installed
        if as_xr_dataarray:
            return data
        if as_xr_dataset:
            return data.to_dataset()

    return z_values
