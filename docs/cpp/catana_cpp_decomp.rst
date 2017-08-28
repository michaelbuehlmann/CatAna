SFB Decomposition (C++)
=======================

Return Types
------------

.. doxygenstruct:: catana::KClkk
   :members:
   :undoc-members:


SFB Decomposition
-----------------

.. doxygenfunction:: catana::sfb_decomposition(PixelizedPointContainer&, unsigned short, unsigned short, double, double, bool, bool)

.. doxygenfunction:: catana::sfb_decomposition(const PointContainer&, unsigned short, unsigned short, double, double, bool, bool)


Analyzer
--------

The :cpp:class:`catana::io::Analyzer` class is a convenience wrapper combining data input, filtering and analyzing.
It requires a :cpp:class:`catana::io::Source` and arbitrary :cpp:class:`catana::io::Filter` instances can be added
for filtering.

.. doxygenclass:: catana::Analyzer
   :members:
   :undoc-members:
