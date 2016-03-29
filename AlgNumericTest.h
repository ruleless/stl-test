#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"

class AlgNumericTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(AlgNumericTest);
	CPPUNIT_TEST(test_accumulate);
	CPPUNIT_TEST_SUITE_END();
  public:
    AlgNumericTest();
    virtual ~AlgNumericTest();
	
	virtual void setUp();
	
	virtual void tearDown();

	void test_accumulate();
};
