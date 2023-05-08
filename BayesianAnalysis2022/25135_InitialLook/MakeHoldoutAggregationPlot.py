import matplotlib.pyplot as plt
import numpy as np
import argparse
import os

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

parser = argparse.ArgumentParser()
parser.add_argument("--Result", help = "result base folder", default = "result")
parser.add_argument("--Base", help = "base name")
args = parser.parse_args()

List = [x for x in os.listdir(args.Result) if (args.Base + "_Holdout") in x]

DataList = AllData['observables'][0][1]

AllX = np.array([])
AllY = np.array([])
AllE = np.array([])
AllP = np.array([])

for HoldoutItem in List:
    HoldoutItemBase = args.Result + '/' + HoldoutItem + '/txt'

    X = np.array([])
    Y = np.array([])
    E = np.array([])
    P = np.array([])

    for Item in DataList:
        X = np.append(X, np.loadtxt(f'{HoldoutItemBase}/{Item}_X.txt'))
        Y = np.append(Y, np.loadtxt(f'{HoldoutItemBase}/{Item}_Y.txt'))
        E = np.append(E, np.loadtxt(f'{HoldoutItemBase}/{Item}_E.txt'))
        P = np.append(P, np.loadtxt(f'{HoldoutItemBase}/{Item}_HoldoutPredictedDesign.txt.gz'))

    figure, axes = plt.subplots(figsize = (5, 5))

    axes.plot([0, 1], [0, 1])
    axes.errorbar(Y, P, xerr = E, fmt = 'bo', alpha = 0.5)

    axes.set_xlim([0, 1])
    axes.set_ylim([0, 1])
    axes.set_xlabel('Input')
    axes.set_ylabel('Output')

    plt.tight_layout()
    figure.savefig(f'result/plots/{HoldoutItem}_HoldoutAggregate.pdf', dpi = 192)
    figure.savefig(f'result/plots/{HoldoutItem}_HoldoutAggregate.png', dpi = 192)

    plt.close('all')

    AllX = np.append(AllX, X)
    AllY = np.append(AllY, Y)
    AllE = np.append(AllE, E)
    AllP = np.append(AllP, P)

figure, axes = plt.subplots(figsize = (5, 5))

axes.plot([0, 1], [0, 1])
axes.errorbar(AllY, AllP, xerr = AllE, fmt = 'bo', alpha = 0.02)

axes.set_xlim([0, 1])
axes.set_ylim([0, 1])
axes.set_xlabel('Input')
axes.set_ylabel('Output')

plt.tight_layout()
figure.savefig(f'result/plots/{args.Base}_HoldoutAggregate.pdf', dpi = 192)
figure.savefig(f'result/plots/{args.Base}_HoldoutAggregate.png', dpi = 192)


