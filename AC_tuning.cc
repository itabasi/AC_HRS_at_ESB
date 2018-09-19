#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
using namespace std;

#include "TApplication.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TFile.h"
#include "TLeaf.h"
#include "TTree.h"
#include "TCut.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TLine.h"
#include "TLatex.h"
#include "TText.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TProfile.h"
#include "TSystem.h"
#include "TColor.h"
#include "TPaveText.h"
#include "TMath.h"
#include "TGaxis.h"

#include "TRandom.h"
#include "ParamMan.hh"
#include "Setting.h"

#define Calibration

static const double PI = 4.0*atan(1.);
static const double mrad_to_deg = 1./1000*180./PI;
const double Mp = 938.272046;          // proton       mass (MeV/c2)
const double c = 0.299792458;          // speed of light in vacuum (m/ns)
//____________________________________________________________________________________________
void FitGaus(TH1F *h, double &gamin, double &gamax){
	 //gamin = h  ->GetBinCenter(h ->GetMaximumBin())-20.0;
	 //gamax = h  ->GetBinCenter(h ->GetMaximumBin())+20.0;
	for(Int_t l=0; l<10; l++){
	TF1 *ga = new TF1("ga","gaus");
      h  ->Fit(ga,"0QR","",gamin,gamax);
      gamin = ga->GetParameter(1) - ga->GetParameter(2)*1.5;
      gamax = ga->GetParameter(1) + ga->GetParameter(2)*1.5;
      ga->Clear();
	}
 return;
 }

///////////////////////////////////////
class AC_ana
{
 public:
         AC_ana();
        ~AC_ana();
    void SetOutputName(string ofname);
    void SetParamFile(string paramname);
    void SetRoot(string ifname);
    void GetHist();
    void fit();
    void draw(); 
    void savecanvas(); 

  private:
    ParamMan *paramMan;
    TFile *ifp;
    Setting *set;
    string output_name;
    string input_param;
    string output_param;

    TH1F *h_NPE_ACu[12], *h_NPE_ACb[12];

    TLine *line;
    int run_num;
    TCanvas *c1,*c2,*c3,*c4,*c5;
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
AC_ana::AC_ana()
{

  gErrorIgnoreLevel = kError;
  gROOT->SetStyle("Plain");
  gROOT->SetBatch(1);

  gStyle->SetOptDate(0);
  gStyle->SetOptFit(1);
  gStyle->SetHistFillStyle(3002);
  gStyle->SetHistFillColor(0);
  gStyle->SetGridWidth(0.5);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetLineWidth(1);
  gStyle->SetOptDate(0);
//  gStyle->SetStatW(0.15);
  gStyle->SetStatFontSize(0.03);
  gStyle->SetStatTextColor(1);
  gStyle->SetTitleX(0.15);
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetTitleTextColor(1);
  gStyle->SetNdivisions(510); // tertiary*10000 + secondary*100 + first
  gStyle->SetOptStat("iMen");
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.13);

  //const Int_t NRGBs = 5;
  //const Int_t NCont = 255;
  //Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  //Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  //Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  //Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  //TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  //gStyle->SetNumberContours(NCont);
      


  line = new TLine(0,0,0,0);
  line ->SetLineColor(6);
  line ->SetLineWidth(1.0);
  line ->SetLineStyle(1);

