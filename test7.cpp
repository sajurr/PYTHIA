#include<iostream>
#include "Pythia8/Pythia.h"

using namespace std;

int main()
{
    int nEvents = 2;
    Pythia8::Pythia pythia;

    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 14000");
    pythia.readString("SoftQCD:all = on");
    pythia.readString("HardQCD:all = on");

    Pythia8::Hist hpz("Momentum distribution", 100, -20, 20);
    
    pythia.init();

    for (int i = 0; i < nEvents; i++)
    {
        if(!pythia.next()) continue;

        int Entries = pythia.event.size();

        cout<<"Event: "<<i<<endl;
        cout<<"Event size: "<<Entries<<endl;

        for(int j = 0; j<Entries; j++)
        {
            int id = pythia.event[j].id();

            double m = pythia.event[j].m();
            
            double px = pythia.event[j].px();
            double py = pythia.event[j].py();
            double pz = pythia.event[j].pz();

            double pabs = sqrt(pow(px, 2) + pow(py, 2) + pow(pz, 2));

            hpz.fill(pz);

            cout<<id<<" "<<m<<" "<<pabs<<endl;

        }
        

    }
    
    cout<<hpz<<endl;
    Pythia8::HistPlot hpl{"test7"};
    hpl.frame("output_plot", "Momentum Distribution", "Momentum", "Entries");
    hpl.add(hpz);
    hpl.plot();
    return 0;

    
}