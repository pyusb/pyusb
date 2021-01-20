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

from usb._debug import methodtrace
import unittest
import utils

from array import array
from usb._interop import as_array

class InteropTest(unittest.TestCase):
    @methodtrace(utils.logger)
    def test_none_as_array(self):
        self.assertEqual(as_array(None), array('B'))

    @methodtrace(utils.logger)
    def test_byte_array_as_array(self):
        data = array('B', [10, 20, 30])
        self.assertEqual(as_array(data), data)
        self.assertIs(as_array(data), data)

    @methodtrace(utils.logger)
    def test_byte_list_as_array(self):
        self.assertEqual(as_array([10, 20, 30]), array('B', [10, 20, 30]))

    @methodtrace(utils.logger)
    def test_byte_typle_as_array(self):
        self.assertEqual(as_array((10, 20, 30)), array('B', [10, 20, 30]))

    @methodtrace(utils.logger)
    def test_bytes_as_array(self):
        self.assertEqual(as_array(b'\x10\x20\x30'), array('B', [16, 32, 48]))

    @methodtrace(utils.logger)
    def test_unicode_string_as_array(self):
        self.assertEqual(as_array('Πύ'), array('B', b'\xce\xa0\xcf\x8d'))


def get_suite():
    suite = unittest.TestSuite()
    suite.addTest(unittest.defaultTestLoader.loadTestsFromTestCase(InteropTest))
    return suite

if __name__ == '__main__':
    utils.run_tests(get_suite())
