#! /bin/sh
# a quick hack script to generate necessary files from 
# auto* tools.
#
# WARNING: if you run this you will change the versions
# of the tools which are used and, maybe, required!

set -e

mkdir -p m4

autoreconf -i
