
//#include <FWCore/Framework/interface/Handle.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/Framework/interface/EventSetup.h>
#include <FWCore/Framework/interface/ESHandle.h>

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "RecoEgamma/EgammaHLTProducers/interface/ESRecHitsMerger.h"

#include "FWCore/Utilities/interface/Exception.h"


using namespace edm;
using namespace std;


ESRecHitsMerger::ESRecHitsMerger(const edm::ParameterSet& pset) {

 debug_ = pset.getUntrackedParameter<bool>("debug");
 
 EgammaSourceES_ = pset.getUntrackedParameter<edm::InputTag>("EgammaSource_ES",edm::InputTag("dummyEgamma"));
 MuonsSourceES_  = pset.getUntrackedParameter<edm::InputTag>("MuonsSource_ES",edm::InputTag("dummyMuons"));
 TausSourceES_  = pset.getUntrackedParameter<edm::InputTag>("TausSource_ES",edm::InputTag("dummyTaus"));
 JetsSourceES_   = pset.getUntrackedParameter<edm::InputTag>("JetsSource_ES",edm::InputTag("dummyJets"));
 RestSourceES_   = pset.getUntrackedParameter<edm::InputTag>("RestSource_ES",edm::InputTag("dummyRest"));
 Pi0SourceES_   = pset.getUntrackedParameter<edm::InputTag>("Pi0Source_ES",edm::InputTag("dummyPi0"));
 EtaSourceES_   = pset.getUntrackedParameter<edm::InputTag>("EtaSource_ES",edm::InputTag("dummyEta"));
  

 OutputLabelES_ = pset.getUntrackedParameter<std::string>("OutputLabel_ES");
 
 InputRecHitES_ = pset.getUntrackedParameter<std::string>("EcalRecHitCollectionES");
 
 produces<EcalRecHitCollection>(OutputLabelES_);
 
}



ESRecHitsMerger::~ESRecHitsMerger() {
}


void ESRecHitsMerger::beginJob(const edm::EventSetup& c){
}

void ESRecHitsMerger::endJob(){
}

void ESRecHitsMerger::produce(edm::Event & e, const edm::EventSetup& iSetup){

 if (debug_) cout << " ESRecHitMerger : Run " << e.id().run() << " Event " << e.id().event() << endl;


 std::vector< edm::Handle<ESRecHitCollection> > EcalRecHits_done;
 e.getManyByType(EcalRecHits_done);
 
 std::auto_ptr<EcalRecHitCollection> ESMergedRecHits(new EcalRecHitCollection);
 
 
 unsigned int nColl = EcalRecHits_done.size();
 
 int nES = 0;


 for (unsigned int i=0; i < nColl; i++) {

   std::string instance = EcalRecHits_done[i].provenance()->productInstanceName();
   std::string module_label = EcalRecHits_done[i].provenance()->moduleLabel();


   if (debug_){
     std::cout<<"ESrechit to be merged from "<<module_label.c_str()<<" "<<instance.c_str()<<std::endl;
   }
   
   if ( module_label != EgammaSourceES_.label() && 
	module_label != MuonsSourceES_.label() &&
	module_label != JetsSourceES_.label() &&
 	module_label != TausSourceES_.label() &&
 	module_label != RestSourceES_.label() &&
        module_label != Pi0SourceES_.label() &&
	module_label != EtaSourceES_.label()) continue;
   
   if (instance == InputRecHitES_) {
     nES += EcalRecHits_done[i] -> size();
   }
   
 }
 
 
 ESMergedRecHits -> reserve(nES);
 
 if (debug_) cout << " Number of ES Rechits to merge  = " << nES << endl;
 
 for (unsigned int i=0; i < nColl; i++) {
   std::string instance = EcalRecHits_done[i].provenance()->productInstanceName(); 

   std::string module_label = EcalRecHits_done[i].provenance()->moduleLabel();
   if ( module_label != EgammaSourceES_.label() && 
	module_label != MuonsSourceES_.label() &&
	module_label != JetsSourceES_.label() &&
 	module_label != TausSourceES_.label() &&
 	module_label != RestSourceES_.label() &&
        module_label != Pi0SourceES_.label() && 
	module_label != EtaSourceES_.label() ) continue;

   if (instance == InputRecHitES_) {
     for (EcalRecHitCollection::const_iterator it=EcalRecHits_done[i]->begin(); it !=EcalRecHits_done[i]->end(); it++) {
       ESMergedRecHits -> push_back(*it);
     }
   }
   
 }
 
 
 // cout << " avant le put " << endl;
 e.put(ESMergedRecHits,OutputLabelES_);
 // cout << " apres le put " << endl;

}

