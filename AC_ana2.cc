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
#include "TLegend.h"

#include "TRandom.h"
#include "Setting.h"

#define Calibration

static const double PI = 4.0*atan(1.);
static const double mrad_to_deg = 1./1000*180./PI;
const double Mp = 938.272046;          // proton       mass (MeV/c2)
const double c = 0.299792458;          // speed of light in vacuum (m/ns)

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

  TApplication *theApp = new TApplication("App", &argc, argv);
  Setting *set = new Setting();

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
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetFrameLineWidth(1);
  gStyle->SetLineWidth(1);
  gStyle->SetOptDate(0);
  gStyle->SetOptStat("ei");
//  gStyle->SetStatW(0.15);
  gStyle->SetStatFontSize(0.03);
  gStyle->SetStatTextColor(1);
  gStyle->SetTitleX(0.15);
  gStyle->SetTitleFontSize(0.05);
  gStyle->SetTitleTextColor(1);
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.13);

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
      
  gStyle->SetGridWidth(1);
  gStyle->SetNdivisions(510); // tertiary*10000 + secondary*100 + first

  TFile *ifp = new TFile(Form("%s",ifname.c_str() ) );
  TTree *tree = (TTree*)ifp->Get("tree");

  TFile *ofp = new TFile();
  if(output_flag){ ofp = new TFile(Form("%s",ofname.c_str()),"recreate"); }

  //////////////
  //SetBraches//
  //////////////
  int ENum;
  double trig_lead[4], trig_rise[4], trig_peak[4], trig_peak_x[4], trig_offset[4], trig_chi2[4], trig_sum[4], trig_integ[4];
  double acu_qdc[12],acb_qdc[12],ac_mqdc[12],ac_gmqdc[12];
  double acu_npe[12],acb_npe[12],ac_mnpe[12],ac_gmnpe[12];
  ENum = tree->GetEntries();
  if(MaxNum>0 && MaxNum<ENum) ENum = MaxNum;
  tree->SetBranchAddress("trig_lead"   , trig_lead   );
  tree->SetBranchAddress("trig_rise"   , trig_rise   );
  tree->SetBranchAddress("trig_peak"   , trig_peak   );
  tree->SetBranchAddress("trig_peak_x" , trig_peak_x );
  tree->SetBranchAddress("trig_offset" , trig_offset );
  tree->SetBranchAddress("trig_chi2"   , trig_chi2   );
  tree->SetBranchAddress("trig_sum"    , trig_sum    );
  tree->SetBranchAddress("trig_integ"  , trig_integ  );
  tree->SetBranchAddress("acu_qdc"     , acu_qdc     );
  tree->SetBranchAddress("acb_qdc"     , acb_qdc     );
  tree->SetBranchAddress("ac_mqdc"     , ac_mqdc     );
  tree->SetBranchAddress("ac_gmqdc"    , ac_gmqdc    );
  tree->SetBranchAddress("acu_npe"     , acu_npe     );
  tree->SetBranchAddress("acb_npe"     , acb_npe     );
  tree->SetBranchAddress("ac_mnpe"     , ac_mnpe     );
  tree->SetBranchAddress("ac_gmnpe"    , ac_gmnpe    );

  gStyle->SetOptStat("iMen");

  /////////
  //param//
  /////////
  double x_min[5] = {-1.25,-0.75,-0.25, 0.25, 0.75};
  double x_max[5] = {-0.75,-0.25, 0.25, 0.75, 1.25};
  double pe_th[5] = {  0.5,  1.,  3.,  5.,  10};//sum
  double pe_th_1PMT[5] = {  0.1,  0.2,  0.3,  0.4,  0.5};//specifit PMT

  //////////////
  //histograms//
  //////////////
  TH1F *h_Trig_ToF, *h_Trig_TDiff_top, *h_Trig_TDiff_bot;
  TH1F *h_Trig_xpos, *h_Trig_xpos_top, *h_Trig_xpos_bot;
  TH1F *h_Trig_TDiff_top_slice[5], *h_Trig_TDiff_bot_slice[5];
  TH1F *h_Trig_xpos_slice[5], *h_Trig_xpos_top_slice[5], *h_Trig_xpos_bot_slice[5];
  TH1F *h_Trig_chi2[4],*h_Trig_chi2_cut[4];
  TH1F *h_npe_AC1, *h_npe_AC12, *h_npe_sum;
  TH2F *h2_TDiff_top_bot, *h2_TDiff_top_bot_cut;
  TH2F *h2_xpos_top_bot, *h2_xpos_top_bot_cut;
  TH2F *h2_ToF_Q1, *h2_ToF_Q2, *h2_ToF_Q3, *h2_ToF_Q4;
  TH2F *h2_npe_TDiff_top, *h2_npe_TDiff_bot;
  TH2F *h2_AC1npe_xpos, *h2_AC5npe_xpos, *h2_AC12npe_xpos;
  TGraphErrors *gr_effAC[5];
  TGraph *gr_effAC_1PMTu[12][5], *gr_effAC_1PMTb[12][5];

  TLegend *leg_AC = new TLegend(0.7,0.7,0.90,0.90);
  leg_AC->SetBorderSize(1);
  leg_AC->SetFillColor(0);
  leg_AC->SetFillStyle(1001);
  leg_AC->SetTextFont(42);

  TLegend *leg_AC_1PMT = new TLegend(0.7,0.7,0.90,0.90);
  leg_AC_1PMT ->SetBorderSize(1);
  leg_AC_1PMT ->SetFillColor(0);
  leg_AC_1PMT ->SetFillStyle(1001);
  leg_AC_1PMT ->SetTextFont(42);

  h_Trig_ToF       = new TH1F("h_Trig_ToF"      ,"h_Trig_ToF"      ,1000, -20, 20);
  h_Trig_TDiff_top = new TH1F("h_Trig_TDiff_top","h_Trig_TDiff_top",1000, -20, 20);
  h_Trig_TDiff_bot = new TH1F("h_Trig_TDiff_bot","h_Trig_TDiff_bot",1000, -20, 20);
  h_Trig_xpos      = new TH1F("h_Trig_xpos"     ,"h_Trig_xpos"     , 100,-2.0,2.0);
  h_Trig_xpos_top  = new TH1F("h_Trig_xpos_top" ,"h_Trig_xpos_top" , 100,-2.0,2.0);
  h_Trig_xpos_bot  = new TH1F("h_Trig_xpos_bot" ,"h_Trig_xpos_bot" , 100,-2.0,2.0);
  h_npe_sum        = new TH1F("h_npe_sum"       ,"h_npe_sum"       ,1500,  -10,  50);
  h_npe_AC1        = new TH1F("h_npe_AC1"       ,"h_npe_AC1"       , 500,  -1,  5);
  h_npe_AC12       = new TH1F("h_npe_AC12"      ,"h_npe_AC12"      , 500,  -1,  5);

  h2_TDiff_top_bot     = new TH2F("h2_TDiff_top_bot"      ,"h2_TDiff_top_bot"      ,1000,-20,20,1000,  -20,   20);
  h2_TDiff_top_bot_cut = new TH2F("h2_TDiff_top_bot_cut"  ,"h2_TDiff_top_bot_cut"  ,1000,-20,20,1000,  -20,   20);
  h2_xpos_top_bot      = new TH2F("h2_xpos_top_bot"       ,"h2_xpos_top_bot"       , 100, -2, 2, 100,   -2,    2);
  h2_xpos_top_bot_cut  = new TH2F("h2_xpos_top_bot_cut"   ,"h2_xpos_top_bot_cut"   , 100, -2, 2, 100,   -2,    2);
  h2_ToF_Q1            = new TH2F("h2_ToF_Q1"             ,"h2_ToF_Q1"             , 100,-20,20, 100,10500,11500);
  h2_ToF_Q2            = new TH2F("h2_ToF_Q2"             ,"h2_ToF_Q2"             , 100,-20,20, 100, 9000,10000);
  h2_ToF_Q3            = new TH2F("h2_ToF_Q3"             ,"h2_ToF_Q3"             , 100,-20,20, 100, 9000,10000);
  h2_ToF_Q4            = new TH2F("h2_ToF_Q4"             ,"h2_ToF_Q4"             , 100,-20,20, 100, 6000, 7000);
  h2_npe_TDiff_top     = new TH2F("h2_npe_TDiff_top"      ,"h2_npe_TDiff_top"      ,1000,-20,20, 100,    0,    5);
  h2_npe_TDiff_bot     = new TH2F("h2_npe_TDiff_bot"      ,"h2_npe_TDiff_bot"      ,1000,-20,20, 100,    0,    5);
  h2_AC1npe_xpos       = new TH2F("h2_AC1npe_xpos"        ,"h2_AC1npe_xpos"        , 100, -2, 2, 100,    0.2,    5);
  h2_AC5npe_xpos       = new TH2F("h2_AC5npe_xpos"        ,"h2_AC5npe_xpos"        , 100, -2, 2, 100,    0.2,    5);
  h2_AC12npe_xpos      = new TH2F("h2_AC12npe_xpos"       ,"h2_AC12npe_xpos"       , 100, -2, 2, 100,    0.2,    5);
  set->SetTH2(h2_xpos_top_bot    ,"x_{top} vs x_{bot}","x_{top}[m]","x_{bottom}[m]");
  set->SetTH2(h2_xpos_top_bot_cut,"x_{top} vs x_{bot}","x_{top}[m]","x_{bottom}[m]");


  for(int i=0;i<5;i++){
    h_Trig_TDiff_top_slice[i]     = new TH1F(Form("h_Trig_TDiff_top_slice%d",i)    ,Form("h_Trig_TDiff_top_slice%d",i)    ,1000,-20,20);
    h_Trig_TDiff_bot_slice[i]     = new TH1F(Form("h_Trig_TDiff_bot_slice%d",i)    ,Form("h_Trig_TDiff_bot_slice%d",i)    ,1000,-20,20);
    h_Trig_xpos_slice[i]         = new TH1F(Form("h_Trig_xpos_slice%d",i)        ,Form("h_Trig_xpos_slice%d",i)    ,100,-2.0,2.0);
    h_Trig_xpos_top_slice[i]     = new TH1F(Form("h_Trig_xpos_top_slice%d",i)    ,Form("h_Trig_xpos_top_slice%d",i)    ,100,-2.0,2.0);
    h_Trig_xpos_bot_slice[i]     = new TH1F(Form("h_Trig_xpos_bot_slice%d",i)    ,Form("h_Trig_xpos_bot_slice%d",i)    ,100,-2.0,2.0);

    set->SetTH1(h_Trig_TDiff_top_slice[i]      , Form("TDiff_top%d",i+1)        , "TDiff[ns]", "counts", 1, 3001, i+2);
    set->SetTH1(h_Trig_TDiff_bot_slice[i]      , Form("TDiff_bot%d",i+1)        , "TDiff[ns]", "counts", 1, 3001, i+2);
    set->SetTH1(h_Trig_xpos_slice[i]          , Form("xpos%d",i+1)        , "xpos[m]", "counts", 1, 3001, i+2);
    set->SetTH1(h_Trig_xpos_top_slice[i]      , Form("xpos_top%d",i+1)        , "xpos[m]", "counts", 1, 3001, i+2);
    set->SetTH1(h_Trig_xpos_bot_slice[i]      , Form("xpos_bot%d",i+1)        , "xpos[m]", "counts", 1, 3001, i+2);
  }
  for(int i=0;i<4;i++){
    h_Trig_chi2[i]     = new TH1F(Form("h_Trig_chi2%d",i)        ,Form("h_Trig_chi2%d",i)        ,1000,-0.1,1000);
    h_Trig_chi2_cut[i] = new TH1F(Form("h_Trig_chi2_cut%d",i)    ,Form("h_Trig_chi2_cut%d",i)    ,1000,-0.1,1000);
    set->SetTH1(h_Trig_chi2[i]          , Form("#chi^{2} Trig. %d",i+1)        , "#chi^{2}", "counts", 1, 3000, 0);
    set->SetTH1(h_Trig_chi2_cut[i]      , Form("#chi^{2} Trig. %d",i+1)        , "#chi^{2}", "counts", 1, 3001, i+2);
  }

  TH2F *h_frame = new TH2F("h_frame","h_frame",10,-1.5,1.5,10,0,1.3);
  set->SetTH2(h_frame,"Ratio of P.E.>PEth","x pos[m]","N_{> p.e.}/N_{total}");

  TH2F *h_frame_1PMTu[12], *h_frame_1PMTb[12];
  for(int i=0;i<12;i++){
    h_frame_1PMTu[i] = new TH2F(Form("h_frame_1PMTu%d",i+1),Form("h_frame_1PMTu%d",i+1),10,-1.5,1.5,10,0,0.5);
    h_frame_1PMTb[i] = new TH2F(Form("h_frame_1PMTb%d",i+1),Form("h_frame_1PMTb%d",i+1),10,-1.5,1.5,10,0,0.5);
    set->SetTH2(h_frame_1PMTu[i],Form("AC%dtop Ratio of P.E.>PEth",i+1)   ,"x pos[m]","N_{> p.e.}/N_{total}");
    set->SetTH2(h_frame_1PMTb[i],Form("AC%dbottom Ratio of P.E.>PEth",i+1),"x pos[m]","N_{> p.e.}/N_{total}");
  }

  for(int i=0;i<5;i++){
    gr_effAC[i] = new TGraphErrors(5);
    set->SetGrErr(gr_effAC[i] ,Form("gr_effAC%d",i+1),"","",1,1+i,22+i,0.5);
    //gr_effAC[i] ->SetMarkerSize(1.1);
    leg_AC->AddEntry(gr_effAC[i],Form(">%.01lf p.e.",pe_th[i]),"p");
    for(int j=0;j<12;j++){
      gr_effAC_1PMTu[j][i] = new TGraph(5);
      gr_effAC_1PMTb[j][i] = new TGraph(5);
      set->SetGr(gr_effAC_1PMTu[j][i] ,Form("gr_effAC_1PMTu%d_%d",j+1,i+1),"","",1,1+i,22+i,0.5);
      set->SetGr(gr_effAC_1PMTb[j][i] ,Form("gr_effAC_1PMTb%d_%d",j+1,i+1),"","",1,1+i,22+i,0.5);
    }
    leg_AC_1PMT ->AddEntry(gr_effAC_1PMTu[0][i],Form(">%.01lf p.e.",pe_th_1PMT[i]),"p");
  }
