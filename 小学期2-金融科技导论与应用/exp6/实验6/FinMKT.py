import numpy as np
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
import os
from sklearn.model_selection import train_test_split
from sklearn.model_selection import GridSearchCV
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import classification_report, confusion_matrix

# some usable model
from sklearn.neighbors import KNeighborsClassifier
from sklearn.svm import SVC
from sklearn.linear_model import LogisticRegression
from sklearn.tree import DecisionTreeClassifier
from sklearn.neural_network import MLPClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import AdaBoostClassifier
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.ensemble import BaggingClassifier

import warnings
warnings.filterwarnings('ignore')
os.chdir('/lqq/FinMKT/venv')


def data_preprocess(data):
    # your code here
    # example:
    x = pd.get_dummies(data)

    #TODO

    # your code end
    return x

def predict(x_train, x_test, y_train):

    # your code here begin
    # train your model on 'x_train' and 'x_test'
    # predict on 'y_train' and get 'y_pred'
    model = KNeighborsClassifier()
    model.fit(x_train, y_train)
    y_pred = model.predict(x_test)

    # your code here end

    return y_pred

def split_data(data):
    y = data.y
    x = data.loc[:, data.columns != 'y']
    x = data_preprocess(x)
    x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.3, random_state=1)
    return x_train, x_test, y_train, y_test

def print_result(y_test, y_pred):
    report = confusion_matrix(y_test, y_pred)
    precision = report[1][1] / (report[:, 1].sum())
    recall = report[1][1] / (report[1].sum())
    print('model precision:' + str(precision)[:4] + ' recall:' + str(recall)[:4])

if __name__ == '__main__':
    data = pd.read_csv('bank-additional-full.csv', sep=';')
    x_train, x_test, y_train, y_test = split_data(data)
    y_pred = predict(x_train, x_test, y_train)
    print_result(y_test, y_pred)

