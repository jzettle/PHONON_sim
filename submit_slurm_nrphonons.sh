#!/bin/bash

#SBATCH -J PHONON_sim
#SBATCH -p general
#SBATCH -o /N/project/phonon/PHONON_Bkgnds/%j.o
#SBATCH -e /N/project/phonon/PHONON_Bkgnds/%j.e
#SBATCH --mail-type=ALL
#SBATCH --mail-user=jczettle@iu.edu 
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --time=24:00:00
#SBATCH -A r00253

#Call "date" at beginning and end just to get timestamps
date

MACROFILE=cevns_phonon.mac
OUTPUT_DIR=/N/project/phonon/PHONON_Bkgnds/job_submission/multisim/${SLURM_JOB_ID}

source /N/project/phonon/setup_phonon.sh
cd /N/project/phonon/PHONON_Bkgnds/ #go to phonon sim directory

# make sure any changes are compiled (optional)
#cd build
#make
#cd ..

module load gnu-parallel     #needed for "parallel" command
mkdir -p ${OUTPUT_DIR}       #this is where all output will go
#( git log ; \
#  git status ; \
#  git diff ) \
#  > ${OUTPUT_DIR}/gitstatus.txt #document the code and any uncommited changes

#off we go
seq -w 0 15 | parallel bash run_slurm.sh ${MACROFILE} ${OUTPUT_DIR} {}

#add the root files together for this node.
#cd ${OUTPUT_DIR}
#hadd sum.root *.root

date
