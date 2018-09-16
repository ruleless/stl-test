#include "rbtree.hpp"
#include "rbtree.h"
#include <gtest/gtest.h>

TEST(rbtree, test)
{
    alg::RBTree<int> t;
    srand(time(NULL));

    int insertTimes = 100;
    int maxNum = -1;
    int minNum = 65535;
    for (int i = 0; i < insertTimes; ++i) {
        t.insert(i);
        if (i > maxNum)
            maxNum = i;
        if (i < minNum)
            minNum = i;
    }
    EXPECT_EQ(t.minimun(), minNum);
    EXPECT_EQ(t.maxmun(), maxNum);
    printf("maxLevel:%d nodenum:%d\n", t.maxLevel(), t.size());

    for (int i = 0; i < insertTimes; ++i) {
        int n = rand() % 65535;
        if (n > maxNum)
            maxNum = n;
        if (n < minNum)
            minNum = n;
        t.insert(n);
    }
    EXPECT_EQ(t.minimun(), minNum);
    EXPECT_EQ(t.maxmun(), maxNum);
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

TEST(rbtree, testErase)
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
