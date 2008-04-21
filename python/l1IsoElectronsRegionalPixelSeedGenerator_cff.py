import FWCore.ParameterSet.Config as cms

import copy
from RecoEgamma.EgammaHLTProducers.hltEgammaRegionalPixelSeedGenerator_cfi import *
#include "RecoEgamma/EgammaHLTProducers/data/hltEgammaRegionalPixelSeedGenerator.cfi"
l1IsoElectronsRegionalPixelSeedGenerator = copy.deepcopy(hltEgammaRegionalPixelSeedGenerator)
l1IsoElectronsRegionalPixelSeedGenerator.candTag = 'l1IsoRecoEcalCandidate'
l1IsoElectronsRegionalPixelSeedGenerator.candTagEle = 'pixelMatchElectronsL1IsoForHLT'
l1IsoElectronsRegionalPixelSeedGenerator.UseZInVertex = True
l1IsoElectronsRegionalPixelSeedGenerator.originHalfLength = 0.5
