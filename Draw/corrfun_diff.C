int corrfun_diff(const char* filename1, const char* filename2, bool ifSave = false)
{
  char* type1="";
  char* type2="PipPip";

  TFile* f1 = new TFile(filename1,"READ");
  TFile* f2 = new TFile(filename2,"READ");

  TH2D* numF1 = ((TH2D*) f1->Get(Form("cnumepNonIdEP%s",type1)));
  TH2D* numF2 = ((TH2D*) f2->Get(Form("cnumepNonIdEP%s",type2)));
  TH2D* denF1 = ((TH2D*) f1->Get(Form("cdenepNonIdEP%s",type1)));
  TH2D* denF2 = ((TH2D*) f2->Get(Form("cdenepNonIdEP%s",type2)));
  TH2D* numB1 = ((TH2D*) f1->Get(Form("cnumepNonIdEPTrue%s",type1)));
  TH2D* numB2 = ((TH2D*) f2->Get(Form("cnumepNonIdEPTrue%s",type2)));
  TH2D* denB1 = ((TH2D*) numF1->Clone());
  TH2D* denB2 = ((TH2D*) numF2->Clone());
  TH2D* numC1 = ((TH2D*) numB1->Clone());
  TH2D* numC2 = ((TH2D*) numB2->Clone());
  TH2D* denC1 = ((TH2D*) f1->Get(Form("cdenepNonIdEPTrue%s",type1)));
  TH2D* denC2 = ((TH2D*) f2->Get(Form("cdenepNonIdEPTrue%s",type2)));

  double eta = 1.1;
  gStyle->SetOptStat(000);
  //TCanvas* canv = new TCanvas("canv", "GENBOD results", 10,10,800,600);
   TCanvas* canv = new TCanvas("canv", "GENBOD results", 10,10,800,400);
  canv->Divide(2,1);

  
  ////// Pure Bkg
  TH2D* numdiffTrue = (TH2D*)numB1->Clone();
  numdiffTrue->Divide(numB2);
  double sB1 = numB1->GetEntries()/denB1->GetEntries();
  numB1->Divide(denB1);
  numB1->Scale(1./sB1);
  numB1->GetYaxis()->SetRangeUser(-eta,eta);
  double sB2 = numB2->GetEntries()/denB2->GetEntries();
  numB2->Divide(denB2);
  numB2->Scale(1./sB2);
  numB2->GetYaxis()->SetRangeUser(-eta,eta);
  canv->cd(1);
  TH2D* corrB1 = (TH2D*) numB1->Clone();
  corrB1->Draw("surf1");
  corrB1->GetZaxis()->SetRangeUser(0.9,6);
  DrawNicely(corrB1, 0,0, "like-sign");
  canv->cd(2);
  TH2D* corrB2 = (TH2D*) numB2->Clone();
  corrB2->Draw("surf1");
  corrB2->GetZaxis()->SetRangeUser(0.9,6);
  DrawNicely(corrB2, 0,0, "unlike-sign");
  //canv->cd(6);
  //numB1->Divide(numB2);
  //DrawNicely(numB1, 0,0, "Background component");
  //numB1->DrawCopy("colz");
  
  //save
  if (ifSave)
  {
    canv->Print(Form("diff_%s_%s_global.png",filename1,filename2));
  }
}

void DrawNicely (TH2D *num, double minC=0, double maxC=0, char* title="")
{
	gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.15);
	gPad->SetBottomMargin(0.15);
	//gStyle->SetOptStat(0000);
	gStyle->SetLabelSize(0.06);

	const Int_t NRGBs = 5;
	const Int_t NCont = 50;

	Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
	Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
	Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
	Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);

	num->GetZaxis()->SetTitleSize(0.07);
	num->GetZaxis()->SetTitleOffset(1.05);
	num->GetYaxis()->SetTitleSize(0.07);
	num->GetXaxis()->SetTitleSize(0.07);
	num->GetXaxis()->SetLabelSize(0.06);
	num->GetYaxis()->SetLabelSize(0.06);
	num->GetZaxis()->SetLabelSize(0.06);
	num->GetYaxis()->SetTitleOffset(1.08);
	num->GetXaxis()->SetTitleOffset(1.08);
	num->GetXaxis()->SetNdivisions(505);
	num->GetYaxis()->SetNdivisions(505);
	num->GetZaxis()->SetNdivisions(505); 

	if(minC!=maxC) num->GetZaxis()->SetRangeUser(minC,maxC);
	num->GetXaxis()->SetTitle("#Delta#varphi");
	num->GetYaxis()->SetTitle("#Delta#eta");
	num->GetZaxis()->SetTitle("C(#Delta#eta,#Delta#varphi)");
	num->SetTitle(title);
	num->SetTitleSize(0.07);
}
