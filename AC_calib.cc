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
#include "Tree.h"
#include "ParamMan.hh"
#include "Setting.h"

#define Calibration

static const double PI = 4.0*atan(1.);
static const double mrad_to_deg = 1./1000*180./PI;
const double Mp = 938.272046;          // proton       mass (MeV/c2)
const double c = 0.299792458;          // speed of light in vacuum (m/ns)
//____________________________________________________________________________________________
void FitGaus(TH1F *h, double &gamin, double &gamax){
	 gamin = h  ->GetBinCenter(h ->GetMaximumBin())-20.0;
	 gamax = h  ->GetBinCenter(h ->GetMaximumBin())+20.0;
	for(Int_t l=0; l<10; l++){
	TF1 *ga = new TF1("ga","gaus");
      h  ->Fit(ga,"0QR","",gamin,gamax);
      gamin = ga->GetParameter(1) - ga->GetParameter(2)*2.5;
      gamax = ga->GetParameter(1) + ga->GetParameter(2)*2.5;
      ga->Clear();
	}
 return;
 }

///////////////////////////////////////
class AC_ana : public Tree
{
 public:
         AC_ana();
        ~AC_ana();
    void SetOutputName(string ofname);
    void SetParamFile(string paramname);
    void SetRoot(string ifname);
    void makehist();
    void loop();
    void fit();
    void draw(); 
    void savecanvas(); 
    void SetMaxEvent( int N )  { ENumMax = N; }

  private:
    ParamMan *paramMan;
    TFile *ofp;
    TTree *tree_out;
    Setting *set;
    string output_name;
    string input_param;
    string output_param;
    int GetMaxEvent() { return ENumMax; }
    int ENumMax;
    int ENum;

