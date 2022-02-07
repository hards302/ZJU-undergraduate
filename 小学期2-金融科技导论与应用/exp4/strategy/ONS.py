# ONS Portfolio
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


def ONS_weight_compute(n, context):
    r = context["R"]
    r = r.T
    x,y = r.shape
    w = np.ones(n)
    w = w / n
    listw = []
    listw.append(w)
    for i in range(1):
        A = np.gradient(np.gradient(np.log(np.dot(w, r))))[2]
        P = matrix(A)
        q = matrix(np.mean(np.zeros((x,y)), axis=1))
    
        G = -matrix(np.eye(n))
        h = matrix(np.zeros(n))
        A = matrix(np.ones(n)).T
        b = matrix(1.0)
        w = solvers.qp(P, -q, G, h, A, b)['x']
        wt = []
        for i in w:
            wt.append(i)
        listw.append(wt)
    return w

if __name__ == "__main__":
    print("this is ONS Portfolio")
