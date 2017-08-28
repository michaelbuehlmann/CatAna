CatAna Points and Containers
============================

.. currentmodule:: catana

Objects in CatAna are points in space given by their angular coordinates and radial distance. To create spherical
clustering statistics on such points, they have to be gathered in either a :class:`PointContainer` or a
:class:`PixelizedPointContainer`. Using the :class:`PixelizedPointContainer` provides a major speedup for a small
trade-off in precision depending on the pixelization resolution.

.. currentmodule:: catana.basictypes

.. autoclass:: Point
   :members:

.. autoclass:: PointContainer
   :members:

.. autoclass:: PixelizedPointContainer
   :members:
