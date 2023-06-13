#include<iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TThread.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"

using namespace std;

int main()
{
    Pythia8::Pythia pythia;
    pythia.readString("Beams:eCM = 7000");
    pythia.readString("SoftQCD:all = on");
    pythia.init();

    double etaCut = 0.8; //Pseudorapidity cut for forwards-backward multiplicity correlation
    double pTMin  = 0.3;
    double pTMax  = 1.5;

    double b_corr        = 0.0;
    double nF_sum        = 0.0;
    double nFSquared_sum = 0.0;
    double nFnB_sum      = 0.0;
    double nB_sum        = 0.0;

    TH2F *nB_vs_nF = new TH2F("nB_vs_nF", "histogram", 20, 0, 30, 20, 0, 30);
    TH1F *hist     = new TH1F("hist", "histogram", 50, 0, 30);

    for(int i = 0; i < 10000; ++i)
    {
        if(!pythia.next()) continue;

        int nF = 0;
        int nB = 0;

        for(int i = 0; i < pythia.event.size(); ++i)
        {
            if(!pythia.event[i].isCharged() || !pythia.event[i].isFinal()) continue;
            if(pythia.event[i].pT() < pTMin || pythia.event[i].pT() > pTMax) continue;
        
            hist->Fill(pythia.event[i].pT());
            if(pythia.event[i].eta() < 0.8)
                nF++;
            if(pythia.event[i].eta() > -0.8)
                nB++;        
        }   
    
    
        nF_sum        += nF;
        nB_sum        += nB;
        nFnB_sum      += nF * nB;
        nFSquared_sum += nF * nF;

        nB_vs_nF->Fill(nF, nB);

        b_corr   = ((nFnB_sum - nF_sum * nB_sum) / (nFSquared_sum - (nF_sum) * (nF_sum)));
        double a = nF_sum - b_corr * (nF_sum);

            cout << "nF = " <<  nF << endl;
            cout << "nB = " <<  nB << endl;
    }
    // cout<< "b_corr = " << b_corr << endl;
    // cout<< "a = " << a << endl;

    // // Fill the histogram with data points
    // for(int i = 0; i < 10000; ++i)
    // {
    //     int nF = 0;
    //     int nB = 0;
    //     for(int j = 0; j < 20; ++j)
    //     {
    //         if(j < nF_sum)
    //             nF = j;
    //         else
    //             nB = nF-j;
    //         nB_vs_nF->Fill(nF, nB);
    //         cout << "nF = " <<  nF << endl;
    //         cout << "nB = " <<  nB << endl;
    //     }

    
    TCanvas canvas("canvas", "FB_Corr", 600, 600);
    canvas.cd();
    nB_vs_nF->Draw("COLZ");

    TFile file("FB_output.root", "recreate");
    nB_vs_nF->Write();
    hist->Write();
    file.Close();
    
    pythia.stat();
    return 0;


}