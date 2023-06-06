#include <iostream>
#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"

int main() {

  Pythia8::Pythia pythia;
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("Beams:eCM = 13000.");  
  pythia.readString("HardQCD:all = on");  
  pythia.init();

  double etaCut = 2.5;  // Pseudorapidity cut for forward-backward correlation
  double ptMin = 1.0;   // Minimum transverse momentum

  TFile *output_FB_corr = new TFile("output_FB_corr.root", "recreate");
  TTree *tree = new TTree("tree", "tree");
  
  double correlation, pT, eta, phi;;
  int pdgId, status;
  
  tree->Branch("correlation", &correlation, "correlation/D");
  tree->Branch("pdgId", &pdgId, "pdgId/I");
  tree->Branch("status", &status, "status/I");
  tree->Branch("pT", &pT, "pT/D");
  tree->Branch("eta", &eta, "eta/D");
  tree->Branch("phi", &phi, "phi/D");

  int nEvents = 1e4;

  for (int i = 0; i < nEvents; ++i) {
    if (!pythia.next()) continue;  // Generate the event

    // std::cout << "Events: " << i << std::endl;

    // Loop over particles in the event
    for (int j = 0; j < pythia.event.size(); ++j) 
    {
      // Select particles within the specified pseudorapidity and transverse momentum range
      if (pythia.event[j].isFinal() && pythia.event[j].isVisible() &&
          fabs(pythia.event[j].eta()) < etaCut && pythia.event[j].pT() > ptMin) 
      
        {

        correlation = pythia.event[j].pz(); // / pythia.event[j].e();
        pdgId       = pythia.event[j].id();
        status      = pythia.event[j].status();
        pT          = pythia.event[j].pT();
        eta         = pythia.event[j].eta();
        phi         = pythia.event[j].phi();

        tree->Fill();
      }
    }
  }

  tree->Write();

  // TBrowser browser;

  output_FB_corr->Write();
  output_FB_corr->Close();

  // pythia->stat();
  return 0;
}
