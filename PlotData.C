    
    // results of fit
    Double_t p0[9];
    Double_t p1[9];
    Double_t p2[9];

double Mean(int n, double *a){
    // First calculate the mean
    double mean = 0;
    for (int i = 0; i < n; i++){
        mean += a[i];
    }
    return mean /= (double)n;
}

double StdDev(int n, double *a){
    // First calculate the mean
    double mean = Mean(n,a);    

    //Then calculate the residuals squared and sum them up
    double sums = 0;
    double res = 0;
    for (int i = 0; i < n; i++){
        res = pow( a[i] - mean ,2);
        sums += res;
    }
    sums /= (double)(n-1);
    sums = sqrt(sums);

    std::cout << "Mean: " << mean << " +- " << sums << std::endl;
    return sums;

}


Double_t poly(Double_t *x, Double_t *par){
    return 
    par[0] + 
    par[1]*x[0] + 
    par[2]*x[0]*x[0] + 
    par[3]*x[0]*x[0]*x[0] + 
    par[4]*x[0]*x[0]*x[0]*x[0] + 
    par[5]*x[0]*x[0]*x[0]*x[0]*x[0] + 
    par[6]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0] + 
    par[7]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0] +
    par[8]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0]*x[0];    

}
Double_t wlsfit(Double_t *x, Double_t *par){
    return 
    par[0]*(poly(x, p0) + par[1]*poly(x, p1) + par[1]*par[1]*poly(x, p2));

}

double GetChi(TH1D* a, std::string mod = "fitFcn" ){
    double val1 = a->GetFunction(mod.c_str())->GetChisquare();
    double val2 = a->GetFunction(mod.c_str())->GetNDF();
    return val1/val2;

}



double pmtNoise[5] = {0.00609601, 0.00693633, 0.00707649, 0.00573606, 0.00670786};

double pmt0_0[7] = {0.263946, 0.264002, 0.273004, 0.268125, 0.273034, 0.275725, 0.272969};

double pmt0_90[3] = {0.238377, 0.289769, 0.289866};



//double pmtSignal[9] = {0.291147, 0.243487, 0.267405, 0.234018, 0.142301, 0.017365, 0.0146086, 0.0154078, 0.0116965};
double pmtSignal[8] = {0.234058, 0.237695, 0.231878, 0.23749, 0.0732485, 0.00706507, 0.00931138, 0.00902537};
//double pmtSignalX[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
double pmtSignalX[8] = {0, 1, 2, 3, 4, 5, 6, 7};


