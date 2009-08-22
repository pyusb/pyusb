#!/usr/bin/env python

import sys
import unittest

sys.path.append('..')

import backend_test

backend_suite = backend_test.get_testsuite()

test_suite = unittest.TestSuite([backend_suite])

runner = unittest.TextTestRunner()
runner.run(test_suite)
