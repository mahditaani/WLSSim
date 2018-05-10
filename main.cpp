// This is a simulation to determine the shape of the WLS efficiency curve.

// C++ Includes
#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>
#include <cmath>

// ROOT Includes
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"

// Defines
#define PI 3.141592654



typedef struct {
	
	double Pos[2] = {0,0}; // Photon position
	double Dir[3] = {0,0,0}; // Photon direction
	double R = 0; // Photon distance from center
	double Theta = 0; // Photon angle from center
	bool hitPMT = false;
	int numBounces = 0;
} Phot ; // Structure to hold the information about the photons

// A function to figure out the angle from the center
double AngFromCenter(double x, double y){
	double ang = 0; 
	ang = atan(abs(y)/abs(x));
	if (x < 0 && y < 0) {ang = PI + ang;} // nothing changes // bottom left
	if (x > 0 && y < 0) {ang = 2*PI - ang; } // bottom right
//	if (x > 0 && y > 0) {} // top right
	if (x < 0 && y > 0) {ang = PI - ang;} // top left
	return ang;
}
// The PropagatePhoton function moves the photon and makes sure it is always in bounds.
void PropagatePhoton(double *p, float i, double pDirX, double pDirY, double wlsL, double pmtR){

	// Simplification -- If the angle direction is within the PMT acceptance then just stop propagation and move on
	double r = sqrt(pow(p[0],2) + pow(p[1],2) );
	double ang = asin(pmtR/r); // angle of acceptance above or below the direct angle
//	double directAng = atan(p[0]/p[1]); // direct angle from position to center of PMT.
//	double travelAng = atan(pDirX/pDirY); // The angle of travel.
	double directAng = atan(abs(p[0])/abs(p[1])); // direct angle from position to center of PMT.
	double travelAng = p[2] ;// The angle of travel.
/*
	//if (p[0] < 0 && p[1] < 0) {directAng = directAng; travelAng = travelAng;} // nothing changes // bottom left
	if (p[0] > 0 && p[1] < 0) {directAng += PI/2; travelAng += PI/2;} // bottom right
	if (p[0] > 0 && p[1] > 0) {directAng += PI; travelAng += PI;} // top right
	if (p[0] < 0 && p[1] > 0) {directAng += 3*PI/4; travelAng += 3*PI/4;} // top left
*/
	if (p[0] < 0 && p[1] < 0) {directAng = PI/2 - directAng;} // nothing changes // bottom left
	if (p[0] > 0 && p[1] < 0) {directAng += PI/2; } // bottom right
	if (p[0] > 0 && p[1] > 0) {directAng = 3*PI/2 - directAng;} // top right
	if (p[0] < 0 && p[1] > 0) {directAng += 3*PI/2;} // top left

	// DEBUG
//	std::cout << "Ang, DirectAng, travel " << ang << " " << directAng << " " << travelAng << std::endl;
	if (travelAng < directAng + ang && travelAng > directAng -ang) {
		p[0] = 0;
		p[1] = 0;
	}

	p[0] += i*pDirX; // Move photon by (increment)*xDir
	p[1] += i*pDirY; // Move photon by (increment)*yDir

	// First we make sure the photon is always in the plate.
	if (p[0] > wlsL/2) { // The photon has moved outside the plate, let's bring it to the edge
		double diffX = p[0] - wlsL/2; 
		double diffI = diffX/pDirX; // Gives the difference as a measure of the x component increment
		p[0] -= diffI*pDirX;
		p[1] -= diffI*pDirY;
	}
	if (p[0] < -wlsL/2) { // The photon has moved outside the plate, let's bring it to the edge
		double diffX = p[0] + wlsL/2; 
		double diffI = diffX/pDirX; // Gives the difference as a measure of the x component increment
		p[0] -= diffI*pDirX;
		p[1] -= diffI*pDirY;
	}
	if (p[1] > wlsL/2) { // The photon has moved outside the plate, let's bring it to the edge
		double diffY = p[1] - wlsL/2; 
		double diffI = diffY/pDirY; // Gives the difference as a measure of the y component increment
		p[0] -= diffI*pDirX;
		p[1] -= diffI*pDirY;
	}
	if (p[1] < -wlsL/2) { // The photon has moved outside the plate, let's bring it to the edge
		double diffY = p[1] + wlsL/2; 
		double diffI = diffY/pDirY; // Gives the difference as a measure of the y component increment
		p[0] -= diffI*pDirX;
		p[1] -= diffI*pDirY;
	}

	

}


bool HitPMT(double *p, double pmtR){
	double r = sqrt(pow(p[0],2) + pow(p[1],2) );
	if (r <= pmtR) return true;
	else return false;

}

// HitEdge function returns true if the photon has hit an edge and false otherwise.
bool HitEdge(double *p, double wlsL){



	if (p[0] == -wlsL/2 || p[0] == wlsL/2 || p[1] == -wlsL/2 || p[1] == wlsL/2) return true; 
	else return false;
	

}

