#include<iostream>

#include "TTree.h"
#include "TFile.h"
#include "TThread.h"

#include "Pythia8/Pythia.h"

using namespace std;

void *handle(void *ptr)
{

    int ith = (long)ptr;
    TFile *output = new TFile(Form("test11_t%d.root", ith), "recreate");
    TTree *tree = new TTree("tree", "tree");

    int id, event, size, no;
    double m, px, py, pz;

    tree->Branch("event", &event, "event/I");
    tree->Branch("size", &size, "size/I");
    tree->Branch("no", &no, "no/I");
    tree->Branch("m", &m, "m/D");
    tree->Branch("px", &px, "px/D");
    tree->Branch("py", &py, "py/D");
    tree->Branch("pz", &pz, "pz/D");

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
        if (!pythia.next()) continue;

        int entries = pythia.event.size();

        cout<< "Event: " << i << endl;

        event = i;
        size = entries;

        for(int j = 0; j < entries; j++)
        {
            id = pythia.event[j].id();
            no = j;
            m = pythia.event[j].m();
            px = pythia.event[j].px();
            py = pythia.event[j].py();
            pz = pythia.event[j].pz();

            tree->Fill();
        }
        
    }
    output->Write();
    output->Close();
    
}

int main()
{
    TThread *th[4];

    for(int i = 0; i < 4; i++)
    {
        th[i] = new TThread(Form("th%d", i), handle, (void*)i);
        th[i]->Run();

    }
    
    for(int i = 0; i < 4; i++)
    {
        th[i]->Join();
    }

    return 0;
}