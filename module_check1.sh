#!/bin/bash

module purge


module avail  >> avail.list1 2>> avail.err.list1
module spider >> spider.list1 2>> spider.err.list1

for f in bacio bufr bufr_dumplist dumpjb EnvVars grib_util grib_util hpss ibmpe ics impi ips NetCDF prod_envir prod_util prod_util python util_shared w3emc w3nco w3lib intel netcdf grads grib 
do
  g=`echo $f | cut -f1 -d\/`
  module -t avail  $g >> avail.list2 2>> avail.err.list2
  module -t spider $g >> avail.spider2 2>> avail.err.spider2
done

module load impi/2018.4
module list

module load netcdf/4.7.2
module list

module load python/3.7.5
module list


module avail >> avail.list3 2>> avail.err.list3
module spider >> spider.list2 2>> spider.err.list3

exit

for f in bacio/2.0.2 bufr/11.2.0 bufr_dumplist/2.0.0 dumpjb/5.0.0 EnvVars EnvVars/1.0.2 grib_util grib_util/1.1.0 hpss ibmpe ics impi/18.0.1 ips/18.0.1.163 module list NetCDF NetCDF/4.5.0 prod_envir/1.0.2 prod_util prod_util/1.1.0 python util_shared/1.1.0 w3emc/2.3.0 w3nco/2.0.6 
do
  module -t avail  $f >> avail.list3 2>> avail.err.list3
  module -t spider $f >> avail.spider3 2>> avail.err.spider3
done


source /etc/profile.d/modules.csh
. /usrx/local/Modules/3.2.10/init/bash
