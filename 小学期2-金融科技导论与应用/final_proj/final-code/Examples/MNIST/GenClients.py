import numpy as np
from Data.Datasets import MNIST_Dataset, get_mnist_train


def get_iid_mnist(samples_per_client: int):
    """
    Get identically distributed mnist datasets
    i.e. Each client will have the same distribution of data
    This is done by simply random distribute the samples
    :param samples_per_client:
    :return:
    """
    mnist_train_data = get_mnist_train().data
    client_datasets = []
    for i in range(int(mnist_train_data.shape[0] / samples_per_client)):
        client_datasets.append(MNIST_Dataset(mnist_train_data[i * samples_per_client: (i + 1) * samples_per_client]))
    return client_datasets


def get_non_iid_mnist(classes_per_client: int, samples_per_class: int):
    """
    Get non-identically distributed mnist dataset
    Each client will have data of several digits
    :param classes_per_client:
    :param samples_per_class:
    :return:
    """
    mnist_train_data = get_mnist_train().data
    class_counts = [int(5000 / samples_per_class) for _ in range(10)]
    mnist_classified = [mnist_train_data[mnist_train_data[:, 0] == i] for i in range(10)]
    clients = []
    while sum(class_counts) >= 2:
        i = np.random.randint(10)
        client_classes = []
        while len(client_classes) != 2:
            if class_counts[i] != 0:
                j = class_counts[i]
                client_classes.append(mnist_classified[i][(j - 1) * classes_per_client: j * classes_per_client])
                class_counts[i] -= 1
            i = (i + 1) % 10
        clients.append(MNIST_Dataset(np.vstack(client_classes)))
    return clients
