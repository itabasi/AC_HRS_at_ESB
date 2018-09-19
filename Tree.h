#ifndef Tree_h
#define Tree_h

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#include "TApplication.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

class Tree
{
public:
  Tree();
  ~Tree();

public:
  int Ndata00,Ndata01,Ndata02,Ndata03,Ndata04,Ndata05,Ndata06,Ndata07,Ndata08,Ndata09;
  int Ndata10,Ndata11,Ndata12,Ndata13,Ndata14,Ndata15,Ndata16,Ndata17,Ndata18,Ndata19;
  int Ndata20,Ndata21,Ndata22,Ndata23,Ndata24,Ndata25,Ndata26,Ndata27,Ndata28,Ndata29;
  int Ndata30,Ndata31;
  double raw00[60],raw01[60],raw02[60],raw03[60],raw04[60],raw05[60],raw06[60],raw07[60],raw08[60],raw09[60];
  double raw10[60],raw11[60],raw12[60],raw13[60],raw14[60],raw15[60],raw16[60],raw17[60],raw18[60],raw19[60];
  double raw20[60],raw21[60],raw22[60],raw23[60],raw24[60],raw25[60],raw26[60],raw27[60],raw28[60],raw29[60];
  double raw30[60],raw31[60];

  double sum[32];

  double Trig_1L_raw[60],Trig_1R_raw[60];
  double Trig_2L_raw[60],Trig_2R_raw[60];

public:
  TChain *tree;

  void readtree();
  void add(string ifname);
  int GetEntries()    const { return tree->GetEntries(); }

private:

};

#endif

