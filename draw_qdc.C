#include "Setting.cc"
#include "ParamMan.cc"

int draw_qdc(){
  gROOT->SetBatch(1);

  ///////////
  //Setting//
  ///////////
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.13);
  Setting *set = new Setting();

  TH1F *h_Q, *h_npe;
  TLine *line;
  line = new TLine(0,0,0,0);
  line ->SetLineColor(6);
  line ->SetLineWidth(1.0);
  line ->SetLineStyle(1);

  TLatex *latex=new TLatex();
  set->SetTLatex(latex,1,0.05,11);

  int runnum = 126;
  int cid =1;//1:AC
  int seg = 6;//1--12
  int tb =0;//top=0,bottom=1
  string ifname,param_name,top_or_bottom;
  ifname = Form("rootfiles/calib/r%03d_calib.root",runnum);
  param_name = Form("param/r%04d.param_tuned",runnum);
  cout<<"input file "<<ifname<<endl;
  cout<<"input param file "<<param_name<<endl;
  TFile *file = new TFile(ifname.c_str());
  ParamMan *param = new ParamMan(param_name.c_str());
  param -> Initialize();
  double offset = param -> GetAdcOffset(cid,seg,tb);
  double gain   = param -> GetAdcGain(cid,seg,tb);
  double spe_peak = offset+1./gain;

  switch(tb){
  case 0:
    h_Q   = (TH1F*)file -> Get(Form("h_Q_ACu%d"  ,seg));
    h_npe = (TH1F*)file -> Get(Form("h_NPE_ACu%d",seg));
    top_or_bottom = "t";
    break;
  case 1:
    h_Q   = (TH1F*)file -> Get(Form("h_Q_ACb%d"  ,seg));
    h_npe = (TH1F*)file -> Get(Form("h_NPE_ACb%d",seg));
    top_or_bottom = "b";
    break;
  default:
    cout<<"tb should be 0 or 1!!!"<<endl;
    return 0;
  }
    set->SetTH1(h_Q  ,"","Charge[ch]"   ,"counts",1,3000,0);
    set->SetTH1(h_npe,"","Num. of P.E." ,"counts",1,3000,0);
    h_Q   -> SetStats(0);
    h_npe -> SetStats(0);
    h_npe -> Rebin(3);

    h_Q   -> GetXaxis()->SetRange(h_Q  ->FindFirstBinAbove(1)-10,h_Q  ->FindLastBinAbove(2)+10);
    h_npe -> GetXaxis()->SetRange(h_npe->FindFirstBinAbove(1)-10,h_npe->FindLastBinAbove(2)+10);

  ////////
  //Draw//
  ////////
  TCanvas *c[2];
  for(int i=0;i<2;i++){
    c[i] = new TCanvas(Form("c%d",i+1),Form("c%d",i+1),800,600);
  }

  c[0]->Clear();c[0]->cd(1);
  gPad->SetLogy(1);
  h_Q->Draw("");
  line->DrawLine(offset,0,offset,h_Q->GetBinContent(h_Q->FindBin(offset)));
  line->DrawLine(spe_peak,0,spe_peak,h_Q->GetBinContent(h_Q->FindBin(spe_peak)));
  latex->DrawLatex(offset,  h_Q->GetBinContent(h_Q->FindBin(offset))  ,"offset");
  latex->DrawLatex(spe_peak,h_Q->GetBinContent(h_Q->FindBin(spe_peak)),"S.P.E.");

  c[1]->Clear();c[1]->cd(1);
  gPad->SetLogy(1);
  h_npe->Draw("");


  string pdf_name=Form("pdf/r%04d_AC%d%s_Q_npe.pdf",runnum,seg,top_or_bottom.c_str());
  c[0] ->Print(Form("%s[",pdf_name.c_str()));
  c[0] ->Print(Form("%s" ,pdf_name.c_str()));
  c[1] ->Print(Form("%s" ,pdf_name.c_str()));
  c[1] ->Print(Form("%s]",pdf_name.c_str()));

return 1;
}
