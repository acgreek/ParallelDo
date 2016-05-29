#include <stdio.h>
#include "thread_processor.hpp"
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
	ThreadProcessor testProcessor(500, 2);

	BatchTracker jq(&testProcessor);
    for (int i=0; i< atoi(argv[1]); i++) {
        jq.post(boost::bind(&func, i));
    }
	jq.wait_until_done();
    return EXIT_SUCCESS;
}
