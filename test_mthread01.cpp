#include <iostream>
#include <vector>
#include <cmath>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TBrowser.h"
#include "TThread.h"

void *handle(void *ptr)
{
  int ith = (long)ptr;


  Pythia8::Pythia pythia;
  pythia.readString("Beams:eCM = 7000.");
  pythia.readString("SoftQCD:all = on");
  pythia.init();

  double etaCut = 2.5;  // Pseudorapidity cut for forward-backward correlation
  double ptMin = 0.3;   // Minimum transverse momentum
  double ptMax = 1.5;   // Maximum transverse momentum

  // Create variables for correlation function calculation
  double b_corr = 0.0;
  double nF_sum = 0.0;
  double nFSquared_sum = 0.0;
  double nBnF_sum = 0.0;
  double nB_sum = 0.0;

  // Create a histogram for b_corr versus n_gap
  TH2D hist2("b_corr_vs_n_gap", "b_corr vs. n_gap;Pseudorapidity Gap (n_gap);b_corr",
             20, 0, 0.7, 20, 0, 1.2);

  for (int iEvent = 0; iEvent < 10000; ++iEvent) {
    if (!pythia.next()) continue;

    // Initialize counters for each event
    int nF = 0;
    int nB = 0;
    
    // Indices of two particles used for n_gap calculation
    int particle_1 = 0;
    int particle_2 = 0;

    for (int i = 0; i < pythia.event.size(); ++i) 
    {
      // Select particles within the specified pseudorapidity and transverse momentum range
      if (pythia.event[i].isFinal() && pythia.event[i].isVisible() &&
          fabs(pythia.event[i].eta()) < etaCut && pythia.event[i].pT() > ptMin && pythia.event[i].pT() < ptMax) 
          {
        // Count the number of forward and backward particles
        if (pythia.event[i].eta() > 0)
          nF++;
        else
          nB++;
      }
    }

    // Accumulate values for correlation function calculation
    nF_sum += nF;
    nFSquared_sum += nF * nF;
    nBnF_sum += nB * nF;
    nB_sum += nB;

    // Calculate pseudorapidity gap (n_gap)
    double n_gap = log(nF) - log(nB); //Not sure if the relation is correct.
    std::cout << "n_gap = " << n_gap << std::endl;

    // Calculate correlation function parameter b_corr
    b_corr = ((nBnF_sum - nF_sum * nB_sum) / (nFSquared_sum - (nF_sum) * (nF_sum)));

    // Fill b_corr vs n_gap histogram
    hist2.Fill(n_gap, b_corr);
  }

  // Calculate correlation function parameter a
  double a = nF_sum - b_corr * (nF_sum);

  // Print correlation function parameters
  std::cout << "a = " << a << std::endl;
  std::cout << "b_corr = " << b_corr << std::endl;

  // Create a histogram for the correlation function
  TH1D hist("correlation_hist", "Forward-Backward Correlation;Number of Forward Particles (n_F);Number of Backward Particles (<n_B>)",
            20, 0, 30);

  // Fill the histogram with data points
  for (int i = 0; i < 10000; ++i)
   {
    int nF = 0;
    int nB = 0;
    for (int j = 0; j < 20; ++j) 
    {
      if (j < nF_sum)
        nF = j;
      else
        nF = nF_sum;
      nB = a + b_corr * nF;
      hist.Fill(nF, nB);
    }
  }

  // Create a TCanvas for plotting correlation function
  TCanvas canvas1("canvas1", "Forward-Backward Correlation Histogram", 800, 600);
  canvas1.cd();
  hist.Draw("COLZ");

  // Create a TCanvas for plotting b_corr vs n_gap
  TCanvas canvas2("canvas2", "b_corr vs. n_gap", 800, 600);
  canvas2.cd();
  hist2.Draw();

  // Write the histograms to a ROOT file
  // TFile file(Form("output_%d.root", ith), "RECREATE");
  TFile file("output.root", "RECREATE");
  hist.Write();
  hist2.Write();
  file.Close();

  // Open the TBrowser to view the histograms
  TBrowser browser;

  // Clean up
  pythia.stat();
}


int main() {
  int x =10;

  TThread* th[x];
  for(int i = 0; i < x; i++)
  {
    th[i] = new TThread(Form("th%d", i), handle, (void*)i);
    th[i]->Run();
  }

  for(int i = 0; i < x; i++)
  {
    th[i]->Join();
  }

  return 0;
}
