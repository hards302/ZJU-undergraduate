import torch
import torch.nn as nn
import numpy as np
from torch.nn import Parameter


"""
Our implementation of STBP on SNN is inspired by 
the open-sourced implementation of STBP for:

Wu, Yujie, Lei Deng, Guoqi Li, Jun Zhu, and Luping Shi. 
"Spatio-temporal backpropagation for training high-performance spiking neural networks." 
Frontiers in neuroscience 12 (2018).

Their implementation for SCNN can be found here:
https://github.com/yjwu17/BP-for-SpikingNN

We would like to thank them for open-source their implementation.
"""


NEURON_VTH = 0.5
NEURON_CDECAY = 1 / 2
NEURON_VDECAY = 3 / 4
SPIKE_PSEUDO_GRAD_WINDOW = 0.5


class PseudoSpikeRect(torch.autograd.Function):
    @staticmethod
    def forward(ctx, input):
        ctx.save_for_backward(input)
        return input.gt(NEURON_VTH).float()
    @staticmethod
    def backward(ctx, grad_output):
        input, = ctx.saved_tensors
        grad_input = grad_output.clone()
        spike_pseudo_grad = (abs(input - NEURON_VTH) < SPIKE_PSEUDO_GRAD_WINDOW)
        return grad_input * spike_pseudo_grad.float()


class ActorNetSpiking(nn.Module):
    """ Spiking Actor Network """
    def __init__(self, state_num, action_num, device, batch_window=50, hidden1=256, hidden2=256, hidden3=256):
        """

        :param state_num: number of states
        :param action_num: number of actions
        :param device: device used
        :param batch_window: window steps
        :param hidden1: hidden layer 1 dimension
        :param hidden2: hidden layer 2 dimension
        :param hidden3: hidden layer 3 dimension
        """
        super(ActorNetSpiking, self).__init__()
        self.state_num = state_num
        self.action_num = action_num
        self.device = device
        self.batch_window = batch_window
        self.hidden1 = hidden1
        self.hidden2 = hidden2
        self.hidden3 = hidden3
        self.pseudo_spike = PseudoSpikeRect.apply
        self.fc1 = nn.Linear(self.state_num, self.hidden1, bias=True)
        self.fc2 = nn.Linear(self.hidden1, self.hidden2, bias=True)
        self.fc3 = nn.Linear(self.hidden2, self.hidden3, bias=True)
        self.fc4 = nn.Linear(self.hidden3, self.action_num, bias=True)

    def neuron_model(self, syn_func, pre_layer_output, current, volt, spike):
        """
        Neuron Model
        :param syn_func: synaptic function
        :param pre_layer_output: output from pre-synaptic layer
        :param current: current of last step
        :param volt: voltage of last step
        :param spike: spike of last step
        :return: current, volt, spike
        """
        current = current * NEURON_CDECAY + syn_func(pre_layer_output)
        volt = volt * NEURON_VDECAY * (1. - spike) + current
        spike = self.pseudo_spike(volt)
        return current, volt, spike

    def forward(self, x, batch_size):
        """

        :param x: state batch
        :param batch_size: size of batch
        :return: out
        """
        fc1_u = torch.zeros(batch_size, self.hidden1, device=self.device)
        fc1_v = torch.zeros(batch_size, self.hidden1, device=self.device)
        fc1_s = torch.zeros(batch_size, self.hidden1, device=self.device)
        fc2_u = torch.zeros(batch_size, self.hidden2, device=self.device)
        fc2_v = torch.zeros(batch_size, self.hidden2, device=self.device)
        fc2_s = torch.zeros(batch_size, self.hidden2, device=self.device)
        fc3_u = torch.zeros(batch_size, self.hidden3, device=self.device)
        fc3_v = torch.zeros(batch_size, self.hidden3, device=self.device)
        fc3_s = torch.zeros(batch_size, self.hidden3, device=self.device)
        fc4_u = torch.zeros(batch_size, self.action_num, device=self.device)
        fc4_v = torch.zeros(batch_size, self.action_num, device=self.device)
        fc4_s = torch.zeros(batch_size, self.action_num, device=self.device)
        fc4_sumspike = torch.zeros(batch_size, self.action_num, device=self.device)
        for step in range(self.batch_window):
            input_spike = x[:, :, step]
            fc1_u, fc1_v, fc1_s = self.neuron_model(self.fc1, input_spike, fc1_u, fc1_v, fc1_s)
            fc2_u, fc2_v, fc2_s = self.neuron_model(self.fc2, fc1_s, fc2_u, fc2_v, fc2_s)
            fc3_u, fc3_v, fc3_s = self.neuron_model(self.fc3, fc2_s, fc3_u, fc3_v, fc3_s)
            fc4_u, fc4_v, fc4_s = self.neuron_model(self.fc4, fc3_s, fc4_u, fc4_v, fc4_s)
            fc4_sumspike += fc4_s
            exit(0)
        out = fc4_sumspike / self.batch_window
        return out

