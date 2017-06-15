#pragma once
#include <boost/utility.hpp>

namespace ParallelDo {
class ThreadProcessor_interface {
	public:
		typedef boost::function<void ()> work_t;

		virtual ~ThreadProcessor_interface() {}
		virtual void set_done() = 0;
		virtual void post(work_t func) = 0;
		virtual void postWorkList(std::list<work_t> &worklist) = 0;
		virtual int queued() const = 0;
		virtual int pending_or_processing() const = 0;
		virtual int numberOfWorkerThreads () const = 0;

		static int defaultNumberOfThreads() {
			return boost::thread::hardware_concurrency();
		}
};

}
