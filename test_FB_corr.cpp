#include <iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TH1D.h"
#include "TBrowser.h"

int main() {
  Pythia8::Pythia pythia;
  pythia.readString("Beams:eCM = 13000.");  // Center-of-mass energy
  pythia.readString("HardQCD:all = on");   // Generate hard QCD processes
  pythia.init();

  // Define observables
  double etaCut = 2.5;  // Pseudorapidity cut for forward-backward correlation
  double ptMin = 1.0;   // Minimum transverse momentum

  // Create a histogram
  TH1D* hist = new TH1D("hist", "Forward-Backward Correlation", 100, -1.0, 1.0);

  // Event loop
  for (int iEvent = 0; iEvent < 10000; ++iEvent) {
    if (!pythia.next()) continue;  // Generate the event

    // Loop over particles in the event
    for (int i = 0; i < pythia.event.size(); ++i) {
      // Select particles within the specified pseudorapidity and transverse momentum range
      if (pythia.event[i].isFinal() && pythia.event[i].isVisible() &&
          fabs(pythia.event[i].eta()) < etaCut && pythia.event[i].pT() > ptMin) {
        // Fill the histogram with the forward-backward correlation observable
        double correlation = pythia.event[i].pz() / pythia.event[i].e();
        hist->Fill(correlation);
      }
    }
  }

  // Save the histogram in a ROOT file
  TFile file("output.root", "RECREATE");
  hist->Write();

  // Open the TBrowser to view the histogram
  TBrowser browser;

  // Clean up
  delete hist;
  pythia.stat();
  return 0;
}

