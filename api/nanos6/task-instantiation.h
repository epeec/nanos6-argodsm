/*
	This file is part of Nanos6 and is licensed under the terms contained in the COPYING file.
	
	Copyright (C) 2015-2017 Barcelona Supercomputing Center (BSC)
*/

#ifndef NANOS6_TASK_INSTANTIATION_H
#define NANOS6_TASK_INSTANTIATION_H


#include <stddef.h>


#pragma GCC visibility push(default)

#ifdef __cplusplus
extern "C" {
#endif

//! \brief Data type to express priorities
typedef signed long nanos6_priority_t;


//! \brief This needs to be incremented every time there is an update to the nanos6_task_info::run
enum nanos6_task_execution_api_t { nanos6_task_execution_api = 1 };


//! \brief This needs to be incremented every time there is an update to nanos6_task_constraints_t
enum nanos6_task_constraints_api_t { nanos6_task_constraints_api = 1 };


typedef struct
{
	size_t cost;
} nanos6_task_constraints_t;


typedef enum
{
	nanos6_host_device=0,
	nanos6_cuda_device,
	nanos6_opencl_device,
	nanos6_device_type_num=3
} nanos6_device_t;


typedef struct
{
	size_t local_address;
	size_t device_address;
} nanos6_address_translation_entry_t;


typedef struct
{
	
	//! \brief Runtime device identifier (original type nanos6_device_t)
	int device_type_id;
	
	//! \brief Wrapper around the actual task implementation
	//! 
	//! \param[in,out] args_block A pointer to a block of data for the parameters
	//! \param[in] device_env a pointer to device-specific data
	//! \param[in] address_translation_table one entry per task symbol that maps host addresses to device addresses
	void (*run)(void *args_block, void *device_env, nanos6_address_translation_entry_t *address_translation_table);
	
	//! \brief Function to retrieve constraint information about the task (cost, memory requirements, ...)
	//! 
	//! \param[in] args_block A pointer to a block of data for the parameters
	//! \param[in,out] constraints An 
	void (*get_constraints)(void *args_block, nanos6_task_constraints_t *constraints);
	
	//! \brief A string that identifies the type of task
	char const *task_label;
	
	//! \brief A string that identifies the source location of the definition of the task
	char const *declaration_source;
	
} nanos6_task_implementation_info_t;


//! \brief This needs to be incremented every time that there is a change in nanos6_task_info
enum nanos6_task_info_contents_t { nanos6_task_info_contents = 6 };

//! \brief Struct that contains the common parts that all tasks of the same type share
typedef struct
{
	//! \brief Number of symbols accessible by the task
	int /*const*/ num_symbols; //TODO: removed const for obstructing construction, until further decision
	
	//! \brief Function that the runtime calls to retrieve the information needed to calculate the dependencies
	//! 
	//! This function should call the nanos6_register_input_dep, nanos6_register_output_dep and nanos6_register_inout_dep
	//! functions to pass to the runtime the information needed to calculate the dependencies
	//! 
	//! \param[in] args_block a pointer to a block of data for the parameters partially initialized
	//! \param[in] handler a handler to be passed on to the registration functions
	void (*register_depinfo)(void *args_block, void *handler);
	
	//! \brief Function that the runtime calls to obtain a user-specified priority for the task instance
	//! 
	//! Note that this field can be null to indicate the default priority.
	//! 
	//! \param[in] args_block a pointer to a block of data for the parameters partially initialized
	//! \param[out] priority a pointer to a block of data where the desired priority is stored
	void (*get_priority)(void *args_block, nanos6_priority_t* priority);
	
	//! \brief A string generated by the compiler that identifies the type of task
	char const *type_identifier;
	
	// \brief Number of implementations of the task
	int /*const*/ implementation_count; //TODO: removed const for obstructing construction, until further decision
	
	//! \brief Array of implementations
	nanos6_task_implementation_info_t /*const*/ *implementations; //TODO: removed const for obstructing construction, until further decision
	
	//! \brief Function that the runtime calls to perform any cleanup needed in the block of data of the parameters
	//! 
	//! \param[in,out] args_block A pointer to a block of data for the parameters
	void (*destroy)(void *args_block);

	
	//! \brief Array of functions that the runtime calls to initialize task
	//! reductions' private storage
	//!
	//! \param[out] oss_priv a pointer to the data to be initialized
	//! \param[in] oss_orig a pointer to the original data, which may be used
	//! \param[in] size the (in Bytes) size of the data to be initialized
	//! during initialization
	void (**reduction_initializers)(void *oss_priv, void *oss_orig, size_t size);
	
	//! \brief Array of functions that the runtime calls to combine task
	//! reductions' private storage
	//!
	//! \param[out] oss_out a pointer to the data where the combination is to
	//! be performed
	//! \param[in] oss_in a pointer to the data which needs to be combined
	//! \param[in] size the size (in Bytes) of the data to be combined
	void (**reduction_combiners)(void *oss_out, void *oss_in, size_t size);
} nanos6_task_info __attribute__((aligned(64)));


//! \brief Struct that contains data shared by all tasks invoked at fixed location in the source code
typedef struct
{
	//! \brief A string that identifies the source code location of the task invocation
	char const *invocation_source;
} nanos6_task_invocation_info __attribute__((aligned(64)));


//! \brief This needs to be incremented on every change to the instantiation API
enum nanos6_instantiation_api_t { nanos6_instantiation_api = 3 };

typedef enum {
	//! Specifies that the task will be a final task
	nanos6_final_task = (1 << 0),
	//! Specifies that the task is in "if(0)" mode
	nanos6_if_0_task = (1 << 1),
	//! Specifies that the task is really a taskloop
	nanos6_taskloop_task = (1 << 2),
	//! Specifies that the task has the "wait" clause
	nanos6_waiting_task = (1 << 3)
} nanos6_task_flag;


//! \brief Allocate space for a task and its parameters
//! 
//! This function creates a task and allocates space for its parameters.
//! After calling it, the user code should fill out the block of data stored in args_block_pointer,
//! and call nanos6_submit_task with the contents stored in task_pointer.
//! 
//! \param[in] task_info a pointer to the nanos6_task_info structure
//! \param[in] task_invocation_info a pointer to the nanos6_task_invocation_info structure
//! \param[in] args_block_size size needed to store the paramerters passed to the task call
//! \param[out] args_block_pointer a pointer to a location to store the pointer to the block of data that will contain the parameters of the task call
//! \param[out] task_pointer a pointer to a location to store the task handler
void nanos6_create_task(
	nanos6_task_info *task_info,
	nanos6_task_invocation_info *task_invocation_info,
	size_t args_block_size,
	/* OUT */ void **args_block_pointer,
	/* OUT */ void **task_pointer,
	size_t flags
);


//! \brief Submit a task
//! 
//! This function should be called after filling out the block of parameters of the task. See nanos6_create_task.
//! 
//! \param[in] task The task handler
void nanos6_submit_task(void *task);


#ifdef __cplusplus
}
#endif

#pragma GCC visibility pop


#endif /* NANOS6_TASK_INSTANTIATION_H */
