/***********************************************************************
 * Component:
 *    Assignment 09, Binary Search Tree (BST)
 *    Brother JonesL, CS 235
 * Author:
*    Adam Goff, Aaron Rook, Martin Melerio, Tanner Stratford, Allan Marina
 * Summary:
 *    Create a binary search tree
 ************************************************************************/

#ifndef BST_H
#define BST_H

namespace custom
{
template<class T>
class BST
{
private:
	class BNode;
	void deleteBinaryTree(BNode*& node)
	{
		if (node == NULL)
		{
			return;
		}

		deleteBinaryTree(node->pLeft);
		deleteBinaryTree(node->pRight);
		delete node;
		node = NULL;
	}

	BNode* root;
	int numElements;

	// Helper function to use recursively
	void privateInsert(T t, BNode*& root);

public:
	BST()
	{
		numElements = 0;
		root = NULL;
	}

	BST(const BST<T>& rhs);

	~BST()
	{
		clear();
	}
	BST<T>& operator = (const BST<T>& rhs);

	int size() const { return numElements; }
	bool empty() const { return size() == NULL; }

	void clear()
	{
		if (root)
			deleteBinaryTree(root);
		numElements = 0;
	}

	BNode* getRoot() { return root; }

	void insert(const T& t);

	class iterator;
	iterator begin();
	iterator end() { return new iterator(); }
	iterator find(const T& value);
	//rBegin and rEnd are for right child,
	//not for reverse_iterator
	iterator rbegin();
	iterator rend() { return new iterator(); }

	/*class reverse_iterator;
	reverse_iterator rbegin() { return new iterator(root); }
	reverse_iterator rend() { return new iterator(); }*/

	void erase(iterator it);
};

/**************************************************
* BST insert / Public & Private
*************************************************/
template<class T>
void BST<T>::insert(const T& t)
{
	privateInsert(t, this->root);
	numElements++;
}

template <class T>
void BST <T> :: privateInsert(T t, BNode*& root)
{

	try {
		if (root == nullptr)
 		{
 			root = new BNode(t);
 		}
 		// Try to move data to left child if it is less than
 		else if (t < root->data)
 		{
 			if (root->pLeft != nullptr)
 			{
 				privateInsert(t, root->pLeft);
 			}
 			else
 			{
 				root->pLeft = new BNode(t, root);
 			}
 		}
 		// Try to move data to right child if it is more than or equal to
 		else if (t > root->data || t == root->data)
 		{
 			if (root->pRight != nullptr)
 			{
 				privateInsert(t, root->pRight);
 			}
 			else
 			{
 				root->pRight = new BNode(t, root);
 			}
 		}
	}
	catch (...)
	{
		throw "ERROR: Unable to allocate a node.";
	}
	return;
}

/*****************************************************
* BNode Class
******************************************************/
template <class T>
class BST<T>::BNode {
public:
	T data;
	BNode* pLeft;
	BNode* pRight;
	BNode* pParent;
	bool isRed;

	BNode() : pLeft(NULL), pRight(NULL), pParent(NULL), data(), isRed(true) {}

	// non-default. Initialize data as we create the node
	BNode(const T& data) : pLeft(NULL), pRight(NULL), pParent(NULL), data(data), isRed(true) {}
	BNode(const T& data, BNode* parent) : pLeft(NULL), pRight(NULL), pParent(parent), data(data), isRed(true) {}

	~BNode()
	{
		data.~T();
		pLeft = NULL;
		pRight = NULL;
		pParent = NULL;
		isRed = NULL;
	}

	/*void BST<T>::deleteBinaryTree(BNode*& node)
	{
		if (node == NULL)
		{
			return;
		}

		deleteBinaryTree(node->pLeft);
		deleteBinaryTree(node->pRight);
		delete node;
		node = NULL;
	}*/

	// add a node the left/right
	void addLeft(BNode* pNode);
	void addRight(BNode* pNode);

