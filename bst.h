/***********************************************************************
 * Component:
 *    Assignment 09, Binary Search Tree (BST)
 *    Brother JonesL, CS 235
 * Author:
*    Adam Goff, Aaron Rook, Martin Melerio, Tanner Stratford
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
	void clear();
	void insert(const T& t);

	class iterator;
	iterator begin();
	iterator end() { return new iterator(); }
	iterator find(const T& value);

	class reverse_iterator;
	reverse_iterator rbegin() { return new iterator(root); }
	reverse_iterator rend() { return new iterator(); }

	void erase(iterator it);

private:
	class BNode;
	BNode* root;
	int numElements;
};

/**************************************************
* BST insert
*************************************************/
template <class T>
void BST <T> :: insert(const T& data) {
	if (root == NULL) { root = new BNode(data); }

	assert(root != NULL);

	BNode* nodeChild;
	BNode* nodeParent = root;
	do {
		if(data >= nodeChild.data) {
			nodeParent = nodeChild;
			nodeChild = nodeParent->pRight;
		}
		if (data <= root.data) {
			nodeParent = nodeChild;
			nodeChild = nodeParent->pLeft;
		}
	} while (nodeChild != NULL);

	assert(nodeParent != NULL);

	try {

		BNode insertNode = new BNode(data);
		insertNode.pParent = nodeParent;

		if (data >= nodeParent.data) {
			nodeParent->pRight = insertNode;
		}
		else {
			nodeParent->pLeft = insertNode;
		}
	}
	catch (...)
	{
		throw "ERROR: Unable to allocate a node.";
	}
	return;
}

/**************************************************
* BST find
*************************************************/
template <class T>
typename BST<T> :: iterator BST<T>:: find(const T& value) {
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
typename BST <T> :: iterator BST<T>:: begin() {
	if (root == NULL) { return new iterator(); }

	assert(root !=NULL);
	BNode* pNode = root;
	BNode* hold;
	do {
		hold = pNode->pLeft;
		if (hold == NULL) { return pNode; }
		else {pNode = hold;}
	} while (hold != NULL);
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

	BNode() : pLeft(NULL), pRight(NULL), pParent(NULL), data() {}

	// non-default. Initialize data as we create the node
	BNode(const T& data) : pLeft(NULL), pRight(NULL), pParent(NULL), data(data) {}

};

/**************************************************
* BST iterator
*************************************************/
template <class T>
class BST <T> ::iterator
{
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
	bool operator != (const iterator& rhs) const { return rhs.pNode != this->pNode; }
	bool operator == (const iterator& rhs) const { return rhs.pNode == this->pNode; }

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

private:
	BNode* pNode;
};

/**************************************************
* BST : reverse iterator
*************************************************/
template <class T>
class BST<T> :: reverse_iterator
{
public:
	// constructors, destructors, and assignment operator
	reverse_iterator() : pNode(NULL) {}
	reverse_iterator(BST* it) : pNode(it) {}
	reverse_iterator(const reverse_iterator& rhs) { *this = rhs; }
	reverse_iterator& operator = (const reverse_iterator& rhs)
	{
		this->pNode = rhs.pNode;
		return *this;
	}

	// equals, not equals operator
	bool operator != (const iterator& rhs) const { return rhs.pNode != this->pNode; }
	bool operator == (const iterator& rhs) const { return rhs.pNode == this->pNode; }

	// dereference operator
	T& operator * ()
	{
		if (pNode)
			return pNode->data;
		else
			throw "ERROR: Trying to dereference a NULL pointer";
	}

	// prefix increment
	reverse_iterator& operator ++ ()
	{
		if (pNode)
			pNode = pNode->pPrev;
		return *this;
	}

	// postfix increment
	reverse_iterator operator ++ (int postfix)
	{
		iterator tmp(*this);
		if (pNode)
			pNode = pNode->pPrev;
		return tmp;
	}

	//prefix decrement
	reverse_iterator& operator -- ()
	{
		if (pNode)
			pNode = pNode->pNext;
		return *this;
	}

	//postfix decrement
	reverse_iterator operator -- (int postfix)
	{
		iterator tmp(*this);
		if (pNode)
			pNode = pNode->pNExt;
		return tmp;
	}

private:
	BNode* pNode;
};

   
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

/*****************************************************
* BST Clear
******************************************************/
template<class T>
void BST<T>:: clear()
{

}

template<class T>
void BST<T>:: erase(BST<T>::iterator it)
{

}

} // namespace custom

#endif // BST_H
