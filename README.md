Instructions for running on IU quartz

Clone this repository in a working area

Run the following commands to build the simulation

source /N/project/phonon/setup_phonon.sh -- sets up access to the Geant4 and G4CMP build in the /N/project/phonon/ area
cd </path/to/working/area>
mkdir PHONON-build
cd PHONON-build
cmake -DG4CMP_DIR=/N/project/phonon/G4CMP/G4CMP-install/cmake/ ../
make

This will build the simulation, to run it navigate one directory lower and run
./PHONON-build/PHONONBkgnd </path/to/macro/macro.mac>
There are example macros in the main directory to run with and without the shielding package