class ActorNetSpikingwithNCP(nn.Module):
    """ Spiking Actor Network with NCP"""
    def __init__(self, state_num, action_num, device, batch_window=50, inter_num=256, command_num=256):
        """

        :param state_num: number of states
        :param action_num: number of actions
        :param device: device used
        :param batch_window: window steps
        :param hidden1: hidden layer 1 dimension
        :param hidden2: hidden layer 2 dimension
        :param hidden3: hidden layer 3 dimension
        """
        super(ActorNetSpikingwithNCP, self).__init__()
        self.state_num = state_num
        self.action_num = action_num
        self.device = device
        self.batch_window = batch_window
        self.hidden1 = inter_num
        self.hidden2 = command_num
        self.pseudo_spike = PseudoSpikeRect.apply

        self.w1, self.b1 = self.initncpw(self.state_num, self.hidden1, 192, 192, False, False)
        self.w2, self.b2 = self.initncpw(self.hidden1, self.hidden2, 192, 192, False, False)
        self.w3, self.b3 = self.initncpw(self.hidden2, self.hidden2, 256, 256, False, True)
        self.w4, self.b4 = self.initncpw(self.hidden2, self.action_num, 192, 192, True, False)

    def initncpw(self, source_n, target_n, source_fanout, target_fanin, ismotor, isrecurrent):
        b = torch.zeros(target_n)
        b = Parameter(b, requires_grad=True)
        w = torch.Tensor(source_n, target_n)
        nn.init.normal_(w)
        w = Parameter(w, requires_grad=False)
        ncp_w = torch.zeros(source_n, target_n)
        ncp_w = Parameter(ncp_w, requires_grad=False)
        rng = np.random.RandomState(22222)

        if ismotor == False:
            unreachable_neurons = [l for l in range(target_n)]
            for src in range(source_n):
                for dest in rng.choice(
                    target_n, size=source_fanout, replace=False
                ):
                    if dest in unreachable_neurons:
                        unreachable_neurons.remove(dest)
                    ncp_w[src][dest] = 1

            if isrecurrent == False:
                # If it happens that some target neurons are not connected, connect them now
                mean_neuron_fanin = int(
                    source_n * source_fanout / target_n
                )
                # Connect "forgetten" inter neuron by at least 1 and at most all sensory neuron
                mean_neuron_fanin = np.clip(
                    mean_neuron_fanin, 1, source_n
                )
                for dest in unreachable_neurons:
                    for src in rng.choice(
                        source_n, size=mean_neuron_fanin, replace=False
                    ):
                        ncp_w[src][dest] = 1
        else:
            unreachable_neurons = [l for l in range(source_n)]
            for dest in range(target_n):
                for src in rng.choice(
                    source_n, size=target_fanin, replace=False
                ):
                    if src in unreachable_neurons:
                        unreachable_neurons.remove(src)
                    ncp_w[src][dest] = 1

            # If it happens that some commandneurons are not connected, connect them now
            mean_neuron_fanout = int(
                target_n * target_fanin / source_n
            )
            # Connect "forgotten" command neuron to at least 1 and at most all motor neuron
            mean_neuron_fanout = np.clip(mean_neuron_fanout, 1, target_n)
            for src in unreachable_neurons:
                for dest in rng.choice(
                    target_n, size=mean_neuron_fanout, replace=False
                ):
                    ncp_w[src][dest] = 1
        w = w * ncp_w
        w = Parameter(w, requires_grad=True)
        return w, b

    def neuron_model(self, Layernum, pre_layer_output, current, volt, spike):
        """
        Neuron Model
        :param syn_func: synaptic function
        :param pre_layer_output: output from pre-synaptic layer
        :param current: current of last step
        :param volt: voltage of last step
        :param spike: spike of last step
        :return: current, volt, spike
        """
        # print("pre_layer_output shape = ", pre_layer_output.shape)
        # print("w1.t shape = ", self.w1.t().shape)
        # print("b1 shape = ", self.b1.shape)
        if Layernum == 1:
            pre_layer_output = nn.functional.linear(pre_layer_output, weight=self.w1.t(), bias=self.b1)
        elif Layernum == 2:
            pre_layer_output = nn.functional.linear(pre_layer_output, weight=self.w2.t(), bias=self.b2)
        elif Layernum == 3:
            pre_layer_output = nn.functional.linear(pre_layer_output, weight=self.w3.t(), bias=self.b3)
        else:
            pre_layer_output = nn.functional.linear(pre_layer_output, weight=self.w4.t(), bias=self.b4)
        
        current = current * NEURON_CDECAY + pre_layer_output
        volt = volt * NEURON_VDECAY * (1. - spike) + current
        spike = self.pseudo_spike(volt)
        return current, volt, spike

    def forward(self, x, batch_size):
        """

        :param x: state batch
        :param batch_size: size of batch
        :return: out
        """
        layer1_u = torch.zeros(batch_size, self.hidden1, device=self.device)
        layer1_v = torch.zeros(batch_size, self.hidden1, device=self.device)
        layer1_s = torch.zeros(batch_size, self.hidden1, device=self.device)
        layer2_u = torch.zeros(batch_size, self.hidden2, device=self.device)
        layer2_v = torch.zeros(batch_size, self.hidden2, device=self.device)
        layer2_s = torch.zeros(batch_size, self.hidden2, device=self.device)
        # layer3_u = torch.zeros(batch_size, self.hidden3, device=self.device)
        # layer3_v = torch.zeros(batch_size, self.hidden3, device=self.device)
        # layer3_s = torch.zeros(batch_size, self.hidden3, device=self.device)
        layer4_u = torch.zeros(batch_size, self.action_num, device=self.device)
        layer4_v = torch.zeros(batch_size, self.action_num, device=self.device)
        layer4_s = torch.zeros(batch_size, self.action_num, device=self.device)
        layer4_sumspike = torch.zeros(batch_size, self.action_num, device=self.device)
        for step in range(self.batch_window):
            input_spike = x[:, :, step]
            layer1_u, layer1_v, layer1_s = self.neuron_model(1, input_spike, layer1_u, layer1_v, layer1_s)
            layer2_u, layer2_v, layer2_s = self.neuron_model(2, layer1_s, layer2_u, layer2_v, layer2_s)
            layer2_u, layer2_v, layer2_s = self.neuron_model(3, layer2_s, layer2_u, layer2_v, layer2_s)
            layer4_u, layer4_v, layer4_s = self.neuron_model(4, layer2_s, layer4_u, layer4_v, layer4_s)
            layer4_sumspike += layer4_s
        out = layer4_sumspike / self.batch_window
        return out

