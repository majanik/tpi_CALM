CC	= g++
LD	= g++

CALM_PATH = ${HOME}/CERN/CALM/CALM
CFLAGS	= `root-config --cflags` -O3 -g -I../SphericalHarmonics -I${CALM_PATH}/include
LDFLAGS = `root-config --libs` -g -lgsl -lgslcblas


all: tpi

tpi: tpi.o PairReader.o PairWeight.o ExpCF3D.o ExpCF1D.o ExpCFSH.o ExpCFEP.o SourceMonitor.o TChainProxy.o ../SphericalHarmonics/ylm.o ../SphericalHarmonics/CorrFctnDirectYlm.o ${CALM_PATH}/build/CMakeFiles/calm.dir/src/ParticleCoor.cxx.o
	$(LD) $^ -o $@ $(LDFLAGS)

%.o: %.cxx
	$(CC) $^ -o $@ $(CFLAGS) -c 

clean: 
	rm -f *.o tpi twointeg_sh_mr test
