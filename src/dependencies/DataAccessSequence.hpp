#ifndef DATA_ACCESS_SEQUENCE_HPP
#define DATA_ACCESS_SEQUENCE_HPP

#include <mutex>
#include <boost/intrusive/list.hpp>

#include "DataAccessRange.hpp"
#include "DataAccessSequenceLinkingArtifacts.hpp"
#include "DataAccessType.hpp"
#include "lowlevel/SpinLock.hpp"

#include <InstrumentDataAccessSequenceId.hpp>


class Task;
struct DataAccess;


struct DataAccessSequence {
	//! The range of data covered by the accesses of this sequence
	DataAccessRange _accessRange;
	
	//! \brief Access originated by the direct parent to the tasks of this access sequence
	DataAccess *_superAccess;
	
	//! \brief SpinLock to protect the sequence and all its subaccesses.
	SpinLock *_lock;
	
	typedef boost::intrusive::list<DataAccess, boost::intrusive::function_hook<DataAccessSequenceLinkingArtifacts>> access_sequence_t;
	
	//! \brief Ordered sequence of accesses to the same location
	access_sequence_t _accessSequence;
	
	//! An identifier for the instrumentation
	Instrument::data_access_sequence_id_t _instrumentationId;
	
	
	DataAccessSequence() = delete;
	inline DataAccessSequence(SpinLock *lock);
	inline DataAccessSequence(DataAccessRange accessRange, SpinLock *lock);
	inline DataAccessSequence(DataAccessRange accessRange, DataAccess *superAccess, SpinLock *lock);
	
	
	//! \brief Get a locking guard
	inline std::unique_lock<SpinLock> getLockGuard();
	
	
	//! \brief Get the Effective Previous access of another given one
	//! 
	//! \param[in] dataAccess the DataAccess that is effectively after the one to be returned or nullptr if the DataAccess is yet to be added and the sequence is empty
	//! 
	//! \returns the Effective Previous access to the one passed by parameter, or nullptr if there is none
	//! 
	//! NOTE: This function assumes that the whole hierarchy has already been locked
	inline DataAccess *getEffectivePrevious(DataAccess *dataAccess);
	
	
};


#include "DataAccess.hpp"
#include "DataAccessSequenceLinkingArtifactsImplementation.hpp"


#endif // DATA_ACCESS_SEQUENCE_HPP
