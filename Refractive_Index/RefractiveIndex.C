// C++ Includes
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>
#include <cmath>
#include <math.h>

// ROOT Includes
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"

// Defines
#define PI 3.141592654

std::default_random_engine gen(12345);
/*
std::uniform_real_distribution<double> Pos(-25.,-3.8);
std::uniform_real_distribution<double> Angle(0., PI/2);
*/
// A function to keep all angle ranges between 0 and 2PI
double AngleC(double x){
	while (x >= 2*PI){
	x -= 2*PI;
	}

	while (x < 0){
	x += 2*PI;
	}
	return x;
}

// finds angles using dot product
double Angles(double a[2], double b[2]){

  double angle = 0;
  double dot = a[0]*b[0] + a[1]*b[1];
  double aval = sqrt( pow(a[0],2) + pow(a[1],2) );
  double bval = sqrt( pow(b[0],2) + pow(b[1],2) );

  angle = acos( dot/(aval*bval) );
  return angle;

}

// A function to figure out the angle from the center
double AngFromCenter(double x, double y){
	double ang = 0;
	ang = atan2(y,x);
	return AngleC(ang);
}
// A function to figure out the angle to the center
double AngToCenter(double x, double y){
	double ang = 0;
	ang = atan2(-1*y,-1*x);
	return AngleC(ang);
}

// A function to rotate cartesian coordinated by theta clockwise
void Rotate(double *vec, double t){
	double tempX = vec[0];
	double tempY = vec[1];

	vec[0] = tempX*cos(t) + tempY*sin(t);
	vec[1] = tempY*cos(t) - tempX*sin(t);

}

void Transmit(double n1, double n2, double incident, double &transmitted ){

  // Snell's Law n1*sin(t_i) = n2*sin(t_t)
  transmitted = asin(n1*sin(incident)/n2);
}

void Fresnel(double n1, double n2, double incident, double vals[6]){
  // Make life easy R + T = 1
  // Reff (unpolarised light)  = 0.5*(Rs+Rp)
  bool canTransmit = true;
  if (n1 > n2) {
    if ( incident >= asin(n2/n1) ) canTransmit = false;
  }
  if (canTransmit){
    double transmitted;
    Transmit(n1, n2, incident, transmitted);
    double Rs = abs( (n1*cos(incident) - n2*cos(transmitted)) / (n1*cos(incident) + n2*cos(transmitted)) );
    double Rp = abs( (n1*cos(transmitted) - n2*cos(incident)) / (n1*cos(transmitted) + n2*cos(incident)) );
    Rs = pow(Rs,2);
    Rp = pow(Rp,2);

    double Ts = 1 - Rs;
    double Tp = 1 - Rp;
    double R = 0.5*(Rs+Rp);
    double T = 0.5*(Ts+Tp);

    vals[0] = Rs;
    vals[1] = Rp;
    vals[2] = Ts;
    vals[3] = Tp;
    vals[4] = R;
    vals[5] = T;
  }
  else {
    vals[0] = 1;
    vals[1] = 1;
    vals[2] = 0;
    vals[3] = 0;
    vals[4] = 1;
    vals[5] = 0;

  }
}

