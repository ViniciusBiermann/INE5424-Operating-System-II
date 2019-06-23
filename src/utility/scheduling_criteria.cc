// EPOS CPU Affinity Scheduler Component Implementation

#include <utility/scheduler.h>
#include <time.h>

__BEGIN_UTIL

// Class attributes
volatile unsigned int Scheduling_Criteria::Variable_Queue::_next_queue;


// The following Scheduling Criteria depend on Alarm, which is not available at scheduler.h
namespace Scheduling_Criteria {
    FCFS::FCFS(int p): Priority((p == IDLE) ? IDLE : Alarm::elapsed()) {}

    HRRN::HRRN(int p, const Microsecond d ): Priority(p), _deadline(d), _time_created(Alarm::elapsed()) {}
    void HRRN::update() {
    	RTC::Microsecond waitTime = Alarm::elapsed() - _time_created + 1;
    	(_priority == IDLE or _priority <= MAIN) ? _priority = _priority : 1 + waitTime/_deadline;
    }
    
};

__END_UTIL
