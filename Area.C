#include "TMath.h"
#include "Area.h"

using namespace TMath;

float Area(float length = 50., char shape = 's'){
  if     ( shape == 's')
    return Area_Square(length);
  else if( shape == 'c')
    return Area_Circle(length);
  else
    return -1;
}

float Area_Square(float length){
  return length*length;
}

float Area_Circle(float diameter){
  return Pi()*diameter/2.*diameter/2.;
}
