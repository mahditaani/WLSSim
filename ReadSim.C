#include <cstdlib>
#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"

/*
int ReadSim(){
// Open a ROOT file
TFile *f = new TFile("WLS.root", "READ");

// Read in the tree
TTree *t = (TTree*) f->Get("simulation");
TH1D *h = (TH1D*) f->Get("radiusCapture");

std::cout << "Number of entries in the file: " << t->GetEntries() << std::endl;
std::cout << "Number of entries in the hist: " << h->GetEntries() << std::endl;

//outfile << "abs" << std::endl;

return h->GetEntries();



int numEntries = t->GetEntries();

double posX;

t->SetBranchAddress("posX", &posX);



for (int i = 0; i < numEntries; i++){
    t->GetEntry(i);

    std::cout << "posX " << posX << std::endl;
}



}
*/

void ReadSim (){

// Creates an empty text file if it doesn't exist and replaces it if one does.
std::ofstream outfile ("data.txt");

std::cout << "Extracting data..." << std::endl;

for(int i = 50; i <= 50; i++){ // Square plate size x and y
    for(int k = 20; k <= 320; k *= 2){ // Attenuation length
        //std::string fileName = "WLS" + std::to_string(i) + ".000000x" + std::to_string(i) + ".000000.root";

        std::string fileName = "WLS" + std::to_string(i) + ".000000x" + std::to_string(i) + ".000000L" + std::to_string(k) + ".000000.root";

        // Open a ROOT file
        TFile *f = new TFile(fileName.c_str(), "READ");

        // Read in the tree
        //TTree *t = (TTree*) f->Get("simulation");
        TH1D *h = (TH1D*) f->Get("radiusCapture");
        TH1D *g = (TH1D*) f->Get("generatedPosCart");

        //std::cout << "Number of entries in the file: " << t->GetEntries() << std::endl;
        //std::cout << "Number of entries in the hist: " << h->GetEntries() << std::endl;

        outfile << i << " " << h->GetEntries() << " " << g->GetEntries() << " " << k << std::endl;
    }
}

outfile.close();

std::cout << "Done!" << std::endl;

}
