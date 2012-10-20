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

template <typename T> 
void setResultWrapper( T func(T & i, T & j), T & a, T & b , T & result) {
	result = func(a,b);
	
}

template <typename T, typename K > 
T ParallelCompute(ThreadProcessor *threadProcessorp, T initial,  T func(T & i, T & j), K begin, K end) {
	T result = initial;
	if (begin == end)
		return result;
	K next = begin;
	next++;
	if (next == end) // only 1 element given
		return *begin;

	std::list<T> result_set;
	BatchTracker jq(threadProcessorp);
	while (begin != end) {
		T & first = *begin; 
		begin++;
		result_set.push_back(first);
		if (begin == end) {// last elemnent doesn't have a any other 
			break; // are done mapping 
		}
		void (*temp_func) (T func(T & i, T & j), T & a, T & b , T & result) = &setResultWrapper<T>;
		jq.post(boost::bind(temp_func,func, boost::ref(first), boost::ref(*begin), boost::ref(result_set.back()) )); 
		begin++;
	}
	jq.wait_until_done();
	return ParallelCompute(threadProcessorp, initial, func, result_set.begin(), result_set.end());


}


#endif
