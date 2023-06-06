#include<iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TBrowser.h"

int main()
{
    Pythia8::Pythia pythia;
    pythia.readString("Beams:eCM  = 7000.");
    pythia.readString("SoftQCD:all  = on");
    pythia.init();

    double etaCut = 0.4;
    double pTMin = 0.3;
    double pTMax = 1.5;

    //Variables for correlation function calculations
    double b_corr = 0.0;
    double nF_Sum = 0.0;
    double nB_Sum = 0.0;
    double nBnF_Sum = 0.0;
    double nFSquared_Sum = 0.0;

    for(int iEvent = 0; iEvent < 10000; ++iEvent)
    {
        if(!pythia.next()) continue;

        //Counter initialization for each event
        int nF = 0;
        int nB = 0;

        for(int i = 0; i < pythia.event.size(); ++i)   
        {
            if(pythia.event[i].isFinal() && pythia.event[i].isVisible() &&
            fabs(pythia.event[i].eta()) < etaCut && pythia.event[i].pT() > pTMin && pythia.event[i].pT() < pTMax)
        {
        if(pythia.event[i].eta() > 0)
            nF++;
            else
            nB++;
        
        }
        }
        nF_Sum += nF;
        nB_Sum += nB;
        nFSquared_Sum += nF * nF;
        nBnF_Sum += nB * nF;
    }
    
    b_corr = ((nBnF_Sum - nF_Sum * nB_Sum) / (nFSquared_Sum - (nF_Sum) * (nF_Sum)));

    double a = nF_Sum - b_corr * (nF_Sum);

    std::cout << "a = " << a << std::endl;
    std::cout << "b_corr = " << b_corr << std::endl;

    TH1D hist("correlation_histogram2", "FB_corr; Number of Particles (n_F); Number of Backward Particles (<n_B>)",
                20, 0, 20);
    
    for(int i = 0; i < 10000; ++i);
    {
        int nF = 0;
        int nB = 0;
        for(int j = 0; j < 20; ++j)
        {
            if (j < nF_Sum)
                nF = j;
            else
                nF = nF_Sum;
            nB = a + b_corr * nF;
            hist.Fill(nF, nB);
        }
    }

    TCanvas canvas("canvas2", "FB_corr Histogram", 600, 600);
    canvas.cd();
    hist.Draw("COLZ");

    TFile file("FB_output.root", "recreate");
    hist.Write();
    file.Close();

    pythia.stat();
    return 0;
}
