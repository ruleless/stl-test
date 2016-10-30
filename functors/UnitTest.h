#ifndef __UNITTEST_H__
#define __UNITTEST_H__

#include "cppunit/TestFixture.h"
#include "cppunit/extensions/HelperMacros.h"

class UnitTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(UnitTest);
	CPPUNIT_TEST(testAccumulate);
	CPPUNIT_TEST_SUITE_END();
  public:
    UnitTest();
    virtual ~UnitTest();
	
	virtual void setUp();
	
	virtual void tearDown();

	void testAccumulate();
};

#endif // __UNITTEST_H__
