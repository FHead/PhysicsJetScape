cd ~/work/CMSSW/CMSSW_11_1_3/src
eval `scramv1 runtime -sh`
cd - > /dev/null
export CMSSW_SEARCH_PATH=$CMSSW_SEARCH_PATH:~/work/CMSSW/CMSSW_11_1_3/work/EssentialFiles

export FastJetConfig=/cvmfs/cms.cern.ch/slc7_amd64_gcc700/external/fastjet/3.3.0-omkpbe/bin/fastjet-config
