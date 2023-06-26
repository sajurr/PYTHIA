
// /*WORKD, but still need to try the method where I 
// calculate b_corr outside the event loop*/
// #include <iostream>
// #include "Pythia8/Pythia.h"
// #include "TFile.h"
// #include "TH2F.h"
// #include "TH1F.h"
// #include "TProfile.h"
// #include "TGraph.h"

// using namespace std;

// int main()
// {
//     Pythia8::Pythia pythia;
//     pythia.readString("Beams:idA = 2212");
//     pythia.readString("Beams:idB = 2212");
//     pythia.readString("SoftQCD:all = on");
//     pythia.readString("Beams:eCM = 900");
//     pythia.init();

//     double pTMin = 0.3;
//     double pTMax = 1.5;
//     int eventNumber = 10000;
//     vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
//     vector<double> fwdMult(window_width.size(), 0.0);
//     vector<double> bkwdMult(window_width.size(), 0.0);
//     vector<double> b_corr_values(window_width.size() - 2, 0.0);
//     vector<double> sum_nB_k_vec(0.0);

//     TH2F *b_corr_vs_width = new TH2F("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 10, 0, 1);
//     TH1F *hist = new TH1F("hist", "pT", 10, 0, 3);
//     TH1F *hist2 = new TH1F("hist2", "eta", 10, -3, 3);
//     TProfile *profile_b_corr = new TProfile("profile_b_corr", "b_corr vs Window Width", window_width.size() - 2, &window_width[1]);

//     // Event loop
//     for (int iEvents = 0; iEvents < eventNumber; ++iEvents)
//     {
//         if (!pythia.next())
//             continue;

//         // Reset forward and backward multiplicities for each event
//         fill(fwdMult.begin(), fwdMult.end(), 0.0);
//         fill(bkwdMult.begin(), bkwdMult.end(), 0.0);

//         // Particle loop
//         for (int i = 0; i < pythia.event.size(); ++i)
//         {
//             // Conditional statement
//             if (!pythia.event[i].isCharged() || !pythia.event[i].isFinal())
//                 continue;
//             if (pythia.event[i].pT() < pTMin && pythia.event[i].pT() > pTMax)
//                 continue;

//             // Scatter plots of pT and eta
//             hist->Fill(pythia.event[i].pT());
//             hist2->Fill(pythia.event[i].eta());

//             // Choosing forward multiplicity within different pseudorapidity windows
//             for (int j = 0; j < window_width.size() - 1; ++j)
//             {
//                 if (pythia.event[i].eta() > window_width[j] && pythia.event[i].eta() <= window_width[j + 2])
//                     ++fwdMult[j];
//                 else if (pythia.event[i].eta() < -window_width[j] && pythia.event[i].eta() >= -window_width[j + 2])
//                     ++bkwdMult[j];
//             }
//         }

//         // Declare loop indices outside the loops
//         double sum_nF_k = 0.0;
//         double sum_nB_k = 0.0;
//         double sum_nBnF_k = 0.0;
//         double nFSquared_sum_k = 0.0;

//         // Calculate and store b_corr values for each window width
//         for (int k = 0; k < window_width.size() - 2; ++k)
//         {
//             sum_nF_k += fwdMult[k];
//             nFSquared_sum_k += fwdMult[k] * fwdMult[k];

//             sum_nB_k = 0.0;
//             sum_nBnF_k = 0.0;

//             for (int b = 0; b <= window_width.size() - k - 2; ++b)
//             {
//                 sum_nB_k += bkwdMult[b];
//                 sum_nBnF_k += fwdMult[k] * bkwdMult[b];
//                 // sum_nB_k.push_back(sum_nB_k_vec);
//             }
    

//             // Calculate b_corr for the current window width
//             double b_corr = ((sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k));
//             double a = sum_nB_k - b_corr * sum_nF_k;

//             if (std::isnan(b_corr) || std::isinf(b_corr))
//                 continue;

//             b_corr_values[k] = b_corr;
//         }

//         // Fill the 2D histogram
//         for (int k = 0; k < window_width.size() - 2; ++k)
//         {
//             b_corr_vs_width->Fill(window_width[k + 1], b_corr_values[k]);
//             profile_b_corr->Fill(window_width[k + 1], b_corr_values[k]);
//         }
//     }

//     // Create a ROOT file and save the histograms
//     TFile outFile("output.root", "RECREATE");
//     hist->Write();
//     hist2->Write();
//     b_corr_vs_width->Write();
//     profile_b_corr->Write();
//     outFile.Close();

//     return 0;
// }

/* ***************************************************************************** */
//THIS CODE WORKS, BUT I NEED TO NORMALIZE IT and MAKE SOME NECESSARY ADJUSTMENTS TO THE LOOP. 
//Probably re-write it.


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
    pythia.readString("Beams:eCM = 900");
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
            for (int j = 0; j < window_width.size() - 1; ++j)
            {
                if (abs(pythia.event[i].eta()) < window_width[j + 1])
                {
                    fwdMult[j]++;
                    break;
                }
            }

            // Choosing backward multiplicity within different pseudorapidity windows
            for (int j = 0; j < window_width.size() - 1; ++j)
            {
                if (abs(pythia.event[i].eta()) > 2.8 - window_width[j + 1])
                {
                    bkwdMult[j]++;
                    break;
                }
            }
        }


        // Print the entries of fwdMult vector
        // for (int j = 0; j < fwdMult.size(); ++j) {
        //     cout << "fwdMult[" << j << "] = " << fwdMult[j] << endl;
        // }

        double sum_nF_k = 0.0;
        double sum_nB_k = 0.0;
        double sum_nBnF_k = 0.0;
        double nFSquared_sum_k = 0.0;

        // Calculate and store values for each window width
        for (int k = 0; k < window_width.size() - 2; ++k)
        {
            sum_nF_k += fwdMult[k];
            nFSquared_sum_k += fwdMult[k] * fwdMult[k];

            sum_nB_k = 0.0;
            sum_nBnF_k = 0.0;

            for (int b = 0; b <= window_width.size() - k - 2; ++b)
            {
                sum_nB_k += bkwdMult[b];
                sum_nBnF_k += fwdMult[k] * bkwdMult[b];
            }

            double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);

            if (std::isnan(b_corr) || std::isinf(b_corr))
                continue;

            b_corr_vs_width.Fill(window_width[k + 1], b_corr);
            profile_b_corr.Fill(window_width[k + 1], b_corr);
        }
    }


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


