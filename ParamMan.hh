/*
  ParamMan.h
*/

#ifndef ParamMan_hh
#define ParamMan_hh 1

#include <iomanip>
#include <sstream>
#include <string>
#include <map>
//#include "define.hh"
//#include "ParamMan.hh"

const int CID_AC = 1;
const int nAC = 12;
const int MaxChar= 100;

class ParamMan
{
private:
//   std::string ParamFileName;
  const char* ParamFileName;
  char* OutputFileName;

public:
//   explicit ParamMan( const std::string & filename );
  ParamMan();
  explicit ParamMan( const char* filename );
  ~ParamMan(){}

private:
  struct Param{
    int cid;
    double adcOffset[nAC*2];
    double adcGain[nAC*2];
  };

  Param AC;

public:
  //void SetFileName( const char* ParamFileName & filename )
  void SetFileName( const char* filename )
  { ParamFileName=filename; } 
  void SetAdcOffset( int cid, int seg, int ud, double adcOffset );
  void SetAdcGain( int cid, int seg, int ud, double adcGain );
  void SetAdcPeak( int cid, int seg, int ud, double adcPeak );
  void SetNpeTune( int cid, int seg, int ud, double npe );
  
public:
  bool Initialize( void );
  double GetAdcOffset( int cid, int seg, int ud );
  double GetAdcGain( int cid, int seg, int ud );
  double npe( int cid, int seg, int ud, double adc);
  void WriteToFile( const char* OutputFileName );
};


#endif
