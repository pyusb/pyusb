# Copyright (C) 2009-2017 Wander Lairson Costa
# Copyright (C) 2017-2018 Robert Wlodarczyk
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# All the hacks necessary to assure compatibility across all
# supported versions come here.
# Please, note that there is one version check for each
# hack we need to do, this makes maintenance easier... ^^

import sys
import array

__all__ = ['_reduce', '_set', '_next', '_update_wrapper']

# we support Python >= 2.4
assert sys.hexversion >= 0x020400f0

# On Python 3, reduce became a functools module function
try:
    import functools
    _reduce = functools.reduce
except (ImportError, AttributeError):
    _reduce = reduce

# all, introduced in Python 2.5
try:
    _all = all
except NameError:
    _all = lambda iter_ : _reduce( lambda x, y: x and y, iter_, True )

# we only have the builtin set type since 2.5 version
try:
    _set = set
except NameError:
    import sets
    _set = sets.Set

# On Python >= 2.6, we have the builtin next() function
# On Python 2.5 and before, we have to call the iterator method next()
def _next(iter):
    try:
        return next(iter)
    except NameError:
        return iter.next()

# functools appeared in 2.5
try:
    import functools
    _update_wrapper = functools.update_wrapper
except (ImportError, AttributeError):
    def _update_wrapper(wrapper, wrapped):
        wrapper.__name__ = wrapped.__name__
        wrapper.__module__ = wrapped.__module__
        wrapper.__doc__ = wrapped.__doc__
        wrapper.__dict__ = wrapped.__dict__

# this is used (as of May 2015) twice in core, once in backend/openusb, and in
# some unit test code. It would probably be clearer if written in terms of some
# definite 3.2+ API (bytearrays?) with a fallback provided for 2.4+.
def as_array(data=None):
    if data is None:
        return array.array('B')

    if isinstance(data, array.array):
        return data

    try:
        return array.array('B', data)
    except TypeError:
        # When you pass a unicode string or a character sequence,
        # you get a TypeError if the first parameter does not match
        a = array.array('B')
        a.fromstring(data) # deprecated since 3.2
        return a
