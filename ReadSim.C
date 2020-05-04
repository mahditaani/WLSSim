#include <cstdlib>
#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"

#include "TH1D.h"

using namespace std;

void ReadSim(string filename){
  
  TFile *f = new TFile(filename.c_str(), "READ");
  TTree *t = (TTree*) f->Get("simulation");
  TH1D  *h = (TH1D*) f->Get("radiusCapture");
  
  cout << "Number of entries in the file: " << t->GetEntries() << endl;
  cout << "Number of entries in the hist: " << h->GetEntries() << endl;

  Double_t        posX;
  Double_t        posY;
  Double_t        posR;
  Double_t        dirX;
  Double_t        dirY;
  Double_t        dirTheta;
  Int_t           hitPMT;
  Int_t           reflections;
  Double_t        dist;
  Double_t        time;
  Int_t           status;
  
  t->SetBranchAddress("posX",&posX);
  t->SetBranchAddress("posY",&posY);
  t->SetBranchAddress("posR",&posR);
  t->SetBranchAddress("dirX",&dirX);
  t->SetBranchAddress("dirY",&dirY);
  t->SetBranchAddress("dirTheta",&dirTheta);
  t->SetBranchAddress("hitPMT",&hitPMT);
  t->SetBranchAddress("reflections",&reflections);
  t->SetBranchAddress("dist",&dist);
  t->SetBranchAddress("time",&time);
  t->SetBranchAddress("status",&status);
  
  Long64_t nentries =t->GetEntries();

  Long64_t nbytes = 0;
//   for (Long64_t i=0; i<nentries;i++) {
//     nbytes +=t->GetEntry(i);
    
//     cout << " posX = " << posX << endl;
//     cout << " posY = " << posY << endl;
    
//   }  

}

void ReadSim (){

  // Creates an empty text file if it doesn't exist and replaces it if one does.
  ofstream outfile ("data.txt");
  
  cout << "Extracting data..." << endl;
  
  for(int i = 50; i <= 50; i++){ // Square plate size x and y
    for(int k = 20; k <= 320; k += 10){ // Attenuation length
      //string fileName = "WLS" + to_string(i) + ".000000x" + to_string(i) + ".000000.root";
      
      string fileName = 
	"WLS"      + to_string(i) + 
	".000000x" + to_string(i) + 
	".000000L" + to_string(k) + 
	".000000.root";
      
      // Open a ROOT file
      TFile *f = new TFile(fileName.c_str(), "READ");
      
      // Read in the tree
      //TTree *t = (TTree*) f->Get("simulation");
      TH1D *h = (TH1D*) f->Get("radiusCapture");
      TH1D *g = (TH1D*) f->Get("generatedPosCart");
      
      //cout << "Number of entries in the file: " << t->GetEntries() << endl;
      //cout << "Number of entries in the hist: " << h->GetEntries() << endl;
	
      outfile << i << " " << h->GetEntries() << " " << g->GetEntries() << " " << k << endl;
    }
  }
  
  outfile.close();
  
  cout << "Done!" << endl;
  
}
