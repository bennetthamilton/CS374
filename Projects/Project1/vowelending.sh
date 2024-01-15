#! /bin/bash

path=$1
ls -a "$path" | grep -i '[AaEeIiOoUu]$'
