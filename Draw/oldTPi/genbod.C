int genbod(bool ifSave = false)
{
  double eta = 1.1;
  gStyle->SetOptStat(111);
  TCanvas* canv = new TCanvas("canv", "GENBOD results", 10,10,1600,1200);
  canv->Divide(2,2);
  TCanvas* canvND = new TCanvas("canvND", "liczniki i mianowniki ", 10,10,1600,1200);
  canvND->Divide(2,2);
  //////   Pure Corr fun
  TH2D* numF = (TH2D*)cnumepNonIdEP->Clone();
  canvND->cd(1); numF->DrawCopy("surf1");
  TH2D* denF = (TH2D*)cdenepNonIdEP;
  // numF->Rebin2D(2,2);
  // denF->Rebin2D(2,2);
  canvND->cd(2); denF->DrawCopy("surf1");
  double sF = numF->GetEntries()/denF->GetEntries();
  numF->Divide(denF);
  numF->Scale(1./sF);
  numF->GetYaxis()->SetRangeUser(-eta,eta);
  // numF->GetZaxis()->SetRangeUser(0.97,1.2);
  canv->cd(1);
  DrawNicely(numF, 0,0, "Femtoscopic component");
  numF->Draw("surf1");
  
  ////// Pure Bkg
  TH2D* numB = (TH2D*)cnumepNonIdEPTrue->Clone();
  canvND->cd(3);
  TH2D* denB = (TH2D*)cnumepNonIdEP;
  // numB->Rebin2D(2,2);
  // denB->Rebin2D(2,2);
  numB->DrawCopy("surf1");
  double sB = numB->GetEntries()/denB->GetEntries();
  numB->Divide(denB);
  numB->Scale(1./sB);
  numB->GetYaxis()->SetRangeUser(-eta,eta);
  //numB->GetZaxis()->SetRangeUser(0.97,1.2);
  canv->cd(2);
  DrawNicely(numB, 0,0, "Background component");
  numB->DrawCopy("surf1");

  //////   Norm Corr fun
  TH2D* numC = (TH2D*)cnumepNonIdEPTrue;
  TH2D* denC = (TH2D*)cdenepNonIdEPTrue;
  // numC->Rebin2D(2,2);
  // denC->Rebin2D(2,2);
  canvND->cd(4); denC->DrawCopy("surf1");
  double sC = numC->GetEntries()/denC->GetEntries();
  numC->Divide(denC);
  numC->Scale(1./sC);
  numC->GetYaxis()->SetRangeUser(-eta,eta);
  // numC->GetZaxis()->SetRangeUser(0.97,1.2);
  canv->cd(3);
  DrawNicely(numC, 0,0, "Correlation function");
  numC->DrawCopy("surf1");
  
  //save
  if (ifSave)
  {
    char* name="";
    strncpy(name,_file0->GetName(),strlen(_file0->GetName())-0);
    canv->Print(Form("%s_global.png",name));
    canvND->Print(Form("%s_global_NumDen.png",name));
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
	num->GetZaxis()->SetTitle("C(#Delta#eta#Delta#phi)");
	num->SetTitle(title);
	num->SetTitleSize(0.07);
}
