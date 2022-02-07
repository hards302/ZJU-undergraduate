import numpy as np

# You may find below useful for Support Vector Machine
# More details in
# https://docs.scipy.org/doc/scipy/reference/generated/scipy.optimize.minimize.html
# from scipy.optimize import minimize

def func(X, y):
    '''
    Classification algorithm.

    Input:  X: Training sample features, P-by-N
            y: Training sample labels, 1-by-N

    Output: w: learned parameters, (P+1)-by-1
    '''
    P, N = X.shape
    w = np.zeros((P+1, 1))

    # YOUR CODE HERE
    # ----------------
    X_t = np.zeros((P+1, N))
    X_t[0] = 1
    X_t[1:] = X
    w = np.dot(np.dot(np.linalg.inv(np.dot(X_t,X_t.transpose())),X_t),y.transpose())
    # ----------------
    return w

def func2(X, y):
    '''
    Classification algorithm with perceptron.
    
    Input:  X: Training sample features, P-by-N
            y: Training sample labels, 1-by-N

    Output: w: learned parameters, (P+1)-by-1
    '''
    P, N = X.shape
    w = np.zeros((P+1, 1))
    # add 1 to X
    X_t = np.zeros((P+1, N))
    X_t[0] = 1
    X_t[1:] = X
    # train
    learning_rate = 0.75
    X_t = X_t.transpose()
    y = y.transpose()
    for i in range(50):
        for i in range(N):
            if np.dot(X_t[i], w) * y[i] <= 0:
                z = learning_rate * X_t[i] * (y[i]-np.dot(X_t[i], w))
                for j in range(P+1):
                    w[j] += z[j]
    # MY CODE HERE
    # ---------------
    return w