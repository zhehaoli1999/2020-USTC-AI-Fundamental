import numpy as np
from numpy import linalg as LA

def PCA(train_data, m=2):
    '''
    Primary Component Analysis. 
    Param
    ---
        traindata: (ndim, ndata)
        m: number of eigenvalues to be selected 
        # TODO should change m to threshold
    Return
    ---
        compressed data, matrix P, eigen value array w
    '''
    # calculate covariance matrix S
    ndim = train_data.shape[0]
    S = np.cov(train_data - np.mean(train_data, axis=1).reshape((ndim,1)))
    assert(S.shape == (ndim, ndim))

    w, v = LA.eig(S) # w: eigen values, v: eigen vectors as column
    print(w)
    selected_idx = np.argsort(w)[::-1][:m]
    P = v[:,selected_idx]
    assert(P.shape == (ndim, m))
    print(selected_idx)
    print(w)
    print("")
    # print(v)
    print("")
    # print(P)

    new_data = np.matmul(P.T, train_data)
    return [new_data.T, P, w]

