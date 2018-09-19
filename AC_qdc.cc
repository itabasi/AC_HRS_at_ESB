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

#define Calibration

static const double PI = 4.0*atan(1.);
static const double mrad_to_deg = 1./1000*180./PI;
const double Mp = 938.272046;          // proton       mass (MeV/c2)
const double c = 0.299792458;          // speed of light in vacuum (m/ns)

const int NAC   = 20;  // No. of AC 

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SetTH1(TH1 *h, TString name, TString xname, TString yname, int LColor=1, int FStyle=0, int FColor=0){
  h->SetTitle(name);
  h->SetLineColor(LColor);
  h->SetLineWidth(0);
  h->SetFillStyle(FStyle);
  h->SetFillColor(FColor);
  h->SetMinimum(0.8);

  h->GetXaxis()->SetTitle(xname);
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitleFont(42);
  h->GetXaxis()->SetTitleOffset(1.0);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetLabelFont(42);
  h->GetXaxis()->SetLabelOffset(0.01);

  h->GetYaxis()->SetTitle(yname);
  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitleFont(42);
  h->GetYaxis()->SetTitleOffset(0.8);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetLabelFont(42);
  h->GetYaxis()->SetLabelOffset(0.01);
  ((TGaxis*)h->GetYaxis())->SetMaxDigits(3);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void SetTH2(TH2 *h, TString name, TString xname, TString yname, double min=0.8, double MStyle=1, double MSize=1.0){
  h->SetTitle(name);
  h->SetMinimum(min);
  h->SetLineWidth(0);
  h->SetTitleSize(0.05,"");
  h->SetMarkerStyle(1);
  h->SetMarkerSize(0.1);
  h->SetMarkerColor(1);

  h->GetXaxis()->SetTitle(xname);
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetTitleFont(42);
  h->GetXaxis()->SetTitleOffset(1.0);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetXaxis()->SetLabelFont(42);
  h->GetXaxis()->SetLabelOffset(0.01);

  h->GetYaxis()->SetTitle(yname);
  h->GetYaxis()->CenterTitle();
  h->GetYaxis()->SetTitleFont(42);
  h->GetYaxis()->SetTitleOffset(0.7);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetLabelFont(42);
  h->GetYaxis()->SetLabelOffset(0.01);
  ((TGaxis*)h->GetYaxis())->SetMaxDigits(3);
}

void SetGrErr(TGraphErrors *gr, TString hname, TString xname, TString yname, int LColor, int MColor, int MStyle, double Yoffset, double min, double max){
  gr->SetTitle(hname);
  gr->SetName(hname);
  gr->GetXaxis()->SetTitle(xname);
  gr->GetXaxis()->CenterTitle();
  gr->GetYaxis()->SetTitle(yname);
  gr->GetYaxis()->CenterTitle();
  gr->SetLineColor(LColor);
  gr->SetMarkerStyle(MStyle);
  gr->SetMarkerColor(MColor);
  gr->SetMarkerSize(0.5);
  gr->GetYaxis()->SetTitleOffset(Yoffset);
//  gr->GetYaxis()->SetRangeUser(min,max);
}

////////////////////////////////////////////////////////////////////////////
//////////////////////////// main //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv){

  string ifname = "output0000.dat";
  string ofname = "root/hoge.root";
  int ch;
  int MaxNum = 0;
  bool output_flag = false;
  bool output_tree_flag = false;
  bool draw_flag = true;
  bool coin_flag = false;
  string pngname;
  extern char *optarg;
  while((ch=getopt(argc,argv,"hf:w:n:bco"))!=-1){
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
    case 'h':
      cout<<"-f : input root filename"<<endl;
      cout<<"-w : output txt filename"<<endl;
      cout<<"-n : maximum number of analysed events"<<endl;
      cout<<"-p : print png file"<<endl;
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

  //TApplication theApp("App", &argc, argv);
  TApplication *theApp = new TApplication("App", &argc, argv);

  time_t start, end;
  start = time(NULL);
  time(&start);

  gErrorIgnoreLevel = kError;
  gROOT->SetStyle("Plain");
  if(!draw_flag) gROOT->SetBatch(1);

  gStyle->SetOptDate(0);
  gStyle->SetOptFit(1);
  gStyle->SetHistFillStyle(3002);
  gStyle->SetHistFillColor(0);
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetFrameLineWidth(0);
  gStyle->SetLineWidth(0);
  gStyle->SetOptDate(0);
  gStyle->SetOptStat("ei");
//  gStyle->SetStatW(0.15);
  gStyle->SetStatFontSize(0.03);
  gStyle->SetStatTextColor(1);
  gStyle->SetTitleX(0.15);
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetTitleTextColor(1);

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
      
  gStyle->SetGridWidth(0);
  gStyle->SetFrameLineWidth(0);
  gStyle->SetLineWidth(0);
  gStyle->SetNdivisions(510); // tertiary*10000 + secondary*100 + first

  TFile *ifp = new TFile(Form("%s",ifname.c_str() ) );
  TTree *tree = (TTree*)ifp->Get("T");

  TFile *ofp = new TFile();
  if(output_flag){ ofp = new TFile(Form("%s",ofname.c_str()),"recreate"); }

  //////////////
  //SetBraches//
  //////////////
  int ENum;
  ENum = tree->GetEntries();
  if(MaxNum>0 && MaxNum<ENum) ENum = MaxNum;
  int Ndata00,Ndata01,Ndata02,Ndata03,Ndata04,Ndata05,Ndata06,Ndata07,Ndata08,Ndata09;
  int Ndata10,Ndata11,Ndata12,Ndata13,Ndata14,Ndata15,Ndata16,Ndata17,Ndata18,Ndata19;
  int Ndata20,Ndata21,Ndata22,Ndata23,Ndata24,Ndata25,Ndata26,Ndata27,Ndata28,Ndata29;
  int Ndata30,Ndata31;
  double raw00[60],raw01[60],raw02[60],raw03[60],raw04[60],raw05[60],raw06[60],raw07[60],raw08[60],raw09[60];
  double raw10[60],raw11[60],raw12[60],raw13[60],raw14[60],raw15[60],raw16[60],raw17[60],raw18[60],raw19[60];
  double raw20[60],raw21[60],raw22[60],raw23[60],raw24[60],raw25[60],raw26[60],raw27[60],raw28[60],raw29[60];
  double raw30[60],raw31[60];
  double sum_raw[32];
  //tree->SetBranchStatus("*"             ,0);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw00"       ,&Ndata00);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw01"       ,&Ndata01);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw02"       ,&Ndata02);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw03"       ,&Ndata03);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw04"       ,&Ndata04);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw05"       ,&Ndata05);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw06"       ,&Ndata06);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw07"       ,&Ndata07);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw08"       ,&Ndata08);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw09"       ,&Ndata09);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw10"       ,&Ndata10);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw11"       ,&Ndata11);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw12"       ,&Ndata12);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw13"       ,&Ndata13);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw14"       ,&Ndata14);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw15"       ,&Ndata15);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw16"       ,&Ndata16);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw17"       ,&Ndata17);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw18"       ,&Ndata18);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw19"       ,&Ndata19);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw20"       ,&Ndata20);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw21"       ,&Ndata21);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw22"       ,&Ndata22);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw23"       ,&Ndata23);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw24"       ,&Ndata24);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw25"       ,&Ndata25);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw26"       ,&Ndata26);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw27"       ,&Ndata27);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw28"       ,&Ndata28);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw29"       ,&Ndata29);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw30"       ,&Ndata30);
  tree->SetBranchAddress("Ndata.sbs.hcal.a.raw31"       ,&Ndata31);
  tree->SetBranchAddress("sbs.hcal.a.raw00"             ,raw00);
  tree->SetBranchAddress("sbs.hcal.a.raw01"             ,raw01);
  tree->SetBranchAddress("sbs.hcal.a.raw02"             ,raw02);
  tree->SetBranchAddress("sbs.hcal.a.raw03"             ,raw03);
  tree->SetBranchAddress("sbs.hcal.a.raw04"             ,raw04);
  tree->SetBranchAddress("sbs.hcal.a.raw05"             ,raw05);
  tree->SetBranchAddress("sbs.hcal.a.raw06"             ,raw06);
  tree->SetBranchAddress("sbs.hcal.a.raw07"             ,raw07);
  tree->SetBranchAddress("sbs.hcal.a.raw08"             ,raw08);
  tree->SetBranchAddress("sbs.hcal.a.raw09"             ,raw09);
  tree->SetBranchAddress("sbs.hcal.a.raw10"             ,raw10);
  tree->SetBranchAddress("sbs.hcal.a.raw11"             ,raw11);
  tree->SetBranchAddress("sbs.hcal.a.raw12"             ,raw12);
  tree->SetBranchAddress("sbs.hcal.a.raw13"             ,raw13);
  tree->SetBranchAddress("sbs.hcal.a.raw14"             ,raw14);
  tree->SetBranchAddress("sbs.hcal.a.raw15"             ,raw15);
  tree->SetBranchAddress("sbs.hcal.a.raw16"             ,raw16);
  tree->SetBranchAddress("sbs.hcal.a.raw17"             ,raw17);
  tree->SetBranchAddress("sbs.hcal.a.raw18"             ,raw18);
  tree->SetBranchAddress("sbs.hcal.a.raw19"             ,raw19);
  tree->SetBranchAddress("sbs.hcal.a.raw20"             ,raw20);
  tree->SetBranchAddress("sbs.hcal.a.raw21"             ,raw21);
  tree->SetBranchAddress("sbs.hcal.a.raw22"             ,raw22);
  tree->SetBranchAddress("sbs.hcal.a.raw23"             ,raw23);
  tree->SetBranchAddress("sbs.hcal.a.raw24"             ,raw24);
  tree->SetBranchAddress("sbs.hcal.a.raw25"             ,raw25);
  tree->SetBranchAddress("sbs.hcal.a.raw26"             ,raw26);
  tree->SetBranchAddress("sbs.hcal.a.raw27"             ,raw27);
  tree->SetBranchAddress("sbs.hcal.a.raw28"             ,raw28);
  tree->SetBranchAddress("sbs.hcal.a.raw29"             ,raw29);
  tree->SetBranchAddress("sbs.hcal.a.raw30"             ,raw30);
  tree->SetBranchAddress("sbs.hcal.a.raw31"             ,raw31);

  gStyle->SetOptStat("iMen");
  //////////////
  //histograms//
  //////////////
  TH1F *h_raw[32],*h_sum_raw[32];
  for(int i=0;i<32;i++){
    h_raw[i]     = new TH1F(Form("h_raw%02d",i)    ,Form("h_raw%02d",i)    ,10001,-1,10000);
    h_sum_raw[i] = new TH1F(Form("h_sum_raw%02d",i),Form("h_sum_raw%02d",i),1501,5000,15000);
    SetTH1(h_raw[i]      , Form("Raw%d",i+1)        , "Raw data[ch]", "counts", 1, 3000, 0);
    SetTH1(h_sum_raw[i]  , Form("RawSum%d(QDC)",i+1),      "Sum[ch]", "counts", 1, 3000, 0);
  }
////////////////////////////////////////////////////////////////////
//////////////       Fill
////////////////////////////////////////////////////////////////////
  // event for loop
  for(int n=0;n<ENum;n++){
    tree->GetEntry(n);

    if(n%10000==0){
     char clout[100];
     end = time(NULL);
     time(&end);
     sprintf(clout,"%.0f sec",difftime(end,start));
     cout<<n<<" / "<<ENum<<" : "<<clout<<endl;
    } // if n%100000==0 std::cout

    for(int i=0;i<32;i++){
      sum_raw[i]=0;
    }

    for(int i=0;i<60;i++){
      h_raw[0] ->Fill(raw00[i]);
      h_raw[1] ->Fill(raw01[i]);
      h_raw[2] ->Fill(raw02[i]);
      h_raw[3] ->Fill(raw03[i]);
      h_raw[4] ->Fill(raw04[i]);
      h_raw[5] ->Fill(raw05[i]);
      h_raw[6] ->Fill(raw06[i]);
      h_raw[7] ->Fill(raw07[i]);
      h_raw[8] ->Fill(raw08[i]);
      h_raw[9] ->Fill(raw09[i]);
      h_raw[10]->Fill(raw10[i]);
      h_raw[11]->Fill(raw11[i]);
      h_raw[12]->Fill(raw12[i]);
      h_raw[13]->Fill(raw13[i]);
      h_raw[14]->Fill(raw14[i]);
      h_raw[15]->Fill(raw15[i]);
      h_raw[16]->Fill(raw16[i]);
      h_raw[17]->Fill(raw17[i]);
      h_raw[18]->Fill(raw18[i]);
      h_raw[19]->Fill(raw19[i]);
      h_raw[20]->Fill(raw20[i]);
      h_raw[21]->Fill(raw21[i]);
      h_raw[22]->Fill(raw22[i]);
      h_raw[23]->Fill(raw23[i]);
      h_raw[24]->Fill(raw24[i]);
      h_raw[25]->Fill(raw25[i]);
      h_raw[26]->Fill(raw26[i]);
      h_raw[27]->Fill(raw27[i]);
      h_raw[28]->Fill(raw28[i]);
      h_raw[29]->Fill(raw29[i]);
      h_raw[30]->Fill(raw30[i]);
      h_raw[31]->Fill(raw31[i]);
      sum_raw[0] +=raw00[i];
      sum_raw[1] +=raw01[i];
      sum_raw[2] +=raw02[i];
      sum_raw[3] +=raw03[i];
      sum_raw[4] +=raw04[i];
      sum_raw[5] +=raw05[i];
      sum_raw[6] +=raw06[i];
      sum_raw[7] +=raw07[i];
      sum_raw[8] +=raw08[i];
      sum_raw[9] +=raw09[i];
      sum_raw[10]+=raw10[i];
      sum_raw[11]+=raw11[i];
      sum_raw[12]+=raw12[i];
      sum_raw[13]+=raw13[i];
      sum_raw[14]+=raw14[i];
      sum_raw[15]+=raw15[i];
      sum_raw[16]+=raw16[i];
      sum_raw[17]+=raw17[i];
      sum_raw[18]+=raw18[i];
      sum_raw[19]+=raw19[i];
      sum_raw[20]+=raw20[i];
      sum_raw[21]+=raw21[i];
      sum_raw[22]+=raw22[i];
      sum_raw[23]+=raw23[i];
      sum_raw[24]+=raw24[i];
      sum_raw[25]+=raw25[i];
      sum_raw[26]+=raw26[i];
      sum_raw[27]+=raw27[i];
      sum_raw[28]+=raw28[i];
      sum_raw[29]+=raw29[i];
      sum_raw[30]+=raw30[i];
      sum_raw[31]+=raw31[i];
    }
    for(int i=0;i<32;i++){
      //cout<<"sum"<<i<<" "<<sum_raw[i]<<endl;
      h_sum_raw[i]->Fill(sum_raw[i]);
    }

  }//event for loop
  cout<<"end of event loop"<<endl;

  ////////
  //Draw//
  ////////
  TCanvas *tc[2];
  for(int i=0;i<2;i++){
    tc[i] = new TCanvas(Form("tc%d",i+1),Form("tc%d",i+1),1000,700);
  }
  cout<<"after make canvas"<<endl;
  tc[0]->Divide(7,5);
  for(int i=0;i<32;i++){
    tc[0]->cd(i+1);gPad->SetLogy(1);h_raw[i]->Draw("");
  }
  tc[1]->Divide(7,5);
  for(int i=0;i<32;i++){
    tc[1]->cd(i+1);gPad->SetLogy(1);h_sum_raw[i]->Draw("");
  }
  string ofname_pdf = ofname;
  ofname_pdf.erase(ofname_pdf.size()-5);
  ofname_pdf.append(".pdf");

  tc[0]->Print(Form("%s[",ofname_pdf.c_str()));
  tc[0]->Print(Form("%s" ,ofname_pdf.c_str()));
  tc[1]->Print(Form("%s" ,ofname_pdf.c_str()));
  tc[1]->Print(Form("%s]",ofname_pdf.c_str()));
  cout<<ofname_pdf <<" saved"<<endl;

  ofp->Write(); ofp->Close();
  gSystem->Exit(1);
  theApp->Run();

  return 0;

}

