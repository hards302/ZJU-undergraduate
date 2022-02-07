import numpy as np
from scipy.spatial.distance import cdist

def kmeans(X, k):
    '''
    K-Means clustering algorithm

    Input:  x: data point features, N-by-P matirx
            k: the number of clusters

    Output:  idx: cluster label, N-by-1 vector
    '''

    N, P = X.shape
    idx = np.zeros(N)
    # YOUR CODE HERE
    # ----------------
    centers = {}
    for i in range(k):
        centers[i] = X[i]
    # update points
    for i in range(1000):
        subsets = {}
        for j in range(k):
            subsets[j] = []
        for point in X:
            dist = []
            for center in centers:
                dist.append(np.linalg.norm(point-centers[center]))
    # update centers
            subsets[dist.index(min(dist))].append(point)
        pre_centers = dict(centers)
        for c in subsets:
            centers[c] = np.average(subsets[c], axis=0)
    # calculate diff
        flag = True
        for c in centers:
            pre_c = pre_centers[c]
            cur_c = centers[c]
            if np.sum(cur_c - pre_c) > 0.01:
                flag = False
        if flag:
            break

    for i in range(N):
        dist = []
        for center in centers:
            dist.append(np.linalg.norm(X[i]-centers[center]))
        idx[i] = dist.index(min(dist))
    # ----------------
    return idx

def spectral(W, k):
    '''
    Spectral clustering algorithm

    Input:  W: Adjacency matrix, N-by-N matrix
            k: number of clusters

    Output:  idx: data point cluster labels, N-by-1 vector
    '''
    N = W.shape[0]
    idx = np.zeros((N, 1))
    # YOUR CODE HERE
    # ----------------
    # get degree matrix
    size_ = len(W)
    D = np.diag(np.zeros(size_))
    for i in range(size_):
        D[i][i] = sum(W[i])
    # get L matrix
    L = D-W
    # get eig matrix
    eigval, eigvec = np.linalg.eig(L)
    dim = len(eigval)
    dictEigval = dict(zip(eigval,range(0,dim)))
    kEig = np.sort(eigval)[0:k]
    ix = [dictEigval[k] for k in kEig]
    X = eigvec[:,ix]
    # ----------------
    X = X.astype(float)  # keep real part, discard imaginary part
    idx = kmeans(X, k)
    return idx

def knn_graph(X, k, threshold):
    '''
    Construct W using KNN graph

    Input:  X:data point features, N-by-P maxtirx.
            k: number of nearest neighbour.
            threshold: distance threshold.

    Output:  W - adjacency matrix, N-by-N matrix.
    '''
    N = X.shape[0]
    W = np.zeros((N, N))
    aj = cdist(X, X, 'euclidean')
    for i in range(N):
        index = np.argsort(aj[i])[:(k+1)]
        W[i, index] = 1
        W[i, i] = 0  # aj[i,i] = 0
    W[aj >= threshold] = 0
    return W