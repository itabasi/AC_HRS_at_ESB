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

#define Calibration

static const double PI = 4.0*atan(1.);
static const double mrad_to_deg = 1./1000*180./PI;
const double Mp = 938.272046;          // proton       mass (MeV/c2)
const double c = 0.299792458;          // speed of light in vacuum (m/ns)

///////////////////////////////////////
double exp_ln(double *x, double *par){
  //par[0]=peak height
  //par[1]=assym
  //par[2]=width2
  //par[3]=offset

  double xx=x[0];
  double val = par[0]*TMath::Exp(-0.5*TMath::Power( TMath::Log(xx/par[1])/par[2],2. ) )+par[3];
  return val;
}
///////////////////////////////////////
struct TreeBranch{
double trig_lead[4], trig_rise[4], trig_peak[4], trig_peak_x[4], trig_offset[4], trig_chi2[4], trig_sum[4], trig_integ[4];
double acu_qdc[12],acb_qdc[12],ac_mqdc[12],ac_gmqdc[12];
double acu_npe[12],acb_npe[12],ac_mnpe[12],ac_gmnpe[12];
};

static TreeBranch tr;         

///////////////////////////////////////
class AC_ana : public Tree
{
 public:
         AC_ana();
        ~AC_ana();
    void makehist();
    void maketree(string ofname);
    void loop();
    void fit();
    void draw(); 
    void savecanvas(string ofname); 
    void SetMaxEvent( int N )  { ENumMax = N; }
    void SetRoot(string ifname);
    void SetParamFile(string paramname);

  private:
    ParamMan *paramMan;
    TFile *ofp;
    TTree *tree_out;
    string input_param;
    int GetMaxEvent() { return ENumMax; }
    int ENumMax;
    int ENum;

    TH1F *h_ctime;
    TH1F *h_TDC_Trig1L, *h_TDC_Trig1R, *h_TDC_Trig2L, *h_TDC_Trig2R;
    TH1F *h_Q_Trig1L, *h_Q_Trig1R, *h_Q_Trig2L, *h_Q_Trig2R;

    TH2F *h2_QI_Trig1L, *h2_QI_Trig1R, *h2_QI_Trig2L, *h2_QI_Trig2R;
    TH2F *h2_QT_Trig1L, *h2_QT_Trig1R, *h2_QT_Trig2L, *h2_QT_Trig2R;

