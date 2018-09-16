# 算法(Algorithm)
标准库提供的算法包括：

  1. find
  1. find_if
  1. count
  1. count_if
  1. replace
  1. replace_if
  1. copy
  1. copy_if
  1. unique_copy
  1. move
  1. sort
  1. equal_range
  1. merge

这些算法多以迭代器为参数，有可能会修改元素的值，但不会增加或删除元素。
如果你需要添加元素，就需要了解容器的特性，如`back_inserter`，或是直接访问容器本身，如`push_back/erase`等。
