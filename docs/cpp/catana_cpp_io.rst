Input / Output (C++)
====================

Sources
-------

Source (parent)
^^^^^^^^^^^^^^^

.. doxygenclass:: catana::io::Source
   :members:
   :undoc-members:

TextSource
^^^^^^^^^^

.. doxygenclass:: catana::io::TextSource
   :members:
   :undoc-members:

GadgetSource
^^^^^^^^^^^^

.. doxygenclass:: catana::io::GadgetSource
   :members:
   :undoc-members:

RawBinarySource
^^^^^^^^^^^^^^^

.. doxygenclass:: catana::io::RawBinarySource
   :members:
   :undoc-members:

PointContainerSource
^^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: catana::io::PointContainerSource
   :members:
   :undoc-members:


Sinks
-----

Sink (parent)
^^^^^^^^^^^^^

.. doxygenclass:: catana::io::Sink
   :members:
   :undoc-members:

TextSink
^^^^^^^^

.. doxygenclass:: catana::io::TextSink
   :members:
   :undoc-members:

RawBinarySink
^^^^^^^^^^^^^

.. doxygenclass:: catana::io::RawBinarySink
   :members:
   :undoc-members:

PointContainerSink
^^^^^^^^^^^^^^^^^^
.. doxygenclass:: catana::io::PointContainerSink
   :members:
   :undoc-members:


Filters
-------

Filter (parent)
^^^^^^^^^^^^^^^

.. doxygenclass:: catana::io::Filter
   :members:
   :undoc-members:

TophatRadialWindowFunctionFilter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: catana::io::TophatRadialWindowFunctionFilter
   :members:
   :undoc-members:

GaussianRadialWindowFunctionFilter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: catana::io::GaussianRadialWindowFunctionFilter
   :members:
   :undoc-members:

GenericRadialWindowFunctionFilter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenclass:: catana::io::GenericRadialWindowFunctionFilter
   :members:
   :undoc-members:

AngularMaskFilter
^^^^^^^^^^^^^^^^^

.. doxygenclass:: catana::io::AngularMaskFilter
   :members:
   :undoc-members:


FilterStreams
-------------

To combine a Source -> Filter -> Sink together, we can use a FilterStream

.. doxygenclass:: catana::io::FilterStream
   :members:
   :undoc-members:


Examples
--------
