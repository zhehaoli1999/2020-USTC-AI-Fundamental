import numpy as np
import matplotlib.pyplot as plt
import main

def label_data(x, centers):
    '''
    Label sample x according to center set.
    Return 
    ---
        label in range (0, total cluster num)
    '''
    ndim = x.shape[0]
    return np.argmin([np.dot(v,v) for v in (centers - x.reshape(1, ndim))]) # return idx in [0, K-1]

def kmeans_fit(train_data, K=3):
    '''
        Do kmeans clustering on train data with cluster num K
        Param
        ---
        train data: (ndata, ndim) \n
        K: default 3
        Return
        ---
        label, centers 
    '''
    # random pick K initial centroids
    ndata = train_data.shape[0]    
    idx = np.random.permutation(np.arange(ndata))[:K]
    centers = train_data[idx]

    while True:
        # label each data
        label = np.array([label_data(x, centers) for x in train_data])

        # select new_centers
        new_centers = np.array([ np.mean(train_data[label == i], axis=0) for i in range(K)])  

        if np.all(new_centers == centers):
            break

        # update centers 
        centers = new_centers

    return [label, centers] 

def calc_RI(predict_label, truth_label):
    '''
    Calculate Rand Coefficient 
    Return
    ---
        RI
    '''
    ndata = len(predict_label)
    m = ndata * (ndata - 1) / 2  # m = a + b + c + d
    a = 0
    d = 0
    for i in range(ndata):
        predict_label_i = predict_label[i]
        truth_label_i = truth_label[i]
        for j in range(i+1, ndata):
            predict_label_j = predict_label[j]
            truth_label_j = truth_label[j]
            if truth_label_i == truth_label_j and predict_label_i == predict_label_j:
                a += 1 
            if truth_label_i != truth_label_j and predict_label_i != predict_label_j:
                d += 1
    RI = (a + d) / m
    return RI

def calc_silhouette(dataset, labelset, K):
    '''
        Calculat silhouette coefficient 
        Return
        ---
            Average S
    '''
    ndata = dataset.shape[0]
    ndim = dataset.shape[1]

    dist_sum = lambda x, dataset: np.array([np.linalg.norm(v, 2) for v in (dataset - x.reshape(1, ndim))]).sum()

    S_array = []
    for n in range(ndata):
        test_data = dataset[n]
        test_label = labelset[n]

        # intra class mean distance 
        intra_dataset = dataset[labelset == test_label]
        n_intradata = intra_dataset.shape[0]
        a = dist_sum(test_data, intra_dataset) / (n_intradata - 1)

        # minimum inter class distance
        inter_dataset = np.array([dataset[labelset == (test_label + i) % K] for i in range(1, K)])
        b = np.min([ dist_sum(test_data, inter_dataset[i]) / inter_dataset[i].shape[0] for i in range(K-1)])

        S = (b - a) / max([a,b])
        S_array.append(S)

    return np.array(S_array).mean()

def test_kmeans(data, label, K=3):
    '''
        test kmeans. Should be called in main.py. 
        Return
        ---
            S, RI, predicted label 
    '''

    predict_label, centers = kmeans_fit(data, K)
    
    S = calc_silhouette(data, predict_label, K)
    RI = calc_RI(predict_label, label )

    print( f"Silhouette Coefficient= {S}" )
    print( f"Rand Coefficient= {RI}")

    ndim = data.shape[1]
    if(ndim == 2):
        plt.figure(2)
        plt.scatter(data[:,0], data[:,1], c=predict_label, s = 50)
        plt.scatter(centers[:, 0], centers[:, 1], c='black', s=100, alpha=0.5)
        plt.show()
    return [S, RI, predict_label]


   