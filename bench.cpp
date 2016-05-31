#include <stdio.h>
#include "thread_processor.h"
#include "batch_processor.h"
int k;

void func(int i) {
    int j = i;
    for(int z=0; z< 1000; z++ ){
        j+=z;
    }
    k +=j;
}
int main(int argc, char * argv[]) {
    if (argc < 1) {
        fprintf(stderr, "need one arg\n");

        return EXIT_FAILURE;
    }
	ParallelDo::ThreadProcessor testProcessor(500, 2);

	ParallelDo::BatchTracker jq(&testProcessor);
    for (int i=0; i< atoi(argv[1]); i++) {
        jq.post(boost::bind(&func, i));
    }
	jq.wait_until_done();
    return EXIT_SUCCESS;
}
