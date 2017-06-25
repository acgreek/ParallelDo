// vim: set noexpandtab
#ifndef DEADLOCK_DETECTING_THREAD_PROCESSOR_HPP
#define DEADLOCK_DETECTING_THREAD_PROCESSOR_HPP
#include "thread_processor.h"
#include "batch_processor.h"
#include <vector>
#include <unistd.h>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace ParallelDo {
class DeadLockDetectingThreadProcessor : public ThreadProcessor{
	public:
		DeadLockDetectingThreadProcessor(int max_wait_for_job = 500, int number_of_worker_threads = 0, int jobs_per_worker=5):
			ThreadProcessor(max_wait_for_job, 0 == number_of_worker_threads ?ThreadProcessor::defaultNumberOfThreads()+1 : number_of_worker_threads+1 , jobs_per_worker) {
				time_t now = time(NULL);
				last_activity_.resize(numberOfWorkerThreads(), now);
				setActionFunc(boost::bind(&DeadLockDetectingThreadProcessor::updateLastActivity, this, _1));
				monitor_.create_thread(boost::bind(&DeadLockDetectingThreadProcessor::monitorActivity, this));
		}
		virtual ~DeadLockDetectingThreadProcessor() {
			setActionFunc(NULL);
			stop();
			monitor_.interrupt_all();
			monitor_.join_all();
		}

		void updateLastActivity(int worker_id) {
			if (!isDone())
				last_activity_[worker_id] = time(NULL);
		}	

		void monitorActivity() {
			while (!isDone()) {
				boost::this_thread::sleep_for(boost::chrono::seconds{10});
				time_t now = time(NULL);
				for (unsigned i=0; isDone() && i < last_activity_.size(); i++) { 
					if ( last_activity_[i] < now && (now - last_activity_[i]) > max_inactivity_) {
						fprintf(stderr, "Thread %d has been inactive. Exiting..\n", i );
						_exit(-1);
					}
				}
			}	
		}

	private:
		int max_inactivity_ = 5 * 60; 
		std::vector<time_t> last_activity_;
		boost::thread_group monitor_;
};
}

#endif
