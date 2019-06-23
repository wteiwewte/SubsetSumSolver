#!/usr/bin/python
from __future__ import print_function
from random import randint
import numpy as np

z = input()
print(str(2*z))
for i in range(0, z):
    subfactors = []
    t = randint(10000, 15000)
    copy_t = t
    while t > 0:
        temp = randint(1, min(25, t))
        subfactors.append(temp)
        t = t -temp
    n = len(subfactors)
    
    copy_t2 = copy_t + randint(1, 2500)
    print(str(n) + ' ' + str(copy_t2))

    for number in subfactors:
        print(str(number), end = ' ')
    print()

    print(str(n) + ' ' + str(copy_t))

    for number in subfactors:
        print(str(number), end = ' ')
    print()
