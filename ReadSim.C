#include <cstdlib>
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TTree.h"

void ReadSim (){

// Creates an empty text file if it doesn't exist and replaces it if one does.
std::ofstream outfile ("data.txt");

// Main loop
std::cout << "Extracting data..." << std::endl;
for(int i = 30; i <= 37; i++){ 				// Loops square plate size  [cm_x,cm_y]
    for(int k = 100; k <= 100; k += 10){ 	// Loops attenuation length [cm_l]
		
		// Generates and checks if the corresponding file exists - otherwise it skips this iteration
        std::string fileName = "WLS" + std::to_string(i) + ".000000x" + std::to_string(i) + ".000000L" + std::to_string(k) + ".000000.root";

		std::ifstream exists(fileName);
		if(!exists){
			std::cout << fileName << " does not exist" << std::endl;
			break;
			}
		
        // Opens the ROOT file
        TFile *f = new TFile(fileName.c_str(), "READ");

        // Reads in the tree
        TH1D *h = (TH1D*) f->Get("radiusCapture");
        TH1D *g = (TH1D*) f->Get("generatedPosCart");

		// Outputs the data into the text file
        outfile << i << " " << h->GetEntries() << " " << g->GetEntries() << " " << k << std::endl;
    }
}

outfile.close();
std::cout << "Done!" << std::endl;
}

/* Saved commands that might be helpful later..
TTree *t = (TTree*) f->Get("simulation");

double posX;
t->SetBranchAddress("posX", &posX);
*/
