#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"

class Test
{
  public:
    Test()
	{
		a = 0;
		b = 0;
	}
    virtual ~Test() {}
  protected:
	int a, b;
};

class UnitTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(UnitTest);
	CPPUNIT_TEST(testMisc);
	CPPUNIT_TEST(testVector);
	CPPUNIT_TEST(testRBTree);
	CPPUNIT_TEST(test_rbtree_erase);
	CPPUNIT_TEST_SUITE_END();
  public:
	UnitTest();
	virtual ~UnitTest();
  
	virtual void setUp();
  
	virtual void tearDown();

	void testMisc();

	void testVector();
	void testRBTree();
	void test_rbtree_erase();
};
