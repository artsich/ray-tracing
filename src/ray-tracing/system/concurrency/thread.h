#pragma once

typedef unsigned int tid;

#define THREAD_PROCEDURE(name) int name(void* args)
typedef THREAD_PROCEDURE(thread_procedure);

tid create_thread(thread_procedure* procedure, void* args, bool is_running);

void suspend_thread(tid thread_id);
void resume_thread(tid thread_id);
void join_thread(tid thread_id);
void close_thread(tid thread_id);

int hardware_concurrency();
