#!/bin/bash

str="blah"
f=./best.yaml
name="./${str}.yaml"

cp $f $name

sed -i -e "s/best_gen_Bottom_1_Pt.txt/${str}.txt/g" $name
