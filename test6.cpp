#include<iostream>
#include "Pythia8/Pythia.h"
#include "TH1F.h"
#include "TCanvas.h"

int main()
{
    Pythia8::Pythia pythia;

    //Setting up the collision process
    pythia.readString("Beams:eCM = 13e3");
    pythia.readString("HardQCD:all = on");
    pythia.readString("SoftQCD:all = on");
    pythia.init();

    const int nEvents = 10000;
    TH1F *histogram = new TH1F("histgram", "Transverse momentum distribution", 100, -50, 50);

    //Generating the events
    for(int iEvents = 0; iEvents < nEvents; ++iEvents)
    {
        if (!pythia.next()) continue;

        for(int i = 0; i < pythia.event.size(); ++i)
        {
            const Pythia8::Particle &particle = pythia.event[i];
            if(particle.isFinal() && particle.id() != 2212)
            {
                double pT = particle.pT(); //The transverse momentum
                histogram->Fill(pT);
            }
        }
    }
    
    //Plotting the histogram
    TCanvas *canvas = new TCanvas("canvas", "Canvas", 800, 800);
    histogram->Draw();
    canvas->Print("transverse_momentum.pdf");

    pythia.stat();

    return 0;
}