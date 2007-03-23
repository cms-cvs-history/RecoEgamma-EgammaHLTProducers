// C/C++ headers
#include <iostream>
#include <vector>
#include <memory>

// Framework
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

// Reconstruction Classes
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"

// Geometry
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"

// Level 1 Trigger
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"
#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"
#include "L1TriggerConfig/L1Geometry/interface/L1CaloGeometry.h"
#include "L1TriggerConfig/L1Geometry/interface/L1CaloGeometryRecord.h"

// EgammaCoreTools
#include "RecoEcal/EgammaCoreTools/interface/PositionCalc.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalEtaPhiRegion.h"

// Class header file
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTIslandClusterProducer.h"

EgammaHLTIslandClusterProducer::EgammaHLTIslandClusterProducer(const edm::ParameterSet& ps)
{
  // The verbosity level
  std::string verbosityString = ps.getParameter<std::string>("VerbosityLevel");
  if      (verbosityString == "DEBUG")   verbosity = IslandClusterAlgo::pDEBUG;
  else if (verbosityString == "WARNING") verbosity = IslandClusterAlgo::pWARNING;
  else if (verbosityString == "INFO")    verbosity = IslandClusterAlgo::pINFO;
  else                                   verbosity = IslandClusterAlgo::pERROR;

  doBarrel_   = ps.getParameter<bool>("doBarrel");

  // Parameters to identify the hit collections
  barrelHitProducer_   = ps.getParameter<std::string>("barrelHitProducer");
  endcapHitProducer_   = ps.getParameter<std::string>("endcapHitProducer");
  barrelHitCollection_ = ps.getParameter<std::string>("barrelHitCollection");
  endcapHitCollection_ = ps.getParameter<std::string>("endcapHitCollection");

  // The names of the produced cluster collections
  barrelClusterCollection_  = ps.getParameter<std::string>("barrelClusterCollection");
  endcapClusterCollection_  = ps.getParameter<std::string>("endcapClusterCollection");

  // Island algorithm parameters
  double barrelSeedThreshold = ps.getParameter<double>("IslandBarrelSeedThr");
  double endcapSeedThreshold = ps.getParameter<double>("IslandEndcapSeedThr");

  // L1 matching parameters
  l1Tag_ = ps.getParameter< edm::InputTag > ("l1Tag");
  l1LowerThr_ = ps.getParameter<double> ("l1LowerThr");
  l1UpperThr_ = ps.getParameter<double> ("l1UpperThr");
  regionEtaMargin_   = ps.getParameter<double>("regionEtaMargin");
  regionPhiMargin_   = ps.getParameter<double>("regionPhiMargin");

  // Parameters for the position calculation:
  std::map<std::string,double> providedParameters;
  providedParameters.insert(std::make_pair("LogWeighted",ps.getParameter<bool>("posCalc_logweight")));
  providedParameters.insert(std::make_pair("T0_barl",ps.getParameter<double>("posCalc_t0_barl")));
  providedParameters.insert(std::make_pair("T0_endc",ps.getParameter<double>("posCalc_t0_endc")));
  providedParameters.insert(std::make_pair("T0_endcPresh",ps.getParameter<double>("posCalc_t0_endcPresh")));
  providedParameters.insert(std::make_pair("W0",ps.getParameter<double>("posCalc_w0")));
  providedParameters.insert(std::make_pair("X0",ps.getParameter<double>("posCalc_x0")));
  posCalculator_ = PositionCalc(providedParameters);

  // Produces a collection of barrel and a collection of endcap clusters

  produces< reco::BasicClusterCollection >(endcapClusterCollection_);
  produces< reco::BasicClusterCollection >(barrelClusterCollection_);

  island_p = new IslandClusterAlgo(barrelSeedThreshold, endcapSeedThreshold, posCalculator_,verbosity);

  nEvt_ = 0;
}


EgammaHLTIslandClusterProducer::~EgammaHLTIslandClusterProducer()
{
  delete island_p;
}


void EgammaHLTIslandClusterProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  //Get the L1 EM Particle Collection
  edm::Handle< l1extra::L1EmParticleCollection > emColl ;
  evt.getByLabel(l1Tag_, emColl ) ;
  // Get the CaloGeometry
  edm::ESHandle<L1CaloGeometry> l1CaloGeom ;
  es.get<L1CaloGeometryRecord>().get(l1CaloGeom) ;

  std::vector<EcalEtaPhiRegion> barrelRegions;
  std::vector<EcalEtaPhiRegion> endcapRegions;

  for( l1extra::L1EmParticleCollection::const_iterator emItr = emColl->begin(); emItr != emColl->end() ;++emItr ){

    if (emItr->et() > l1LowerThr_ && emItr->et() < l1UpperThr_) {

      // Access the GCT hardware object corresponding to the L1Extra EM object.
      int etaIndex = emItr->gctEmCand()->etaIndex() ;


      int phiIndex = emItr->gctEmCand()->phiIndex() ;
      // Use the L1CaloGeometry to find the eta, phi bin boundaries.
      double etaLow  = l1CaloGeom->etaBinLowEdge( etaIndex ) ;
      double etaHigh = l1CaloGeom->etaBinHighEdge( etaIndex ) ;
      double phiLow  = l1CaloGeom->emJetPhiBinLowEdge( phiIndex ) ;
      double phiHigh = l1CaloGeom->emJetPhiBinHighEdge( phiIndex ) ;


	int isforw=0;
        if(fabs((float) ((etaLow+etaHigh)/2.))>1.5) isforw=1;

	//std::cout<<"Island etaindex "<<etaIndex<<" low hig : "<<etaLow<<" "<<etaHigh<<" phi low hig" <<phiLow<<" " << phiHigh<<" isforw "<<emItr->gctEmCand()->regionId().isForward()<<" isforwnew" <<isforw<< std::endl;

      etaLow -= regionEtaMargin_;
      etaHigh += regionEtaMargin_;
      phiLow -= regionPhiMargin_;
      phiHigh += regionPhiMargin_;


      EcalEtaPhiRegion region(etaLow,etaHigh,phiLow,phiHigh);
      //if (emItr->gctEmCand()->regionId().isForward()) {
      if (isforw) {
	endcapRegions.push_back(region);
      } else {
	barrelRegions.push_back(region);
      }

    }
  }

  //if (
  //endcapRegions.size()!=0
  //) {
  //std::cout<<"endcapregions: "<<endcapRegions.size()<<std::endl;
  //std::cout<<"barrelregions: "<<barrelRegions.size()<<std::endl;
  if (!doBarrel_ 
      //&&endcapRegions.size()!=0
) {

    clusterizeECALPart(evt, es, endcapHitProducer_, endcapHitCollection_, endcapClusterCollection_, endcapRegions, IslandClusterAlgo::endcap);
  }
  //if (doBarrel_ 
  //&& barrelRegions.size()!=0) {
  if (doBarrel_ 
      //&& barrelRegions.size()!=0
      ) {
    clusterizeECALPart(evt, es, barrelHitProducer_, barrelHitCollection_, barrelClusterCollection_, barrelRegions, IslandClusterAlgo::barrel);
  }
  nEvt_++;
}


const EcalRecHitCollection * EgammaHLTIslandClusterProducer::getCollection(edm::Event& evt,
                                                                  const std::string& hitProducer_,
                                                                  const std::string& hitCollection_)
{
  edm::Handle<EcalRecHitCollection> rhcHandle;
  try
    {
      evt.getByLabel(hitProducer_, hitCollection_, rhcHandle);
      if (!(rhcHandle.isValid())) 
	{
	  //std::cout << "could not get a handle on the EcalRecHitCollection!" << std::endl;
	  return 0;
	}
    }
  catch ( cms::Exception& ex ) 
    {
      edm::LogError("EgammaHLTIslandClusterProducerError") << "Error! can't get the product " << hitCollection_.c_str() ;
      return 0;
    }
  return rhcHandle.product();
}


void EgammaHLTIslandClusterProducer::clusterizeECALPart(edm::Event &evt, const edm::EventSetup &es,
                                               const std::string& hitProducer,
                                               const std::string& hitCollection,
                                               const std::string& clusterCollection,
                                               const std::vector<EcalEtaPhiRegion>& regions,
                                               const IslandClusterAlgo::EcalPart& ecalPart)
{
  // get the hit collection from the event:
  const EcalRecHitCollection *hitCollection_p = getCollection(evt, hitProducer, hitCollection);

  // get the geometry and topology from the event setup:
  edm::ESHandle<CaloGeometry> geoHandle;
  es.get<IdealGeometryRecord>().get(geoHandle);

  const CaloSubdetectorGeometry *geometry_p;
  CaloSubdetectorTopology *topology_p;

  if (ecalPart == IslandClusterAlgo::barrel) 
    {
      geometry_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);
      topology_p = new EcalBarrelTopology(geoHandle);
    }
  else
    {
      geometry_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
      topology_p = new EcalEndcapTopology(geoHandle); 
   }

  const CaloSubdetectorGeometry *geometryES_p;
  geometryES_p = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);

  // Run the clusterization algorithm:
  reco::BasicClusterCollection clusters;
  clusters = island_p->makeClusters(hitCollection_p, geometry_p, topology_p, geometryES_p, ecalPart, true, regions);

  // create an auto_ptr to a BasicClusterCollection, copy the barrel clusters into it and put in the Event:
  std::auto_ptr< reco::BasicClusterCollection > clusters_p(new reco::BasicClusterCollection);
  clusters_p->assign(clusters.begin(), clusters.end());
  edm::OrphanHandle<reco::BasicClusterCollection> bccHandle;
  if (ecalPart == IslandClusterAlgo::barrel) 
    bccHandle = evt.put(clusters_p, barrelClusterCollection_);
  else
    bccHandle = evt.put(clusters_p, endcapClusterCollection_);

  delete topology_p;
}
