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

    double b_corr = 0.0;
    double nF_sum = 0.0;
    double nFSquared_sum = 0.0;
    double nFnB_sum = 0.0;
    double nB_sum = 0.0;

    TH2F *nB_vs_nF = new TH2F("nB_vs_nF", "histogram", 50, 0, 30, 50, 0, 30);
    TH1F *hist = new TH1F("hist", "histogram", 50, 0, 30);
    TProfile *profile = new TProfile("profile", "nB vs nF", 20, 0, 30);

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

            hist->Fill(pythia.event[j].pT());
            if (pythia.event[j].eta() < etaCut)
                nF++;
            if (pythia.event[j].eta() > -etaCut)
                nB++;
        }

        nF_sum += nF;
        nB_sum += nB;
        nFnB_sum += nF * nB;
        nFSquared_sum += nF * nF;

        nB_vs_nF->Fill(nF, nB);

        b_corr = ((nFnB_sum - nF_sum * nB_sum) / (nFSquared_sum - (nF_sum) * (nF_sum)));
        double a = nF_sum - b_corr * (nF_sum);

        profile->Fill(nF, nB);
        
        // cout << "nF = " << nF << endl;
        // cout << "nB = " << nB << endl;
    }

    TCanvas canvas("canvas", "FB_Corr", 600, 600);
    canvas.cd();
    nB_vs_nF->Draw("COLZ");
    nB_vs_nF->GetXaxis()->SetTitle("n_F");
    nB_vs_nF->GetYaxis()->SetTitle("n_B");
    // nB_vs_nF->SetContour(1000);

    TFile file("FB_output.root", "recreate");
    nB_vs_nF->Write();
    hist->Write();
    profile->Write();
    file.Close();

    pythia.stat();
    return 0;
}
