## módulo PC

''' Método predictor corrector de orden 5 para resolver
    el problema de valores iniciales {y}' = {F(x,{y})},
    donde {y} = {y[0],y[1],...y[N-1]}.
    X,Y = integrate(F,x0,y0,xfinal,N).
    x0,y0 = condiciones iniciales
    xfinal = valor final de x
    N = número de subintervalos de la partición
    h = incremento de x usado en la integración
    F = función suplida por el usuario que devuelve el
    array F(x,y) = {y'[0],y'[1],...,y'[N-1]}.
'''

import numpy as np
import sympy as sp

def integrate(F,x0,y0,xfinal,N):

    #Predictor
    def P(F,x0,y0,y1,y2,y3,y4,h):
        return (1901*F(x0+4*h,y4)-2774*F(x0+3*h,y3)+2616*F(x0+2*h,y2)-1274*F(x0+h,y1)+251*F(x0,y0))/720

    #Corrector
    def C(F,x0,y0,y1,y2,y3,y4,y5,h):
        return (251*F(x0+5*h,y5)+646*F(x0+4*h,y4)-264*F(x0+3*h,y3)+106*F(x0+2*h,y2)-19*F(x0+h,y1))/720
    
    X = np.linspace(x0,xfinal,N+1)
    h = (xfinal-x0)/float(N)

    # Utilizamos el método de euler explícito para calcular las 4 semillas que nos hacen falta
    # para aplicar el método. 
    y1 = y0+h*F(x0,y0)
    y2 = y1+h*F(x0+h,y1)
    y3 = y2+h*F(x0+2*h,y2)
    y4 = y3+h*F(x0+3*h,y3)
    
    Y = [y0,y1,y2,y3,y4]
    for n in range(N-4):
        y5 = Y[n+4]+h*P(F,X[n],Y[n],Y[n+1],Y[n+2],Y[n+3],Y[n+4],h)
        Y.append(Y[n+4]+h*C(F,X[n],Y[n],Y[n+1],Y[n+2],Y[n+3],Y[n+4],y5,h))
    return np.array(X),np.array(Y)
