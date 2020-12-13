#pragma once
#include <functional>

/**
 * @brief A task is an object that wraps the code to run on an worker thread
*/
class Task
{
public:
	Task(std::function<void(std::function<void(void)> completionCallback)> runner);
private:

};

