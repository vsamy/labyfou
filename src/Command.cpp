#include "Command.h"

void CommandQueue::push(const Command & command)
{
	queue_.push(command);
}

Command CommandQueue::pop()
{
	Command nextCommand = queue_.front();
	queue_.pop();
	return nextCommand;
}

bool CommandQueue::isEmpty() const
{
	return queue_.empty();
}
