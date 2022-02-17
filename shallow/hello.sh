#!/bin/sh
export MP_CLOCK_SOURCE=OS
#export MP_DEVTYPE=ib
#export MP_EUIDEVICE=sn_all
#export MP_EUILIB=us
export MP_PROCS=2
export MP_RESD=poe
export MP_HOSTFILE=hostfile

#time poe ./shallow > stdout 2> stderr
#poe ./hello > stdout 2> stderr 
echo poe ./hello 
./hello 
