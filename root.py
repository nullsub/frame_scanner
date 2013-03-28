#!/usr/bin/python
# vim: set fileencoding=utf-8:

import sys

voltage = float(3.3/4095)*float(sys.argv[1])

#16.2537 * x4 – 129.893 * x3 + 382.268 * x2 – 512.611 * x + 306.439
a = voltage*voltage*voltage*voltage*16.2537
b = -voltage*voltage*voltage*129.893
c = voltage*voltage*382.268
d = -voltage*512.611
e = 306.439

distance = a+b+c+d+e

distance *= 10
print distance
