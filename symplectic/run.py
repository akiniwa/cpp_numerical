class C1:
    def __init__(self):
        print 'C1'

    x = 3
    def __del__(self):
        print 'del'

class C2(C1):
    def __init__(self):
        print 'C2'

    def call(self):
        a = 4
        self.ddd = 6
        return a

    def __del__(self):
        print 'C2del'

a = C1()
print a.x

b = C2()
print b.x
print b.call()
print b.x
print b.ddd
print C1.x

import house
print dir(house)
print house.__path__
print house.__package__
import house.roof as hr
print dir(hr)

import numpy as np
print dir(np.__package__)
print dir(__path__)
