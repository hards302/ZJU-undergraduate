# Best Portfolio
# Inputs: m, n,  τ（span_t）

# Output:
# The portfolio weight vectors ωk
import numpy as np
import cvxpy as cp
import os
from data_load.stocks import Stocks
from trade.portfolio import Portfolio


span_t = 120


def BCRP_compute(n, all_price_relative):
    price_relative = all_price_relative
    
    p = cp.Variable(n)

    objective = cp.Maximize(cp.sum(cp.log(p * price_relative.T)))

    constraints = [p >= 0, cp.sum(p) == 1]

    prob = cp.Problem(objective, constraints)
    prob.solve()
    w = np.array(p.value)
    return w

def BCRP_weight_compute(n, context):
    w = context["BCRP"]
    return w


if __name__ == "__main__":
    print("this is BCRP Portfolio")
