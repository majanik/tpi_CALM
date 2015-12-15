int corrfun(char* infile = "/media/wfpw/ElemExt1/CALM/FINAL_SEPT_MJ/tpi_Reggae/CALM_FinalSeptMJ_reggaeGlobal_10_18.root", char* type ="PipPim", bool ifSave = false)
{

  TFile* f1 = new TFile(infile,"READ");


  double eta = 1.5;
  gStyle->SetOptStat(000);
  TCanvas* canv = new TCanvas("canv", "GENBOD results", 10,10,600,400);

  TH2D* numF = ((TH2D*)f1->Get(Form("cnumepNonIdEP%s",type)))->Clone();
  TH2D* denF =  ((TH2D*) f1->Get(Form("cdenepNonIdEP%s",type)));


  ////// Pure Bkg
  TH2D* numB = ((TH2D*) f1->Get(Form("cnumepNonIdEPTrue%s",type)));
  TH2D* denB = ((TH2D*) numF->Clone());
  // numB->Rebin2D(2,2);
  // denB->Rebin2D(2,2);
  double sB = numB->GetEntries()/denB->GetEntries();
  numB->Divide(denB);
  numB->Scale(1./sB);
  numB->GetYaxis()->SetRangeUser(-eta,eta);
  numB->GetZaxis()->SetRangeUser(0.89,1.3);
  DrawNicely(numB, 0,0, "");
  numB->DrawCopy("surf1");
 
  //save
  // if (ifSave)
  // {
  //   char* name="";
  //   strncpy(name,_file0->GetName(),strlen(_file0->GetName())-0);
  //   canv->Print(Form("%s_global.png",name));
  //   canvND->Print(Form("%s_global_NumDen.png",name));
  // }
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
