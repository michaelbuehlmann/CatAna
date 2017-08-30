Basic Usage
===========

Points and PointContainers
--------------------------

The main objects in CatAna are `points <Point>`, coordinates in 3d space stored in spherical components.
They can either be directly obtained from python arrays or read from files using the `catana.io` module.

Points are collected in `point containers <PointContainer>`, which can then be given to a spherical Fourier-Bessel
transform routine to analyze.

.. code-block:: python

   import numpy as np
   import catana

   # Some random particle positions in a 200x200x200 box centered at 0
   python_particles = np.random.uniform(-100, 100, (100000, 3))

   # Load into PointContainer
   point_container = catana.PointContainer(python_particles, 'cartesian')

   # If we want to retreive a numpy array from the point_container again
   # (note that the coordinates will be spherical, with columns [r, theta, phi])
   python_particles_spherical = np.array(point_container)


Computing the SFB transform will be faster if the points are binned in angular pixels. CatAna uses the
`HEALPix <http://healpix.sourceforge.net/>`_ pixelization scheme. The resolution of the angular grid is given by
the ``nside`` parameter, which determines how many times each base pixel is subdivided and has to be a power of 2.

CatAna contains a specialized container, the `PixelizedPointContainer`. It can be used as follows:

.. code-block:: python

   # Create a PixelizedPointContainer with nside 64
   pixelized_point_container = catana.PixelizedPointContainer(64, point_container)

   # We can create a HEALPix map with each pixel containing the number of points it contains
   import healpy as hp
   countmap = pixelized_point_container.get_countmap()
   hp.mollview(countmap)

.. image:: figures/countmap.png


Computing the SFB transform
---------------------------

The SFB decomposition can either be directly done on `PointContainers <PointContainer>` (brute-force) or on
`PixelizedObjectContainers <PixelizedPointContainer>` which is much faster.

.. code-block:: python

   # brute-force from PointContainer (not recommended)
   kclkk = catana.sfb_decomposition(point_container, lmax=100, nmax=100, rmax=100, 1, store_flmn=True)
   k_ln = np.array(kclkk.k_ln)
   c_ln = np.array(kclkk.c_ln)
   f_2mn = np.array(kclkk.f_lmn[2])


.. note:: Normalization

   (Add description on how to normalize output)

Reading Data
------------

