from gmpy2 import mpq, mpz

def mod1(x):
    return x-mpz(x)
def pi():
    x = 0
    n = 1
    while 1:
        p = mpq((120*n-89)*n+16, (((512*n-1024)*n+712)*n-206)*n+21)
        x = mod1(16*x + p)
        n += 1
        yield int(16*x)
        
def allpi():
    for n, p in enumerate(pi()):
        print("%x" % p)
        
        if n == 1000:
            break
            
allpi()