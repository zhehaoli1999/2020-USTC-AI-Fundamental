import numpy as np
import qpsolvers as qs
import main
import math
from time import clock 

def kernel_linear():
    return lambda x,y: np.dot(x,y)

def kernel_poly(n=2):
    return lambda x,y: (np.dot(x,y) + 1)**n

def kernel_rbf(gamma=0.1):
    return lambda x,y: np.exp(-gamma * np.linalg.norm(x - y, 2))

def kernel_sigmoid(a = 1,b = 0): 
    return lambda x,y: np.tanh(a * np.dot(x,y) + b)

def SVM_fit(train_data, train_label, kernel, C):
    '''
    Train SVM on train data with train label 1 or -1.
    kernel: a function: linear, poly, rbf, sigmoid. 
    Param
    ---
        train data, train label, kernel, C
    Return
    ---
        w, b, alpha
    '''
    ndata = train_data.shape[0] # num of samples
    # vkernel = np.vectorize(kernel, signature='(m,n),(n)->(m)')
    
    P = np.zeros((ndata,ndata))
    for i in range(ndata):
        P[i] = np.array([kernel(train_data[j], train_data[i]) for j in range(ndata)])
    P = .5 * P * train_label.reshape((ndata, 1)) * train_label.reshape((1, ndata))   
    P = P + 1e-8 * np.eye(ndata) # Note: add a small perturbation to make this matrix positive-definite!

    q = -np.ones(ndata)
    lb = np.full(ndata, 0.0)
    ub = np.full(ndata, C)

    A = np.array(train_label, dtype= np.float)
    b = np.array([0.])

    # use quadratic programming to solve alpha
    alpha = qs.solve_qp(P, q, G=None, h=None, A=A, b=b, lb=lb, ub=ub)
    
    w = np.dot(alpha, train_data * train_label.reshape(ndata,1))

    # calc b use average 
    b = []
    for i in range(ndata):
        if alpha[i] > 1e-4:
            b.append(train_label[i] - np.dot(w, train_data[i]))
            break
    b = np.mean(b)

    return [w, b, alpha]

def test_SVM(train_data, train_label, test_data, test_label, kernel_type):        
    '''
        Test SVM. 
        Param: kernel_type: linear, gaussian, rbf
    '''
    predict = lambda x: 1 if np.dot(w, x) + b >= 0 else -1

    C = 500
    gamma = 0.001
    n = 3
    print(f"======= SVM: C = {C}, gamma = {gamma}, n = {n}, kernel type: {kernel_type} ====== ")

    if kernel_type == "linear":
        kernel = kernel_linear()
    elif kernel_type == "rbf":
        kernel = kernel_rbf(gamma)
    elif kernel_type == "poly":
        kernel = kernel_poly(n)
    elif kernel_type == "sigmoid": # problem of not positive definite!
        kernel = kernel_sigmoid()
    
    t0 = clock()    
    w, b, _ = SVM_fit(train_data, train_label, kernel, C)
    predict_label = np.array([predict(x) for x in test_data])
    t1 = clock()

    f1, p, r = main.calc_F1_score(predict_label, test_label)
    # print(f"time used: {t1 - t0}")

    return [f1, p, r, predict_label]