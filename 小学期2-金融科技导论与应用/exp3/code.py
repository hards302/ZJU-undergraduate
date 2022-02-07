import jieba
from jieba import analyse
import sklearn as sl
from sklearn.model_selection import KFold
from sklearn.naive_bayes import MultinomialNB
from sklearn.metrics import classification_report
import pandas as pd
import numpy as np
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer

# take handle of the data and merge the csv
data = pd.read_csv("nlp_dataset/business_scope.csv")
label = pd.read_csv("nlp_dataset/index_member.csv")
for tc in data.ts_code:
    tc = tc.strip()
for cc in label.con_code:
    cc = cc.strip()
label = label.rename(columns={"con_code":"ts_code"})
da = pd.merge(data, label, on="ts_code")

# cut the text and store in a list
N = len(da.business_scope)
seg_list = []
for i in range(N):
    seg_list.append((jieba.lcut(da.business_scope[i])))

# take the keywords from the text and store in a list
keywords = []
for i in range(N):
    keywords.append(analyse.extract_tags(da.business_scope[i]))

# transform the list in keywords to string to fit in the TfidfVectorizer().fit_transform()
for i in range(len(keywords)):
    keywords[i] = " ".join('%s' %id for id in keywords[i])

# tfidf
vectorizer = CountVectorizer()
vctf = TfidfVectorizer()
vc_fit = vectorizer.fit_transform(keywords)
tfidf = vctf.fit_transform(keywords)

# KFold with 7 numbers
kf = KFold(n_splits=7, shuffle=True, random_state=0)
clf = MultinomialNB()
labels = da.index_name

# classification and analys
for train_index, test_index in kf.split(tfidf):
    clf = MultinomialNB().fit(tfidf[train_index], labels[train_index])
    y_pred = clf.predict(tfidf[test_index])
    print(classification_report(labels[test_index], y_pred))