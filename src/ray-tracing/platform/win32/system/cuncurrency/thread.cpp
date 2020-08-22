#include "system/concurrency/thread.h"

#include <Windows.h>
#include <assert.h>

#include <map>

struct win32_routine_data {
	thread_procedure* procedure;
	void* args;
};

std::map<DWORD, HANDLE> thread_handle_map;
std::map<DWORD, win32_routine_data*> thread_id_to_win32_routine_data;

static inline tid from_dword_to_tid(DWORD val) {
	return (unsigned int)val;
}

static inline DWORD from_tid_to_dword(tid val) {
	return (DWORD)val;	
}

static HANDLE get_thread_native_handle(tid thread_id) {
	DWORD dwId = from_tid_to_dword(thread_id);

	if (thread_handle_map.find(dwId) != thread_handle_map.end()) {
		return thread_handle_map[thread_id];
	}

	return nullptr;
}

static win32_routine_data* get_routine_data(tid id) {
	DWORD dwId = from_tid_to_dword(id);
	if (thread_id_to_win32_routine_data.find(dwId) != thread_id_to_win32_routine_data.end()) {
		return thread_id_to_win32_routine_data[dwId];
	}

	return nullptr;
}

static void free_routine_data(tid id) { 
	win32_routine_data* data = get_routine_data(id);
	thread_id_to_win32_routine_data.erase(id);

	BOOL result = HeapFree(GetProcessHeap(), 0, data);
	if (!result) {
		//todo: log.
	}
}

int hardware_concurrency() {
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int numCPU = sysinfo.dwNumberOfProcessors;
	return numCPU;
}

static DWORD WINAPI Win32ThreadRoutine(LPVOID lpThreadParameter) 
{
	assert(lpThreadParameter);
	win32_routine_data* routine = (win32_routine_data*)lpThreadParameter;
	routine->procedure(routine->args);
	return 1;
}

tid create_thread(thread_procedure* procedure, void* procedure_args, bool is_running) {
	DWORD creation_flag = is_running ? 0 : CREATE_SUSPENDED;

	win32_routine_data* routine_data = (win32_routine_data*)HeapAlloc(GetProcessHeap(), 0, sizeof(win32_routine_data));

	routine_data->procedure = procedure;
	routine_data->args = procedure_args;

	DWORD win32_thread_id;
	HANDLE win32_thread = CreateThread(0, 0, Win32ThreadRoutine, routine_data, creation_flag, &win32_thread_id);

	if (win32_thread == INVALID_HANDLE_VALUE) {
		//todo: fuck! | log!
		assert(false);
	}

	thread_handle_map[win32_thread_id] = win32_thread;
	thread_id_to_win32_routine_data[win32_thread_id] = routine_data;

	return from_dword_to_tid(win32_thread_id);
}

void suspend_thread(tid thread_id) {
	HANDLE thread_handle = get_thread_native_handle(thread_id);
	if (thread_handle) {
		SuspendThread(thread_handle);
	}
	else {
		//todo: log.
	}
}

void resume_thread(tid thread_id) {
	HANDLE thread_handle = get_thread_native_handle(thread_id);
	if (thread_handle) {
		ResumeThread(thread_handle);
	}
	else {
		//todo: log.
	}
}

void join_thread(tid thread_id) {
	HANDLE native_handle = get_thread_native_handle(thread_id);
	if (native_handle) {
		DWORD result = WaitForSingleObject(native_handle, INFINITE);
		assert(SUCCEEDED(result));
	}
	else {
		assert(false);
	}
}
/*
	`Not fully implemented.`
	doesn't realy close thread.
	just close the handle.
*/
void close_thread(tid thread_id) {
	HANDLE native_handle = get_thread_native_handle(thread_id);
	if (native_handle) {
		BOOL result = CloseHandle(native_handle);
		free_routine_data(thread_id);
		
		if (!result) {
			assert(result);
		}
	}
	else {
		assert(native_handle);
		//todo: log native_handle not found
	}
}