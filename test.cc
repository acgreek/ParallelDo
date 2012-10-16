#include<ExtremeCUnit.h>
#include "thread_processor.hpp"
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

TEST(ThreadProcessorQueue)	 
{
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
TEST(ThreadProcessorQueue4)	 
{
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
TEST(ThreadProcessorQueue2Mixed)	 
{
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
TEST(ParallelForEach) {
	ThreadProcessor testProcessor(10);
	std::vector<int > foo;
	foo.push_back(5);
	foo.push_back(6) ;
	foo.push_back(7) ;
	foo.push_back(8) ;
	foo.push_back(9) ;
	AssertEqInt(foo[0], 5);
	ParallelForEach (&testProcessor, &func2, foo);
	AssertEqInt(foo[0], 6);
	return 0;
}

#ifdef __CYGWIN__ 
int main (int argc, char * argv[]){
	return windows_main(argc, argv);
}
#endif
