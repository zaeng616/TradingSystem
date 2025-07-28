#include "gmock/gmock.h"
#include "test.cpp"
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}