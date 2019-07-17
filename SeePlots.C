#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TStyle.h"







void SeePlots(){
	gStyle->SetOptStat(0);

	TFile *inFile = new TFile("WLS50.000000x50.000000L100.000000.root", "READ");
	TTree *inTree = (TTree*) inFile->Get("simulation");

	double posX, posY, posR, dirX, dirY, dirT ;
	int hitPMT, ref;
	double WLSLength = 100.0;
	double granularity = 0.5;
//	double granularity =1;

	const int size = WLSLength/granularity;	// Make sure to check that the granularity fits in nicely

	inTree->SetBranchAddress("posX", &posX);
	inTree->SetBranchAddress("posY", &posY);
	inTree->SetBranchAddress("posR", &posR);
	inTree->SetBranchAddress("dirX", &dirX);
	inTree->SetBranchAddress("dirY", &dirY);
	inTree->SetBranchAddress("dirTheta", &dirT);
	inTree->SetBranchAddress("hitPMT", &hitPMT);
	inTree->SetBranchAddress("reflections", &ref);


	int plateGen[size][size];
	int plateHit[size][size];
	int plateHit0[size][size];
	int plateHit1[size][size];
	int plateHit2[size][size];
//	int plateGen[56][56];
//	int plateHit[56][56];

//	int plateGen[size][size];
//	int plateHit[size][size];
//	std::vector<std::vector<double (size)>> plateGen (size);
//	std::vector<std::vector<double (size)>> plateHit (size);

/*
	inTree->GetEntry(1);
	std::cout << "There are " << inTree->GetEntries() << " photons." << std::endl;
	std::cout << "pos " << posX << " " << posY << " " << posR << std::endl;
	std::cout << "dir " << dirX << " " << dirY << " " << dirT << std::endl;
	std::cout << "Hit " << hitPMT << " with " << ref << " reflections." << std::endl;
*/
	// Initialise array

	for (int i = 0; i <size; i++){

		for (int j = 0; j<size; j++){
			plateHit[i][j] = 0;
			plateHit0[i][j] = 0;
			plateHit1[i][j] = 0;
			plateHit2[i][j] = 0;
			plateGen[i][j] = 0;
		}

	}



	for (int i = 0; i < inTree->GetEntries(); i++) {
//	for (int i = 0; i < 10; i++) {
		inTree->GetEntry(i);

		int xBin = (int) ( (WLSLength/2 + posX)/granularity );
		int yBin = (int) ( (WLSLength/2 + posY)/granularity );
//		std::cout << WLSLength/2 << " " << posX << " " << granularity << std::endl;
//		std::cout << WLSLength/2 << " " << posY << " " << granularity << std::endl;
//		std::cout << xBin << " " << yBin << std::endl;
		plateGen[xBin][yBin]++;

		if (hitPMT) {
			plateHit[xBin][yBin]++;
			if (ref == 0) plateHit0[xBin][yBin]++;
			if (ref == 1) plateHit1[xBin][yBin]++;
			if (ref == 2) plateHit2[xBin][yBin]++;
		}

	}

	TH2D * mapGen = new TH2D ("mapGen", "mapGen", size, 0, WLSLength, size, 0, WLSLength);
	TH2D * mapEff = new TH2D ("mapEff", "mapEff", size, 0, WLSLength, size, 0, WLSLength);
	mapEff->SetMaximum(1);
	mapEff->SetMinimum(0);
	TH2D * mapEff0 = new TH2D ("mapEff0", "mapEff0", size, 0, WLSLength, size, 0, WLSLength);
	mapEff0->SetMaximum(1);
	mapEff0->SetMinimum(0);
	TH2D * mapEff1 = new TH2D ("mapEff1", "mapEff1", size, 0, WLSLength, size, 0, WLSLength);
	mapEff1->SetMaximum(1);
	mapEff1->SetMinimum(0);
	TH2D * mapEff2 = new TH2D ("mapEff2", "mapEff2", size, 0, WLSLength, size, 0, WLSLength);
	mapEff2->SetMaximum(1);
	mapEff2->SetMinimum(0);


	for (int i = 0; i <size; i++){

		for (int j = 0; j<size; j++){
			double Val = (double)plateHit[i][j];
			double Val0 = (double)plateHit0[i][j];
			double Val1 = (double)plateHit1[i][j];
			double Val2 = (double)plateHit2[i][j];
			double norm = (double)plateGen[i][j];
//			std::cout << i << " " << j << " " << norm << std::endl;
			if (norm == 0) norm = 1;
			double normVal = Val/norm;
			double normVal0 = Val0/norm;
			double normVal1 = Val1/norm;
			double normVal2 = Val2/norm;
			mapGen->SetBinContent(i+1,j+1, norm );
			mapEff->SetBinContent(i+1,j+1, normVal );
			mapEff0->SetBinContent(i+1,j+1, normVal0 );
			mapEff1->SetBinContent(i+1,j+1, normVal1 );
			mapEff2->SetBinContent(i+1,j+1, normVal2 );
			if (normVal >1 || normVal0>1 || normVal1>1 || normVal2 >1) std::cout << "Normalisation Error Occured."<<std::endl; //DEBUG
//			mapEff->SetBinContent(i+1,j+1, norm );

//			std::cout << i << " " << j << " " << Val << std::endl;
//			std::cout << i << " " << j << " " << plateHit[i][j] << std::endl;
		}

	}

//std::cout << "Try: " << plateGen[0][0] << std::endl;
	TCanvas *c0 = new TCanvas("c0", "c0");
	mapGen->Draw("COLZ");
	TCanvas *c1 = new TCanvas("c1", "c1");
	mapEff->Draw("COLZ");
	TCanvas *c2 = new TCanvas("c2", "c2");
	mapEff0->Draw("COLZ");
	TCanvas *c3 = new TCanvas("c3", "c3");
	mapEff1->Draw("COLZ");
	TCanvas *c4 = new TCanvas("c4", "c4");
	mapEff2->Draw("COLZ");






}
