#!/bin/sh
#31 Aug 2006  Robert Grumbine

x=`basename $1`
#echo " "
#echo $1
if [ $x = 'Makefile' -o -x = 'configure' ] ; then
  echo skipping makefiles
else
  findcounts ./$x
fi
