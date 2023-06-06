// #include "Pythia8/Pythia.h"
// #include "TFile.h"
// #include "TTree.h"
// #include "TH2D.h"
// #include "TCanvas.h"

// int main() {
//   // Set up PYTHIA
//   Pythia8::Pythia pythia;
//   pythia.readString("Beams:eCM = 900."); // Center of mass energy
//   pythia.readString("SoftQCD:all = on"); // Enable SoftQCD processes
//   pythia.init();

//   // Create a ROOT file to store the TTree and histogram
//   TFile file("forward_backward_correlation.root", "RECREATE");

//   // Create the TTree
//   TTree tree("tree", "Forward-Backward Correlation");

//   // Variables for storing the results
//   double b_corr;
//   double n_gap;
//   double nBnF_sum = 0.0;
//   double nFSquared_sum = 0.0;
//   double nB_sum = 0.0;

//   // Set the branches in the TTree
//   tree.Branch("b_corr", &b_corr, "b_corr/D");
//   tree.Branch("n_gap", &n_gap, "n_gap/D");

//   // Create a histogram to store b_corr versus n_gap
//   TH2D hist("hist", "Forward-Backward Correlation; Pseudorapidity Gap (|n|); Correlation Strength (b_corr)",
//             10, 0.0, 1.0, 100, -1.0, 1.0);

//   // Event loop
//   int nEvents = 0;
//   while (nEvents < 10000) 
//   {
//     if (!pythia.next()) continue; // Generate the event and check if it failed

//     // Get the number of particles and their properties
//     int nParticles = pythia.event.size();
//     double nB = 0.0;
//     double nF = 0.0;

//     for (int i = 0; i < nParticles; ++i) 
//     {
//       double eta = pythia.event[i].eta(); // Pseudorapidity of the particle
//       if (eta > 0.0)
//         nF += 1.0; // Count forward particles
//       else
//         nB += 1.0; // Count backward particles
//     }

//     // Calculate the correlation strength
//     double nBF = nB * nF;
//     double nF2 = nF * nF;
//     double nF_avg = nF / nParticles;
//     nFSquared_sum += nF * nF;
//     nB_sum += nB;
//     double nF_sum;
//     double a = nF_sum - b_corr * (nF_sum);
//     nF_sum += nF;
//     nB_sum += nB;
//     nBnF_sum += nB * nF;
//     b_corr = ((nBnF_sum - nF_sum * nB_sum) / (nFSquared_sum -(nF_sum)*(nF_sum)));

//     // Calculate the pseudorapidity gap
//     n_gap = std::abs(nF - nB);

//     // Fill the TTree
//     tree.Fill();

//     // Fill the histogram
//     hist.Fill(n_gap, b_corr);

//     nEvents++;
//   }

//   // Write the TTree and histogram to the ROOT file
//   tree.Write();
//   hist.Write();

//   // Close the ROOT file
//   file.Close();

//   // Plot the histogram
//   TCanvas canvas("canvas", "Forward-Backward Correlation", 800, 600);
//   hist.Draw("COLZ");
//   canvas.SaveAs("correlation_plot.png");

//   // End of program
//   return 0;
// }


#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TCanvas.h"

int main() {
  // Set up PYTHIA
  Pythia8::Pythia pythia;
  pythia.readString("Beams:eCM = 7000."); // Center of mass energy
  pythia.readString("SoftQCD:all = on"); // Enable SoftQCD processes
  pythia.init();

  // Create a ROOT file to store the TTree and histogram
  TFile file("forward_backward_correlation.root", "RECREATE");

  // Create the TTree
  TTree tree("tree", "Forward-Backward Correlation");

  // Variables for storing the results
  double b_corr = 0.0;
  double nF_sum = 0.0;
  double nFSquared_sum = 0.0;
  double nBnF_sum = 0.0;
  double nB_sum = 0.0;
  double n_gap;

  // Set the branches in the TTree
  tree.Branch("b_corr", &b_corr, "b_corr/D");
  tree.Branch("n_gap", &n_gap, "n_gap/D");

  // Create a histogram to store b_corr versus n_gap
  TH2D hist("hist", "Forward-Backward Correlation; Pseudorapidity Gap (|n|); Correlation Strength (b_corr)",
            10, 0.0, 1.2, 10, 0, 0.7);

  // Event loop
  int nEvents = 0;
  while (nEvents < 10000) 
  {
    if (!pythia.next()) continue; // Generate the event and check if it failed

    // Get the number of particles and their properties
    int nParticles = pythia.event.size();
    double nB = 0.0;
    double nF = 0.0;
    double etaCut = 0.4;  // Pseudorapidity cut for forward-backward correlation
    double ptMin = 0.3;   // Minimum transverse momentum
    double ptMax = 1.5;   // Minimum transverse momentum


    for (int i = 0; i < nParticles; ++i) 
    {
      // Select particles within the specified pseudorapidity and transverse momentum range
      if (pythia.event[i].isFinal() && pythia.event[i].isVisible() &&
          fabs(pythia.event[i].eta()) < etaCut && pythia.event[i].pT() > ptMin && pythia.event[i].pT() < ptMax) 
      {
      double eta = pythia.event[i].eta(); // Pseudorapidity of the particle
      if (eta > 0.0)
        nF += 1.0; // Count forward particles
      else
        nB += 1.0; // Count backward particles
      }
    }

    // Calculate the correlation strength
    nF_sum += nF;
    nFSquared_sum += nF * nF;
    nBnF_sum += nB * nF;
    nB_sum += nB;
    b_corr = ((nBnF_sum - nF_sum * nB_sum) / (nFSquared_sum -(nF_sum)*(nF_sum)));

    // Calculate the pseudorapidity gap
    n_gap = std::abs(nF - nB);

    // Fill the TTree
    tree.Fill();

    // Fill the histogram
    hist.Fill(n_gap, b_corr);

    nEvents++;
  }

  // Write the TTree and histogram to the ROOT file
  tree.Write();
  hist.Write();

  // Close the ROOT file
  file.Close();

  // Plot the histogram
  TCanvas canvas("canvas", "Forward-Backward Correlation", 800, 600);
  hist.Draw("COLZ");
  canvas.SaveAs("correlation_plot.png");

  // End of program
  return 0;
}
