/* -----------------------------------------------------------------------------
Description: I Task source file
Author: Daniel
Version:
    v0.1 - first implementation 
----------------------------------------------------------------------------- */
#include "i_task.hpp"

namespace hrd31
{

ITask::ITask(Priority priority_) : m_priority(priority_) {}

ITask::~ITask() {}

bool ITask::operator<(const ITask& other_) const
{
    return m_priority < other_.m_priority;
}


}

