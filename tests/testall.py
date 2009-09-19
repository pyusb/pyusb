#!/usr/bin/env python

import sys
import unittest
import glob
import os.path

if __name__ == '__main__':
    sys.path.append('..')
    suite = unittest.TestSuite()

    for i in glob.glob('*.py'):
        m = __import__(os.path.splitext(i)[0])
        if hasattr(m, 'get_suite'):
            suite.addTest(m.get_suite())

    runner = unittest.TextTestRunner()
    runner.run(suite)
