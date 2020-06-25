# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %% [markdown]
# ### KNN.py
# 
import pandas as pd 
import numpy as np 
from time import clock
# %%
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


# %%


def calc_F1_score(predict_label, truth_label):
    '''
        Function: calculate F1 score 
        Param: predict_label, truth_label
        Return: list: [F1, Precision, Recall] 

    '''
    compare = predict_label - truth_label 
    FP = len(compare[compare == 1]) # predict = 1, truth = 0
    FN = len(compare[compare == -1]) # predict = 0, truth = 1
    
    compare = predict_label + truth_label
    TP = len(compare[compare == 2]) # predict = 1, truth = 1
    TN = len(compare[compare == 0]) # predict = 0, truth = 0
    P = TP / (TP + FP) # precision
    R = TP / (TP + FN) # recall
    F1 = 2 * P * R / (P + R)
    return [F1, P, R, TP, FP, TN, FN] 


def main():
    math_data = pd.read_csv('data/student/student-mat.csv', sep=';')

    # select feature 
    feature_name = ['G1','G2','G3']
    data = math_data[feature_name]

    # G3 == 1: pass, G3 == 0: fail
    data.G3[data.G3 < 10] = 0
    data.G3[data.G3 >= 10] = 1
    # other preprocessing here  #TODO

    # train data and test data partition
    train_data = data.sample(frac = 0.7, axis = 0)
    test_data = data[~data.index.isin(train_data.index)]

    train_label = train_data['G3'].to_numpy()
    test_label = test_data['G3'].to_numpy()

    del train_data['G3']
    del test_data['G3']

    train_data = train_data.to_numpy()
    test_data = test_data.to_numpy()

    print(f"train data size: {train_data.shape[0]}, test data size: {test_data.shape[0]}")

    test_knn(train_data, train_label, test_data, test_label, 9)
    
if __name__ == "__main__":
    main()

# %% [markdown]
# ### KNN

# %%


