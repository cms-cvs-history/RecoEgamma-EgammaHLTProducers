import FWCore.ParameterSet.Config as cms

from RecoTracker.CkfPattern.CkfTrajectoryBuilderESProducer_cff import *
import copy
from RecoTracker.CkfPattern.CkfTrajectoryBuilderESProducer_cfi import *
#TrajectoryBuilder
TrajectoryBuilderForPixelMatchElectronsL1NonIso = copy.deepcopy(CkfTrajectoryBuilder)
# Navigation School record
from RecoTracker.TkNavigation.NavigationSchoolESProducer_cff import *
import copy
from RecoEgamma.EgammaHLTProducers.pixelMatchElectronsForHLT_cfi import *
#pixelMatchedElectrons
pixelMatchElectronsL1NonIsoForHLT = copy.deepcopy(pixelMatchElectronsForHLT)
TrajectoryBuilderForPixelMatchElectronsL1NonIso.ComponentName = 'TrajectoryBuilderForPixelMatchElectronsL1NonIso'
TrajectoryBuilderForPixelMatchElectronsL1NonIso.estimator = 'egammaHLTChi2'
pixelMatchElectronsL1NonIsoForHLT.TrackProducer = 'ctfL1NonIsoWithMaterialTracks'
pixelMatchElectronsL1NonIsoForHLT.TrajectoryBuilder = 'TrajectoryBuilderForPixelMatchElectronsL1NonIso'
