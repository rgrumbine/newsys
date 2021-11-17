export MP_CLOCK_SOURCE=OS
export MP_PROCS=1
export MP_RESD=poe
#export MP_HOSTFILE=hostfile

time poe ./shallow > stdout 2> stderr
#poe ./hello > stdout 2> stderr 