  set = new Setting();
  c1= new TCanvas("c1","c1",1400,800 );
  c2= new TCanvas("c2","c2",1400,800 );
  c3= new TCanvas("c3","c3",1400,800 );
  c4= new TCanvas("c4","c4",1400,800 );
  c5= new TCanvas("c5","c5",1400,800 );

}
////////////////////////////////////////////////////////////////////////////
AC_ana::~AC_ana(){
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::SetRoot(string ifname){
  ifp = new TFile(Form("%s",ifname.c_str()),"readonly");
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::SetParamFile(string paramname){
  input_param = paramname;
  output_param = paramname;
  output_param.append("_tuned");
  cout<<"input param name: "<<input_param<<endl;
  paramMan= new ParamMan(input_param.c_str());
  paramMan->Initialize();
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::SetOutputName(string ofname){
  output_name = ofname;
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::GetHist(){

  for(int i=0;i<12;i++){
    h_NPE_ACu[i] = (TH1F*)ifp->Get(Form("h_NPE_ACu%d",i+1));
    h_NPE_ACb[i] = (TH1F*)ifp->Get(Form("h_NPE_ACb%d",i+1));
  }
  
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::fit(){
  double gamin_u,gamax_u;
  double gamin_b,gamax_b;
  double min=0.5,max=1.5;
    for(int i=0;i<12;i++){
      gamin_u = gamin_b = 0.5;
      gamax_u = gamax_b = 1.5;
      TF1 *g1 = new TF1("g1","gaus",min,max);
      set->SetTF1(g1,2,1,1);
      FitGaus(h_NPE_ACu[i],gamin_u,gamax_u);
      h_NPE_ACu[i]->Fit(g1,"QR","",gamin_u,gamax_u);
      paramMan ->SetNpeTune(1,i+1,0,g1->GetParameter(1));

      TF1 *g2 = new TF1("g2","gaus",min,max);
      set->SetTF1(g2,2,1,1);
      FitGaus(h_NPE_ACb[i],gamin_b,gamax_b);
      h_NPE_ACb[i]->Fit(g2,"QR","",gamin_b,gamax_b);
      paramMan ->SetNpeTune(1,i+1,1,g2->GetParameter(1));
    }

    paramMan ->WriteToFile(output_param.c_str());

}
////////////////////////////////////////////////////////////////////////////
void AC_ana::draw(){

  c1->Clear();
  c1->Divide(4,6);
  for(int i=0;i<12;i++){
    c1->cd(i+1);  gPad->SetLogy(1);h_NPE_ACu[i]->Draw();
                 line->DrawLine(1,0.5,1,h_NPE_ACu[i]->GetBinContent(h_NPE_ACu[i]->FindBin(1)));
    c1->cd(i+13); gPad->SetLogy(1);h_NPE_ACb[i]->Draw();
                 line->DrawLine(1,0.5,1,h_NPE_ACb[i]->GetBinContent(h_NPE_ACb[i]->FindBin(1)));
  }

}
////////////////////////////////////////////////////////////////////////////
void AC_ana::savecanvas(){
  string ofname_pdf = output_name;
  //ofname_pdf.erase(ofname_pdf.size()-5);
  //ofname_pdf.append(".pdf");
  c1->Print(Form("%s[",ofname_pdf.c_str()));
  c1->Print(Form("%s" ,ofname_pdf.c_str()));
  c1->Print(Form("%s]",ofname_pdf.c_str()));
  cout<<ofname_pdf.c_str()<<" saved"<<endl;
}
////////////////////////////////////////////////////////////////////////////
//////////////////////////// main //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv){

  string ifname = "output0000.dat";
  string ofname = "pdf/test.pdf";
  int ch;
  int MaxNum = 0;
  bool output_flag = false;
  bool output_tree_flag = false;
  bool draw_flag = true;
  bool coin_flag = false;
  string paramname="param/default.param";
  extern char *optarg;
  while((ch=getopt(argc,argv,"hf:w:n:p:bco"))!=-1){
    switch(ch){
    case 'f':
      ifname = optarg;
      cout<<"input filename : "<<ifname<<endl;
      break;
    case 'w':
      output_flag = true;
      draw_flag = false;
      ofname = optarg;
      cout<<"output filename : "<<ofname<<endl;
      break;
    case 'n':
      MaxNum = atoi(optarg);
      break;
    case 'c':
      coin_flag = true;
      break;
    case 'b':
      draw_flag = false;
      cout<<"BACH MODE!"<<endl;
      break;
    case 'p':
      paramname = optarg;
      break;
    case 'h':
      cout<<"-f : input root filename"<<endl;
      cout<<"-w : output pdf filename"<<endl;
      cout<<"-n : maximum number of analysed events"<<endl;
      cout<<"-p : param file"<<endl;
      return 0;
      break;
    case '?':
      cout<<"unknown option...."<<endl;
      return 0;
      break;
    default:
      cout<<"type -h to see help!!"<<endl;
      return 0;
    }
  }

  TApplication *theApp = new TApplication("App", &argc, argv);
  AC_ana *ana = new AC_ana();

  ana->SetParamFile(paramname);
  ana->SetRoot(ifname);
  ana->SetOutputName(ofname);
  ana->GetHist();
  ana->fit();
  ana->draw();
  ana->savecanvas();
  delete ana;

  gSystem->Exit(1);
  theApp->Run();
  return 0;
}

