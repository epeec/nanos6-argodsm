#include <cassert>

#include <nanos6/blocking.h>

#include "executors/threads/ThreadManager.hpp"
#include "executors/threads/WorkerThread.hpp"

#include "scheduling/Scheduler.hpp"


extern "C" void *nanos_get_current_blocking_context()
{
	WorkerThread *currentWorkerThread = WorkerThread::getCurrentWorkerThread();
	assert(currentWorkerThread != nullptr);
	
	Task *task = currentWorkerThread->getTask();
	assert(task != nullptr);
	
	return task;
}


extern "C" void nanos_block_current_task(__attribute__((unused)) void *blocking_context)
{
	WorkerThread *currentWorkerThread = WorkerThread::getCurrentWorkerThread();
	assert(currentWorkerThread != nullptr);
	
	CPU *cpu = nullptr;
	cpu = currentWorkerThread->getHardwarePlace();
	assert(cpu != nullptr);
	
	WorkerThread *replacementThread = ThreadManager::getIdleThread(cpu);
	
	ThreadManager::switchThreads(currentWorkerThread, replacementThread);
}


extern "C" void nanos_unblock_task(void *blocking_context)
{
	Task *task = static_cast<Task *>(blocking_context);
	
	Scheduler::taskGetsUnblocked(task, nullptr);
}
