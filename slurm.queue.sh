#!/bin/bash

# Usage: slurm queue.sh
#
# Rejected: #SBATCH --partition=batch
# Rejected: #SBATCH --partition=400p48h
# OK (30 minute limit):  #SBATCH --partition=debug
#SBATCH --partition=orion
#SBATCH --account=fv3-cpu
#SBATCH --time=01:00:00
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH -J=qtest

echo hello from queue orion
