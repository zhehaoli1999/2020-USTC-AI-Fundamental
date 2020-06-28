import pandas as pd 
import numpy as np
import PCA 
import Kmeans 
import matplotlib.pyplot as plt

def main():
    data = pd.read_csv('/Users/bytedance/Desktop/AI/data/wine.data.csv')
    label = data["0"].to_numpy()
    del data["0"]

    data = data / data.max(axis = 0) # normalize
    data = data.to_numpy()

    # PCA 
    new_data, _, _ = PCA.PCA(data.T, 5)

    plt.figure(1)
    plt.scatter(new_data[:,0], new_data[:,1], s = 50)

    K = 3
    Kmeans.test_kmeans(new_data, label, K)

if __name__ == "__main__":
    main()