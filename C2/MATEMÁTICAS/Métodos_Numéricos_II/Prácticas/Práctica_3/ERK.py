## módulo ERK
''' Método de Runge-Kutta explícito para resolver
    el problema de valores iniciales {y}' = {F(x,{y})}, 
    donde    {y} = {y[0],y[1],...y[N-1]}.
    X,Y    = integrate(F,x0,y0,xfinal,h).
    x0,y0  = condiciones iniciales
    xfinal = valor final de x
    N      = número de subintervalos de la partición
    h      = incremento de x usado en la integración
    F      = función suplida por el usuario que devuelve el
            array F(x,y) = {y'[0],y'[1],...,y'[N-1]}.
    butcher = arreglo de butcher
'''
import numpy as np
def integrate(F,x0,y0,xfinal,butcher,N):
    a,b,c = [], [], []

    for k in butcher:
        c.append(k[0])
        a.append(k[1])
        b.append(k[2])
    
    def RK(F,xn,yn,h):
        K = []
        for i in range(len(butcher)):
            s = 0
            for j in range(i):
                s += a[i][j]*K[j]
            K.append(F(xn+c[i]*h,yn+h*s))
        
        return sum(b[i]*K[i] for i in range(len(butcher)))
    
    X = np.linspace(x0,xfinal,N+1)
    Y = [y0]; h = (xfinal-x0)/N
    for n in range(N):
        Y.append( Y[n] + h*RK(F,X[n],Y[n],h) )
    return np.array(X),np.array(Y)