	// create a node and add it to the left/right
	void addLeft(const T& t);
	void addRight(const T& t);

	// is the passed node our right child?
	bool isRightChild(BNode* pNode) const { return pRight == pNode; }
	bool isLeftChild(BNode* pNode) const { return pLeft == pNode; }

	// balance the tree
	void balance();
};

/**************************************************
* BST iterator
*************************************************/
template <class T>
class BST <T> ::iterator
{
private:
	typename BST <T>::BNode* pNode;

public:
	// constructors, destructors, and assignment operator
	iterator() : pNode(NULL) {}
	iterator(BST* it) : pNode(it) {}
	iterator(const iterator& rhs) { *this = rhs; }
	iterator& operator = (const iterator& rhs)
	{
		this->pNode = rhs.pNode;
		return *this;
	}

	// equals, not equals operator
	bool operator != (const iterator& rhs) const  { return rhs.pNode != this->pNode; }
	bool operator == (const iterator& rhs) const  { return rhs.pNode == this->pNode; }

	// dereference operator
	T& operator * ()
	{
		if (pNode)
			return pNode->data;
		else
			throw "ERROR: Trying to dereference a NULL pointer";
	}

	// prefix increment
	iterator& operator ++ ()
	{
		if (pNode)
			pNode = pNode->pNext;
		return *this;
	}

	// postfix increment
	iterator operator ++ (int postfix)
	{
		iterator tmp(*this);
		if (pNode)
			pNode = pNode->pNext;
		return tmp;
	}

	//prefix decrement
	iterator& operator -- ()
	{
		// do nothing if we have nothing
		if (NULL == pNode)
			return *this;

		// if there is a left node, take it
		if (NULL != pNode->pLeft)
		{
			// go left
			pNode = pNode->pLeft;

			// jig right - there might be more right-most children
			while (pNode->pRight)
				pNode = pNode->pRight;
			return *this;
		}

		// there are no left children, the right are done
		assert(NULL == pNode->pLeft);
		BNode* pSave = pNode;

		// go up
		pNode = pNode->pParent;

		// if the parent is the NULL, we are done!
		if (NULL == pNode)
			return *this;

		// if we are the right-child, got to the parent.
		if (pSave == pNode->pRight)
			return *this;

		// we are the left-child, go up as long as we are the left child!
		while (NULL != pNode && pSave == pNode->pLeft)
		{
			pSave = pNode;
			pNode = pNode->pParent;
		}

		return *this;
	}

