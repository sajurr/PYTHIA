#include "Pythia8/Pythia.h"

// PythiaParallel.h must be included explicitly.
#include "Pythia8/PythiaParallel.h"
using namespace Pythia8;
void main() {
// The PythiaParallel object is created
// and initialized as normal.
PythiaParallel pythia;
pythia.readString("SoftQCD:nonDiffractive = on");
pythia.readString("Main:numberOfEvents = 10000");
pythia.init();
// Example: plot charged multiplicity
Hist nCh("Charged multiplicity", 100, -0.5, 399.5);
// This defines the callback that will analyse events.
function<void(Pythia& pythiaNow)> callback = [&nCh](Pythia& pythiaNow) 
{
    int nChNow = 0;
        for (int i = 0; i < pythiaNow.event.size(); ++i)
        if (pythiaNow.event[i].isFinal() &&
        pythiaNow.event[i].isCharged()) nChNow += 1;
        nCh.fill(nChNow);
};
// Generate events in parallel, using
// the specified callback for analysis.
pythia.run(callback);
// Print histogram.
cout << nCh;
}
