import pandas as pd
import numpy as np

data = pd.read_csv('data/train_new.csv')

data.head()

missing_value_sum = data.isnull().sum()
print(missing_value_sum[0:10])

corr = data.corr()
print(corr.head())

cov = data.cov()
print(cov.head())

data.fillna(-1, inplace=True)
print(data.head())

data.replace(to_replace=-1,value=np.NaN,inplace=True)
print(data.head())

data.fillna(data.mean(),inplace=True)
print(data.head())

data.dropna(inplace=True)
print(data.head())

data.dropna(how='all',inplace=True)
print(data.head())

data.dropna(thresh=10,inplace=True)
print(data.head())

data.dropna(subset=['Y'],inplace=True)
print(data.head())

numeric_feats = data.dtypes[data.dtypes!="object"].index
data[numeric_feats] = data[numeric_feats].apply(lambda x:(x-x.min())/(x.max()-x.min()))
print(data.head())

numeric_feats = data.dtypes[data.dtypes!="object"].index
data[numeric_feats] = data[numeric_feats].apply(lambda x:(x-x.mean())/(x.std()))
print(data.head())

dataX65_bin = pd.qcut(data.X65,q=10,duplicates='drop')
print(dataX65_bin)

dataX66_bin = pd.cut(data.X66,bins=[100,200,300,400,500,600])
print(dataX66_bin)

def add_cross_feature(data,feature1,feature2):
    comb_index = data[[feature1,feature2]].drop_duplicates()
    comb_index[feature1+'_'+feature2]=np.arange(comb_index.shape[0])
    data = pd.merge(data, comb_index, 'left', on=[feature1,feature2])
    return data

data = add_cross_feature(data, 'X34', 'X36')