#include <sys/stat.h>
#include <math.h>
#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TProfile.h>
#include <TMath.h>

#include "CorrFctnDirectYlm.h"
#include "TPIGlobal.h"
#include "PairWeight.h"
#include "ExpCF3D.h"
#include "ExpCF1D.h"
#include "ExpCFSH.h"
#include "ExpCFEP.h"
#include "SourceMonitor.h"

#include "PairReader.h"
#include "TChainProxy.h"

#include <TRandom2.h>

using namespace std;

extern double ktmin, ktmax;
extern double ptmin1, ptmax1, ptmin2, ptmax2;

//extern PARTICLE particle1, particle2, particle1lcms, particle2lcms, particle1prf, particle2prf;
extern ParticleCoor particle1, particle2, particle1lcms, particle2lcms, particle1prf, particle2prf;

extern double btmin, btmax;
extern int    pairtype;
extern int    partpid;
extern int    partpid2;
extern int    doresolution;

extern double mRL, mRS, mRO, mDT;
extern double mKO, mKS, mKL, mDE;
extern double mKOm, mKSm, mKLm, mDEm;
extern double deta, dphi;
extern double phiLow, phiUp;
extern double ev2weight;

extern double mKStarLong, mKStarOut, mKStarSide, mKStarSigned, mRLong, mDTime, mRTrans, mROut, mRSide, mRSidePairCMS, mRLongPairCMS, mDTimePairLCMS, mROutPairCMS, mDTimePairCMS, mRStar, mBetat;
extern double mKStarLongm, mKStarOutm, mKStarSidem, mKStarSignedm, mBetatm;
extern double mKT;
extern double mKTm;
extern int twospin;
extern double pionac;

//PARTICLE buf;
//PARTICLE **evbuf;
//PARTICLE **evbuf2;

ParticleCoor buf;
//ParticleCoor buf2;
ParticleCoor **evbuf;
ParticleCoor **evbuf2;

Int_t *evtscount;
Int_t *evtscount2;

char* fileName;

TRandom2 *mRand;


