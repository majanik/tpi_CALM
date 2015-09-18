int QA_calm_plots(const char* filename1, bool ifSave = false)
{
  
  TFile* f1 = new TFile(filename1,"READ");
 
  TH2F* hevmultPID = ((TH2F*) f1->Get("hevmultPID"));

  TH1D* heta  =  ((TH1D*) f1->Get("heta"));
  TH1D* hpt   =  ((TH1D*) f1->Get("hpt"));
  TH1D* hphiS =  ((TH1D*) f1->Get("hphiS"));
  TH1D* hphiP =  ((TH1D*) f1->Get("hphiP"));
  TH1D* hpid1 =  ((TH1D*) f1->Get("hpid1"));
  TH1D* hpid2 =  ((TH1D*) f1->Get("hpid2"));
  TH1D* hevmultAll = ((TH1D*) f1->Get("hevmultAll"));
  TH1D* hevmult = ((TH1D*) f1->Get("hevmult"));
  TH1D* hevweight = ((TH1D*) f1->Get("hevweight"));
  TH1D* hevweighttaken = ((TH1D*) f1->Get("hevweighttaken"));
  TH1D* hevweightdensity= ((TH1D*) f1->Get("hevweightdensity"));


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
      
      
    }

  heta->Scale(1./heta->Integral());

  TCanvas* canv = new TCanvas("canv", "GENBOD results", 10,10,3*550,2*400);
  canv->Divide(3,3);

  canv->cd(1);
  heta->Draw();

  canv->cd(2);
  hpt->GetXaxis()->SetRangeUser(0,5);
  hpt->Draw();

  canv->cd(3);
  hphiS->Draw();
  hphiP->SetLineColor(kRed);
  hphiP->Draw("same");

  canv->cd(4);
  hpid1->Draw();

  canv->cd(5);
  hpid2->Draw();

  canv->cd(6);
  hevmultAll->GetXaxis()->SetRangeUser(5,21);
  hevmultAll->Draw();

  canv->cd(7);
  hevmult->GetXaxis()->SetRangeUser(0,21);
  hevmult->Draw();

  canv->cd(8);
  hevweight->Draw();

  canv->cd(9);
  hevweightdensity->Draw();
  hevweightdensity->SetLineColor(kRed);
  hevweightdensity->Draw("same");


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
