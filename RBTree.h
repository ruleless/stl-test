#ifndef __RBTREE_H__
#define __RBTREE_H__

namespace alg
{
enum _ERBTreeColor
{
	RBTree_Red,
	RBTree_Black,
};

template <class _Key, class _Value>
struct _RBTreeNode
{
	_ERBTreeColor color;
	
	_RBTreeNode *left;
	_RBTreeNode *right;
	_RBTreeNode *parent;

	_Key key;
	_Value val;

	_RBTreeNode()
			:color(RBTree_Black)
			,left(NULL)
			,right(NULL)
			,parent(NULL)
	{
	}
};

template <class _Key, class _Value, class _Compare>
class RBTree
{
  protected:
	typedef _RBTreeNode<_Key, _Value> Node;
	typedef _Compare<_Key> Cmp;
	
	Node *mRoot;
	int mNodeCount;

	Cmp mCmp;
  public:
	typedef _Key key_type;
	typedef _Value value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;	
  public:
    RBTree() : mRoot(NULL), mNodeCount(0), mCmp() {}
	
    virtual ~RBTree()
	{		
	}

	void insert(const key_type &key, const value_type &val)
	{}
	
	void erase(const key_type &key)
	{}
  protected:
	void _insert(Node* n, const key_type &key, const value_type &val)
	{}
			
	void _erase(Node* n, const key_type)
	{}
};

}

#endif // __RBTREE_H__
