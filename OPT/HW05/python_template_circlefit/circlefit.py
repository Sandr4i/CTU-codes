import random
import scipy.io as sio
import numpy as np
from math import pi
from matplotlib import pyplot as plt

def quad_to_center(d,e,f):
    # x0 = d
    # y0 = e
    # r = f
    # return x0, y0, r
    x0 = -d/2
    y0 = -e/2
    r = np.sqrt((d**2 + e**2)/4 - f)
    return x0, y0, r

def fit_circle_nhom(X):
    # d = 2
    # e = 1
    # f = 1

    # return d,e,f
    A = np.column_stack((X[:, 0] + X[:, 1], X[:, 0], X[:, 1], np.ones(X.shape[0])))
    b = (X[:, 0] + X[:, 1])
    coeff = np.linalg.lstsq(A, b, rcond=None)[0]
    return coeff[0], coeff[1], coeff[2]

def fit_circle_hom(X):
    # d = 1
    # e = 2
    # f = 0.5
    # return d,e,f
    A = np.column_stack((X[:, 0]**2 + X[:, 1]**2, X[:, 0], X[:, 1], np.ones(X.shape[0])))
    U, S, Vt = np.linalg.svd(A)
    coeff = Vt[-1]
    return coeff[1]/coeff[0], coeff[2]/coeff[0], coeff[3]/coeff[0]

def dist(X, x0, y0, r):
    distance = np.sqrt((X[:, 0] -x0)**2 + (X[:, 1] - y0)**2) - r
    return distance

def fit_circle_ransac(X, num_iter, threshold):
    # d,e,f = fit_circle_nhom(X)
    # x0, y0, r = quad_to_center(d,e,f)
    # return x0, y0, r
    best = None
    max_inlines = 0
    
    for _ in range(num_iter):
        sample = X[random.sample(range(X.shape[0]), 3)]

        A = np.array([
            [sample[0,0], sample[0,1], 1],
            [sample[1,0], sample[1,1], 1],
            [sample[2,0], sample[2,1], 1],
        ])
        
        B = np.array([
            [sample[0,0]**2 + sample[0,1]**2],
            [sample[1,0]**2 + sample[1,1]**2],
            [sample[2,0]**2 + sample[2,1]**2],
        ])
        
        C = np.linalg.det(A)
        
        if(C == 0): 
            continue
        
        A_inv = np.linalg.inv(A)
        X_cent =  0.5 * np.dot(A_inv, B)
        x0 = X_cent[0,0]
        y0 = X_cent[1,0]
        r = np.sqrt((x0-sample[0,0])**2 + (y0 - sample[0,1])**2)
        
        distance = dist(X, x0, y0, r)
        inlines = np.abs(distance) < threshold
        num_inlines = np.sum(inlines)
        
        if(num_inlines > max_inlines):
            max_inlines = num_inlines
            best = (x0, y0, r)
        
    return best
        
def plot_circle(x0,y0,r, color, label):
    t = np.arange(0,2*pi,0.01)
    X = x0 + r*np.cos(t)
    Y = y0 + r*np.sin(t)
    plt.plot(X,Y, color=color, label=label)

if(__name__ == '__main__'):
    data = sio.loadmat('data.mat')
    X = data['X'] # only inliers
    A = data['A'] # X + outliers

    def_nh = fit_circle_nhom(X)
    x0y0r_nh = quad_to_center(*def_nh)
    dnh = dist(X, *x0y0r_nh)

    def_h = fit_circle_hom(X)
    x0y0r_h = quad_to_center(*def_h)
    dh = dist(X, *x0y0r_h)

    results = {'def_nh':def_nh, 'def_h':def_h, 
               'x0y0r_nh' : x0y0r_nh, 'x0y0r_h': x0y0r_nh,
               'dnh': dnh, 'dh':dh}
    
    GT = sio.loadmat('GT.mat')
    for key in results:
        print('max difference',  np.amax(np.abs(results[key] - GT[key])), 'in', key)


    x = fit_circle_ransac(A, 2000, 0.1)

    plt.figure(1)
    plt.subplot(121)
    plt.scatter(X[:,0], X[:,1], marker='.', s=3)
    plot_circle(*x0y0r_h, 'r', 'hom')
    plot_circle(*x0y0r_nh, 'b', 'nhom')
    plt.legend()
    plt.axis('equal')    
    plt.subplot(122)
    plt.scatter(A[:,0], A[:,1], marker='.', s=2)
    plot_circle(*x, 'y', 'ransac')
    plt.legend()
    plt.axis('equal')
    plt.show()
    
