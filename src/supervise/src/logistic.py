import numpy as np
import main

calc_logistic = lambda w,x: np.exp(np.dot(w, x)) / (1 + np.exp(np.dot(w, x))) 
calc_likelyhood = lambda label, sigma: label * np.log(sigma) + (1 - label) * np.log(1-sigma)
    
def logistic_fit(train_data, train_label, learning_rate=0.01, threshold=0.001):  
    '''
    Do gradient descending to train logistic model
    Param
    ---
        train data, train label (0 and 1), learn rate, threshold in gradient descend
    Return
    ---
        [w, likelyhood]
    '''
    ndata = train_data.shape[0]
    ndim = train_data.shape[1]
    w = np.zeros(ndim)
 
    logistic_list = np.array([calc_logistic(w, train_data[i]) for i in range(ndata)])
    
    likelyhood = np.sum([calc_likelyhood(train_label[i], logistic_list[i]) for i in range(ndata)])
    
    while True:
        delta_w = np.sum([train_label[i] - logistic_list[i]] * train_data[i] for i in range(ndata))
        w = w + learning_rate * delta_w
        
        logistic_list = np.array([calc_logistic(w, train_data[i]) for i in range(ndata)])
        new_likelyhood = np.sum([calc_likelyhood(train_label[i], logistic_list[i]) for i in range(ndata)])
        if(np.abs(likelyhood - new_likelyhood) < threshold):
            break

        likelyhood = new_likelyhood

    return [w,likelyhood]

    
def test_logistic(train_data, train_label, test_data, test_label, learning_rate=0.001, threshold=0.01):
    '''
    Test logistic. Rememer to make sure label in 0 or 1
    Return 
    ---
        f1, precision ,recall, predict label
    '''
    print(f"======= Logistic: learning rate = {learning_rate} ======= ")

    train_label[train_label == -1] = 0
    w, likelyhood = logistic_fit(train_data, train_label, learning_rate, threshold)
    
    ndata = test_data.shape[0]
    predict_label = [1 if calc_logistic(w, test_data[i]) >= .5 else -1 for i in range(ndata)]

    f1, p, r = main.calc_F1_score(predict_label, test_label)
    
    return [f1, p, r, predict_label]
