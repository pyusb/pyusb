# Python 2/3 interoperability module
import sys

if sys.version_info[0] > 2:
    import functools
    _reduce = functools.reduce
else:
    _reduce = reduce
