// This is a simulation to determine the shape of the WLS efficiency curve.
// coordinate system PMT center is (0,0)
// angles from 0->2*pi [(1,0) is 0 degrees and it increases anti-clockwise]
// C++ Includes
#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>
#include <cmath>
#include <exception>

// ROOT Includes
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TTree.h"

// Defines
#define PI 3.141592654

std::default_random_engine gen(12345);
std::uniform_real_distribution<double> ProbabilityGen(0.,1.);


typedef struct {

	double Pos[2] = {0,0}; // Photon position
	double Dir[3] = {0,0,0}; // Photon direction
	double R = 0; // Photon distance from center
	double Theta = 0; // Photon angle from center
	bool hitPMT = false;
	int numBounces = 0;
	double dist = 0; // photon travel distance
	double time = 0; // photon travel time
	int status = -1; // 0: captured; 1: lost; 2: attenuated;
} Phot ; // Structure to hold the information about the photons

enum Shape {
	Circle,
	Square,
	Rectangle
}; // Enumeration to hold the different WLS shapes

void ShowHelpMessage(std::string a){
   std::cerr << "Usage: " << a << " <option(s)\n"
              << "Options:\n"
              << "\t-h,--help\t\tShow this help message\n"
			  << "\t--fname\t\t\tChoose a filename\n"
			  << "\t-s,--shape\t\tSpecify the shape of the plate\n"
			  << "\t-p,--pmtradius\t\tSpecify the PMT radius [cm]\n"
			  << "\t-x,--platex\t\tSpecify the x length of the plate [cm]\n"
			  << "\t-y,--platey\t\tSpecify the y length of the plate [cm]\n"
			  << "\t-a,--attenuation\tSpecify the attenuation length of the plate [cm]\n"
			  << "\t-n,--numphots\t\tSpecify the number of photons to generate in the plate\n"
			  << "\t--air\t\t\tThe plate is immersed in air\n"
			  << "\t--water\t\t\tThe plate is immersed in water\n"
			  << "\t--line\t\t\tPhotons generated along one axis\n"
			  << "\t--p3\t\t\tPhotons given an additional 3D propagation angle\n"
			  << "\t-v,--verbosity\t\tEnables verbose mode\n"
			  << "\t-u,--uniform\t\tSpecify the number of photons to generate per cm^2\n"
			  << "\t--seed\t\t\tSpecify a seed for the random number generator\n"
			  << "\t-b,--bounce\t\tSpecify the number of photon bounces to track\n"
			  << "\t-m,--mylar\t\tEnables mirrored edges\n"
              << std::endl;

}

bool HasRootExt(std::string a){
	std::string test;
	int strL = a.length();
	int startPoint = strL - 5;
	for (int i= startPoint; i< strL; i++)
	{
		test += a.at(i);
	}
	if (test == ".root") return true;
	else return false;
}
// A function to see if two values are roughly equal
bool RoughlyEqual(double a, double b){
	double tolerance = 0.001;
  if ( a >= b - tolerance && a <= b+ tolerance) {
  	return true;}
  else { return false;}
 }

// A function to check that the input is a double
bool StringIsDouble(std::string a){
	bool value = true;
	try{
		double f = std::stod(a);
	}catch(std::exception& ia){
		value = false;
	}
	return value;

}
// A function to check that the input is an int
bool StringIsInt(std::string a){
	bool value = true;
	try{
		int f = std::stoi(a);
	}catch(std::exception& ia){
		value = false;
	}
	return value;

}
// A function to see if the photon has died or not
 bool IsAttenuated(double d, double lambda){
	 if ( lambda != -1 && ProbabilityGen(gen) >= exp(-1*d/lambda) )
	 {return true;}
	 else {return false;}
 }
// A function to keep all angle ranges between 0 and 2PI
double Angle(double x){
	while (x >= 2*PI){
	x -= 2*PI;
	}

	while (x < 0){
	x += 2*PI;
	}
	return x;
}


// A function to figure out the angle from the center
double AngFromCenter(double x, double y){
	double ang = 0;
	ang = atan2(y,x);
	return Angle(ang);
}
// A function to figure out the angle to the center
double AngToCenter(double x, double y){
	double ang = 0;
	ang = atan2(-1*y,-1*x);
	return Angle(ang);
}

// A function to rotate cartesian coordinated by theta clockwise
void Rotate(double *vec, double t){
	double tempX = vec[0];
	double tempY = vec[1];

	vec[0] = tempX*cos(t) + tempY*sin(t);
	vec[1] = tempY*cos(t) - tempX*sin(t);

}

// A function to work out if the angle is less than the critical angle
// rotates the coordinates by a required amount (to make vec(1,0) the 0 degrees)
// then sees if the angle of incidence is within the critical angle limits
bool IsCrit(double angDir, double angle, double crit){
	double ang = Angle(angDir - angle);
	if (ang >= Angle(crit) && ang <= Angle(-crit) ) {return true;} else {return false;}
}

