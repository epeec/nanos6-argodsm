#ifndef __EXECUTION_WORKFLOW_HOST_HPP__
#define __EXECUTION_WORKFLOW_HOST_HPP__

#include "ExecutionStep.hpp"
#include <functional>

class MemoryPlace;
class ComputePlace;
class Task;
class DataAccess;

namespace ExecutionWorkflow {
	class HostAllocationAndPinningStep : public Step {
	public:
		HostAllocationAndPinningStep(
			__attribute__((unused)) RegionTranslation &regionTranslation,
			__attribute__((unused)) MemoryPlace const *memoryPlace
		) : Step()
		{
		}
		
		//! Start the execution of the Step
		void start();
	};
	
	class HostDataLinkStep : public Step {
	public:
		HostDataLinkStep(__attribute__((unused)) MemoryPlace const *sourceMemoryPlace,
			__attribute__((unused)) MemoryPlace const *targetMemoryPlace,
			__attribute__((unused)) DataAccess const *access,
			__attribute__((unused)) RegionTranslation const &targetTranslation,
			__attribute__((unused)) Task *task) :
		Step()
		{
		}
		
		//! start the execution of the Step
		void start();
	};
	
	class HostExecutionStep : public Step {
		Task *_task;
		ComputePlace *_computePlace;
	public:
		HostExecutionStep(__attribute__((unused)) Task *task,
			__attribute__((unused)) ComputePlace *computePlace)
			: Step(), _task(task), _computePlace(computePlace)
		{
		}
		
		//! Start the execution of the Step
		void start();
	};
	
	class HostNotificationStep : public Step {
		std::function<void ()> const _callback;
	public:
		HostNotificationStep(
			__attribute__((unused)) std::function<void ()> const &callback
		) : _callback(callback)
		{
		}
		
		//! start the execution of the Step
		void start();
	};
	
	class HostUnpinningStep : public Step {
	public:
		HostUnpinningStep(
			__attribute__((unused)) MemoryPlace const *targetMemoryPlace,
			__attribute__((unused)) RegionTranslation const &targetTranslation
		) : Step()
		{
		}
		
		//! start the execution of the Step
		void start();
	};
};

#endif /* __EXECUTION_WORKFLOW_HOST_HPP__ */