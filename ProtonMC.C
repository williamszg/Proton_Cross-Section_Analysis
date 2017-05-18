#define ProtonMC_cxx
#include "ProtonMC.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void ProtonMC::Loop()
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   int nEntries = 50000;
   int nProtonInelastic = 0;
   int nEventsHadronicProton = 0;
   int nEventsProtonInelastic = 0;

   //for (Long64_t jentry=0; jentry<nentries; jentry++) {
   for (Long64_t jentry=0; jentry<nEntries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      int nParticlesInEvent = geant_list_size;
      int process = Process[jentry];
      if (process == 10) {
         nProtonInelastic++;   
      }

      for (int iPart=0; iPart<nParticlesInEvent; iPart++) {
         int IsPrimary = process_primary[iPart];
         int PDG = pdg[iPart];
         if (IsPrimary == 1 && PDG == 2212 && process == 10) {
            nEventsProtonInelastic++;
         }     

         if (IsPrimary == 1 && PDG == 2212 && process == 3) {
            nEventsHadronicProton++;
         }
         
      }

   }
   
   int nTotalProton = nEventsHadronicProton + nEventsProtonInelastic;
   std::cout<<"|---------------------------------------------------------------------------|"<<std::endl;
   std::cout<<"     Number of Inelastic Proton Events: "<<nProtonInelastic<<std::endl;
   std::cout<<"|---------------------------------------------------------------------------|"<<std::endl;
   std::cout<<" "<<std::endl;
   std::cout<<"|---------------------------------------------------------------------------|"<<std::endl;
   std::cout<<"     Number of Events that were Hadronic Elastic: "<<nEventsHadronicProton<<std::endl;
   std::cout<<"|---------------------------------------------------------------------------|"<<std::endl;
   std::cout<<"     Number of Events that were Proton Inelastic: "<<nEventsProtonInelastic<<std::endl;
   std::cout<<"|===========================================================================|"<<std::endl;
   std::cout<<"     Number of Total Events that were Hadronic (Inelastic and Elastic): "<<nTotalProton<<std::endl;
   std::cout<<"|---------------------------------------------------------------------------|"<<std::endl;
}
