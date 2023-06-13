#include "Pythia8/Pythia.h"
#include "TH1F.h"
#include "TCanvas.h"

using namespace Pythia8;

int main() {

  Pythia8::Pythia pythia;
  pythia.readString("Beams:eCM = 900.");
  pythia.readString("SoftQCD:all = on");
  pythia.init();

  // Parameters
  const double etaMax = 5.0;   // Maximum pseudorapidity range
  const double dn = 0.2;       // Pseudorapidity gap window width

  // Create histogram
  TH1F* histogram = new TH1F("histogram", "Correlation Strength vs Pseudorapidity Gap", 100, -etaMax, etaMax);

  // Event loop
  for (double n_gap = -etaMax; n_gap <= etaMax; n_gap += dn) {
    // Variables for correlation calculation
    int nF = 0;                  // Forward counter
    int nB = 0;                  // Backward counter
    double sumNF = 0.0;          // Sum of n_F
    double sumNB = 0.0;          // Sum of n_B
    double sumNBNF = 0.0;        // Sum of n_B * n_F
    double sumNFSquared = 0.0;   // Sum of (n_F)^2

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
        if (particle.isFinal() && particle.isCharged()) {
          double eta = particle.eta();
          if (eta > -etaMax && eta < etaMax) {
            // Count particles in forward and backward regions
            if (eta > dn / 2) {
              nF++;
              sumNF += eta;
              sumNFSquared += eta * eta;
            } else if (eta < -dn / 2) {
              nB++;
              sumNB += eta;
              sumNBNF += eta * nF;
            }
          }
        }
      }

      // Calculate correlation function
      double b_corr = ((sumNBNF - sumNF * sumNB) / (sumNFSquared - (sumNF) * (sumNF)));
      double a = sumNF - b_corr * (sumNF);

      // Fill the histogram with correlation data
      histogram->Fill(n_gap, b_corr);
    }
  }

  // Create a canvas and draw the histogram
  TCanvas* canvas = new TCanvas("canvas", "Correlation Plot");
  histogram->Draw();
  canvas->Print("correlation.png");

  // End of the program
  return 0;
}









// #include "Pythia8/Pythia.h"
// #include "TFile.h"
// #include "TTree.h"

// using namespace Pythia8;

// int main() {
  
//   Pythia8::Pythia pythia;
//   pythia.readString("Beams:eCM = 900.");
//   pythia.readString("SoftQCD:all = on");
//   pythia.init();

//   // Parameters
//   const double etaMax = 5.0;   // Maximum pseudorapidity range
//   const double dn = 0.2;       // Pseudorapidity gap window width

//   // Variables for correlation calculation
//   int nF = 0;                  // Forward counter
//   int nB = 0;                  // Backward counter
//   int nPairs = 0;              // Number of forward-backward pairs
//   double sumNF = 0.0;          // Sum of n_F
//   double sumNB = 0.0;          // Sum of n_B
//   double sumNBNF = 0.0;        // Sum of n_B * n_F
//   double sumNFSquared = 0.0;   // Sum of (n_F)^2

//   // Event loop
//   for (int iEvent = 0; iEvent < 10000; ++iEvent) 
//   {
//     if (!pythia.next()) continue;

//     // Reset counters and sums for each event
//     nF = 0;
//     nB = 0;
//     sumNF = 0.0;
//     sumNB = 0.0;
//     sumNBNF = 0.0;
//     sumNFSquared = 0.0;
    
//     // Particle loop
//     for (int i = 0; i < pythia.event.size(); ++i) 
//     {
//       Particle& particle = pythia.event[i];
//       if (particle.isFinal() && particle.isCharged()) 
//       {
//         double eta = particle.eta();
//         if (eta > -etaMax && eta < etaMax) {
//           // Count particles in forward and backward regions
//           if (eta > dn / 2) {
//             nF++;
//             sumNF += eta;
//             sumNFSquared += eta * eta;
//           } else if (eta < -dn / 2) {
//             nB++;
//             sumNB += eta;
//             sumNBNF += eta * nF;
//             sumNF += nF;
//             sumNB += nB;
//             sumNBNF += nB * nF;
//           }
//         }
//       }
//     }

//     // Update number of pairs
//     nPairs += nF * nB;
//   }

//   // Calculate correlation function
//   // double a = (sumNB - (sumNBNF / nPairs)) / nB;
//   double b_corr = ((sumNBNF - sumNF * sumNB) / (sumNFSquared - (sumNF) * (sumNF)));
//   double a = sumNF - b_corr * (sumNF);
//   // double b_corr = (sumNBNF / nPairs - (sumNF / nF) * (sumNB / nB)) / (sumNFSquared / nF - (sumNF / nF) * (sumNF / nF));
  
//   // Create a ROOT file and tree
//   TFile file("correlation.root", "RECREATE");
//   TTree tree("correlation", "Forward-Backward Correlation");

//   // Branches for correlation data
//   double pseudorapidityGap = log(nF) - log(nB);
//   double correlationStrength;
//   tree.Branch("PseudorapidityGap", &pseudorapidityGap, "PseudorapidityGap/D");
//   tree.Branch("CorrelationStrength", &correlationStrength, "CorrelationStrength/D");

//   // Fill the tree with correlation data
//   for (double gap = -etaMax; gap <= etaMax; gap += dn) {
//     pseudorapidityGap = n_gap;
//     correlationStrength = b_corr;
//     tree.Fill();
//   }

//   // Write the tree to the file and close
//   tree.Write();
//   file.Close();

//   // End of the program
//   return 0;
// }
