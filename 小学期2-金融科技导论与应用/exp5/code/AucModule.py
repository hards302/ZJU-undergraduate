def calcAUC(labels, probs):
    N = 0
    P = 0
    neg_prob = []
    pos_prob = []
    for _,i in enumerate(labels):
        if (i == 1):
            P += 1
            pos_prob.append(probs[_])
        else:
            N += 1
            neg_prob.append(probs[_])
    number = 0
    for pos in pos_prob:
        for neg in neg_prob:
            if (pos > neg):
                number += 1
            elif (pos == neg):
                number += 0.5
    return number / (N * P)