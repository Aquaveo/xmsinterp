*****************
InterpAnisotropic
*****************

Class that performs anisotropic interpolation. Transforms points
so x is a distance of a projected normal onto the centerline and y
is the distance from (above or below) the centerline. Interpolate
using inverse distance weighted interpolation after scaling the
transformed x-values.

.. autoclass:: xmsinterp.interpolate.InterpAnisotropic
   :members:
