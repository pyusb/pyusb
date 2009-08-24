#!/usr/bin/env python

import sys
import unittest

sys.path.append('..')

import backend_test
import util_test
import core_test

backend_suite = backend_test.get_testsuite()
util_suite = util_test.get_testsuite()
core_suite = core_test.get_testsuite()

test_suite = unittest.TestSuite([util_suite, backend_suite, core_suite])

runner = unittest.TextTestRunner()
runner.run(test_suite)
