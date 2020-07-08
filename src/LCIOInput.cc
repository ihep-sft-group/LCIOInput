#include "LCIOInput.h"
#include "FWCore/DataWrapper.h"

#include "edm4hep/MCParticleCollection.h"
#include "edm4hep/SimTrackerHitCollection.h"
#include "edm4hep/TPCHitCollection.h"
#include "edm4hep/TrackerHitCollection.h"
#include "edm4hep/TrackCollection.h"
#include "edm4hep/SimCalorimeterHitCollection.h"
#include "edm4hep/RawCalorimeterHitCollection.h"
#include "edm4hep/CalorimeterHitCollection.h"
#include "edm4hep/ClusterCollection.h"
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/MCRecoTrackerAssociationCollection.h"
#include "edm4hep/MCRecoCaloAssociationCollection.h"
#include "edm4hep/MCRecoParticleAssociationCollection.h"

#include "GaudiKernel/IEventProcessor.h"

DECLARE_COMPONENT(LCIOInput)

LCIOInput::LCIOInput(const std::string& name, ISvcLocator* svcLoc)
    : GaudiAlgorithm(name, svcLoc),
      m_nEvents(0)
{
    declareProperty("inputs", m_files = {}, "Names of the files to read");
    declareProperty("input", m_file = "", "Names of the file to read");
}

StatusCode LCIOInput::initialize()
{
    if (GaudiAlgorithm::initialize().isFailure()) return StatusCode::FAILURE;

    if ( ! m_file.empty() ) {
        m_files.push_back(m_file);
    }

    std::vector<std::string> colNames;

    for ( const auto& col : m_collections ) {
        std::string colName = col.first;
        std::string colType = col.second;

        if ( colType == "MCParticle" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::MCParticleCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        else if ( colType == "SimTrackerHit" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::SimTrackerHitCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        else if ( colType == "TPCHit" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::TPCHitCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        else if ( colType == "TrackerHit" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::TrackerHitCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        else if ( colType == "Track" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::TrackCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        else if ( colType == "SimCalorimeterHit" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::SimCalorimeterHitCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        else if ( colType == "RawCalorimeterHit" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::RawCalorimeterHitCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        else if ( colType == "CalorimeterHit" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::CalorimeterHitCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        else if ( colType == "Cluster" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::ClusterCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        else if ( colType == "ReconstructedParticle" ) {
            m_dataHandles[colName] =
                new DataHandle<edm4hep::ReconstructedParticleCollection>(colName, Gaudi::DataHandle::Writer, this);
        }
        //TODO: more types to be added, such as LCRelation, Vertex ...
        //else if ( colType == "LCRelation" ) {
        //    //m_dataHandles[colName] =
        //    //    new DataHandle<edm4hep::FIXME>(colName, Gaudi::DataHandle::Writer, this);
        //}
        else {
            error() << "invalid collection type: " << colType << endmsg;
            return StatusCode::FAILURE;
        }

        colNames.push_back(colName);
    }

    m_lcioReader.open(m_files);
    m_lcioReader.setReadCollectionNames(colNames);

    m_incidentSvc = service( "IncidentSvc", true  );

    return StatusCode::SUCCESS;
}

StatusCode LCIOInput::execute()
{
    m_lcioReader.readNextEvent();

    if ( m_lcioReader.isValid() ) {
        for ( const auto& col : m_collections ) {
            std::string colName = col.first;
            std::string colType = col.second;

            if ( colType == "MCParticle" ) {
                registCollection<edm4hep::MCParticleCollection>(colName);
            }
            else if ( colType == "SimTrackerHit" ) {
                registCollection<edm4hep::SimTrackerHitCollection>(colName);
            }
            else if ( colType == "SimCalorimeterHit" ) {
                registCollection<edm4hep::SimCalorimeterHitCollection>(colName);
            }
            else if ( colType == "Track" ) {
                registCollection<edm4hep::TrackCollection>(colName);
            }
        }

        ++m_nEvents;
    }
    else {
        info() << "reach end of input files" << endmsg;
        m_incidentSvc->fireIncident( Incident( name(), IncidentType::AbortEvent  )  );
        auto ep = serviceLocator()->as<IEventProcessor>();
        ep->stopRun();
    }

    return StatusCode::SUCCESS;
}

StatusCode LCIOInput::finalize()
{
  info() << "totally read " << m_nEvents << " events" << endmsg;

  m_lcioReader.close();

  for ( auto& dh : m_dataHandles ) {
      delete dh.second;
  }
  m_dataHandles.clear();

  return GaudiAlgorithm::finalize();
}
