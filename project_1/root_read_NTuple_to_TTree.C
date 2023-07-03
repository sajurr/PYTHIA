
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include "TH2.h"
#include <cmath>
using namespace std;

// MACRO TO TRANSLATE, ROTATE, AND WEIGHT PLOTS
// ADJUST LINES 17, 106, 241, 242 WHEN SWAPPING BETWEEN SIGNAL AND BACKGROUND ROOT FILES 
// ADJUST LINES 181, 237 TO CHANGE WEIGHTS
// ADJUST LINES 234-236 FOR JET RESCALING

void Signal4()
{

// Open files //switch background and signal here
	//TFile *file = TFile::Open("/mnt/storage/lborgna/SignalStitch/SigAll2_Training.root");
	TFile *file = TFile::Open("/mnt/storage/lborgna/Wprime/m600.root");
	TTree *tree = (TTree*)file -> Get("FlatSubstructureJetTree;6");
//	TCanvas *can1 = new TCanvas ("c1", "Eta Phi Plot", 600, 400);
	
// Define histogram list and output file
    TList* hList = new TList();      // list of histograms to store
    TFile* outfile = new TFile("/mnt/storage/lborgna/WprimePreP/Pre_Sig7_m600_Test_25p.root", "RECREATE");

  
//	TH2F *histo1 = new TH2F("histo1", "	",50,-1,-1,50,-1,1);

// Initialize
	float clus_N;
	float clus_E;		
	float clus_P;	
	float fjet_eta;
	float fjet_phi;
	float fjet_E;
	float fjet_pt;
	float max;
	float weight;
	float clus_dist;	
	float theta;
	float clus_Nfin;
	float clus_Pfin;
	float fjet_truthJet_pt;



	vector <float> *clus_eta 		    = 0;
	vector <float> *clus_energy 		    = 0;		
	vector <float> *clus_phi 		    = 0;

					
	int fjet_fatjet_dRmatched_particle_flavor;
    int particle_count = 0;

	const float PI = 3.1415927;
	const float GeV = 1000;
		
// Set variables
	tree -> SetBranchAddress("clus_eta", 		&clus_eta);
	tree -> SetBranchAddress("clus_phi", 		&clus_phi);
	tree -> SetBranchAddress("clus_E", 		&clus_energy);
	tree -> SetBranchAddress("fjet_pt", 		&fjet_pt);
	tree -> SetBranchAddress("fjet_eta", 		&fjet_eta);
	tree -> SetBranchAddress("fjet_phi", 		&fjet_phi);	
	tree -> SetBranchAddress("fjet_E", 		&fjet_E);
	tree -> SetBranchAddress("fjet_truthJet_pt", &fjet_truthJet_pt);
	tree -> SetBranchAddress("fjet_fatjet_dRmatched_particle_flavor", &fjet_fatjet_dRmatched_particle_flavor);		
	int tree_size = tree -> GetEntries();
	
	cout << tree_size << endl;


	//not accepting dynamic tree_size for some reason

	TH2F *myhist[20000000];
    char *histname = new char[20000000];


// Fill histo1 with clusters
	//tree_size=2; // ADJUST NUMBER OF JETS TO LOOK AT
	//tree_size = 1000;
	int clus_hits;

	for (int i = 0; i<tree_size; i++) {
		tree->GetEntry(i);


		// select only W bosons jets
		//while (((fjet_fatjet_dRmatched_particle_flavor == 24) || (fjet_fatjet_dRmatched_particle_flavor == -24)) && ( fjet_truthJet_pt > 1500* GeV && fjet_truthJet_pt < 2000 * GeV) && (fjet_pt > 1500 * GeV && fjet_pt < 2000 * GeV)) {	//PGID, comment out for background file
        while (fjet_fatjet_dRmatched_particle_flavor == 24 || fjet_fatjet_dRmatched_particle_flavor == -24) {
			cout << "Hello" << endl; 	
            sprintf(histname,"histo%d",particle_count);
  		    myhist[particle_count] = new TH2F(histname,"",25,-1,1,25,-1,1);
    	    cout << histname << endl;

			// determine which cluster has the largest energy
			for (int k = 0; k<clus_eta->size(); k++) {
				if (k == 0)
					max = clus_energy->at(k);
				if (clus_energy->at(k) > max)
					max = clus_energy->at(k);
				}		

			// run through each cluster for all clusters in a jet	
			for (int j = 0; j<clus_eta->size(); j++) {
		 
				// for only the cluster with the largest energy...
				while (clus_energy->at(j) == max) {
					
					// get initial cluster	
					float clus_N = clus_eta->at(j);
					float clus_P = clus_phi->at(j);
					float clus_E = clus_energy->at(j);				
			
					// first shift to center at jet (0,0)
					float clus_N = clus_N - fjet_eta;				
					float clus_P = clus_P - fjet_phi;				
					
					// correct bounds for overshift	(azimuthal periodicity)
					if (clus_P > PI) 
						float clus_P = clus_P - 2*PI;	
			
					else if (clus_P < -PI) 
						float clus_P = clus_P + 2*PI ;				

					else 
						float clus_P = clus_P;			
					
					// find angle between cluster and the y-axis
					float clus_dist = sqrt( pow((clus_N-0),2) + pow((clus_P-0),2) );
					
					// correct for global theta
						// quadrant 1
						if (clus_N > 0 && clus_P > 0) {
							float theta = asin(clus_N/clus_dist);
							cout << "theta #1: " << theta << endl; 
							} 
					
						// quadrant 2
						else if (clus_N < 0 && clus_P > 0) {
							clus_N = abs(clus_N);
							float theta = asin(clus_N/clus_dist);
							float theta = 2*PI - theta;
							cout << "theta #2: " << theta << endl; 
							}
						
						// quadrant 3
						else if (clus_N < 0 && clus_P < 0) {
							clus_N = abs(clus_N);
							float theta = asin(clus_N/clus_dist);
							float theta = PI + theta;
							cout << "theta #3: " << theta << endl; 
							} 
						
						// quadrant 4 
						else if (clus_N > 0 && clus_P < 0) {
							float theta = asin(clus_N/clus_dist);
							float theta = PI - theta;
							cout << "theta #4: " << theta << endl; 
							} 	
						
						else
						cout << "No Quadrant Selected" << endl; 			
					
					// rotate this cluster to the y-axis
					//float clus_N = 0;
					//float clus_P = clus_dist;

					myhist[particle_count]->Fill(clus_N, clus_P, clus_E);

					break;
					}
					
			}		
			
			// run through each cluster for all clusters in a jet again	
			for (int q = 0; q<clus_eta->size(); q++) {					
					
				// rotate other clusters by theta 
				//while (clus_energy->at(q) != max) {
				
					// get initial cluster	
					
					float clus_N 		= clus_eta->at(q);
					float clus_P 		= clus_phi->at(q);
					float clus_E	 	= clus_energy->at(q);
			
					float clus_theta = 2*atan(exp(-clus_N));
					float clus_et = abs(clus_E*cos(clus_theta)); //	transverse energy, var 5 (use in report)	
					
					// first shift to centralize at jet (0,0)
					float clus_N = clus_N - fjet_eta;				
					float clus_P = clus_P - fjet_phi;				
					
					// correct bounds for overshift	
					if (clus_P > PI) 
						float clus_P = clus_P - 2*PI;	
			
					else if (clus_P < -PI) 
						float clus_P = clus_P + 2*PI ;				

					else 
						float clus_P = clus_P;					
		
					// rotate the clusters
					
					float sf = 300000/fjet_pt; // scale
					//float clus_Nfin = (clus_N*cos(theta) - clus_P*sin(theta))*sf;
					//float clus_Pfin = (clus_N*sin(theta) + clus_P*cos(theta))*sf;
		            clus_Nfin = clus_N;
		            clus_Pfin = clus_P;
					
					myhist[particle_count] -> Fill(clus_Nfin, clus_Pfin, clus_E);	// switch out third value to weight differently

              
                    clus_hits++;
              
					}
					
        hList -> Add(myhist[particle_count]);
        particle_count = particle_count + 1;

		break;	//PGID, comment out for background file
		}       //PGID, comment out for background file	
		//hList->Add(myhist[i]);
										
	}

  
  
// Plot histo1
/*	histo1 -> GetYaxis() -> SetTitle("Cluster Azimuthal Angle (Phi)");
	histo1 -> GetYaxis() -> SetTitleOffset(2);
	histo1 -> GetXaxis() -> SetTitle("Cluster Pseudorapidity (Eta)");
	histo1 -> GetXaxis() -> SetTitleOffset(2);
	histo1 -> GetZaxis() -> SetTitle("clus_ET");
	histo1 -> GetZaxis() -> SetTitleOffset(1.35);
   	gStyle->SetOptStat(0); // stat box display off
   	histo1 -> Draw("COLZ");
   	c1 -> SaveAs ("jetwisebckplot1");
	return c1;	*/
  
  hList->Write();
  outfile->Close();

}

