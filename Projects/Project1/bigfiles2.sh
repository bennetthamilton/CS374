#! /bin/bash

path=$1
ls -la "$path" | tr -s " " | cut -d' ' -f 5,9 | sort -rn | head -n 7
