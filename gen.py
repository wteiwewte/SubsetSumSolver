#!/usr/bin/python
from __future__ import print_function
from random import randint
import numpy as np

z = 1
print(str(z))
for i in range(0, z):
    n = randint(2000, 5000)
    t = randint(1000, 2000)
    #n = randint(200000, 300000)
    tab = [randint(0, 500) for x in range(0, n)]

    print(str(n) + ' ' + str(t))

    for number in tab:
        print(str(number), end = ' ')
    print()
