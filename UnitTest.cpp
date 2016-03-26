#include "UnitTest.h"
#include "vector.h"

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

void UnitTest::testVoid()
{
}

void UnitTest::testVector()
{
	std::vector<int> v;
	v.push_back(1);
}
