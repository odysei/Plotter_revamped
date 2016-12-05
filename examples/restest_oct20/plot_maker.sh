#!/bin/bash

FILES=best_gen_*.yaml
for f in $FILES
do
    ../../bin/plotter $f
done
