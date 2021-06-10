import math
def f(x):
    return math.pow(math.e,x)+x+7
def secant(xk,xkm1,f,number):
    for i in range(number):
        xkp1 = xk-(xk-xkm1)/(f(xk)-f(xkm1))*f(xk)
        xkm1 = xk
        xk = xkp1
        print("xk: ",xk)
    return xkp1
