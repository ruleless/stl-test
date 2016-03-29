# test for SGI STL

## Iterators

### 概要

迭代器是一种行为类似指针的对象，而指针的各种行为中最常见也最重要的就是 dereference
和 member access，因此，迭代器最重要的编程工作就是对 operator* 和 operator-> 进行重载。

### 迭代器的设计

如果可以针对所有数据容器设计一个通用的迭代器自然是我们最好的选择，然而，这是不可能做到的。
考虑数组和双向链表：针对数组而言， 我们可以通过 `typedef T* iterator;` 定义迭代器；
而双向链表的迭代器则不可能有这么精简的结构。

由于容器的多样性，不同的容器往往需要不同的迭代算法。将迭代器的实现交给容器本身，
才是最合理的。

虽然迭代器的实现交由容器本身实现，但各容器不能随便定义接口迥异的迭代器。
为保证迭代器的访问一致性，我们需要为迭代器实现一组一致的行为。

所有迭代器必须定义以下内嵌型别：

``` c++
template <class _Category, class _Tp, class _Distance = ptrdiff_t,
          class _Pointer = _Tp*, class _Reference = _Tp&>
struct iterator {
  typedef _Category  iterator_category;
  typedef _Tp        value_type;
  typedef _Distance  difference_type;
  typedef _Pointer   pointer;
  typedef _Reference reference;
};
```

所有迭代器必须重载以下运算符：

``` c++
template<class _Tp, class _Ref, class _Ptr>
struct _Iterator {
  typedef _Iterator<_Tp,_Tp&,_Tp*>             iterator;
  typedef _Iterator<_Tp,const _Tp&,const _Tp*> const_iterator;
  typedef _Iterator<_Tp,_Ref,_Ptr>             _Self;

  reference operator*() const;
  pointer operator->() const;

  _Self& operator++();
  _Self operator++(int);
  _Self& operator--();
  _Self operator--(int);

  bool operator==(const _Iterator& __x) const;
  bool operator!=(const _Iterator& __x) const;
};
```

双向链表的迭代器实现示例：

``` c++
struct _List_iterator_base {
  typedef size_t                     size_type;
  typedef ptrdiff_t                  difference_type;
  typedef bidirectional_iterator_tag iterator_category;

  _List_node_base* _M_node;

  _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
  _List_iterator_base() {}

  void _M_incr() { _M_node = _M_node->_M_next; }
  void _M_decr() { _M_node = _M_node->_M_prev; }

  bool operator==(const _List_iterator_base& __x) const {
    return _M_node == __x._M_node;
  }
  bool operator!=(const _List_iterator_base& __x) const {
    return _M_node != __x._M_node;
  }
};

template<class _Tp, class _Ref, class _Ptr>
struct _List_iterator : public _List_iterator_base {
  typedef _List_iterator<_Tp,_Tp&,_Tp*>             iterator;
  typedef _List_iterator<_Tp,const _Tp&,const _Tp*> const_iterator;
  typedef _List_iterator<_Tp,_Ref,_Ptr>             _Self;

  typedef _Tp value_type;
  typedef _Ptr pointer;
  typedef _Ref reference;
  typedef _List_node<_Tp> _Node;

  _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
  _List_iterator() {}
  _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}

  reference operator*() const { return ((_Node*) _M_node)->_M_data; }

#ifndef __SGI_STL_NO_ARROW_OPERATOR
  pointer operator->() const { return &(operator*()); }
#endif /* __SGI_STL_NO_ARROW_OPERATOR */

  _Self& operator++() {
    this->_M_incr();
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    this->_M_incr();
    return __tmp;
  }
  _Self& operator--() {
    this->_M_decr();
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    this->_M_decr();
    return __tmp;
  }
};
```

### 型别萃取

迭代器型别萃取指的是：萃取迭代器所指向对象的型别。我们为迭代器定义了一组一致的内嵌型别，
直接通过域运算符获取不就可以了吗？

我们之前说过：迭代器是一种行为类似指针的对象。既然如此，那么原生指针算不算迭代器？
算！事实上，vector 的迭代器就是原生指针。

所以，在迭代器没有专门实现，而是纯粹指针的情况下，我们不能利用一致的接口来萃取型别。
于是就有了如下的“型别萃取机”：

``` c++
template <class _Iterator>
struct iterator_traits {
  typedef typename _Iterator::iterator_category iterator_category;
  typedef typename _Iterator::value_type        value_type;
  typedef typename _Iterator::difference_type   difference_type;
  typedef typename _Iterator::pointer           pointer;
  typedef typename _Iterator::reference         reference;
};

template <class _Tp>
struct iterator_traits<_Tp*> {
  typedef random_access_iterator_tag iterator_category;
  typedef _Tp                         value_type;
  typedef ptrdiff_t                   difference_type;
  typedef _Tp*                        pointer;
  typedef _Tp&                        reference;
};

template <class _Tp>
struct iterator_traits<const _Tp*> {
  typedef random_access_iterator_tag iterator_category;
  typedef _Tp                         value_type;
  typedef ptrdiff_t                   difference_type;
  typedef const _Tp*                  pointer;
  typedef const _Tp&                  reference;
};
```

### 迭代器的一些通用函数

`distance` 求两个迭代器之间的“距离”

