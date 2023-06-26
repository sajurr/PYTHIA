// #include <iostream>
// #include "Pythia8/Pythia.h"
// #include "TFile.h"
// #include "TThread.h"
// #include "TH2F.h"
// #include "TH1F.h"
// #include "TCanvas.h"
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
//     double sum_nBnF = 0.0;
//     double sum_nF = 0.0;
//     double sum_nB = 0.0;
//     double nFSquared_sum = 0.0;
//     int eventNumber = 10000;
//     int nB = 0;
//     int nF = 0;
//     vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
//     vector<double> fwdMult(window_width.size(), 0.0); // Array to store forward multiplicity within desired pseudorapidity window.
//     vector<double> bkwdMult(window_width.size(), 0.0); // Array to store backward multiplicity within desired pseudorapidity window.
//     vector<double> b_corr_values(window_width.size(), 0.0); // Array to store b_corr values.

//     TH2F *b_corr_vs_width = new TH2F("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 10, 0, 1);
//     TH1F *hist = new TH1F("hist", "pT", 10, 0, 3);
//     TH1F *hist2 = new TH1F("hist2", "eta", 10, -3, 3);

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

//         // Calculate and store b_corr values for each window width
//         for (int k = 0; k < window_width.size() - 1; ++k)
//         {
//             double sum_nF_k = 0.0;
//             double sum_nB_k = 0.0;
//             double sum_nBnF_k = 0.0;
//             double nFSquared_sum_k = 0.0;

//             // Update sum_nF_k, nFSquared_sum_k for each forward multiplicity
//             for (int f = 0; f <= k; ++f)
//             {
//                 sum_nF_k += fwdMult[f];
//                 nFSquared_sum_k += fwdMult[f] * fwdMult[f];
//             }

//             // Update sum_nB_k, sum_nBnF_k for each backward multiplicity
//             for (int b = 0; b <= window_width.size() - k - 2; ++b)
//             {
//                 sum_nB_k += bkwdMult[b];
//                 sum_nBnF_k += fwdMult[k] * bkwdMult[b];
//             }


//             // Calculate b_corr for the current window width
//             double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);
//             double a = sum_nB_k - b_corr * sum_nF_k;

//             if (std::isnan(b_corr) || std::isinf(b_corr))
//                 continue;

//             b_corr_values[k] = b_corr;
//         }

//         for (auto i = b_corr_values.begin(); i != b_corr_values.end(); ++i)
//         {
//             cout << *i << endl;
//         }

//         // Fill the 2D histogram
//         for (int k = 0; k < window_width.size() - 2; ++k)
//         {
//             b_corr_vs_width->Fill(window_width[k], b_corr_values[k]);
//         }
//     }

//     // Create a ROOT file and save the histograms
//     TFile outFile("output.root", "RECREATE");
//     hist->Write();
//     hist2->Write();
//     b_corr_vs_width->Write();
//     outFile.Close();

//     return 0;
// }

/* ****** The issue with the code above is that I do not get the bins properly filled at the desired window_widths.
 Maybe this issue can be rectified if I increase the number of events. But I am still speculating.****** */




// #include <iostream>
// #include "Pythia8/Pythia.h"
// #include "TFile.h"
// #include "TThread.h"
// #include "TH2F.h"
// #include "TH1F.h"
// #include "TCanvas.h"
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
//     double sum_nBnF = 0.0;
//     double sum_nF = 0.0;
//     double sum_nB = 0.0;
//     double nFSquared_sum = 0.0;
//     int eventNumber = 10000;
//     int nB = 0;
//     int nF = 0;
//     vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
//     vector<double> fwdMult(window_width.size(), 0.0); // Array to store forward multiplicity within desired pseudorapidity window.
//     vector<double> bkwdMult(window_width.size(), 0.0); // Array to store backward multiplicity within desired pseudorapidity window.
//     vector<vector<double>> b_corr_values(window_width.size() - 2, vector<double>(eventNumber, 0.0)); // 2D array to store b_corr values for each window width.

//     TH2F *b_corr_vs_width = new TH2F("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 10, 0, 1);
//     TH1F *hist = new TH1F("hist", "pT", 10, 0, 3);
//     TH1F *hist2 = new TH1F("hist2", "eta", 10, -3, 3);

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

//         // Calculate and store b_corr values for each window width
//         for (int k = 0; k < window_width.size() - 2; ++k)
//         {
//             double sum_nF_k = 0.0;
//             double sum_nB_k = 0.0;
//             double sum_nBnF_k = 0.0;
//             double nFSquared_sum_k = 0.0;

//             // Update sum_nF_k, nFSquared_sum_k for each forward multiplicity
//             for (int f = 0; f <= k; ++f)
//             {
//                 sum_nF_k += fwdMult[f];
//                 nFSquared_sum_k += fwdMult[f] * fwdMult[f];
//             }

//             // Update sum_nB_k, sum_nBnF_k for each backward multiplicity
//             for (int b = 0; b <= window_width.size() - k - 2; ++b)
//             {
//                 sum_nB_k += bkwdMult[b];
//                 sum_nBnF_k += fwdMult[k] * bkwdMult[b];
//             }

//             // Calculate b_corr for the current window width
//             double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);
//             double a = sum_nB_k - b_corr * sum_nF_k;

//             if (std::isnan(b_corr) || std::isinf(b_corr))
//                 continue;

//             b_corr_values[k][iEvents] = b_corr;
//         }

//         // Fill the 2D histogram
//         for (int k = 0; k < window_width.size() - 2; ++k)
//         {
//             b_corr_vs_width->Fill(window_width[k], b_corr_values[k][iEvents]);
//         }
//     }

