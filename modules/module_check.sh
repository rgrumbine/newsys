#!/bin/bash

module purge
module avail  >> avail.list1 2>> avail.err.list1
module spider >> spider.list1 2>> spider.err.list1


for f in bacio bufr bufr_dumplist dumpjb EnvVars grib_util grib_util hpss ibmpe ics impi ips NetCDF prod_envir prod_util python util_shared w3emc w3nco w3lib intel netcdf grads grib wgrib cnvgrib lsf slurm munge hdf hdf5 jasper g2lib
do
  g=`echo $f | cut -f1 -d\/`
  module -t avail  $g >> avail.list2 2>> avail.err.list2
  module -t spider $g >> spider.list2 2>> spider.err.list2
done

module load python/3.7.5
#module list

module load impi/2018.4
#module list
#Gives munge and slurm as well

module load netcdf/4.7.2
#module list
#Gives hdf as well

module load w3lib/2.0.6
module load cnvgrib/3.1.1
module load wgrib/2.0.8 #note conflict about grib1, named wgrib/1.8.0b 
#also have jasper, g2lib
module load grads/2.2.1

module list > also.1 2> also.2


for f in bacio bufr bufr_dumplist dumpjb grib_util EnvVars lsf hpss intel ibmpe ics ips prod_envir prod_util util_shared w3emc w3nco 
do
  module -t avail  $f >> avail.list3 2>> avail.err.list3
  module -t spider $f >> spider.list3 2>> spider.err.list3
done

exit

module load contrib
for f in bacio bufr bufr_dumplist dumpjb EnvVars grib_util lsf hpss intel ibmpe ics ips prod_envir prod_util util_shared w3emc w3nco 
do
  module -t avail  $f >> avail.list4 2>> avail.err.list4
  module -t spider $f >> spider.list4 2>> spider.err.list4
done

