#include <TRandom2.h>
#include <TFile.h>
#include <TMath.h>
#include "CorrFctnDirectYlm.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
  double Ro = 1.5*TMath::Sqrt(2.0)/0.197327;
  double Rs = 1.3*TMath::Sqrt(2.0)/0.197327;
  double Rl = 2.1*TMath::Sqrt(2.0)/0.197327;

  CorrFctnDirectYlm *cylm = new CorrFctnDirectYlm("pitest",3,50,0.0,1.0);

  double qs, qo, ql, wq;
  double xs, xo, xl;

  TRandom2 *mRand = new TRandom2();


  for (int iter=0; iter<10000000; iter++) {
    qo = mRand->Rndm()*1.5 - 0.75;
    qs = mRand->Rndm()*1.5 - 0.75;
    ql = mRand->Rndm()*1.5 - 0.75;

    xo = mRand->Gaus(0.0, Ro);
    xs = mRand->Gaus(0.0, Rs);
    xl = mRand->Gaus(0.0, Rl);

    wq = 1.0+cos(qo*xo + qs*xs + ql*xl);

    cylm->AddRealPair(qo, qs, ql, wq);
    cylm->AddMixedPair(qo, qs, ql, 1.0);
    
    if ((iter-1)%100000 == 0) cout << iter/10000 << endl;
  }

  TFile *ofile = new TFile("pitest.root","RECREATE");
  ofile->cd();
  cylm->Write();
  
  return 0;
}
