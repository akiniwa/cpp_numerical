import numpy as np

x = np.random.randn(1000)
mean = x.mean()
std = x.std()
var = x.var()

print mean
print std
print var