void AnglesOnACircle(double &t, int med = 0, double RI = 1.58, double plateL = 50, double pmtR = 3.8 ){
  // med = 0, 1, 2 --- air, water, glass
  // RI refractive index
  // t = average transmittance



  double circleRadius = pmtR; //cm
	//double circleRadius = 12.7; //cm
  int numRuns = 10000;
  int verbose = 0;


  std::uniform_real_distribution<double> Pos(-(plateL/2),-(circleRadius));
  std::uniform_real_distribution<double> Angle(0., PI/2);


  double wlsRI = RI;
	//double wlsRI = 1.58;
	double glassRI = 1.5;
	double waterRI = 1.33;
	double airRI = 1.;

	double averageRef = 0;
	double averageTran = 0;

  for (int i = 0; i < numRuns; i++){
	//if (i % 100 == 0) std::cout << "Processing event: " << i << std::endl;
		double rs = 0;
		double rp = 0;
		double ts = 0;
		double tp = 0;
		double r = 0;
		double t = 0;

		double vals[6];
    // create a random point on the axis and generate an angle between 0 and pi/2
    double pos[2] = {Pos(gen), 0};
    //double pos[2] = {-10., 0};
    double ang = Angle(gen);
    double dir[2] = {cos(ang), sin(ang)};

    // y = mx + c
    double m = dir[1]/dir[0];
    double c = pos[1] - m*pos[0];

    double circleIntercepts[2] = {-circleRadius, circleRadius};
    circleIntercepts[0] = ( (-m*c) + sqrt(( (pow(m,2)+1)*pow(circleRadius,2) ) - pow(c,2) ) ) /(pow(m,2) +1 );
    circleIntercepts[1] = ( (-m*c) - sqrt(( (pow(m,2)+1)*pow(circleRadius,2) ) - pow(c,2) ) ) /(pow(m,2) +1 );
    if ( isnan(circleIntercepts[0]) || isnan(circleIntercepts[0]) ) {i--; continue;}
    double dists[2];
    dists[0] = abs(pos[0] - circleIntercepts[0]);
    dists[1] = abs(pos[0] - circleIntercepts[1]);
    int selectedX = 0;
    if (dists[1] < dists[0] ) selectedX = 1;

    double pointOnCircle[2];
    pointOnCircle[0] = circleIntercepts[selectedX];
    pointOnCircle[1] = sqrt( pow(circleRadius, 2) - pow(circleIntercepts[selectedX], 2) );

    // tangent vector
    double angfromcent = AngFromCenter(pointOnCircle[0], pointOnCircle[1] );
    double dirfromcent[2] = {cos(angfromcent), sin(angfromcent)};
    double tangent[2] = {1, -(dirfromcent[0]/dirfromcent[1])};
    double b[2] = {1,0};
    double tangang = Angles(tangent,  b);

    double incidence = PI/2 - Angles(dir, tangent);

////////////////////////////////////////////////////////////////////////////////////////////////////
  
	double tangent1[2] = {dirfromcent[0], dirfromcent[1]};
	Rotate(tangent1, PI/2);
	double incidence1 = Angles(dir, tangent);
	if (verbose) std::cout << "Incidence(old, new):\t" << incidence << "  " << incidence1  << std::endl;
		
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
   
    if (med == 0) Fresnel(wlsRI, waterRI, incidence, vals);
		if (med == 1) Fresnel(wlsRI, airRI, incidence, vals);
    if (med == 2) Fresnel(wlsRI, glassRI, incidence, vals);
		
		rs = vals[0];
		rp = vals[1];
		ts = vals[2];
		tp = vals[3];
		r = vals[4];
		t = vals[5];

		averageRef += vals[4];
		averageTran += vals[5];


  } // end of loop over events

  averageRef /= (double)(numRuns);
	averageTran /= (double)(numRuns);
  t = averageTran;



} // end of function


