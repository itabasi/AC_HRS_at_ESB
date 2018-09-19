/*
  ParamMan.cc
*/


#include "ParamMan.hh"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <map>
using namespace std;

ParamMan::ParamMan( const char* filename )
  : ParamFileName(filename)
{
  AC.cid=1;
  for(int i=0; i<nAC*2; i++){
    AC.adcOffset[i]=8000.00;   AC.adcGain[i]=0.0350;
  }
}
////////////////////////////////
//ParamMan::~ParamMan()
//{
//}
///////////////////////////////////
bool ParamMan::Initialize( void )
{
  static const std::string funcname = "ParamMan::Initialize";
  FILE *fp;
  char str[MaxChar];
  
  if((fp=fopen(ParamFileName,"r"))==0){
    std::cerr << "[" << funcname << "]: file open fail" << std::endl;
    return false;
    //exit(-1);
  }
  
  int cid,seg,at,ud;
  double p0, p1;
  
  while(fgets(str,MaxChar,fp)!=0){
    if(str[0]=='#') continue;
    else if(sscanf(str,"%d %d %d %d %lf %lf",&cid,&seg,&at,&ud,
		   &p0,&p1)==6){
      if(at==0){ /* ADC */
	    if(cid==AC.cid)
	      { AC.adcOffset[seg-1+nAC*ud]=p0;
	      AC.adcGain[seg-1+nAC*ud]=p1;}
 	  else{
 	    std::cerr << "[" << funcname << "]: new fail (A) "
 		      << " Cid=" << std::setw(2) << cid
 		      << " UD=" << std::setw(2) << ud << std::endl;
 	  }
     }
	
    }   /* if(sscanf...) */
  }       /* while(fgets...) */
  fclose(fp);
  
//   std::cout << "[" << funcname << "]: Initialization finished" << std::endl;
  return true;
}

///////////////////////////////////
void ParamMan::SetAdcOffset( int cid, int seg, int ud,
				 double adcOffset )
{
  static const std::string funcname = "ParamMan::SetAdcOffset";
  
  if(cid==AC.cid)
    AC.adcOffset[seg-1+nAC*ud]=adcOffset;
  else   cerr << "[" << funcname << "]: unknown id" << endl;
}

void ParamMan::SetAdcGain( int cid, int seg, int ud,
			       double adcGain )
{
  static const std::string funcname = "ParamMan::SetAdcGain";
  
  if(cid==AC.cid)          AC.adcGain[seg-1+nAC*ud]=adcGain;
  else   cerr << "[" << funcname << "]: unknown id" << endl;
}

///////////////////////////////////
void ParamMan::SetAdcPeak( int cid, int seg, int ud,
			       double adcPeak )
{
  static const std::string funcname = "ParamMan::SetAdcPeak";
  
  if(adcPeak==0) return;
  else if(cid==AC.cid){
        AC.adcGain[seg-1+nAC*ud]=1.0/(adcPeak-AC.adcOffset[seg-1+nAC*ud]);
         }
  else   cerr << "[" << funcname << "]: unknown id" << endl;
}


///////////////////////////////////
void ParamMan::SetNpeTune( int cid, int seg, int ud,
				double npe )
{
  static const std::string funcname = "ParamMan::SetNpeTune";
  
  if(cid==AC.cid )
    AC.adcGain[seg-1+nAC*ud]/=npe;
  else   cerr << "[" << funcname << "]: unknown id" << endl;
}

///////////////////////////////////

double ParamMan::GetAdcOffset( int cid, int seg, int ud )
{
  static const std::string funcname = "ParamMan::GetAdcOffset";

  if(cid==AC.cid )          return AC.adcOffset[seg-1+nAC*ud];
  else   cerr << "[" << funcname << "]: unknown id" << endl;

  return -1.;
}

double ParamMan::GetAdcGain( int cid, int seg, int ud )
{
  static const std::string funcname = "ParamMan::GetAdcGain";

  if(cid==AC.cid )          return AC.adcGain[seg-1+nAC*ud];
  else   cerr << "[" << funcname << "]: unknown id" << endl;

  return -1.;
}

double ParamMan::npe( int cid, int seg, int ud, double adc )
{
  static const std::string funcname = "ParamMan::npe";

  if(cid==AC.cid )
    return AC.adcGain[seg-1+nAC*ud]*(adc-AC.adcOffset[seg-1+nAC*ud]);
  else   cerr << "[" << funcname << "]: unknown id" << endl;

  return -1.;
}



void ParamMan::WriteToFile(const char* OutputFileName)   //wrinting param file
{
  ofstream fout;
  if( fout.is_open() ) fout.close();
  fout.open(OutputFileName, ios::out|ios::trunc);
  fout.setf(ios_base::fixed);
//   fout.open(name.str().c_str(), std::ios::out|std::ios::trunc);
//   fout.setf(std::ios_base::fixed);
  fout << "#" << endl
       << "#  "  << OutputFileName << endl
       << "#" << endl;
  int at=0; // ADC
  fout << "# CID SEG AT  UD      Offs        Gain" << endl;
  fout << "# AC"<< endl;
  for(int k=0; k<2; k++){//k:u,d
    for(int i=0; i<nAC; i++)
      fout << std::setw(4) << CID_AC
	   << std::setw(4) << i+1
	   << std::setw(4) << at
	   << std::setw(4) << k
	   << std::setw(13) << std::setprecision(6)
	   << AC.adcOffset[i+nAC*k]
	   << std::setw(11) << std::setprecision(6)
	   << AC.adcGain[i+nAC*k] << endl;
  }
  if(fout.is_open()) fout.close();
}
