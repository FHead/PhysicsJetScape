import matplotlib.pyplot as plt
import numpy as np

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

DataList = AllData["observables"][0][1]

N = len(DataList)

figure, axes = plt.subplots(figsize = (3 * N, 3 * N), nrows = N, ncols = N)

for I1, Item1 in enumerate(DataList):
    for I2, Item2 in enumerate(DataList):
        Cov = AllData["cov"]["HeavyIon"][("R_AA", Item1)][("R_AA", Item2)]
        if Cov != None:
            axes[I1][I2].imshow(Cov, vmin = 0)

plt.tight_layout()
tag = AllData['tag']
figure.savefig(f'result/{tag}/plots/Covariance.pdf', dpi = 192)