// ReflectPhoton function reverses the direction of the photon hitting an edge.
void ReflectPhoton(double *p, double &pDirX, double &pDirY,double wlsL){

	// work out new direction components
	if (p[0] == -wlsL/2){ 
		pDirX*=-1;
	}
	if (p[0] == wlsL/2){
		pDirX*=-1;
	}
	if (p[1] == -wlsL/2){
		pDirY*=-1;
	}
	if (p[1] == wlsL/2){
		pDirY*=-1;
	}

	
	// work out new travelling angle
	p[2] = acos(pDirX);
	
	if (pDirY < 0){
		p[2] = 2*PI - p[2];
	}
}


// Main function

int main(){
	bool verbosity = false;
	int numBounce = 2; // Maximum number of bounces of light to trace
	int numPhots = 1000000; // Number of photons to generate 
	int seed = 12345; // Seed for random generator
	float increment = 0.1; // Value to increment the steps of the photon

	int nBin = 56;

	// WLS properties
	double WLSLength = 28.0; // cm.
	double WLSThickness = 0; // Not used yet (2D approximation)
	double WLSRefractiveIndex = 1.58; // Not used yet 

	// PMT properties
	double PMTRadius = 3.8; // cm.

	// Efficiencies for model
	double WLSEfficiency = 1; 
	double WLSReflection = 1; 



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
       int reflect = 0;



       TFile *outfile = new TFile("WLS.root", "RECREATE");
       TTree *tree = new TTree("simulation", "simulation");
       tree->Branch("posX", &photPosX, "posX/D");
       tree->Branch("posY", &photPosY, "posY/D");
       tree->Branch("posR", &photPosR, "posR/D");
       tree->Branch("dirX", &photDirX, "dirX/D");
       tree->Branch("dirY", &photDirY, "dirY/D");
       tree->Branch("dirTheta", &photDirTheta, "dirTheta/D");
       tree->Branch("hitPMT", &hPMT, "hitPMT/I");
       tree->Branch("reflections", &reflect, "reflections/I");


	if (verbosity){
	
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "------------SETTINGS------------------" << std::endl;
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "Number of Bounces:\t"<< numBounce << std::endl;
		std::cout << "Number of Photons:\t"<< numPhots << std::endl;
		std::cout << "Random Generator Seed:\t"<< seed << std::endl;
		std::cout << "Increment:\t"<< increment << std::endl;
		std::cout << "Number of Photons:\t"<< numPhots << std::endl;
		std::cout << "Number of Photons:\t"<< numPhots << std::endl;
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "------------WLS PROPERTIES------------" << std::endl;
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "WLS Length:\t"<< WLSLength << std::endl;
		std::cout << "WLS RI:\t"<< WLSRefractiveIndex << std::endl;
		std::cout << "WLS Efficiency:\t"<< WLSEfficiency << std::endl;
		std::cout << "WLS Reflection:\t"<< WLSReflection << std::endl;
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "------------PMT PROPERTIES------------" << std::endl;
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "PMT Radius:\t"<< PMTRadius << std::endl;
		std::cout << "--------------------------------------" << std::endl;
		std::cout << "--------------------------------------" << std::endl;
	


	
	}

	
	std::vector<Phot> PhotonVector;
	// Generate a point in the WLS

	std::default_random_engine generator(seed);
	std::uniform_real_distribution<double> distributionPos(-WLSLength/2,WLSLength/2);
	std::uniform_real_distribution<double> distributionDir(0.0,2*PI);
//--------------------------------------Start of LOOP---------------------------------------------------------------
	for (int i = 0; i <numPhots; i++){	
        	if (i % 1000 == 0){std::cout << "Generating Photon: " << i << std::endl;}
		photPosX = 0;
		photPosY = 0;
		photPosR = 0;
		photDirTheta = 0;
		photDirX = 0;
		photDirY = 0;
		initDirX = 0;
		initDirY = 0;
		bool inPlate = false;	
		while (!inPlate) {
			// Across the whole plate
			photPosX = distributionPos(generator);
			photPosY = distributionPos(generator);
			// On axis 
			//photPosX = 0; 
			//photPosY = distributionPos(generator);
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
		// Generate a photon direction 
		
//		double photDirTheta = 2*PI*distributionDir(generator);
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
		hPMT = 0;
		reflect = 0;
		
		if (verbosity){
			std::cout << "Propagating photon: " << 1 << std::endl;
			std::cout << "Pos: " << photonPosTemp[0] << " " << photonPosTemp[1] << std::endl;
		}

		while (!hitPMT && reflect <= numBounce ){
		
			// Move the photon
	//		photonPosTemp[0] += increment*photPosX;
	//		photonPosTemp[1] += increment*photPosY;
			PropagatePhoton(photonPosTemp, increment, photDirX, photDirY, WLSLength, PMTRadius);
			if (verbosity){
				std::cout << "Pos: " << photonPosTemp[0] << " " << photonPosTemp[1] << std::endl;
			}

			if (HitPMT(photonPosTemp, PMTRadius)) {
				hitPMT = true;
				hPMT = 1;		
				if (verbosity){
					std::cout << "PMT Hit" << std::endl;
				
				}

			}
			if (HitEdge(photonPosTemp, WLSLength)) {
				ReflectPhoton(photonPosTemp, photDirX, photDirY, WLSLength);
				reflect++;
				if (verbosity){
					std::cout << "Edge Hit" << std::endl;
				
				}
			}

		
		
		}
		
		Phot photon;	
		photon.Pos[0] = photPosX;
		photon.Pos[1] = photPosY;
		photon.Dir[0] = initDirX;
		photon.Dir[1] = initDirY;
		photon.Dir[2] = photDirTheta;
		photon.R = photPosR;
//		photon.Theta = atan(photPosY/photPosX); // Photon angle from center
		photon.Theta = AngFromCenter(photPosX, photPosY); // Photon angle from center
		photon.hitPMT = hitPMT;
		photon.numBounces = reflect;
		
		PhotonVector.push_back(photon);	
//		std::cout << initDirX << "\t" << initDirY << std::endl;
	}
//-------------------------END OF LOOP--------------------------------------------------------------------------------------

	// Save output to ROOT file
	tree->Fill();




	
	// Histograms
	TH1D *generatedTheta = new TH1D("generatedTheta", "generatedTheta", nBin, 0, 2*PI); // captured photons plotted by radius
	TH1D *radiusHist = new TH1D("radiusHist", "radiusHist", nBin, 0, WLSLength); // captured photons plotted by radius
	TH1D *radiusCapture = new TH1D("radiusCapture", "radiusCapture", nBin, 0, WLSLength); // captured photons plotted by radius
	TH1D *radiusCapture0 = new TH1D("radiusCapture0", "radiusCapture0", nBin, 0, WLSLength); // captured photons without bouncing
	TH1D *radiusCapture1 = new TH1D("radiusCapture1", "radiusCapture1", nBin, 0, WLSLength); // captured photons bouncing 1 time
	TH1D *radiusCapture2 = new TH1D("radiusCapture2", "radiusCapture2", nBin, 0, WLSLength); // captured photons bouncing two times
	TH1D *radiusCaptureR = new TH1D("radiusCaptureR", "radiusCaptureR", nBin, 0, WLSLength); // captured photons plotted by radius
	TH1D *radiusCapture0R = new TH1D("radiusCapture0R", "radiusCapture0R", nBin, 0, WLSLength); // captured photons without bouncing
	TH1D *radiusCapture1R = new TH1D("radiusCapture1R", "radiusCapture1R", nBin, 0, WLSLength); // captured photons bouncing 1 time
	TH1D *radiusCapture2R = new TH1D("radiusCapture2R", "radiusCapture2R", nBin, 0, WLSLength); // captured photons bouncing two times
	
	TH2D *generatedPosCart = new TH2D("generatedPosCart", "generatedPosCart", nBin, -WLSLength/2, WLSLength/2, nBin, -WLSLength/2, WLSLength/2); // mapping of plate
	TH2D *generatedDirCart = new TH2D("generatedDirCart", "generatedDirCart", nBin, -WLSLength/2, WLSLength/2, nBin, -WLSLength/2, WLSLength/2); // mapping of plate
	TH2D *captureMapCart = new TH2D("captureMapCart", "captureMapCart", nBin, -WLSLength/2, WLSLength/2, nBin, -WLSLength/2, WLSLength/2); // mapping of plate
	TH2D *captureMap = new TH2D("captureMap", "captureMap", nBin, 0, 2*PI, nBin, 0, WLSLength); // mapping of plate



        radiusHist->Sumw2();
        radiusCapture->Sumw2();
        radiusCapture0->Sumw2();
        radiusCapture1->Sumw2();
        radiusCapture2->Sumw2();
        radiusCaptureR->Sumw2();
        radiusCapture0R->Sumw2();
        radiusCapture1R->Sumw2();
        radiusCapture2R->Sumw2();
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
	
//	std::cout << "Bin:\t" << i << "\t" << radiusCapture2->GetBinContent(i) << "\t" << radiusHist->GetBinContent(i)<< "\t" <<radiusCapture2->GetBinContent(i)/radiusHist->GetBinContent(i) << std::endl;  	
	}


	radiusHist->Write();
	radiusCapture->Write();
	radiusCapture0->Write();
	radiusCapture1->Write();
	radiusCapture2->Write();
	radiusCaptureR->Write();
	radiusCapture0R->Write();
	radiusCapture1R->Write();
	radiusCapture2R->Write();
	captureMap->Write();
	generatedPosCart->Write();
	generatedDirCart->Write();
	generatedTheta->Write();
	captureMapCart->Write();
	tree->Write();

	outfile->Close();









		
	return 0;




} // End of the main function.


