Points and Containers
=====================

.. module:: catana

Objects in CatAna are points in space given by their angular coordinates and radial distance. To create spherical
clustering statistics on such points, they have to be gathered in either a :class:`PointContainer` or a
:class:`PixelizedPointContainer`. Using the :class:`PixelizedPointContainer` provides a major speedup for a small
trade-off in precision depending on the pixelization resolution.

Point
-----

.. autoclass:: Point
   :members:


PointContainer
--------------

.. autoclass:: PointContainer
   :members:
   :special-members: __getitem__


PixelizedPointContainer
-----------------------

.. autoclass:: PixelizedPointContainer
   :members:
