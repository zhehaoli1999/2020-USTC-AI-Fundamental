import numpy as np
from time import clock
from main import calc_F1_score

def quadratic_knn_search(dataset, testdata, K):
    '''
        Funtion: Find the K nearest neighbor of testdata in dataset 
        Param: 
            dataset: train/test dataset
            testdata: single data vector 
            K: knn param
        Return: the idx of K results in dataset. 
    '''
    ndata = dataset.shape[0]
    K = K if K < ndata else ndata
    distance = ((testdata - dataset)**2).sum(axis = 1)
    idx = np.argsort(distance)
    idx = idx[:K] # select the cloest K points 
    return idx 

def knn_predict(train_data, train_label, test_data, K):
    '''
        Function: predict label with knn 
        Param: 
            train_data: train dataset 
            test_data: test dataset 
            K: cluster num 
        Return: prediction label of testdata
    '''
    knn_predict_label = []
    n_test_data = test_data.shape[0]
    for i in range(n_test_data):
        knn_idx = quadratic_knn_search(train_data, test_data[i], K)
        predict_label = 1 if train_label[knn_idx].sum() > K / 2 else 0
        knn_predict_label.append(predict_label) 
    return knn_predict_label


def test_knn(train_data, train_label, test_data, test_label, K):
    t0 = clock()
    predict_label = knn_predict(train_data, train_label, test_data, K)
    F1, P, R, TP, FP, TN, FN = calc_F1_score(predict_label, test_label)
    t1 = clock()
    
    print(f"======= KNN: K = {K} ====== ")
    print(f"F1 score: {F1}, Precision: {P}, Recall: {R}")
    print(f"TP: {TP}, FP: {FP}, TN: {TN}, FN: {FN}")
    print(f"time used: {t1 - t0}")