#include "Pythia8/Pythia.h"
#include "TH1F.h"
#include "TCanvas.h"

int main() {
  // Set up PYTHIA
  Pythia8::Pythia pythia;
  pythia.readString("Beams:eCM = 7000."); // Center of mass energy
  pythia.readString("SoftQCD:all = on"); // Enable hard QCD processes
  pythia.init();

  // Set up histograms
  const int numBins = 100;
  TH1F hist("hist", "Forward-Backward Correlation Strength", numBins, -1.0, 1.0);

  // Event loop
  for (int iEvent = 0; iEvent < 10000; ++iEvent) {
    if (!pythia.next()) continue; // Generate the event

    // Get the number of particles and their properties
    int nParticles = pythia.event.size();
    double nF = 0.0; // Number of forward particles
    double nB = 0.0; // Number of backward particles

    for (int i = 0; i < nParticles; ++i) {
      double eta = pythia.event[i].eta(); // Pseudorapidity of the particle
      if (eta > 0.0) nF += 1.0; // Count forward particles
      else nB += 1.0; // Count backward particles
    }

    // Calculate the correlation strength
    double b_corr = (nB * nF - nB * nF) / (nF * nF - nF * nF);

    // Fill the histogram
    hist.Fill(b_corr);
  }

  // Create a TCanvas for plotting
  TCanvas canvas("canvas", "Forward-Backward Correlation Strength", 800, 600);
  canvas.cd();

  // Plot the histogram
  hist.Draw();

  // Save the plot as a PDF file
  canvas.Print("forward_backward_correlation.pdf");

  // Clean up
  pythia.stat();
  return 0;
}
