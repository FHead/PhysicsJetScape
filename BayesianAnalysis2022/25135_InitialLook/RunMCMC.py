
import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

print(AllData['mcmc'])

import subprocess
subprocess.call(['python3', '-m', 'src.mcmc',
                 '--nwalkers', str(AllData['mcmc']['walker']),
                 '--nburnsteps', str(AllData['mcmc']['burn']), str(AllData['mcmc']['step'])])

