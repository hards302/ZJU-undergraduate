from typing import Callable, Tuple, List
import numpy as np
import torch
import torch.nn as nn
from torch.optim import SGD

from Utils import BasicDataset, copy_model_params


class FederatedScheme:
    def __init__(self, clients: List[BasicDataset], create_model: Callable[[], nn.Module], loss_func: Callable,
                 device: str='cuda:0'):
        self.clients = clients
        self.create_model = create_model
        self.global_model = create_model().to(device)
        self.loss_func = loss_func
        self.device = device

        # The communication rounds
        self.comm_rounds = 0
        self.comm_size = 0

    def add_client_dateset(self, data: np.ndarray):
        current_client_id = len(self.clients)
        self.clients.append(BasicDataset(data))
        return current_client_id

    def train_client(self, client_id: int, batch_size: int, n_batches: int, learning_rate: float):
        # Copy the global model
        local_model = self.create_model().to(self.device)
        copy_model_params(self.global_model, local_model)
        optimizer = SGD(local_model.parameters(), learning_rate)

        # Train the local model
        for i in range(n_batches):
            batch_xs, batch_ys = self.clients[client_id].get_batch_xy(batch_size)
            batch_xs = torch.from_numpy(batch_xs).to(self.device)
            batch_ys = torch.from_numpy(batch_ys).to(self.device)
            optimizer.zero_grad()
            loss = self.loss_func(local_model(batch_xs), batch_ys)
            # print(loss.item())
            loss.backward()
            optimizer.step()

        # Return the local model
        return local_model

    def fed_avg_one_step(self, n_clients: int, local_batch_size: int, n_local_batches: int, local_learning_rate: float):
        client_indices = np.random.choice(len(self.clients), n_clients)
        local_paras = [[] for _ in self.global_model.parameters()]
        for i, client_idx in enumerate(client_indices):
            client_model = self.train_client(client_idx, local_batch_size, n_local_batches, local_learning_rate)
            for j, param in enumerate(client_model.parameters()):
                local_paras[j].append(param.data)

        # Set the global weights to be mean of local weights
        for i, param in enumerate(self.global_model.parameters()):
            param.data = torch.mean(torch.stack(local_paras[i]), dim=0)

        self.comm_rounds += 1
        self.comm_size += n_clients

#-------------------------------------CNN---------------------------------------------------------------------
class FederatedSchemewithCNN:
    def __init__(self, clients: List[BasicDataset], create_model: Callable[[], nn.Module], loss_func: Callable,
                 device: str='cuda:0'):
        self.clients = clients
        self.create_model = create_model
        self.global_model = create_model().to(device)
        self.loss_func = loss_func
        self.device = device

        # The communication rounds
        self.comm_rounds = 0
        self.comm_size = 0

    def add_client_dateset(self, data: np.ndarray):
        current_client_id = len(self.clients)
        self.clients.append(BasicDataset(data))
        return current_client_id

    def train_client(self, client_id: int, batch_size: int, n_batches: int, learning_rate: float):
        # Copy the global model
        local_model = self.create_model().to(self.device)
        copy_model_params(self.global_model, local_model)
        optimizer = SGD(local_model.parameters(), learning_rate)

        # Train the local model
        for i in range(n_batches):
            batch_xs, batch_ys = self.clients[client_id].get_batch_xy(batch_size)
            batch_xs = torch.from_numpy(batch_xs).to(self.device)
            #------------CNN--------------------
            # iid
            batch_xs = batch_xs.view(batch_size, 1, 28, 28)
            # non-iid
            # batch_xs = batch_xs.view(4, 1, 28, 28)
            #----------------------------------- 
            batch_ys = torch.from_numpy(batch_ys).to(self.device)
            optimizer.zero_grad()
            loss = self.loss_func(local_model(batch_xs), batch_ys)
            # print(loss.item())
            loss.backward()
            optimizer.step()

        # Return the local model
        return local_model

    def fed_avg_one_step(self, n_clients: int, local_batch_size: int, n_local_batches: int, local_learning_rate: float):
        client_indices = np.random.choice(len(self.clients), n_clients)
        local_paras = [[] for _ in self.global_model.parameters()]
        for i, client_idx in enumerate(client_indices):
            client_model = self.train_client(client_idx, local_batch_size, n_local_batches, local_learning_rate)
            for j, param in enumerate(client_model.parameters()):
                local_paras[j].append(param.data)

        # Set the global weights to be mean of local weights
        for i, param in enumerate(self.global_model.parameters()):
            param.data = torch.mean(torch.stack(local_paras[i]), dim=0)

        self.comm_rounds += 1
        self.comm_size += n_clients
        
#---------------------------------------RNN--------------------------------------------------------------------
class FederatedSchemewithRNN:
    def __init__(self, clients: List[BasicDataset], create_model: Callable[[], nn.Module], loss_func: Callable,
                 device: str='cuda:0'):
        self.clients = clients
        self.create_model = create_model
        self.global_model = create_model().to(device)
        self.loss_func = loss_func
        self.device = device

        # The communication rounds
        self.comm_rounds = 0
        self.comm_size = 0

    def add_client_dateset(self, data: np.ndarray):
        current_client_id = len(self.clients)
        self.clients.append(BasicDataset(data))
        return current_client_id

    def train_client(self, client_id: int, batch_size: int, n_batches: int, learning_rate: float):
        # Copy the global model
        local_model = self.create_model().to(self.device)
        copy_model_params(self.global_model, local_model)
        optimizer = SGD(local_model.parameters(), learning_rate)

        # Train the local model
        for i in range(n_batches):
            batch_xs, batch_ys = self.clients[client_id].get_batch_xy(batch_size)
            batch_xs = torch.from_numpy(batch_xs).to(self.device)
            #------------CNN--------------------
            # iid
            batch_xs = batch_xs.view(batch_size, 28, 28)
            # non-iid
            # batch_xs = batch_xs.view(4, 28, 28)
            #----------------------------------- 
            batch_ys = torch.from_numpy(batch_ys).to(self.device)
            optimizer.zero_grad()
            loss = self.loss_func(local_model(batch_xs), batch_ys)
            # print(loss.item())
            loss.backward()
            optimizer.step()

        # Return the local model
        return local_model

    def fed_avg_one_step(self, n_clients: int, local_batch_size: int, n_local_batches: int, local_learning_rate: float):
        client_indices = np.random.choice(len(self.clients), n_clients)
        local_paras = [[] for _ in self.global_model.parameters()]
        for i, client_idx in enumerate(client_indices):
            client_model = self.train_client(client_idx, local_batch_size, n_local_batches, local_learning_rate)
            for j, param in enumerate(client_model.parameters()):
                local_paras[j].append(param.data)

        # Set the global weights to be mean of local weights
        for i, param in enumerate(self.global_model.parameters()):
            param.data = torch.mean(torch.stack(local_paras[i]), dim=0)

        self.comm_rounds += 1
        self.comm_size += n_clients