int main(int argc, char **argv)
{

  try
    {// BEGIN OF TRY
      Int_t evtomix = 20;
      Double_t tcut = TMAX;
      Int_t weakpid[16] = {3334, -3334, 3312, -3312, 3322, -3322,   3112,  -3112,   3212,  -3212,   3222,  -3222,  3122, -3122,  311, -311};
      Int_t isweak;

      if (argc > 5)
	evtomix = atoi(argv[5]);

      if (argc > 6)
	tcut = atof(argv[6]);

      partpid2 = 0;

      int nbin;
      int onlyprim = 0;
      int docoulomb = 0;
      int dotrue = 0;
      int doylm = 0;
      int dosourcemon = 0;
      doresolution = 1;

      if (argc > 1)
	nbin = atoi(argv[1]);
      else {
	cout << "Usage: tpi <kt bin> <only primaries> <do coulomb> <pair type> <events to mix> <maximum time> <do true> <do ylm> <monitor> <do resolution> <input file name>" << endl;
	cout << "Pair types: " << endl << "0 - pion-pion" << endl << "1 - kaon-kaon" << endl << "2 - pion-kaon" << endl << "3 - pion-kaon unlike-sign" << endl << "4 - pion-proton" << endl << "5 - pion-proton unlike-sign" << endl << "6 - proton - Sigma+" << endl << "7 - anti-proton - Sigma+" << endl << "8 - proton - Lambda (strong)" << endl << "9 - anti-proton - Lambda (strong)" << endl << "10 - Sigma+ - Sigma+" << endl << "11 - Sigma+ - anti-Sigma+" << endl << "12 - proton-proton" << endl<< "13 - kaon+ - proton" << endl << "16 - pion-pion unlike-sign" << endl << "17 - lambda-lambda" << endl << "18 - proton-xiZero" << endl << "19 - kaon-kaon unlike-sign" <<endl << "20 - proton-proton unlike-sign" << endl<< "21 - pion0-pion0" << endl << "22 - pion minus"  << endl << "23 - pion+ pion+ (two buffers)" <<endl;
	exit(0);
      }

      if (argc > 2) 
	onlyprim = atoi(argv[2]);

      if (argc > 3) 
	docoulomb = atoi(argv[3]);

      if (argc > 4)
	pairtype = atoi(argv[4]);

      if (argc > 7)
	dotrue = atoi(argv[7]);

      if (argc > 8)
	doylm = atoi(argv[8]);

      if (argc > 9)
	dosourcemon = atoi(argv[9]);

      if (argc > 10)
	doresolution = atoi(argv[10]);

      if (argc > 11)
	{
	  fileName = new char[strlen(argv[11])+1+7];
	  strcpy(fileName,argv[11]);
	}
 
      btmin = 0.0;
      btmax = 1.0;

      switch(nbin) {
      case 0:
	ktmin = 0.15;
	ktmax = 0.25;
	break;
      case 1:
	ktmin = 0.25;
	ktmax = 0.35;
	break;
      case 2:
	ktmin = 0.35;
	ktmax = 0.45;
	break;
      case 3:
	ktmin = 0.45;
	ktmax = 0.6;
	break;
      case 4:
	ktmin = 0.12;
	ktmax = 0.2;
	break;
      case 5:
	ktmin = 0.2;
	ktmax = 0.3;
	break;
      case 6:
	ktmin = 0.3;
	ktmax = 0.4;
	break;
      case 7:
	ktmin = 0.4;
	ktmax = 0.5;
	break;
      case 8:
	ktmin = 0.5;
	ktmax = 0.6;
	break;
      case 9:
	ktmin = 0.6;
	ktmax = 0.7;
	break;
      case 10:
	ktmin = 0.2;
	ktmax = 0.3;
	break;
      case 11:
	ktmin = 0.3;
	ktmax = 0.36;
	break;
      case 12:
	ktmin = 0.36;
	ktmax = 0.42;
	break;
      case 13:
	ktmin = 0.42;
	ktmax = 0.48;
	break;
      case 14:
	ktmin = 0.48;
	ktmax = 0.54;
	break;
      case 15:
	ktmin = 0.54;
	ktmax = 0.60;
	break;
      case 16:
	ktmin = 0.60;
	ktmax = 0.75;
	break;
      case 17:
	ktmin = 0.75;
	ktmax = 1.0;
	break;
      case 18:
	ktmin = 1.0;
	ktmax = 1.2;
	break;
      case 19:
	ktmin = 0.2;
	ktmax = 2.0;
	break;
      case 20:
	ktmin = 0.15;
	ktmax = 0.35;
	break;
      case 21:
	ktmin = 0.35;
	ktmax = 0.55;
	break;
      case 22:
	ktmin = 0.55;
	ktmax = 0.80;
	break;
      case 30:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.3;
	btmax = 0.95;
	break;
      case 31:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.35;
	btmax = 0.5;
	break;
      case 32:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.5;
	btmax = 0.65;
	break;
      case 33:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.65;
	btmax = 0.8;
	break;
      case 34:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.8;
	btmax = 0.95;
	break;
      case 35: // full beta_T range
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.35;
	btmax = 0.95;
	break;
      case 40:
	ktmin = 0.4;
	ktmax = 0.8;
	break;
      case 41:
	ktmin = 0.8;
	ktmax = 1.1;
	break;
      case 42:
	ktmin = 1.1;
	ktmax = 1.4;
	break;
      case 43:
	ktmin = 1.4;
	ktmax = 10.0;
	break;
      case 51:
	ktmin = 0.12;
	ktmax = 0.2;
	break;
      case 52:
	ktmin = 0.2;
	ktmax = 0.3;
	break;
      case 53:
	ktmin = 0.3;
	ktmax = 0.4;
	break;
      case 54:
	ktmin = 0.4;
	ktmax = 0.5;
	break;
      case 55:
	ktmin = 0.5;
	ktmax = 0.6;
	break;
      case 56:
	ktmin = 0.6;
	ktmax = 0.7;
	break;
      case 57:
	ktmin = 0.7;
	ktmax = 0.8;
	break;
      case 58:
	ktmin = 0.8;
	ktmax = 1.0;
	break;
      case 59:
	ktmin = 1.0;
	ktmax = 1.2;
	break;
      case 61:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.5;
	btmax = 0.9;
	break;
      case 71:
	ktmin = 0.5;
	ktmax = 2.5;
	btmin = 0.0;
	btmax = 1.0;
	break;
      case 81:
	ktmin = 0.1;
	ktmax = 2.5;
	btmin = 0.0;
	btmax = 1.0;
	break;
      case 91:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.65;
	btmax = 0.9;
	break;
      case 92:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.3;
	btmax = 0.99;
	break;
      case 93:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.3;
	btmax = 0.99;
	break;
      case 95:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.65;
	btmax = 0.9;
      case 101:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.65;
	btmax = 0.9;
	break;
      case 201:
	ktmin = 0.0;
	ktmax = 2.0;
	btmin = 0.0;
	btmax = 1.0;
	break;
      case 202:
	ktmin = 0.0;
	ktmax = 20.0;
	break;
      }

      TDatime dat;
      mRand = new TRandom2();
      mRand->SetSeed(dat.GetTime());

      switch (pairtype) {
      case 0:
      case 16:
      case 21:
      case 22:
      case 23:
	ptmin1 = PTMIN;
	ptmin2 = PTMIN;
	ptmax1 = PTMAX;
	ptmax2 = PTMAX;
	break;
      case 1:
      case 19:
	ptmin1 = PTMIN;
	ptmin2 = PTMIN;
	ptmax1 = PTMAX;
	ptmax2 = PTMAX;
	break;
      case 2:
      case 3:
	ptmin1 = 0.05;
	ptmin2 = 0.05;
	ptmax1 = PTMAX;
	ptmax2 = 1.5;
	break;
      case 4:
      case 5:
	ptmin1 = 0.05;
	ptmin2 = 0.3;
	ptmax1 = PTMAX;
	ptmax2 = 2.0;
	break;
      case 6:
      case 7:
	ptmin1 = 0.3;
	ptmin2 = 0.3;
	ptmax1 = 1.2;
	ptmax2 = 1.7;
	break;
      case 8:
      case 9:
	ptmin1 = 0.3;
	ptmin2 = 0.3;
	ptmax1 = 1.2;
	ptmax2 = 1.7;
	break;
      case 10:
      case 11:
	ptmin1 = 0.3;
	ptmin2 = 0.3;
	ptmax1 = 1.8;
	ptmax2 = 1.8;
	break;
      case 12:
      case 20:
	ptmin1 = PTMIN;
	ptmin2 = PTMIN;
	ptmax1 = PTMAX;
	ptmax2 = PTMAX;
	break;
      case 13:
	ptmin1 = 0.05;
	ptmin2 = 0.05;
	ptmax1 = 1.2;
	ptmax2 = 1.5;
	break;
      }

      if (nbin == 201 || ((nbin >= 30) && (nbin < 201))) {
	switch (pairtype) {
	case 2:
	case 3:
	  ptmin1 = 0.10;
	  ptmax1 = 1.5;
	  ptmin2 = 0.2;
	  ptmax2 = 2.0;
	  break;
	case 4:
	case 5:
	  ptmin1 = 0.10;
	  ptmax2 = 1.5;
	  ptmin2 = 0.5;
	  ptmax2 = 3.0;
	  break;
	case 8: // LUKASZ
	  ptmin1 = 0.5;
	  ptmin2 = 0.4;
	  ptmax1 = 200;
	  ptmax2 = 200;
	  break;
	case 9: // LUKASZ
	  ptmin1 = 0.3;
	  ptmin2 = 0.4;
	  ptmax1 = 200;
	  ptmax2 = 200;
	  break;
	case 13:
	case 14:
	  ptmin1 = 0.2;
	  ptmax1 = 2.0;
	  ptmin2 = 0.5;
	  ptmax2 = 3.0;
	  break;

	}
      }

      if (((pairtype == 2) || (pairtype == 3)) && (nbin ==91))
	{
	  ptmin1 = 0.11;
	  ptmax1 = 1.5;
	  ptmin2 = 0.2;
	  ptmax2 = 2.0;
	}

      if (((pairtype == 4) || (pairtype == 5)) && (nbin ==92))
	{
	  ptmin1 = 0.11;
	  ptmax1 = 1.5;
	  ptmin2 = 0.5;
	  ptmax2 = 3.0;
	}

      if (((pairtype == 13) || (pairtype == 13)) && (nbin ==93))
	{
	  ptmin1 = 0.2;
	  ptmax1 = 2.0;
	  ptmin2 = 0.5;
	  ptmax2 = 3.0;
	}

      if (((pairtype == 2) || (pairtype == 3)) && (nbin ==95))
	{
	  ptmin1 = 0.11;
	  ptmax1 = 0.9;
	  ptmin2 = 0.2;
	  ptmax2 = 1.5;
	}

      InitializeGamow();
      if (!docoulomb)
	pionac = 0;

      //      TPISourceMonitorInit();
      //if (!dosourcemon)
      //	TPISourceMonitorSetOff(kTRUE);

      //   evbuf = (PARTICLE **) malloc(sizeof(PARTICLE *) * evtomix);
      //   evtscount = (Int_t *) malloc(sizeof(Int_t) * evtomix);
      //   if (partpid2 != 0) {
      //     evbuf2 = (PARTICLE **) malloc(sizeof(PARTICLE *) * evtomix);
      //     evtscount2 = (Int_t *) malloc(sizeof(Int_t) * evtomix);
      //   }
      //   else {
      //     evbuf2 = evbuf;
      //     evtscount2 = evtscount;
      //   }

      //   for (int imix=0; imix<evtomix; imix++) {
      //     evbuf[imix] = (PARTICLE *) malloc(sizeof(PARTICLE) * 3000);
      //     evtscount[imix] = 0;
      //     if (partpid2 != 0) {
      //       evbuf2[imix] = (PARTICLE *) malloc(sizeof(PARTICLE) * 3000);
      //       evtscount2[imix] = 0;
      //     }
      //   }
 
      evbuf = (ParticleCoor **) malloc(sizeof(ParticleCoor *) * evtomix);
      evtscount = (Int_t *) malloc(sizeof(Int_t) * evtomix);
      if (partpid2 != 0) {
	evbuf2 = (ParticleCoor **) malloc(sizeof(ParticleCoor *) * evtomix);
	evtscount2 = (Int_t *) malloc(sizeof(Int_t) * evtomix);
      }
      else {
	evbuf2 = evbuf;
	evtscount2 = evtscount;
      }

      for (int imix=0; imix<evtomix; imix++) {
	evbuf[imix] = (ParticleCoor *) malloc(sizeof(ParticleCoor) * 3000);
	evtscount[imix] = 0;
	if (partpid2 != 0) {
	  evbuf2[imix] = (ParticleCoor *) malloc(sizeof(ParticleCoor) * 3000);
	  evtscount2[imix] = 0;
	}
      }

      PairReader *rpair = new PairReader("pairs.pp.root",PAIR_READER_WRITE);


      TH1D *hbetat;
      TH1D *hkt;
      TH1D *hevmult;
      TH1D *hevmultAll;
      TH2F *hevmultPID;
      
      TH1D *hevweight;
      TH1D *hevweightdensity;
      TH1D *hevweighttaken;

      TH1D *hpt;
      TH1D *heta;
      TH1D *hphiP;
      TH1D *hphiS;
      TH1D *hpid1;
      TH1D *hpid2;
      TH1D *hptTotal;

      ExpCF3D *cf3da;
      ExpCF3D *cf3dac;
      ExpCF3D *cf3datrue;
      ExpCF3D *cf3dactrue;

      char pairname[20];
      int pdgpart1 = 0;
      if(pairtype==0){
	strcpy(pairname,"PipPip"); pdgpart1=211;}
      else if(pairtype==16){
	strcpy(pairname,"PipPim"); pdgpart1=211;}
      else if(pairtype==22){
	strcpy(pairname,"PimPim"); pdgpart1=-211;}
      else if(pairtype==21){
	strcpy(pairname,"Pi0Pi0"); pdgpart1=111;}
      else if(pairtype==1){
	strcpy(pairname,"KpKp"); pdgpart1=321;}
      else if(pairtype==19){
	strcpy(pairname,"KpKm"); pdgpart1=321;}
      else  if(pairtype==12){
	strcpy(pairname,"PP"); pdgpart1=2212;}
      else  if(pairtype==20){
	strcpy(pairname,"PaP"); pdgpart1=2212;}
      else if(pairtype==17){
	strcpy(pairname,"LL"); pdgpart1=3122;}
      else
	strcpy(pairname,"NoName");

      if ((pairtype == 0) || (pairtype == 1) || (pairtype == 10) || (pairtype == 12)) {
	if (0) { // AuAu ranges
	  // 3D cf for identical particle - all - pure quantum weight 
	  cf3da  = new ExpCF3D("a", 81, -0.2025, 0.2025);
	  // 3D cf for identical particles - all - full weight
	  cf3dac = new ExpCF3D("ac", 81, -0.2025, 0.2025);
	}

	if (1) { // pp ranges 
	  // 3D cf for identical particle - all - pure quantum weight 
	  cf3da  = new ExpCF3D(Form("a%s",pairname), 81, -0.81, 0.81);
	  // 3D cf for identical particles - all - full weight
	  cf3dac = new ExpCF3D(Form("ac%s",pairname), 81, -0.81, 0.81);
      
	  // 3D cf for identical particle - all - pure quantum weight 
	  cf3datrue  = new ExpCF3D(Form("atrue%s",pairname), 81, -0.81, 0.81);
	  // 3D cf for identical particles - all - full weight
	  cf3dactrue = new ExpCF3D(Form("actrue%s",pairname), 81, -0.81, 0.81);
	}
      }
      else {
	// 3D cf for non-identical particle - all - pure quantum weight 
	cf3da  = new ExpCF3D(Form("a%s",pairname), 60, -0.2, 0.2);
	// 3D cf for non-identical particles - all - full weight
	cf3dac = new ExpCF3D(Form("ac%s",pairname), 60, -0.2, 0.2);    

	// 3D cf for non-identical particle - all - pure quantum weight 
	cf3datrue  = new ExpCF3D(Form("at%s",pairname), 60, -0.2, 0.2);
	// 3D cf for non-identical particles - all - full weight
	cf3dactrue = new ExpCF3D(Form("act%s",pairname), 60, -0.2, 0.2);    
      }

      ExpCF1D *cf1da = new ExpCF1D(Form("1da%s",pairname),200,0.0,0.8);
      ExpCF1D *cf1dp = new ExpCF1D(Form("1dp%s",pairname),200,0.0,0.8);
      ExpCF1D *cf1datrue = new ExpCF1D(Form("1datrue%s",pairname),200,0.0,0.8);
  
      hbetat = new TH1D(Form("hbetat%s",pairname), Form("hbetat%s",pairname), 100, 0.0, 1.0);
      hkt    = new TH1D(Form("hkt%s",pairname),    Form("hkt%s",pairname)  , 100, 0.0, 1.2);
      hevmult = new TH1D(Form("hevmult%s",pairname),Form("hevmult%s",pairname),1001,-0.5,1000.5);
      hevmultAll = new TH1D(Form("hevmultAll%s",pairname),Form("hevmultAll%s",pairname),1001,-0.5,1000.5);
      hevmultPID = new TH2F(Form("hevmultPID%s",pairname),Form("hevmultPID%s",pairname),11,0,11,51,-0.5,50.5);
      hevmultPID->GetXaxis()->SetBinLabel(1,"Pi+");
      hevmultPID->GetXaxis()->SetBinLabel(2,"Pi-");
      hevmultPID->GetXaxis()->SetBinLabel(3,"Pi0");
      hevmultPID->GetXaxis()->SetBinLabel(4,"K+");
      hevmultPID->GetXaxis()->SetBinLabel(5,"K-");
      hevmultPID->GetXaxis()->SetBinLabel(6,"P");
      hevmultPID->GetXaxis()->SetBinLabel(7,"AP");
      hevmultPID->GetXaxis()->SetBinLabel(8,"#Lambda");
      hevmultPID->GetXaxis()->SetBinLabel(9,"A#Lambda");
      hevmultPID->GetXaxis()->SetBinLabel(10,"K0+");
      hevmultPID->GetXaxis()->SetBinLabel(11,"K0-");
      heta = new TH1D(Form("heta%s",pairname),Form("heta%s",pairname),300,-3,3);
      hpt = new TH1D(Form("hpt%s",pairname),Form("hpt%i",pairtype),ptmax1*50,0,ptmax1);
      hptTotal = new TH1D(Form("hptTotal%s",pairname),Form("hptTotal%i",pairtype),ptmax1*50,0,ptmax1);
      hphiP = new TH1D(Form("hphiP%s",pairname),Form("hphiP%i",pairtype),200,-TMath::Pi(),TMath::Pi());
      hphiS = new TH1D(Form("hphiS%s",pairname),Form("hphiS%s",pairname),200,-TMath::Pi(),TMath::Pi());
      hpid1 = new TH1D(Form("hpid1%s",pairname),Form("hpid from GENBOD %s",pairname),6247,-3123,3123);
      hpid2 = new TH1D(Form("hpid2%s",pairname),Form("hpid for TPI %s",pairname),6247,-3123,3123);

      const int XBINS = 35; // orig = 80
      const double XMAX = 2.; // orig = 0.8

      // Spherical harmonics correlation function
      ExpCFSH *cylm = new ExpCFSH(Form("NonIdCYlm%s",pairname), XBINS, 0.0, XMAX);
      ExpCFSH *cylmtrue = new ExpCFSH(Form("NonIdCYlmTrue%s",pairname), XBINS, 0.0, XMAX);

      // Spherical harmonics correlation function
      ExpCFSH *cylml = new ExpCFSH(Form("IdLCYlm%s",pairname), XBINS, 0.0, XMAX);
      ExpCFSH *cylmltrue = new ExpCFSH(Form("IdLCYlmTrue%s",pairname), XBINS, 0.0, XMAX);

      // DetaDPhi correlation function
      ExpCFEP *cep = new ExpCFEP(Form("NonIdEP%s",pairname), XBINS, XBINS);
      ExpCFEP *ceptrue = new ExpCFEP(Form("NonIdEPTrue%s",pairname), XBINS, XBINS);
      ExpCFEP *cepNoQS = new ExpCFEP(Form("NonIdEPNoQS%s",pairname), XBINS, XBINS);
      ExpCFEP *ceptrueNoQS = new ExpCFEP(Form("NonIdEPTrueNoQS%s",pairname), XBINS, XBINS);
      phiLow = (-(int)(XBINS/4)+0.5)*2.*TMath::Pi()/XBINS;
      phiUp = 2*TMath::Pi()+(-(int)(XBINS/4)+0.5)*2.*TMath::Pi()/XBINS;

      // Turn the correlation functions on/off 
      // according to the switches
      if (onlyprim) {
	// cf3da->SetOff(kTRUE);
	// cf3dac->SetOff(kTRUE);
	// cf1da->SetOff(kTRUE);
	// cf3datrue->SetOff(kTRUE);
	// cf3dactrue->SetOff(kTRUE);
      }

      if (!docoulomb) {
	cf3dac->SetOff(kTRUE);
	cf3dactrue->SetOff(kTRUE);
      }

      if (!dotrue) {
	cf1datrue->SetOff(kTRUE);
	cylmtrue->SetOff(kTRUE);
	cylmltrue->SetOff(kTRUE);
	cf3datrue->SetOff(kTRUE);
	cf3dactrue->SetOff(kTRUE);
	ceptrue->SetOff(kTRUE);
	ceptrueNoQS->SetOff(kTRUE);
      }

      if (!doylm) {
	cylm->SetOff(kTRUE);
	cylmtrue->SetOff(kTRUE);
	cylml->SetOff(kTRUE);
	cylmltrue->SetOff(kTRUE);
      }
      if (doylm == 1) {
	//     cylm->SetOff(kTRUE);
	//     cylmtrue->SetOff(kTRUE);
	cylml->SetOff(kTRUE);
	cylmltrue->SetOff(kTRUE);
      }
      if (doylm == 2) {
	cylm->SetOff(kTRUE);
	cylmtrue->SetOff(kTRUE);
	//     cylml->SetOff(kTRUE);
	//     cylmltrue->SetOff(kTRUE);
      }

      TChainProxy *chn = new TChainProxy;
      
      //TTree *chnReplaced = new TTree();
      //chnReplaced->Branch("particle",&buf2,"mass/F:t:x:y:z:e:px:py:pz:eventweight/D:decayed/I:pid:fatherpid:rootpid:eid:fathereid:eventid/i");
      int pcount = 0;

      char fname[100];
      int nfile = 0;
      struct stat fbuf;
      ifstream ifile;
      ifile.open(fileName);
      //sprintf(fname, "event001.root");
      while( ifile>>fname )
	{
	  cout<<" Adding "<<fname<<endl;
	  chn->Add(fname);
	  //cout<<"  "<<chn->GetEntries();
	  nfile++;
	}
      ifile.close();

      Long64_t npart = chn->GetEntries();
      //it must be initialized
      chn->SetBranchAddress("particle",&buf);
      Int_t curev = -1;
      Int_t eviter = 0;
      Int_t eviterAll = 0;
      Double_t evweightSum = 0;
      Double_t pt, rap, peta;
      double coulombweight;
      double coulombweightNoQS;
      double quantumweight;
      double iscorrelated;
      int partcountev = 0;
      int partcountevAll = 0;
      int partcountPionsPlus = 0;
      int partcountPionsMinus = 0;
      int partcountPions0 = 0;
      int partcountKaonsPlus = 0;
      int partcountProtonsPlus = 0;
      int partcountKaonsMinus = 0;
      int partcountProtonsMinus = 0;
      int partcountLambdasPlus = 0;
      int partcountLambdasMinus = 0;
      int partcountKaon0Plus = 0;
      int partcountKaon0Minus = 0;
      double totalPt = 0;
      int sw = 0;

      /// STEP 1: calculate events number, create all histograms/arrays
      for (Long64_t iter=0; iter<npart; iter++)
	{
	  chn->GetEntry(iter);
	  if (buf.eventid != curev)
	    {
	      eviterAll++;
	      curev = buf.eventid;
	    }
	}
      Int_t* tEventStart = new Int_t[eviterAll];
      hevweight = new TH1D(Form("hevweight%s",pairname),Form("hevweight%s",pairname),eviterAll+1,-0.5,eviterAll+0.5);
      hevweightdensity = new TH1D(Form("hevweightdensity%s",pairname),Form("hevweightdensity%s",pairname),eviterAll+1,-0.5,eviterAll+0.5);
      hevweighttaken = new TH1D(Form("hevweighttaken%s",pairname),Form("hevweighttaken%s",pairname),eviterAll+1,-0.5,eviterAll+0.5);
      eviterAll =0;

      /// STEP 2: create cumulative distr. fnc., save event start point and length
    
      for (Long64_t iter=0; iter<npart; iter++)
	{
	  chn->GetEntry(iter);
	  if (buf.eventid != curev)
	    {
	      eviterAll++;
	      tEventStart[eviterAll-1]=iter;
	      evweightSum+=buf.eventweight;
	      hevweight->SetBinContent(eviterAll, evweightSum);
	      hevweightdensity->SetBinContent(eviterAll, buf.eventweight);
	      curev = buf.eventid;
	    }
	  hpid1->Fill(buf.pid);
	  }
        
      //cout<<"  mean multiplicity changed from  "<<chn->GetEntries()<<" to "<<chnReplaced->GetEntries()<<endl;
      //npart = chnReplaced->GetEntries();
     npart = chn->GetEntries();

      /// STEP 4: as before: cuts, mixing, filling hist.
      for (Long64_t iter=0; iter<npart; iter++) {
	chn->GetEntry(iter);
	//chnReplaced->GetEntry(iter);

	
	// if (dosourcemon) {
	//   if ((buf.pid == 211) || (buf.pid==-211)  || (buf.pid == 321) || (buf.pid==-321) || (buf.pid == 2212) || (buf.pid==-2212)) {
	//     peta = -TMath::Log(TMath::Tan(TMath::ATan2(pt, buf.pz)/2.0));
	//     //if (TMath::Abs(peta)<ETAABS)
	//   }
	// }

	if(buf.pid==pdgpart1) partcountev++;

	if(buf.pid == PIPID)
	  partcountPionsPlus++; 
	if(buf.pid == -PIPID)
	  partcountPionsMinus++;
	if(buf.pid == PI0PID)
	  partcountPions0++;
	if(buf.pid == KPID)
	  partcountKaonsPlus++;
	if(buf.pid == -KPID)
	  partcountKaonsMinus++;
	if(buf.pid == PPID)
	  partcountProtonsPlus++;
	if(buf.pid == PPID)
	  partcountProtonsMinus++;
	if(buf.pid == LAPID)
	  partcountLambdasPlus++;
	if(buf.pid == -LAPID)
	  partcountLambdasMinus++;
	if(buf.pid == K0PID)
	  partcountKaon0Plus++;
	if(buf.pid == -K0PID)
	  partcountKaon0Minus++;

	partcountevAll++;
	totalPt+=sqrt(buf.px*buf.px+buf.py*buf.py+buf.pz*buf.pz);
	hpid2->Fill(buf.pid);

	//    cout << "Found pid " << buf.pid << " " << buf.eventid << endl;
	if (((buf.pid == partpid) || ((buf.pid == partpid2) && (partpid2 !=0))) && (buf.t<tcut)) {
	  pt = hypot(buf.px, buf.py);
	  if (    (((pt>ptmin1) && (pt<ptmax1) && (buf.pid == partpid)) ||
		   ((pt>ptmin2) && (pt<ptmax2) && (buf.pid == partpid2) && (partpid2 != 0)))   &&
		  ((onlyprim == 0) || ((buf.fatherpid == partpid) || ((buf.fatherpid == partpid2) && (partpid2 != 0))))    ) {
	    rap = 0.5*log((buf.e+buf.pz)/(buf.e-buf.pz));
	    peta = -TMath::Log(TMath::Tan(TMath::ATan2(pt, buf.pz)/2.0));
	    hpt->Fill(pt);
	    heta->Fill(peta);
	    hphiS->Fill(buf.GetPhiS());
	    hphiP->Fill(buf.GetPhiP());

	    // Check if it is coming from a weak decay
	    isweak = 0;
	    for (int witer=0; witer<16; witer++) 
	      if (buf.fatherpid == weakpid[witer]) isweak = 1;
	       
	    //cout<<"checking eta"<<endl;
	    if ((fabs(rap) < ABSRAP) &&
		((fabs(peta) < ETAABS) || (nbin>19)) &&
		((!isweak) || (onlyprim == 2)))
	      {
		if (buf.pid == partpid) {
		  //      cout << "Found pid " << partpid << endl;
		  evbuf[eviter][evtscount[eviter]].x = buf.x;
		  evbuf[eviter][evtscount[eviter]].y = buf.y;
		  evbuf[eviter][evtscount[eviter]].z = buf.z;
		  evbuf[eviter][evtscount[eviter]].t = buf.t;
		  evbuf[eviter][evtscount[eviter]].px = buf.px;
		  evbuf[eviter][evtscount[eviter]].py = buf.py;
		  evbuf[eviter][evtscount[eviter]].pz = buf.pz;
		  evbuf[eviter][evtscount[eviter]].e = buf.e;
		  evbuf[eviter][evtscount[eviter]].pid = buf.pid;
		  evbuf[eviter][evtscount[eviter]].fatherpid = buf.fatherpid;
		  evbuf[eviter][evtscount[eviter]].rootpid = buf.rootpid;
		  evbuf[eviter][evtscount[eviter]].mass = buf.mass;
		  evbuf[eviter][evtscount[eviter]].eventid = buf.eventid;
		  evbuf[eviter][evtscount[eviter]].eventweight = buf.eventweight;
		  evtscount[eviter]++;
		}
		if ((buf.pid == partpid2) && (partpid2 != 0)) {
		  evbuf2[eviter][evtscount2[eviter]].x = buf.x;
		  evbuf2[eviter][evtscount2[eviter]].y = buf.y;
		  evbuf2[eviter][evtscount2[eviter]].z = buf.z;
		  evbuf2[eviter][evtscount2[eviter]].t = buf.t;
		  evbuf2[eviter][evtscount2[eviter]].px = buf.px;
		  evbuf2[eviter][evtscount2[eviter]].py = buf.py;
		  evbuf2[eviter][evtscount2[eviter]].pz = buf.pz;
		  evbuf2[eviter][evtscount2[eviter]].e = buf.e;
		  evbuf2[eviter][evtscount2[eviter]].pid = buf.pid;
		  evbuf2[eviter][evtscount2[eviter]].fatherpid = buf.fatherpid;
		  evbuf2[eviter][evtscount2[eviter]].rootpid = buf.rootpid;
		  evbuf2[eviter][evtscount2[eviter]].mass = buf.mass;
		  evbuf2[eviter][evtscount2[eviter]].eventid = buf.eventid;
		  evbuf2[eviter][evtscount2[eviter]].eventweight = buf.eventweight;
		  evtscount2[eviter]++;
		}
	      }
	  }
	}
	if (buf.eventid != curev) {
	  hevmult->Fill(partcountev);
	  hevmultAll->Fill(partcountevAll);

	  hevmultPID->Fill(0.5,partcountPionsPlus);
	  hevmultPID->Fill(1.5,partcountPionsMinus);
	  hevmultPID->Fill(2.5,partcountPions0);
	  hevmultPID->Fill(3.5,partcountKaonsPlus);
	  hevmultPID->Fill(4.5,partcountKaonsMinus);
	  hevmultPID->Fill(5.5,partcountProtonsPlus);
	  hevmultPID->Fill(6.5,partcountProtonsMinus);
	  hevmultPID->Fill(7.5,partcountLambdasPlus);
	  hevmultPID->Fill(8.5,partcountLambdasMinus);
	  hevmultPID->Fill(9.5,partcountKaon0Plus);
	  hevmultPID->Fill(10.5,partcountKaon0Minus);

	  hptTotal->Fill(totalPt);

	  partcountev = 0;
	  partcountevAll = 0;
	  partcountPionsPlus = 0;
	  partcountPionsMinus = 0;
	  partcountPions0 = 0;
	  partcountKaonsPlus = 0;
	  partcountKaonsMinus = 0;
	  partcountProtonsPlus = 0;
	  partcountProtonsMinus = 0;
	  partcountLambdasPlus =0;
	  partcountLambdasMinus = 0;
	  partcountKaon0Plus = 0;
	  partcountKaon0Minus=0;
	  totalPt = 0;

	  //	  cout << "Mix event with " << endl;
	  
	  if (iter>0) {
	    // Mix particles
	    for (int mixiter=0; mixiter<evtomix; mixiter++) {
	      for (int fiter=0; fiter<evtscount[eviter]; fiter++) {
		for (int siter=0; siter<evtscount2[mixiter]; siter++) {
		  if (((partpid2 == 0)||(partpid == partpid2))&& (mixiter==eviter) &&  (siter>=fiter) ) break;
		  PairKinematics(evbuf[eviter][fiter],
				 evbuf2[mixiter][siter]);
		  if ((mKTm>ktmin) && (mKTm<ktmax) ) {
		    if ( evbuf[eviter][0].eventid != evbuf2[mixiter][0].eventid )
		      {
			cf3da->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			if (partpid2 == 0)
			  cf1da->AddMixedPair(fabs(mKStarSignedm)*2.0, 1.0);
			else
			  cf1da->AddMixedPair(fabs(mKStarSignedm), 1.0, mKStarOutm>0);
			cylm->AddMixedPair(mKStarOutm, mKStarSidem, mKStarLongm, 1.0);
			cylml->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			//if(mixiter != eviter)
			cep->AddMixedPair(deta, dphi, 1.0 );
			cepNoQS->AddMixedPair(deta, dphi, 1.0 );

			if (fabs(mKStarSignedm)<0.3) {
			  rpair->WritePair(evbuf[eviter][fiter].px,
					   evbuf[eviter][fiter].py,
					   evbuf[eviter][fiter].pz,
					   evbuf[eviter][fiter].e,
					   evbuf2[mixiter][siter].px,
					   evbuf2[mixiter][siter].py,
					   evbuf2[mixiter][siter].pz,
					   evbuf2[mixiter][siter].e);
			}

			if (partpid2 != 0) {
			  hbetat->Fill(mBetatm);
			  hkt->Fill(mKTm);
			}
		      }
		    if (docoulomb) {
		      if ( evbuf[eviter][0].eventid != evbuf2[mixiter][0].eventid )
			{
			  cf3dac->AddMixedPair(mKOm, mKSm, mKLm, 1.0);

			  quantumweight = GetQuantum();
			  cf3da->AddRealPair(mKOm, mKSm, mKLm, quantumweight);

			  coulombweight = GetFull();
			  coulombweightNoQS = GetFullNoQS();

			  if (partpid2 == 0)
			    cf1da->AddRealPair(fabs(mKStarSignedm)*2.0, coulombweight);
			  else
			    cf1da->AddRealPair(fabs(mKStarSignedm), coulombweight, mKStarOutm>0);

			  cf3dac->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			  cylm->AddRealPair(mKStarOutm, mKStarSidem, mKStarLongm, coulombweight);
			  cylml->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			  cep->AddRealPair(deta, dphi, coulombweight );
			  cepNoQS->AddRealPair(deta, dphi, coulombweightNoQS );

			}
		      if (mixiter == eviter) {
			coulombweight = GetFull();
			coulombweightNoQS = GetFullNoQS();
			ceptrue->AddRealPair(deta, dphi, coulombweight);
			ceptrueNoQS->AddRealPair(deta, dphi, coulombweightNoQS);

			if (partpid2 == 0) {
			  if (sw) {
			    cylmtrue->AddRealPair(mKStarOutm, mKStarSidem, mKStarLongm, coulombweight);
			    cylmltrue->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			    cf3datrue->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			  }
			  else {
			    cylmtrue->AddRealPair(-mKStarOutm, -mKStarSidem, -mKStarLongm, coulombweight);
			    cylmltrue->AddRealPair(-mKOm, -mKSm, -mKLm, coulombweight);
			    cf3datrue->AddRealPair(-mKOm, -mKSm, -mKLm, coulombweight);
			  }
			  sw = !sw;
			}
			else {
			  cylmtrue->AddRealPair(mKStarOutm, mKStarSidem, mKStarLongm, coulombweight);
			  cylmltrue->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			  cf3datrue->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			}
			if (partpid2 == 0)
			  cf1datrue->AddRealPair(fabs(mKStarSignedm)*2.0, coulombweight);
			else
			  cf1datrue->AddRealPair(fabs(mKStarSignedm), coulombweight, mKStarOutm>0);

		      }
		      else {
                                 
			if ( evbuf[eviter][0].eventid != evbuf2[mixiter][0].eventid )
			  {
			    ceptrue->AddMixedPair(deta, dphi, 1.0 );
			    ceptrueNoQS->AddMixedPair(deta, dphi, 1.0 );
			    if (partpid2 == 0) {
			      if (sw) {
				cylmtrue->AddMixedPair(mKStarOutm, mKStarSidem, mKStarLongm, 1.0);
				cylmltrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
				cf3datrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			      }
			      else {
				cylmtrue->AddMixedPair(-mKStarOutm, -mKStarSidem, -mKStarLongm, 1.0);
				cylmltrue->AddMixedPair(-mKOm, -mKSm, -mKLm, 1.0);
				cf3datrue->AddMixedPair(-mKOm, -mKSm, -mKLm, 1.0);
			      }
			      sw = !sw;
			    }
			    else {
			      cylmtrue->AddMixedPair(mKStarOutm, mKStarSidem, mKStarLongm, 1.0);
			      cylmltrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			      cf3datrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			    }
			    if (partpid2 == 0)
			      cf1datrue->AddMixedPair(fabs(mKStarSignedm)*2.0, 1.0);
			    else
			      cf1datrue->AddMixedPair(fabs(mKStarSignedm), 1.0, mKStarOutm>0);
			  }}
		    }
		    else {
		      quantumweight = GetQuantum();
		      //quantumweight = 1.0;
		  
		      if ( evbuf[eviter][0].eventid != evbuf2[mixiter][0].eventid )
			{
			  if (partpid2 == 0)
			    cf1da->AddRealPair(fabs(mKStarSignedm)*2.0, quantumweight);
			  else
			    cf1da->AddRealPair(fabs(mKStarSignedm), quantumweight, mKStarOutm>0);

			  cf3da->AddRealPair(mKOm, mKSm, mKLm, quantumweight);
			  cylm->AddRealPair(mKStarOutm, mKStarSidem, mKStarLongm, quantumweight);
			  cylml->AddRealPair(mKOm, mKSm, mKLm, quantumweight);
			  //if(mixiter != eviter)
			  cep->AddRealPair(deta, dphi, quantumweight);
			  cepNoQS->AddRealPair(deta, dphi, 1.);
			}
		      if (mixiter == eviter) {
			ceptrue->AddRealPair(deta, dphi, quantumweight );
			ceptrueNoQS->AddRealPair(deta, dphi, 1. );
			if (partpid2 == 0) {
			  if (sw) {
			    cylmtrue->AddRealPair(mKStarOutm, mKStarSidem, mKStarLongm, quantumweight);
			    cylmltrue->AddRealPair(mKOm, mKSm, mKLm, quantumweight);
			    cf3datrue->AddRealPair(mKOm, mKSm, mKLm, quantumweight);
			  }
			  else {
			    cylmtrue->AddRealPair(-mKStarOutm, -mKStarSidem, -mKStarLongm, quantumweight);
			    cylmltrue->AddRealPair(-mKOm, -mKSm, -mKLm, quantumweight);
			    cf3datrue->AddRealPair(-mKOm, -mKSm, -mKLm, quantumweight);
			  }
			  sw = !sw;
			}
			else {
			  cylmtrue->AddRealPair(mKStarOutm, mKStarSidem, mKStarLongm, quantumweight);
			  cylmltrue->AddRealPair(mKOm, mKSm, mKLm, quantumweight);
			  cf3datrue->AddRealPair(mKOm, mKSm, mKLm, quantumweight);
			}
			if (partpid2 == 0)
			  cf1datrue->AddRealPair(fabs(mKStarSignedm)*2.0, quantumweight);
			else
			  cf1datrue->AddRealPair(fabs(mKStarSignedm), quantumweight, mKStarOutm>0);
		      }
		      else {
			if ( evbuf[eviter][0].eventid != evbuf2[mixiter][0].eventid )
			  {
			    ceptrue->AddMixedPair(deta, dphi, 1.0 );
			    ceptrueNoQS->AddMixedPair(deta, dphi, 1.0 );
			    if (partpid2 == 0) {
			      if (sw) {
				cylmtrue->AddMixedPair(mKStarOutm, mKStarSidem, mKStarLongm, 1.0);
				cylmltrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
				cf3datrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);	
			      }
			      else {
				cylmtrue->AddMixedPair(-mKStarOutm, -mKStarSidem, -mKStarLongm, 1.0);
				cylmltrue->AddMixedPair(-mKOm, -mKSm, -mKLm, 1.0);
				cf3datrue->AddMixedPair(-mKOm, -mKSm, -mKLm, 1.0);
			      }
			      sw = !sw;
			    }
			    else {
			      cylmtrue->AddMixedPair(mKStarOutm, mKStarSidem, mKStarLongm, 1.0);
			      cylmltrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			      cf3datrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			    }
			    if (partpid2 == 0)
			      cf1datrue->AddMixedPair(fabs(mKStarSignedm)*2.0, 1.0);
			    else
			      cf1datrue->AddMixedPair(fabs(mKStarSignedm), 1.0, mKStarOutm>0);
			  }
		      }
		    }
		  }
		}
	      }
	      if (partpid2 == 0) continue;
	      for (int fiter=0; fiter<evtscount[mixiter]; fiter++) {
		for (int siter=0; siter<evtscount2[eviter]; siter++) {
		  PairKinematics(evbuf[mixiter][fiter], 
				 evbuf2[eviter][siter]);
		  if ((mKTm>ktmin) && (mKTm<ktmax)) {
                           
		    if ( evbuf[eviter][0].eventid != evbuf2[mixiter][0].eventid )
		      {
			cf3dac->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			cf1da->AddMixedPair(fabs(mKStarSignedm), 1.0, mKStarOutm>0);
			cylm->AddMixedPair(mKStarOutm, mKStarSidem, mKStarLongm, 1.0);
			cylml->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			cylmtrue->AddMixedPair(mKStarOutm, mKStarSidem, mKStarLongm, 1.0);
			cylmltrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			cf3datrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			cep->AddMixedPair(deta, dphi, 1.0 );
			cepNoQS->AddMixedPair(deta, dphi, 1.0 );
			ceptrue->AddMixedPair(deta, dphi, 1.0 );
			ceptrueNoQS->AddMixedPair(deta, dphi, 1.0 );

			if (partpid2 != 0) {
			  hbetat->Fill(mBetatm);
			  hkt->Fill(mKTm);
			}
		      }
		    if (docoulomb) {
		      coulombweight = GetFull();
		      coulombweightNoQS = GetFullNoQS();
		  
		      if ( evbuf[eviter][0].eventid != evbuf2[mixiter][0].eventid )
			{
			  cf1da->AddRealPair(fabs(mKStarSignedm), coulombweight, mKStarOutm>0);
		  
			  cf3dac->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			  cylm->AddRealPair(mKStarOutm, mKStarSidem, mKStarLongm, coulombweight);
			  cylml->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			  cep->AddRealPair(deta, dphi, coulombweight );
			  cepNoQS->AddRealPair(deta, dphi, coulombweightNoQS );
			}
		      if (mixiter == eviter) {
			cylmtrue->AddRealPair(mKStarOutm, mKStarSidem, mKStarLongm, coulombweight);
			cylmltrue->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			cf3datrue->AddRealPair(mKOm, mKSm, mKLm, coulombweight);
			cf1datrue->AddRealPair(fabs(mKStarSignedm), coulombweight, mKStarOutm>0);
			ceptrue->AddRealPair(deta, dphi, coulombweight );
			ceptrueNoQS->AddRealPair(deta, dphi, coulombweightNoQS );
		      }
		      else {
                                 
			if ( evbuf[eviter][0].eventid == evbuf[mixiter][0].eventid )
			  {
			    cylmtrue->AddMixedPair(mKStarOutm, mKStarSidem, mKStarLongm, 1.0);
			    cylmltrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			    cf3datrue->AddMixedPair(mKOm, mKSm, mKLm, 1.0);
			    cf1datrue->AddMixedPair(fabs(mKStarSignedm), 1.0, mKStarOutm>0);
			    ceptrue->AddMixedPair(deta, dphi, 1.0 );
			    ceptrueNoQS->AddMixedPair(deta, dphi, 1.0 );
			  }
		      }
		    } // if docoulomb
		  } // if pT
		} // for siter
	      } // for fiter
	    } // for mixiter
	  }
        
	  curev = buf.eventid;
	  if (iter>0) {
	    eviter = (eviter + 1) % evtomix;
      
	    //cout << "Event: "<< curev << " particle 1 count " << evtscount[eviter] <<  endl;
	    evtscount[eviter] = 0;
	    if (partpid2 != 0) {
	      //cout << "Event: "<< curev << " particle 2 count " << evtscount2[eviter] <<  endl;
	      evtscount2[eviter] = 0;
	    }
	  }
	}
      }
      // cout << "[DBG]" << __FILE__ << " : " <<  __LINE__ << endl;
      char bufs[100];
      if (doresolution) {
	if (pairtype == 0)
	  sprintf(bufs, "outfilecf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 1)
	  sprintf(bufs, "outfilekkcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 2)
	  sprintf(bufs, "outfilepikcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 3)
	  sprintf(bufs, "outfilepikulcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 4)
	  sprintf(bufs, "outfilepipcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 5)
	  sprintf(bufs, "outfilepipulcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 6)
	  sprintf(bufs, "outfilepspulcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 7)
	  sprintf(bufs, "outfileapspulcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 8)
	  sprintf(bufs, "outfileplacf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 9)
	  sprintf(bufs, "outfileaplacf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 10)
	  sprintf(bufs, "outfilespspcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 11)
	  sprintf(bufs, "outfilespspulcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 12)
	  sprintf(bufs, "outfileppcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 13)
	  sprintf(bufs, "outfilekpcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 14)
	  sprintf(bufs, "outfilekpulcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 16)
	  sprintf(bufs, "outfilepipiulcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 19)
	  sprintf(bufs, "outfilekkulcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 20)
	  sprintf(bufs, "outfileppulcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 21)
	  sprintf(bufs, "outfilepi0cf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 22)
	  sprintf(bufs, "outfilepimcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 23)
	  sprintf(bufs, "outfilepipcf%i%s_mr.root", nbin, onlyprim ? "p" : "a");
      }
      else {
	if (pairtype == 0)
	  sprintf(bufs, "outfilecf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 1)
	  sprintf(bufs, "outfilekkcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 2)
	  sprintf(bufs, "outfilepikcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 3)
	  sprintf(bufs, "outfilepikulcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 4)
	  sprintf(bufs, "outfilepipcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 5)
	  sprintf(bufs, "outfilepipulcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 6)
	  sprintf(bufs, "outfilepspulcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 7)
	  sprintf(bufs, "outfileapspulcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 8)
	  sprintf(bufs, "outfileplacf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 9)
	  sprintf(bufs, "outfileaplacf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 10)
	  sprintf(bufs, "outfilespspcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 11)
	  sprintf(bufs, "outfilespspulcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 12)
	  sprintf(bufs, "outfileppcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 13)
	  sprintf(bufs, "outfilekpcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 14)
	  sprintf(bufs, "outfilekpulcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 16)
	  sprintf(bufs, "outfilepipiulcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 19)
	  sprintf(bufs, "outfilekkulcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 20)
	  sprintf(bufs, "outfileppulcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 21)
	  sprintf(bufs, "outfilepi0cf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 22)
	  sprintf(bufs, "outfilepimcf%i%s.root", nbin, onlyprim ? "p" : "a");
	else if (pairtype == 23)
	  sprintf(bufs, "outfilepipcf%i%s.root", nbin, onlyprim ? "p" : "a");
      }

      rpair->CloseFile();

      TFile *ofile = new TFile(bufs, "RECREATE");
      ofile->cd();
  
      cylm->Write();
      cylmtrue->Write();

      cylml->Write();
      cylmltrue->Write();

      cep->Write();
      ceptrue->Write();
      cepNoQS->Write();
      ceptrueNoQS->Write();

      cf1da->Write();
      cf1datrue->Write();

      cf3da->Write();
      cf3dac->Write();
  
      cf3datrue->Write();
      //  cf3dac->Write();
  
      hbetat->Write();
      hkt->Write();

      hpt->Write();
      heta->Write();
      hphiS->Write();
      hphiP->Write();
      hpid1->Write();
      hpid2->Write();
      hevmultAll->Write();
      hevmultPID->Write();
      hptTotal->Write();

      if (dosourcemon)
	hevmult->Write();

      hevweight->Write();
      hevweighttaken->Write();
      hevweightdensity->Write();

      //TPISourceMonitorWrite();

    }// END OF TRY
  catch(_Pair_Reader_Exception &exception)
    {
      switch(exception)
	{
	case PAIR_READER_EXCEPTION_END_OF_ENTRIES:
	  cerr << "[Pair Reader] !!! END OF ENTRIES !!!" << endl;
	  break;
	case PAIR_READER_EXCEPTION_NO_FILE:
	  cerr << "[Pair Reader] !!! NO FILE !!!" << endl;
	  break;
	case PAIR_READER_EXCEPTION_NO_NTUPLE:
	  cerr << "[Pair Reader] !!! NO NTUPLE !!!" << endl;
	  break;
	}
      return 0;
    }
}
