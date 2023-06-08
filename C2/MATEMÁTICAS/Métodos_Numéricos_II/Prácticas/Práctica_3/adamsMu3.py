## módulo AM3
''' Método de Adams-Moulton (con k = 3) para resolver
    el problema de valores iniciales {y}' = {F(x,{y})}, 
    donde    {y} = {y[0],y[1],...y[N-1]}.
    X,Y    = integrate(F,x0,y0,xfinal,N).
    x0,y0  = condiciones iniciales
    xfinal = valor final de x
    N      = número de subintervalos de la partición
    h      = incremento de x usado en la integración
    F      = función suplida por el usuario que devuelve el
            array F(x,y) = {y'[0],y'[1],...,y'[N-1]}.
'''

def integrate(F,x0,y0,xfinal,N):
    
    import numpy as np
    import sympy as sp
    
    def AM3(F,x0,y0,y1,y2,y_3,h):
        return 9/24*F(x0+3*h, y_3) + 19/24*F(x0+2*h, y2) - 5/24*F(x0+h, y1) + 1/24*F(x0, y0)
    
    X = np.linspace(x0,xfinal,N+1)
    h = (xfinal-x0)/N
    Y = [y0]
    
    for i in range(3-1):  # num_pasos - 1
        y1 = y0 + h*F(x0,y0)
        Y.append(y1)
        y0 = y1
        
    y_2 = sp.Symbol('y_2')
    y3 = sp.Symbol('y3') 
    Y_list = Y  # Lista para almacenar los valores calculados

    for n in range(N-1):
        Y_list.append(sp.solve(Y_list[n] + h*AM3(F, X[n], Y_list[n], Y_list[n+1], Y_list[n+2], y3, h) - y3, y_2)[0])
    
    return X, np.array(Y_list) 
