import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--Key", help = "key to print", default = "")
args = parser.parse_args()

import pickle
AllData = {}
with open('input/default.p', 'rb') as handle:
    AllData = pickle.load(handle)

if args.Key == "":
    print(AllData)
else:
    print(AllData[args.Key])
