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
pythia.readString("SoftQCD:inelastic = on");
pythia.readString("Tune:ee = 7");//monash tuning
pythia.readString("Tune:pp = 14");//monash tuning
pythia.readString("Random:setSeed = on");
pythia.readString("Random:seed = 5738921");   //run time ran number
pythia.readString("Beams:idA = 2212");
pythia.readString("Beams:idB = 2212");   // pid for proton
pythia.readString("Beams:eCM = 13000.0");   //Set the C.O.M energy in GeV here

pythia.readString("PartonLevel:MPI = on");                                                                                         
pythia.readString("MultiPartonInteractions:pT0Ref = 2.15");

pythia.readString("ColourReconnection:reconnect = on");
pythia.readString("ColourReconnection:mode = 1");
pythia.readString("ColourReconnection:allowDoubleJunRem = off");                                                                   
pythia.readString("ColourReconnection:m0= 0.3");                                                                                   
pythia.readString("ColourReconnection:allowJunctions = on");                                                                       
pythia.readString("ColourReconnection:junctionCorrection = 1.2");                                                                  
pythia.readString("ColourReconnection:timeDilationMode = 2");                                                                      
pythia.readString("ColourReconnection:timeDilationPar = 0.18");

pythia.readString("Ropewalk:RopeHadronization = off"); 
    //pythia.readString("Beams:idA = 2212");
    //pythia.readString("Beams:idB = 2212");
    //pythia.readString("SoftQCD:all = on");
    // pythia.readString("Beams:eCM = 900");
    pythia.init();

    double pTMin = 0.3;
    double pTMax = 1.5;
    int eventNumber = 10000;
    vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
    vector<double> fwdMult(window_width.size(), 0.0);
    vector<double> bkwdMult(window_width.size(), 0.0);

    // Create histograms and profiles
    TH1F hist("hist", "pT", 100, pTMin, pTMax);
    TH1F hist2("hist2", "eta", 100, -3, 3);

    TH2F b_corr_vs_width("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 100, 0, 1.2);
    TProfile profile_b_corr("profile_b_corr", "b_corr vs Window Width", window_width.size() - 2, &window_width[1]);

    // Event loop
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
            // Conditional statement
            if (!pythia.event[i].isCharged() || !pythia.event[i].isFinal())
                continue;
            if (pythia.event[i].pT() < pTMin || pythia.event[i].pT() > pTMax)
                continue;

            // Scatter plots of pT and eta
            hist.Fill(pythia.event[i].pT());
            hist2.Fill(pythia.event[i].eta());

            // Choosing forward multiplicity within different pseudorapidity windows
            for (int j = 0; j < window_width.size() - 1; ++j) {
                if ((pythia.event[i].eta()) >= window_width[j] && (pythia.event[i].eta()) < window_width[j + 1]) {
                    fwdMult[j]++;
                    break;

                }
            }
            // Choosing backward multiplicity within different pseudorapidity windows
            for (int j = 0; j < window_width.size() - 1; ++j) {
                if ((pythia.event[i].eta()) <= -window_width[j] && (pythia.event[i].eta()) > -window_width[j + 1]) {
                    bkwdMult[j]++;
                    break;
                }
            }
        }   // Particle loop end


            double sum_nF_k = 0.0;
            double sum_nB_k = 0.0;
            double sum_nBnF_k = 0.0;
            double nFSquared_sum_k = 0.0;

            // Calculate and store values for each window width
            for (int k = 0; k < window_width.size() - 2; ++k) {
            sum_nF_k += fwdMult[k];
            nFSquared_sum_k += fwdMult[k] * fwdMult[k];

            sum_nB_k = 0.0;
            sum_nBnF_k = 0.0;

            for (int b = k + 1; b < window_width.size() - 1; ++b) {
                sum_nB_k += bkwdMult[b];
                sum_nBnF_k += fwdMult[k] * bkwdMult[b];
            }

            double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);

            if (std::isnan(b_corr) || std::isinf(b_corr))
                continue;

            b_corr_vs_width.Fill(window_width[k], b_corr);
            profile_b_corr.Fill(window_width[k], b_corr);
        }

    }   // Event loop end

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