double kuraraySignal[10] = {0.206982, 0.0901313, 0.0425512, 0.023341, 0.0215871, 0.0230505, 0.017642, 0.0170367, 0.0177709, 0.00898462};
double kuraraySignalX[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

double eljenSquareSignal[8] = {0.215201, 0.10851, 0.0363705, 0.0336374, 0.0220142, 0.0196729, 0.021078, 0.0209378};

double eljenSquareSignalX[8] = {3, 4, 5, 6, 7, 8, 9, 10};

double eljenRectSignal[23] = {0.226576, 0.0873089, 0.0290761, 0.0281246, 0.0253032, 0.0205229, 0.0170097, 0.0167401, 0.0168162, 0.0151671, 0.0141192, 0.0146515, 0.013896, 0.014136, 0.0140285, 0.0139435, 0.0125839, 0.0138376, 0.0104417, 0.0136138, 0.0129643, 0.010185, 0.0100856};
double eljenRectSignalX[23] = {3, 4, 5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22, 23, 24, 25};

double eljenRectSignalMylar[21] = {0.227384, 0.0967949, 0.0264549, 0.0327548, 0.0321295, 0.0208333, 0.0226523, 0.0200792, 0.0208763, 0.0168912, 0.0206993, 0.0193986, 0.0190015, 0.0227855, 0.018985, 0.0191073, 0.019538, 0.0150137, 0.023238, 0.0216836, 0.022592};
double eljenRectSignalMylarX[21] = {3, 4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21, 22, 23};

/*
double skodPetalRectSignalMylar[19] = {1.04545111,1.013711309,1.074274537,0.938146761,0.368599955,0.127073959,0.115014339,0.113192495,0.103037421,0.099959994,0.096186119,0.093540015,0.094206388,0.098087896,0.091662377,0.098341149,0.122410927,0.118372323,0.166112608};
double skodPetalRectSignalMylarX[19] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};
*/

double skodPetalRectSignalMylar[15] = {0.368599955,0.127073959,0.115014339,0.113192495,0.103037421,0.099959994,0.096186119,0.093540015,0.094206388,0.098087896,0.091662377,0.098341149,0.122410927,0.118372323,0.166112608};
double skodPetalRectSignalMylarX[15] = {4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};



// Errors

double noiseMean = Mean(5, pmtNoise);
double noiseSD = StdDev(5, pmtNoise);
double signalCenterMean = Mean(7, pmt0_0);
double signalCenterSD = StdDev(7, pmt0_0);
double signalEdgeMean = Mean(3,pmt0_90);
double signalEdgeSD = StdDev(3,pmt0_90);
double positionError = 0.2; //cm


// first work out the true signal
// f = 1*A - 1*B
// Sig = PlateSig - PlateBack
// stdf = sqrt( sdA^2 + sdB^2 )


// then work out the efficiency
// f = A/B
// stdf = |f|*sqrt( (sdA/A)^2 + (sdB/B)^2 )

// fill plate points, correcting the signal in array as, calculating the error for array ae and giving a position error for axe
void FillDataPoints(int n, double *as, double *ae, double *axe){

    for (int i = 0; i < n; i++){

        as[i] -= noiseMean;
        // error from subtracting noise
        double err = sqrt(  pow(noiseSD, 2) + pow(signalCenterSD, 2) ); 
   //     as[i] /= signalCenterMean;
        // error from dividing by center signal
        ae[i] = abs(as[i])*sqrt(  pow( err/as[i], 2) + pow( signalCenterSD/signalCenterMean, 2) );

        axe[i] = positionError;

    }

}

void PlotData(){

    double lowfit = 7;
    double highfit = 18;

    double pmtSignalError[8];
    double pmtSignalXError[8]; 

    double kuraraySignalError[10]; 
    double kuraraySignalXError[10]; 

    double eljenSquareSignalError[8];
    double eljenSquareSignalXError[8];

    double eljenRectSignalError[23];
    double eljenRectSignalXError[23];

    double eljenRectSignalMylarError[21]; 
    double eljenRectSignalMylarXError[21]; 

    double skodPetalRectSignalMylarError[15]; 
    double skodPetalRectSignalMylarXError[15]; 

    FillDataPoints(8, pmtSignal, pmtSignalError, pmtSignalXError);
    FillDataPoints(10, kuraraySignal, kuraraySignalError, kuraraySignalXError);
    FillDataPoints(8, eljenSquareSignal, eljenSquareSignalError, eljenSquareSignalXError);
    FillDataPoints(23, eljenRectSignal, eljenRectSignalError, eljenRectSignalXError);
    FillDataPoints(21, eljenRectSignalMylar, eljenRectSignalMylarError, eljenRectSignalMylarXError);
    FillDataPoints(15, skodPetalRectSignalMylar, skodPetalRectSignalMylarError, skodPetalRectSignalMylarXError);

    

    TGraphErrors *pmt = new TGraphErrors(8, pmtSignalX, pmtSignal, pmtSignalXError, pmtSignalError);

    TGraphErrors *kuraray = new TGraphErrors(10, kuraraySignalX, kuraraySignal, kuraraySignalXError, kuraraySignalError);

    TGraphErrors *eljensquare = new TGraphErrors(8, eljenSquareSignalX, eljenSquareSignal, eljenSquareSignalXError, eljenSquareSignalError);

    TGraphErrors *eljenrect = new TGraphErrors(23, eljenRectSignalX,  eljenRectSignal, eljenRectSignalXError, eljenRectSignalError);

    TGraphErrors *eljenrectmylar = new TGraphErrors(21, eljenRectSignalMylarX,  eljenRectSignalMylar, eljenRectSignalMylarXError, eljenRectSignalMylarError);
  
    TGraphErrors *skodpetalrectmylar = new TGraphErrors(15, skodPetalRectSignalMylarX,  skodPetalRectSignalMylar, skodPetalRectSignalMylarXError, skodPetalRectSignalMylarError);


    pmt->SetName("pmt");
    kuraray->SetName("kuraray");
    eljensquare->SetName("eljensquare");
    eljenrect->SetName("eljenrect");
    eljenrectmylar->SetName("eljenrectmylar");
    skodpetalrectmylar->SetName("skodpetalrectmylar");

    pmt->SetTitle("pmt");
    kuraray->SetTitle("kuraray");
    eljensquare->SetTitle("eljensquare");
    eljenrect->SetTitle("eljenrect");
    eljenrectmylar->SetTitle("eljenrectmylar");
    skodpetalrectmylar->SetTitle("skodpetalrectmylar");

    pmt->SetLineColor(kBlack);
    kuraray->SetLineColor(kBlue);
    eljensquare->SetLineColor(kRed);
    eljenrect->SetLineColor(kMagenta);
    eljenrectmylar->SetLineColor(kGreen);
    skodpetalrectmylar->SetLineColor(kGray);

    pmt->SetLineWidth(3);
    kuraray->SetLineWidth(3);
    eljensquare->SetLineWidth(3);
    eljenrect->SetLineWidth(3);
    eljenrectmylar->SetLineWidth(3);
    skodpetalrectmylar->SetLineWidth(3);

    TLegend *leg = new TLegend(0.7,0.7,1.0,1.0);
    leg->AddEntry(pmt, "PMT Only", "l");
    leg->AddEntry(kuraray, "Kuraray", "l");
    leg->AddEntry(eljensquare, "Eljen Square", "l");
    leg->AddEntry(eljenrect, "Eljen Rectangle", "l");
    leg->AddEntry(eljenrectmylar, "Eljen Rectangle Mylar", "l");
    leg->AddEntry(skodpetalrectmylar, "SKOD Petal Rectangle Mylar", "l");


    /*
    TCanvas *c1 = new TCanvas("c1", "c1");
    pmt->Draw();
    TCanvas *c2 = new TCanvas("c2", "c2");
    kuraray->Draw();
    TCanvas *c3 = new TCanvas("c3", "c3");
    eljensquare->Draw();
    TCanvas *c4 = new TCanvas("c4", "c4");
    eljenrect->Draw();
    TCanvas *c5 = new TCanvas("c5", "c5");
    eljenrectmylar->Draw();
    */
    ///*
    TCanvas *c10 = new TCanvas("c10", "c10");
    eljenrectmylar->Draw();
    pmt->Draw("same");
    kuraray->Draw("same");
    eljensquare->Draw("same");
    eljenrect->Draw("same");
    eljenrectmylar->Draw("same");
    skodpetalrectmylar->Draw("same");
    leg->Draw("same");
    //*/




    // lets try to fit these things now
    //TFile *infilemylar = new TFile("JustToRun/WLS_rectangle_29.500000x49.500000L-1.000000_mylar.root", "READ");
    TFile *infilemylar = new TFile("WLS_rectangle_7.620000x36.000000L-1.000000.root", "READ");
    TH1D* mylar0 = (TH1D*)infilemylar->Get("radiusCapture0");
    TH1D* mylar1 = (TH1D*)infilemylar->Get("radiusCapture1");
    TH1D* mylar2 = (TH1D*)infilemylar->Get("radiusCapture2");
    TH1D* radiusmylar = (TH1D*)infilemylar->Get("radiusHist");
    mylar0->Divide(radiusmylar);
    mylar1->Divide(radiusmylar);
    mylar2->Divide(radiusmylar);
    TCanvas *c1 = new TCanvas("c1","c1");
    mylar0->Draw();
    TCanvas *c2 = new TCanvas("c2","c2");
    mylar1->Draw();
    TCanvas *c3 = new TCanvas("c3","c3");
    mylar2->Draw();

       

    TH1D *mylardat = new TH1D("mdat", "mdat", 16, 3, 18);
    for (int i = 1; i < 16; i++){
        //mylardat->SetBinContent(i, eljenRectSignalMylar[i-1]);
        //mylardat->SetBinError(i, eljenRectSignalMylarError[i-1]);
        mylardat->SetBinContent(i, skodPetalRectSignalMylar[i-1]);
        mylardat->SetBinError(i, skodPetalRectSignalMylarError[i-1]);
	std::cout << "Point:\t" << i << std::endl;
	std::cout << skodPetalRectSignalMylar[i-1] << std::endl;

    }

    // create a TF1 with the range from 0 to 25 and 9 parameters
    TF1 *fitFcn = new TF1("fitFcn",poly,lowfit,highfit,9);
    //mylardat->Fit("fitFcn");
    mylar0->Fit("fitFcn", "","",  lowfit, highfit);
    mylar1->Fit("fitFcn", "","",  lowfit, highfit);
    mylar2->Fit("fitFcn", "","",  lowfit, highfit);


    for (int i = 0; i < 9; i++){
        p0[i] = mylar0->GetFunction("fitFcn")->GetParameter(i);
        p1[i] = mylar1->GetFunction("fitFcn")->GetParameter(i);
        p2[i] = mylar2->GetFunction("fitFcn")->GetParameter(i);
    }
    

    TCanvas *c4 = new TCanvas("c4","c4");
    TF1 *fitmodel = new TF1("fitmodel",wlsfit,lowfit,highfit,2);
    mylardat->Fit("fitmodel", "","",  lowfit, highfit);

    std::cout << "chi0:\t" << GetChi(mylar0) << std::endl;
    std::cout << "chi1:\t" << GetChi(mylar1) << std::endl;
    std::cout << "chi2:\t" << GetChi(mylar2) << std::endl;
    std::cout << "chi:\t" << GetChi(mylardat, "fitmodel") << std::endl;
    //TCanvas *c4 = new TCanvas("c4","c4");
    //mylardat->Draw();






}



/*
for (int i = 0; i < 9; i++){

    pmtSignal[i] -= noiseMean;
    // error from subtracting noise
    double err = sqrt(  pow(noiseSD, 2) + pow(signalCenterSD, 2) ); 
    pmtSignal[i] /= signalCenterMean;
    // error from dividing by center signal
    pmtSignalError[i] = abs(pmtSignal[i])*sqrt(  pow( err/pmtSignal[i], 2) + pow( signalCenterSD/signalCenterMean, 2) );

    pmtSignalXError[i] = positionError;

}
*/