    TGraph *gr_Trig[4];

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
  gStyle->SetPadGridX(1);
  gStyle->SetPadGridY(1);
  gStyle->SetFrameLineWidth(0);
  gStyle->SetLineWidth(0);
  gStyle->SetOptDate(0);
//  gStyle->SetStatW(0.15);
  gStyle->SetStatFontSize(0.03);
  gStyle->SetStatTextColor(1);
  gStyle->SetTitleX(0.15);
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetTitleTextColor(1);
  gStyle->SetGridWidth(0);
  gStyle->SetFrameLineWidth(0);
  gStyle->SetLineWidth(0);
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
  cout<<"input param name: "<<input_param<<endl;
  paramMan= new ParamMan(input_param.c_str());
  paramMan->Initialize();
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::maketree(string ofname){
  ofp = new TFile(Form("%s",ofname.c_str()),"recreate");
  tree_out = new TTree("tree","tree");
  tree_out->Branch("trig_lead"   , tr.trig_lead   ,"trig_lead[4]/D"  );//leading edge TDC like info.
  tree_out->Branch("trig_rise"   , tr.trig_rise   ,"trig_rise[4]/D"  );//timing info.
  tree_out->Branch("trig_peak"   , tr.trig_peak   ,"trig_peak[4]/D"  );//peak height
  tree_out->Branch("trig_peak_x" , tr.trig_peak_x ,"trig_peak_x[4]/D");//peak position
  tree_out->Branch("trig_offset" , tr.trig_offset ,"trig_offset[4]/D");//ground level info.
  tree_out->Branch("trig_chi2"   , tr.trig_chi2   ,"trig_chi2[4]/D"  );//fitting result
  tree_out->Branch("trig_sum"    , tr.trig_sum    ,"trig_sum[4]/D"   );//added all fADC value
  tree_out->Branch("trig_integ"  , tr.trig_integ  ,"trig_integ[4]/D" );//integrate fitting result
  tree_out->Branch("acu_qdc"     , tr.acu_qdc     ,"acu_qdc[12]/D" );
  tree_out->Branch("acb_qdc"     , tr.acb_qdc     ,"acb_qdc[12]/D" );
  tree_out->Branch("ac_mqdc"     , tr.ac_mqdc     ,"ac_mqdc[12]/D" );
  tree_out->Branch("ac_gmqdc"    , tr.ac_gmqdc    ,"ac_gmqdc[12]/D");
  tree_out->Branch("acu_npe"     , tr.acu_npe     ,"acu_npe[12]/D" );
  tree_out->Branch("acb_npe"     , tr.acb_npe     ,"acb_npe[12]/D" );
  tree_out->Branch("ac_mnpe"     , tr.ac_mnpe     ,"ac_mnpe[12]/D" );
  tree_out->Branch("ac_gmnpe"    , tr.ac_gmnpe    ,"ac_gmnpe[12]/D");

}
////////////////////////////////////////////////////////////////////////////
void AC_ana::makehist(){
  h_ctime    = new TH1F("h_ctime"   ,"h_ctime"   ,2400,-30,30);

  h_TDC_Trig1L    = new TH1F("h_TDC_Trig1L"   ,"h_TDC_Trig1L"   ,70,   0,   70);
  h_TDC_Trig1R    = new TH1F("h_TDC_Trig1R"   ,"h_TDC_Trig1R"   ,70,   0,   70);
  h_TDC_Trig2L    = new TH1F("h_TDC_Trig2L"   ,"h_TDC_Trig2L"   ,70,   0,   70);
  h_TDC_Trig2R    = new TH1F("h_TDC_Trig2R"   ,"h_TDC_Trig2R"   ,70,   0,   70);
  h_Q_Trig1L      = new TH1F("h_Q_Trig1L"     ,"h_Q_Trig1L"   ,1501,5000,15000);
  h_Q_Trig1R      = new TH1F("h_Q_Trig1R"     ,"h_Q_Trig1R"   ,1501,5000,15000);
  h_Q_Trig2L      = new TH1F("h_Q_Trig2L"     ,"h_Q_Trig2L"   ,1501,5000,15000);
  h_Q_Trig2R      = new TH1F("h_Q_Trig2R"     ,"h_Q_Trig2R"   ,1501,5000,15000);

  h2_QI_Trig1L    = new TH2F("h2_QI_Trig1L","h2_QI_Trig1L",200,   0, 50,1501,5000,15000);
  h2_QI_Trig1R    = new TH2F("h2_QI_Trig1R","h2_QI_Trig1R",200,   0, 50,1501,5000,15000);
  h2_QI_Trig2L    = new TH2F("h2_QI_Trig2L","h2_QI_Trig2L",200,   0, 50,1501,5000,15000);
  h2_QI_Trig2R    = new TH2F("h2_QI_Trig2R","h2_QI_Trig2R",200,   0, 50,1501,5000,15000);
  h2_QT_Trig1L    = new TH2F("h2_QT_Trig1L","h2_QT_Trig1L",200,-100,100,1501,5000,15000);
  h2_QT_Trig1R    = new TH2F("h2_QT_Trig1R","h2_QT_Trig1R",200,-100,100,1501,5000,15000);
  h2_QT_Trig2L    = new TH2F("h2_QT_Trig2L","h2_QT_Trig2L",200,-100,100,1501,5000,15000);
  h2_QT_Trig2R    = new TH2F("h2_QT_Trig2R","h2_QT_Trig2R",200,-100,100,1501,5000,15000);

  for(int i=0;i<4;i++){gr_Trig[i] = new TGraph(59);}
  
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::loop(){
  double Trig_1L_dif[60],Trig_1R_dif[60];
  double Trig_2L_dif[60],Trig_2R_dif[60];
  double Trig_sum[4];
  double Trig_1L_dif_max,Trig_1R_dif_max;
  double Trig_2L_dif_max,Trig_2R_dif_max;
  double Trig_1L_dif_max_time,Trig_1R_dif_max_time;
  double Trig_2L_dif_max_time,Trig_2R_dif_max_time;
  double ACu_Q[12], ACb_Q[12], ACu_npe[12], ACb_npe[12];//bkl sum.
  double AC_Qm[12], AC_Qgm[12], AC_npem[12], AC_npegm[12];//mean & geometric mean(soujo-heikin)

  if( GetMaxEvent()>0 && GetMaxEvent()<ENum) ENum = GetMaxEvent();
  for(int n=0;n<ENum;n++){
    tree->GetEntry(n);
    if(n%200==0) cout<<n<<" / "<<ENum<<endl;

    //////////////
    //initiarize//
    //////////////
    for(int i=0;i<4;i++){Trig_sum[i]=0;}
    Trig_1L_dif_max=-100;Trig_1R_dif_max=-100;
    Trig_2L_dif_max=-100;Trig_2R_dif_max=-100;
    Trig_1L_dif_max_time=-100;Trig_1R_dif_max_time=-100;
    Trig_2L_dif_max_time=-100;Trig_2R_dif_max_time=-100;

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

    for(int i=0;i<12;i++){
      ACu_npe[i] = paramMan->npe(1, i+1, 0, ACu_Q[i]);
      ACb_npe[i] = paramMan->npe(1, i+1, 1, ACb_Q[i]);
      AC_Qm[i]=(ACu_Q[i]+ACb_Q[i])/2.;
      AC_Qgm[i]=sqrt(ACu_Q[i]*ACb_Q[i]);
      AC_npem[i]=(ACu_npe[i]+ACb_npe[i])/2.;
      AC_npegm[i]=sqrt(ACu_npe[i]*ACb_npe[i]);
    }

    for(int i=0;i<12;i++){
      tr.acu_qdc[i] = ACu_Q[i];   tr.acb_qdc[i]  = ACb_Q[i];
      tr.ac_mqdc[i] = AC_Qm[i];   tr.ac_gmqdc[i] = AC_Qgm[i];
      tr.acu_npe[i] = ACu_npe[i]; tr.acb_npe[i]  = ACb_npe[i];
      tr.ac_mnpe[i] = AC_npem[i]; tr.ac_gmnpe[i] = AC_npegm[i];
    }

    ////////////
    //Trig Ana//
    ////////////
    for(int i=0;i<59;i++){
      Trig_sum[0] += Trig_1L_raw[i];
      Trig_sum[1] += Trig_1R_raw[i];
      Trig_sum[2] += Trig_2L_raw[i];
      Trig_sum[3] += Trig_2R_raw[i];
      Trig_1L_dif[i] = Trig_1L_raw[i+1] - Trig_1L_raw[i];
      Trig_1R_dif[i] = Trig_1R_raw[i+1] - Trig_1R_raw[i];
      Trig_2L_dif[i] = Trig_2L_raw[i+1] - Trig_2L_raw[i];
      Trig_2R_dif[i] = Trig_2R_raw[i+1] - Trig_2R_raw[i];
      if(Trig_1L_dif_max<Trig_1L_dif[i]){Trig_1L_dif_max = Trig_1L_dif[i];Trig_1L_dif_max_time = i;}
      if(Trig_1R_dif_max<Trig_1R_dif[i]){Trig_1R_dif_max = Trig_1R_dif[i];Trig_1R_dif_max_time = i;}
      if(Trig_2L_dif_max<Trig_2L_dif[i]){Trig_2L_dif_max = Trig_2L_dif[i];Trig_2L_dif_max_time = i;}
      if(Trig_2R_dif_max<Trig_2R_dif[i]){Trig_2R_dif_max = Trig_2R_dif[i];Trig_2R_dif_max_time = i;}

      gr_Trig[0] ->SetPoint(i,i,Trig_1L_raw[i]);
      gr_Trig[1] ->SetPoint(i,i,Trig_1R_raw[i]);
      gr_Trig[2] ->SetPoint(i,i,Trig_2L_raw[i]);
      gr_Trig[3] ->SetPoint(i,i,Trig_2R_raw[i]);
    }

    double TrigT_mean = (Trig_1L_dif_max_time + Trig_1R_dif_max_time)/2.-(Trig_2L_dif_max_time + Trig_2R_dif_max_time)/2.;
    
    h_TDC_Trig1L   ->Fill(Trig_1L_dif_max_time);
    h_TDC_Trig1R   ->Fill(Trig_1R_dif_max_time);
    h_TDC_Trig2L   ->Fill(Trig_2L_dif_max_time);
    h_TDC_Trig2R   ->Fill(Trig_2R_dif_max_time);
    h_Q_Trig1L     ->Fill(Trig_sum[0]);
    h_Q_Trig1R     ->Fill(Trig_sum[1]);
    h_Q_Trig2L     ->Fill(Trig_sum[2]);
    h_Q_Trig2R     ->Fill(Trig_sum[3]);

    h2_QT_Trig1L   ->Fill(TrigT_mean,Trig_sum[0]);
    h2_QT_Trig1R   ->Fill(TrigT_mean,Trig_sum[1]);
    h2_QT_Trig2L   ->Fill(TrigT_mean,Trig_sum[2]);
    h2_QT_Trig2R   ->Fill(TrigT_mean,Trig_sum[3]);

    double peak[4],peak_x[4],offset[4],rise_point[4],integral[4];
    double lead_point[4];
    for(int i=0;i<4;i++){
      TF1 *f1 = new TF1("f1",exp_ln,0.1,100,4);
      //set->SetTF1(f1,2,1,1.5);
      double param[4]={10,2,5,100};
      f1->SetParameters(&param[0]);f1->SetNpx(2000);
      gr_Trig[i]->Fit(f1,"0QR","");
      peak[i]   = f1->GetMaximum(0,100);
      peak_x[i] = f1->GetMaximumX(0,100);
      offset[i] = f1->GetParameter(3);
      lead_point[i] = f1->GetX(20+offset[i], 0,peak_x[i]);
      rise_point[i] = f1->GetX((peak[i]-offset[i])*0.1+offset[i], 0,peak_x[i]);
      integral[i] = f1->Integral(0,60);
      tr.trig_lead[i]   =4.*lead_point[i];//fADC = 250MHz
      tr.trig_rise[i]   =4.*rise_point[i];//fADC = 250MHz
      tr.trig_peak[i]   =peak[i];
      tr.trig_peak_x[i] =peak_x[i];
      tr.trig_offset[i] =offset[i];
      tr.trig_chi2[i]   =f1->GetChisquare();
      tr.trig_sum[i]    =Trig_sum[i];
      tr.trig_integ[i]  =integral[i];
    }

    //h2_QI_Trig1L   ->Fill(peak[0]-offset[0],Trig_sum[0]);
    //h2_QI_Trig1R   ->Fill(peak[1]-offset[1],Trig_sum[1]);
    //h2_QI_Trig2L   ->Fill(peak[2]-offset[2],Trig_sum[2]);
    //h2_QI_Trig2R   ->Fill(peak[3]-offset[3],Trig_sum[3]);

    tree_out -> Fill();
  }//event loop

  ofp->Write();
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::fit(){
}
////////////////////////////////////////////////////////////////////////////
void AC_ana::draw(){

c1->Clear();
c1->Divide(4,4);
c1->cd(1); gPad->SetLogy(1);h_TDC_Trig1L ->Draw();
c1->cd(2); gPad->SetLogy(1);h_TDC_Trig1R ->Draw();
c1->cd(3); gPad->SetLogy(1);h_TDC_Trig2L ->Draw();
c1->cd(4); gPad->SetLogy(1);h_TDC_Trig2R ->Draw();
c1->cd(5); gPad->SetLogy(1);h_Q_Trig1L   ->Draw();
c1->cd(6); gPad->SetLogy(1);h_Q_Trig1R   ->Draw();
c1->cd(7); gPad->SetLogy(1);h_Q_Trig2L   ->Draw();
c1->cd(8); gPad->SetLogy(1);h_Q_Trig2R   ->Draw();
c1->cd(9); gPad->SetLogz(1);h2_QT_Trig1L ->Draw("colz");
c1->cd(10);gPad->SetLogz(1);h2_QT_Trig1R ->Draw("colz");
c1->cd(11);gPad->SetLogz(1);h2_QT_Trig2L ->Draw("colz");
c1->cd(12);gPad->SetLogz(1);h2_QT_Trig2R ->Draw("colz");
c1->cd(13);gPad->SetLogz(1);h2_QI_Trig1L ->Draw("colz");
c2->Clear();
c3->Clear();
c4->Clear();
c5->Clear();

}
////////////////////////////////////////////////////////////////////////////
void AC_ana::savecanvas(string ofname){
  //string ofname = "test.root";//tmp
  string ofname_pdf = ofname;
  ofname_pdf.erase(ofname_pdf.size()-5);
  ofname_pdf.append(".pdf");
  c1->Print(Form("%s[",ofname_pdf.c_str()));
  c1->Print(Form("%s" ,ofname_pdf.c_str()));
  c1->Print(Form("%s]",ofname_pdf.c_str()));
cout<<ofname_pdf.c_str()<<" saved"<<endl;
}
////////////////////////////////////////////////////////////////////////////
//////////////////////////// main //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv){
  

  string ifname = "rootfiles/fadcAERO_117.root";
  string ofname = "rootfiles/conv/hoge.root";
  string paramname="param/default.param";
  int ch;
  int MaxNum = 0;
  bool output_flag = false;
  bool draw_flag = true;
  extern char *optarg;
  while((ch=getopt(argc,argv,"hf:w:n:p:b"))!=-1){
    switch(ch){
    case 'f':
      ifname = optarg;
      cout<<"input filename : "<<ifname<<endl;
      break;
    case 'w':
      output_flag = true;
      draw_flag = true;
      ofname = optarg;
      cout<<"output filename : "<<ofname<<endl;
      break;
    case 'n':
      MaxNum = atoi(optarg);
      break;
    case 'b':
      draw_flag = false;
      cout<<"BACH MODE!"<<endl;
      break;
    case 'p':
      paramname = optarg;
      break;
    case 'h':
      cout<<"-f : input root file name"<<endl;
      cout<<"-w : output root file name"<<endl;
      cout<<"-n : maximum number of analysed events"<<endl;
      cout<<"-p : input parameter file name"<<endl;
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
  ana->makehist();
  ana->maketree(ofname);
  ana->loop();
  //ana->fit();
  if(draw_flag){
    ana->draw();
    ana->savecanvas(ofname);
  }
  delete ana;

  gSystem->Exit(1);
  theApp->Run();
  return 0;
}

