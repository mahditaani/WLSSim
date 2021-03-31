TPolyLine* DrawBoxConts(double lowc, double highc, TH1D *h, int col, int style = 3044){

        double grLow = h->GetMinimum();
        double grHigh = h->GetMaximum();
        double x[5] = {lowc, lowc, highc, highc, lowc };
        double y[5] = {grLow, grHigh, grHigh, grLow, grLow};
        TPolyLine *a = new TPolyLine(5,x,y);
        a->SetFillStyle(style);
        a->SetFillColor(col);
        return a;
}

void ScaleReflect(TH1F *a, double b){


	for (int i = 1; i <= 10; i++){
		double c = a->GetBinContent(i);
		a->SetBinContent(i, c*pow(b,i));
	}

}

void MakePlots(){

	gStyle->SetOptStat(0);
	/*
	std::string file_0 = "EljenBigSquareNoAtt_2Ref.root";
	std::string file0 = "EljenBigSquareNoAtt.root";
	std::string file1 = "EljenBigSquareAtt100.root";
	std::string file2 = "EljenBigSquareAtt200.root";
	std::string file3 = "EljenBigSquareAtt300.root";
	std::string file4 = "EljenBigSquareAtt400.root";
	*/
	///*
	std::string file_0 = "EljenSquareNoAtt_2Ref.root";
	std::string file0 = "EljenSquareNoAtt.root";
	std::string file1 = "EljenSquareAtt100.root";
	std::string file2 = "EljenSquareAtt200.root";
	std::string file3 = "EljenSquareAtt300.root";
	std::string file4 = "EljenSquareAtt400.root";
	//*/
	/*
	std::string file_0 = "EljenCircleNoAtt_2Ref.root";
	std::string file0 = "EljenCircleNoAtt.root";
	std::string file1 = "EljenCircleAtt100.root";
	std::string file2 = "EljenCircleAtt200.root";
	std::string file3 = "EljenCircleAtt300.root";
	std::string file4 = "EljenCircleAtt400.root";
	*/
	/*
	std::string file_0 = "EljenRectangleNoAtt_2Ref.root";
	std::string file0 = "EljenRectangleNoAtt.root";
	std::string file1 = "EljenRectangleAtt100.root";
	std::string file2 = "EljenRectangleAtt200.root";
	std::string file3 = "EljenRectangleAtt300.root";
	std::string file4 = "EljenRectangleAtt400.root";
	*/

	std::string histName = "radiusCaptureR";
	int ref_0, status_0, ref0, ref1, ref2, status0, status1, status2;
	int ref3, ref4, status3, status4;
	TFile *f_0 = new TFile (file_0.c_str(), "READ");
	TTree *t_0 = (TTree*) f_0->Get("simulation");
	t_0->SetBranchAddress("reflections", &ref_0);
	t_0->SetBranchAddress("status", &status_0);
	TH1D *r_0 = (TH1D*)f_0->Get(histName.c_str());
	

	TFile *f0 = new TFile(file0.c_str(),"READ");
	TTree *t0 = (TTree*) f0->Get("simulation");
	t0->SetBranchAddress("reflections", &ref0);
	t0->SetBranchAddress("status", &status0);
	TH1D *r0 = (TH1D*)f0->Get(histName.c_str());

	TFile *f1 = new TFile(file1.c_str(),"READ");
	TTree *t1 = (TTree*) f1->Get("simulation");
	t1->SetBranchAddress("reflections", &ref1);
	t1->SetBranchAddress("status", &status1);
	TH1D *r1 = (TH1D*)f1->Get(histName.c_str());

	TFile *f2 = new TFile(file2.c_str(),"READ");
	TTree *t2 = (TTree*) f2->Get("simulation");
	t2->SetBranchAddress("reflections", &ref2);
	t2->SetBranchAddress("status", &status2);
	TH1D *r2 = (TH1D*)f2->Get(histName.c_str());

	TFile *f3 = new TFile(file3.c_str(),"READ");
	TTree *t3 = (TTree*) f3->Get("simulation");
	t3->SetBranchAddress("reflections", &ref3);
	t3->SetBranchAddress("status", &status3);
	TH1D *r3 = (TH1D*)f3->Get(histName.c_str());

	TFile *f4 = new TFile(file4.c_str(),"READ");
	TTree *t4 = (TTree*) f4->Get("simulation");
	t4->SetBranchAddress("reflections", &ref4);
	t4->SetBranchAddress("status", &status4);
	TH1D *r4 = (TH1D*)f4->Get(histName.c_str());

	TH1F *h_0 = new TH1F("no_att", "no_att", 10, 0, 10);
	TH1F *h0 = new TH1F("noatt", "noatt", 10, 0, 10);
	TH1F *h1 = new TH1F("att1", "att1", 10, 0, 10);
	TH1F *h2 = new TH1F("att2", "att2", 10, 0, 10);
	TH1F *h3 = new TH1F("att3", "att3", 10, 0, 10);
	TH1F *h4 = new TH1F("att4", "att4", 10, 0, 10);


	for(int i = 0; i < t_0->GetEntries(); i++){
		t_0->GetEntry(i);
		if (status_0 == 0) h_0->Fill(ref_0);
	}
	for(int i = 0; i < t0->GetEntries(); i++){
		t0->GetEntry(i);
		if (status0 == 0) h0->Fill(ref0);
	}
	for(int i = 0; i < t1->GetEntries(); i++){
		t1->GetEntry(i);
		if (status1 == 0) h1->Fill(ref1);
	}
	for(int i = 0; i < t2->GetEntries(); i++){
		t2->GetEntry(i);
		if (status2 == 0) h2->Fill(ref2);
	}
	for(int i = 0; i < t3->GetEntries(); i++){
		t3->GetEntry(i);
		if (status3 == 0) h3->Fill(ref3);
	}
	for(int i = 0; i < t4->GetEntries(); i++){
		t4->GetEntry(i);
		if (status4 == 0) h4->Fill(ref4);
	}


	h_0->SetLineColor(kViolet);
	h0->SetLineColor(kBlack);
	h1->SetLineColor(kBlue);
	h2->SetLineColor(kRed);
	h3->SetLineColor(kGreen);
	h4->SetLineColor(kMagenta);

	h_0->SetLineWidth(2);
	h0->SetLineWidth(2);
	h1->SetLineWidth(2);
	h2->SetLineWidth(2);
	h3->SetLineWidth(2);
	h4->SetLineWidth(2);

	r_0->SetLineColor(kViolet);
	r0->SetLineColor(kBlack);
	r1->SetLineColor(kBlue);
	r2->SetLineColor(kRed);
	r3->SetLineColor(kGreen);
	r4->SetLineColor(kMagenta);

	r_0->SetLineWidth(2);
	r0->SetLineWidth(2);
	r1->SetLineWidth(2);
	r2->SetLineWidth(2);
	r3->SetLineWidth(2);
	r4->SetLineWidth(2);

	double area_0 = 0;
	double area0 = 0;
	double area1 = 0;
	double area2 = 0;
	double area3 = 0;
	double area4 = 0;
	double parea_0 = 0;
	double parea0 = 0;
	double parea1 = 0;
	double parea2 = 0;
	double parea3 = 0;
	double parea4 = 0;

	std::cout << "Changing edge reflectivity" << std::endl;
	double reflec = 0.9;
	ScaleReflect(h_0, reflec);
	ScaleReflect(h0, reflec);
	ScaleReflect(h1, reflec);
	ScaleReflect(h2, reflec);
	ScaleReflect(h3, reflec);
	ScaleReflect(h4, reflec);
		
	for (int i = 1; i <= 10; i++){
		area_0 += h_0->GetBinContent(i);
		if (i < 4) parea_0 += h_0->GetBinContent(i);
		area0 += h0->GetBinContent(i);
		if (i < 4) parea0 += h0->GetBinContent(i);
		area1 += h1->GetBinContent(i);
		if (i < 4) parea1 += h1->GetBinContent(i);
		area2 += h2->GetBinContent(i);
		if (i < 4) parea2 += h2->GetBinContent(i);
		area3 += h3->GetBinContent(i);
		if (i < 4) parea3 += h3->GetBinContent(i);
		area4 += h4->GetBinContent(i);
		if (i < 4) parea4 += h4->GetBinContent(i);
	
	}

	std::cout << "Ratios of uncounted." << std::endl;
	std::cout << "_0:\t" << parea_0/area_0 << std::endl;
	std::cout << "0:\t" << parea0/area0 << std::endl;
	std::cout << "1:\t" << parea1/area1 << std::endl;
	std::cout << "2:\t" << parea2/area2 << std::endl;
	std::cout << "3:\t" << parea3/area3 << std::endl;
	std::cout << "4:\t" << parea4/area4 << std::endl;
	// Lets test some scaling
	double val0e = h_0->GetBinContent(1); // gets value for 0 reflection
	double val1e = h3->GetBinContent(1); // gets value for 0 reflection
	double val0r = h_0->GetBinContent(2); // gets value for 1 reflection
	double val1r = h3->GetBinContent(2); // gets value for 1 reflection
	double factorEff = val1e/val0e;
	double factorRef = val1r/(factorEff*val0r);
	std::cout << "The efficiency factor is :\t" << factorEff << std::endl;
	std::cout << "The reflectivity factor is :\t" << factorRef << std::endl;
	//factor = 1;
	for (int i = 1; i <=10; i++){
		double temp = h0->GetBinContent(i);
		//h0->SetBinContent(i,factorEff*temp*pow(factorRef, i-1));		
	
	}

	//r0->Scale(factorEff*factorRef*factorRef);


	TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
	leg->AddEntry(h0, "No Attenuation", "l");
	leg->AddEntry(h_0, "2 bounces", "l");
	leg->AddEntry(h1, "Att Length 1m", "l");
	leg->AddEntry(h2, "Att Length 2m", "l");
	leg->AddEntry(h3, "Att Length 3m", "l");
	leg->AddEntry(h4, "Att Length 4m", "l");

	h0->SetTitle("The Number of Reflections for Each Detected Hit");
	h0->GetXaxis()->SetTitle("Number of Reflections");
	TCanvas *c1 = new TCanvas ("c1", "c1");
	h0->Draw("HIST");
	h_0->Draw("HIST SAME");
	h1->Draw("HIST SAME");
	h2->Draw("HIST SAME");
	h3->Draw("HIST SAME");
	h4->Draw("HIST SAME");
	leg->Draw("SAME");
	c1->Update();

	TCanvas *c2 = new TCanvas ("c2", "c2");
	r0->Draw("HIST");
	r_0->Draw("HIST SAME");
	r1->Draw("HIST SAME");
	r2->Draw("HIST SAME");
	r3->Draw("HIST SAME");
	r4->Draw("HIST SAME");
	leg->Draw("SAME");
	c2->Update();


	TH1D *h0_f = (TH1D*)f_0->Get("radiusCaptureR");
	TH1D *h0_0 = (TH1D*)f_0->Get("radiusCapture0R");
	TH1D *h0_1 = (TH1D*)f_0->Get("radiusCapture1R");
	TH1D *h0_2 = (TH1D*)f_0->Get("radiusCapture2R");
	h0_f->SetTitle("Plate Efficiency vs Radial Distance");
	h0_f->GetXaxis()->SetTitle("Radial Distance [cm]");
	h0_f->GetYaxis()->SetTitle("Plate Efficiency");
	h0_f->GetXaxis()->SetRangeUser(0,17);
	h0_f->GetYaxis()->SetRangeUser(0,1);

	h0_f->SetMarkerColor(kBlack);
	h0_0->SetMarkerColor(kBlue);
	h0_1->SetMarkerColor(kRed);
	h0_2->SetMarkerColor(kMagenta);

	h0_f->SetMarkerStyle(20);
	h0_0->SetMarkerStyle(21);
	h0_1->SetMarkerStyle(22);
	h0_2->SetMarkerStyle(33);

	h0_f->SetLineWidth(0);
	h0_0->SetLineWidth(0);
	h0_1->SetLineWidth(0);
	h0_2->SetLineWidth(0);
	TPolyLine* pmt_r = DrawBoxConts(0, 3.81, h0_f, kBlack , 3345);
	pmt_r->SetLineWidth(0);

	// see effect
	//TH1D *modelv = (TH1D*)f_0->Get("radiusCapture0R");
	TH1D *modelv = (TH1D*)f_0->Get("radiusCapture0R")->Clone();
	TH1D *fullv =  (TH1D*)f3->Get("radiusCapture0R")->Clone();

	modelv->Scale(factorEff);
	modelv->Add((TH1D*)f_0->Get("radiusCapture1R")->Clone(), factorEff*factorRef);
	modelv->Add((TH1D*)f_0->Get("radiusCapture2R")->Clone(), factorEff*factorRef*factorRef);
	fullv->Scale(factorEff);
	fullv->Add((TH1D*)f3->Get("radiusCapture1R")->Clone());
	fullv->Add((TH1D*)f3->Get("radiusCapture2R")->Clone());
	fullv->Add((TH1D*)f3->Get("radiusCapture3R")->Clone());
	fullv->Add((TH1D*)f3->Get("radiusCapture4R")->Clone());
	fullv->Add((TH1D*)f3->Get("radiusCapture5R")->Clone());
	fullv->Add((TH1D*)f3->Get("radiusCapture6R")->Clone());
	fullv->Add((TH1D*)f3->Get("radiusCapture7R")->Clone());


	TLegend *leg2 = new TLegend(0.7,0.7,0.9,0.9);
	leg2->AddEntry(pmt_r,  "PMT Radius", "f");
	leg2->AddEntry(h0_f, "Full Efficiency", "p");
	leg2->AddEntry(h0_0, "0 Reflections", "p");
	leg2->AddEntry(h0_1, "1 Reflection", "p");
	leg2->AddEntry(h0_2, "2 Reflections", "p");


	TCanvas *c3 = new TCanvas("c3","c3");
	h0_f->Draw();
	h0_0->Draw("SAME");
	h0_1->Draw("SAME");
	h0_2->Draw("SAME");
	pmt_r->Draw("F");
	leg2->Draw("SAME");
	
	TLegend *leg3 = new TLegend(0.7,0.7,0.9,0.9);
	leg3->AddEntry(modelv,  "Model", "l");
	leg3->AddEntry(fullv, "Full Efficiency", "l");

	TCanvas *c4 = new TCanvas("c4","c4");
	modelv->Draw();
	fullv->Draw("same");
	leg3->Draw("same");
}
