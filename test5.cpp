// #include <iostream>
// // #include <vector>
// // #include "Pythia8/Pythia.h"
// #include "TH1F.h"
// #include "TCanvas.h"
// #include "Pythia8/Pythia.h"

// int main() {
//     Pythia8::Pythia pythia;

//     // Set up the collision process
//     pythia.readString("Beams:eCM = 13000.");  // Set center-of-mass energy to 13 TeV
//     pythia.readString("HardQCD:all = on");  // Enable hard QCD processes
//     pythia.init();

//     const int nEvents = 100;
//     TH1F *histogram = new TH1F("histogram", "Transverse Momentum Distribution", 50, 0, 100);

//     // Generate events
//     for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
//         if (!pythia.next()) continue;

//         // Loop over final-state particles
//         for (int i = 0; i < pythia.event.size(); ++i) {
//             const Pythia8::Particle &particle = pythia.event[i];
//             if (particle.isFinal()) {
//                 double pT = particle.pT();  // Transverse momentum
//                 histogram->Fill(pT);  // Fill the histogram
//             }
//         }

//         pythia.event.reset();
//     }

//     // Plot the histogram
//     TCanvas *canvas = new TCanvas("canvas", "Canvas", 800, 600);
//     histogram->Draw();
//     canvas->Print("transverse_momentum.png");

//     // Finalize Pythia
//     pythia.stat();

//     // Cleanup
//     delete histogram;
//     delete canvas;

//     return 0;
// }

// **********************************************************

// #include <iostream>
// #include "Pythia8/Pythia.h"
// #include "TH1F.h"
// #include "TCanvas.h"

// int main() {
//     Pythia8::Pythia pythia;

//     // Set up the collision process
//     pythia.readString("Beams:eCM = 13000.");  // Set center-of-mass energy to 13 TeV
//     pythia.readString("HardQCD:all = on");  // Enable hard QCD processes
//     pythia.init();

//     const int nEvents = 10000;
//     TH1F *histogram = new TH1F("histogram", "Transverse Momentum Distribution", 50, 0, 100);

//     // Generate events
//     for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
//         if (!pythia.next()) continue;

//         // Loop over final-state particles
//         for (int i = 0; i < pythia.event.size(); ++i) {
//             const Pythia8::Particle &particle = pythia.event[i];
//             if (particle.isFinal()) {
//                 double pT = particle.pT();  // Transverse momentum
//                 histogram->Fill(pT);  // Fill the histogram
//             }
//         }

//         pythia.event.reset();
//     }

//     // Plot the histogram
//     TCanvas *canvas = new TCanvas("canvas", "Canvas", 800, 600);
//     histogram->Draw();
//     canvas->Print("transverse_momentum.png");

//     // Finalize Pythia
//     pythia.stat();

//     // Cleanup
//     // delete histogram;
//     // delete canvas;

//     return 0;
// }

//*********************************************************

#include <iostream>
#include "Pythia8/Pythia.h"
#include "TH1F.h"
#include "TCanvas.h"

int main() {
    Pythia8::Pythia pythia;

    // Set up the collision process
    pythia.readString("Beams:eCM = 13000.");  // Set center-of-mass energy to 13 TeV
    pythia.readString("SoftQCD:all = on");  // Enable hard QCD processes
    pythia.init();

    const int nEvents = 10000;
    TH1F *histogram = new TH1F("histogram", "Transverse Momentum Distribution", 100, 0, 10);

    // Generate events
    for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
        if (!pythia.next()) continue;

        // Loop over final-state particles
        for (int i = 0; i < pythia.event.size(); ++i) {
            const Pythia8::Particle &particle = pythia.event[i];
            if (particle.isFinal() && particle.id() != 2212) {  //  (ID = 2212)
                double pT = particle.pT();  // Transverse momentum
                histogram->Fill(pT);  // Fill the histogram
            }
        }
    }

    // Plot the histogram
    TCanvas *canvas = new TCanvas("canvas", "Canvas", 800, 600);
    histogram->Draw();
    canvas->Print("transverse_momentum_2.png");

    // Finalize Pythia
    pythia.stat();
    pythia.info.sigmaGen();
    pythia.info.sigmaErr();

    // Cleanup
    delete histogram;
    delete canvas;

    return 0;
}
