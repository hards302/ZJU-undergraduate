import torch
import torchvision
import torch.nn as nn
import torch.optim as optim
from torchvision import transforms, datasets, utils
from model.LeNet5 import *
# import ssl

if torch.cuda.is_available():
    DEVICE = "cuda"
else:
    DEVICE = "cpu"

class mnist():
    def train():
        print("MNIST Train begin...")
        # get the dataset
        print("get the dataset...")
        transform = torchvision.transforms.ToTensor()
        train_data = torchvision.datasets.MNIST(root="./dataset/mnist",
                                                train=True,
                                                transform=transform,
                                                download=False)         # if you don't get mnist dataset, set download to True
        train_loader = torch.utils.data.DataLoader(dataset = train_data,batch_size = 64,shuffle = True)
        print("get the dataset finished.")
        # set the network and loss function
        net = LeNet5(1).to(DEVICE)
        loss_func = nn.CrossEntropyLoss()
        optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

        # train begin
        print("begin training...")
        epochs = 10
        for epoch in range(1, epochs+1):
            sum_loss = 0
            for _, datas in enumerate(train_loader):
                data, label = datas
                data = data.to(DEVICE)
                label = label.to(DEVICE)

                optimizer.zero_grad()

                output = net(data)
                loss = loss_func(output, label)
                loss.backward()
                optimizer.step()
                sum_loss += loss.item()

            print("[Epoch]:%d train loss: %.03f" % (epoch, sum_loss/1000))
        torch.save(net.state_dict(),'./model/mnist.mdl')
        print("finsh training.")

    def test():
        print("MNIST Test begin...")
        # get the dataset
        print("get the dataset...")
        transform = torchvision.transforms.ToTensor()
        test_data = torchvision.datasets.MNIST(root="./dataset/mnist",
                                                train=False,
                                                transform=transform,
                                                download=False)
        test_loader = torch.utils.data.DataLoader(dataset = test_data,batch_size = 64,shuffle = False)
        print("get dataset finished.")
        # get the model
        net = LeNet5(1).to(DEVICE)
        net.load_state_dict(torch.load('./model/mnist.mdl'))
        # begin test
        print("begin testing...")
        correct = 0
        total = 0

        for datas in test_loader:
            data, label = datas
            data = data.to(DEVICE)
            label = label.to(DEVICE)
            output = net(data)
            _, predict = torch.max(output, 1)
            total += label.size(0)
            correct += (predict == label).sum()
        print(f"the correct rate is {correct.item()/total}")
        print("finish testing.")

class cifar():
    def train():
        print("CIFAR-10 Train begin...")
        # get the dataset
        print("get the dataset...")
        transform = torchvision.transforms.ToTensor()
        # ssl._create_default_https_context = ssl._create_unverified_context  # if you want to download the cifar-10, add this
        train_data = torchvision.datasets.CIFAR10(root="./dataset/cifar10",
                                                train=True,
                                                transform=transform,
                                                download=False)         # if you don't get cifar10 dataset, set download to True
        train_loader = torch.utils.data.DataLoader(dataset = train_data,batch_size = 64,shuffle = True)
        print("get the dataset finished.")
        # set the network and loss function
        net = LeNet5(3).to(DEVICE)
        loss_func = nn.CrossEntropyLoss()
        optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)

        # train begin
        print("begin training...")
        epochs = 100
        for epoch in range(1, epochs+1):
            sum_loss = 0
            for _, datas in enumerate(train_loader):
                data, label = datas
                data = data.to(DEVICE)
                label = label.to(DEVICE)

                optimizer.zero_grad()

                output = net(data)
                loss = loss_func(output, label)
                loss.backward()
                optimizer.step()
                sum_loss += loss.item()

            print("[Epoch]:%d train loss: %.03f" % (epoch, sum_loss/1000))
        torch.save(net.state_dict(),'./model/cifar.mdl')
        print("finsh training.")
    def test():
        print("CIFAR10 Test begin...")
        # get the dataset
        print("get the dataset...")
        transform = torchvision.transforms.ToTensor()
        test_data = torchvision.datasets.CIFAR10(root="./dataset/cifar10",
                                                train=False,
                                                transform=transform,
                                                download=False)
        test_loader = torch.utils.data.DataLoader(dataset = test_data,batch_size = 64,shuffle = False)
        print("get dataset finished.")
        # get the model
        net = LeNet5(3).to(DEVICE)
        net.load_state_dict(torch.load('./model/cifar.mdl'))
        # begin test
        print("begin testing...")
        correct = 0
        total = 0

        for datas in test_loader:
            data, label = datas
            data = data.to(DEVICE)
            label = label.to(DEVICE)
            output = net(data)
            _, predict = torch.max(output, 1)
            total += label.size(0)
            correct += (predict == label).sum()
        print(f"the correct rate is {correct.item()/total}")
        print("finish testing.")

if __name__ == "__main__":
    print("Which task do you want to do? input 1 for mnist, 2 for CIFAR-10")
    task = int(input())
    if task == 1:
        print("train or test? 1 for train, 2 for test")
        choice = int(input())
        if choice == 1:
            mnist.train()
        elif choice == 2:
            mnist.test()
    elif task == 2:
        print("train or test? 1 for train, 2 for test")
        choice = int(input())
        if choice == 1:
            cifar.train()
        elif choice == 2:
            cifar.test()