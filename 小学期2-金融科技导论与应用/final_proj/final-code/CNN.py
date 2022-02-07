import numpy as np
import torch
import torch.nn as nn
from FedLearn import FederatedSchemewithCNN
from Data.Datasets import get_mnist_test
from Examples.MNIST.Model import MNIST_CNN
from Examples.MNIST.GenClients import get_iid_mnist, get_non_iid_mnist
import matplotlib.pyplot as plt

mnist_test = get_mnist_test()

clients = get_iid_mnist(100)  # Each client get 100 samples
# clients = get_non_iid_mnist(2, 50)     # use it to change to data to non-iid. and remember change "FederatedSchemewithCNN" in FedLearn too.
model = MNIST_CNN

#--------------do research on C,B,E just need to change it and redo code below----------
n_clients = 10 # C=0.1
client_batch_size = 10 # B
n_client_batches = 1 # E

fed_scheme = FederatedSchemewithCNN(clients, lambda: model(), nn.MSELoss())

x = []
y = []
for i in range(2001):
    # Calculate the accuracy on the test set of the global model every 100 iterations
    if i % 20 == 0:
        test_xs_np, test_ys_np = mnist_test.get_batch_xy(1000)
        test_xs = torch.from_numpy(test_xs_np).to(fed_scheme.device)
        test_ys = torch.from_numpy(test_ys_np).to(fed_scheme.device)
        #----------CNN----------------------
        test_xs = test_xs.view(1000,1,28,28)
        #-----------------------------------
        pred_ys = fed_scheme.global_model(test_xs).detach().cpu().numpy()
        acc = np.mean(np.argmax(pred_ys, axis=-1) == np.argmax(test_ys_np, axis=-1))
        print(f"Global round {i:6d}, test accuracy {acc:.3f}")
        x.append(i)
        y.append(acc)

    # The learning rate should not be too large, otherwise the loss will explode
    fed_scheme.fed_avg_one_step(n_clients, client_batch_size, n_client_batches, 0.1)

plt.plot(x,y)
plt.show()