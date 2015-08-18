int genbod_drawNice2(const char* filename1, const char* filename2, bool ifSave = false)
{
  TFile* f1 = new TFile(filename1,"READ");
  TFile* f2 = new TFile(filename2,"READ");

  TH2D* numF1 = ((TH2D*) f1->Get("cnumepNonIdEP"));
  TH2D* numF2 = ((TH2D*) f2->Get("cnumepNonIdEP"));
  TH2D* denF1 = ((TH2D*) f1->Get("cdenepNonIdEP"));
  TH2D* denF2 = ((TH2D*) f2->Get("cdenepNonIdEP"));
  TH2D* numB1 = ((TH2D*) f1->Get("cnumepNonIdEPTrue"));
  TH2D* numB2 = ((TH2D*) f2->Get("cnumepNonIdEPTrue"));
  TH2D* denB1 = ((TH2D*) numF1->Clone());
  TH2D* denB2 = ((TH2D*) numF2->Clone());
  TH2D* numC1 = ((TH2D*) numB1->Clone());
  TH2D* numC2 = ((TH2D*) numB2->Clone());
  TH2D* denC1 = ((TH2D*) f1->Get("cdenepNonIdEPTrue"));
  TH2D* denC2 = ((TH2D*) f2->Get("cdenepNonIdEPTrue"));

  double eta = 1.1;
  gStyle->SetOptStat(000);//111);

  int drawDifference = 0;

  const char* first = "";//Cons. laws only";
  const char* second = "";//Minijets";

  int drawWhich = 0; // 0 - cons. laws, 1 - femto, 2 - full corr fun

  //TCanvas* canv = new TCanvas("canv", "GENBOD results", 10,10,800,600);
  TCanvas* canv = new TCanvas("canv", "GENBOD results", 10,10,(2+drawDifference)*550,400);
  canv->Divide(2+drawDifference,1);

  //TCanvas* canvND = new TCanvas("canvND", "liczniki i mianowniki ", 10,10,800,600);
  // TCanvas* canvND = new TCanvas("canvND", "liczniki i mianowniki ", 10,10,1600,1200);
  // canvND->Divide(2,2);

  if(drawWhich==1){
    //////   Pure Corr fun
    TH2D* numdiff = (TH2D*)numF1->Clone();
    numdiff->Divide(numF2);
    //canvND->cd(1); numdiff->DrawCopy("colz");
    TH2D* dendiff = (TH2D*)denF1->Clone();
    dendiff->Divide(denF2);
    //canvND->cd(2); dendiff->DrawCopy("colz");
    double sF1 = numF1->GetEntries()/denF1->GetEntries();
    numF1->Divide(denF1);
    numF1->Scale(1./sF1);
    numF1->GetYaxis()->SetRangeUser(-eta,eta);
    double sF2 = numF2->GetEntries()/denF2->GetEntries();
    numF2->Divide(denF2);
    numF2->Scale(1./sF2);
    numF2->GetYaxis()->SetRangeUser(-eta,eta);
    canv->cd(1);
    TH2D* corrF1 = (TH2D*) numF1->Clone();
    corrF1->Draw("surf1");
    DrawNicely(corrF1, 0,0, filename1);
    canv->cd(2);
    TH2D* corrF2 = (TH2D*) numF2->Clone();
    corrF2->Draw("surf1");
    DrawNicely(corrF2, 0,0, filename2);
    if(drawDifference){
      canv->cd(3);
      numF1->Divide(numF2);
      DrawNicely(numF1, 0,0, "Femtoscopic component");
      numF1->Draw("colz");
    }
  }
  if(drawWhich==0){
    ////// Pure Bkg
    TH2D* numdiffTrue = (TH2D*)numB1->Clone();
    numdiffTrue->Divide(numB2);
    //canvND->cd(3); numdiffTrue->DrawCopy("colz");
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
    DrawNicely(corrB1, 0.7,2.4, first);//filename1);
    canv->cd(2);
    TH2D* corrB2 = (TH2D*) numB2->Clone();
    corrB2->Draw("surf1");
    DrawNicely(corrB2, 0.7,2.4, second);//filename2);
    if(drawDifference){
      canv->cd(3);
      numB1->Divide(numB2);
      DrawNicely(numB1, 0,0, "Background component");
      numB1->DrawCopy("colz");
    }
  }
  if(drawWhich==2){
    //////   Norm Corr fun
    TH2D* dendiffTrue = (TH2D*)denC1->Clone();
    dendiffTrue->Divide(denC2);
    //canvND->cd(4); dendiffTrue->DrawCopy("colz");
    double sC1 = numC1->GetEntries()/denC1->GetEntries();
    numC1->Divide(denC1);
    numC1->Scale(1./sC1);
    numC1->GetYaxis()->SetRangeUser(-eta,eta);
    double sC2 = numC2->GetEntries()/denC2->GetEntries();
    numC2->Divide(denC2);
    numC2->Scale(1./sC2);
    numC2->GetYaxis()->SetRangeUser(-eta,eta);
    canv->cd(1);
    TH2D* corrC1 = (TH2D*) numC1->Clone();
    corrC1->Draw("surf1");
    DrawNicely(corrC1, 0,0, first);//filename1);
    canv->cd(2);
    TH2D* corrC2 = (TH2D*) numC2->Clone();
    corrC2->Draw("surf1");
    DrawNicely(corrC2, 0,0, second);//filename2);
    if(drawDifference){
      canv->cd(3);
      numC1->Divide(numC2);
      DrawNicely(numC1, 0,0, "Correlation function");
      numC1->DrawCopy("colz");
    }
  }

  //save
  if (ifSave)
  {
    canv->Print(Form("diff_%s_%s_global.png",filename1,filename2));
    //canvND->Print(Form("diff_%s_%s_global_NumDen.png",filename1,filename2));
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
	num->GetZaxis()->SetTitle("C(#Delta#eta, #Delta#varphi)");
	num->SetTitle(title);
	num->SetTitleSize(0.07);
}