/* ***************************************************************************** */


// #include <iostream>
// #include <vector>
// #include "Pythia8/Pythia.h"
// #include "TFile.h"
// #include "TH2F.h"
// #include "TH1F.h"
// #include "TProfile.h"
// #include "TGraph.h"
// #include "TCanvas.h"
// #include "TStyle.h"

// using namespace std;

// int main()
// {
//     Pythia8::Pythia pythia;
//     pythia.readString("Beams:idA = 2212");
//     pythia.readString("Beams:idB = 2212");
//     pythia.readString("SoftQCD:all = on");
//     pythia.readString("Beams:eCM = 900");
//     pythia.init();

//     double pTMin = 0.3;
//     double pTMax = 1.5;
//     int eventNumber = 10000;
//     vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
//     vector<double> fwdMult(window_width.size(), 0.0);
//     vector<double> bkwdMult(window_width.size(), 0.0);

//     // Create histograms and profiles
//     TH1F hist("hist", "pT", 100, pTMin, pTMax);
//     TH1F hist2("hist2", "eta", 100, -3, 3);

//     TH2F b_corr_vs_width("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 100, 0, 1);
//     TProfile profile_b_corr("profile_b_corr", "b_corr vs Window Width", window_width.size() - 2, &window_width[1]);

//     // Event loop
//     for (int iEvents = 0; iEvents < eventNumber; ++iEvents)
//     {
//         if (!pythia.next())
//             continue;

//         // Reset forward and backward multiplicities for each event
//         fill(fwdMult.begin(), fwdMult.end(), 0.0);
//         fill(bkwdMult.begin(), bkwdMult.end(), 0.0);

//         // Particle loop
//         for (int i = 0; i < pythia.event.size(); ++i)
//         {
//             // Conditional statement
//             if (!pythia.event[i].isCharged() || !pythia.event[i].isFinal())
//                 continue;
//             if (pythia.event[i].pT() < pTMin || pythia.event[i].pT() > pTMax)
//                 continue;
//             if (abs(pythia.event[i].eta()) > 0.8) // Modified pseudorapidity selection
//                 continue;

//             // Scatter plots of pT and eta
//             hist.Fill(pythia.event[i].pT());
//             hist2.Fill(pythia.event[i].eta());

//             // Choosing forward multiplicity within different pseudorapidity windows
//             for (int j = 0; j < window_width.size() - 1; ++j)
//             {
//                 if (abs(pythia.event[i].eta()) < window_width[j + 1])
//                 {
//                     fwdMult[j]++;
//                     break;
//                 }
//             }

//             // Choosing backward multiplicity within different pseudorapidity windows
//             for (int j = 0; j < window_width.size() - 1; ++j)
//             {
//                 if (abs(pythia.event[i].eta()) > 2.8 - window_width[j + 1])
//                 {
//                     bkwdMult[j]++;
//                     break;
//                 }
//             }
//         }

//         double sum_nF_k = 0.0;
//         double sum_nB_k = 0.0;
//         double sum_nBnF_k = 0.0;
//         double nFSquared_sum_k = 0.0;

//         // Calculate and store values for each window width
//         for (int k = 0; k < window_width.size() - 2; ++k)
//         {
//             sum_nF_k += fwdMult[k];
//             nFSquared_sum_k += fwdMult[k] * fwdMult[k];

//             sum_nB_k = 0.0;
//             sum_nBnF_k = 0.0;

//             for (int b = 0; b <= window_width.size() - k - 2; ++b)
//             {
//                 sum_nB_k += bkwdMult[b];
//                 sum_nBnF_k += fwdMult[k] * bkwdMult[b];
//             }

//             double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);

//             if (std::isnan(b_corr) || std::isinf(b_corr))
//                 continue;

//             b_corr_vs_width.Fill(window_width[k + 1], b_corr);
//             profile_b_corr.Fill(window_width[k + 1], b_corr);
//         }
//     }

//     // Create a ROOT file and save the histograms
//     TFile outFile("output.root", "RECREATE");
//     hist.Write();
//     hist2.Write();
//     b_corr_vs_width.Write();
//     profile_b_corr.Write();
//     outFile.Close();

//     // Plot the histogram
//     TCanvas canvas("canvas", "b_corr vs Window Width", 800, 600);
//     gStyle->SetOptStat(0); // Disable statistics box
//     b_corr_vs_width.GetZaxis()->SetRangeUser(-1, 1); // Set z-axis range
//     b_corr_vs_width.Draw("colz");
//     canvas.SetLogz();
//     canvas.Print("b_corr_vs_width.png");

//     return 0;
// }


