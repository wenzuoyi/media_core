#include <iostream>
#include "output_media_source_testsuite.h"
int main(int argc, const char* argv) {
	output::OutputMediaSourceTestSuite output_media_source_test_suite;
	output_media_source_test_suite.Init();
	output_media_source_test_suite.TestPlayAudioFile();
	int ctrl;
	std::cin >> ctrl;
	output_media_source_test_suite.Fini();
	return 0;
}