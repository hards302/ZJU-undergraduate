# MV Portfolio
# Inputs: m, n

# Output:
# The portfolio weight vectors ωk
import numpy as np
import cvxopt as opt
from cvxopt import matrix, solvers
import os
from data_load.stocks import Stocks
from trade.portfolio import Portfolio


def MV_weight_compute(n, context):
    R = context["R"]
    R = R.T
    x, y = R.shape
    P = matrix(np.cov(R))
    q = matrix(np.mean(np.zeros((x,y)), axis=1))
    
    G = -matrix(np.eye(n))
    h = matrix(np.zeros(n))
    A = matrix(np.ones(n)).T
    b = matrix(1.0)
    w = solvers.qp(P, -q, G, h, A, b)['x']
    wt = []
    for i in w:
        wt.append(i)
    return wt

if __name__ == "__main__":
    print("this is MV Portfolio")
