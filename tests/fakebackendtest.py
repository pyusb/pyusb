import fakebackend
import unittest

class BackendTest(unittest.TestCase):
    def __init__(self):
        unittest.TestCase.__init__(self)
        self.backend = fakebackend.get_backend()
    def runTest(self):
        pass

def get_testsuite():
    suite = unittest.TestSuite()
    suite.addTest(BackendTest())
    return suite
