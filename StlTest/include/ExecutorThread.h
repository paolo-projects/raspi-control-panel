#pragma once
#include <deque>

#include "Task.h"

/**
 * @brief A class that spawns an Executor Thread (or worker thread).
 * The thread is used for handling intensive and I/O operations without
 * burdening the main thread, which should only care about drawing stuff
*/
class ExecutorThread
{
public:

private:
	std::deque<Task> taskDeque;
};

