Input/Output Module
===================

.. automodule:: catana.io
   :noindex:

.. module:: catana.io

The ``catana.io`` module contains functions and classes to read data from files and containers, filters that can be
applied to objects (points) and facilites to write those points to files and containers. The implementation follows
a "data-stream" concept, where the data flows from a source through one or multiple filters to a so-called sink.

The available sources, sinks and filters are listed below. The data-stream pipeline is implemented in the `FilterStream`
class. If only reading from a file to a `PointContainer` or `PixelizedPointContainer` is required you can directly use
the ``Source.get_point_container`` and ``Source.get_pixelized_point_container`` functions.


Sources
-------

Source
^^^^^^

.. autoclass:: Source

CartesianTextSource
^^^^^^^^^^^^^^^^^^^
.. autoclass:: CartesianTextSource
   :members:
   :inherited-members:

SphericalTextSource
^^^^^^^^^^^^^^^^^^^

.. autoclass:: SphericalTextSource
   :members:
   :inherited-members:

   .. note::

       For 3dex compatible text files with columns [theta, phi, r], use the `SphericalTextSource_3dex` class with
       the same functionality and syntax.

GadgetSource
^^^^^^^^^^^^

.. autoclass:: GadgetSource
   :members:
   :inherited-members:

PointContainerSource
^^^^^^^^^^^^^^^^^^^^

.. autoclass:: PointContainerSource
   :members:
   :inherited-members:

RawBinarySource
^^^^^^^^^^^^^^^

.. autoclass:: RawBinarySource
   :members:
   :inherited-members:

------------------------------------------------------------------------------------------------------------------------


Sinks
-----

Sink
^^^^

.. autoclass:: Sink

CartesianTextSink
^^^^^^^^^^^^^^^^^

.. autoclass:: CartesianTextSink
   :members:
   :inherited-members:

SphericalTextSink
^^^^^^^^^^^^^^^^^

.. autoclass:: SphericalTextSink
   :members:
   :inherited-members:

   .. note::

      For 3dex compatible text files with columns [theta, phi, r], use the `SphericalTextSink_3dex` class with
      the same functionality and syntax.

PointContainerSink
^^^^^^^^^^^^^^^^^^

.. autoclass:: PointContainerSink
   :members:
   :inherited-members:

RawBinarySink
^^^^^^^^^^^^^

.. autoclass:: RawBinarySink
   :members:
   :inherited-members:


------------------------------------------------------------------------------------------------------------------------

.. _filters:

Filters
-------

Filter (abstract class)
^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: Filter

TophatRadialWindowFunctionFilter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: TophatRadialWindowFunctionFilter
   :members:
   :inherited-members:
   :special-members: __call__

GaussianRadialWindowFunctionFilter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: GaussianRadialWindowFunctionFilter
   :members:
   :inherited-members:
   :special-members: __call__

GenericRadialWindowFunctionFilter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. autoclass:: GenericRadialWindowFunctionFilter
   :members:
   :inherited-members:
   :special-members: __call__

AngularMaskFilter
^^^^^^^^^^^^^^^^^

.. autoclass:: AngularMaskFilter
   :members:
   :inherited-members:
   :special-members: __call__


------------------------------------------------------------------------------------------------------------------------

FilterStream
------------

To combine a Source -> Filter -> Sink together, we can use a FilterStream

.. autoclass:: FilterStream
   :members:
