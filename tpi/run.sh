#!/bin/bash

#cd /media/passport/CERN/Therminator_pPb/pPb/eventsbal
#unlike-sign
#52
for i in 1
do
# 16 - pi-pi unlikesign
    ./tpi 202 1 0 16 10 10000000 1 0 1 0 inputFiles/calm_v2_fixjet_minijets_$i.txt
    mv outfilepipiulcf202p.root outfilepipiulcf202p.v2_fixjet_minijets.root.$i

# 0 - pi-pi 
    ./tpi 202 1 0 0 10 10000000 1 0 1 0 inputFiles/calm_v2_fixjet_minijets_$i.txt
    mv outfilecf202p.root outfilecf202p.v2_fixjet_minijets.root.$i

# 1- KK
    ./tpi 202 1 0 1 10 10000000 1 0 1 0 inputFiles/calm_v2_fixjet_minijets_$i.txt
    mv outfilekkcf202p.root outfilekkcf202p.v2_fixjet_minijets.root.$i

# 19 - KK unlike
    ./tpi 202 1 0 19 10 10000000 1 0 1 0 inputFiles/calm_v2_fixjet_minijets_$i.txt
    mv outfilekkulcf202p.root outfilekkulcf202p.v2_fixjet_minijets.root.$i

#12 - proton-proton
    ./tpi 202 1 0 12 10 10000000 1 0 1 0 inputFiles/calm_v2_fixjet_minijets_$i.txt
    mv outfileppcf202p.root outfileppcf202p.v2_fixjet_minijets.root.$i

#20 - proton-proton unlike-sign

    ./tpi 202 1 0 20 10 10000000 1 0 1 0 inputFiles/calm_v2_fixjet_minijets_$i.txt
    mv outfileppulcf202p.root outfileppulcf202p.v2_fixjet_minijets.root.$i

#21 - pion0-pion0
    ./tpi 202 1 0 21 10 10000000 1 0 1 0 inputFiles/calm_v2_fixjet_minijets_$i.txt
    mv outfilepi0cf202p.root outfilepi0cf202p.v2_fixjet_minijets.root.$i

#22 - pion minus
    ./tpi 202 1 0 22 10 10000000 1 0 1 0 inputFiles/calm_v2_fixjet_minijets_$i.txt
    mv outfilepimcf202p.root outfilepimcf202p.v2_fixjet_minijets.root.$i

done
#cd /opt/CERN/2013/pA/pion/IRC/Therminator/tpi/tpi

 #c d e f g h i j k l m n o p q r s t u v x y z
