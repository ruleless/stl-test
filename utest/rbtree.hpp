#ifndef __RBTREE_HPP__
#define __RBTREE_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace test
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
    int mNodeCount;
  public:
    typedef T value_type;
  public:   
    RBTree() : mRoot(NULL), mNodeCount(0)
    {
    }
    
    virtual ~RBTree()
    {
    }

    void insert(const T& val)
    {
        ++mNodeCount;
        NodeType *newNode = new NodeType();
        newNode->data = val;
        newNode->left = newNode->right = NULL;
        newNode->parent = NULL;     

        if (NULL == mRoot)
        {
            mRoot = newNode;
            mRoot->color = RBTree_Black;
            return;
        }

        NodeType *cur = mRoot;
        while (1)
        {
            if (val <= cur->data)
            {
                if (NULL == cur->left)
                {
                    newNode->color = RBTree_Red;
                    cur->left = newNode;
                    newNode->parent = cur;
                    insertCase1(newNode);
                    break;
                }
                else
                    cur = cur->left;
            }
            else
            {
                if (NULL == cur->right)
                {
                    newNode->color = RBTree_Red;
                    cur->right = newNode;
                    newNode->parent = cur;
                    insertCase1(newNode);
                    break;
                }
                else
                    cur = cur->right;
            }
        }
    }

    bool erase(const T& t)
    {
        if (NULL == mRoot)
            return false;
        return deleteChild(mRoot, t);
    }

    int size() const
    {
        return mNodeCount;
    }

    int count(const T& val) const
    {
        if (mRoot != NULL)
            return _count(mRoot, val);
        return 0;
    }
    int _count(const NodeType *n, const T& val) const
    {
        if (NULL == n)
            return 0;
        if (val < n->data)
            return _count(n->left, val);
        else if (val > n->data)
            return _count(n->right, val);
        else
            return 1+_count(n->left, val)+_count(n->right, val);
    }

    T maxmun() const
    {
        NodeType *cur = mRoot, *exNode = mRoot;
        while (cur)
        {
            exNode = cur;
            cur = cur->right;
        }
        if (exNode)
        {
            return exNode->data;
        }
        else
        {
            return (T)(0);
        }
    }

    T minimun() const
    {
        NodeType *cur = mRoot, *exNode = mRoot;
        while (cur)
        {
            exNode = cur;
            cur = cur->left;
        }
        if (exNode)
        {
            return exNode->data;
        }
        else
        {
            return (T)(0);
        }
    }

    int maxLevel() const
    {
        return _maxLevel(mRoot);
    }   

    int _maxLevel(NodeType *node) const
    {
        if (NULL == node)
            return 0;

        int leftLevel = _maxLevel(node->left);
        int rightLevel = _maxLevel(node->right);
        int res = leftLevel > rightLevel ? leftLevel : rightLevel;
        return res+1;
    }   
  protected:
    void insertCase1(NodeType *n)
    {
        if (n->parent == NULL)
            n->color = RBTree_Black;
        else
            insertCase2(n);
    }
    void insertCase2(NodeType *n)
    {
        if (n->parent->color == RBTree_Black)
            return;
        else
            insertCase3(n);
    }
    void insertCase3(NodeType *n)
    {
        if (uncle(n) && uncle(n)->color == RBTree_Red)
        {
            grandParent(n)->color = RBTree_Red;
            uncle(n)->color = RBTree_Black;
            n->parent->color = RBTree_Black;
            insertCase1(grandParent(n));
        }
        else
        {
            insertCase4(n);
        }
    }
    void insertCase4(NodeType *n)
    {
        NodeType *x = n;
        if (grandParent(n)->left == n->parent && n->parent->right == n)
        {
            rotateLeft(n->parent);
            x = n->left;
        }
        else if (grandParent(n)->right == n->parent && n->parent->left == n)
        {
            rotateRight(n->parent);
            x = n->right;
        }
        insertCase5(x);
    }
    void insertCase5(NodeType *n)
    {
        grandParent(n)->color = RBTree_Red;
        n->parent->color = RBTree_Black;
        
        if (grandParent(n)->left == n->parent)
        {
            rotateRight(grandParent(n));
        }
        else
        {
            rotateLeft(grandParent(n));
        }
    }
    
    void rotateLeft(NodeType* x)
    {
        NodeType *y = x->right;     
        if (y->left != NULL)
            y->left->parent = x;

        if (mRoot == x)
            mRoot = y;
        else if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
        
        y->parent = x->parent;
        x->right = y->left;
        y->left = x;
        x->parent = y;      
    }
    void rotateRight(NodeType *x)
    {
        NodeType *y = x->left;

        if (mRoot == x)
            mRoot = y;
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

    bool deleteChild(NodeType *n, T t)
    {
        if (t < n->data)
        {
            if (NULL == n->left)
                return false;
            return deleteChild(n->left, t);
        }
        else if (t > n->data)
        {
            if (NULL == n->right)
                return false;
            return deleteChild(n->right, t);
        }
        else
        {
            if (n->right)
            {
                NodeType *smallestNode = getSmallestNode(n->right);
                n->data = smallestNode->data;
                deleteOneChild(smallestNode);
            }
            else
            {
                deleteOneChild(n);              
            }
            --mNodeCount;
            return true;
        }
    }

    NodeType* getSmallestNode(NodeType *n)
    {
        if (NULL == n->left)
            return n;
        return getSmallestNode(n->left);
    }

    void deleteOneChild(NodeType *n)            
    {
        NodeType* child = n->left ? n->left : n->right;
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
                    deleteCase1(child);
            }
            else
            {
                deleteCase1(n->parent);
            }
        }
        delete n;
    }

    void deleteCase1(NodeType *n)
    {
        if (n != mRoot)
            deleteCase2(n);
    }
    void deleteCase2(NodeType *n)
    {
        NodeType* s = sibling(n);
        if (s->color == RBTree_Red)
        {
            n->parent->color = RBTree_Red;
            s->color = RBTree_Black;
            if (s->parent->left == s)
                rotateRight(n->parent);
            else
                rotateLeft(n->parent);
        }
        deleteCase3(n);
    }
    void deleteCase3(NodeType *n)
    {
        NodeType *s = sibling(n);
        if (s->color == RBTree_Black &&
            (s->left == NULL || s->left->color == RBTree_Black) &&
            (s->right == NULL || s->right->color == RBTree_Black) &&
            n->parent->color == RBTree_Black)
        {
            s->color = RBTree_Red;
            deleteCase1(n->parent);
        }
        else
        {
            deleteCase4(n);
        }
    }
    void deleteCase4(NodeType *n)
    {
        NodeType *s = sibling(n);
        if (s->color == RBTree_Black &&
            (s->left == NULL || s->left->color == RBTree_Black) &&
            (s->right == NULL || s->right->color == RBTree_Black) &&
            n->parent->color == RBTree_Red)
        {
            n->parent->color = RBTree_Black;
            s->color = RBTree_Red;
        }
        else
        {
            deleteCase5(n);
        }
    }
    void deleteCase5(NodeType *n)
    {
        NodeType *s = sibling(n);
        if ((n->parent->left == n) &&
            (s->color == RBTree_Black &&
             (s->right == NULL || s->right->color == RBTree_Black) &&
             (s->left && s->left->color == RBTree_Red)))
        {
            s->color = RBTree_Red;
            s->left->color = RBTree_Black;
            rotateRight(s);
        }
        else if ((n->parent->right == n) &&
                 (s->color == RBTree_Black &&
                  (s->left == NULL || s->left->color == RBTree_Black) &&
                  (s->right && s->right->color == RBTree_Red)))
        {
            s->color = RBTree_Red;
            s->right->color = RBTree_Black;
            rotateLeft(s);
        }
        deleteCase6(n);
    }
    void deleteCase6(NodeType *n)
    {
        NodeType *s = sibling(n);
        s->color = n->parent->color;
        n->parent->color = RBTree_Black;

        if (n->parent->left == n)
        {
            s->right->color = RBTree_Black;
            rotateLeft(n->parent);
        }
        else
        {
            s->left->color = RBTree_Black;
            rotateRight(n->parent);
        }
    }
    
    inline NodeType* grandParent(NodeType *n)
    {
        return n->parent->parent;
    }

    inline NodeType* uncle(NodeType *n)
    {
        if (grandParent(n)->left == n->parent)
            return grandParent(n)->right;
        else
            return grandParent(n)->left;
    }

    inline NodeType* sibling(NodeType *n)
    {
        if (n->parent->left == n)
            return n->parent->right;
        else
            return n->parent->left;
    }
};
}

#endif // __RBTREE_HPP__
