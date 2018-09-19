#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

#include "Tree.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++//
Tree::Tree()
{
  tree = new TChain("T");
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++//
Tree::~Tree()
{
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Tree::add(string ifname)
{
  tree->Add(ifname.c_str());
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void Tree::readtree()
{
  tree->SetBranchStatus("*",0);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw00"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw00"       ,&Ndata00);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw01"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw01"       ,&Ndata01);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw02"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw02"       ,&Ndata02);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw03"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw03"       ,&Ndata03);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw04"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw04"       ,&Ndata04);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw05"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw05"       ,&Ndata05);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw06"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw06"       ,&Ndata06);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw07"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw07"       ,&Ndata07);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw08"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw08"       ,&Ndata08);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw09"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw09"       ,&Ndata09);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw10"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw10"       ,&Ndata10);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw11"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw11"       ,&Ndata11);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw12"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw12"       ,&Ndata12);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw13"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw13"       ,&Ndata13);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw14"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw14"       ,&Ndata14);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw15"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw15"       ,&Ndata15);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw16"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw16"       ,&Ndata16);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw17"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw17"       ,&Ndata17);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw18"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw18"       ,&Ndata18);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw19"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw19"       ,&Ndata19);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw20"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw20"       ,&Ndata20);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw21"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw21"       ,&Ndata21);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw22"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw22"       ,&Ndata22);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw23"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw23"       ,&Ndata23);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw24"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw24"       ,&Ndata24);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw25"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw25"       ,&Ndata25);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw26"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw26"       ,&Ndata26);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw27"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw27"       ,&Ndata27);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw28"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw28"       ,&Ndata28);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw29"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw29"       ,&Ndata29);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw30"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw30"       ,&Ndata30);
  tree->SetBranchStatus("Ndata.sbs.hcal.a.raw31"       ,0);tree->SetBranchAddress("Ndata.sbs.hcal.a.raw31"       ,&Ndata31);
  tree->SetBranchStatus("sbs.hcal.a.raw00"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw00"             ,raw00);
  tree->SetBranchStatus("sbs.hcal.a.raw01"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw01"             ,raw01);
  tree->SetBranchStatus("sbs.hcal.a.raw02"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw02"             ,raw02);
  tree->SetBranchStatus("sbs.hcal.a.raw03"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw03"             ,raw03);
  tree->SetBranchStatus("sbs.hcal.a.raw04"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw04"             ,raw04);
  tree->SetBranchStatus("sbs.hcal.a.raw05"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw05"             ,raw05);
  tree->SetBranchStatus("sbs.hcal.a.raw06"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw06"             ,raw06);
  tree->SetBranchStatus("sbs.hcal.a.raw07"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw07"             ,raw07);
  tree->SetBranchStatus("sbs.hcal.a.raw08"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw08"             ,raw08);
  tree->SetBranchStatus("sbs.hcal.a.raw09"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw09"             ,raw09);
  tree->SetBranchStatus("sbs.hcal.a.raw10"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw10"             ,raw10);
  tree->SetBranchStatus("sbs.hcal.a.raw11"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw11"             ,raw11);
  tree->SetBranchStatus("sbs.hcal.a.raw12"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw12"             ,raw12);
  tree->SetBranchStatus("sbs.hcal.a.raw13"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw13"             ,raw13);
  tree->SetBranchStatus("sbs.hcal.a.raw14"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw14"             ,raw14);
  tree->SetBranchStatus("sbs.hcal.a.raw15"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw15"             ,raw15);
  tree->SetBranchStatus("sbs.hcal.a.raw16"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw16"             ,raw16);
  tree->SetBranchStatus("sbs.hcal.a.raw17"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw17"             ,raw17);
  tree->SetBranchStatus("sbs.hcal.a.raw18"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw18"             ,raw18);
  tree->SetBranchStatus("sbs.hcal.a.raw19"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw19"             ,raw19);
  tree->SetBranchStatus("sbs.hcal.a.raw20"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw20"             ,raw20);
  tree->SetBranchStatus("sbs.hcal.a.raw21"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw21"             ,raw21);
  tree->SetBranchStatus("sbs.hcal.a.raw22"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw22"             ,raw22);
  tree->SetBranchStatus("sbs.hcal.a.raw23"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw23"             ,raw23);
  tree->SetBranchStatus("sbs.hcal.a.raw24"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw24"             ,raw24);
  tree->SetBranchStatus("sbs.hcal.a.raw25"             ,1);tree->SetBranchAddress("sbs.hcal.a.raw25"             ,Trig_1L_raw);//Trig. scinti.
  tree->SetBranchStatus("sbs.hcal.a.raw26"             ,1);tree->SetBranchAddress("sbs.hcal.a.raw26"             ,Trig_1R_raw);//Trig. scinti.
  tree->SetBranchStatus("sbs.hcal.a.raw27"             ,1);tree->SetBranchAddress("sbs.hcal.a.raw27"             ,Trig_2L_raw);//Trig. scinti.
  tree->SetBranchStatus("sbs.hcal.a.raw28"             ,1);tree->SetBranchAddress("sbs.hcal.a.raw28"             ,Trig_2R_raw);//Trig. scinti.
  tree->SetBranchStatus("sbs.hcal.a.raw29"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw29"             ,raw29);
  tree->SetBranchStatus("sbs.hcal.a.raw30"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw30"             ,raw30);
  tree->SetBranchStatus("sbs.hcal.a.raw31"             ,0);tree->SetBranchAddress("sbs.hcal.a.raw31"             ,raw31);
  tree->SetBranchStatus("sbs.hcal.blksum"              ,1);tree->SetBranchAddress("sbs.hcal.blksum"              ,sum);//QDC like
}

