#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"


#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTElectronTrackIsolationProducers.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTElectronDetaDphiProducer.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTRecoEcalCandidateProducers.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTPixelMatchElectronProducers.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTRegionalPixelSeedGeneratorProducers.h"

#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTClusterShapeProducer.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTHybridClusterProducer.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTIslandClusterProducer.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTMulti5x5ClusterProducer.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTRemoveDuplicatedSC.h"

#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTHcalIsolationProducersRegional.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTEcalIsolationProducersRegional.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTHcalIsolationDoubleConeProducers.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EgammaHLTPhotonTrackIsolationProducersRegional.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EcalListOfFEDSProducer.h"
#include "RecoEgamma/EgammaHLTProducers/interface/EcalRecHitsMerger.h"


DEFINE_SEAL_MODULE();
DEFINE_FWK_MODULE(EgammaHLTElectronTrackIsolationProducers);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTElectronDetaDphiProducer);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTRecoEcalCandidateProducers);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTPixelMatchElectronProducers);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTRegionalPixelSeedGeneratorProducers);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTClusterShapeProducer);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTHybridClusterProducer);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTIslandClusterProducer);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTMulti5x5ClusterProducer);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTEcalIsolationProducersRegional);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTHcalIsolationProducersRegional);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTHcalIsolationDoubleConeProducers);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTPhotonTrackIsolationProducersRegional);
DEFINE_ANOTHER_FWK_MODULE(EgammaHLTRemoveDuplicatedSC);
DEFINE_ANOTHER_FWK_MODULE(EcalListOfFEDSProducer);
DEFINE_ANOTHER_FWK_MODULE(EcalRecHitsMerger);
