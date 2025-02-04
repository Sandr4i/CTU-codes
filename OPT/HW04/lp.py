import numpy as np
from scipy.optimize import linprog
import matplotlib.pyplot as plt


#Prvni uloha
def vyhra(c, k):
    A_ub = np.array([
        [-c[0], -c[1], 0, 0, 0, 1],
        [0, -c[1], -c[2], -c[3], 0, 1], 
        [0, 0, 0, -c[3], -c[4], 1]
    ])
    obj = [0, 0, 0, 0, 0, -1]
    A_eq = [[1, 1, 1, 1, 1, 0]]
    b_eq = np.array([k])
    b_ub = np.zeros(3)

    ret = linprog(obj, A_ub,  b_ub, A_eq, b_eq)

    return ret.x[:5]

def vyhra2(c, k, m):
    A_ub = np.array([
        [-c[0], 0, 0, 1],
        [0, -c[1], 0, 1], 
        [0, 0, -c[2], 1]
    ])
    obj = [0, 0, 0, -1]
    A_eq = [[1, 1, 1, 0]]
    b_eq = np.array([k])
    b_ub = np.zeros(3)
    
    bounds = [(m, None), (m, None), (m, None), (None, None)]
    
    ret = linprog(obj, A_ub, b_ub, A_eq, b_eq, bounds)

    return ret.x[:3]
#----------------------------
#Druha uloha

def minimaxfit(x,y):
    n,m = x.shape
    c = np.zeros(n+2)
    c[-1] = 1
    
    A = np.zeros((m*2, n+2))
    b = np.zeros(m*2)
    
    for i in range(m):
        xi = x[:, i]
        yi = y[0, i]
        
        A[i, :n] = xi
        A[i, n] = 1
        A[i, n+1] = -1
        A[i+m, :n] = -xi
        A[i+m, n] = -1
        A[i+m, n+1] = -1
        b[i] = yi
        b[i+m] = -yi
        
    bounds  = [(None, None)] * (n+1) + [(0,None)]
    ret = linprog(c, A, b, bounds=bounds)
    a = ret.x[:n]
    b = ret.x[n]
    r = ret.x[-1]
    
    return a, b, r
        
    
     
def plotline(x,y,a,b,r):
    x_values = np.dot(a, x) + b
    y_values = y[0, :]
    
    y_fit = np.dot(a, x)+b
    upper = y_fit + r
    lower = y_fit - r
    
    plt.plot(x_values, y_values, 'x', label='Data')
    plt.plot(x_values, y_fit, 'g--', label='Prediction')
    plt.plot(x_values, upper, 'r--', label='Upper border')
    plt.plot(x_values, lower, 'r--', label='Lower border')

    plt.xlabel('x')
    plt.ylabel('y')
    plt.axis('tight')
    plt.axis('equal')
    plt.legend()
    plt.show()

if __name__=="__main__":
    #Prvni uloha
    c = np.array([1.27, 1.02, 4.70, 3.09, 9.00])
    k = 3000
    x = vyhra(c, k)
    print(x)
    m = np.min([c[0]*x[0] + c[1]*x[1], c[1]*x[1] + c[2]*x[2] + c[3]*x[3], c[4]*x[4]])
    print(m)
    
    c = np.array([1.27, 4.70, 9.00])
    k = 3000
    m = 400
    x = vyhra2(c,k,m)
    print(x)
    #---------------------------
    #Druha uloha
    x = np.array([[1, 2, 3, 3, 2], [4, 1, 2, 5, 6], [7,8,9, -5,7]])
    #x = np.array([[1, 2, 3, 3, 2]])
    y = np.array([[7,4,1,2,5]])
    a, b, r = minimaxfit(x,y)
    a = np.array(a)
    print("a: ",a ,"b: ", b,"r: ", r)
    x = np.array([[1, 2, 3, 3, 2]])
    #plotline(x,y,a,b,r)
    