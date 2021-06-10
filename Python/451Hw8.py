import math
import matplotlib.pyplot as plt

def f(x):
    return math.pow(math.e,x)+x+7
def secant(xk,xkm1,f,number):
    for i in range(number):
        if(f(xk)-f(xkm1) == 0):
            break
        xkp1 = xk-((xk-xkm1)/(f(xk)-f(xkm1)))*f(xk)
        xkm1 = xk
        xk = xkp1
        print("xk: ",xk)
    return xkp1
def foo(x):
    return math.pow(math.e,math.pow(math.sin(x),3))+math.pow(x,6)-2*math.pow(x,4)-math.pow(x,3)-1
def dxfoo(x):
    return math.pow(math.e,math.pow(math.sin(x),3))*3*math.pow(math.sin(x),2)*math.cos(x)+6*math.pow(x,5)-8*math.pow(x,3)-3*math.pow(x,2)
def newton(f,df,x0,number,tol):
    n=0
    dx=f(x0)/df(x0)
    while ((dx>tol) and (f(x0)>tol) or (n<number)):
        n+=1
        x0-=dx
        dx=f(x0)/df(x0)
    return x0-dx
u=[]
v=[]
for i in range(401):
    u.append((i/100)-2)
    v.append(foo((i/100)-2))
plt.scatter(u, v)
plt.show()
