#include <iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TThread.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TGraph.h"

using namespace std;

int main()
{
    Pythia8::Pythia pythia;
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("SoftQCD:all = on");
    pythia.readString("Beams:eCM = 7000");
    pythia.init();

    double b_corr = 0.0;
    double fwdMult = 0.0;
    double bkwdMult = 0.0;
    double nF_sum = 0.0;
    double nFSquared_sum = 0.0;
    double nFnB_sum = 0.0;
    double nB_sum = 0.0;
    double pTMin = 0.3;
    double pTMax = 1.5;
    double etaMin = 0.0;

    //PLOT THE HISTOGRAM and make SCATTER PLOTS of pT and eta (done in event loop).
    TH2F *b_corr_vs_n_gap = new TH2F("b_corr_vs_n_gap", "b_corr_vs_n_gap", 10, 0, 1.2, 10, 0, 0.6);

    //Event loop
    for(int iEvents = 0; iEvents < 1000; ++iEvents)
    {
        if(!pythia.next()) 
        continue;

        vector<double> fwdMult; //Array to store forward multiplicity within desired pseudorapidity window.
        vector<double> bkwdMult; //Array to store backward multiplicity within desired pseudorapidity window.
        vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
        vector<double> b_corr_values; //Array to store b_corr values.
        vector<int> nF;
        vector<int> nB;

        nF_sum += nF;
        nB_sum += nB;
        nFnB_sum += nF * nB;
        nFSquared_sum += nF * nF;
        
        //Particle loop
        for(int i = 0; i < particle.event.size(); ++i)
        {
            //Conditional statement
            if(!particle.event[i].isCharged() || !pythia.event[i].isHadron() 
            || !pythia.event[i].isFinal())
            continue;
            if(!pythia.event[i].eta > pTMin || !pythia.event[i].pt() < pTMax)
            continue;
            //Plotting pT and 
            hist->Fill(pythia.event[j].pT());
            hist2->Fill(pythia.event[j].eta());

            //Choosing forward multiplicity within different pseudorapidity windows
            if(pythia.event[i].eta > 0)
            {
                for(int j = 0; j < 8; ++j)
                {
                    if(pythia.event[j].eta() > window_width[j] && window_width[j+2])
                    ++fwdMult[j];
                    //Store the values in the array
                    fwdMult.push_back(nF);
                    break;
                }
            }

            if(pythia.event[i].eta < 0)
            {
                for(int j = 0; j < 8; ++j)
                {
                    if(pythia.event[j].eta() > window_width[j] && window_width[j+2])
                    ++bkwdMult[j];
                    fwdMult.push_back(nB);
                }
            }
        }

        for(int k = 0; k < b_corr_values.size(); ++k)
        {
        b_corr = ((nFnB_sum - nF_sum * nB_sum) / (nFSquared_sum - (nF_sum) * (nF_sum)));
        double a = nF_sum - b_corr * (nF_sum);

            if (std::isnan(n_gap) || std::isinf(n_gap) || std::isnan(b_corr) || std::isinf(b_corr))
                continue;

        b_corr.push_back(b_corr_values);
        
        b_corr_vs_n_gap->Fill(window_width[j], b_corr_values[i]);
        }
    }

    // Plot the 2D histogram
    TCanvas canvas("canvas", "b_corr_vs_n_gap", 800, 600);
    canvas.cd();
    b_corr_vs_n_gap->Draw("COLZ");
    canvas.SaveAs("b_corr_vs_n_gap.png");

    return 0;
}