``` c++
template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type
__distance(_InputIterator __first, _InputIterator __last, input_iterator_tag)
{
  typename iterator_traits<_InputIterator>::difference_type __n = 0;
  while (__first != __last) {
    ++__first; ++__n;
  }
  return __n;
}

template <class _RandomAccessIterator>
inline typename iterator_traits<_RandomAccessIterator>::difference_type
__distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
           random_access_iterator_tag) {
  __STL_REQUIRES(_RandomAccessIterator, _RandomAccessIterator);
  return __last - __first;
}

template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type
distance(_InputIterator __first, _InputIterator __last) {
  typedef typename iterator_traits<_InputIterator>::iterator_category
    _Category;
  __STL_REQUIRES(_InputIterator, _InputIterator);
  return __distance(__first, __last, _Category());
}
```

`advance` 迭代器步进函数

``` c++
template <class _BidirectionalIterator, class _Distance>
inline void __advance(_BidirectionalIterator& __i, _Distance __n,
                      bidirectional_iterator_tag) {
  __STL_REQUIRES(_BidirectionalIterator, _BidirectionalIterator);
  if (__n >= 0)
    while (__n--) ++__i;
  else
    while (__n++) --__i;
}

template <class _RandomAccessIterator, class _Distance>
inline void __advance(_RandomAccessIterator& __i, _Distance __n,
                      random_access_iterator_tag) {
  __STL_REQUIRES(_RandomAccessIterator, _RandomAccessIterator);
  __i += __n;
}

template <class _InputIterator, class _Distance>
inline void advance(_InputIterator& __i, _Distance __n) {
  __STL_REQUIRES(_InputIterator, _InputIterator);
  __advance(__i, __n, iterator_category(__i));
}
```

## Containers

### 序列式容器

---

#### vector

##### 概要

vector 是实现为单向开口的连续线性空间容器。

---

#### list

##### 概要

list 是双向链表容器。

---

#### deque

##### 概要

deque 是实现为双向开口的连续线性空间容器。

---

### 关联式容器

---

#### RB Tree(红黑树)

##### 概要

红黑树是满足下列条件的二叉搜索树：

  1. 每个节点不是红色，就是黑色
  2. 根节点为黑色
  3. 红色节点的子节点必须为黑色
  4. 任一节点至叶子节点的任何路径，所含之黑节点数必须相等

---

## Algorithms

## Adapters

## Functors

### 概要

仿函数的主要作用在哪里？STL 提供的各种算法，往往有两个版本。
其中一个表现出最常用（或最直观）的某种运算；另一个则表现出泛化的运算流程，
允许用户以 template 参数来指定所要采用的策略。以 accumulate 为例，
其一般行为是将指定范围内的所有元素相加，泛化版本则允许你指定某种“操作”，
取代第一个版本中的“相加”行为。

据上所述，函数指针似乎能代替仿函数达到同样的目的，那又何必用所谓的仿函数呢？
原因在于函数指针毕竟不能满足 STL 对抽象性的要求，也不能满足软件积木的要求：
函数指针无法和 STL 的其他组件（如 Adapter）搭配，产生更灵活的变化。

### 仿函数基础类

stl 定义了两个基础的仿函数接口，它们定义了标准的接受一元或二元参数的仿函数内嵌型别。

``` c++
template <class _Arg, class _Result>
struct unary_function {
  typedef _Arg argument_type;
  typedef _Result result_type;
};

template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
  typedef _Arg1 first_argument_type;
  typedef _Arg2 second_argument_type;
  typedef _Result result_type;
};
```

### 数值运算仿函数

``` c++
template <class _Tp>
struct plus : public binary_function<_Tp,_Tp,_Tp> {
  _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x + __y; }
};

template <class _Tp>
struct minus : public binary_function<_Tp,_Tp,_Tp> {
  _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x - __y; }
};

template <class _Tp>
struct multiplies : public binary_function<_Tp,_Tp,_Tp> {
  _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x * __y; }
};

template <class _Tp>
struct divides : public binary_function<_Tp,_Tp,_Tp> {
  _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x / __y; }
};

template <class _Tp>
struct modulus : public binary_function<_Tp,_Tp,_Tp>
{
  _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x % __y; }
};

template <class _Tp>
struct negate : public unary_function<_Tp,_Tp>
{
  _Tp operator()(const _Tp& __x) const { return -__x; }
};
```

### 关系运算仿函数

``` c++
template <class _Tp>
struct equal_to : public binary_function<_Tp,_Tp,bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x == __y; }
};

template <class _Tp>
struct not_equal_to : public binary_function<_Tp,_Tp,bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x != __y; }
};

template <class _Tp>
struct greater : public binary_function<_Tp,_Tp,bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x > __y; }
};

template <class _Tp>
struct less : public binary_function<_Tp,_Tp,bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp>
struct greater_equal : public binary_function<_Tp,_Tp,bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x >= __y; }
};

template <class _Tp>
struct less_equal : public binary_function<_Tp,_Tp,bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x <= __y; }
};
```

### 逻辑运算仿函数

``` c++
template <class _Tp>
struct logical_and : public binary_function<_Tp,_Tp,bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x && __y; }
};

template <class _Tp>
struct logical_or : public binary_function<_Tp,_Tp,bool>
{
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x || __y; }
};

template <class _Tp>
struct logical_not : public unary_function<_Tp,bool>
{
  bool operator()(const _Tp& __x) const { return !__x; }
};
```

## Allocators
