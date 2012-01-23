#ifndef RECOEGAMMA_EGAMMAHLTPRODUCERS_EGAMMAHLTGSFTRACKVARPRODUCER
#define RECOEGAMMA_EGAMMAHLTPRODUCERS_EGAMMAHLTGSFTRACKVARPRODUCER


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateTransform.h"
#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateMode.h"
#include "RecoEgamma/EgammaElectronAlgos/interface/ElectronUtilities.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
//#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

class EgammaHLTGsfTrackVarProducer : public edm::EDProducer {
private:
  class TrackExtrapolator {
    
    
    unsigned long long cacheIDTDGeom_;
    unsigned long long cacheIDMagField_;
    
    edm::ESHandle<MagneticField> magField_;
    edm::ESHandle<TrackerGeometry> trackerHandle_;
    
    MultiTrajectoryStateMode mtsMode_; 
    const MultiTrajectoryStateTransform * mtsTransform_; //we own it
  

  public:
    TrackExtrapolator():cacheIDTDGeom_(0),cacheIDMagField_(0),mtsTransform_(0){}
    TrackExtrapolator(const TrackExtrapolator& rhs);
    ~TrackExtrapolator(){delete mtsTransform_;}
    TrackExtrapolator* operator=(const TrackExtrapolator& rhs);
    
 
    void setup(const edm::EventSetup& iSetup);
    
    GlobalPoint extrapolateTrackPosToPoint(const reco::GsfTrack& gsfTrack,const GlobalPoint& pointToExtrapTo);
    GlobalVector extrapolateTrackMomToPoint(const reco::GsfTrack& gsfTrack,const GlobalPoint& pointToExtrapTo);
    
    edm::ESHandle<TrackerGeometry> trackerGeomHandle()const{return trackerHandle_;}
    const MultiTrajectoryStateTransform * mtsTransform()const{return mtsTransform_;}
    const MultiTrajectoryStateMode* mtsMode()const{return &mtsMode_;}
  };

public:
  explicit EgammaHLTGsfTrackVarProducer(const edm::ParameterSet&);
  ~EgammaHLTGsfTrackVarProducer();
  
  
  virtual void produce(edm::Event&, const edm::EventSetup&); 

private:
  
private:
  // ----------member data ---------------------------
  edm::InputTag recoEcalCandTag_;
  edm::InputTag inputCollectionTag_;
  edm::InputTag beamSpotTag_;
  TrackExtrapolator trackExtrapolator_;
};

#endif
