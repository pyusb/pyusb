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

import utils
import unittest
import glob
import os.path

if __name__ == '__main__':
    suite = unittest.TestSuite()

    for i in glob.glob('*.py'):
        m = __import__(os.path.splitext(i)[0])
        if hasattr(m, 'get_suite'):
            suite.addTest(m.get_suite())

    utils.run_tests(suite)
