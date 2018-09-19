#include "Setting.cc"
void draw_histo(){
  gROOT->SetBatch(0);

  ///////////
  //Setting//
  ///////////
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.13);
  Setting *set = new Setting();

  string ifname;
  //ifname = "pdf/r126_129_ana2.root";
  ifname = "pdf/r117_ana2.root";
  TFile *file = new TFile(ifname.c_str());

  TH1F *h_Trig_ToF;
  TH1F *h_Trig_xpos;
  TH1F *h_Trig_xpos_slice[5];
  TH1F *h_Trig_chi2[4],*h_Trig_chi2_cut[4];
  TH1F *h_npe_sum;

  TH2F *h2_xpos_top_bot, *h2_xpos_top_bot_cut;
  //////////////////
  //Get histograms//
  //////////////////
  h_Trig_ToF=(TH1F*)file->Get("h_Trig_ToF");
  set->SetTH1(h_Trig_ToF,"","ToF[ns]","counts/40ps",1,3000,0);
  h_Trig_ToF->GetXaxis()->SetRangeUser(-10,10);

  h_Trig_xpos=(TH1F*)file->Get("h_Trig_xpos");
  set->SetTH1(h_Trig_xpos,"","x pos[m]","counts/4cm",1,3000,0);

  h_npe_sum=(TH1F*)file->Get("h_npe_sum");
  set->SetTH1(h_npe_sum,"","Num. of P.E.","counts/0.04P.E.",1,3000,0);
  h_npe_sum->GetXaxis()->SetRangeUser(-4,20);

  for(int i=0;i<5;i++){
    h_Trig_xpos_slice[i] =(TH1F*)file->Get(Form("h_Trig_xpos_slice%d",i));
  }
#if 1
  for(int i=0;i<4;i++){
    h_Trig_chi2[i]     =(TH1F*)file->Get(Form("h_Trig_chi2%d",i));
    h_Trig_chi2_cut[i] =(TH1F*)file->Get(Form("h_Trig_chi2_cut%d",i));
    set->SetTH1(h_Trig_chi2[i]          , "", "#chi^{2}", "counts", 1, 3000, 0);
  }

  h2_xpos_top_bot     =(TH2F*)file->Get("h2_xpos_top_bot");
  h2_xpos_top_bot_cut =(TH2F*)file->Get("h2_xpos_top_bot_cut");
  set->SetTH2(h2_xpos_top_bot    ,"","x_{top}[m]","x_{bottom}[m]");
  set->SetTH2(h2_xpos_top_bot_cut,"","x_{top}[m]","x_{bottom}[m]");

  //Fit
  TF1 *f1 = new TF1("f1","gaus",-20,20);
  set->SetTF1(f1,2,1,2);
  h_Trig_ToF->Fit(f1,"QR","");
  double param[3];
  f1->GetParameters(&param[0]);
  cout<<"mean --- sigma ---"<<endl;
  cout<<param[1]<<"  "<<param[2]<<endl;

  ////////
  //Draw//
  ////////
  TCanvas *c[6];
  for(int i=0;i<6;i++){
    c[i] = new TCanvas(Form("c%d",i+1),Form("c%d",i+1),800,600);
  }


  c[0]->Clear();c[0]->cd(1);
  h_Trig_ToF->SetStats(0);
  h_Trig_ToF->Draw();

  c[1]->Clear();c[1]->cd(1);
  h_Trig_xpos->SetStats(0);
  h_Trig_xpos->Draw();
  for(int i=0;i<5;i++){
    h_Trig_xpos_slice[i]->Draw("same");
  }

  c[2]->Clear();c[2]->cd(1);
  gPad->SetLogy(1);
  h_Trig_chi2[0]->SetStats(0);
  h_Trig_chi2[0]->Draw("");
  h_Trig_chi2_cut[0]->Draw("same");

  c[3]->Clear();c[3]->cd(1);
  gPad->SetLogy(1);
  h_npe_sum->SetStats(0);
  h_npe_sum->Draw();

  c[4]->Clear();c[4]->cd(1);
  h2_xpos_top_bot->Draw("colz");
  c[5]->Clear();c[5]->cd(1);
  h2_xpos_top_bot_cut->Draw("colz");

  ///////////////
  //Save canvas//
  ///////////////
  string ofname_pdf = ifname;
  ofname_pdf.erase(ifname.size()-5);
  ofname_pdf.append("_histo.pdf");
  c[0]->Print(Form("%s[",ofname_pdf.c_str()));
  for(int i=0;i<6;i++){
  c[i]->Print(Form("%s" ,ofname_pdf.c_str()));
  }
  c[5]->Print(Form("%s]",ofname_pdf.c_str()));
#endif
}
