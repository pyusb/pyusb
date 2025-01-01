# Copyright 2009-2017 Wander Lairson Costa
# Copyright 2009-2021 PyUSB contributors
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# 1. Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# All the hacks necessary to assure compatibility across all
# supported versions come here.
# Please, note that there is one version check for each
# hack we need to do, this makes maintenance easier... ^^

import sys
import array

__all__ = []

# we support Python >= 3.9
assert sys.hexversion >= 0x030900f0

def as_array(data=None):
    """Convert loosely specified `data` to a byte array.

    The following types of `data` are supported:

    - the `None` value (returns an new empty array);
    - an `array('B')` value (no-op, returns it back);
    - lists or iterables of small enough integers;
    - strings.
    """

    if data is None:
        return array.array('B')

    if isinstance(data, array.array) and data.typecode == 'B':
        return data

    if isinstance(data, str):
        return array.array('B', data.encode('utf-8'))

    return array.array('B', data)
