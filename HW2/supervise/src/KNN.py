import numpy as np
from time import clock
import main

def quadratic_knn_search(dataset, testdata, K):
    '''
        Find the K nearest neighbors of testdata in dataset 
        Param
        --- 
            dataset: train/test dataset
            testdata: single data vector 
            K: knn param
        Return
        ---
            the idx of K results in dataset. 
    '''
    ndata = dataset.shape[0]
    K = K if K < ndata else ndata
    distance = ((testdata - dataset)**2).sum(axis = 1)
    idx = np.argsort(distance)
    idx = idx[:K] # select the cloest K points 
    return idx 

def knn_predict(train_data, train_label, test_data, K):
    '''
        Predict label with knn 
        Param
        ---
            train_data: train dataset 
            test_data: test dataset 
            K: cluster num 
        Return
        ---
            prediction label of testdata
    '''
    knn_predict_label = []
    n_test_data = test_data.shape[0]
    for i in range(n_test_data):
        knn_idx = quadratic_knn_search(train_data, test_data[i], K)
        predict_label = 1 if train_label[knn_idx].sum() > K / 2 else 0
        knn_predict_label.append(predict_label) 
    return knn_predict_label


def test_knn(train_data, train_label, test_data, test_label, K):
    '''
    Test KNN. main.py should call this function for testing. 
    Param
    ---
        train data, train label, test data, test label, K
    Return
        predict label
    ---

    '''
    print(f"======= KNN: K = {K} ====== ")

    t0 = clock()
    predict_label = knn_predict(train_data, train_label, test_data, K)
    f1, p, r = main.calc_F1_score(predict_label, test_label)
    t1 = clock()
    
    # print(f"time used: {t1 - t0}")

    return [f1, p, r, predict_label]