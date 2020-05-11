#include "TMath.h"

using namespace TMath;

float Length(float area = 2500, char shape = 'S'){

  if     ( shape == 'S')
    return Length_Square(area);
  else if( shape == 'C')
    return Diameter_Circle(area);
  else
    return -1;
}

float Length_Square(float area){
  return Sqrt(area);
}

float Diameter_Circle(float area){
  return 2*Sqrt(area/Pi());
}
