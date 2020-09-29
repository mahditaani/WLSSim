#include <iostream>
#include <string>
#include "TROOT.h"
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TFrame.h"

#include "Area.C"

#include "wmStyle.C"

float Area(float diameter, char shape);

string GetFileName(float diameter, char shape, string path);

float Efficiency(string filename = "WLS_circle_10.000000x10.000000L100.000000_air.root"){
  
  TFile * f = new TFile(filename.c_str(), "READ");
  TTree * t = (TTree*) f->Get("simulation");
  TH1D  * h = (TH1D*)  f->Get("radiusCapture");
  
  int nBeam = t->GetEntries();
  int nPMT  = h->GetEntries();
  
  float efficiency = (float)nPMT/nBeam;
  
  return efficiency;
}

void GraphSensitivity(char shape = 'c',
		      int  firstDiameter = 10,
		      bool mylar = true,
		      bool eightInch = false, // 3 inch std
		      int  nFiles = 13, 
		      string path = "../Simulation_Data"){
  
  string outfileName;
  float  pmt_diameter = 3.0*2.54; // 7.62
  if(eightInch){
    nFiles = 11;
    path += "/8_Inch_Mylar/";
    outfileName = "WLS_sensitivity_8_inch.root";
    pmt_diameter = 8.0*2.54; // 20.32
  }
  else {
    outfileName = "WLS_sensitivity_3_inch.root";
    if(mylar)
      path += "/Yes_Mylar/";
    else
      path += "/Not_Mylar/";
  }
  

  TFile *outFile = new TFile(outfileName.c_str(),
			     "update",
			     outfileName.c_str()); 
  
  
//   double efficiency[nFiles]  = {0.};
//   double sensitivity[nFiles] = {0.};
//   double diameter[nFiles]    = {0.};

  double efficiency[100];
  double sensitivity[100];
  double diameter[100];
  
   // loop over diameters  
  int di = firstDiameter-4;
  for (int i = 0 ; i < nFiles ; i++){
    
    di += 4;    
    diameter[i] = (float)di;
    efficiency[i] = Efficiency(GetFileName(diameter[i],shape,path));
    // need to include subtraction of pmt circle
    sensitivity[i] = efficiency[i]*(Area(diameter[i],shape)-Area(pmt_diameter,'c')); 
    
    cout << endl;
    cout << " diameter    = " << diameter[i] << endl;
    cout << " efficiency  = " << efficiency[i] << endl;
    cout << " sensitivity = " << sensitivity[i] << endl;
    cout << endl;  
     
  }
  
  TStyle *wmStyle = GetwmStyle();
  
  wmStyle->SetPadLeftMargin(1.5);
  wmStyle->SetLabelSize(0.035,"x");
  wmStyle->SetLabelSize(0.035,"y");
  
  gROOT->SetStyle("wmStyle");
  
  gROOT->ForceStyle();

  TCanvas *c1 = new TCanvas("c1","",200,10,1200,600);
  c1->SetGrid();

  c1->Divide(2,1);
  
  c1->cd(1);
    
  gPad->SetTitle(0);
  

  TGraph * gr1 = new TGraph(nFiles,diameter,efficiency);
  gr1->SetLineColor(2);
  gr1->SetLineWidth(3);
  gr1->SetMarkerColor(4);
  gr1->SetMarkerStyle(21);
  gr1->SetTitle("");
  gr1->GetXaxis()->SetTitle("diameter (cm)");
  gr1->GetYaxis()->SetTitle("efficiency per photon");

//   gr1->SetMinimum(0);
//   gr1->SetMaximum(0.4);
  gr1->Draw("ACP");
  
  c1->cd(2);
 
  TGraph * gr2 = new TGraph(nFiles,diameter,sensitivity);
  gr2->SetLineColor(3);
  gr2->SetLineWidth(3);
  gr2->SetMarkerColor(5);
  gr2->SetMarkerStyle(21);
  gr2->SetTitle("");
  gr2->GetXaxis()->SetTitle("diameter (cm)");
  gr2->GetYaxis()->SetTitle("sensitivity = efficiency x area ");
  //  gr2->GetXaxis()->SetTitleOffset(2.);
//   gr2->SetMinimum(0);
//   gr2->SetMaximum(200);
  // gr2->GetXaxis()->SetRangeUser(0, 40.);
  gr2->Draw("ACP");

  string name_effi = "gr_circle_efficiency_yes_mylar";
  string name_sens = "gr_circle_sensitivity_yes_mylar";
  string name_canv = "circle_yes_mylar_three_inch.pdf";
  
  if(shape=='c'){
    
    if(!mylar){
      name_effi = "gr_circle_efficiency_not_mylar";
      name_sens = "gr_circle_sensitivity_not_mylar";
      name_canv = "circle_not_mylar";
    }
    else{ 
      name_effi = "gr_circle_efficiency_yes_mylar";
      name_sens = "gr_circle_sensitivity_yes_mylar";
      name_canv = "circle_yes_mylar";
    }
  }
  else {
    
    if(!mylar){
      name_effi = "gr_square_efficiency_not_mylar";
      name_sens = "gr_square_sensitivity_not_mylar";
      name_canv = "square_not_mylar";
    }
    else{
      name_effi = "gr_square_efficiency_yes_mylar";
      name_sens = "gr_square_sensitivity_yes_mylar";
      name_canv = "square_yes_mylar";
    }
  }

  if(eightInch)
    name_canv += "_eight_inch.pdf";
  else
    name_canv += "_three_inch.pdf";
  
  c1->SaveAs(name_canv.c_str());

  gr1->SetName(name_effi.c_str());
  gr2->SetName(name_sens.c_str());
  
  outFile->cd();
  
  gr1->Write();
  gr2->Write();
  
  outFile->Write();
  outFile->Delete();
  
}

string GetFileName(float diameter ,char shape = 'c', string path = "../Simulation_Data/"){

  float length = diameter;
    
  if(shape=='c')
    length = diameter/2.;
  
  string filename = "WLS_circle_20.000000x20.000000L100.000000_air.root";
  
  filename = path;

  if(shape=='c')
    filename +=  "WLS_circle_";
  else
    filename +=  "WLS_square_";
  
  filename += to_string(length);

  filename += "x";
  
  filename += to_string(length);

  filename += "L100.000000_air.root";
  
  return filename;
}
