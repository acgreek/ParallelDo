#ifndef PARALLEL_FOR_EACH_HPP
#define PARALLEL_FOR_EACH_HPP

#include "thread_processor.hpp"
#include "boost/foreach.hpp"

template <typename T, typename K > 
void ParallelForEach (ThreadProcessor *threadProcessorp, void func(T & i), K begin, K end) {
	BatchTracker jq(threadProcessorp);
	while (begin != end) {
		jq.post(boost::bind(func,  boost::ref(*begin))); 
		begin++;
	}
	jq.wait_until_done();
}

#endif
