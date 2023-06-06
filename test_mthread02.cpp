#include <iostream>
#include <vector>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TBrowser.h"
#include "TThread.h"

void *handle(void *ptr)
{
  int ith = (long)ptr;


  Pythia8::Pythia pythia;
  pythia.readString("Beams:eCM = 7000.");
  pythia.readString("SoftQCD:all = on");
  pythia.readString("Random:setSeed = on");
  pythia.readString(Form("Random:seed = %d"), ith);

  pythia.init();

  double etaCut = 0.4;  // Pseudorapidity cut for forward-backward correlation
  double ptMin = 0.3;   // Minimum transverse momentum
  double ptMax = 1.5;   // Minimum transverse momentum

  // Create variables for correlation function calculation
  double b_corr = 0.0;
  double nF_sum = 0.0;
  double nFSquared_sum = 0.0;
  double nBnF_sum = 0.0;
  double nB_sum = 0.0;

  for (int iEvent = 0; iEvent < 10000; ++iEvent) {
    if (!pythia.next()) continue;

    // Initialize counters for each event
    int nF = 0;
    int nB = 0;

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
  }

  // Calculate correlation function parameter b_corr
  b_corr = ((nBnF_sum - nF_sum * nB_sum) / (nFSquared_sum -(nF_sum)*(nF_sum)));

  // Calculate correlation function parameter a
  double a = nF_sum - b_corr * (nF_sum);

  // Print correlation function parameters
  std::cout << "a = " << a << std::endl;
  std::cout << "b_corr = " << b_corr << std::endl;

  // Create a histogram for the correlation function
  TH1D hist("correlation_hist", "Forward-Backward Correlation;Number of Forward Particles (n_F);Number of Backward Particles (<n_B>)",
            20, 0, 20);
  
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
    //   std::cout << "nF =  " << nF << std::endl;
    }
  } 

  // Create a TCanvas for plotting
  TCanvas canvas("canvas", "Forward-Backward Correlation Histogram", 800, 600);
  canvas.cd();
  hist.Draw("COLZ");

  // Write the histogram and tree to a ROOT file
  TFile file(Form("output_FB_%d.root", ith), "RECREATE");
  hist.Write();
  file.Close();

  // Open the TBrowser to view the histogram and tree
  TBrowser browser;

  // Clean up
  pythia.stat();
}

int main() 
{
  int nthreads = 5;
  TThread* th[nthreads];
  for(int i = 0; i < nthreads; i++)
  {
    th[i] = new TThread(Form("th_%d", i), handle, (void*)i);
    th[i]->Run();
  }

  for(int i = 0; i < nthreads ; i++)
  {
    th[i]->Join();
  }

  return 0;
}
