Installation
------------

XmsInterp can be installed using `Anaconda <https://www.anaconda.com/download/>`_.

You can install XmsInterp using the `conda <https://www.anaconda.com/download/>`_ command::

   conda install -c aquaveo xmsinterp

This will install XmsInterp and **all** the needed dependencies.


Usage
-----

The XmsInterp library contains classes for performing 2D (x,y) linear, natural
neighbor, and idw interpolation. Natural neighbor interpolation is performed using
the linear interpolation class. 3D (x,y,z) interpolation can be performed using the
idw class. Points (x,y) must be given to create the interpolation class.

The first step is to create a PolyInput that will contain your polygon definition. In this example
we will start with 4 corners and use the PolyRedistributePts object to specify a spacing on initial
polygon.

Usage and documentation for each class can be found in the **User Interface** section
of this site. There are also additional examples that can be found on the Examples_ page

.. _Examples: https://aquaveo.github.io/examples/xmsinterp/xmsinterp.html