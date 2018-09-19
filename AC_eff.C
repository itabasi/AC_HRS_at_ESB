#include "Setting.cc"
void AC_eff(){

Setting *set = new Setting();
TGraph *gr_ACu, *gr_ACb, *gr_ACm, *gr_ACgm;
TLegend *leg_AC = new TLegend(0.7,0.7,0.90,0.90);
leg_AC->SetBorderSize(1);
leg_AC->SetFillColor(0);
leg_AC->SetFillStyle(1001);
leg_AC->SetTextFont(42);

int n=12;

double x[12];for(int i=0;i<12;i++){x[i]=i+1;}
double ratio_ACu[12] = {0.120806, 0.179235, 0.192491, 0.249966, 
                        0.282653, 0.302583, 0.246425, 0.250829, 
                        0.218368, 0.189813, 0.137922, 0.128070};
double ratio_ACb[12] = {0.111136, 0.120761, 0.171880, 0.205929, 
                        0.187588, 0.278976, 0.182639, 0.260544, 
                        0.255686, 0.199664, 0.148182, 0.158987};

double ratio_ACm[12] = {0.136106, 0.165433, 0.226676, 0.271349, 
                        0.285332, 0.327235, 0.252372, 0.274618, 
                        0.308984, 0.235030, 0.182140, 0.190720};

double ratio_ACgm[12] = {0.0315522, 0.0405412, 0.0550688, 0.0762700, 
                         0.0887093, 0.1466840, 0.0883007, 0.1042360, 
                         0.0787216, 0.0595633, 0.0417669, 0.0411767};
gr_ACu = new TGraph(n,x,ratio_ACu);
gr_ACb = new TGraph(n,x,ratio_ACb);
gr_ACm = new TGraph(n,x,ratio_ACm);
gr_ACgm = new TGraph(n,x,ratio_ACgm);
//SetGr(TGraph *gr, TString hname, TString xname, TString yname, int LColor, int MColor, int MStyle, double Yoffset){
set->SetGr(gr_ACu ,"","","",1,2,22,0.5);
set->SetGr(gr_ACb ,"","","",1,4,23,0.5);
set->SetGr(gr_ACm ,"","","",1,6,24,0.5);
set->SetGr(gr_ACgm,"","","",1,1,25,0.5);
  gr_ACu ->SetMarkerSize(1.1);
  gr_ACb ->SetMarkerSize(1.1);
  gr_ACm ->SetMarkerSize(1.1);
  gr_ACgm->SetMarkerSize(1.1);
leg_AC->AddEntry(gr_ACu ,"top"   ,"p");
leg_AC->AddEntry(gr_ACb ,"bottom","p");
leg_AC->AddEntry(gr_ACm ,"mean"  ,"p");
leg_AC->AddEntry(gr_ACgm,"gmean" ,"p");

  h_frame = new TH2F("h_frame","h_frame",10,0,13,10,0,0.5);
  set->SetTH2(h_frame,"Ratio of P.E.>0.5","AC seg","N_{>0.5p.e.}/N_{total}");
  //h_frame->GetXaxis()->SetNdivisions(000);
  //h_frame->GetYaxis()->SetNdivisions(000);
  h_frame->SetStats(0);

  h_frame->Draw();
  gr_ACu  ->Draw("Psame");
  gr_ACb  ->Draw("Psame");
  gr_ACm  ->Draw("Psame");
  gr_ACgm ->Draw("Psame");
  leg_AC  ->Draw("same");
}
