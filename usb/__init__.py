r"""PyUSB - Easy USB access in Python

This package exports the following modules and subpackages:

    core - the main USB implementation
    legacy - the compatibility layer with 0.x version
    backend - the support for backend implementations.

Since version 1.0, main PyUSB implementation lives in the 'usb.core'
module. New applications are encouraged to use it.
"""

__author__ = 'Wander Lairson Costa'

__all__ = ['legacy', 'core', 'backend', 'util']

# We import all 'legacy' module symbols to provide compatility
# with applications that use 0.x versions.
from legacy import *

