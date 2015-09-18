#!/bin/bash

path="/home/wfpw/CERN/CALM/CALM3/eventsGENBOD_minijets_10_20_newTotMom/eventsGENBOD_minijets_10_20_newTotalMom_"
name="CALM_minijetsGENBOD_10_20_newTotalMom"

#52
for i in 1
do

#21 - pion0-pion0
    ./tpi 202 1 0 21 10 10000000 1 0 1 0 $path$i.txt
    mv outfilepi0cf202p.root outfilepi0cf202p.$name.root.$i

#22 - pion minus
    ./tpi 202 1 0 22 10 10000000 1 0 1 0 $path$i.txt
    mv outfilepimcf202p.root outfilepimcf202p.$name.root.$i

# 16 - pi-pi unlikesign
    ./tpi 202 1 0 16 10 10000000 1 0 1 0 $path$i.txt
    mv outfilepipiulcf202p.root outfilepipiulcf202p.$name.root.$i

# 0 - pi-pi 
    ./tpi 202 1 0 0 10 10000000 1 0 1 0 $path$i.txt
    mv outfilecf202p.root outfilecf202p.$name.root.$i

# 1- KK
    ./tpi 202 1 0 1 10 10000000 1 0 1 0 $path$i.txt
    mv outfilekkcf202p.root outfilekkcf202p.$name.root.$i

# 19 - KK unlike
    ./tpi 202 1 0 19 10 10000000 1 0 1 0 $path$i.txt
    mv outfilekkulcf202p.root outfilekkulcf202p.$name.root.$i

#12 - proton-proton
    ./tpi 202 1 0 12 10 10000000 1 0 1 0 $path$i.txt
    mv outfileppcf202p.root outfileppcf202p.$name.root.$i

#20 - proton-proton unlike-sign

    ./tpi 202 1 0 20 10 10000000 1 0 1 0 $path$i.txt
    mv outfileppulcf202p.root outfileppulcf202p.$name.root.$i

done


hadd $name.root outfile*$name.root*

#cd /opt/CERN/2013/pA/pion/IRC/Therminator/tpi/tpi

 #c d e f g h i j k l m n o p q r s t u v x y z