class ActorNetSpikingwithRSTDP(nn.Module):
    """ Spiking Actor Network with NCP"""
    def __init__(self, state_num, action_num, device, batch_window=50, hidden1=256, hidden2=256, hidden3=256, nu=0.05, dt=1):
        """

        :param state_num: number of states
        :param action_num: number of actions
        :param device: device used
        :param batch_window: window steps
        :param hidden1: hidden layer 1 dimension
        :param hidden2: hidden layer 2 dimension
        :param hidden3: hidden layer 3 dimension
        """
        super(ActorNetSpikingwithRSTDP, self).__init__()
        self.state_num = state_num
        self.action_num = action_num
        self.device = device
        self.batch_window = batch_window
        self.hidden1 = hidden1
        self.hidden2 = hidden2
        self.hidden3 = hidden3
        self.pseudo_spike = PseudoSpikeRect.apply

        self.w1, self.b1 = self.initncpw(self.state_num, self.hidden1, 256, 256, False, False)
        self.w2, self.b2 = self.initncpw(self.hidden1, self.hidden2, 256, 256, False, False)
        self.w3, self.b3 = self.initncpw(self.hidden2, self.hidden3, 256, 256, False, False)
        self.w4, self.b4 = self.initncpw(self.hidden3, self.action_num, 256, 256, True, False)
        # w1 = torch.Tensor(self.state_num,self.hidden1)
        # w2 = torch.Tensor(self.hidden1,self.hidden2)
        # w3 = torch.Tensor(self.hidden2,self.hidden3)
        # w4 = torch.Tensor(self.hidden3,self.action_num)
        # self.w1 = Parameter(w1, requires_grad=False)
        # self.w2 = Parameter(w2, requires_grad=False)
        # self.w3 = Parameter(w3, requires_grad=False)
        # self.w4 = Parameter(w4, requires_grad=False)
        # b1 = torch.zeros(self.hidden1)
        # b2 = torch.zeros(self.hidden2)
        # b3 = torch.zeros(self.hidden3)
        # b4 = torch.zeros(self.action_num)
        # self.b1 = Parameter(b1, requires_grad=True)
        # self.b2 = Parameter(b2, requires_grad=True)
        # self.b3 = Parameter(b3, requires_grad=True)
        # self.b3 = Parameter(b4, requires_grad=True)
        # print("hidden3 = ",hidden3)
        # print("b3.shape = ", self.b3.shape)
        self.input_spike = torch.zeros(self.state_num, device=self.device)
        self.fc1_spike = torch.zeros(self.hidden1, device=self.device)
        self.fc2_spike = torch.zeros(self.hidden2, device=self.device)
        self.fc3_spike = torch.zeros(self.hidden3, device=self.device)
        self.fc4_spike = torch.zeros(self.action_num, device=self.device)

        self.nu = nu
        self.dt = dt
        self.tc_plus = torch.tensor(20.0)
        self.tc_minus = torch.tensor(20.0)
        self.tc_e_trace = torch.tensor(150.0)
        self.p_plus1 = torch.zeros((self.state_num), device=self.device)
        self.p_plus2 = torch.zeros((self.hidden1), device=self.device)
        self.p_plus3 = torch.zeros((self.hidden2), device=self.device)
        self.p_plus4 = torch.zeros((self.hidden3), device=self.device)
        self.p_minus1 = torch.zeros((self.hidden1), device=self.device)
        self.p_minus2 = torch.zeros((self.hidden2), device=self.device)
        self.p_minus3 = torch.zeros((self.hidden3), device=self.device)
        self.p_minus4 = torch.zeros((self.action_num), device=self.device)
        self.eligibility1 = torch.zeros((self.state_num, self.hidden1), device=self.device)
        self.eligibility2 = torch.zeros((self.hidden1, self.hidden2), device=self.device)
        self.eligibility3 = torch.zeros((self.hidden2, self.hidden3), device=self.device)
        self.eligibility4 = torch.zeros((self.hidden3, self.action_num), device=self.device)
        self.eligibility_trace1 = torch.zeros((self.state_num, self.hidden1), device=self.device)
        self.eligibility_trace2 = torch.zeros((self.hidden1, self.hidden2), device=self.device)
        self.eligibility_trace3 = torch.zeros((self.hidden2, self.hidden3), device=self.device)
        self.eligibility_trace4 = torch.zeros((self.hidden3, self.action_num), device=self.device)

    def initncpw(self, source_n, target_n, source_fanout, target_fanin, ismotor, isrecurrent):
        b = torch.zeros(target_n)
        b = Parameter(b, requires_grad=True)
        w = torch.Tensor(source_n, target_n)
        nn.init.normal_(w)
        w = Parameter(w, requires_grad=False)
        ncp_w = torch.zeros(source_n, target_n)
        ncp_w = Parameter(ncp_w, requires_grad=False)
        rng = np.random.RandomState(22222)

        if ismotor == False:
            unreachable_neurons = [l for l in range(target_n)]
            for src in range(source_n):
                for dest in rng.choice(
                    target_n, size=source_fanout, replace=False
                ):
                    if dest in unreachable_neurons:
                        unreachable_neurons.remove(dest)
                    ncp_w[src][dest] = 1

            if isrecurrent == False:
                # If it happens that some target neurons are not connected, connect them now
                mean_neuron_fanin = int(
                    source_n * source_fanout / target_n
                )
                # Connect "forgetten" inter neuron by at least 1 and at most all sensory neuron
                mean_neuron_fanin = np.clip(
                    mean_neuron_fanin, 1, source_n
                )
                for dest in unreachable_neurons:
                    for src in rng.choice(
                        source_n, size=mean_neuron_fanin, replace=False
                    ):
                        ncp_w[src][dest] = 1
        else:
            unreachable_neurons = [l for l in range(source_n)]
            for dest in range(target_n):
                for src in rng.choice(
                    source_n, size=target_fanin, replace=False
                ):
                    if src in unreachable_neurons:
                        unreachable_neurons.remove(src)
                    ncp_w[src][dest] = 1

            # If it happens that some commandneurons are not connected, connect them now
            mean_neuron_fanout = int(
                target_n * target_fanin / source_n
            )
            # Connect "forgotten" command neuron to at least 1 and at most all motor neuron
            mean_neuron_fanout = np.clip(mean_neuron_fanout, 1, target_n)
            for src in unreachable_neurons:
                for dest in rng.choice(
                    target_n, size=mean_neuron_fanout, replace=False
                ):
                    ncp_w[src][dest] = 1
        w = w * ncp_w
        w = Parameter(w, requires_grad=False)
        return w, b

    def neuron_model(self, Layernum, pre_layer_output, current, volt, spike):
        """
        Neuron Model
        :param syn_func: synaptic function
        :param pre_layer_output: output from pre-synaptic layer
        :param current: current of last step
        :param volt: voltage of last step
        :param spike: spike of last step
        :return: current, volt, spike
        """
        # print("Laynum = ", Layernum)
        # print("pre_layer_output shape = ", pre_layer_output.shape)
        # print("w.t shape = ", self.w3.t().shape)
        # print("b shape = ", self.b3.shape)
        if Layernum == 1:
            pre_layer_output = nn.functional.linear(pre_layer_output, weight=self.w1.t(), bias=self.b1)
        elif Layernum == 2:
            pre_layer_output = nn.functional.linear(pre_layer_output, weight=self.w2.t(), bias=self.b2)
        elif Layernum == 3:
            pre_layer_output = nn.functional.linear(pre_layer_output, weight=self.w3.t(), bias=self.b3)
        else:
            pre_layer_output = nn.functional.linear(pre_layer_output, weight=self.w4.t(), bias=self.b4)
        
        current = current * NEURON_CDECAY + pre_layer_output
        volt = volt * NEURON_VDECAY * (1. - spike) + current
        spike = self.pseudo_spike(volt)
        return current, volt, spike

    def forward(self, x, batch_size):
        """

        :param x: state batch
        :param batch_size: size of batch
        :return: out
        """
        fc1_u = torch.zeros(batch_size, self.hidden1, device=self.device)
        fc1_v = torch.zeros(batch_size, self.hidden1, device=self.device)
        fc1_s = torch.zeros(batch_size, self.hidden1, device=self.device)
        fc2_u = torch.zeros(batch_size, self.hidden2, device=self.device)
        fc2_v = torch.zeros(batch_size, self.hidden2, device=self.device)
        fc2_s = torch.zeros(batch_size, self.hidden2, device=self.device)
        fc3_u = torch.zeros(batch_size, self.hidden3, device=self.device)
        fc3_v = torch.zeros(batch_size, self.hidden3, device=self.device)
        fc3_s = torch.zeros(batch_size, self.hidden3, device=self.device)
        fc4_u = torch.zeros(batch_size, self.action_num, device=self.device)
        fc4_v = torch.zeros(batch_size, self.action_num, device=self.device)
        fc4_s = torch.zeros(batch_size, self.action_num, device=self.device)
        x_sumspike = torch.zeros(batch_size, self.state_num, device=self.device)
        fc1_sumspike = torch.zeros(batch_size, self.hidden1, device=self.device)
        fc2_sumspike = torch.zeros(batch_size, self.hidden2, device=self.device)
        fc3_sumspike = torch.zeros(batch_size, self.hidden3, device=self.device)
        fc4_sumspike = torch.zeros(batch_size, self.action_num, device=self.device)
        for step in range(self.batch_window):
            '''
            input_spike [1, 256]
            fc1_s [1,256]
            fc2_s [1,256]
            fc3_s [1,256]
            fc4_s [1,2]
            '''
            input_spike = x[:, :, step]
            fc1_u, fc1_v, fc1_s = self.neuron_model(1, input_spike, fc1_u, fc1_v, fc1_s)
            fc2_u, fc2_v, fc2_s = self.neuron_model(2, fc1_s, fc2_u, fc2_v, fc2_s)
            fc3_u, fc3_v, fc3_s = self.neuron_model(3, fc2_s, fc3_u, fc3_v, fc3_s)
            fc4_u, fc4_v, fc4_s = self.neuron_model(4, fc3_s, fc4_u, fc4_v, fc4_s)
            x_sumspike += input_spike
            fc1_sumspike += fc1_s
            fc2_sumspike += fc2_s
            fc3_sumspike += fc3_s
            fc4_sumspike += fc4_s
        x_sumspike = x_sumspike / self.batch_window
        fc1_sumspike = fc1_sumspike / self.batch_window
        fc2_sumspike = fc2_sumspike / self.batch_window
        fc3_sumspike = fc3_sumspike / self.batch_window
        fc4_sumspike = fc4_sumspike / self.batch_window
        # print("batch_size:{},x_:{},fc1_s:{},fc2_s:{},fc3_s:{},fc4_s:{}".format(batch_size, x_sumspike.shape, fc1_sumspike.shape,fc2_sumspike.shape,fc3_sumspike.shape,fc4_sumspike.shape))
        self.input_spike = torch.sum(x_sumspike, dim=0)
        self.fc1_spike = torch.sum(fc1_sumspike, dim=0)
        self.fc2_spike = torch.sum(fc2_sumspike, dim=0)
        self.fc3_spike = torch.sum(fc3_sumspike, dim=0)
        self.fc4_spike = torch.sum(fc4_sumspike, dim=0)
        # print("x_sum:{},fc1_s:{},fc2_s:{},fc3_s:{},fc4_s:{}".format(self.input_spike.shape, self.fc1_spike.shape,self.fc2_spike.shape,self.fc3_spike.shape,self.fc4_spike.shape))
        out = fc4_sumspike / self.batch_window
        return out

    def _update(self, reward):
        self._updateone(reward, self.state_num, self.hidden1, self.w1, self.input_spike, self.fc1_spike, self.p_plus1, self.p_minus1, self.eligibility1, self.eligibility_trace1)
        self._updateone(reward, self.hidden1, self.hidden2, self.w2, self.fc1_spike, self.fc2_spike, self.p_plus2, self.p_minus2, self.eligibility2, self.eligibility_trace2)
        self._updateone(reward, self.hidden2, self.hidden3, self.w3, self.fc2_spike, self.fc3_spike, self.p_plus3, self.p_minus3, self.eligibility3, self.eligibility_trace3)
        self._updateone(reward, self.hidden3, self.action_num,  self.w4, self.fc3_spike, self.fc4_spike, self.p_plus4, self.p_minus4, self.eligibility4, self.eligibility_trace4)

    def _updateone(self, reward, source_n, target_n, w, source_spikes, target_spikes, p_plus, p_minus, eligibility, eligibility_trace):
        a_plus = torch.tensor(1.0)
        a_minus = torch.tensor(-1.0)

        eligibility_trace *= -1 / self.tc_e_trace
        eligibility_trace += self.nu * eligibility

        w += reward * eligibility_trace
        # print("p_plus shape:{}, source_spikes shape:{}".format(p_plus.shape, source_spikes.shape))
        p_plus *= torch.exp(-self.dt / self.tc_plus)
        p_plus += a_plus * source_spikes
        p_minus *= torch.exp(-self.dt / self.tc_minus)
        p_minus += a_minus * target_spikes

        eligibility = torch.ger(p_plus, target_spikes) + torch.ger(source_spikes, p_minus)
