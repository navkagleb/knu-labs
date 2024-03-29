#ifndef OS_LAB1_SRC_PROCESS_HPP_
#define OS_LAB1_SRC_PROCESS_HPP_

#include <cstdint>
#include <functional>

namespace os_lab1 {

using ProcessId       = pid_t;
using ProcessExecutor = std::function<int()>;

namespace process {

void Kill(ProcessId id, int signal);

} // namespace process

namespace this_process {

ProcessId GetId() noexcept;
ProcessId GetParentId() noexcept;

ProcessId SpawnChild(const ProcessExecutor& process);
ProcessId SplitExecution(const ProcessExecutor& process);

void WaitForChild();
void WaitForChildren();

void Kill(int status = 0) noexcept;

} // namespace this_process

} // namespace os_lab1

#endif // OS_LAB1_SRC_PROCESS_HPP_
