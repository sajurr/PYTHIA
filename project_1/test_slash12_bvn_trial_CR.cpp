
//This is a macro to generate MB events in Pythia
//Subhadeep Roy



// Stdlib header file for input and output.
#include <iostream>

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for histogramming.
#include "TH1.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"

// ROOT, for saving file.
#include "TFile.h"
#include "TNtuple.h"

// using namespace Pythia8;

int main()
{
// Create the ROOT application environment.
// TApplication theApp("hist", &argc, argv);


Int_t nEvent = 5000;

Pythia8::Pythia pythia;  // pythia object created

// Settings& settings = pythia.settings;
// Info& info = pythia.info;

// Setting beam parameters & proj/target information
//  select Pythia min. bias model
pythia.readString("SoftQCD:inelastic = on");
pythia.readString("Tune:ee = 7");//monash tuning
pythia.readString("Tune:pp = 14");//monash tuning
pythia.readString("Random:setSeed = on");
pythia.readString("Random:seed = 5738921");   //run time ran number
pythia.readString("Beams:idA = 2212");
pythia.readString("Beams:idB = 2212");   // pid for proton
pythia.readString("Beams:eCM = 13000.0");   //Set the C.O.M energy in GeV here

pythia.readString("PartonLevel:MPI = on");                                                                                         
pythia.readString("MultiPartonInteractions:pT0Ref = 2.15");

pythia.readString("ColourReconnection:reconnect = on");
pythia.readString("ColourReconnection:mode = 1");
pythia.readString("ColourReconnection:allowDoubleJunRem = off");                                                                   
pythia.readString("ColourReconnection:m0= 0.3");                                                                                   
pythia.readString("ColourReconnection:allowJunctions = on");                                                                       
pythia.readString("ColourReconnection:junctionCorrection = 1.2");                                                                  
pythia.readString("ColourReconnection:timeDilationMode = 2");                                                                      
pythia.readString("ColourReconnection:timeDilationPar = 0.18");

pythia.readString("Ropewalk:RopeHadronization = off");                                                                              


// Initialising Pythia
pythia.init();

// Create file on which histogram(s) can be saved.

//TFile* outFile = new TFile("/qnap2/subhadeep/meanpt_23sep21/pp13TeV/generated_data/CRoff_RHoff/CRoff_RHoff_$i.root", "RECREATE");
TFile* outFile = new TFile("/home/sagar/project_1/pythia8MB_CR1_RHoff_test.root", "RECREATE");


Float_t px = 0., py = 0., pz = 0., pt = 0., eta = 0., phi = 0.;
Float_t mass = 0., energy = 0., pid = 0., pidMother1 = 0., pidMother2 = 0. ;
Int_t mult = 0, multV0 = 0;
TH1D *hMult = new TH1D("hMult", "hMult", 20000, 0, 20000);
TH1D *hMultV0 = new TH1D("hMultV0", "hMultV0", 20000, 0, 20000);
double pTMin = 0.3;
double pTMax = 1.5;
// int eventNumber = 10000;
vector<double> window_width = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
vector<double> fwdMult(window_width.size(), 0.0);
vector<double> bkwdMult(window_width.size(), 0.0);

// Create histograms and profiles
TH1F hist("hist", "pT", 100, pTMin, pTMax);
TH1F hist2("hist2", "eta", 100, -3, 3);
TH1D *hPt = new TH1D("hPt", "hPt", 2000, 0., 200.);
TH1D *hXsec = new TH1D("hXsec", "hXsec", 1, 0., 1.);

TH2F b_corr_vs_width("b_corr_vs_width", "b_corr vs Window Width", window_width.size() - 2, &window_width[1], 100, 0, 1.2);
TProfile profile_b_corr("profile_b_corr", "b_corr vs Window Width", window_width.size() - 2, &window_width[1]);

TNtuple *nt;

// Begin Event Loop. Generate Event; Skip if aborted.
for (int iEvent = 0; iEvent < nEvent; ++iEvent) // Event Loop Starts...
{
     if (!pythia.next()) continue;
    mult=0;
    multV0 = 0;

hXsec->Fill("<#sigma_{CrossSection}>",info.sigmaGen());

nt = new TNtuple(Form("Event%d", iEvent), "Ntuple", "pt:eta:phi:px:py:pz:mass:energy:pid:pidMother1:pidMother2");

// Reset forward and backward multiplicities for each event
        fill(fwdMult.begin(), fwdMult.end(), 0.0);
        fill(bkwdMult.begin(), bkwdMult.end(), 0.0);

// Particle Loop Starts...
for (int iPart = 0; iPart < pythia.event.size(); ++iPart)
{
if (pythia.event[iPart].isFinal() && pythia.event[iPart].isCharged())
if (pythia.event[i].pT() < pTMin || pythia.event[i].pT() > pTMax)
continue;
{
// Conditional statement
            
            // Scatter plots of pT and eta
            hist.Fill(pythia.event[i].pT());
            hist2.Fill(pythia.event[i].eta());

            // Choosing forward multiplicity within different pseudorapidity windows
            for (int j = 0; j < window_width.size() - 1; ++j) {
                if ((pythia.event[i].eta()) >= window_width[j] && (pythia.event[i].eta()) < window_width[j + 1]) {
                    fwdMult[j]++;
                    break;

                }
            }
            // Choosing backward multiplicity within different pseudorapidity windows
            for (int j = 0; j < window_width.size() - 1; ++j) {
                if ((pythia.event[i].eta()) <= -window_width[j] && (pythia.event[i].eta()) > -window_width[j + 1]) {
                    bkwdMult[j]++;
                    break;
                }
            }
pt = pythia.event[iPart].pT() ;
eta = pythia.event[iPart].eta() ;
phi = pythia.event[iPart].phi() ;
px = pythia.event[iPart].px() ;
py = pythia.event[iPart].py() ;
pz = pythia.event[iPart].pz() ;
mass = pythia.event[iPart].m() ;
energy = pythia.event[iPart].e() ;
pid = pythia.event[iPart].id() ;

pidMother1 = pythia.event[pythia.event[iPart].mother1()].id();
pidMother2 = pythia.event[pythia.event[iPart].mother2()].id();
//cout << "pidMother1: "<<pidMother1<<"\t"<<"pidMother2: "<< pidMother2<<endl;

nt->Fill(pt,eta,phi,px,py,pz,mass,energy,pid,pidMother1,pidMother2);

if(fabs(eta)<1.0 ){

hPt->Fill(pt);
mult++;
}

if((eta > 2.8 && eta < 5.1 )||( eta > -3.7 && eta < -1.7))
{
multV0++;
}

}
}// Particle Loop ends

            double sum_nF_k = 0.0;
            double sum_nB_k = 0.0;
            double sum_nBnF_k = 0.0;
            double nFSquared_sum_k = 0.0;

            // Calculate and store values for each window width
            for (int k = 0; k < window_width.size() - 2; ++k) {
            sum_nF_k += fwdMult[k];
            nFSquared_sum_k += fwdMult[k] * fwdMult[k];

            sum_nB_k = 0.0;
            sum_nBnF_k = 0.0;

            for (int b = k + 1; b < window_width.size() - 1; ++b) {
                sum_nB_k += bkwdMult[b];
                sum_nBnF_k += fwdMult[k] * bkwdMult[b];
            }

            double b_corr = (sum_nBnF_k - sum_nF_k * sum_nB_k) / (nFSquared_sum_k - sum_nF_k * sum_nF_k);

            if (std::isnan(b_corr) || std::isinf(b_corr))
                continue;

            b_corr_vs_width.Fill(window_width[k], b_corr);
            profile_b_corr.Fill(window_width[k], b_corr);
        }

outFile->cd();
/*E-TDirectoryFile::WriteTObject: Directory ./Kinematics.root is not writable*/

nt->Write();
delete nt;

if(iEvent%10000 == 0)cout << " iEvent = " << iEvent << endl;
//cout << "event size of" << iEvent << " is equal to "<< k << endl;

// Here  par_count is actually total no. of particles in one event
hMult->Fill(mult);
hMultV0->Fill(multV0);


}// Event Loop ends here

// Statistics on Event Generation
pythia.stat();

// Writing in TTree
hMult->Write();
hMultV0->Write();
hPt->Write();
hXsec->Write();

hist.Write();
hist2.Write();
b_corr_vs_width.Write();
profile_b_corr.Write();
outFile.Close();

outFile->Close();

return 0;
}
