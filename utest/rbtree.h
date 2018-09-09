#ifndef __RBTREE_H__
#define __RBTREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>

namespace alg
{
enum _ERBTreeColor
{
    RBTree_Red,
    RBTree_Black,
};

struct _RBTreeNodeBase
{
    _ERBTreeColor color;

    _RBTreeNodeBase *left;
    _RBTreeNodeBase *right;
    _RBTreeNodeBase *parent;

    _RBTreeNodeBase()
            :color(RBTree_Red)
            ,left(NULL), right(NULL), parent(NULL)
    {}
};

static _RBTreeNodeBase* _grandParent(_RBTreeNodeBase *n)
{
    return n->parent->parent;
}

static _RBTreeNodeBase* _uncle(_RBTreeNodeBase *n)
{
    if (_RBTreeNodeBase *g = _grandParent(n))
    {
        if (g->left == n->parent)
            return g->right;
        else
            return g->left;
    }
    return NULL;
}

static _RBTreeNodeBase* _sibling(_RBTreeNodeBase *n)
{
    if (n->parent->left == n)
        return n->parent->right;
    else
        return n->parent->left;
}

static void _rotateLeft(_RBTreeNodeBase *x, _RBTreeNodeBase *&root)
{
    _RBTreeNodeBase *y = x->right;

    if (x == root)
        root = y;
    else if (x->parent->left == x)
        x->parent->left = y;
    else
        x->parent->right = y;

    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;
    x->right = y->left;
    y->left = x;
    x->parent = y;
}

static void _rotateRight(_RBTreeNodeBase *x, _RBTreeNodeBase *&root)
{
    _RBTreeNodeBase *y = x->left;

    if (root == x)
        root = y;
    else if (x->parent->left == x)
        x->parent->left = y;
    else
        x->parent->right = y;

    if (y->right)
        y->right->parent = x;
    y->parent = x->parent;
    x->left = y->right;
    y->right = x;
    x->parent = y;
}

static void _rebalance(_RBTreeNodeBase* n, _RBTreeNodeBase*& root)
{
    if (NULL == n->parent) // case 1
    {
        n->color = RBTree_Black;
        return;
    }
    if (RBTree_Black == n->parent->color) // case 2
        return;

    if (_uncle(n) && _uncle(n)->color == RBTree_Red) // case 3
    {
        _grandParent(n)->color = RBTree_Red;
        _uncle(n)->color = RBTree_Black;
        n->parent->color = RBTree_Black;
        _rebalance(_grandParent(n), root);
    }
    else
    {
        // case 4
        if (_grandParent(n)->left == n->parent && n->parent->right == n)
        {
            _rotateLeft(n->parent, root);
            n = n->left;
        }
        else if (_grandParent(n)->right == n->parent && n->parent->left == n)
        {
            _rotateRight(n->parent, root);
            n = n->right;
        }

        // case 5
        _grandParent(n)->color = RBTree_Red;
        n->parent->color = RBTree_Black;

        if (_grandParent(n)->left == n->parent)
        {
            _rotateRight(_grandParent(n), root);
        }
        else
        {
            _rotateLeft(_grandParent(n), root);
        }
    }
}

static void _rebalanceForErase(_RBTreeNodeBase* n, _RBTreeNodeBase*& root)
{
    if (root == n) // case 1
        return;

    _RBTreeNodeBase* s = _sibling(n);
    if (s->color == RBTree_Red) // case 2
    {
        n->parent->color = RBTree_Red;
        s->color = RBTree_Black;
        if (s->parent->left == s)
            _rotateRight(n->parent, root);
        else
            _rotateLeft(n->parent, root);
    }
    else if ((s->left == NULL || s->left->color == RBTree_Black) &&
             (s->right == NULL || s->right->color == RBTree_Black) &&
             n->parent->color == RBTree_Black) // case 3
    {
        s->color = RBTree_Red;
        _rebalanceForErase(n->parent, root);
    }
    else if ((s->left == NULL || s->left->color == RBTree_Black) &&
             (s->right == NULL || s->right->color == RBTree_Black) &&
             n->parent->color == RBTree_Red) // case 4
    {
        n->parent->color = RBTree_Black;
        s->color = RBTree_Red;
    }
    else
    {
        // case 5
        if ((n->parent->left == n) &&
            (s->right == NULL || s->right->color == RBTree_Black) &&
            (s->left && s->left->color == RBTree_Red))
        {
            s->color = RBTree_Red;
            s->left->color = RBTree_Black;
            _rotateRight(s, root);
        }
        else if ((n->parent->right == n) &&
                 (s->left == NULL || s->left->color == RBTree_Black) &&
                 (s->right && s->right->color == RBTree_Red))
        {
            s->color = RBTree_Red;
            s->right->color = RBTree_Black;
            _rotateLeft(s, root);
        }

        // case 6
        s = _sibling(n);
        s->color = n->parent->color;
        n->parent->color = RBTree_Black;

        if (n->parent->left == n)
        {
            s->right->color = RBTree_Black;
            _rotateLeft(n->parent, root);
        }
        else
        {
            s->left->color = RBTree_Black;
            _rotateRight(n->parent, root);
        }
    }
}

template <class T>
struct _RBTreeNode : public _RBTreeNodeBase
{
    T data;

