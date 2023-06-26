#include <iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TThread.h"
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

    double pTMin = 0.3;
    double pTMax = 1.5;
    double sum_nBnF = 0.0;
    double sum_nF = 0.0;
    double sum_nB = 0.0;
    double nFSquared_sum = 0.0;
    int eventNumber = 1000;
    int nB = 0;
    int nF = 0;
    vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

    TH2F *b_corr_vs_width = new TH2F("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 10, 0, 1);
    TProfile *b_corr_profile = new TProfile("b_corr_profile", "b_corr Profile", window_width.size() - 2, &window_width[1]);

    // Event loop
    for (int iEvents = 0; iEvents < eventNumber; ++iEvents)
    {
        if (!pythia.next())
            continue;

        vector<double> fwdMult(window_width.size(), 0.0); // Array to store forward multiplicity within desired pseudorapidity window.
        vector<double> bkwdMult(window_width.size(), 0.0); // Array to store backward multiplicity within desired pseudorapidity window.
        vector<double> b_corr_values(window_width.size(), 0.0); // Array to store b_corr values.

        // Particle loop
        for (int i = 0; i < pythia.event.size(); ++i)
        {
            // Conditional statement
            if (!pythia.event[i].isCharged() || !pythia.event[i].isHadron() || !pythia.event[i].isFinal())
                continue;
            if (!(pythia.event[i].pT() > pTMin && pythia.event[i].pT() < pTMax))
                continue;

            // Choosing forward multiplicity within different pseudorapidity windows
            for (int j = 0; j < window_width.size() - 1; ++j)
            {
                if (pythia.event[i].eta() > window_width[j] && pythia.event[i].eta() <= window_width[j + 2])
                    ++fwdMult[j];
                else if (pythia.event[i].eta() < -window_width[j] && pythia.event[i].eta() >= -window_width[j + 1])
                    ++bkwdMult[j];
            }
        }

        // Calculate and store b_corr values for each window width
        for (int k = 0; k < window_width.size() - 1; ++k)
        {
            sum_nF = 0.0;
            sum_nB = 0.0;
            sum_nBnF = 0.0;
            nFSquared_sum = 0.0;

            for (int f = 0; f < window_width.size(); ++f)
            {
                sum_nF += fwdMult[f] / eventNumber;
            }

            for (int b = 0; b < window_width.size(); ++b)
            {
                sum_nB += bkwdMult[b] / eventNumber;
                sum_nBnF += (fwdMult[f] * bkwdMult[b]) / eventNumber;
                nFSquared_sum += (fwdMult[f] * fwdMult[f]) / eventNumber;
            }

            double b_corr = ((sum_nBnF - sum_nF * sum_nB) / (nFSquared_sum - sum_nF * sum_nF));
            double a = sum_nB - b_corr * sum_nF;

            if (std::isnan(b_corr) || std::isinf(b_corr))
                continue;

            b_corr_values[k] = b_corr;
        }

        // Fill the 2D histogram and profile
        for (int k = 0; k < window_width.size() - 2; ++k)
        {
            b_corr_vs_width->Fill(window_width[k], b_corr_values[k]);
            b_corr_profile->Fill(window_width[k], b_corr_values[k]);
        }
    }

    // Create a ROOT file and save the histogram and profile
    TFile outFile("output.root", "RECREATE");
    b_corr_vs_width->Write();
    b_corr_profile->Write();
    outFile.Close();

    return 0;
}
