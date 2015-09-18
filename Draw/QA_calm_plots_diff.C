int QA_calm_plots_diff(const char* filename1, const char* filename2, const char* type = "PipPip", const char* type2 = "PipPip", bool ifSave = false)
{
  
  TFile* f1 = new TFile(filename1,"READ");
 
  TH2F* hevmultPID = ((TH2F*) f1->Get(Form("hevmultPID%s",type)));
  TH1D* heta  =  ((TH1D*) f1->Get(Form("heta%s",type)));
  TH1D* hpt   =  ((TH1D*) f1->Get(Form("hpt%s",type)));
  TH1D* hphiS =  ((TH1D*) f1->Get(Form("hphiS%s",type)));
  TH1D* hphiP =  ((TH1D*) f1->Get(Form("hphiP%s",type)));
  TH1D* hpid1 =  ((TH1D*) f1->Get(Form("hpid1%s",type)));
  TH1D* hpid2 =  ((TH1D*) f1->Get(Form("hpid2%s",type)));
  TH1D* hevmultAll = ((TH1D*) f1->Get(Form("hevmultAll%s",type)));
  TH1D* hevmult = ((TH1D*) f1->Get(Form("hevmult%s",type)));
  TH1D* hevweight = ((TH1D*) f1->Get(Form("hevweight%s",type)));
  TH1D* hevweighttaken = ((TH1D*) f1->Get(Form("hevweighttaken%s",type)));
  TH1D* hevweightdensity= ((TH1D*) f1->Get(Form("hevweightdensity%s",type)));
  TH1D* hptTotal   =  ((TH1D*) f1->Get(Form("hptTotal%s",type)));


  TFile* f2 = new TFile(filename2,"READ");
  TH2F* hevmultPID2 = ((TH2F*) f2->Get(Form("hevmultPID%s",type2)));
  TH1D* heta2  =  ((TH1D*) f2->Get(Form("heta%s",type2)));
  TH1D* hpt2   =  ((TH1D*) f2->Get(Form("hpt%s",type2)));
  TH1D* hphiS2 =  ((TH1D*) f2->Get(Form("hphiS%s",type2)));
  TH1D* hphiP2 =  ((TH1D*) f2->Get(Form("hphiP%s",type2)));
  TH1D* hpid12 =  ((TH1D*) f2->Get(Form("hpid1%s",type2)));
  TH1D* hpid22 =  ((TH1D*) f2->Get(Form("hpid2%s",type2)));
  TH1D* hevmultAll2 = ((TH1D*) f2->Get(Form("hevmultAll%s",type2)));
  TH1D* hevmult2 = ((TH1D*) f2->Get(Form("hevmult%s",type2)));
  TH1D* hevweight2 = ((TH1D*) f2->Get(Form("hevweight%s",type2)));
  TH1D* hevweighttaken2 = ((TH1D*) f2->Get(Form("hevweighttaken%s",type2)));
  TH1D* hevweightdensity2= ((TH1D*) f2->Get(Form("hevweightdensity%s",type2)));
  TH1D* hptTotal2   =  ((TH1D*) f2->Get(Form("hptTotal%s",type)));


  int rebin = 2;

  double eta = 1.1;
  gStyle->SetOptStat(000);//111);

  const char* first = "";//Cons. laws only";
  const char* second = "";//Minijets";

  if(rebin>0)
    {
      heta->Rebin(rebin);
      hpt->Rebin(rebin);
      hphiS->Rebin(rebin);
      hphiP->Rebin(rebin);
      
      heta2->Rebin(rebin);
      hpt2->Rebin(rebin);
      hphiS2->Rebin(rebin);
      hphiP2->Rebin(rebin);  
    }

  heta2->SetLineColor(kGreen+2);
  hpt2->SetLineColor(kGreen+2);
  hphiS2->SetLineColor(kGreen+1);
  hphiP2->SetLineColor(kGreen+2);
  hpid12->SetLineColor(kGreen+2);
  hpid22->SetLineColor(kGreen+2);
  hevmultAll2->SetLineColor(kGreen+2);
  hevmult2->SetLineColor(kGreen+2);

  heta->Scale(1./heta->Integral());
  heta2->Scale(1./heta2->Integral());
  hpt->Scale(1./hpt->Integral());
  hpt2->Scale(1./hpt2->Integral());
  hevmult->Scale(1./hevmult->Integral());
  hevmult2->Scale(1./hevmult2->Integral());
  hevmultAll->Scale(1./hevmultAll->Integral());
  hevmultAll2->Scale(1./hevmultAll2->Integral());
  hpid1->Scale(1./hpid1->Integral());
  hpid2->Scale(1./hpid2->Integral());
  hpid12->Scale(1./hpid12->Integral());
  hpid22->Scale(1./hpid22->Integral());
  hphiP->Scale(1./hphiP->Integral());
  hphiS->Scale(1./hphiS->Integral());
  hphiP2->Scale(1./hphiP2->Integral());
  hphiS2->Scale(1./hphiS2->Integral());
  hptTotal->Scale(1./hptTotal->Integral());
  hptTotal2->Scale(1./hptTotal2->Integral());

  TCanvas* canv = new TCanvas("canv", "GENBOD results", 10,10,3*550,2*400);
  canv->Divide(3,3);

  canv->cd(1);
  heta->Draw();
  heta2->Draw("same");
  canv->cd(2);
  hpt->GetXaxis()->SetRangeUser(0,5);
  hpt->Draw();
  hpt2->Draw("same");

  canv->cd(3);
  hphiS->Draw();
  hphiP->SetLineColor(kRed);
  hphiP->Draw("same");
  hphiP2->Draw("same");
  hphiS2->Draw("same");

  canv->cd(4);
  hpid1->Draw();
  hpid12->Draw("same");
  canv->cd(5);
  hpid2->Draw();
  hpid22->Draw("same");

  canv->cd(6);
  hevmultAll->GetXaxis()->SetRangeUser(5,41);
  hevmultAll->Draw();
  hevmultAll2->Draw("same");

  canv->cd(7);
  hevmult->GetXaxis()->SetRangeUser(0,41);
  hevmult->Draw();
  hevmult2->Draw("same");

  canv->cd(8);
  hevweight->Draw();
  hevweight->Draw("same");

  canv->cd(9);
  //hevweightdensity->SetLineColor(kRed);
  //hevweightdensity->Draw();
  //hevweightdensity2->Draw("same");
  hptTotal->Draw();
  hptTotal2->SetLineColor(kGreen+2);
  hptTotal2->Draw("same");
  TF1* Ptot = new TF1("Ptot","4.33538e-02*TMath::Landau(x,3.24886e+00,2.17010e+00)*exp(8.34570e-03*x)",0,30);
  Ptot->Draw("same");

  TCanvas* canv2 = new TCanvas("canv2", "GENBOD results", 10,10,550,400);
  //canv->Divide(3,3);
  canv2->cd();
  hevmultPID->Draw("colz");

  double counter[11];
  double counttrue[] = {1.493,1.493, 1.493,0.183, 0.183,0.083,0.083, 0.048, 0.048,0.183,0.183};

  for(int i=1;i<12;i++)
      counter[i-1] = ((TH1F*)hevmultPID->ProjectionY("_py",i,i))->GetMean();


  for(int i=1;i<12;i++)
    {
      TLatex *part = new TLatex(0.072*i+0.05,0.7,Form("%.2f",counter[i-1]));
      part->SetNDC();
      //part->SetTextColor(kRed+2);
      part->SetTextFont(42);
      part->SetTextSize(0.04);
      part->SetLineWidth(2);
      part->Draw();

      TLatex *part1 = new TLatex(0.072*i+0.05,0.6,Form("%.1f%%",counter[i-1]/counter[0]*100.));
      cout<<Form("%.1f%%",counter[i-1]/counter[0]*100.)<<endl;
      part1->SetNDC();
      part1->SetTextColor(kRed+2);
      part1->SetTextFont(42);
      part1->SetTextSize(0.03);
      part1->SetLineWidth(2);
      part1->Draw();

      TLatex *part2 = new TLatex(0.072*i+0.05,0.5,Form("%.1f%%",counttrue[i-1]/counttrue[0]*100.));
      cout<<Form("%.1f%%",counttrue[i-1]/counttrue[0]*100.)<<endl;
      part2->SetNDC();
      part2->SetTextColor(kBlue+2);
      part2->SetTextFont(42);
      part2->SetTextSize(0.03);
      part2->SetLineWidth(2);
      part2->Draw();

    }
 }
