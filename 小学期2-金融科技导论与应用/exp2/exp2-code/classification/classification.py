import numpy as np
from gen_data import gen_data
from plot import plot
from todo import func

no_iter = 1000  # number of iteration
no_train = 80 # YOUR CODE HERE # number of training data
no_test = 20 # YOUR CODE HERE  # number of testing data
no_data = 100  # number of all data
assert(no_train + no_test == no_data)

cumulative_train_err = 0
cumulative_test_err = 0

for i in range(no_iter):
    X, y, w_gt = gen_data(no_data)
    X_train, X_test = X[:, :no_train], X[:, no_train:]
    y_train, y_test = y[:, :no_train], y[:, no_train:]
    w_l = func(X_train, y_train)
    # Compute training, testing error
    # YOUR CODE HERE
    # ----------------
    train_err = 0
    test_err = 0
    P, N = X_train.shape
    X_t = np.zeros((P+1, N))
    X_t[0] = 1
    X_t[1:] = X_train
    trainy = np.dot(w_l.transpose(),X_t)
    P, N = X_test.shape
    X_t = np.zeros((P+1, N))
    X_t[0] = 1
    X_t[1:] = X_test
    testy = np.dot(w_l.transpose(),X_t)
    for i in range(no_train):
        if trainy[0][i] * y_train[0][i] < 0:
            train_err += 1
    for i in range(no_test):
        if testy[0][i] * y_test[0][i] < 0:
            test_err += 1
    # ----------------
    cumulative_train_err += train_err
    cumulative_test_err += test_err

plot(X, y, w_gt, w_l, "Classification")
print("Training error: %s" % train_err)
print("Testing error: %s" % test_err)