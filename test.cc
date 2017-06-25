#include<ExtremeCUnit.h>
#include "thread_processor.h"
#include "batch_processor.h"
#include "continuous_stream.h"
#include "deadlock_detecting_thread_processor.h"
using namespace ParallelDo;

#include "parallel_for_each.hpp"


void func(int &i) {
	i++;
}
TEST(ThreadProcessor1) {
	int i=0;
	ThreadProcessor testProcessor(200, 1);
	testProcessor.post(boost::bind(&func, boost::ref(i)));
	while (testProcessor.queued())
		sleep(1);
	AssertEqInt(i, 1);
	return 0;
}


void func2(int & i) {
	i++;
}

TEST(ThreadProcessorQueue) {
	int i=0, j=0;
	ThreadProcessor testProcessor(10);

	BatchTracker jq(&testProcessor);
	jq.post(boost::bind(&func2, boost::ref(i)));
	jq.wait_until_done();
	AssertEqInt(i, 1);
	jq.reset();
	i=j=0;
	jq.post(boost::bind(&func2, boost::ref(i)));
	jq.post(boost::bind(&func2, boost::ref(i)));
	jq.post(boost::bind(&func2, boost::ref(i)));
	jq.post(boost::bind(&func2, boost::ref(i)));
	jq.wait_until_done();
	AssertEqInt(i, 4);
	return 0;
}
TEST(ThreadProcessorQueue4) {
	int i=0;
	ThreadProcessor testProcessor(10);
	BatchTracker jq(&testProcessor);
	jq.post(boost::bind(&func2, boost::ref(i)));
	jq.post(boost::bind(&func2, boost::ref(i)));
	jq.post(boost::bind(&func2, boost::ref(i)));
	jq.post(boost::bind(&func2, boost::ref(i)));
	jq.wait_until_done();
	AssertEqInt(i, 4);
	return 0;
}
TEST(ThreadProcessorQueue2Mixed) {
	int i=0, j=0;
	ThreadProcessor testProcessor(10);
	BatchTracker jq(&testProcessor);
	BatchTracker jq2(&testProcessor);
	jq.post(boost::bind(&func2,  boost::ref(i)));
	jq2.post(boost::bind(&func2, boost::ref(j)));
	jq.post(boost::bind(&func2,  boost::ref(i)));
	jq2.post(boost::bind(&func2, boost::ref(j)));
	jq.post(boost::bind(&func2,  boost::ref(i)));
	jq2.post(boost::bind(&func2, boost::ref(j)));
	jq.post(boost::bind(&func2,  boost::ref(i)));
	jq.post(boost::bind(&func2,  boost::ref(i)));
	jq2.post(boost::bind(&func2, boost::ref(j)));
	jq.wait_until_done();
	jq2.wait_until_done();
	AssertEqInt(i, 5);
	AssertEqInt(j, 4);
	return 0;
}
TEST(ThreadProcessorQueue2MixedWorkList) {
	int i=0, j=0;
	{
		ThreadProcessor testProcessor(10);
		BatchTracker jq2(&testProcessor);
		std::list<ThreadProcessor::work_t> wlist;
		wlist.push_back(boost::bind(&func2,  boost::ref(i)));
		jq2.post(boost::bind(&func2, boost::ref(j)));
		wlist.push_back(boost::bind(&func2,  boost::ref(i)));
		jq2.post(boost::bind(&func2, boost::ref(j)));
		wlist.push_back(boost::bind(&func2,  boost::ref(i)));
		jq2.post(boost::bind(&func2, boost::ref(j)));
		wlist.push_back(boost::bind(&func2,  boost::ref(i)));
		wlist.push_back(boost::bind(&func2,  boost::ref(i)));
		testProcessor.postWorkList(wlist);
		jq2.post(boost::bind(&func2, boost::ref(j)));
		jq2.wait_until_done();
	}
	// don't have to sleep because last job is on a batch processor

	AssertEqInt(i, 5);
	AssertEqInt(j, 4);
	return 0;
}

TEST(ParallelForEach_Vector) {
	ThreadProcessor testProcessor(10);
	std::vector<int > foo;
	foo.push_back(5);
	foo.push_back(6) ;
	foo.push_back(7) ;
	foo.push_back(8) ;
	foo.push_back(9) ;
	AssertEqInt(foo[0], 5);
	ParallelDo::forEach (&testProcessor, &func2, foo.begin(), foo.end());
	AssertEqInt(foo[0], 6);
	AssertEqInt(foo[4], 10);
	return 0;
}

