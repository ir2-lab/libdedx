#!/bin/bash

#
# Variables
#

# path to generator exec
generator="./.build/generator"
# folder with SRIM96 scoef.*
data="./data"
# SRmodule directory
srmod_dir="/home/george/.local/SRIM-2013/srmodule"
# SRmodule command
srmod_cmd="wine SRModule.exe"
# dpass directory
dpass_dir="/home/george/Dev/ionsim/PASS/DPASS2106/"
# dpass command
dpass_cmd="wine DPASS.exe -batch"

$generator "$data" "$srmod_dir" "$srmod_cmd" "$dpass_dir" "$dpass_cmd"

