// -*- C++ -*-
//
// Package:    EgammaHLTProducers
// Class:      EgammaHLTPixelMatchElectronProducers
// 
/**\class EgammaHLTPixelMatchElectronProducers RecoEgamma/ElectronProducers/src/EgammaHLTPixelMatchElectronProducers.cc

 Description: EDProducer of HLT Electron objects

*/
//
// Original Author: Monica Vazquez Acosta (CERN)
// $Id:$
//
//

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTPixelMatchElectronProducers.h"
#include "RecoEgamma/EgammaHLTAlgos/interface/EgammaHLTPixelMatchElectronAlgo.h"
#include "DataFormats/EgammaReco/interface/ElectronPixelSeedFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronPixelSeed.h"
#include "DataFormats/EgammaReco/interface/SeedSuperClusterAssociation.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"

#include <iostream>

using namespace reco;
 
EgammaHLTPixelMatchElectronProducers::EgammaHLTPixelMatchElectronProducers(const edm::ParameterSet& iConfig) : conf_(iConfig)
{
  //register your products
  produces<ElectronCollection>();

  //create algo
  algo_ = new EgammaHLTPixelMatchElectronAlgo();

}


EgammaHLTPixelMatchElectronProducers::~EgammaHLTPixelMatchElectronProducers()
{
  delete algo_;
}

void EgammaHLTPixelMatchElectronProducers::beginJob(edm::EventSetup const&iSetup) 
{     
  algo_->setupES(iSetup,conf_);  
}

// ------------ method called to produce the data  ------------
void EgammaHLTPixelMatchElectronProducers::produce(edm::Event& e, const edm::EventSetup& iSetup) 
{

  // Create the output collections   
  std::auto_ptr<ElectronCollection> pOutEle(new ElectronCollection);
  
  // invoke algorithm
    algo_->run(e,*pOutEle);

  // put result into the Event
    e.put(pOutEle);
  
}


