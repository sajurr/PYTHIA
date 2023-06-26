#include <iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TThread.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TProfile.h"
#include <vector>

using namespace std;

double calculatePseudorapidity(Pythia8::Particle particle) {
    double theta = particle.p().theta();
    return -log(tan(theta / 2.0));
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
    double n_window = 0.2;

    double b_corr = 0.0;

    vector<double> pseudorapidity_gaps;
    vector<double> b_corr_values;

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
            if (pythia.event[j].pT() < pTMin || pythia.event[j].pT() > pTMax)
                continue;

            if (pythia.event[j].eta() < etaCut + n_window)
                nF++;
            if (pythia.event[j].eta() > -etaCut - n_window)
                nB++;
        }

        double n_gap = calculatePseudorapidity(pythia.event[nF]) - calculatePseudorapidity(pythia.event[nB]);
        b_corr = ((nF * nB) - (nF + nB)) / (nF + nB);
        double a = nF - b_corr * (nF + nB);

        pseudorapidity_gaps.push_back(n_gap);
        b_corr_values.push_back(b_corr);
    }

    TCanvas canvas("canvas", "b_corr_vs_gap", 600, 600);
    TH2F *b_corr_vs_gap = new TH2F("b_corr_vs_gap", "b_corr vs Pseudorapidity Gap", 10, 0, 1.2, 10, 0, 0.6);
    for (size_t i = 0; i < pseudorapidity_gaps.size(); ++i)
    {
        b_corr_vs_gap->Fill(pseudorapidity_gaps[i], b_corr_values[i]);
    }
    canvas.cd();
    b_corr_vs_gap->Draw("COLZ");
    b_corr_vs_gap->GetXaxis()->SetTitle("Pseudorapidity Gap");
    b_corr_vs_gap->GetYaxis()->SetTitle("b_corr");

    TFile file("b_corr_output.root", "recreate");
    b_corr_vs_gap->Write();
    file.Close();

    pythia.stat();
    return 0;
}
