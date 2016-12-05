#!/bin/bash

template_best=./template_best.yaml
template_smeared=./template_smeared.yaml

f=/afs/cern.ch/user/s/shtan/toprecoody_may31/jul6/test_wttHReco_newcode2/results/test/old/best_gen_Wd11_Pt.txt
#for f in $FILES
#do
echo "Processing $f file..."
filename=$(basename "$f")
filename_str="${filename%.*}"

best_yaml_name="./${filename_str}.yaml"

cp $template_best $best_yaml_name

sed -i -e "s/put_input_file_name_here/$filename/g" $best_yaml_name

prefix="best"
tempsmearedname=${filename_str#$prefix}
smeared_filename="smeared${tempsmearedname}.txt"
smeared_yaml_name="./smeared${tempsmearedname}.yaml"

cp $template_smeared $smeared_yaml_name

sed -i -e "s/put_extra_file_name_here/$smeared_filename/g" $best_yaml_name

sed -i -e "s/put_input_file_name_here/$smeared_filename/g" $smeared_yaml_name

#done
