#!/usr/bin/python3
from __future__ import print_function
from random import randint
import numpy as np

z = input()
print(str(z))
for i in range(0, int(z)):
    n = 100
    t = 100
    tab = [randint(0, 10) for x in range(0, n)]

    print(str(n) + ' ' + str(t))

    for number in tab:
        print(str(number), end = ' ')
    print()
