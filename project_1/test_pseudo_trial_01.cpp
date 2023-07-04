/* ***************************************************************************************** */
#include <iostream>
#include <vector>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"

using namespace std;

int main()
{
    Pythia8::Pythia pythia;
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("SoftQCD:all = on");
    pythia.readString("Beams:eCM = 7000");
    pythia.init();

    // Declaration of variables and vectors used in the event loop
    double pTMin = 0.3;
    double pTMax = 1.5;
    int eventNumber = 50000;
    vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
    vector<double> fwdMult(window_width.size(), 0.0);
    vector<double> bkwdMult(window_width.size(), 0.0);

    // Create histograms and profiles
    
    /*Scatter plots of eta and pT to see if the event loop runs fine*/
    TH1F hist("hist", "pT", 100, pTMin, pTMax);
    TH1F hist2("hist2", "eta", 100, -3, 3);

    /*Plot of b_corr versus n_gap and its TProfile*/
    TH2F b_corr_vs_width("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 100, 0, 1.2);
    TProfile profile_b_corr("profile_b_corr", "b_corr vs Window Width", window_width.size() - 2, &window_width[1]);

    // Event loop begins
    for (int iEvents = 0; iEvents < eventNumber; ++iEvents)
    {
        if (!pythia.next())
            continue;

        // Reset forward and backward multiplicities for each event
        fill(fwdMult.begin(), fwdMult.end(), 0.0);
        fill(bkwdMult.begin(), bkwdMult.end(), 0.0);

        // Particle loop
        for (int i = 0; i < pythia.event.size(); ++i)
        {
            // Declaring cut on transverse momentum
            if (!pythia.event[i].isCharged() || !pythia.event[i].isFinal())
                continue;
            if (pythia.event[i].pT() < pTMin || pythia.event[i].pT() > pTMax)
                continue;

            // Scatter plots of pT and eta
            hist.Fill(pythia.event[i].pT());
            hist2.Fill(pythia.event[i].eta());

            // Choosing and storing forward multiplicity within different pseudorapidity windows
            for (int j = 0; j < window_width.size() - 1; ++j) {
                if ((pythia.event[i].eta()) >= window_width[j] && (pythia.event[i].eta()) < window_width[j + 1]) {
                    fwdMult[j]++;
                    break;

                }
            }
            // Choosing and storing backward multiplicity within different pseudorapidity windows
            for (int j = 0; j < window_width.size() - 1; ++j) {
                if ((pythia.event[i].eta()) <= -window_width[j] && (pythia.event[i].eta()) > -window_width[j + 1]) {
                    bkwdMult[j]++;
                    break;
                }
            }
        }    // Particle loop ends

            // Initialization of variables used in b_corr calculation
            double sum_nF_k = 0.0;
            double sum_nB_k = 0.0;
            double sum_nBnF_k = 0.0;
            double nFSquared_sum_k = 0.0;

            // Calculate and store values for each window width
            for (int k = 0; k < window_width.size() - 2; ++k) 
            {
            // Picking out the forward multiplicity particles
            sum_nF_k += fwdMult[k];
            nFSquared_sum_k += fwdMult[k] * fwdMult[k];

            sum_nB_k = 0.0;
            sum_nBnF_k = 0.0;

            // Picking out the backward multiplicity particles
            sum_nB_k += bkwdMult[k];
            sum_nBnF_k += fwdMult[k] * bkwdMult[k];

            double b_corr = (sum_nBnF_k / eventNumber - (sum_nF_k * sum_nB_k) / eventNumber) / (nFSquared_sum_k / eventNumber - (sum_nF_k * sum_nF_k) / eventNumber);
            /*------------------------------------------------------------------------------------------------------------------------------------------------------*/
            // Since we are taking TProfile of b_corr, I suppose there is no need to divide by eventNumber in the calculation of b_corr. NEED TO VERIFY !!
            /*------------------------------------------------------------------------------------------------------------------------------------------------------*/
            if (std::isnan(b_corr) || std::isinf(b_corr))
                continue;

            b_corr_vs_width.Fill(window_width[k], b_corr);
            profile_b_corr.Fill(window_width[k], b_corr);
            }
    }   // Event loop ends

    // Create a ROOT file and save the histograms
    TFile outFile("output.root", "RECREATE");
    hist.Write();
    hist2.Write();
    b_corr_vs_width.Write();
    profile_b_corr.Write();
    outFile.Close();

    // Plot the histogram
    TCanvas canvas("canvas", "b_corr vs Window Width", 800, 600);
    gStyle->SetOptStat(0); // Disable statistics box
    b_corr_vs_width.GetZaxis()->SetRangeUser(-1, 1); // Set z-axis range
    b_corr_vs_width.Draw("colz");
    canvas.SetLogz();
    canvas.Print("b_corr_vs_width.png");

    return 0;
}