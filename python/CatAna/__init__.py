from .basictypes import *
from . import io
from . import besseltools
from . import decomposition


def init_random(seed=None):
    """Sets the random seed for the io and decomposition module (used for filtering) 
    
    :param seed: The random seed. If None, will be drawn from a random device.
    :type seed: unsigned integer
    """
    if seed is None:
        decomposition.init_random()
        io.init_random()
    else:
        decomposition.init_random(seed)
        io.init_random(seed)