// The PropagatePhoton function moves the photon and makes sure it is always in bounds.
void PropagatePhoton(double *p, double i, double pDirX, double pDirY, double *wlsL, double pmtR, Shape shape, double &dist){

	// Variables used to work out the travel distance
	double startX = p[0];
	double startY = p[1];

	// Simplification -- If the angle direction is within the PMT acceptance then just stop propagation and move on
	double r = sqrt(pow(p[0],2) + pow(p[1],2) );
	//double h = sqrt(pow(r,2) + pow(pmtR,2) ); // distance between the photon and the edge of the PMT forming a right angle with the center of the PMT
	double h = r;
	double ang = asin(pmtR/h); // angle of acceptance above or below the direct angle

	double directAng = AngToCenter(p[0],p[1]); // direct angle from position to center of PMT.
	double travelAng = p[2] ;// The angle of travel.

	// Move photon to the center if it is heading for the PMT
	if (travelAng < directAng + ang && travelAng > directAng -ang) {
        p[0] = 0;
        p[1] = 0;
        dist -= pmtR; // this is to be subtracted because the distance should only count until the edge of the pmt. This code puts the photon in the center so it overshot by pmtR.
	}

	p[0] += i*pDirX; // Move photon by (increment)*xDir
	p[1] += i*pDirY; // Move photon by (increment)*yDir

	// First we make sure the photon is always in the plate.
	// If the plate is a Square or a Rectangle
	if (shape == Square || shape == Rectangle){
		if (p[0] > wlsL[0]/2) { // The photon has moved outside the plate, let's bring it to the edge
			double diffX = p[0] - wlsL[0]/2;
			double diffI = diffX/pDirX; // Gives the difference as a measure of the x component increment
			p[0] -= diffI*pDirX;
			p[1] -= diffI*pDirY;
		}
		if (p[0] < -wlsL[0]/2) { // The photon has moved outside the plate, let's bring it to the edge
			double diffX = p[0] + wlsL[0]/2;
			double diffI = diffX/pDirX; // Gives the difference as a measure of the x component increment
			p[0] -= diffI*pDirX;
			p[1] -= diffI*pDirY;
		}
		if (p[1] > wlsL[1]/2) { // The photon has moved outside the plate, let's bring it to the edge
			double diffY = p[1] - wlsL[1]/2;
			double diffI = diffY/pDirY; // Gives the difference as a measure of the y component increment
			p[0] -= diffI*pDirX;
			p[1] -= diffI*pDirY;
		}
		if (p[1] < -wlsL[1]/2) { // The photon has moved outside the plate, let's bring it to the edge
			double diffY = p[1] + wlsL[1]/2;
			double diffI = diffY/pDirY; // Gives the difference as a measure of the y component increment
			p[0] -= diffI*pDirX;
			p[1] -= diffI*pDirY;
		}
	}
	// If the plate is a Circle
	if (shape == Circle){
		double r = sqrt(pow(p[0],2) + pow(p[1],2) );
		int selectedX = 0; // This value will determine which quadratic solution is chosen
		double distTo0 = 0; // This shows how far the current point is to the first quadratic solution
		double distTo1 = 0; // This shows how far the current point is to the second quadratic solution

		if (r > wlsL[0]) { // The photon has moved outside the plate, let's bring it to the edge
			double potX[2] = {-wlsL[0],wlsL[0]}; // stores the two x Values for the quadratic solution

			double m =pDirY/pDirX ; // gradient of photon path
			double c = p[1] - m*p[0]; // y intercept of photon path

			potX[0] = ( (-m*c) + sqrt(( (pow(m,2)+1)*pow(wlsL[0],2) ) - pow(c,2) ) ) /(pow(m,2) +1 );
			potX[1] = ( (-m*c) - sqrt(( (pow(m,2)+1)*pow(wlsL[0],2) ) - pow(c,2) ) ) /(pow(m,2) +1 );

			distTo0 = abs(p[0] - potX[0]);
			distTo1 = abs(p[0] - potX[1]);

			if (distTo1 < distTo0 ) selectedX = 1;

			double diffX = p[0] - potX[selectedX];
			double diffI = diffX/pDirX;
			p[0] -= diffI*pDirX;
			p[1] -= diffI*pDirY;

		}

	}
	dist += sqrt( pow(startX - p[0], 2) + pow(startY - p[1], 2) ); // add the distance travelled onto the overall distance
}


bool HitPMT(double *p, double pmtR){
	double r = sqrt(pow(p[0],2) + pow(p[1],2) );
	if (r <= pmtR) return true;
	else return false;
}

// HitEdge function returns true if the photon has hit an edge and false otherwise.
bool HitEdge(double *p, double *wlsL, Shape shape){

	double r = sqrt(pow(p[0],2) + pow(p[1],2) );


	if ( ( shape == Square || shape == Rectangle) && (p[0] == -wlsL[0]/2 || p[0] == wlsL[0]/2 || p[1] == -wlsL[1]/2 || p[1] == wlsL[1]/2) ) return true;
	else if ( shape == Circle &&  RoughlyEqual(r,wlsL[0]) ) return true;
	else return false;


}

