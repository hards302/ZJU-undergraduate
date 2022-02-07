import numpy as np
import torch
import torch.nn as nn
from FedLearn import FederatedScheme
from Data.Datasets import get_mnist_test
from Examples.MNIST.Model import MNIST_CNN, MNIST_2NN
from GenClients import get_iid_mnist, get_non_iid_mnist

mnist_test = get_mnist_test()

# clients = get_iid_mnist(500)  # Each client get 500 samples
clients = get_non_iid_mnist(2, 250)
model = MNIST_2NN

n_clients = 10
client_batch_size = 50
n_client_batches = 10




fed_scheme = FederatedScheme(clients, lambda: model(), nn.MSELoss())

# Train for 1000 global iterations
for i in range(1000):
    # Calculate the accuracy on the test set of the global model every 100 iterations
    if i % 100 == 0:
        test_xs_np, test_ys_np = mnist_test.get_batch_xy(1000)
        test_xs = torch.from_numpy(test_xs_np).to(fed_scheme.device)
        test_ys = torch.from_numpy(test_ys_np).to(fed_scheme.device)

        pred_ys = fed_scheme.global_model(test_xs).detach().cpu().numpy()
        acc = np.mean(np.argmax(pred_ys, axis=-1) == np.argmax(test_ys_np, axis=-1))
        print(f"Global round {i:6d}, test accuracy {acc:.3f}")

    # The learning rate should not be too large, otherwise the loss will explode
    fed_scheme.fed_avg_one_step(n_clients, client_batch_size, n_client_batches, 0.0005)
