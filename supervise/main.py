import pandas as pd 
import numpy as np
from sklearn import preprocessing
from KNN import *
from SVM import *
from logistic import *

def calc_F1_score(predict_label, truth_label):
    '''
        Function: calculate F1 score 
        Param
        ---
            predict label, truth label
        Return
        ---
            list: F1, Precision, Recall 
    '''
    compare = predict_label - truth_label 
    FP = len(compare[compare == 2]) # predict = 1, truth = -1
    FN = len(compare[compare == -2]) # predict = -1, truth = 1
    
    compare = predict_label + truth_label
    TP = len(compare[compare == 2]) # predict = 1, truth = 1
    TN = len(compare[compare == -2]) # predict = -1, truth = -1

    # print(f"TP: {TP}, FP: {FP}, TN: {TN}, FN: {FN}")

    try:
        P = TP / (TP + FP) # precision
        R = TP / (TP + FN) # recall
        F1 = 2 * P * R / (P + R)
        # print(f"F1 score: {F1}, Precision: {P}, Recall: {R}")
        return [F1, P, R]
    except ZeroDivisionError:
        print("divide zero!")


def main():
    '''
    + Read data
    + select feature
    + data normalzation and label encoding
    + partition train data and test data (7:3)
    '''
    fname = ['data/student/student-mat.csv', 'data/student/student-por.csv']
    for name in fname:
        data = pd.read_csv(name, sep=';')

        # select feature 
        feature_name1 = ['G1','G2','G3']
        # feature_name2 = ['Medu','Fedu','failures', 'age','goout','G3']
        # data = data[feature_name1]
        data = data[feature_name1]
        # data = data.drop(['G1', 'G2'], axis = 1)
        # G3 == 1: pass, G3 == 0: fail
        data.G3[data.G3 < 10] = -1 # 0 or -1  
        data.G3[data.G3 >= 10] = 1
        # other preprocessing here  #TODO

        # train data and test data partition
        train_data = data.sample(frac = 0.7, axis = 0)
        test_data = data[~data.index.isin(train_data.index)]

        train_label = train_data['G3'].to_numpy()
        test_label = test_data['G3'].to_numpy()

        del train_data['G3']
        del test_data['G3']

        # label encode
        train_data = train_data.apply(preprocessing.LabelEncoder().fit_transform)
        test_data = test_data.apply(preprocessing.LabelEncoder().fit_transform)
        
        # normalize data 
        train_data = train_data / train_data.max(axis = 0)
        test_data = test_data / test_data.max(axis = 0)

        train_data = train_data.to_numpy()
        test_data = test_data.to_numpy()

        print(f"============= data file: {name}, feature: {data.columns} =============")
        print(f"train data size: {train_data.shape[0]}, test data size: {test_data.shape[0]}")

        F1_list = []
        p_list = []
        r_list = []
        K = 9
        for i in range(10):
            # f1, p, r, _ = test_knn(train_data, train_label, test_data, test_label, K)
            # f1, p, r, _ = test_SVM(train_data, train_label, test_data, test_label, "poly")
            f1, p, r, _ = test_logistic(train_data, train_label, test_data, test_label)

            F1_list.append(f1)
            p_list.append(p)
            r_list.append(r)
        
        print(f"Average F1 score: {np.mean(F1_list)}, Precision: {np.mean(p_list)}, Recall: {np.mean(r_list)}\n")

    
if __name__ == "__main__":
    main()