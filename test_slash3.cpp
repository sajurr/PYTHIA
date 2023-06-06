#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"

using namespace Pythia8;

int main() {
  // PYTHIA initialization
  Pythia pythia;
  pythia.readString("Beams:eCM = 900.");
  pythia.readString("SoftQCD:all = on");
  pythia.init();

  // Parameters
  const double etaMax = 1.2;   // Maximum pseudorapidity range
  const double dn = 0.2;       // Pseudorapidity gap window width
  const double pTMin = 0.3;    // Minimum transverse momentum
  const double pTMax = 1.5;    // Maximum transverse momentum

  // Variables for correlation calculation
  int nF = 0;                  // Forward counter
  int nB = 0;                  // Backward counter
  double sumNF = 0.0;          // Sum of n_F
  double sumNB = 0.0;          // Sum of n_B
  double sumNBNF = 0.0;        // Sum of n_B * n_F
  double sumNFSquared = 0.0;   // Sum of (n_F)^2

  // Create a 2D histogram
  TH2D hist("b_corr_vs_n_gap", "b_corr vs. Pseudorapidity Gap", 50, 0, etaMax, 50, 0, 1.0);
  hist.GetXaxis()->SetTitle("Pseudorapidity Gap (n_gap)");
  hist.GetYaxis()->SetTitle("Correlation Strength (b_corr)");

  // Event loop
  for (int iEvent = 0; iEvent < 10000; ++iEvent) {
    if (!pythia.next()) continue;

    // Reset counters and sums for each event
    nF = 0;
    nB = 0;
    sumNF = 0.0;
    sumNB = 0.0;
    sumNBNF = 0.0;
    sumNFSquared = 0.0;

    // Particle loop
    for (int i = 0; i < pythia.event.size(); ++i) {
      Particle& particle = pythia.event[i];
      if (particle.isFinal() && particle.isCharged() && particle.pT() > pTMin && particle.pT() < pTMax) {
        double eta = particle.eta();
        if (eta > -etaMax && eta < etaMax) {
          // Count particles in forward and backward regions
          if (eta > dn / 2) {
            nF++;
            // sumNF += eta;
            // sumNFSquared += eta * eta;
          } else if (eta < -dn / 2) {
            nB++;
            // sumNB += eta;
            // sumNBNF += eta * nF;
          }
        }
      }
    }

    // Calculate correlation function
    double b_corr = ((sumNBNF - sumNF * sumNB) / (sumNFSquared -(sumNF)*(sumNF)));
    double a = sumNF - b_corr * (sumNF);
    
    // Fill the histogram with correlation data
    hist.Fill(n_gap, b_corr);
  }

  // Save the histogram to a ROOT file
  TFile file("correlation.root", "RECREATE");
  hist.Write();
  file.Close();

  // End of the program
  return 0;
}
