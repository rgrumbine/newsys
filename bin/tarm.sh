#!/bin/sh

tar xf $1
if [ $? -eq 0 ] ; then
  rm $1
fi
