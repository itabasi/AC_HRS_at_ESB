#include "Setting.cc"
void draw_graphs(){
  gROOT->SetBatch(1);

  ///////////
  //Setting//
  ///////////
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.13);
  double pe_th[5] = {  0.5,  1.,  3.,  5.,  10};//sum
  double pe_th_1PMT[5] = {  0.1,  0.2,  0.3,  0.4,  0.5};//specifit PMT
  Setting *set = new Setting();

  string ifname;
  ifname = "pdf/r126_129_ana2.root";
  //ifname = "pdf/r117_ana2.root";
  TFile *file = new TFile(ifname.c_str());
  TGraphErrors *gr_effAC[5];
  TGraph *gr_effAC_1PMTu[12][5], *gr_effAC_1PMTb[12][5];

  TH2F *h_frame = new TH2F("h_frame","h_frame",10,-1.5,1.5,10,0,1.3);
  set->SetTH2(h_frame,"","x pos[m]","Efficiency");
  TH2F *h_frame_1PMTu[12], *h_frame_1PMTb[12];
  for(int i=0;i<12;i++){
    h_frame_1PMTu[i] = new TH2F(Form("h_frame_1PMTu%d",i+1),Form("h_frame_1PMTu%d",i+1),10,-1.5,1.5,10,0,0.5);
    h_frame_1PMTb[i] = new TH2F(Form("h_frame_1PMTb%d",i+1),Form("h_frame_1PMTb%d",i+1),10,-1.5,1.5,10,0,0.5);
    set->SetTH2(h_frame_1PMTu[i],Form("AC%dtop Ratio of P.E.>PEth",i+1)   ,"x pos[m]","N_{> p.e.}/N_{total}");
    set->SetTH2(h_frame_1PMTb[i],Form("AC%dbottom Ratio of P.E.>PEth",i+1),"x pos[m]","N_{> p.e.}/N_{total}");
  }

  TLegend *leg_AC = new TLegend(0.7,0.72,0.88,0.92);
  leg_AC->SetBorderSize(1);
  leg_AC->SetFillColor(0);
  leg_AC->SetFillStyle(1001);
  leg_AC->SetTextFont(42);

  TLegend *leg_AC_1PMT = new TLegend(0.7,0.72,0.88,0.92);
  leg_AC_1PMT ->SetBorderSize(1);
  leg_AC_1PMT ->SetFillColor(0);
  leg_AC_1PMT ->SetFillStyle(1001);
  leg_AC_1PMT ->SetTextFont(42);

  //////////////
  //Get graphs//
  //////////////
  for(int i=0;i<5;i++){
    gr_effAC[i] =(TGraphErrors*)file->Get(Form("gr_effAC%d",i+1));
    gr_effAC[i]->SetMarkerSize(1.8);
    gr_effAC[i]->SetMarkerStyle(20+i);
    if(i==4){gr_effAC[i]->SetMarkerStyle(34);gr_effAC[i]->SetMarkerColor(6);}
    leg_AC->AddEntry(gr_effAC[i],Form(">%.01lf p.e.",pe_th[i]),"p");
    for(int j=0;j<12;j++){
      gr_effAC_1PMTu[j][i] = (TGraph*)file->Get(Form("gr_effAC_1PMTu%d_%d",j+1,i+1));
      gr_effAC_1PMTu[j][i]->SetMarkerSize(1.8);
      gr_effAC_1PMTu[j][i]->SetMarkerStyle(20+i);
      if(i==4){gr_effAC_1PMTu[j][i]->SetMarkerStyle(34);gr_effAC_1PMTu[j][i]->SetMarkerColor(6);}

      gr_effAC_1PMTb[j][i] = (TGraph*)file->Get(Form("gr_effAC_1PMTb%d_%d",j+1,i+1));
      gr_effAC_1PMTb[j][i]->SetMarkerSize(1.8);
      gr_effAC_1PMTb[j][i]->SetMarkerStyle(20+i);
      if(i==4){gr_effAC_1PMTb[j][i]->SetMarkerStyle(34);gr_effAC_1PMTb[j][i]->SetMarkerColor(6);}
    }
    leg_AC_1PMT ->AddEntry(gr_effAC_1PMTu[0][i],Form(">%.01lf p.e.",pe_th_1PMT[i]),"p");
  }

  ////////
  //Draw//
  ////////
  TCanvas *c[25];
  for(int i=0;i<25;i++){
    c[i] = new TCanvas(Form("c%d",i+1),Form("c%d",i+1),800,600);
  }


  c[0]->Clear();
  c[0]->cd(1);
  h_frame->Draw();
  for(int i=0;i<5;i++){
    gr_effAC[i]->Draw("sameP");
  }
    leg_AC ->Draw("same");

  for(int i=0;i<12;i++){
    c[i+1]->Clear();
    c[i+1]->cd(1);
    h_frame_1PMTu[i]->Draw();
    for(int j=0;j<5;j++){gr_effAC_1PMTu[i][j]->Draw("sameP");}
    leg_AC_1PMT ->Draw("same");
  }

  for(int i=0;i<12;i++){
    c[i+13]->Clear();
    c[i+13]->cd(1);
    h_frame_1PMTb[i]->Draw();
    for(int j=0;j<5;j++){gr_effAC_1PMTb[i][j]->Draw("sameP");}
    leg_AC_1PMT ->Draw("same");
  }

  ///////////////
  //Save canvas//
  ///////////////
  string ofname_pdf = ifname;
  ofname_pdf.erase(ifname.size()-5);
  ofname_pdf.append("_graph.pdf");
  c[0]->Print(Form("%s[",ofname_pdf.c_str()));
  for(int i=0;i<25;i++){
  c[i]->Print(Form("%s" ,ofname_pdf.c_str()));
  }
  c[24]->Print(Form("%s]",ofname_pdf.c_str()));
}
