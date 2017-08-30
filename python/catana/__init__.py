from .basictypes import *
from . import io
from . import besseltools
from .decomposition import *


def init_random(seed=None):
    """Sets the random seed for the io and decomposition module (used for filtering) 

    The catana.io and catana.decomposition module contain their own random number generator. Calling this function will
    initialize both of them and setting the same seed, if seed is not None.

    Parameters:
        seed (unsigned int): The random seed. If None, will be drawn from a random device.
    """
    if seed is None:
        decomposition.init_random()
        io.init_random()
    else:
        decomposition.init_random(seed)
        io.init_random(seed)


from ._version import get_versions

__version__ = get_versions()['version']
del get_versions
