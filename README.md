# VBS VH/VVH
This repository contains the code used to run the following analyses:
- VBS WH
- VBS VVH (all-hadronic)

It is structured as follows:
- `abcdnet`: Code to train ABCDNet (VBS VVH)
- `analysis`: [RAPIDO](https://github.com/jkguiang/rapido) analysis code
- `combine`: Code for running Higgs Combine limits
- `notebooks`: Collection of Jupyter notebooks
- `skimmer`: Deprecated [nanoAOD-tools](https://github.com/cms-nanoAOD/nanoAOD-tools) skimming code

# Instructions for reproducing VBS VVH (all-hadronic)
### 1. Log on to a UCSD UAF machine
First, decide on one UAF to use out of `uaf-2`, `uaf-3`, and `uaf-4`. 
Each machine has 7 TB of NVMe storage mounted to `/data`, which we will use heavily, but they are separate from each other, so we must select one (preferably, one that is the least heavily utilized).
```bash
ssh USER@uaf-N.t2.ucsd.edu
```

### 2. Create a symlink to the skims
```bash
mkdir -p /data/userdata/${USER}/nanoaod/
ln -s /data/userdata/phchang/nanoaod/VBSVVHSkim /data/userdata/${USER}/nanoaod/VBSVVHSkim
```

### 3. Clone the main repository
```bash
git clone git@github.com:jkguiang/vbs.git
cd vbs/analysis
source setup.sh
```

### 4. Obtain a copy of the `vbs/analysis/data` directory
This directory contains scale factors, cross section, python scripts containing lists of Project Metis `DBSSample` objects, etc.
```bash
scp -r uaf-2:/data/userdata/jguiang/vbs_data data # if you are on uaf-2 already, just cp -R
```

### 5. Set up RAPIDO
```bash
git clone git@github.com:jkguiang/rapido.git
cd rapido
make -j
cd -
```

### 6. Set up NanoTools (and make some modifications)
Eventually, we will create a branch of [NanoTools](https://github.com/cmstas/NanoTools) that has these changes. 
The changes below primarily add the branches needed to read the custom branch that we add for the ttH lepton ID MVA for NanoAOD v9. 
```bash
cd NanoTools/NanoCORE
cp /home/users/jguiang/projects/NanoTools/NanoCORE/Nano.h .
cp /home/users/jguiang/projects/NanoTools/NanoCORE/Nano.cc .
cp /home/users/jguiang/projects/NanoTools/NanoCORE/ElectronSelections.h .
cp /home/users/jguiang/projects/NanoTools/NanoCORE/ElectronSelections.cc .
cp /home/users/jguiang/projects/NanoTools/NanoCORE/MuonSelections.h .
cp /home/users/jguiang/projects/NanoTools/NanoCORE/MuonSelections.cc .
cp /home/users/jguiang/projects/NanoTools/NanoCORE/Tools/jetcorr/JetResolutionUncertainty.h Tools/jetcorr/JetResolutionUncertainty.h
rm -rf Tools/jetcorr/data/; cp -r /home/users/jguiang/projects/NanoTools/NanoCORE/Tools/jetcorr/data/ Tools/jetcorr/data/
cp /home/users/jguiang/projects/NanoTools/NanoCORE/Makefile . # comment out RHEL 7 line and comment in RHEL 8 line
cd -
```

### 7. Compile analysis code
```bash
make study=vbsvvhjets
```

### 8. Run the code (writes babies)
The command below will write babies (ROOT files with TTrees) to `/data/userdata/$USER/vbs_studies/vbsvvhjets/output_TestRun` using 64 parallel threads.
```bash
./bin/run vbsvvhjets --n_workers=64 --basedir=/data/userdata/$USER/vbs_studies --skimdir=/data/userdata/$USER/nanoaod/VBSVVHSkim --skimtag=0lep_2ak4_2ak8_ttH --data --tag=TestRun
```

### 9. Merge the output (contains TTree)
```bash
./bin/merge_vbsvvhjets vbsvvhjets --basedir=/data/userdata/$USER/vbs_studies --tag=TestRun
```

### 10. Install python stuff (in the mean time)
Assuming you have [Miniconda](https://docs.anaconda.com/free/miniconda/#quick-command-line-install) installed:
```bash
conda create -n vbs
conda activate vbs
conda install -c conda-forge numpy pandas matplotlib tqdm mplhep uproot scikit-learn
pip install yahist
```

### 11. Make plots
The command below will create plots in `$HOME/public_html/vbsvvhjets_plots/TestRun/`. You can use the `--help` flag to see all available options.
```bash
export PYTHONPATH=${PYTHONPATH}:$PWD
python scripts/make_plots_vbsvvh.py TestRun --allmerged # use --semimerged for semi-merged channel
```

### 12. Running the full workflow (nominal + all JEC/JER variations)
The command below will write babies (ROOT files with TTrees) to `/data/userdata/$USER/vbs_studies/vbsvvhjets/output_v1` using 64 parallel threads. 
A different output directory is populated for each JEC and JER variation.
```bash
sh scripts/runall_vbsvvhjets.sh v1 # v1 can be any string for tag
```

### 13. Creating the data cards
The following command will create datacards in `../combine/vbsvvh/datacards/VBSVVH_allmerged_v1`:
```bash
python scripts/make_datacards_vbsvvh.py v1 --allmerged # use --semimerged for semi-merged channel
```
Alternatively, the private samples (C2W, C2Z scan) can be used, and datacards will instead be written to `../combine/vbsvvh/datacards/Private_C2W_C2Z_allmerged_v1`:
```bash
python scripts/make_datacards_vbsvvh.py v1 --private --allmerged # use --semimerged for semi-merged channel
```

### 14. Setting up Higgs combine (in a different terminal)
```bash
cd ../combine # i.e. cd to the vbs/combine directory!
# Now set up combine:
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmssw-el7 # launch an SLC7 Singularity container
cmsrel CMSSW_11_3_4
cd CMSSW_11_3_4/src
cmsenv
git clone https://github.com/cms-analysis/HiggsAnalysis-CombinedLimit.git HiggsAnalysis/CombinedLimit
cd HiggsAnalysis/CombinedLimit
scramv1 b clean
scramv1 b -j 4
cd ../../../../
bash <(curl -s https://raw.githubusercontent.com/cms-analysis/CombineHarvester/main/CombineTools/scripts/sparse-checkout-https.sh)
cd CMSSW_11_3_4/src/
scramv1 b -j 4
cd ../../
```

### 15. Run the final limits and create the plots
```bash
cd vbsvvh
sh runLimits.sh datacards/VBSVVH_allmerged_v1 # results written to results/VBSVVH_allmerged_v1
sh plotLimits.sh results/VBSVVH_allmerged_v1
```
    
