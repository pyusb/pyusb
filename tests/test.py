#!/usr/bin/env python

import sys
import unittest

sys.path.append('..')

import backendtest
import fakebackendtest
import utiltest
import inttest

backend_suite = backendtest.get_testsuite()
fakebackend_suite = fakebackendtest.get_testsuite()
util_suite = utiltest.get_testsuite()
int_suite = inttest.get_testsuite()

suite_list = (
    fakebackend_suite,
    util_suite,
    backend_suite,
    int_suite
)

test_suite = unittest.TestSuite(suite_list)
runner = unittest.TextTestRunner()
runner.run(test_suite)
