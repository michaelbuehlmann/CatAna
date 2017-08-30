Miscellaneous
=============

.. currentmodule:: catana


Random number generator
-----------------------

The RNG used internally for random subsets and probability filtering is automatically seeded with a random seed when
the module is being imported. To manually set the seed use the `init_random` function.

.. autofunction:: init_random


Function Interpolator
---------------------

.. autoclass:: FunctionInterpolator
   :members:
   :special-members: __call__