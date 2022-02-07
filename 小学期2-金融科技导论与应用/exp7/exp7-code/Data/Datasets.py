import numpy as np
import pandas as pd


from Utils import BasicDataset


dataset_root = "Data/RawData/"


class MNIST_Dataset(BasicDataset):
    def __init__(self, data):
        super(MNIST_Dataset, self).__init__(data)

    # Return xs with shape [784] and ys with shape [10]
    def get_batch_xy(self, batch_size: int):
        batch_data = self.get_batch(batch_size)
        ys = batch_data[:, 0].astype(int)
        ys = np.eye(10, dtype=np.float32)[ys]
        xs = batch_data[:, 1:]
        return (xs - 128) / 128, ys


def get_mnist_train():
    return MNIST_Dataset(pd.read_csv(dataset_root + "mnist_train.csv").values.astype(np.float32))


def get_mnist_test():
    return MNIST_Dataset(pd.read_csv(dataset_root + "mnist_test.csv").values.astype(np.float32))
