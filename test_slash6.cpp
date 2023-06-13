#include <iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TThread.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TProfile.h"

using namespace std;

double calculatePseudorapidity(Pythia8::Particle particle) {
    double theta = particle.p().theta();
    return -log(tan(theta/2.0));
}

int main()
{
    Pythia8::Pythia pythia;
    pythia.readString("Beams:eCM = 7000");
    pythia.readString("SoftQCD:all = on");
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.init();

    double etaCut = 0.8; // Pseudorapidity cut for forwards-backward multiplicity correlation
    double pTMin = 0.3;
    double pTMax = 1.5;
    double delta_eta = 0.2;

    double b_corr = 0.0;
    double nF_sum = 0.0;
    double nFSquared_sum = 0.0;
    double nFnB_sum = 0.0;
    double nB_sum = 0.0;

    TH2F *b_corr_vs_gap = new TH2F("b_corr_vs_gap", "b_corr vs Pseudorapidity Gap", 10, -50, 40, 10, 0, 5);
    TH1F *hist = new TH1F("hist", "pT", 50, 0, 30);
    TH1F *hist2 = new TH1F("hist2", "eta", 50, 0, 30);
    TProfile *profile2 = new TProfile("profile2", "b_corr vs n_gap", 20, 0, 1.2);

    for (int i = 0; i < 10000; ++i)
    {
        if (!pythia.next())
            continue;

        int nF = 0;
        int nB = 0;

        for (int j = 0; j < pythia.event.size(); ++j)
        {
            if (!pythia.event[j].isCharged() || !pythia.event[j].isFinal())
                continue;
            if (pythia.event[j].pT() < pTMin || pythia.event[j].pT() > pTMax || fabs(pythia.event[j].eta()) > 0.8)
                continue;

            hist->Fill(pythia.event[j].pT());
            hist2->Fill(pythia.event[j].eta());
            if (pythia.event[j].eta() > 0) //forward rapidity region; apply condition on greater than and less than 0.
                nF++;
            if (pythia.event[j].eta() < 0) // backward
                nB++;
        }

        nF_sum += nF;
        nB_sum += nB;
        nFnB_sum += nF * nB;
        nFSquared_sum += nF * nF;

        b_corr = ((nFnB_sum - nF_sum * nB_sum) / (nFSquared_sum - (nF_sum) * (nF_sum)));
        double n_gap = calculatePseudorapidity(pythia.event[nF]) - calculatePseudorapidity(pythia.event[nB]);
        // double a = nF_sum - b_corr * (nF_sum);
        


        if (std::isnan(n_gap) || std::isinf(n_gap) || std::isnan(b_corr) || std::isinf(b_corr))
            continue;

        cout << "b_corr = " << b_corr << endl;
        cout << "n_gap = " << n_gap << endl;
        // cout << "eta = " << eta << endl;

        b_corr_vs_gap->Fill(n_gap, b_corr);
        profile2->Fill(n_gap, b_corr);
    }

    TCanvas canvas("canvas", "b_corr_vs_gap", 600, 600);
    canvas.cd();
    b_corr_vs_gap->Draw("COLZ");
    b_corr_vs_gap->GetXaxis()->SetTitle("Pseudorapidity Gap");
    b_corr_vs_gap->GetYaxis()->SetTitle("b_corr");

    TFile file("b_corr_output.root", "recreate");
    b_corr_vs_gap->Write();
    hist->Write();
    hist2->Write();
    profile2->Write();
    file.Close();

    pythia.stat();
    return 0;
}
