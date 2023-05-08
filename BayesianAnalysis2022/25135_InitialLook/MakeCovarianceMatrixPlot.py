import matplotlib.pyplot as plt
import numpy as np

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

DataList = AllData["observables"][0][1]

N = len(DataList)

RC = int(np.sqrt(N))
CC = int(np.ceil(N / RC))

figure, axes = plt.subplots(figsize = (3 * RC, 3 * CC), nrows = RC, ncols = CC)

for I, Item in enumerate(DataList):
    ax = I % RC
    ay = int(np.floor(I / RC))

    axes[ax][ay].set_xlabel(r"$p_{T}$ index")
    axes[ax][ay].set_ylabel(r"$p_{T}$ index")
    axes[ax][ay].set_title(Item)

    Cov = AllData["cov"]["HeavyIon"][("R_AA", Item)][("R_AA", Item)]
    if Cov.any() != None:
        axes[ax][ay].imshow(Cov, vmin = 0)

plt.tight_layout()
tag = AllData['tag']
figure.savefig(f'result/{tag}/plots/DiagonalCovariance.pdf', dpi = 192)