    TH1F *h_Q_ACu[12], *h_Q_ACb[12];
    TH1F *h_Q_ACu_wt[12], *h_Q_ACb_wt[12];
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
  add(ifname);
  readtree();
  ENum = GetEntries();
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::SetParamFile(string paramname){
  input_param = paramname;
  output_param = "output.param";
  cout<<"input param name: "<<input_param<<endl;
  paramMan= new ParamMan(input_param.c_str());
  paramMan->Initialize();
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::SetOutputName(string ofname){
  output_name = ofname;
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::makehist(){
  ofp = new TFile(Form("%s",output_name.c_str()),"recreate");

  for(int i=0;i<12;i++){
    h_Q_ACu[i] = new TH1F(Form("h_Q_ACu%d",i+1),Form("h_Q_ACu%d",i+1),1000,5000,15000);
    h_Q_ACb[i] = new TH1F(Form("h_Q_ACb%d",i+1),Form("h_Q_ACb%d",i+1),1000,5000,15000);
    set->SetTH1(h_Q_ACu[i],Form("QDC AC top%d",i+1)   ,"QDC[ch]","counts/10ch",1,3000,2);
    set->SetTH1(h_Q_ACb[i],Form("QDC AC bottom%d",i+1),"QDC[ch]","counts/10ch",1,3000,2);

    h_Q_ACu_wt[i] = new TH1F(Form("h_Q_ACu%d_wt",i+1),Form("h_Q_ACu%d_wt",i+1),1000,5000,15000);
    h_Q_ACb_wt[i] = new TH1F(Form("h_Q_ACb%d_wt",i+1),Form("h_Q_ACb%d_wt",i+1),1000,5000,15000);

    h_NPE_ACu[i] = new TH1F(Form("h_NPE_ACu%d",i+1),Form("h_NPE_ACu%d",i+1),1000,-1,10);
    h_NPE_ACb[i] = new TH1F(Form("h_NPE_ACb%d",i+1),Form("h_NPE_ACb%d",i+1),1000,-1,10);
    set->SetTH1(h_NPE_ACu[i],Form("Num. of P.E. top%d",i+1)   ,"NPE","counts",1,3000,0);
    set->SetTH1(h_NPE_ACb[i],Form("Num. of P.E. bottom%d",i+1),"NPE","counts",1,3000,0);
  }
  
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::loop(){
  double ACu_Q[12], ACb_Q[12], ACu_npe[12], ACb_npe[12];//bkl sum.
  double AC_Qm[12], AC_Qgm[12], AC_npem[12], AC_npegm[12];//mean & geometric mean(soujo-heikin)

  int ACu_over05pe[12],ACb_over05pe[12],ACm_over05pe[12],ACgm_over05pe[12];//num of event npe>0.5pe for each AC
    for(int i=0;i<12;i++){
      ACu_over05pe[i]=ACb_over05pe[i]=ACm_over05pe[i]=ACgm_over05pe[i]=0;
    }


  if( GetMaxEvent()>0 && GetMaxEvent()<ENum) ENum = GetMaxEvent();
  for(int n=0;n<ENum;n++){
    tree->GetEntry(n);
    if(n%1000==0) cout<<n<<" / "<<ENum<<endl;

    //////////////
    //initiarize//
    //////////////
    ACu_Q[0] = sum[0] ; ACb_Q[0] = sum[23];
    ACu_Q[1] = sum[1] ; ACb_Q[1] = sum[22];
    ACu_Q[2] = sum[2] ; ACb_Q[2] = sum[21];
    ACu_Q[3] = sum[3] ; ACb_Q[3] = sum[20];
    ACu_Q[4] = sum[4] ; ACb_Q[4] = sum[19];
    ACu_Q[5] = sum[5] ; ACb_Q[5] = sum[18];
    ACu_Q[6] = sum[6] ; ACb_Q[6] = sum[17];
    ACu_Q[7] = sum[7] ; ACb_Q[7] = sum[16];
    ACu_Q[8] = sum[31]; ACb_Q[8] = sum[15];
    ACu_Q[9] = sum[9] ; ACb_Q[9] = sum[14];
    ACu_Q[10]= sum[10]; ACb_Q[10]= sum[13];
    ACu_Q[11]= sum[11]; ACb_Q[11]= sum[12];

    ACu_npe[0] = paramMan->npe(1, 1, 0,sum[0] ); ACb_npe[0] = paramMan->npe(1, 1, 1, sum[23]);
    ACu_npe[1] = paramMan->npe(1, 2, 0,sum[1] ); ACb_npe[1] = paramMan->npe(1, 2, 1, sum[22]);
    ACu_npe[2] = paramMan->npe(1, 3, 0,sum[2] ); ACb_npe[2] = paramMan->npe(1, 3, 1, sum[21]);
    ACu_npe[3] = paramMan->npe(1, 4, 0,sum[3] ); ACb_npe[3] = paramMan->npe(1, 4, 1, sum[20]);
    ACu_npe[4] = paramMan->npe(1, 5, 0,sum[4] ); ACb_npe[4] = paramMan->npe(1, 5, 1, sum[19]);
    ACu_npe[5] = paramMan->npe(1, 6, 0,sum[5] ); ACb_npe[5] = paramMan->npe(1, 6, 1, sum[18]);
    ACu_npe[6] = paramMan->npe(1, 7, 0,sum[6] ); ACb_npe[6] = paramMan->npe(1, 7, 1, sum[17]);
    ACu_npe[7] = paramMan->npe(1, 8, 0,sum[7] ); ACb_npe[7] = paramMan->npe(1, 8, 1, sum[16]);
    ACu_npe[8] = paramMan->npe(1, 9, 0,sum[31]); ACb_npe[8] = paramMan->npe(1, 9, 1, sum[15]);
    ACu_npe[9] = paramMan->npe(1,10, 0,sum[9] ); ACb_npe[9] = paramMan->npe(1,10, 1, sum[14]);
    ACu_npe[10]= paramMan->npe(1,11, 0,sum[10]); ACb_npe[10]= paramMan->npe(1,11, 1, sum[13]);
    ACu_npe[11]= paramMan->npe(1,12, 0,sum[11]); ACb_npe[11]= paramMan->npe(1,12, 1, sum[12]);
    for(int i=0;i<12;i++){
      AC_Qm[i]=(ACu_Q[i]+ACb_Q[i])/2.;
      AC_Qgm[i]=sqrt(ACu_Q[i]*ACb_Q[i]);
      AC_npem[i]=(ACu_npe[i]+ACb_npe[i])/2.;
      AC_npegm[i]=sqrt(ACu_npe[i]*ACb_npe[i]);
      if(ACu_npe[i]>0.5) ACu_over05pe[i]++;
      if(ACb_npe[i]>0.5) ACb_over05pe[i]++;
      if(AC_npem[i]>0.5) ACm_over05pe[i]++;
      if(AC_npegm[i]>0.5)ACgm_over05pe[i]++;

      h_Q_ACu[i]->Fill(ACu_Q[i]);
      h_Q_ACb[i]->Fill(ACb_Q[i]);
      h_Q_ACu_wt[i]->Fill(ACu_Q[i]);
      h_Q_ACb_wt[i]->Fill(ACb_Q[i]);

      h_NPE_ACu[i]->Fill(ACu_npe[i]);
      h_NPE_ACb[i]->Fill(ACb_npe[i]);
    }

  }//event loop

    cout<<"*********"<<endl;
    cout<<"AC pe>0.5"<<endl;
    cout<<"*********"<<endl;
    cout<<"U B Mean gMean"<<endl;
  for(int i=0;i<12;i++){
    cout<<(double)ACu_over05pe[i]/ENum<<" "<<(double)ACb_over05pe[i]/ENum<<" "<<(double)ACm_over05pe[i]/ENum<<" "<<(double)ACgm_over05pe[i]/ENum<<endl;
  }

  ofp->Write();
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::fit(){
  double gamin_u,gamax_u;
  double gamin_b,gamax_b;
  double min=5000,max=15000;
    for(int i=0;i<12;i++){
      TF1 *g1 = new TF1("g1","gaus",min,max);
      g1->SetLineColor(2);
      FitGaus(h_Q_ACu[i],gamin_u,gamax_u);
      h_Q_ACu[i]->Fit(g1,"QR","",gamin_u,gamax_u);
      paramMan ->SetAdcOffset(1,i+1,0,g1->GetParameter(1));

      TF1 *g2 = new TF1("g2","gaus",min,max);
      g2->SetLineColor(2);
      FitGaus(h_Q_ACb[i],gamin_b,gamax_b);
      h_Q_ACb[i]->Fit(g2,"QR","",gamin_b,gamax_b);
      paramMan ->SetAdcOffset(1,i+1,1,g2->GetParameter(1));
    }

    for(int i=0;i<12;i++){
      min = 200+paramMan->GetAdcOffset(1,i+1,0);
      h_Q_ACu_wt[i]->GetXaxis()->SetRangeUser(min,max);

      min = 200+paramMan->GetAdcOffset(1,i+1,1);
      h_Q_ACb_wt[i]->GetXaxis()->SetRangeUser(min,max);
    }
    for(int i=0;i<12;i++){
      paramMan->SetAdcPeak(1,i+1,0,h_Q_ACu_wt[i]->GetXaxis()->GetBinCenter(h_Q_ACu_wt[i]->GetMaximumBin()));
      paramMan->SetAdcPeak(1,i+1,1,h_Q_ACb_wt[i]->GetXaxis()->GetBinCenter(h_Q_ACb_wt[i]->GetMaximumBin()));
    }
    paramMan ->WriteToFile(output_param.c_str());

}
////////////////////////////////////////////////////////////////////////////
void AC_ana::draw(){

c1->Clear();
c1->Divide(4,6);
for(int i=0;i<12;i++){
  c1->cd(i+1);  gPad->SetLogy(1);h_Q_ACu[i]->Draw();h_Q_ACu_wt[i]->Draw("same");
  c1->cd(i+13); gPad->SetLogy(1);h_Q_ACb[i]->Draw();h_Q_ACb_wt[i]->Draw("same");
}
c2->Clear();
c2->Divide(4,6);
for(int i=0;i<12;i++){
  c2->cd(i+1);  gPad->SetLogy(1);h_NPE_ACu[i]->Draw();
               line->DrawLine(1,0.5,1,h_NPE_ACu[i]->GetBinContent(h_NPE_ACu[i]->FindBin(1)));
  c2->cd(i+13); gPad->SetLogy(1);h_NPE_ACb[i]->Draw();
               line->DrawLine(1,0.5,1,h_NPE_ACb[i]->GetBinContent(h_NPE_ACb[i]->FindBin(1)));
}
c3->Clear();
c4->Clear();
c5->Clear();

}
////////////////////////////////////////////////////////////////////////////
void AC_ana::savecanvas(){
  string ofname_pdf = output_name;
  ofname_pdf.erase(ofname_pdf.size()-5);
  ofname_pdf.append(".pdf");
  c1->Print(Form("%s[",ofname_pdf.c_str()));
  c1->Print(Form("%s" ,ofname_pdf.c_str()));
  c2->Print(Form("%s" ,ofname_pdf.c_str()));
  c2->Print(Form("%s]",ofname_pdf.c_str()));
cout<<ofname_pdf.c_str()<<" saved"<<endl;
  ofp->Close();
}
////////////////////////////////////////////////////////////////////////////
//////////////////////////// main //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv){

  string ifname = "output0000.dat";
  string ofname = "rootfiles/calib/test.root";
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
      cout<<"-w : output txt filename"<<endl;
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

  ana->SetMaxEvent(MaxNum);
  ana->SetParamFile(paramname);
  ana->SetRoot(ifname);
  ana->SetOutputName(ofname);
  ana->makehist();
  ana->loop();
  ana->fit();
  ana->draw();
  ana->savecanvas();
  delete ana;

  gSystem->Exit(1);
  theApp->Run();
  return 0;
}

