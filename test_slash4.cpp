#include <iostream>
#include "Pythia8/Pythia.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"

int main() {
  // Create a Pythia instance
  Pythia8::Pythia pythia;

  // Set up the collision process and energy
  pythia.readString("SoftQCD:all = on");
  pythia.readString("Beams:eCM = 900.0");

  // Initialize Pythia
  pythia.init();

  // Define histogram variables
  const double ptMin = 0.3;   // Minimum transverse momentum (GeV)
  const double ptMax = 1.5;   // Maximum transverse momentum (GeV)
  const double dn = 0.2;      // Pseudorapidity gap width
  const int nbins = 10;       // Number of bins for the histogram
  double b_corr = 0.0;
  double nF_sum = 0.0;
  double nFSquared_sum = 0.0;
  double nBnF_sum = 0.0;
  double nB_sum = 0.0;
  int nF = 0.0;
  int nB = 0.0;

  // Create a 2D histogram for pseudorapidity gap vs b_corr
  TH2F* h2 = new TH2F("h2", "Pseudorapidity Gap vs b_corr", nbins, 0, dn, nbins, -1, 1);

  // Event loop
  int nEvents = 10000;  // Number of events to generate
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    // Generate a new event
    pythia.next();

    // Loop over the particles in the event
    for (int i = 0; i < pythia.event.size(); ++i) {
      // Get the particle and its properties
      const Pythia8::Particle& particle = pythia.event[i];

      // Check if the particle satisfies the transverse momentum requirement
      if (particle.pT() > ptMin && particle.pT() < ptMax) {
        // Calculate b_corr for the particle
        // double b_corr = (particle.pT() * particle.eta()) / (particle.pT() * particle.pT());

        nF_sum += nF;
        nFSquared_sum += nF * nF;
        nBnF_sum += nB * nF;
        nB_sum += nB;
        // Calculate correlation function parameter a
        double a = nF_sum - b_corr * (nF_sum);    
        b_corr = ((nBnF_sum - nF_sum * nB_sum) / (nFSquared_sum -(nF_sum)*(nF_sum)));


        // Calculate the pseudorapidity gap
        double etaGap = ;

        // Fill the histogram with the pseudorapidity gap vs b_corr
        h2->Fill(etaGap, b_corr);
      }
    }
  }

  // Save the histogram to a ROOT file
  TFile* outputFile = new TFile("output.root", "RECREATE");
  h2->Write();
  outputFile->Close();

  // Draw the histogram
  TCanvas* canvas = new TCanvas("canvas", "Pseudorapidity Gap vs b_corr", 800, 600);
  h2->Draw("colz");
  canvas->SaveAs("output.png");

  // End of the program
  pythia.stat();
  return 0;
}
