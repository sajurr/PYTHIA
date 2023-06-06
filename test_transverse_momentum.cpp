#include <iostream>
#include "Pythia8/Pythia.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

int main()
{
    TFile *pT_output = new TFile("pT_output.root", "recreate");
    TTree *tree = new TTree("tree", "tree");
    double pT;

    tree->Branch("pT", &pT, "pT/D");

    Pythia8::Pythia pythia;

    pythia.readString("Beams:eCM = 14.e3");
    pythia.readString("SoftQCD:all = on");
    pythia.init();

    const int nevents = 1.5e4;
    TH1F *histogram = new TH1F("histogram", "Transverese momentum distribution", 100, 0, 20);

    for(int ievents = 0; ievents < nevents; ++ievents)
    {
        if(!pythia.next()) continue;

        for(int i = 0; i < pythia.event.size(); ++i)
        {
            const Pythia8::Particle &particle = pythia.event[i];
            if(particle.isFinal() && particle.id() !=2212)
            {
                pT = particle.pT();
                histogram->Fill(pT);

                tree->Fill();
            }
        }
    }

    pT_output->Write();
    pT_output->Close();

    TCanvas *c1 = new TCanvas("c1", "Canvas", 600, 600);
    histogram->Draw();
    c1->Print("pT.png");

    pythia.stat();

    delete histogram;
    delete c1;

    return 0;

}