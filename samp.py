import numpy as np

G = 6.6743E-11
M = 5.9722E24
R = 6.3781E6

r = pow(G*M*pow((3600*24)/(2*np.pi), 2), 1/3.0)
print r
v = r * 2 * np.pi / (3600*24)
print v