// ReflectPhoton function reverses the direction of the photon hitting an edge. Includes total internal reflection
bool ReflectPhoton(double *p, double &pDirX, double &pDirY,double *wlsL, Shape shape, double crit, bool hasMylar){

	bool ref = false;
	bool mirror = hasMylar;
	bool lost = false;

	p[2] = Angle(p[2]); // Make sure angle is in correct range

	// Ensures that total internal reflection is treated separately from the reflective additions
	if (shape == Square || shape == Rectangle){
		if (p[0] == -wlsL[0]/2){
			if ( IsCrit(p[2], PI , crit) ){ref = true;}
		}
		if (p[0] == wlsL[0]/2){
			if ( IsCrit(p[2], 0 , crit) ){ref = true;}
		}
		if (p[1] == -wlsL[1]/2){
			if ( IsCrit(p[2], 3*PI/2 , crit) ){ref = true;}
		}
		if (p[1] == wlsL[1]/2){
			if ( IsCrit(p[2], PI/2 , crit) ) {ref = true;}
		}

	}
	if (shape == Circle){
		// rotates the circle so that the point of the photon is at 0 degrees then sees if it is larger than the critical angle
		if(IsCrit(p[2], AngFromCenter(p[0], p[1]) , crit) ){
			ref = true;
		}
	}

	// work out new direction components
	if ( (shape == Square || shape == Rectangle) && (ref || mirror)){
		if (p[0] == -wlsL[0]/2){
			pDirX*=-1;
		}
		if (p[0] == wlsL[0]/2){
			pDirX*=-1;
		}
		if (p[1] == -wlsL[1]/2){
			pDirY*=-1;
		}
		if (p[1] == wlsL[1]/2){
			pDirY*=-1;
		}

		// work out new travelling angle
		p[2] = AngFromCenter(pDirX, pDirY);
	}

	if (shape == Circle  && (ref || mirror) ){
		double posTemp[2] = {p[0],p[1]}; // store the position temporarily
		double dirTemp[2] = {pDirX,pDirY}; // store the direction temporarily
		double angle = AngFromCenter(p[0],p[1]); // Gives the angle of rotation to get the tangent

		// Rotate the coordinates for simple calculations
		Rotate(posTemp, angle);
		Rotate(dirTemp, angle);

		dirTemp[0] *= -1; // flip the x' direction
		Rotate(dirTemp, (2*PI-angle)	); // Rotate back the direction coordinates

		pDirX = dirTemp[0];
		pDirY = dirTemp[1];

		// work out new travelling angle
		p[2] = AngFromCenter(pDirX, pDirY);

	}

	if (!ref && !mirror) lost = true;
	return lost;
}


// Main function

