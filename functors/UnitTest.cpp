#include "UnitTest.h"
#include <stdio.h>
#include <stdlib.h>

#include <numeric>
#include <vector>

CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest);

UnitTest::UnitTest()
{	
}

UnitTest::~UnitTest()
{
}
	
void UnitTest::setUp()
{
}		
	
void UnitTest::tearDown()
{
}

void UnitTest::testAccumulate()
{	
	srand(time(NULL));
	
	std::vector<int> v;
	int sum = 0;
	int count = 100;
	for (int i = 0; i < count; ++i)
	{
		int n = rand() % 10000;
		sum += n;
		v.push_back(n);
	}

	int res = 0;
	res = accumulate(v.begin(), v.end(), res);
	CPPUNIT_ASSERT(res == sum);
}
