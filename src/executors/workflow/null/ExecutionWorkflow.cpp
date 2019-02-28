#include <ExecutionWorkflow.hpp>
#include "executors/threads/WorkerThread.hpp"
#include "executors/threads/TaskFinalization.hpp"
#include "hardware/places/MemoryPlace.hpp"
#include "hardware/places/ComputePlace.hpp"
#include "tasks/Task.hpp"

#include <DataAccessRegistration.hpp>

#include <InstrumentTaskExecution.hpp>
#include <InstrumentTaskStatus.hpp>
#include <InstrumentInstrumentationContext.hpp>
#include <InstrumentThreadInstrumentationContext.hpp>
#include <InstrumentThreadInstrumentationContextImplementation.hpp>
#include <InstrumentThreadManagement.hpp>
#include <instrument/support/InstrumentThreadLocalDataSupport.hpp>



namespace ExecutionWorkflow {
	void executeTask(
		Task *task,
		ComputePlace *targetComputePlace,
		__attribute__((unused))MemoryPlace *targetMemoryPlace
	) {
		WorkerThread *currentThread = WorkerThread::getCurrentWorkerThread();
		assert(currentThread != nullptr);
		CPU *cpu = (CPU *)targetComputePlace;
		
		task->setThread(currentThread);
		Instrument::task_id_t taskId = task->getInstrumentationTaskId();
		
		Instrument::ThreadInstrumentationContext instrumentationContext(
			taskId,
			cpu->getInstrumentationId(),
			currentThread->getInstrumentationId()
		);
		
		if (task->hasCode()) {
			nanos6_address_translation_entry_t *translationTable = nullptr;
			
			nanos6_task_info_t const * const taskInfo = task->getTaskInfo();
			if (taskInfo->num_symbols >= 0) {
				translationTable = (nanos6_address_translation_entry_t *)
						alloca(
							sizeof(nanos6_address_translation_entry_t)
							* taskInfo->num_symbols
						);
				
				for (int index = 0; index < taskInfo->num_symbols; index++) {
					translationTable[index] = {0, 0};
				}
			}
			
			Instrument::startTask(taskId);
			Instrument::taskIsExecuting(taskId);
			
			// Run the task
			std::atomic_thread_fence(std::memory_order_acquire);
			task->body(nullptr, translationTable);
			std::atomic_thread_fence(std::memory_order_release);
			
			Instrument::taskIsZombie(taskId);
			Instrument::endTask(taskId);
		}
		
		// Update the CPU since the thread may have migrated
		cpu = currentThread->getComputePlace();
		instrumentationContext.updateComputePlace(cpu->getInstrumentationId());
		
		if (task->markAsFinished(cpu)) {
			DataAccessRegistration::unregisterTaskDataAccesses(
				task,
				cpu,
				cpu->getDependencyData()
			);
			
			if (task->markAsReleased()) {
				TaskFinalization::disposeOrUnblockTask(task, cpu);
			}
		}
	}
}