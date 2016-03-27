#ifndef __RBTREE_HPP__
#define __RBTREE_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace alg
{
enum _ERBTreeColor
{
	RBTree_Red,
	RBTree_Black,
};

template <class T>
struct _RBTreeNode
{
	_ERBTreeColor color;
	_RBTreeNode *left;
	_RBTreeNode *right;
	_RBTreeNode *parent;

	T data;

	_RBTreeNode()
			:color(RBTree_Black)
			,left(NULL)
			,right(NULL)
	{
	}
};

template <class T>
class RBTree
{
  protected:
	typedef _RBTreeNode<T> NodeType;
	NodeType *mRoot;
  public:
	typedef T value_type;
  public:	
    RBTree() : mRoot(NULL)
	{
	}
	
    virtual ~RBTree()
	{
	}

  protected:
	void insertCase1(NodeType *n)
	{
		if (n->parent == NULL)
			n->color = 
	}

	void insertCase2(NodeType *n)
	{
		
	}

	void insertCase3(NodeType *n)
	{
		
	}

	void insertCase4(NodeType *n)
	{
		
	}

	void insertCase5(NodeType *n)
	{
		
	}

	void rotateLeft(NodeType* x)
	{
		assert(x->right);

		NodeType *y = x->right;
		x->right = y->left;
	}
	void rotateRight(NodeType *n)
	{
	}

	inline NodeType* grandParent(NodeType *n)
	{
		return n->parent->parent;
	}

	inline NodeType* uncle(NodeType *n)
	{
		if (grandParent()->left == n->parent)
			return grandParent()->right;
		else
			return grandParent()->left;
	}
};
}

#endif // __RBTREE_HPP__
