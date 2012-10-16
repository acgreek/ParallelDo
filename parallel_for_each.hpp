#ifndef PARALLEL_FOR_EACH_HPP
#define PARALLEL_FOR_EACH_HPP

#include "thread_processor.hpp"
#include "boost/foreach.hpp"

template <typename T, typename K > 
void ParallelForEach (ThreadProcessor *threadProcessorp, void func(T & i), K & values ) {

	BatchTracker jq(threadProcessorp);
	BOOST_FOREACH(T &i, values) {
		jq.post(boost::bind(func,  boost::ref(i))); 
	}
	jq.wait_until_done();
}

#endif
