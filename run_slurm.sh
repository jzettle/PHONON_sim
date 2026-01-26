#!/bin/bash
MACRO=$1
DIR=$2
INSTANCE=$3

#Make a temporary copy of the macro with a filename "<original filename>_N"
#according to whichever thread N we are in. The 'sed' command replaces
#the existing *.root file with *_N.root
sed -e 's|setOutputFile \(.*\).root|setOutputFile '"${DIR}"'/\1_'"${INSTANCE}"'.root|' -e 's|offset 0|offset '"${INSTANCE}"'|' < ${MACRO} > ${MACRO}_${INSTANCE}
./PHONON-build/PHONONBkgnd -m ${MACRO}_${INSTANCE} -s ${INSTANCE} > $DIR/output_${INSTANCE}.txt
rm ${MACRO}_${INSTANCE}
