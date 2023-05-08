
import matplotlib.pyplot as plt
import numpy as np

import src
src.Initialize()
from src import mcmc
chain = mcmc.Chain()
MCMCSamples = chain.load()

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)


NDimension = len(AllData["labels"])
Ranges = np.array(AllData["ranges"]).T
figure, axes = plt.subplots(figsize = (3 * NDimension, 3 * NDimension), ncols = NDimension, nrows = NDimension)
Names = AllData["labels"]

Truth = np.full(NDimension, -999)
if 'holdoutdesign' in AllData:
    Truth = AllData['holdoutdesign'][0]

for i, row in enumerate(axes):
    for j, ax in enumerate(row):
        if i==j:
            ax.hist(MCMCSamples[:,i], bins=50,
                    range=Ranges[:,i], histtype='step', color='green')
            ax.axvline(x = Truth[i], ymin = 0, color='red', linestyle='dotted', linewidth=5)
            ax.set_xlabel(Names[i])
            ax.set_xlim(*Ranges[:,j])
        if i>j:
            ax.hist2d(MCMCSamples[:, j], MCMCSamples[:, i],
                      bins=50, range=[Ranges[:,j], Ranges[:,i]],
                      cmap='Greens')
            ax.scatter(Truth[j], Truth[i], s = 80, linewidths = 2.5, marker = 'o', facecolors = 'none', edgecolors = 'r')
            ax.set_xlabel(Names[j])
            ax.set_ylabel(Names[i])
            ax.set_xlim(*Ranges[:,j])
            ax.set_ylim(*Ranges[:,i])
        if i<j:
            ax.axis('off')
plt.tight_layout()
tag = AllData['tag']
plt.savefig(f'result/{tag}/plots/Correlation.pdf', dpi = 192)
# plt.savefig(f'result/{tag}/plots/Correlation.png', dpi = 192)