//     // Create a ROOT file and save the histograms
//     TFile outFile("output.root", "RECREATE");
//     hist->Write();
//     hist2->Write();
//     b_corr_vs_width->Write();
//     outFile.Close();

//     return 0;
// }


/****************************************************************************************/

/* ****** Here, I am not getting the bins filled, rather a scatter plot of b_corr vs 
window_width. I have to update the loop in the next piece of code.****** */


// #include <iostream>
// #include "Pythia8/Pythia.h"
// #include "TFile.h"
// #include "TThread.h"
// #include "TH2F.h"
// #include "TH1F.h"
// #include "TCanvas.h"
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
//     double sum_nBnF = 0.0;
//     double sum_nF = 0.0;
//     double sum_nB = 0.0;
//     double nFSquared_sum = 0.0;
//     int eventNumber = 10000;
//     int nB = 0;
//     int nF = 0;
//     vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
//     vector<double> fwdMult(window_width.size(), 0.0); // Array to store forward multiplicity within desired pseudorapidity window.
//     vector<double> bkwdMult(window_width.size(), 0.0); // Array to store backward multiplicity within desired pseudorapidity window.
//     vector<vector<double>> b_corr_values(window_width.size() - 2, vector<double>(eventNumber, 0.0)); // 2D array to store b_corr values for each window width.

//     TH2F *b_corr_vs_width = new TH2F("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 10, 0, 1);
//     TH1F *hist = new TH1F("hist", "pT", 10, 0, 3);
//     TH1F *hist2 = new TH1F("hist2", "eta", 10, -3, 3);

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
//             if (pythia.event[i].pT() > pTMin && pythia.event[i].pT() < pTMax && pythia.event[i].pT() !== 0)
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

//         // Calculate and store b_corr values for each window width
//         for (int k = 0; k < window_width.size() - 2; ++k)
//         {
//             double sum_nF_k = 0.0;
//             double sum_nB_k = 0.0;
//             double sum_nBnF_k = 0.0;
//             double nFSquared_sum_k = 0.0;

//             // Update sum_nF_k, nFSquared_sum_k for each forward multiplicity
//             for (int f = 0; f <= k; ++f)
//             {
//                 sum_nF_k += fwdMult[f];
//                 nFSquared_sum_k += fwdMult[f] * fwdMult[f];
//             }

//             // Update sum_nB_k, sum_nBnF_k for each backward multiplicity
//             for (int b = 0; b <= window_width.size() - k - 2; ++b)
//             {
//                 sum_nB_k += bkwdMult[b];
//                 sum_nBnF_k += fwdMult[k] * bkwdMult[b];
//             }

//             // Calculate b_corr for the current window width
//             double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);
//             double a = sum_nB_k - b_corr * sum_nF_k;

//             if (std::isnan(b_corr) || std::isinf(b_corr))
//                 continue;

//             b_corr_values[k] = b_corr;
//         }

//         // Fill the 2D histogram
//         for (int k = 0; k < window_width.size() - 2; ++k)
//         {
//             b_corr_vs_width->Fill(window_width[k], b_corr_values[k]);
//         }
//     }

//     // Create a ROOT file and save the histograms
//     TFile outFile("output.root", "RECREATE");
//     hist->Write();
//     hist2->Write();
//     b_corr_vs_width->Write();
//     outFile.Close();

//     return 0;
// }




/******************************************************************************************/
/*TRIAL CODE*/ 

/*WORKED, but still need to try the method where I 
calculate b_corr outside the event loop*/
#include <iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TGraph.h"

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
    vector<double> b_corr_values(window_width.size() - 2, 0.0);

    TH2F *b_corr_vs_width = new TH2F("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 10, 0, 1);
    TH1F *hist = new TH1F("hist", "pT", 10, 0, 3);
    TH1F *hist2 = new TH1F("hist2", "eta", 10, -3, 3);
    TProfile *profile_b_corr = new TProfile("profile_b_corr", "b_corr vs Window Width", window_width.size() - 2, &window_width[1]);

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
            hist->Fill(pythia.event[i].pT());
            hist2->Fill(pythia.event[i].eta());

            // Choosing forward multiplicity within different pseudorapidity windows
            for (int j = 0; j < window_width.size() - 1; ++j)
            {
                if (pythia.event[i].eta() > window_width[j] && pythia.event[i].eta() <= window_width[j + 2])
                    ++fwdMult[j];
                else if (pythia.event[i].eta() < -window_width[j] && pythia.event[i].eta() >= -window_width[j + 2])
                    ++bkwdMult[j];
            }
        }

        // Declare loop indices outside the loops
        double sum_nF_k = 0.0;
        double sum_nB_k = 0.0;
        double sum_nBnF_k = 0.0;
        double nFSquared_sum_k = 0.0;

        // Calculate and store b_corr values for each window width
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

            // Calculate b_corr for the current window width
            double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);
            double a = sum_nB_k - b_corr * sum_nF_k;

            if (std::isnan(b_corr) || std::isinf(b_corr))
                continue;

            b_corr_values[k] = b_corr;
        }

        // Fill the 2D histogram
        for (int k = 0; k < window_width.size() - 2; ++k)
        {
            b_corr_vs_width->Fill(window_width[k + 1], b_corr_values[k]);
            profile_b_corr->Fill(window_width[k + 1], b_corr_values[k]);
        }
    }

    // Create a ROOT file and save the histograms
    TFile outFile("output.root", "RECREATE");
    hist->Write();
    hist2->Write();
    b_corr_vs_width->Write();
    profile_b_corr->Write();
    outFile.Close();

    return 0;
}