int main(int argc, char* argv[]){

    // Plate Options
    double WLSx = 23.0;
    double WLSy = 23.0;
	double attL = -1; // attenuation length in cm (-1 means no attenuation)
	double PMTRadius = 3.8; // cm.
	Shape WLSShape = Square;
	std::string shape = "square";
	
	bool inWater = false;
	bool inAir = true;
	bool mylar = false;
	bool line = false;
	bool prop3d = false;
	bool chosenName = false;
	std::string fileName;
	
	// Simulation Options
    int numPhots = 1000000; // number of photons to generate
	bool verbosity = false; 
	bool uniform = false; // toggle uniform density of photons

	int numBounce = 4; // Maximum number of bounces of light to trace
	int seed = 12345; // Seed for random generator
	
	for (int i = 1; i < argc; i++){

		// Shape option
		if (std::string(argv[i]) == "-s" || std::string(argv[i]) == "--shape") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( std::string(argv[i+1]) ==  "Square" ||
				  	std::string(argv[i+1]) ==  "square") {
					i++; // don't check next argument
					WLSShape = Square;
					shape = "square";
				} 
				else if (	std::string(argv[i+1]) ==  "Rectangle" ||
				  			std::string(argv[i+1]) ==  "rectangle" ){
					
					i++; // don't check next argument
					WLSShape = Rectangle;
					shape = "rectangle";
				}  
				else if (	std::string(argv[i+1]) ==  "Circle" ||
				  			std::string(argv[i+1]) ==  "circle") {

					i++; // don't check next argument
					WLSShape = Circle;
					shape = "circle";
				} else {
					std::cerr << "--shape option has an invalid argument. Options are Square, Circle or Rectangle." << std::endl;
					exit(1);

				}
					  
					  
                 
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--shape option requires one argument." << std::endl;
                exit(1);
            }  
        }

		// PMTradius option
		if (std::string(argv[i]) == "-p" || std::string(argv[i]) == "--pmtradius") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( StringIsDouble( std::string(argv[i+1]) ) ) {
					PMTRadius = std::stod( argv[i+1] );
					i++; // don't check next argument
					if (PMTRadius <= 0){
						std::cerr << "--pmtradius option has an invalid argument. Value must be larger than 0." << std::endl;
						exit(1);
					}
				} 
				else {
					std::cerr << "--pmtradius option has an invalid argument. Options must be a number larger than 0." << std::endl;
					exit(1);
				}                 
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--pmtradius option requires one argument." << std::endl;
                exit(1);
            }  
        }

		// PlateX option
		if (std::string(argv[i]) == "-x" || std::string(argv[i]) == "--platex") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( StringIsDouble( std::string(argv[i+1]) ) ) {
					WLSx = std::stod( argv[i+1] );
					i++; // don't check next argument
					if (WLSx <= 0){
						std::cerr << "--x option has an invalid argument. Value must be larger than 0." << std::endl;
						exit(1);
					}
				} 
				else {
					std::cerr << "--x option has an invalid argument. Options must be a number larger than 0." << std::endl;
					exit(1);
				}                 
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--x option requires one argument." << std::endl;
                exit(1);
            }  
        }

		// PlateY option
		if (std::string(argv[i]) == "-y" || std::string(argv[i]) == "--platey") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( StringIsDouble( std::string(argv[i+1]) ) ) {
					WLSy = std::stod( argv[i+1] );
					i++; // don't check next argument
					if (WLSy <= 0){
						std::cerr << "--y option has an invalid argument. Value must be larger than 0." << std::endl;
						exit(1);
					}
				} 
				else {
					std::cerr << "--y option has an invalid argument. Options must be a number larger than 0." << std::endl;
					exit(1);
				}                 
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--y option requires one argument." << std::endl;
                exit(1);
            }  
        }

		// Attenuation option
		if (std::string(argv[i]) == "-a" || std::string(argv[i]) == "--attenuation") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( StringIsDouble( std::string(argv[i+1]) ) ) {
					attL = std::stod( argv[i+1] );
					i++; // don't check next argument
					if (attL <= 0){
						std::cerr << "--attenuation option has an invalid argument. Value must be larger than 0." << std::endl;
						exit(1);
					}
				} 
				else {
					std::cerr << "--attenuation option has an invalid argument. Options must be a number larger than 0." << std::endl;
					exit(1);
				}                 
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--attenuation option requires one argument." << std::endl;
                exit(1);
            }  
        }

		// Number of Photons option
		if (std::string(argv[i]) == "-n" || std::string(argv[i]) == "--numphots") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( StringIsInt( std::string(argv[i+1]) ) ) {
					numPhots = std::stoi( argv[i+1] );
					i++; // don't check next argument
					if (numPhots <= 0){
						std::cerr << "--n option has an invalid argument. Value must be larger than 0." << std::endl;
						exit(1);
					}
				} 
				else {
					std::cerr << "--n option has an invalid argument. Options must be an integer larger than 0." << std::endl;
					exit(1);
				}                 
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--n option requires one argument." << std::endl;
                exit(1);
            }  
        }
		// Shape option
		if (std::string(argv[i]) == "--fname") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( HasRootExt(std::string(argv[i+1]))) {
					fileName = std::string(argv[i+1]);
					chosenName = true;
					i++; // don't check next argument
				} else {
					std::cerr << "--fname option has an invalid argument. Option must be a root filename e.g. file.root" << std::endl;
					exit(1);
				}
					  
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--fname option requires one argument." << std::endl;
                exit(1);
            }  
        }
		// Air
		if(std::string(argv[i]) == "--air"){
			inAir = true;
			inWater = !inAir;
		}
		// Water
		if(std::string(argv[i]) == "--water"){
			inWater = true;
			inAir = !inWater;
		}
		// On axis
		if(std::string(argv[i]) == "--line"){
			line = true;
		}
		// 3D propagation (just scale 2D by giving it an additional angle)
		if(std::string(argv[i]) == "--p3"){
			prop3d = true;
		}
		// Verbosity
		if(std::string(argv[i]) == "-v" || std::string(argv[i]) == "--verbosity"){
			verbosity = true;
		}
		// Uniformity
		if (std::string(argv[i]) == "-u" || std::string(argv[i]) == "--uniform") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( StringIsInt( std::string(argv[i+1]) ) ) {
					numPhots = std::stoi( argv[i+1] );
					i++; // don't check next argument
					uniform = true;
					if (numPhots <= 0){
						std::cerr << "--u option has an invalid argument. Value must be larger than 0." << std::endl;
						exit(1);
					}
				} 
				else {
					std::cerr << "--u option has an invalid argument. Options must be an integer larger than 0." << std::endl;
					exit(1);
				}                 
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--u option requires one argument." << std::endl;
                exit(1);
            }  
        }
		// Seed
		if (std::string(argv[i]) == "--seed") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( StringIsInt( std::string(argv[i+1]) ) ) {
					seed = std::stoi( argv[i+1] );
					i++; // don't check next argument
					if (seed <= 0){
						std::cerr << "--seed option has an invalid argument. Value must be larger than 0." << std::endl;
						exit(1);
					}
				} 
				else {
					std::cerr << "--seed option has an invalid argument. Options must be an integer larger than 0." << std::endl;
					exit(1);
				}                 
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--seed option requires one argument." << std::endl;
                exit(1);
            }  
        }

		// NumBounces
		if (std::string(argv[i]) == "-b" || std::string(argv[i]) == "--bounce") {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
				if( StringIsInt( std::string(argv[i+1]) ) ) {
					numBounce = std::stoi( argv[i+1] );
					i++; // don't check next argument
					if (numBounce <= 0){
						std::cerr << "--bounce option has an invalid argument. Value must be larger than 0." << std::endl;
						exit(1);
					}
				} 
				else {
					std::cerr << "--bounce option has an invalid argument. Options must be an integer larger than 0." << std::endl;
					exit(1);
				}                 
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--bounce option requires one argument." << std::endl;
                exit(1);
            }  
        }
		// Help
		if(std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help"){
			ShowHelpMessage(argv[0]);
			exit(1);
		}
		// Mylar
		if(std::string(argv[i]) == "-m" || std::string(argv[i]) == "--mylar"){
			mylar = true;
		}
	}

	double increment = 0.1; // Value to increment the steps of the photon
	int nBin = 56;

	double WLSLength[2];
	if (WLSShape == Square) {// cm. // Square each component is a full length
		WLSLength[0] = WLSx;
		WLSLength[1] = WLSx;

	} else if (WLSShape == Rectangle) {// cm. // Rectangle x,y component are full lengths
		WLSLength[0] = WLSx;
		WLSLength[1] = WLSy;
	} else if (WLSShape == Circle){// cm. // Circle each component is the radius
		WLSLength[0] = WLSx/2;
		WLSLength[1] = WLSx/2;
	}

	// Some checks
	if (PMTRadius > WLSLength[0]/2 ||  PMTRadius > WLSLength[1]/2) {
		std::cerr << "PMT is larger than the plate." << std::endl;
		exit(1);
	}
    	

   // Toggle fixed density for squares and rectangles -> density = numPhots per square cm
    if(uniform){
		if (WLSShape == Square || WLSShape == Rectangle){
			double effArea = ((WLSLength[0] * WLSLength[1]) - (PI * PMTRadius * PMTRadius));
			numPhots *= (int)effArea;
		} else {
			double effArea = ((PI*WLSLength[0] * WLSLength[0]) - (PI * PMTRadius * PMTRadius));
			numPhots *= (int)effArea;
		}
    }

	double WLSThickness = 0; // Not used yet (2D approximation)
	double WLSRefractiveIndex = 1.58;
	double criticalAngle;
	if (inWater){criticalAngle = asin(1.33/WLSRefractiveIndex);}
	if (inAir){criticalAngle = asin(1./WLSRefractiveIndex);}

	// Efficiencies for model
	double WLSEfficiency = 1;
	double WLSReflection = 1;

	// Speed of light in plate
	double photonSpeed = 299792000/WLSRefractiveIndex; // speed of light in vacuum (m/s) / refractive index
	photonSpeed *= 100; // m/s into cm/s


	// Really messy way to output to a new file
	double  photPosX = 0;
	double  photPosY = 0;
	double  photPosR = 0;
	double photDirTheta = 0;
	double photDirX = 0;
	double photDirY = 0;
	double initDirX = 0;
	double initDirY = 0;
	int hPMT = 0;
	double dist = 0;
	double time = 0;
	int reflect = 0;
	int status = -1;


	if (!chosenName){
    	// Designates a seperate tag for each combination of plate size
    	fileName = "WLS_" + shape + "_" + std::to_string(WLSLength[0]) + "x" + 	std::to_string(WLSLength[1]) + "L" + std::to_string(attL) + ".root";
	} 


    TFile *outfile = new TFile(fileName.c_str(), "RECREATE");
	TTree *tree = new TTree("simulation", "simulation");
	tree->Branch("posX", &photPosX, "posX/D");
	tree->Branch("posY", &photPosY, "posY/D");
	tree->Branch("posR", &photPosR, "posR/D");
	tree->Branch("dirX", &photDirX, "dirX/D");
	tree->Branch("dirY", &photDirY, "dirY/D");
	tree->Branch("dirTheta", &photDirTheta, "dirTheta/D");
	tree->Branch("hitPMT", &hPMT, "hitPMT/I");
	tree->Branch("reflections", &reflect, "reflections/I");
	tree->Branch("dist", &dist, "dist/D");
	tree->Branch("time", &time, "time/D");
	tree->Branch("status", &status, "status/I");


	
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "------------SETTINGS------------------" << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "Number of Bounces:\t"<< numBounce << std::endl;
	std::cout << "Number of Photons:\t"<< numPhots << std::endl;
	std::cout << "Random Generator Seed:\t"<< seed << std::endl;
	std::cout << "Increment:\t"<< increment << std::endl;
	std::cout << "Water:\t"<< inWater << std::endl;
	std::cout << "Air:\t"<< inAir << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "------------WLS PROPERTIES------------" << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "WLS Shape:\t"<< shape << std::endl;
	std::cout << "WLS Length[x]:\t"<< WLSLength[0] << std::endl;
	std::cout << "WLS Length[y]:\t"<< WLSLength[1] << std::endl;
	std::cout << "WLS RI:\t"<< WLSRefractiveIndex << std::endl;
	std::cout << "WLS Efficiency:\t"<< WLSEfficiency << std::endl;
	std::cout << "WLS Reflection:\t"<< WLSReflection << std::endl;
	std::cout << "WLS Attenuation:\t"<< attL << std::endl;
	std::cout << "WLS Mylar:\t"<< mylar << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "------------PMT PROPERTIES------------" << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "PMT Radius:\t"<< PMTRadius << std::endl;
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "--------------------------------------" << std::endl;

	std::vector<Phot> PhotonVector;
	// Generate a point in the WLS

	std::default_random_engine generator(seed);
	std::uniform_real_distribution<double> distributionPos;
	std::uniform_real_distribution<double> distributionDir;
	std::uniform_real_distribution<double> distributionPosX;
	std::uniform_real_distribution<double> distributionPosY;
	std::uniform_real_distribution<double> distributionPosR;
	std::normal_distribution<double> distributionWidth(0., 0.2);
	std::uniform_real_distribution<double> distribution3DProp(0., criticalAngle);

	if (WLSShape == Square){
	distributionPos = std::uniform_real_distribution<double> (-WLSLength[0]/2,WLSLength[0]/2);
	distributionDir = std::uniform_real_distribution<double> (0.0,2*PI);
	}
	if (WLSShape == Rectangle){
	distributionPosX = std::uniform_real_distribution<double> (-WLSLength[0]/2,WLSLength[0]/2);
	distributionPosY = std::uniform_real_distribution<double> (-WLSLength[1]/2,WLSLength[1]/2);
	distributionDir = std::uniform_real_distribution<double> (0.0,2*PI);
	}
	if (WLSShape == Circle){
	distributionPosR = std::uniform_real_distribution<double> (pow(PMTRadius,2),pow(WLSLength[0],2));
	distributionDir = std::uniform_real_distribution<double> (0.0,2*PI);
	}

