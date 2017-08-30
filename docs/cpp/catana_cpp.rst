Points and Containers (C++)
===========================

Point
-----

Point
^^^^^

.. doxygenstruct:: catana::Point
   :members:

From cartesian coordinates
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: catana::point_from_box_position(const double&, const double&, const double&, const double&, const double&)

.. doxygenfunction:: catana::point_from_box_position(const FLOAT_TYPE *, const double&, const double&)

From spherical coordinates
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygenfunction:: catana::point_from_spherical_position(const double&, const double&, const double&, const double&)

.. doxygenfunction:: catana::point_from_spherical_position(const FLOAT_TYPE *, const double&)


PointContainer
--------------

PointContainer
^^^^^^^^^^^^^^

.. doxygenclass:: catana::PointContainer
   :members:

Subsampling
^^^^^^^^^^^

.. doxygenfunction:: catana::create_point_container_subset

PixelizedPointContainer
-----------------------

.. doxygenclass:: catana::PixelizedPointContainer
   :members: