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

string GetFileName(float diameter, char shape);

float Efficiency(string filename = "WLS_circle_10.000000x10.000000L100.000000_air.root"){
  
  TFile * f = new TFile(filename.c_str(), "READ");
  TTree * t = (TTree*) f->Get("simulation");
  TH1D  * h = (TH1D*)  f->Get("radiusCapture");
  
  int nBeam = t->GetEntries();
  int nPMT  = h->GetEntries();
  
  float efficiency = (float)nPMT/nBeam;
  //float radius     = h->GetXaxis()->GetXmax();
  
  return efficiency;
}

//TGraph * GraphSensitivity(){
void GraphSensitivity(char shape = 'c',
		      int firstDiameter = 10){
  

  TFile *outFile = new TFile("WLS_sensitivity.root",
			     "update",
			     "WLS_sensitivity.root"); 
  
  static const int nBins = 12;
  double efficiency[nBins]  = {0.};
  double sensitivity[nBins] = {0.};
  double diameter[nBins]    = {0.};
  
   // loop over diameters  
  int di = firstDiameter-4;
  for (int i = 0 ; i < nBins ; i++){
    
    di += 4;    
    diameter[i] = (float)di;
    efficiency[i] = Efficiency(GetFileName(diameter[i],shape));
    sensitivity[i] = efficiency[i]*Area(diameter[i],shape);
    
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
  

  TGraph * gr1 = new TGraph(nBins,diameter,efficiency);
  gr1->SetLineColor(2);
  gr1->SetLineWidth(3);
  gr1->SetMarkerColor(4);
  gr1->SetMarkerStyle(21);
  gr1->SetTitle("");
  gr1->GetXaxis()->SetTitle("diameter (cm)");
  gr1->GetYaxis()->SetTitle("efficiency per photon");

  gr1->SetMinimum(0);
  gr1->SetMaximum(0.4);
  gr1->Draw("ACP");
  
  c1->cd(2);
 
  TGraph * gr2 = new TGraph(nBins,diameter,sensitivity);
  gr2->SetLineColor(3);
  gr2->SetLineWidth(3);
  gr2->SetMarkerColor(5);
  gr2->SetMarkerStyle(21);
  gr2->SetTitle("");
  gr2->GetXaxis()->SetTitle("diameter (cm)");
  gr2->GetYaxis()->SetTitle("sensitivity = efficiency x area ");
  //  gr2->GetXaxis()->SetTitleOffset(2.);
  gr2->SetMinimum(0);
  gr2->SetMaximum(200);
  // gr2->GetXaxis()->SetRangeUser(0, 40.);
  gr2->Draw("ACP");

  if(shape=='c'){
    c1->SaveAs("./circle.pdf");
    gr1->SetName("gr_circle_efficiency");
    gr2->SetName("gr_circle_sensitivity");
  }
  else {
    c1->SaveAs("./square.pdf");
    gr1->SetName("gr_square_efficiency");
    gr2->SetName("gr_square_sensitivity");
  }
  
  
  outFile->cd();
  
  gr1->Write();
  gr2->Write();
  
  outFile->Write();
  outFile->Delete();

  
//   // TCanvas::Update() draws the frame, after which one can change it
//   c1->Update();
//   c1->GetFrame()->SetFillColor(21);
//   c1->GetFrame()->SetBorderSize(12);
//   c1->Modified();
  
}

string GetFileName(float diameter ,char shape = 'c'){

  float length = diameter;
    
  if(shape=='c')
    length = diameter/2.;
  
  string filename = "WLS_circle_20.000000x20.000000L100.000000_air.root";
  
  if(shape=='c')
    filename =  "WLS_circle_";
  else
    filename =  "WLS_square_";
  
  filename += to_string(length);

  filename += "x";
  
  filename += to_string(length);

  filename += "L100.000000_air.root";
  
  return filename;
}
