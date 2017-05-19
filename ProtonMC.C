#define ProtonMC_cxx
#include "ProtonMC.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

//==================================================================|
//=====                 Defining Histograms                    =====|
//==================================================================|
TH1D *h1 = new TH1D("h1", "", 100, 0, 1);
TH1D *h2 = new TH1D("h2", "", 100, 0, 1);
TH1D *h3 = new TH1D("h3", "", 100, 0, 1);
TH1D *h4 = new TH1D("h4", "", 100, 0, 1);
//==================================================================|

void ProtonMC::Loop()
{
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;

   //---------------------------------------------------------------|
   //-----                 Defining Counters                   -----|
   //---------------------------------------------------------------|
   int nEntries = 50000;
   int nProtonInelastic = 0;
   int nEventsHadronicProton = 0;
   int nEventsProtonInelastic = 0;
   //---------------------------------------------------------------|
   
   //---------------------------------------------------------------|
   //-----         Defining the Dimensions of LArIAT           -----|
   //---------------------------------------------------------------|
   double x0 = 0; //This is in centimeters and is left x coordinate
   double xF = 47; //This is in centimeters and is right x coordinate
   double y0 = -20; //This is in centimeters and is left y coordinate
   double yF = 20; //This is in centimeters and is right y coordinate
   double z0 = 0; //This is in centimeters and is left z coordinate
   double zF = 90; //This is in centimeters and is right z coordinate
   //---------------------------------------------------------------|

   //###############################################################|
   //#####                    Event Loop                       #####|
   //###############################################################|
   for (Long64_t jentry=0; jentry<nEntries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      int nParticlesInEvent = geant_list_size;
      int process = Process[jentry];
      if (process == 10) {
         nProtonInelastic++;   
      }

      //############################################################|
      //#####                  Particle Loop                   #####|
      //############################################################|
      for (int iPart=0; iPart<nParticlesInEvent; iPart++) {
         int IsPrimary = process_primary[iPart];
         int PDG = pdg[iPart];
	 double protonFinalPositionX = EndPointx[iPart];
	 double protonFinalPositionY = EndPointy[iPart];
	 double protonFinalPositionZ = EndPointz[iPart];
         if (IsPrimary == 1 && PDG == 2212 && process == 10 && protonFinalPositionX >= x0 && protonFinalPositionX <= xF && protonFinalPositionY >= y0 && protonFinalPositionY <= yF && protonFinalPositionZ >= z0 && protonFinalPositionZ <= zF) {
            nEventsProtonInelastic++;
         }     

         if (IsPrimary == 1 && PDG == 2212 && process == 3 && protonFinalPositionX >= x0 && protonFinalPositionX <= xF && protonFinalPositionY >= y0 && protonFinalPositionY <= yF && protonFinalPositionZ >= z0 && protonFinalPositionZ <= zF) {
            nEventsHadronicProton++;
         }
         
      }
      //############################################################|

   }
   //###############################################################|
   
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
