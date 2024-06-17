#ifndef PARTICLENET_MREG_H
#define PARTICLENET_MREG_H

#include "TString.h"
#include "TRandom3.h"

double massSmear(double mass, unsigned long lumi, unsigned long event, int year, std::string var, double sigma=1.)
{
  if(year==2018)
  {
    if(var=="nominal") sigma=0.1;
    else if (var=="up") sigma=0.4;
    else if (var=="down") sigma=0;  
  } 
  else if(year==2017)
  {       
    if(var=="nominal") sigma=0.09;
    else if (var=="up") sigma=0.39;
    else if (var=="down") sigma=0;
  } 
  else if (year==2016)
  {       
    if(var=="nominal") sigma=0.15;
    else if (var=="up") sigma=0.45;
    else if (var=="down") sigma=0;
  } 
  else if(year==-2016)
  {       
    if(var=="nominal") sigma=0.13;
    else if (var=="up") sigma=0.43;
    else if (var=="down") sigma=0;
  } 
  sigma=0.1*sigma; //default is 1
  TRandom3 rnd((lumi << 10) + event);
  return rnd.Gaus(1, sigma)*mass;
}

double massScale(double mass, int year, std::string var, double scaleVal=1.0) 
{ 
  if(year==2018)
  {       
    if(var=="nominal") scaleVal=-0.04;
    else if (var=="up") scaleVal=0.06;
    else if (var=="down") scaleVal=-0.14;
  } 
  else if(year==2017)
  {
    if(var=="nominal") scaleVal=-0.11;
    else if (var=="up") scaleVal=0;
    else if (var=="down") scaleVal=-0.22;
  }
  else if(year==2016)
  {
    if(var=="nominal") scaleVal=-0.24;
    else if (var=="up") scaleVal=0;
    else if (var=="down") scaleVal=-0.48;
  }
  else if(year==-2016)
  {
    if(var=="nominal") scaleVal=-0.3;
    else if (var=="up") scaleVal=0;
    else if (var=="down") scaleVal=-0.6;
  }
  scaleVal=0.05*scaleVal+1; //1.05 is default
  return scaleVal*mass; 
}

double jms_jmr(double mass, unsigned long lumi, unsigned long event, int year, std::string variation)
{
 std::string jmr="nominal";
 std::string jms="nominal";
 if(variation=="jmr_up") jmr="up";
 else if (variation=="jmr_dn") jmr="down";
 else if (variation=="jms_up") jms="up";
 else if (variation=="jms_dn") jms="down";
 mass = massScale(mass, year, jms);
 mass = massSmear(mass, lumi, event, year, jmr);
 return mass;
}
#endif