//--------------------------------------Start of LOOP---------------------------------------------------------------
	for (int i = 0; i <numPhots; i++){
            if (i == 0){std::cout << "Plate size: " << WLSLength[0] << " x " << WLSLength[1] << ". Attenuation: " << attL << std::endl;}
        	if (i % 100000 == 0){std::cout << "Generating Photon: " << i << std::endl;}
		photPosX = 0;
		photPosY = 0;
		photPosR = 0;
		photDirTheta = 0;
		photDirX = 0;
		photDirY = 0;
		initDirX = 0;
		initDirY = 0;
		dist = 0;
		time = 0;

		bool inPlate = false;

		if (WLSShape == Square){
			while (!inPlate) {
				// Across the whole plate
				if (!line){
					photPosX = distributionPos(generator);
					photPosY = distributionPos(generator);
				}
				// On axis
				if (line){
					//photPosX = 0.0;
					photPosX = distributionWidth(generator);
					photPosY = std::abs(distributionPos(generator));
				}

				photPosR = sqrt( pow(photPosX,2) + pow(photPosY,2) );

				if (verbosity){
					std::cout << "Generated a point (x y):\t" << photPosX << " " << photPosY << std::endl;
				}

				if (photPosR > PMTRadius ) {
					inPlate = true;
				} else if (verbosity){
					std::cout << "Generated point was inside PMT. Generating a new point." << std::endl;
				}
			}
		}
		if (WLSShape == Rectangle){
			while (!inPlate) {
				// Across the whole plate
				if (!line){
					photPosX = distributionPosX(generator);
					photPosY = distributionPosY(generator);
				}
				// On axis
				if (line){
					//photPosX = 0;
					photPosX = distributionWidth(generator);
					photPosY = distributionPosY(generator);
				}

				photPosR = sqrt( pow(photPosX,2) + pow(photPosY,2) );

				if (verbosity){
					std::cout << "Generated a point (x y):\t" << photPosX << " " << photPosY << std::endl;
				}

				if (photPosR > PMTRadius ) {
					inPlate = true;
				} else if (verbosity){
					std::cout << "Generated point was inside PMT. Generating a new point." << std::endl;
				}
			}
		}

		if (WLSShape == Circle){
			while (!inPlate) {
				// Across the whole plate
				if (!line){
					double genR = distributionPosR(generator);
					double genTheta = distributionDir(generator);
					photPosX = sqrt(genR)*cos(genTheta);
					photPosY = sqrt(genR)*sin(genTheta);
				}
				// On axis
				if (line){
					//photPosX = 0;
					photPosX = distributionWidth(generator);
					photPosY = distributionPosY(generator);
				}

				photPosR = sqrt( pow(photPosX,2) + pow(photPosY,2) );

				if (verbosity){
					std::cout << "Generated a point (x y):\t" << photPosX << " " << photPosY << std::endl;

				}
					inPlate = true;
			}
		}

		// Generate a photon direction
		photDirTheta = distributionDir(generator);
		photDirX = cos(photDirTheta);
		photDirY = sin(photDirTheta);
		initDirX = photDirX;
		initDirY = photDirY;

		if (verbosity){
			std::cout << "Generated a photon direction (theta x y):\t" << photDirTheta << " " << photDirX << " "<< photDirY << std::endl;
		}

		// Trace photon for numBounce bounces and see if it reaches the PMT
		// Keep track of the number of bounces
		double photonPosTemp[3] = {photPosX, photPosY,photDirTheta};
		bool hitPMT = false;
		bool lost = false;
		bool attenuated = false;
		hPMT = 0;
		reflect = 0;

		if (verbosity){
			std::cout << "Propagating photon: " << i << std::endl;
			std::cout << "Pos: " << photonPosTemp[0] << " " << photonPosTemp[1] << std::endl;
		}

		while (!hitPMT && reflect <= numBounce && !lost && !attenuated){

			PropagatePhoton(photonPosTemp, increment, photDirX, photDirY, WLSLength, PMTRadius, WLSShape, dist);
			if (verbosity){
				std::cout << "Pos: " << photonPosTemp[0] << " " << photonPosTemp[1] << std::endl;
			}

			if (HitPMT(photonPosTemp, PMTRadius)) {
				hitPMT = true;
				hPMT = 1;
				status = 0;
				if (verbosity){
					std::cout << "PMT Hit" << std::endl;

				}

			}
			if ( HitEdge(photonPosTemp, WLSLength, WLSShape)) {

				lost = ReflectPhoton(photonPosTemp, photDirX, photDirY, WLSLength, WLSShape, criticalAngle, mylar);
				if (!lost) {
					reflect++;
					status = 1;
				}

				if (verbosity){
					std::cout << "Edge Hit" << std::endl;

				}
			}
		if (prop3d){
			// scale the distance according to 3D propagation
			dist /= 1/cos(distribution3DProp(generator));
		}
		attenuated = IsAttenuated(dist, attL); // checks to see if the photon survived
		if (attenuated){status = 2; hitPMT = false; hPMT = 0;}

		}

		time = dist/photonSpeed;

		Phot photon;
		photon.Pos[0] = photPosX;
		photon.Pos[1] = photPosY;
		photon.Dir[0] = initDirX;
		photon.Dir[1] = initDirY;
		photon.Dir[2] = photDirTheta;
		photon.R = photPosR;
		photon.Theta = AngFromCenter(photPosX, photPosY); // Photon angle from center
		photon.hitPMT = hitPMT;
		photon.numBounces = reflect;
		photon.dist = dist;
		photon.time = time;
		photon.status = status;
		PhotonVector.push_back(photon);
		// Save output to ROOT file
		tree->Fill();
	}
