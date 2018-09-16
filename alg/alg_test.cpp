#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <list>
#include <algorithm>
#include <gtest/gtest.h>

typedef std::list<int> LIST;
typedef std::vector<int> VECTOR;

static const int kMinNum = 1;
static const int kMaxNum = 100;

static inline int genN()
{
    return random() % (kMaxNum - kMinNum) + kMinNum;
}

class AlgTest : public testing::Test
{
  public:
    void SetUp()
    {
        srand(time(NULL));
        int n = genN();
        for (int i = 0; i < n; i++) {
            list_.push_back(genN());
        }

        n = genN();
        array_.reserve(n);
        for (int i = 1; i <= n; i++) {
            array_.push_back(i);
        }
    }

    void TearDown() {}

    LIST list_;
    VECTOR array_;
};

TEST_F(AlgTest, testFind)
{
    // for_each
    std::for_each(list_.begin(), list_.end(),
                  [&](int x) {
                      printf("%d ", x);
                  });
    puts("");

    // std::find
    auto found = std::find(list_.begin(), list_.end(), genN());
    if (found != list_.end()) {
        printf("std::find() found %d\n", *found);
    } else {
        printf("std::find() found no %d\n", *found);
    }

    // std::find_if
    int n = genN();
    found = std::find_if(list_.begin(), list_.end(),
                         [&](int i) {
                             return i == n;
                         });
    if (found != list_.end()) {
        printf("std::find_if() found %d\n", *found);
    } else {
        printf("std::find_if() found no %d\n", *found);
    }
}

TEST_F(AlgTest, testShuffle)
{
    puts("before shuffle:");
    std::for_each(array_.begin(), array_.end(),
                  [](int x) {
                      printf("%d ", x);
                  });
    puts("");

    std::random_shuffle(array_.begin(), array_.end());
    puts("after shuffle:");
    std::for_each(array_.begin(), array_.end(),
                  [](int x) {
                      printf("%d ", x);
                  });
    puts("");
}
