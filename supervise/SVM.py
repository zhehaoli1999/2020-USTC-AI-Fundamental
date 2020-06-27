import numpy as np
import qpsolvers as qs
# from main import *
import main
import math
from time import clock 

def kernel_linear(x,y):
    return np.dot(x,y)

def kernel_poly(x,y,n):
    return (np.dot(x,y) + 1)**n

def kernel_rbf(x,y,sigma):
    return math.exp( - np.linalg.norm(x - y, 2) / sigma**2)

def kernel_sigmoid(x,y,k,sigma):
    pass

def SVM_fit(train_data, train_label, kernel, C):
    '''
    '''
    ndata = train_data.shape[0] # num of samples
    vkernel = np.vectorize(kernel, signature='(m,n),(n)->(m)')
    
    P = np.zeros((ndata,ndata))
    for i in range(ndata):
        P[i] = vkernel(train_data , train_data[i])

    P = .5 * P * train_label.reshape((ndata, 1)) * train_label.reshape((1, ndata))   
    P = P + 1e-8 * np.eye(ndata) # Note 

    q = -np.ones(ndata)
    lb = np.full(ndata, 0.0)
    ub = np.full(ndata, C)

    A = np.array(train_label, dtype= np.float)
    b = np.array([0.])

    # use quadratic programming to solve alpha
    alpha = qs.solve_qp(P, q, G=None, h=None, A=A, b=b, lb=lb, ub=ub)
    
    w = np.dot(alpha, train_data * train_label.reshape(ndata,1))

    # calc b 
    for i in range(ndata):
        if alpha[i] > 1e-4:
            b = train_label[i] - np.dot(w, train_data[i])
            break

    return [w, b, alpha]

def test_SVM(train_data, train_label, test_data, test_label, kernel_type):        
    '''
        Test SVM. 
        Param: kernel_type: linear, gaussian, rbf
    '''

    predict = lambda x: 1 if np.dot(w, x) + b >= 0 else -1


    print(f"======= SVM: C = {C}, kernel type: {kernel_type} ====== ")

    if kernel_type == "linear":
        kernel = kernel_linear
    
    t0 = clock()    
    w, b, _ = SVM_fit(train_data, train_label, kernel, C)
    predict_label = np.array([predict(x) for x in test_data])
    t1 = clock()

    main.calc_F1_score(predict_label, test_label)
    print(f"time used: {t1 - t0}")