    _RBTreeNode() : _RBTreeNodeBase()
    {}
};

template <class T>
struct Less
{
    int operator()(T a, T b) const
    {
        return a-b;
    }
};

template <class T>
struct PrintT
{
    void print(const T& v) const {}
};

template <>
struct PrintT<int>
{
    void print(const int& v) const
    {
        printf("%d", v);
    }
};

template <
    class T,
    template <class> class _Compare = Less,
    template <class> class P = PrintT >
class RBTree
{
  protected:
    typedef _RBTreeNode<T> Node;
    typedef _RBTreeNode<T>* LinkType;

    _RBTreeNodeBase *mRoot;
    int mNodeCount;

    _Compare<T> mCmp;
    P<T> mPrint;
  public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef const value_type* const_pointer;
    typedef const value_type& const_reference;
  public:
    RBTree() : mRoot(NULL), mNodeCount(0), mCmp(), mPrint() {}

    virtual ~RBTree()
    {
    }

    void insert(const value_type& v)
    {
        if (NULL == mRoot)
        {
            Node *pNew = new Node();
            pNew->color = RBTree_Black;
            pNew->left = pNew->right = pNew->parent = NULL;
            pNew->data = v;
            mRoot = pNew;
            ++mNodeCount;
        }
        else
        {
            _insert(mRoot, v);
        }
    }

    bool erase(const value_type &v)
    {
        if (NULL == mRoot)
            return false;
        return _erase(mRoot, v);
    }

    T minimun() const
    {
        if (mRoot != NULL)
            return _minimun(mRoot);
        return T(0);

    }

    T maxmun() const
    {
        if (mRoot != NULL)
        {
            return _maxmun(mRoot);
        }
        return T(0);
    }

    int count(const T& val) const
    {
        if (mRoot != NULL)
            return _count(mRoot, val);
        return 0;
    }

    int size() const
    {
        return mNodeCount;
    }

    int maxLevel() const
    {
        return _maxLevel(mRoot);
    }

