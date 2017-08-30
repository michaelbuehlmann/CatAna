Miscellaneous (C++)
===================

Random Number Generator
-----------------------

The library uses a RNG that is shared amongst the different functions that utilize it. To initialize (=seed) it, there
is a :cpp:func:`catana::init_random()` function.

.. doxygenfunction:: catana::init_random()

.. doxygenfunction:: catana::init_random(unsigned int)
