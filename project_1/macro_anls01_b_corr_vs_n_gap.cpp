// void macro_anls01_b_corr_vs_n_gap()
// {
//     TChain *chain = new TChain("t"); // Update the TChain object to use the correct tree name

//     // Add the file(s) to the TChain
//     chain->Add("pythia8MB_CR1_RHoff_1.root");

//     // Set up the event branch in the TChain
//     Int_t nParticles;
//     Float_t pT[10000];
//     Float_t eta[10000];

//     chain->SetBranchAddress("nParticles", &nParticles);
//     chain->SetBranchAddress("pT", pT);
//     chain->SetBranchAddress("eta", eta);

//     // Set up variables for histogramming
//     double pTMin = 0.3;
//     double pTMax = 1.5;
//     Int_t eventNumber = chain->GetEntries();
//     std::vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
//     std::vector<double> fwdMult(window_width.size(), 0.0);
//     std::vector<double> bkwdMult(window_width.size(), 0.0);

//     // Create histograms and profiles
//     TH1F *hist = new TH1F("hist", "pT", 100, pTMin, pTMax);
//     TH1F *hist2 = new TH1F("hist2", "eta", 100, -3, 3);

//     TH2F *b_corr_vs_width = new TH2F("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 100, 0, 1.2);
//     TProfile *profile_b_corr = new TProfile("profile_b_corr", "b_corr vs Window Width", window_width.size() - 2, &window_width[1]);

//     // Event loop
//     for (Int_t iEvents = 0; iEvents < eventNumber; ++iEvents)
//     {
//         chain->GetEntry(iEvents); // Get the event for each entry in the TChain

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
//             hist->Fill(pT[i]);
//             hist2->Fill(eta[i]);

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

//             b_corr_vs_width->Fill(window_width[k], b_corr);
//             profile_b_corr->Fill(window_width[k], b_corr);
//         }
//     }

//     // Create a ROOT file and save the histograms
//     TFile *output = new TFile("output.root", "RECREATE");
//     hist->Write();
//     hist2->Write();
//     b_corr_vs_width->Write();
//     profile_b_corr->Write();
//     output->Close();

//     // Plot the histogram
//     TCanvas *canvas = new TCanvas("canvas", "b_corr vs Window Width", 800, 600);
//     gStyle->SetOptStat(0); // Disable statistics box
//     b_corr_vs_width->GetZaxis()->SetRangeUser(-1, 1); // Set z-axis range
//     b_corr_vs_width->Draw("colz");
//     canvas->SetLogz();
//     canvas->Print("b_corr_vs_width.png");
// }


void macro_anls01_b_corr_vs_n_gap()
{
    TChain *chain = new TChain(""); // Create a TChain object

    // Add the tuples to the TChain
    for (Int_t i = 999961; i <= 999999; ++i)
    {
        TString tupleName = Form("Event%d", i);
        chain->Add(Form("pythia8MB_CR1_RHoff_1.root/%s", tupleName.Data()));
    }

    // Set up variables for histogramming
    const double pTMin = 0.3;
    const double pTMax = 1.5;
    const std::vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
    const size_t num_windows = window_width.size();
    std::vector<double> fwdMult(num_windows - 1, 0.0);
    std::vector<double> bkwdMult(num_windows - 1, 0.0);

    // Create histograms and profiles
    TH1F *hist = new TH1F("hist", "pT", 100, pTMin, pTMax);
    TH1F *hist2 = new TH1F("hist2", "eta", 100, -3, 3);

    TH2F *b_corr_vs_width = new TH2F("b_corr_vs_width", "b_corr vs Window Width", num_windows - 2, &window_width[1], 100, 0, 1.2);
    TProfile *profile_b_corr = new TProfile("profile_b_corr", "b_corr vs Window Width", num_windows - 2, &window_width[1]);

    // Event loop
    Long64_t eventNumber = chain->GetEntries();
    for (Long64_t iEvent = 0; iEvent < eventNumber; ++iEvent)
    {
        chain->GetEntry(iEvent); // Get the event for each entry in the TChain

        // Reset forward and backward multiplicities for each event
        std::fill(fwdMult.begin(), fwdMult.end(), 0.0);
        std::fill(bkwdMult.begin(), bkwdMult.end(), 0.0);

        // Particle loop
        for (Int_t i = 0; i < chain->GetLeaf("nParticles")->GetValue(); ++i)
        {
            // Conditional statement
            if (chain->GetLeaf("pT")->GetValue(i) < pTMin || chain->GetLeaf("pT")->GetValue(i) > pTMax)
                continue;

            // Scatter plots of pT and eta
            hist->Fill(chain->GetLeaf("pT")->GetValue(i));
            hist2->Fill(chain->GetLeaf("eta")->GetValue(i));

            // Choosing forward multiplicity within different pseudorapidity windows
            for (Int_t j = 0; j < num_windows - 1; ++j)
            {
                if (chain->GetLeaf("eta")->GetValue(i) >= window_width[j] && chain->GetLeaf("eta")->GetValue(i) < window_width[j + 1])
                {
                    fwdMult[j]++;
                    break;
                }
            }

            // Choosing backward multiplicity within different pseudorapidity windows
            for (Int_t j = 0; j < num_windows - 1; ++j)
            {
                if (chain->GetLeaf("eta")->GetValue(i) <= -window_width[j] && chain->GetLeaf("eta")->GetValue(i) > -window_width[j + 1])
                {
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
        for (Int_t k = 0; k < num_windows - 2; ++k)
        {
            sum_nF_k += fwdMult[k];
            nFSquared_sum_k += fwdMult[k] * fwdMult[k];

            sum_nB_k = 0.0;
            sum_nBnF_k = 0.0;

            for (Int_t b = k + 1; b < num_windows - 1; ++b)
            {
                sum_nB_k += bkwdMult[b];
                sum_nBnF_k += fwdMult[k] * bkwdMult[b];
            }

            Double_t b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);

            if (std::isnan(b_corr) || std::isinf(b_corr))
                continue;

            b_corr_vs_width->Fill(window_width[k], b_corr);
            profile_b_corr->Fill(window_width[k], b_corr);
        }
    }

    // Create a ROOT file and save the histograms
    TFile *output = new TFile("output.root", "RECREATE");
    hist->Write();
    hist2->Write();
    b_corr_vs_width->Write();
    profile_b_corr->Write();
    output->Close();

    // Plot the histogram
    TCanvas *canvas = new TCanvas("canvas", "b_corr vs Window Width", 800, 600);
    gStyle->SetOptStat(0); // Disable statistics box
    b_corr_vs_width->GetZaxis()->SetRangeUser(-1, 1); // Set z-axis range
    b_corr_vs_width->Draw("colz");
    canvas->SetLogz();
    canvas->Print("b_corr_vs_width.png");

    delete canvas;
    delete output;
    delete profile_b_corr;
    delete b_corr_vs_width;
    delete hist2;
    delete hist;
    delete chain;
}