    struct PrintNode
    {
        int layer;
        LinkType n;
    };
    void print() const
    {
        std::queue<PrintNode> que;
        if (mRoot)
        {
            PrintNode pn;
            pn.layer = 0;
            pn.n = (LinkType)mRoot;
            que.push(pn);
        }

        while (!que.empty())
        {
            PrintNode cur = que.front();
            que.pop();

            printf("layer:%d  ", cur.layer);
            mPrint.print(cur.n->data);
            printf("  color:%-8s", cur.n->color == RBTree_Red ? "red" : "black");
            if (cur.n->parent)
            {
                if (cur.n->parent->left == cur.n)
                    printf("left");
                else
                    printf("right");
            }
            printf("\n");

            if (cur.n->left)
            {
                PrintNode pn;
                pn.layer = cur.layer + 1;
                pn.n = (LinkType)cur.n->left;
                que.push(pn);
            }
            if (cur.n->right)
            {
                PrintNode pn;
                pn.layer = cur.layer + 1;
                pn.n = (LinkType)cur.n->right;
                que.push(pn);
            }
        }
    }
  protected:
    void _insert(_RBTreeNodeBase* n, const value_type &v)
    {
        if (mCmp(v, ((LinkType)n)->data) <= 0)
        {
            if (NULL == n->left)
            {
                Node *pNew = new Node();
                pNew->color = RBTree_Red;
                pNew->left = pNew->right = NULL;
                pNew->parent = n;
                pNew->data = v;
                n->left = pNew;
                ++mNodeCount;

                _rebalance(pNew, mRoot);
            }
            else
            {
                _insert(n->left, v);
            }
        }
        else
        {
            if (NULL == n->right)
            {
                Node *pNew = new Node();
                pNew->color = RBTree_Red;
                pNew->left = pNew->right = NULL;
                pNew->parent = n;
                pNew->data = v;
                n->right = pNew;
                ++mNodeCount;

                _rebalance(pNew, mRoot);
            }
            else
            {
                _insert(n->right, v);
            }
        }
    }

    bool _erase(_RBTreeNodeBase* n, const value_type &v)
    {
        int cmp = mCmp(v, ((LinkType)n)->data);
        if (cmp < 0)
        {
            if (NULL == n->left)
                return false;
            return _erase(n->left, v);
        }
        else if (cmp > 0)
        {
            if (NULL == n->right)
                return false;
            return _erase(n->right, v);
        }
        else
        {
            if (n->right)
            {
                _RBTreeNodeBase *smallestNode = _getSmallestNode(n->right);
                ((LinkType)n)->data = ((LinkType)smallestNode)->data;
               _eraseForOneChild(smallestNode);
            }
            else
            {
                _eraseForOneChild(n);
            }
            --mNodeCount;
            return true;
        }
    }

    void _eraseForOneChild(_RBTreeNodeBase *n)
    {
        _RBTreeNodeBase* child = n->left ? n->left : n->right;
        if (n->parent == NULL)
        {
            if (child)
            {
                child->color = RBTree_Black;
                child->parent = NULL;
            }
            mRoot = child;
            delete n;
            return;
        }

        if (n->parent->left == n)
            n->parent->left = child;
        else
            n->parent->right = child;
        if (child)
            child->parent = n->parent;

        if (n->color == RBTree_Black)
        {
            if (child)
            {
                if (child->color == RBTree_Red)
                    child->color = RBTree_Black;
                else
                    _rebalanceForErase(child, mRoot);
            }
            else
            {
                _rebalanceForErase(n->parent, mRoot);
            }
        }
        delete n;
    }

    _RBTreeNodeBase* _getSmallestNode(_RBTreeNodeBase *n) const
    {
        if (NULL == n->left)
            return n;
        return _getSmallestNode(n->left);
    }

    T _minimun(const _RBTreeNodeBase* n) const
    {
        if (NULL == n->left)
        {
            return ((LinkType)n)->data;
        }
        return _minimun(n->left);
    }

    T _maxmun(const _RBTreeNodeBase* n) const
    {
        if (NULL == n->right)
        {
            return ((LinkType)n)->data;
        }
        return _maxmun(n->right);
    }

    int _count(const _RBTreeNodeBase *n, const T& val) const
    {
        if (NULL == n)
            return 0;

        int cmp = mCmp(val, ((LinkType)n)->data);
        if (cmp < 0)
            return _count(n->left, val);
        else if (cmp > 0)
            return _count(n->right, val);
        else
            return 1+_count(n->left, val)+_count(n->right, val);
    }

    int _maxLevel(const _RBTreeNodeBase *node) const
    {
        if (NULL == node)
            return 0;

        int leftLevel = _maxLevel(node->left);
        int rightLevel = _maxLevel(node->right);
        int res = leftLevel > rightLevel ? leftLevel : rightLevel;
        return res+1;
    }
};
}

#endif // __RBTREE_H__
