#include <iostream>

#include "Pythia8/Pythia.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

int main()
{
    TFile *output = new TFile("test10.root", "recreate");
    TTree *tree = new TTree("tree", "tree");
    int id, event, size, no;
    double m, px, py, pz;

    tree->Branch("event", &event, "event/I");
    tree->Branch("size", &size, "size/I");
    tree->Branch("no", &no, "no/I");
    tree->Branch("id", &id, "id/I");
    tree->Branch("m", &m, "m/D");
    tree->Branch("px", &m, "px/D");
    tree->Branch("py", &m, "py/D");
    tree->Branch("pz", &m, "pz/D");

    int nevents = 1e4;

    Pythia8::Pythia pythia;
    
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 14.e3");
    pythia.readString("SoftQCD:all = on");
    pythia.readString("HardQCD:all = on");
    
    
    pythia.init();
    
    for(int i = 0; i < nevents; i++)
    {
        if(!pythia.next()) continue;
        
        int entries = pythia.event.size();
        
        cout << "Event: " << i << endl;
        // cout << "Event size: " << entries << endl;
        
        event = i;
        size = entries;

        for(int j = 0; j < entries; j++)
        {
            int id = pythia.event[j].id();

            no = j;
            
            m = pythia.event[j].m();
            
            px = pythia.event[j].px();
            py = pythia.event[j].py();
            pz = pythia.event[j].pz();
            
            // double pabs = sqrt(pow(px,2) + pow(py,2) + pow(pz,2));
            
            // cout << id << " " << m << " " << pabs << endl;

            tree->Fill();
        }
    }

    output->Write();
    output->Close();

    return 0;
}