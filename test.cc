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

TEST(ParallelForEach_Vector) {
	ThreadProcessor testProcessor(10);
	std::vector<int > foo;
	foo.push_back(5);
	foo.push_back(6) ;
	foo.push_back(7) ;
	foo.push_back(8) ;
	foo.push_back(9) ;
	AssertEqInt(foo[0], 5);
	ParallelForEach (&testProcessor, &func2, foo.begin(), foo.end());
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
	ParallelForEach (&testProcessor, &func2, foo.begin(), foo.end());
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
	ParallelForEach (&testProcessor, &func2, foo, &foo[5]);
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
	int sum = ParallelCompute(&testProcessor,24, &add, foo.begin(),foo.end());
	AssertEqInt(sum,24); 
}
TEST(ParallelComputeOne) {
	ThreadProcessor testProcessor;
	std::vector<int > foo;
	foo.push_back(5);
	int sum = ParallelCompute(&testProcessor,0, &add, foo.begin(),foo.end());
	AssertEqInt(sum,5); 
}
TEST(ParallelComputeTwo) {
	ThreadProcessor testProcessor;
	std::vector<int > foo;
	foo.push_back(5);
	foo.push_back(6);
	int sum = ParallelCompute(&testProcessor,0, &add, foo.begin(),foo.end());
	AssertEqInt(sum,11); 
}
TEST(ParallelComputeThree) {
	ThreadProcessor testProcessor;
	std::vector<int > foo;
	foo.push_back(5);
	foo.push_back(6);
	foo.push_back(7);
	int sum = ParallelCompute(&testProcessor,0, &add, foo.begin(),foo.end());
	AssertEqInt(sum,18); 
}
TEST(ParallelCompute) {
	ThreadProcessor testProcessor(10);
	int foo[5];
	foo[0]=1;
	foo[1]=2;
	foo[2]=3;
	foo[3]=4;
	foo[4]=5;
	int sum = ParallelCompute(&testProcessor,0, &add, foo, &foo[5]);
	AssertEqInt(sum,15); 
}


#ifdef __CYGWIN__ 
int main (int argc, char * argv[]){
	return windows_main(argc, argv);
}
#endif
