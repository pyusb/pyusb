# Copyright (C) 2009-2010 Wander Lairson Costa 
# 
# The following terms apply to all files associated
# with the software unless explicitly disclaimed in individual files.
# 
# The authors hereby grant permission to use, copy, modify, distribute,
# and license this software and its documentation for any purpose, provided
# that existing copyright notices are retained in all copies and that this
# notice is included verbatim in any distributions. No written agreement,
# license, or royalty fee is required for any of the authorized uses.
# Modifications to this software may be copyrighted by their authors
# and need not follow the licensing terms described here, provided that
# the new terms are clearly indicated on the first page of each file where
# they apply.
# 
# IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY
# FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
# ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
# DERIVATIVES THEREOF, EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# 
# THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
# INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
# IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE
# NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
# MODIFICATIONS.

# All the hacks necessary to assure compatibility across all
# supported versions come here.
# Please, note that there is one version check for each
# hack we need to do, this makes maintenance easier... ^^

import sys

__all__ = ['_reduce', '_set', '_next', '_groupby', '_sorted']

_major_ver = sys.version_info[0]
_minor_ver = sys.version_info[1]

# we support Python >= 2.3
assert _major_ver >= 2
if _major_ver == 2:
    assert _minor_ver >= 3

# On Python 3, reduce became a functools module function
if _major_ver > 2:
    import functools
    _reduce = functools.reduce
else:
    _reduce = reduce

# we only have the builtin set type since 2.5 version
if _major_ver == 2 and _minor_ver <= 4:
    import sets
    _set = sets.Set
else:
    _set = set

# On Python >= 2.6, we have the builtin next() function
# On Python 2.5 and before, we have to call the iterator method next()
if _major_ver == 2 and _minor_ver < 6:
    def _next(iter):
        return iter.next()
else:
    def _next(iter):
        return next(iter)

# groupby is available only since 2.4 version
if _major_ver == 2 and _minor_ver < 4:
    # stolen from Python docs
    class _groupby(object):
        # [k for k, g in groupby('AAAABBBCCDAABBB')] --> A B C D A B
        # [list(g) for k, g in groupby('AAAABBBCCD')] --> AAAA BBB CC D
        def __init__(self, iterable, key=None):
            if key is None:
                key = lambda x: x
            self.keyfunc = key
            self.it = iter(iterable)
            self.tgtkey = self.currkey = self.currvalue = object()
        def __iter__(self):
            return self
        def next(self):
            while self.currkey == self.tgtkey:
                self.currvalue = _next(self.it)    # Exit on StopIteration
                self.currkey = self.keyfunc(self.currvalue)
            self.tgtkey = self.currkey
            return (self.currkey, self._grouper(self.tgtkey))
        def _grouper(self, tgtkey):
            while self.currkey == tgtkey:
                yield self.currvalue
                self.currvalue = _next(self.it)    # Exit on StopIteration
                self.currkey = self.keyfunc(self.currvalue)
else:
    import itertools
    _groupby = itertools.groupby

# builtin sorted function is only availale since 2.4 version
if _major_ver == 2 and _minor_ver < 4:
    def _sorted(l, key=None, reverse=False):
        # sort function on Python 2.3 does not
        # support 'key' parameter
        class KeyToCmp(object):
            def __init__(self, K):
                self.key = K
            def __call__(self, x, y):
                kx = self.key(x)
                ky = self.key(y)
                if kx < ky:
                    return reverse and 1 or -1
                elif kx > ky:
                    return reverse and -1 or 1
                else:
                    return 0
        tmp = list(l)
        tmp.sort(KeyToCmp(key))
        return tmp
else:
    _sorted = sorted