	//postfix decrement
	iterator operator -- (int postfix)
	{
		iterator tmp(*this);
		if (pNode)
			pNode = pNode->pPrev;
		return tmp;
	}
};

/**************************************************
* BST : reverse iterator
*************************************************/
//template <class T>
//class BST<T> :: reverse_iterator
//{
//public:
//	// constructors, destructors, and assignment operator
//	reverse_iterator() : pNode(NULL) {}
//	reverse_iterator(BST* it) : pNode(it) {}
//	reverse_iterator(const reverse_iterator& rhs) { *this = rhs; }
//	reverse_iterator& operator = (const reverse_iterator& rhs)
//	{
//		this->pNode = rhs.pNode;
//		return *this;
//	}
//
//	// equals, not equals operator
//	bool operator != (const iterator& rhs) const { return rhs.pNode != this->pNode; }
//	bool operator == (const iterator& rhs) const { return rhs.pNode == this->pNode; }
//
//	// dereference operator
//	T& operator * ()
//	{
//		if (pNode)
//			return pNode->data;
//		else
//			throw "ERROR: Trying to dereference a NULL pointer";
//	}
//
//	// prefix increment
//	reverse_iterator& operator ++ ()
//	{
//		if (pNode)
//			pNode = pNode->pPrev;
//		return *this;
//	}
//
//	// postfix increment
//	reverse_iterator operator ++ (int postfix)
//	{
//		iterator tmp(*this);
//		if (pNode)
//			pNode = pNode->pPrev;
//		return tmp;
//	}
//
//	//prefix decrement
//	reverse_iterator& operator -- ()
//	{
//		if (pNode)
//			pNode = pNode->pNext;
//		return *this;
//	}
//
//	//postfix decrement
//	reverse_iterator operator -- (int postfix)
//	{
//		iterator tmp(*this);
//		if (pNode)
//			pNode = pNode->pNExt;
//		return tmp;
//	}
//
//private:
//	typename BST <T>::BNode* pNode;
//};


/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 *     advance by one.
 * Author:      Br. Helfrich
 * Performance: O(log n) though O(1) in the common case
 *************************************************/
//template <class T>
//typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
//{
//   // do nothing if we have nothing
//   if (NULL == pNode)
//      return *this;
//
//   // if there is a left node, take it
//   if (NULL != pNode->pLeft)
//   {
//      // go left
//      pNode = pNode->pLeft;
//
//      // jig right - there might be more right-most children
//      while (pNode->pRight)
//         pNode = pNode->pRight;
//      return *this;
//   }
//
//   // there are no left children, the right are done
//   assert(NULL == pNode->pLeft);
//   BNode * pSave = pNode;
//
//   // go up
//   pNode = pNode->pParent;
//
//   // if the parent is the NULL, we are done!
//   if (NULL == pNode)
//      return *this;
//
//   // if we are the right-child, got to the parent.
//   if (pSave == pNode->pRight)
//      return *this;
//
//   // we are the left-child, go up as long as we are the left child!
//   while (NULL != pNode && pSave == pNode->pLeft)
//   {
//      pSave = pNode;
//      pNode = pNode->pParent;
//   }
//
//   return *this;
//}

/**************************************************
* BST find
*************************************************/
template <class T>
typename BST<T> ::iterator BST<T>::find(const T& value) {
	BNode* nodeParent = root;
	BNode* nodeChild = root;
	do {
		if (value > nodeChild.data) {
			nodeParent = nodeChild;
			nodeChild = nodeParent->pRight;
		}
		if (value < nodeChild.data) {
			nodeParent = nodeChild;
			nodeChild = nodeParent->left;
		}
		if (value == nodeChild.data) { return new iterator(nodeChild); }
	} while (nodeChild != NULL);

	assert(nodeChild == NULL);
	return end();
}

/**************************************************
* BST begin
*************************************************/
template <class T>
typename BST <T> ::iterator BST<T>::begin() {
	if (root == NULL) { return new iterator(); }

	assert(root != NULL);
	BNode* pNode = root;
	BNode* hold;
	do {
		hold = pNode->pLeft;
		if (hold == NULL) { return pNode; }
		else { pNode = hold; }
	} while (hold != NULL);
}

/**************************************************
* BST begin
*************************************************/
template <class T>
typename BST <T> ::iterator BST<T>::rbegin() {
	if (root == NULL) { return new iterator(); }

	assert(root != NULL);
	BNode* pNode = root;
	BNode* hold;
	do {
		hold = pNode->pRight;
		if (hold == NULL) { return pNode; }
		else { pNode = hold; }
	} while (hold != NULL);
}

/*****************************************************
* BST Copy Constructor
******************************************************/
template<class T>
BST<T> ::BST(const BST<T>& rhs): root(NULL), numElements(0)
{
	try
	{
		if (!rhs.empty())
		{
			*this = rhs;
		}

	}
	catch (...)
	{
		throw "ERROR: Unable to allocate a node";
	}
}

/*****************************************************
* BST Assignment Operator
******************************************************/
template<class T>
BST<T>& BST<T> :: operator = (const BST<T>& rhs)
{
	return *this;
}

template<class T>
void BST<T>:: erase(BST<T>::iterator it)
{

}

} // namespace custom

#endif // BST_H
