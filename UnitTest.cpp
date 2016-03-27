#include "UnitTest.h"
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest);

UnitTest::UnitTest()
{}

UnitTest::~UnitTest()
{}
  
void UnitTest::setUp()
{
}
  
void UnitTest::tearDown()
{
}

void UnitTest::testVector()
{
	std::vector<int> v;
	v.push_back(1);
}
