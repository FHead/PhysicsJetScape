
import matplotlib.pyplot as plt
import numpy as np

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

NDimension = len(AllData["labels"])
Ranges = np.array(AllData["ranges"]).T
DesignPoints = AllData["design"]

figure, axes = plt.subplots(figsize = (3 * NDimension, 3 * NDimension), ncols = NDimension, nrows = NDimension)
Names = AllData["labels"]
for i, row in enumerate(axes):
    for j, ax in enumerate(row):
        if i>j:
            ax.hist2d(DesignPoints[:, j], DesignPoints[:, i],
                      bins=50, range=[Ranges[:,j], Ranges[:,i]],
                      cmap='Greens')
            ax.set_xlabel(Names[j])
            ax.set_ylabel(Names[i])
            ax.set_xlim(*Ranges[:,j])
            ax.set_ylim(*Ranges[:,i])
        if i<=j:
            ax.axis('off')
plt.tight_layout()
tag = AllData['tag']
plt.savefig(f'result/{tag}/plots/DesignPoints.pdf', dpi = 192)
# plt.savefig(f'result/{tag}/plots/DesignPoints.png', dpi = 192)
# figure
