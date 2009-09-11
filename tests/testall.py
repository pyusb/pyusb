#!/usr/bin/env python

import sys
import unittest
import glob
import os.path

sys.path.append('..')
suite = unittest.TestSuite()

for i in glob.glob('*.py'):
    m = __import__(os.path.splitext(i)[0])
    try:
        suite.addTest(m.get_suite())
    except:
        continue

runner = unittest.TextTestRunner()
runner.run(suite)
