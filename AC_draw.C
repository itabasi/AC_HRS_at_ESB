#include "Setting.cc"
double exp_ln(double *x, double *par){
  //par[0]=peak height
  //par[1]=assym
  //par[2]=width2
  //par[3]=offset

  double xx=x[0];
  double val = par[0]*TMath::Exp(-0.5*TMath::Power( TMath::Log(xx/par[1])/par[2],2. ) )+par[3];
  return val;
}
/////////////////////////////////
int AC_draw(int runnum=111){

  //TFile *ifp = new TFile(Form("rootfiles/fadcACSIS_%04d.root",runnum ) );
  TFile *ifp = new TFile(Form("rootfiles/fadcAERO_%03d.root",runnum ) );
  TTree *tree=(TTree*)ifp->Get("T");

  Setting *set = new Setting();
  TLine *line = new TLine(0,0,1,1);
  line -> SetLineColor(6);
  line -> SetLineWidth(1.0);
  line -> SetLineStyle(1);
  double raw[100];
  int ch=20;
  //cout<<"type ch num.(0--31)"<<endl;
  //cin>>ch;

  switch(ch){
    case 0:
      tree->SetBranchAddress("sbs.hcal.a.raw00"             ,raw);
    break;
    case 1:
      tree->SetBranchAddress("sbs.hcal.a.raw01"             ,raw);
    break;
    case 2:
      tree->SetBranchAddress("sbs.hcal.a.raw02"             ,raw);
    break;
    case 3:
      tree->SetBranchAddress("sbs.hcal.a.raw03"             ,raw);
    break;
    case 4:
      tree->SetBranchAddress("sbs.hcal.a.raw04"             ,raw);
    break;
    case 5:
      tree->SetBranchAddress("sbs.hcal.a.raw05"             ,raw);
    break;
    case 6:
      tree->SetBranchAddress("sbs.hcal.a.raw06"             ,raw);
    break;
    case 7:
      tree->SetBranchAddress("sbs.hcal.a.raw07"             ,raw);
    break;
    case 8:
      tree->SetBranchAddress("sbs.hcal.a.raw08"             ,raw);
    break;
    case 9:
      tree->SetBranchAddress("sbs.hcal.a.raw09"             ,raw);
    break;
    case 10:
      tree->SetBranchAddress("sbs.hcal.a.raw10"             ,raw);
    break;
    case 11:
      tree->SetBranchAddress("sbs.hcal.a.raw11"             ,raw);
    break;
    case 12:
      tree->SetBranchAddress("sbs.hcal.a.raw12"             ,raw);
    break;
    case 13:
      tree->SetBranchAddress("sbs.hcal.a.raw13"             ,raw);
    break;
    case 14:
      tree->SetBranchAddress("sbs.hcal.a.raw14"             ,raw);
    break;
    case 15:
      tree->SetBranchAddress("sbs.hcal.a.raw15"             ,raw);
    break;
    case 16:
      tree->SetBranchAddress("sbs.hcal.a.raw16"             ,raw);
    break;
    case 17:
      tree->SetBranchAddress("sbs.hcal.a.raw17"             ,raw);
    break;
    case 18:
      tree->SetBranchAddress("sbs.hcal.a.raw18"             ,raw);
    break;
    case 19:
      tree->SetBranchAddress("sbs.hcal.a.raw19"             ,raw);
    break;
    case 20:
      tree->SetBranchAddress("sbs.hcal.a.raw20"             ,raw);
    break;
    case 21:
      tree->SetBranchAddress("sbs.hcal.a.raw21"             ,raw);
    break;
    case 22:
      tree->SetBranchAddress("sbs.hcal.a.raw22"             ,raw);
    break;
    case 23:
      tree->SetBranchAddress("sbs.hcal.a.raw23"             ,raw);
    break;
    case 24:
      tree->SetBranchAddress("sbs.hcal.a.raw24"             ,raw);
    break;
    case 25:
      tree->SetBranchAddress("sbs.hcal.a.raw25"             ,raw);
    break;
    case 26:
      tree->SetBranchAddress("sbs.hcal.a.raw26"             ,raw);
    break;
    case 27:
      tree->SetBranchAddress("sbs.hcal.a.raw27"             ,raw);
    break;
    case 28:
      tree->SetBranchAddress("sbs.hcal.a.raw28"             ,raw);
    break;
    case 29:
      tree->SetBranchAddress("sbs.hcal.a.raw29"             ,raw);
    break;
    case 30:
      tree->SetBranchAddress("sbs.hcal.a.raw30"             ,raw);
    break;
    case 31:
      tree->SetBranchAddress("sbs.hcal.a.raw31"             ,raw);
    break;
    default:
      cout<<"!!"<<endl;
    return 0;
  }
cout<<"set"<<endl;

  double x[60]; //x axis(time)
  int np=60;
  for(int i=0;i<60;i++){
    x[i]=i+1;
  }

  TCanvas *tc = new TCanvas("tc","tc",900,600);
  
  int n=tree->GetEntries();
  for(int i=0;i<100;i++){
    tree->GetEntry(i);
    tc->Clear();
    TGraph *tg_wave = new TGraph(np,x,raw);
    set->SetGr(tg_wave,Form("ch%d",ch),"time[ch]","fADC[ch]",4,1,22,1.5);
    TF1 *f1 = new TF1("f1",exp_ln,0.1,100,4);
    set->SetTF1(f1,2,1,1.5);
    double param[4]={10,2,5,100};
    f1->SetParameters(&param[0]);
    f1->SetNpx(2000);
    //tg_wave->Fit(f1);

    double peak,peak_x,offset,rise_point;

/*
    TF1 *f2 = new TF1("f2","landau+[3]",0.1,100);
    set->SetTF1(f2,4,2,1.5);
    f2->SetParameters(100,9,10,90);
    f2->SetNpx(2000);
    tg_wave->Fit(f2);
*/

    //f2->Draw("same");

    peak   = f1->GetMaximum(0,100);
    peak_x = f1->GetMaximumX(0,100);
    offset = f1->GetParameter(3);
    rise_point = f1->GetX((peak-offset)*0.1+offset, 0,peak_x);
    cout<<"++++++++++++++"<<endl;
    cout<<Form("peak %.0lf, peak_x %.0lf, offset %.0lf, rise_point %.2lf",peak,peak_x,offset,rise_point)<<endl;
    cout<<"event "<<i<<" / "<<n<<endl;


    tg_wave->GetXaxis()->SetRangeUser(0,peak_x+30);
    tg_wave->Draw("APL");
    f1->Draw("same");
    line ->DrawLine(rise_point,(peak-offset)*0.1+offset ,rise_point,0);
    line ->DrawLine(rise_point,(peak-offset)*0.1+offset ,0,(peak-offset)*0.1+offset);
    tc->Update();
    getchar();

  }

return 0;
}




