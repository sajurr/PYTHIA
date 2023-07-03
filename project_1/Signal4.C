#include <ROOT/RVec.hxx>

using doubles = ROOT::VecOps::RVec<double>;

void Signal4() {
   std::unique_ptr<TFile> file{TFile::Open("/Users/axel/Downloads/kine_test5_CC_pe-_hww_4j_sm_FCChe_cteq_P-80_60_50000_exotics.root")};
   ROOT::RDataFrame df("FlatSubstructureJetTree", file.get());
   auto pass = df.Filter([](int fl){ return fl == 24 || fl == -24; }, "fjet_fatjet_dRmatched_particle_flavor");

   auto idxOfMax = [](const doubles &vec) {
      auto imax = std::max_element(vec.begin(), vec.end());
      return imax - vec.begin();
   };
   auto dfExtended = pass.Define("maxClusterIdx", idxOfMax, {"clus_energy"})
   .Define("clus_N", [](double clus_eta, double fjet_eta) {
                        return std::fabs(clus_eta - fjet_eta);
                     }, {"clus_eta", "fjet_eta"})
   .Define("clus_P", [](double clus_phi, double fjet_phi) {
                        return TVector2::Phi_mpi_pi(clus_phi - fjet_phi);
                     }, {"clus_phi", "fjet_phi"});
   auto hist = dfExtended.Histo2D<doubles>({"histo", "Energy;eta;phi", 25,-1,1,25,-1,1},
                              "clus_N", "clus_P", "clus_energy");
   hist->DrawClone();
}
