#!/bin/bash

template_best_gev=./template_best_gev.yaml
template_smeared=./template_smeared.yaml

template_best_angle=./template_best_angle.yaml

FILES_Pt=/afs/cern.ch/user/s/shtan/toprecoody_may31/jul6/test_wttHReco_newcode2/results/test/best_gen_*_Pt.txt
FILES_Phi=/afs/cern.ch/user/s/shtan/toprecoody_may31/jul6/test_wttHReco_newcode2/results/test/best_gen_*_Phi.txt
FILES_Eta=/afs/cern.ch/user/s/shtan/toprecoody_may31/jul6/test_wttHReco_newcode2/results/test/best_gen_*_Eta.txt
FILES_M=/afs/cern.ch/user/s/shtan/toprecoody_may31/jul6/test_wttHReco_newcode2/results/test/best_gen_*_M.txt

for f in $FILES_Pt $FILES_M
do
    echo "Processing $f file..."
    filename=$(basename "$f")
    filename_str="${filename%.*}"

    best_yaml_name="./${filename_str}.yaml"

    cp $template_best_gev $best_yaml_name

    sed -i -e "s/put_input_file_name_here/$filename/g" $best_yaml_name

    prefix="best"
    tempsmearedname=${filename_str#$prefix}
    smeared_filename_str="smeared${tempsmearedname}"
    smeared_yaml_name="./smeared${tempsmearedname}.yaml"

    cp $template_smeared $smeared_yaml_name

    sed -i -e "s/put_extra_file_name_here/${smeared_filename_str}.yaml/g" $best_yaml_name

    sed -i -e "s/put_input_file_name_here/${smeared_filename_str}.txt/g" $smeared_yaml_name
done

for f in $FILES_Phi $FILES_Eta
do
    echo "Processing $f file..."
    filename=$(basename "$f")
    filename_str="${filename%.*}"

    best_yaml_name="./${filename_str}.yaml"

    cp $template_best_angle $best_yaml_name

    sed -i -e "s/put_input_file_name_here/$filename/g" $best_yaml_name

    prefix="best"
    tempsmearedname=${filename_str#$prefix}
    smeared_filename_str="smeared${tempsmearedname}"
    smeared_yaml_name="./smeared${tempsmearedname}.yaml"

    cp $template_smeared $smeared_yaml_name

    sed -i -e "s/put_extra_file_name_here/${smeared_filename_str}.yaml/g" $best_yaml_name

    sed -i -e "s/put_input_file_name_here/${smeared_filename_str}.txt/g" $smeared_yaml_name
done
