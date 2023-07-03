// /* ***************************************************************************************** */
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

//     TH2F b_corr_vs_width("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 100, 0, 1.2);
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

//             // Scatter plots of pT and eta
//             hist.Fill(pythia.event[i].pT());
//             hist2.Fill(pythia.event[i].eta());

//             // Choosing forward multiplicity within different pseudorapidity windows
//             for (int j = 0; j < window_width.size() - 1; ++j) {
//                 if ((pythia.event[i].eta()) >= window_width[j] && (pythia.event[i].eta()) < window_width[j + 1]) {
//                     fwdMult[j]++;
//                     break;

//                 }
//             }
//             // Choosing backward multiplicity within different pseudorapidity windows
//             for (int j = 0; j < window_width.size() - 1; ++j) {
//                 if ((pythia.event[i].eta()) <= -window_width[j] && (pythia.event[i].eta()) > -window_width[j + 1]) {
//                     bkwdMult[j]++;
//                     break;
//                 }
//             }

//             double sum_nF_k = 0.0;
//             double sum_nB_k = 0.0;
//             double sum_nBnF_k = 0.0;
//             double nFSquared_sum_k = 0.0;

//             // Calculate and store values for each window width
//             for (int k = 0; k < window_width.size() - 2; ++k) {
//             sum_nF_k += fwdMult[k];
//             nFSquared_sum_k += fwdMult[k] * fwdMult[k];

//             sum_nB_k = 0.0;
//             sum_nBnF_k = 0.0;

//             for (int b = k + 1; b < window_width.size() - 1; ++b) {
//                 sum_nB_k += bkwdMult[b];
//                 sum_nBnF_k += fwdMult[k] * bkwdMult[b];
//             }

//             double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);

//             if (std::isnan(b_corr) || std::isinf(b_corr))
//                 continue;

//             b_corr_vs_width.Fill(window_width[k], b_corr);
//             profile_b_corr.Fill(window_width[k], b_corr);
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


/** * ************************************************************** * **/


/* This section of the code will access data from a .root file in local directory and use the events stored in it its tuple
to iterate over the particle loop and plot the histogram of b_corr versus n_gap using the code written previosuly*/

// #include <iostream>
// #include <vector>
// #include "Pythia8/Pythia.h"
// #include "TH2F.h"
// #include "TH1F.h"
// #include "TProfile.h"
// #include "TGraph.h"
// #include "TCanvas.h"
// #include "TStyle.h"

// #include "TFile.h"
// #include "TTree.h"
// #include "TChain.h"

// // Open the root file
// TFile* file = new TFile("CRoff_RHoff_test2_SR", "READ");

// // Get the event tree or chain
// TTree* tree = (TTree*)file->Get("hist_plot");

// // Create variables to hold the tuple data
// Double_t bkwdMult, fwdMult;

// // Set the branch addresses for the variables
// tree->SetBranchAddress("branch_1", &variable1);
// tree->SetBranchAddress("branch_2", &variable2);

// // Create histograms
// TH2D* histogram = new TH2D("histogram", "b_corr vs n_gap", nbinsx, xmin, xmax, nbinsy, ymin, ymax);

// // Loop over the tuples
// for (Long64_t iEvent = 0; iEvent < tree->GetEntries(); ++iEvent) {
//     // Load the event tuple
//     tree->GetEntry(iEvent);
    
//     // Perform your particle loop and histogram filling here
//     // Access the variables variable1 and variable2 to analyze the event
    
//     // Example: Fill the histogram
//     histogram->Fill(variable1, variable2);
// }

// // Plot and save the histogram
// TCanvas* canvas = new TCanvas("canvas", "2D Histogram Canvas");
// histogram->Draw("COLZ");
// canvas->SaveAs("histogram.png");
// canvas->Draw();



/** * ************************************************************** * **/



#include <iostream>
#include <vector>
#include "TFile.h"
#include "TChain.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TStyle.h"

using namespace std;

