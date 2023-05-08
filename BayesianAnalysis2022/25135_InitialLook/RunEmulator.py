
import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

import subprocess
subprocess.call(['python3', '-m', 'src.emulator',
                 '--retrain', '--npc', str(AllData['emulator']['npc']),
                 '--kernelchoice', AllData['emulator']['kernel'],
                 '--nu', str(AllData['emulator']['nu'] if 'nu' in AllData['emulator'] else 1.5),
                 '--nrestarts', str(AllData['emulator']['retrain'])])

