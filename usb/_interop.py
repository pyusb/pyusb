# Python 2/3 interoperability module
import sys

if sys.version_info[0] > 2:
    import functools
    _reduce = functools.reduce
else:
    _reduce = reduce

# we only have the builtin set type since 2.5 version
if sys.version_info[0] <= 2 and sys.version_info[1] <= 4:
    import sets
    _set = sets.Set
else:
    _set = set