int main()
{
    TChain chain("tree"); // Create a TChain object to chain multiple ROOT files

    // Add the file(s) to the TChain
    chain.Add("/qnap2/R2P2pythia8MB/pp13TeV/CR1_RHoff/pythia8MB_CR1_RHoff_100.root");
    // chain.Add("/sa/ssaa/haaha/file2.root");
    // Add more files if necessary

    // Create a Pythia event object
    Pythia8::Event event;

    // Set up the event branch in the TChain
    chain.SetBranchAddress("eventBranchName", &event);

    // Set up other branches if needed

    // Set up variables for histogramming
    double pTMin = 0.3;
    double pTMax = 1.5;
    int eventNumber = chain.GetEntries();
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
        chain.GetEntry(iEvents); // Get the event for each entry in the TChain

        // Reset forward and backward multiplicities for each event
        fill(fwdMult.begin(), fwdMult.end(), 0.0);
        fill(bkwdMult.begin(), bkwdMult.end(), 0.0);

        // Particle loop
        for (int i = 0; i < event.size(); ++i)
        {
            // Conditional statement
            if (!event[i].isCharged() || !event[i].isFinal())
                continue;
            if (event[i].pT() < pTMin || event[i].pT() > pTMax)
                continue;

            // Scatter plots of pT and eta
            hist.Fill(event[i].pT());
            hist2.Fill(event[i].eta());

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
        }
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

            for (int b = k + 1; b < window_width.size() - 1; ++b) 
            {
                sum_nB_k += bkwdMult[b];
                sum_nBnF_k += fwdMult[k] * bkwdMult[b];
            }

            double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);

            if (std::isnan(b_corr) || std::isinf(b_corr))
                continue;

            b_corr_vs_width.Fill(window_width[k], b_corr);
            profile_b_corr.Fill(window_width[k], b_corr);
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


/* *****************************************************************************************************************************8 */

/*HERE IS STORED THE ROOT MACRO TO BE EXECUTED IN THE CLUSTER TERMINAL*/




// int main()
// {
//     TChain chain("tree"); // Create a TChain object to chain multiple ROOT files

//     // Add the file(s) to the TChain
//     chain.Add("/qnap2/R2P2pythia8MB/pp13TeV/CR1_RHoff/pythia8MB_CR1_RHoff_100.root");
//     // chain.Add("/sa/ssaa/haaha/file2.root");
//     // Add more files if necessary

//     // Create a Pythia event object
//     TTree *eventTree = dynamic_cast<TTree*>(chain.GetFile()->Get("eventTree"));

//     // Set up the event branch in the TChain
//     Int_t nParticles;
//     Float_t pT[10000];
//     Float_t eta[10000];

//     eventTree->SetBranchAddress("nParticles", &nParticles);
//     eventTree->SetBranchAddress("pT", pT);
//     eventTree->SetBranchAddress("eta", eta);

//     // Set up variables for histogramming
//     double pTMin = 0.3;
//     double pTMax = 1.5;
//     Int_t eventNumber = chain.GetEntries();
//     std::vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
//     std::vector<double> fwdMult(window_width.size(), 0.0);
//     std::vector<double> bkwdMult(window_width.size(), 0.0);

//     // Create histograms and profiles
//     TH1F hist("hist", "pT", 100, pTMin, pTMax);
//     TH1F hist2("hist2", "eta", 100, -3, 3);

//     TH2F b_corr_vs_width("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 100, 0, 1.2);
//     TProfile profile_b_corr("profile_b_corr", "b_corr vs Window Width", window_width.size() - 2, &window_width[1]);

//     // Event loop
//     for (Int_t iEvents = 0; iEvents < eventNumber; ++iEvents)
//     {
//         eventTree->GetEntry(iEvents); // Get the event for each entry in the TChain

//         // Reset forward and backward multiplicities for each event
//         fill(fwdMult.begin(), fwdMult.end(), 0.0);
//         fill(bkwdMult.begin(), bkwdMult.end(), 0.0);

//         // Particle loop
//         for (Int_t i = 0; i < nParticles; ++i)
//         {
//             // Conditional statement
//             if (pT[i] < pTMin || pT[i] > pTMax)
//                 continue;

//             // Scatter plots of pT and eta
//             hist.Fill(pT[i]);
//             hist2.Fill(eta[i]);

//             // Choosing forward multiplicity within different pseudorapidity windows
//             for (Int_t j = 0; j < window_width.size() - 1; ++j) {
//                 if (eta[i] >= window_width[j] && eta[i] < window_width[j + 1]) {
//                     fwdMult[j]++;
//                     break;
//                 }
//             }

//             // Choosing backward multiplicity within different pseudorapidity windows
//             for (Int_t j = 0; j < window_width.size() - 1; ++j) {
//                 if (eta[i] <= -window_width[j] && eta[i] > -window_width[j + 1]) {
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
//         for (Int_t k = 0; k < window_width.size() - 2; ++k) {
//             sum_nF_k += fwdMult[k];
//             nFSquared_sum_k += fwdMult[k] * fwdMult[k];

//             sum_nB_k = 0.0;
//             sum_nBnF_k = 0.0;

//             for (Int_t b = k + 1; b < window_width.size() - 1; ++b) {
//                 sum_nB_k += bkwdMult[b];
//                 sum_nBnF_k += fwdMult[k] * bkwdMult[b];
//             }

//             Double_t b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);

//             if (std::isnan(b_corr) || std::isinf(b_corr))
//                 continue;

//             b_corr_vs_width.Fill(window_width[k], b_corr);
//             profile_b_corr.Fill(window_width[k], b_corr);
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