void RefractiveIndex(){

  double air[100];
  double airc[100];
  double airt[100];
  double water[100];
  double waterc[100];
  double watert[100];
  double refind[100];

  bool Gap = true;

  int maxEvent = 100;
  for (int i = 0; i < maxEvent; i++){
	  if (i % 10 == 0) std::cout << "Processing event " << i << "/" << maxEvent << std::endl;
      double ri = 1 + (i)*0.02; 
      double t;
      refind[i] = ri;

       // Gap between PMT and plate
	   if (Gap){
		  if (ri <= 1.){
			air[i] = airc[i] = airt[i] = 0;
		  } 
		  else{
			AnglesOnACircle(t, 0, ri , 50, 3.8); // air
			air[i] = t;
			airc[i] = cos( asin(1/ri)  );
			airt[i] = air[i]*airc[i];
		  }
		  if (ri <= 1.33){
			water[i] = waterc[i] = watert[i] = 0;
		  } 
		  else{
			AnglesOnACircle(t, 1, ri , 50, 3.8); // water
			water[i] = t;
			waterc[i] = cos( asin(1.33/ri)  );
			watert[i] = water[i]*waterc[i];
			
		  }
      
		}
       // No Gap between PMT and plate
	  else {
		  if (ri <= 1.){
			air[i] = airc[i] = airt[i] = 0;
		  } 
		  else{
			AnglesOnACircle(t, 2, ri , 50, 3.8); // air
			air[i] = t;
			airc[i] = cos( asin(1/ri)  );
			airt[i] = air[i]*airc[i];
		  }
		  if (ri <= 1.33 ){
			water[i] = waterc[i] = watert[i] = 0;
		  } 
		  else{
			AnglesOnACircle(t, 2, ri , 50, 3.8); // water
			water[i] = t;
			waterc[i] = cos( asin(1.33/ri)  );
			watert[i] = water[i]*waterc[i];
			
		  }
	  }
      

      /*
      std::cout << "RI:\t" << refind[i] << std::endl;
      std::cout << "water:\t" << water[i] << std::endl;
      std::cout << "waterc:\t" << waterc[i] << std::endl;
      std::cout << "watert:\t" << watert[i] << std::endl;
      std::cout << "air:\t" << air[i] << std::endl;
      std::cout << "airc:\t" << airc[i] << std::endl;
      std::cout << "airt:\t" << airt[i] << std::endl;
      */
  }

  TGraph *airGraph = new TGraph( 100, refind, air  );
  airGraph->SetName("Air");
  airGraph->SetLineColor(kRed);
  airGraph->SetLineWidth(3);
  airGraph->SetLineStyle(7);

  TGraph *waterGraph = new TGraph( 100, refind, water  );
  waterGraph->SetName("Water");
  waterGraph->SetLineColor(kBlue);
  waterGraph->SetLineWidth(3);
  waterGraph->SetLineStyle(7);

  TGraph *aircGraph = new TGraph( 100, refind, airc  );
  aircGraph->SetName("AirC");
  aircGraph->SetLineColor(kRed);
  aircGraph->SetLineWidth(3);
  aircGraph->SetLineStyle(9);

  TGraph *watercGraph = new TGraph( 100, refind, waterc  );
  watercGraph->SetName("WaterC");
  watercGraph->SetLineColor(kBlue);
  watercGraph->SetLineWidth(3);
  watercGraph->SetLineStyle(9);

  TGraph *airtGraph = new TGraph( 100, refind, airt  );
  airtGraph->SetName("AirT");
  airtGraph->SetLineColor(kRed);
  airtGraph->SetLineWidth(3);
  airtGraph->SetLineStyle(1);

  TGraph *watertGraph = new TGraph( 100, refind, watert  );
  watertGraph->SetName("WaterT");
  watertGraph->SetLineColor(kBlue);
  watertGraph->SetLineWidth(3);
  watertGraph->SetLineStyle(1);

  TLegend *leg = new TLegend(0.7,0.7,1.0,1.0);
  leg->AddEntry(airGraph, "Air Transmit", "l");
  leg->AddEntry(aircGraph, "Air Capture", "l");
  leg->AddEntry(airtGraph, "Air Total", "l");
  leg->AddEntry(waterGraph, "Water Transmit", "l");
  leg->AddEntry(watercGraph, "Water Capture", "l");
  leg->AddEntry(watertGraph, "Water Total", "l");

  TCanvas *c1 = new TCanvas("c1","c1");
  c1->SetGridx();
  c1->SetGridy();
  c1->SetTickx();
  c1->SetTicky();
  airGraph->GetYaxis()->SetRangeUser(0.0,1.0);
  airGraph->Draw();
  aircGraph->Draw("same");
  airtGraph->Draw("same");
  waterGraph->Draw("same");
  watercGraph->Draw("same");
  watertGraph->Draw("same");
  leg->Draw("same");

}