TEST(ParallelForEach_list) {
	ThreadProcessor testProcessor(10);
	std::list<int > foo;
	foo.push_back(5);
	foo.push_back(6) ;
	foo.push_back(7) ;
	foo.push_back(8) ;
	foo.push_back(9) ;
	AssertEqInt(foo.front(), 5);
	ParallelDo::forEach (&testProcessor, &func2, foo.begin(), foo.end());
	AssertEqInt(foo.front(), 6);
	AssertEqInt(foo.back(), 10);
	return 0;
}

TEST(ParallelForEach_carray) {
	ThreadProcessor testProcessor(10);
	int foo[5];
	foo[0]=5;
	foo[1]=6;
	foo[2]=7;
	foo[3]=8;
	foo[4]=9;
	AssertEqInt(foo[0], 5);
	ParallelDo::forEach (&testProcessor, &func2, foo, &foo[5]);
	AssertEqInt(foo[0], 6);
	AssertEqInt(foo[1], 7);
	AssertEqInt(foo[2], 8);
	AssertEqInt(foo[3], 9);
	AssertEqInt(foo[4], 10);
	return 0;
}
int add( int &a, int& b) {
	return a+b;
}
TEST(ParallelComputeEmpty) {
	ThreadProcessor testProcessor(10);
	std::vector<int > foo;
	int sum = ParallelDo::compute(&testProcessor,24, &add, foo.begin(),foo.end());
	AssertEqInt(sum,24);
	return 0;
}
TEST(ParallelComputeOne) {
	ThreadProcessor testProcessor;
	std::vector<int > foo;
	foo.push_back(5);
	int sum = ParallelDo::compute(&testProcessor,0, &add, foo.begin(),foo.end());
	AssertEqInt(sum,5);
	return 0;
}
TEST(ParallelComputeTwo) {
	ThreadProcessor testProcessor;
	std::vector<int > foo;
	foo.push_back(5);
	foo.push_back(6);
	int sum = ParallelDo::compute(&testProcessor,0, &add, foo.begin(),foo.end());
	AssertEqInt(sum,11);
	return 0;
}
TEST(ParallelComputeThree) {
	ThreadProcessor testProcessor;
	std::vector<int > foo;
	foo.push_back(5);
	foo.push_back(6);
	foo.push_back(7);
	int sum = ParallelDo::compute(&testProcessor,0, &add, foo.begin(),foo.end());
	AssertEqInt(sum,18);
	return 0;
}
TEST(ParallelCompute) {
	ThreadProcessor testProcessor(10);
	int foo[5];
	foo[0]=1;
	foo[1]=2;
	foo[2]=3;
	foo[3]=4;
	foo[4]=5;
	int sum =  ParallelDo::compute(&testProcessor,0, &add, foo, &foo[5]);
	AssertEqInt(sum,15);
	return 0;
}

TEST(DeadLockDetection) {
	DeadLockDetectingThreadProcessor testProcessor(10);
	int foo[5];
	foo[0]=1;
	foo[1]=2;
	foo[2]=3;
	foo[3]=4;
	foo[4]=5;
	int sum = ParallelDo::compute(&testProcessor,0, &add, foo, &foo[5]);
	AssertEqInt(sum,15);
	return 0;
}

/*
int c_sum = 0;
int continuous_inc=0;

void inc(int & i, int j) {
	continuous_inc++;
	i += j;
}


void scheduleMore(ContinousStream * continousStream) {

	if (continuous_inc > 1000)
		return ;

	for (int i=0; i < 10; i++)  {
		continousStream->post(boost::bind(&inc, boost::ref(c_sum), i));
		if (i == 5) {
			continousStream->checkForMoreWork();
		}
	}
}

TEST(ContinuousTest) {
	ThreadProcessor testProcessor(200, 1);
	ContinousStream continousStream(&testProcessor);
	continousStream.run(scheduleMore);
	continousStream.wait_until_done();
	AssertEqInt(1000, c_sum);
	return 0;
}
*/

#ifdef __CYGWIN__
int main (int argc, char * argv[]){
	return windows_main(argc, argv);
}
#endif

