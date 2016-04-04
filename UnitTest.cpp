#include "UnitTest.h"
#include "RBTree.hpp"
#include "RBTree.h"
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

static const Test& returnTest()
{
	return Test();
}

void UnitTest::testMisc()
{
	Test a = returnTest();
}

void UnitTest::testVector()
{
	std::vector<int> v;
	
	v.push_back(1);
}

void UnitTest::testRBTree()
{
	alg::RBTree<int> t;
	srand(time(NULL));

	int insertTimes = 100;
	int maxNum = -1;
	int minNum = 65535;
	for (int i = 0; i < insertTimes; ++i)
	{
		t.insert(i);
		if (i > maxNum)
			maxNum = i;
		if (i < minNum)
			minNum = i;
	}
	CPPUNIT_ASSERT(t.minimun() == minNum);
	CPPUNIT_ASSERT(t.maxmun() == maxNum);
	printf("maxLevel:%d nodenum:%d\n", t.maxLevel(), t.size());
	
	for (int i = 0; i < insertTimes; ++i)
	{
		int n = rand() % 65535;
		if (n > maxNum)
			maxNum = n;
		if (n < minNum)
			minNum = n;
		t.insert(n);
	}
	CPPUNIT_ASSERT(t.minimun() == minNum);
	CPPUNIT_ASSERT(t.maxmun() == maxNum);
	printf("maxLevel:%d nodenum:%d\n", t.maxLevel(), t.size());

	alg::RBTree<int> rbt;
	rbt.insert(30);
	rbt.insert(20);
	rbt.insert(40);
	rbt.insert(50);
	rbt.print();
	
	rbt.erase(30);
	rbt.print();
	rbt.erase(20);
	rbt.print();
	rbt.erase(40);
	rbt.print();
	rbt.erase(50);
	rbt.print();
}

void UnitTest::test_rbtree_erase()
{
	alg::RBTree<int> t;
	t.insert(80);
	t.insert(60);
	t.insert(30);
	t.insert(98);
	t.insert(69);
	t.insert(10);
	t.insert(100);
	t.insert(70);

	int testNum[] = {
		80, 81, 60, 98, 30, 70, 100, 10, 69,
	};
	printf("\nrbtree size=%d\n", t.size());
	for (int i = 0; i < sizeof(testNum) / sizeof(int); ++i)
	{
		int n = testNum[i];
		printf("%d's count=%d erase=%d and count=%d\n",
			   n, t.count(n), t.erase(n), t.count(n));
	}
	printf("rbtree size=%d\n", t.size());
}
