import FWCore.ParameterSet.Config as cms

hltEgammaEcalRecIsolation = cms.EDProducer( "EgammaHLTEcalRecIsolationProducer",
                                                        recoEcalCandidateProducer = cms.InputTag( "hltL1NonIsoRecoEcalCandidate" ),
                                                        ecalBarrelRecHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
                                                        ecalBarrelRecHitCollection = cms.InputTag("EcalRecHitsEB"),
                                                        ecalEndcapRecHitProducer = cms.InputTag("hltEcalRegionalEgammaRecHit"),
                                                        ecalEndcapRecHitCollection = cms.InputTag("EcalRecHitsEE"),

                                                        intRadiusBarrel = cms.double(0.045),
                                                        intRadiusEndcap = cms.double(0.07),
                                                        extRadius = cms.double(0.3),
                                                        etMinBarrel = cms.double(-9999),
                                                        eMinBarrel = cms.double(0.08),
                                                        etMinEndcap = cms.double(-9999),
                                                        eMinEndcap = cms.double(0.3),
                                                        jurassicWidth = cms.double(0.02),  #dEta strip width

                                                        useIsolEt = cms.bool(True),
                                                        tryBoth   = cms.bool(True),
                                                        subtract  = cms.bool(False),
                                                    )