////////////////////////////////////////////////////////////////////
//////////////       Fill
////////////////////////////////////////////////////////////////////
  int total_ev =0;
  int pe_ev =0;
  int total_ev_x[5],pe_ev_x[5][5],pe_ev_x_1PMTu[5][5][12],pe_ev_x_1PMTb[5][5][12];

  for(int i=0;i<5;i++){
    total_ev_x[i]=0;
      for(int j=0;j<5;j++){
        pe_ev_x[i][j]=0;

        for(int k=0;k<12;k++){
          pe_ev_x_1PMTu[i][j][k]=0;
          pe_ev_x_1PMTb[i][j][k]=0;
        }

      }
  }

  // event for loop
  for(int n=0;n<ENum;n++){
    tree->GetEntry(n);
    double npe_sum=0;
    bool chi2_flag = false, vertical_flag=false;

    if(n%10000==0){
     char clout[100];
     end = time(NULL);
     time(&end);
     sprintf(clout,"%.0f sec",difftime(end,start));
     cout<<n<<" / "<<ENum<<" : "<<clout<<endl;
    } // if n%100000==0 std::cout

    if(trig_chi2[0]<400&&trig_chi2[1]<400&&trig_chi2[2]<400&&trig_chi2[3]<400)chi2_flag=true;

    for(int i=0;i<12;i++){
      if(acu_npe[i]>0.2)npe_sum += acu_npe[i];
      if(acb_npe[i]>0.2)npe_sum += acb_npe[i];
    }

    double ToF       =(trig_rise[0]+trig_rise[1])/2.-(trig_rise[2]+trig_rise[3])/2.;
    double TDiff_top = trig_rise[0]-trig_rise[1] - 0.707014;
    double TDiff_bot = trig_rise[2]-trig_rise[3] - 0.928838;
    double x_top = TDiff_top * 0.2;//Unit:m, Assume light speed in scinti. = 0.2m/ns
    double x_bot = TDiff_bot * 0.2;//Unit:m, Assume light speed in scinti. = 0.2m/ns
    double x_mean = (x_top+x_bot)/2.;
    for(int i=0;i<4;i++){
      h_Trig_chi2[i]   ->Fill(trig_chi2[i]);
    }
    h2_TDiff_top_bot ->Fill(TDiff_top, TDiff_bot);
    h2_xpos_top_bot ->Fill(x_top, x_bot);
    if(fabs(x_top-x_bot)<0.3)vertical_flag=true;

    if(chi2_flag&&vertical_flag){
      total_ev++;
      if(npe_sum>0.4)pe_ev++;
      h_Trig_ToF ->Fill(ToF);
      h2_ToF_Q1  ->Fill(ToF,trig_sum[0]);
      h2_ToF_Q2  ->Fill(ToF,trig_sum[1]);
      h2_ToF_Q3  ->Fill(ToF,trig_sum[2]);
      h2_ToF_Q4  ->Fill(ToF,trig_sum[3]);
      for(int i=0;i<4;i++){
        h_Trig_chi2_cut[i]   ->Fill(trig_chi2[i]);
      }

      h2_TDiff_top_bot_cut ->Fill(TDiff_top, TDiff_bot);
      h2_xpos_top_bot_cut ->Fill(x_top, x_bot);
      h_Trig_TDiff_top ->Fill(TDiff_top);
      h_Trig_TDiff_bot ->Fill(TDiff_bot);
      h_Trig_xpos      ->Fill(x_mean);
      h_Trig_xpos_top  ->Fill(x_top);
      h_Trig_xpos_bot  ->Fill(x_bot);

      h_npe_sum->Fill(npe_sum);
      h2_npe_TDiff_top ->Fill(TDiff_top,npe_sum);
      h2_npe_TDiff_bot ->Fill(TDiff_bot,npe_sum);

      h2_AC1npe_xpos  ->Fill((x_top+x_bot)/2.,acu_npe[0] +acb_npe[0] );
      h2_AC5npe_xpos  ->Fill((x_top+x_bot)/2.,acu_npe[4] +acb_npe[4] );
      h2_AC12npe_xpos ->Fill((x_top+x_bot)/2.,acu_npe[11]+acb_npe[11]);

      for(int i=0;i<5;i++){//i:xpos
        if(x_mean>x_min[i]&&x_mean<x_max[i]){
          total_ev_x[i]++;
          h_Trig_xpos_slice[i]    ->Fill(x_mean);
          h_Trig_xpos_top_slice[i]->Fill(x_top);
          h_Trig_xpos_bot_slice[i]->Fill(x_bot);
          for(int j=0;j<5;j++){//j:npe threshold
            if(npe_sum>pe_th[j])pe_ev_x[j][i]++;

            for(int k=0;k<12;k++){//k:PMT num
              if((acu_npe[k])>pe_th_1PMT[j])pe_ev_x_1PMTu[j][i][k]++;
              if((acb_npe[k])>pe_th_1PMT[j])pe_ev_x_1PMTb[j][i][k]++;
            }
          }
        }
      }
    }//end of if(flags)

  }//event for loop
  cout<<"end of event loop"<<endl;

  cout<<"aa "<<pe_ev<<" / "<<total_ev<<endl;
    cout<<"x slice"<<endl;
  for(int i=0;i<5;i++){
    cout<< x_min[i]<<" ~ "<<x_max[i]<<endl;

    for(int j=0;j<5;j++){
      cout<<pe_th[j]<<"  "<<pe_ev_x[j][i]<<" / "<<total_ev_x[i]<<endl;
      gr_effAC[j]->SetPoint(i,(x_min[i]+x_max[i])/2., (double)pe_ev_x[j][i]/total_ev_x[i]);
      gr_effAC[j]->SetPointError(i,0.,(double)sqrt(pe_ev_x[j][i]*(total_ev_x[i]-pe_ev_x[j][i])/total_ev_x[i])/total_ev_x[i]);

      for(int k=0;k<12;k++){
        gr_effAC_1PMTu[k][j]->SetPoint(i,(x_min[i]+x_max[i])/2., (double)pe_ev_x_1PMTu[j][i][k]/total_ev_x[i]);
        gr_effAC_1PMTb[k][j]->SetPoint(i,(x_min[i]+x_max[i])/2., (double)pe_ev_x_1PMTb[j][i][k]/total_ev_x[i]);
      }
    }
  }

  ////////
  //Draw//
  ////////
  TCanvas *tc[4];
  for(int i=0;i<4;i++){
    tc[i] = new TCanvas(Form("tc%d",i+1),Form("tc%d",i+1),1000,700);
  }

  cout<<"after make canvas"<<endl;
  tc[0]->Divide(4,4);
  tc[0]->cd(1) ;gPad->SetLogy(1);h_Trig_ToF      ->Draw("");
  tc[0]->cd(2) ;gPad->SetLogy(1);h_Trig_TDiff_top->Draw("");
  tc[0]->cd(3) ;gPad->SetLogy(1);h_Trig_TDiff_bot->Draw("");
  tc[0]->cd(4) ;gPad->SetLogz(0);h2_TDiff_top_bot_cut->Draw("colz");
  tc[0]->cd(5) ;gPad->SetLogy(1);h_npe_sum       ->Draw("");
  tc[0]->cd(6) ;gPad->SetLogz(1);h2_npe_TDiff_top->Draw("colz");
  tc[0]->cd(7) ;gPad->SetLogz(1);h2_npe_TDiff_bot->Draw("colz");
  tc[0]->cd(8) ;gPad->SetLogz(1);h2_ToF_Q1       ->Draw("colz");
  tc[0]->cd(9) ;gPad->SetLogz(1);h2_ToF_Q2       ->Draw("colz");
  tc[0]->cd(10);gPad->SetLogz(1);h2_ToF_Q3       ->Draw("colz");
  tc[0]->cd(11);gPad->SetLogz(1);h2_ToF_Q4       ->Draw("colz");
  for(int i=0;i<4;i++){
   tc[0]->cd(12+i);gPad->SetLogy(1);h_Trig_chi2[i]   ->Draw();h_Trig_chi2_cut[i]   ->Draw("same");
  }

  tc[1]->Divide(4,4);
  tc[1]->cd(1) ;gPad->SetLogz(0);h2_TDiff_top_bot    ->Draw("colz");
  tc[1]->cd(2) ;gPad->SetLogz(0);h2_TDiff_top_bot_cut->Draw("colz");
  tc[1]->cd(3) ;gPad->SetLogz(0);h2_xpos_top_bot    ->Draw("colz");
  tc[1]->cd(4) ;gPad->SetLogz(0);h2_xpos_top_bot_cut->Draw("colz");
  tc[1]->cd(5) ;gPad->SetLogy(1);h_Trig_xpos  ->Draw("");for(int i=0;i<5;i++){h_Trig_xpos_slice[i]  ->Draw("same");}
  tc[1]->cd(6) ;gPad->SetLogy(1);h_Trig_xpos_top  ->Draw("");for(int i=0;i<5;i++){h_Trig_xpos_top_slice[i]  ->Draw("same");}
  tc[1]->cd(7) ;gPad->SetLogy(1);h_Trig_xpos_bot  ->Draw("");for(int i=0;i<5;i++){h_Trig_xpos_bot_slice[i]  ->Draw("same");}

  tc[1]->cd(8) ;h_frame->Draw();for(int i=0;i<5;i++){gr_effAC[i]  ->Draw("sameP");}
               leg_AC ->Draw("same");
  tc[1]->cd(9) ;gPad->SetLogz(1);h2_AC1npe_xpos  ->Draw("colz");
  tc[1]->cd(10);gPad->SetLogz(1);h2_AC5npe_xpos  ->Draw("colz");
  tc[1]->cd(11);gPad->SetLogz(1);h2_AC12npe_xpos ->Draw("colz");

  tc[2]->Divide(4,3);
  for(int i=0;i<12;i++){
    tc[2]->cd(i+1);h_frame_1PMTu[i]->Draw();for(int j=0;j<5;j++){gr_effAC_1PMTu[i][j]->Draw("sameP");}
                   leg_AC_1PMT ->Draw("same");
  }

  tc[3]->Divide(4,3);
  for(int i=0;i<12;i++){
    tc[3]->cd(i+1);h_frame_1PMTb[i]->Draw();for(int j=0;j<5;j++){gr_effAC_1PMTb[i][j]->Draw("sameP");}
                   leg_AC_1PMT ->Draw("same");
  }

  string ofname_pdf = ofname;
  ofname_pdf.erase(ofname_pdf.size()-5);
  ofname_pdf.append(".pdf");

  tc[0]->Print(Form("%s[",ofname_pdf.c_str()));
  tc[0]->Print(Form("%s" ,ofname_pdf.c_str()));
  tc[1]->Print(Form("%s" ,ofname_pdf.c_str()));
  tc[2]->Print(Form("%s" ,ofname_pdf.c_str()));
  tc[3]->Print(Form("%s" ,ofname_pdf.c_str()));
  tc[3]->Print(Form("%s]",ofname_pdf.c_str()));
  cout<<ofname_pdf <<" saved"<<endl;

  for(int i=0;i<5;i++){
    ofp->Add(gr_effAC[i]);
    for(int j=0;j<12;j++){
      ofp->Add(gr_effAC_1PMTu[j][i]);
      ofp->Add(gr_effAC_1PMTb[j][i]);
    }
  }
  ofp->Write(); ofp->Close();
  gSystem->Exit(1);
  theApp->Run();

  return 0;

}

