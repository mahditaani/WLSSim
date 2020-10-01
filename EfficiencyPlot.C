void DrawLine(TH1D* h, TLegend* l, std::string s, int col, string opt = ""){

	TGraph *g = new TGraph();
	string n = h->GetName();
	n += "_graph";
	g->SetName(n.c_str());
	for (int i = 1; i <= h->GetNbinsX(); i++)
		  if (h->GetBinContent(i) != 0.0){
			      g->SetPoint(g->GetN(), h->GetBinCenter(i), h->GetBinContent(i));
	}
	g->GetYaxis()->SetRangeUser(0, h->GetMaximum()*1.5);
	g->SetLineColor(col);
	g->SetLineWidth(2);
	g->GetXaxis()->SetTitle("Radial Distance [cm]");
	g->GetYaxis()->SetTitle("Efficiency");
	l->AddEntry(g, s.c_str(), "l"); // add entry to legend 
	if (g->GetN() > 1) 
		g->Draw(opt.c_str());
}


void EfficiencyPlot(std::string file = "WLS.root"){

	double reflectivity = 0.9;
	double efficiency = 0.3;

	TFile *input = new TFile (file.c_str(), "READ");

	TH1D* r0 = (TH1D*) input->Get("radiusCapture0R"); 
	TH1D* r1 = (TH1D*) input->Get("radiusCapture1R"); 
	TH1D* r2 = (TH1D*) input->Get("radiusCapture2R"); 
	TH1D* r3 = (TH1D*) input->Get("radiusCapture3R"); 
	TH1D* r4 = (TH1D*) input->Get("radiusCapture4R"); 
	TH1D* r5 = (TH1D*) input->Get("radiusCapture5R"); 
	TH1D* r6 = (TH1D*) input->Get("radiusCapture6R"); 
	TH1D* r7 = (TH1D*) input->Get("radiusCapture7R"); 
	
	r0->Scale( efficiency * pow(reflectivity, 0) );
	r1->Scale( efficiency * pow(reflectivity, 1) );
	r2->Scale( efficiency * pow(reflectivity, 2) );
	r3->Scale( efficiency * pow(reflectivity, 3) );
	r4->Scale( efficiency * pow(reflectivity, 4) );
	r5->Scale( efficiency * pow(reflectivity, 5) );
	r6->Scale( efficiency * pow(reflectivity, 6) );
	r7->Scale( efficiency * pow(reflectivity, 7) );

	TH1D* full = (TH1D*) r0->Clone();
	
	full->Add(r1);
	full->Add(r2);
	full->Add(r3);
	full->Add(r4);
	full->Add(r5);
	full->Add(r6);
	full->Add(r7);
	TLegend* leg = new TLegend(0.7, 0.7, 0.9, 0.9);
	TCanvas *c1 = new TCanvas ("c1","c1");

	DrawLine(full, leg, "Full",  kBlack);
	DrawLine(r0, leg, "0 reflections", kBlue, "SAME");
	DrawLine(r1, leg, "1 reflection", kRed, "SAME");
	DrawLine(r2, leg,  "2 reflections", kMagenta, "SAME");
	leg->Draw("SAME");
}
