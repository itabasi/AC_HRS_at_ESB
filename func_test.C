double exp_ln(double *x, double *par){
  //par[0]=peak
  //par[1]=assymetry
  //par[2]=width
  //par[3]=offset

  double xx=x[0];
  double val = par[0]*TMath::Exp(-0.5*TMath::Power( TMath::Log(xx/par[1])/par[2],2. ) )+par[3];
  return val;
 // return par[0]+par[1]*x+par[2]*x*x+par[3]*x*x*x;
}
//++++++++++++++++++++++++++++//
void func_test(){
  //TF1 *f1 = new TF1("f1","landau",0.,100);
  TF1 *f1 = new TF1("f1",exp_ln,0,100,4);
  //TF1 *f1 = new TF1("f1","gaus",0,100);
  double param[4]={10,30,2,0};
  f1->SetParameters(&param[0]);
  f1->Draw();
}
