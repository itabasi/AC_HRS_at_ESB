void aa(){
TCanvas *c1 = new TCanvas("c1","c1",900,900);
c1->Divide(6,6);
for(int i=0;i<32;i++){
  c1->cd(i+1);
  gPad->SetLogy(1);
  T->Draw(Form("sbs.hcal.blksum[%d]",i),"","");
}

}
