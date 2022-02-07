# EG Portfolio
# Inputs: m, n

# Output:
# The portfolio weight vectors ωk
import numpy as np
import cvxpy as cp
import cvxopt as opt
from cvxopt import matrix, solvers
import os
from data_load.stocks import Stocks
from trade.portfolio import Portfolio


def EG_weight_compute(n, context):
    X = context["R"] # 120 * 36
    X = X.T          # 36*120
    x, y = X.shape
    w = np.ones(n)
    w = w / n
    wt = np.zeros(n)
    d = np.dot(w,X)
    d1 = np.exp(X[0]/d)
    z = np.multiply(w[0],d1)
    for i in range(1,x):
        z += np.multiply(w[i],np.exp(X[i]/np.dot(w,X)))
    for i in range(n):
        wt[i] = (np.multiply(w[i],np.exp(X[i]/np.dot(w,X)))/z)[0]
    return wt

if __name__ == "__main__":
    print("this is EG Portfolio")
