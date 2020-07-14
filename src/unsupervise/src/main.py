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
    K = 3 
    for thresh in [0.9, 0.8, 0.7, 0.6,0.5]:
        new_data, _, _ = PCA.PCA(data.T, 2, True, thresh)

        ndim = new_data.shape[1]
        print(f"======== kmeans, K = {K}, ndim = {ndim}, thresh = {thresh} =========")

        if ndim == 2:
            plt.figure(1)
            plt.scatter(new_data[:,0], new_data[:,1], s = 50)

        S, RI, predicted_label = Kmeans.test_kmeans(new_data, label, K)
        df_data = pd.DataFrame(new_data)
        df_label = pd.DataFrame(predicted_label)
        result_df = pd.concat([df_label, df_data],axis=1)
        result_df.to_csv(f"./result_ndim{ndim}_K{K}.csv")

if __name__ == "__main__":
    main()