//-------------------------END OF LOOP--------------------------------------------------------------------------------------

	// Histograms
	TH1D *generatedTheta = new TH1D("generatedTheta", "generatedTheta", nBin, 0, 2*PI); // captured photons plotted by radius
	TH1D *radiusHist = new TH1D("radiusHist", "radiusHist", nBin, 0, WLSLength[1]); // captured photons plotted by radius
	TH1D *radiusCapture = new TH1D("radiusCapture", "radiusCapture", nBin, 0, WLSLength[1]); // captured photons plotted by radius
	TH1D *radiusCapture0 = new TH1D("radiusCapture0", "radiusCapture0", nBin, 0, WLSLength[1]); // captured photons without bouncing
	TH1D *radiusCapture1 = new TH1D("radiusCapture1", "radiusCapture1", nBin, 0, WLSLength[1]); // captured photons bouncing 1 time
	TH1D *radiusCapture2 = new TH1D("radiusCapture2", "radiusCapture2", nBin, 0, WLSLength[1]); // captured photons bouncing two times
	TH1D *radiusCapture3 = new TH1D("radiusCapture3", "radiusCapture3", nBin, 0, WLSLength[1]); // captured photons bouncing 3 times
	TH1D *radiusCapture4 = new TH1D("radiusCapture4", "radiusCapture4", nBin, 0, WLSLength[1]); // captured photons bouncing 4 times
	TH1D *radiusCapture5 = new TH1D("radiusCapture5", "radiusCapture5", nBin, 0, WLSLength[1]); // captured photons bouncing 5 times
	TH1D *radiusCapture6 = new TH1D("radiusCapture6", "radiusCapture6", nBin, 0, WLSLength[1]); // captured photons bouncing 6 times
	TH1D *radiusCapture7 = new TH1D("radiusCapture7", "radiusCapture7", nBin, 0, WLSLength[1]); // captured photons bouncing 7 times
	TH1D *radiusCaptureR = new TH1D("radiusCaptureR", "radiusCaptureR", nBin, 0, WLSLength[1]); // captured photons plotted by radius
	TH1D *radiusCapture0R = new TH1D("radiusCapture0R", "radiusCapture0R", nBin, 0, WLSLength[1]); // captured photons without bouncing
	TH1D *radiusCapture1R = new TH1D("radiusCapture1R", "radiusCapture1R", nBin, 0, WLSLength[1]); // captured photons bouncing 1 time
	TH1D *radiusCapture2R = new TH1D("radiusCapture2R", "radiusCapture2R", nBin, 0, WLSLength[1]); // captured photons bouncing 2 times
	TH1D *radiusCapture3R = new TH1D("radiusCapture3R", "radiusCapture3R", nBin, 0, WLSLength[1]); // captured photons bouncing 3 times
	TH1D *radiusCapture4R = new TH1D("radiusCapture4R", "radiusCapture4R", nBin, 0, WLSLength[1]); // captured photons bouncing 4 times
	TH1D *radiusCapture5R = new TH1D("radiusCapture5R", "radiusCapture5R", nBin, 0, WLSLength[1]); // captured photons bouncing 5 times
	TH1D *radiusCapture6R = new TH1D("radiusCapture6R", "radiusCapture6R", nBin, 0, WLSLength[1]); // captured photons bouncing 6 times
	TH1D *radiusCapture7R = new TH1D("radiusCapture7R", "radiusCapture7R", nBin, 0, WLSLength[1]); // captured photons bouncing 7 times

	TH2D *generatedPosCart = new TH2D("generatedPosCart", "generatedPosCart", nBin, -WLSLength[0]/2, WLSLength[0]/2, nBin, -WLSLength[1]/2, WLSLength[1]/2); // mapping of plate
	TH2D *generatedDirCart = new TH2D("generatedDirCart", "generatedDirCart", nBin, -WLSLength[0]/2, WLSLength[0]/2, nBin, -WLSLength[1]/2, WLSLength[1]/2); // mapping of plate
	TH2D *captureMapCart = new TH2D("captureMapCart", "captureMapCart", nBin, -WLSLength[0]/2, WLSLength[0]/2, nBin, -WLSLength[1]/2, WLSLength[1]/2); // mapping of plate
	TH2D *captureMap = new TH2D("captureMap", "captureMap", nBin, 0, 2*PI, nBin, 0, WLSLength[1]); // mapping of plate

	radiusHist->Sumw2();
	radiusCapture->Sumw2();
	radiusCapture0->Sumw2();
	radiusCapture1->Sumw2();
	radiusCapture2->Sumw2();
	radiusCapture3->Sumw2();
	radiusCapture4->Sumw2();
	radiusCapture5->Sumw2();
	radiusCapture6->Sumw2();
	radiusCapture7->Sumw2();
	radiusCaptureR->Sumw2();
	radiusCapture0R->Sumw2();
	radiusCapture1R->Sumw2();
	radiusCapture2R->Sumw2();
	radiusCapture3R->Sumw2();
	radiusCapture4R->Sumw2();
	radiusCapture5R->Sumw2();
	radiusCapture6R->Sumw2();
	radiusCapture7R->Sumw2();
	captureMap->Sumw2();
	generatedPosCart->Sumw2();
	generatedDirCart->Sumw2();
	generatedTheta->Sumw2();
	captureMapCart->Sumw2();

	for (int i = 0; i < PhotonVector.size(); i++ ){

		generatedPosCart->Fill(PhotonVector[i].Pos[0], PhotonVector[i].Pos[1]);
		generatedDirCart->Fill(PhotonVector[i].Dir[0], PhotonVector[i].Dir[1]);
		generatedTheta->Fill(PhotonVector[i].Dir[2]);
		radiusHist->Fill(PhotonVector[i].R);

		if (PhotonVector[i].hitPMT){

			radiusCapture->Fill(PhotonVector[i].R);

			if (PhotonVector[i].numBounces == 0) {radiusCapture0->Fill(PhotonVector[i].R);}
			if (PhotonVector[i].numBounces == 1) {radiusCapture1->Fill(PhotonVector[i].R);}
			if (PhotonVector[i].numBounces == 2) {radiusCapture2->Fill(PhotonVector[i].R);}
			if (PhotonVector[i].numBounces == 3) {radiusCapture3->Fill(PhotonVector[i].R);}
			if (PhotonVector[i].numBounces == 4) {radiusCapture4->Fill(PhotonVector[i].R);}
			if (PhotonVector[i].numBounces == 5) {radiusCapture5->Fill(PhotonVector[i].R);}
			if (PhotonVector[i].numBounces == 6) {radiusCapture6->Fill(PhotonVector[i].R);}
			if (PhotonVector[i].numBounces == 7) {radiusCapture7->Fill(PhotonVector[i].R);}

			captureMap->Fill(PhotonVector[i].Theta, PhotonVector[i].R);
			captureMapCart->Fill(PhotonVector[i].Pos[0], PhotonVector[i].Pos[1]);
		}

	}

	for (int i = 1; i <= nBin; i++){
		double normVal = radiusHist->GetBinContent(i);
		if (normVal ==0) normVal = 1;

      radiusCaptureR->SetBinContent(i,radiusCapture->GetBinContent(i)/normVal);
      radiusCaptureR->SetBinError(i,radiusCapture->GetBinError(i)/normVal);
      radiusCapture0R->SetBinContent(i,radiusCapture0->GetBinContent(i)/normVal);
      radiusCapture0R->SetBinError(i,radiusCapture0->GetBinError(i)/normVal);
      radiusCapture1R->SetBinContent(i,radiusCapture1->GetBinContent(i)/normVal);
      radiusCapture1R->SetBinError(i,radiusCapture1->GetBinError(i)/normVal);
      radiusCapture2R->SetBinContent(i,radiusCapture2->GetBinContent(i)/normVal);
      radiusCapture2R->SetBinError(i,radiusCapture2->GetBinError(i)/normVal);
      radiusCapture3R->SetBinContent(i,radiusCapture3->GetBinContent(i)/normVal);
      radiusCapture3R->SetBinError(i,radiusCapture3->GetBinError(i)/normVal);
      radiusCapture4R->SetBinContent(i,radiusCapture4->GetBinContent(i)/normVal);
      radiusCapture4R->SetBinError(i,radiusCapture4->GetBinError(i)/normVal);
      radiusCapture5R->SetBinContent(i,radiusCapture5->GetBinContent(i)/normVal);
      radiusCapture5R->SetBinError(i,radiusCapture5->GetBinError(i)/normVal);
      radiusCapture6R->SetBinContent(i,radiusCapture6->GetBinContent(i)/normVal);
      radiusCapture6R->SetBinError(i,radiusCapture6->GetBinError(i)/normVal);
      radiusCapture7R->SetBinContent(i,radiusCapture7->GetBinContent(i)/normVal);
      radiusCapture7R->SetBinError(i,radiusCapture7->GetBinError(i)/normVal);

	}

	radiusHist->Write();
	radiusCapture->Write();
	radiusCapture0->Write();
	radiusCapture1->Write();
	radiusCapture2->Write();
	radiusCapture3->Write();
	radiusCapture4->Write();
	radiusCapture5->Write();
	radiusCapture6->Write();
	radiusCapture7->Write();
	radiusCaptureR->Write();
	radiusCapture0R->Write();
	radiusCapture1R->Write();
	radiusCapture2R->Write();
	radiusCapture3R->Write();
	radiusCapture4R->Write();
	radiusCapture5R->Write();
	radiusCapture6R->Write();
	radiusCapture7R->Write();
	captureMap->Write();
	generatedPosCart->Write();
	generatedDirCart->Write();
	generatedTheta->Write();
	captureMapCart->Write();
	tree->Write();

	outfile->Close();

	return 0;

} // End of the main function.
