import numpy as np
from numpy import linalg as LA

def PCA(train_data, m=2, use_threshold = False, threshold = 0.9):
    '''
    Primary Component Analysis. 
    Param
    ---
        traindata: (ndim, ndata)
        m: number of eigenvalues to be selected 
        use threshold: true of false. If true, m is not used.
        threshold: set to 0.9 by default. 
    Return
    ---
        compressed data, matrix P, eigen value array w
    '''
    # calculate covariance matrix S
    ndim = train_data.shape[0]
    S = np.cov(train_data - np.mean(train_data, axis=1).reshape((ndim,1)))
    assert(S.shape == (ndim, ndim))

    w, v = LA.eig(S) # w: eigen values, v: eigen vectors as column
    if(use_threshold == False):
        selected_idx = np.argsort(w)[::-1][:m]
    else:
        w = np.sort(w)[::-1]
        w_normalized = w / np.sum(w)
        print(w_normalized)

        for i in range(1, len(w_normalized)):
            w_normalized[i] = w_normalized[i] + w_normalized[i-1]
        selected_idx = np.argwhere(w_normalized <= threshold)
        selected_idx = selected_idx.reshape(len(selected_idx))
    P = v[:,selected_idx]

    new_data = np.matmul(P.T, train_data)
    return [new_data.T, P, w]

