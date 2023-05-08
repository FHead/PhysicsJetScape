import matplotlib.pyplot as plt
import numpy as np
from sklearn.decomposition import PCA
from sklearn.preprocessing import StandardScaler

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

if 'pcacheck' in AllData['emulator']:
    PCACheck = AllData['emulator']['pcacheck']
else:
    PCACheck = [1, AllData['emulator']['npc']]

X = []
Slices = {}
NObs = 0
for Item in AllData['data']['HeavyIon']['R_AA']:
    X.append(AllData['model']['HeavyIon']['R_AA'][Item]['Y'])
    N = X[-1].shape[1]
    Slices[Item] = slice(NObs, NObs + N)
    NObs = NObs + N
X = np.concatenate(X, axis=1)

SystemCount = len(AllData["systems"])
BinCount = len(AllData['observables'][0][1])

RC = int(np.sqrt(BinCount))
CC = int(np.ceil(BinCount / RC))

def ReduceDimension(Data, NPC, Slices):
    scaler = StandardScaler(copy = True)
    pca = PCA(n_components = NPC, copy = True, whiten = True, svd_solver = 'full')
    Y = pca.fit_transform(scaler.fit_transform(Data))
    XTemp = pca.inverse_transform(Y) * scaler.scale_ + scaler.mean_
    return {"R_AA": {Item: XTemp[..., S] for Item, S in Slices.items()}}

RMS = {}

for NPC in PCACheck:
    print(f'Processing dimension reduction with NPC = {NPC}...')

    XNew = ReduceDimension(X, NPC, Slices)

    # Check reduced dimension

    figure, axes = plt.subplots(figsize = (3 * CC, 3 * RC), nrows = RC, ncols = CC)

    for s2 in range(0, BinCount):
        ax = s2 % RC
        ay = int(np.floor(s2 / RC))
        axes[ax][ay].set_xlabel(r"$p_{T}$")
        axes[ax][ay].set_ylabel(r"$R_{AA}$")

        S1 = AllData["systems"][0]
        O  = AllData["observables"][0][0]
        S2 = AllData["observables"][0][1][s2]

        DX = AllData["data"][S1][O][S2]['x']
        DY = AllData["data"][S1][O][S2]['y']
        DE = np.sqrt(AllData["data"][S1][O][S2]['yerr']['stat'][:,0]**2 + AllData["data"][S1][O][S2]['yerr']['sys'][:,0]**2)

        axes[ax][ay].set_title(AllData["observables"][0][1][s2])

        # for i, y in enumerate(XNew[O][S2]):
        #     axes[ax][ay].plot(DX, y, 'b-', alpha=0.2, label="Posterior" if i==0 else '')
        # axes[ax][ay].errorbar(DX, DY, yerr = DE, fmt='ro', label="Measurements")

    # plt.tight_layout()
    # tag = AllData['tag']
    # figure.savefig(f'result/{tag}/plots/PCACheck_N{NPC}.pdf', dpi = 192)

    # Check pull

    figure, axes = plt.subplots(figsize = (3 * CC, 3 * RC), nrows = RC, ncols = CC)

    allpull = []

    for s2 in range(0, BinCount):
        ax = s2 % RC
        ay = int(np.floor(s2 / RC))
        axes[ax][ay].set_xlabel(r"$p_{T}$")
        axes[ax][ay].set_ylabel(r"$R_{AA}$ (PCA'd - Original) / Error")

        S1 = AllData["systems"][0]
        O  = AllData["observables"][0][0]
        S2 = AllData["observables"][0][1][s2]

        DX = AllData["data"][S1][O][S2]['x']
        DY = AllData["data"][S1][O][S2]['y']
        DE = np.sqrt(AllData["data"][S1][O][S2]['yerr']['stat'][:,0]**2 + AllData["data"][S1][O][S2]['yerr']['sys'][:,0]**2)

        axes[ax][ay].set_title(AllData["observables"][0][1][s2])

        for i, y in enumerate(XNew[O][S2]):
            yoriginal = AllData['model']['HeavyIon']['R_AA'][S2]['Y'][i]
            yerror    = AllData['model']['HeavyIon']['R_AA'][S2]['YError'][i]
            pull      = (y - yoriginal) / yerror
            # axes[ax][ay].plot(DX, pull, 'b-', alpha=0.2)

            allpull.append(pull)

    # plt.tight_layout()
    # tag = AllData['tag']
    # figure.savefig(f'result/{tag}/plots/PCAPullCheck_N{NPC}.pdf', dpi = 192)

    # Pull summary plot

    allpull = np.concatenate(allpull)

    figure, axes = plt.subplots(figsize = (3, 3), nrows = 1, ncols = 1)
    figure.subplots_adjust(top = 0.95, left = 0.1, right = 0.95, bottom = 0.1)

    axes.hist(allpull, bins = 200, histtype = 'step')

    axes.text(0.125, 0.90, 'Mean = {:.2f}'.format(np.mean(allpull)), transform = axes.transAxes)
    axes.text(0.125, 0.85, 'RMS = {:.2f}'.format(np.std(allpull)), transform = axes.transAxes)

    tag = AllData['tag']
    figure.savefig(f'result/{tag}/plots/PCAPullSummary_N{NPC}.pdf', dpi = 192)

    plt.close('all')

    RMS[NPC] = np.std(allpull)

figure, axes = plt.subplots(figsize = (5, 5), nrows = 1, ncols = 1)
figure.subplots_adjust(top = 0.95, left = 0.15, right = 0.95, bottom = 0.15)

axes.plot(list(RMS.keys()), list(RMS.values()), 'bo-')
axes.set_xlabel("NPC")
axes.set_ylabel("Pull width")

axes.axhline(y = 1, xmin = 0, color='red', linestyle='dotted', linewidth=2)
axes.axhline(y = 2, xmin = 0, color='red', linestyle='dotted', linewidth=2)
plt.grid('both')

tag = AllData['tag']
figure.savefig(f'result/{tag}/plots/PCAPullRMS.pdf', dpi = 192)

plt.close('all')




