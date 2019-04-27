#!/usr/bin/env python

import autograder_base
import os.path

from autograder_base import file_locations, AbsoluteTestCase, FractionalTestCase, main

tests = [
  ("CPU addiu test",AbsoluteTestCase(os.path.join(file_locations,'CPU-addiu.circ'),os.path.join(file_locations,'out-files/CPU-addiu.out'),1)),
  ("CPU lui test",AbsoluteTestCase(os.path.join(file_locations,'CPU-lui.circ'),os.path.join(file_locations,'out-files/CPU-lui.out'),1)),
   ("CPU slt test",AbsoluteTestCase(os.path.join(file_locations,'CPU-slt.circ'),os.path.join(file_locations,'out-files/CPU-slt.out'),1)),
  ("CPU halt test",AbsoluteTestCase(os.path.join(file_locations,'CPU-halt.circ'),os.path.join(file_locations,'out-files/CPU-halt.out'),1)),

]

if __name__ == '__main__':
  main(tests)
