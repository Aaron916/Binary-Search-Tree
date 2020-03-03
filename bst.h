/***********************************************************************
 * Component:
 *    Assignment 09, Binary Search Tree (BST)
 *    Brother <your instructor name here>, CS 235
 * Author:
 *    <your name here>
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

	~BST()
	{
		deleteBTree(root);
	}

	int size() const { return numElements; }
	bool empty() const { return size() == NULL; }
	void clear();
	void insert(const T& t);

	class iterator;
	class reverse_iterator;

private: 
	class BNode;

	BNode<T>* root;
	int numElements;
};


/**************************************************
* BST iterator
*************************************************/
template <class T>
class BST <T> ::iterator
{
public:
	// constructors, destructors, and assignment operator
	iterator() : p(NULL) {              }
	iterator(Node* it) : p(it) {              }
	iterator(const reverse_iterator& it) : p(it.p) { }
	iterator(const iterator& rhs) { *this = rhs; }
	iterator& operator = (const iterator& rhs)
	{
		this->p = rhs.p;
		return *this;
	}

	// equals, not equals operator
	bool operator != (const iterator& rhs) const { return rhs.p != this->p; }
	bool operator == (const iterator& rhs) const { return rhs.p == this->p; }

	// dereference operator
	T& operator * ()
	{
		if (p)
			return p->data;
		else
			throw "ERROR: Trying to dereference a NULL pointer";
	}

	// prefix increment
	iterator& operator ++ ()
	{
		if (p)
			p = p->pNext;
		return *this;
	}

	// postfix increment
	iterator operator ++ (int postfix)
	{
		iterator tmp(*this);
		if (p)
			p = p->pNext;
		return tmp;
	}

	//prefix decrement
	iterator& operator -- ()
	{
		if (p)
			p = p->pPrev;
		return *this;
	}

	//postfix decrement
	iterator operator -- (int postfix)
	{
		iterator tmp(*this);
		if (p)
			p = p->pPrev;
		return tmp;
	}

	T getValue()
	{
		return p->data;
	}

	//friends who need to access p
	friend iterator list <T> ::insert(iterator& it, const T& data);
	friend iterator list <T> ::erase(iterator it);



	typename list <T> ::Node* p;
};

/**************************************************
* BST : reverse iterator
*************************************************/
template <class T>
class BST<T> :: reverse_iterator
{
public:
	// constructors, destructors, and assignment operator
	reverse_iterator() : p(NULL) {              }
	reverse_iterator(Node* it) : p(it) {              }
	reverse_iterator(const iterator& it) : p(it.p) { }
	reverse_iterator(const reverse_iterator& rhs) { *this = rhs; }
	reverse_iterator& operator = (const reverse_iterator& rhs)
	{
		this->p = rhs.p;
		return *this;
	}

	// equals, not equals operator
	bool operator != (const iterator& rhs) const { return rhs.p != this->p; }
	bool operator == (const iterator& rhs) const { return rhs.p == this->p; }

	// dereference operator
	T& operator * ()
	{
		if (p)
			return p->data;
		else
			throw "ERROR: Trying to dereference a NULL pointer";
	}

	// prefix increment
	reverse_iterator& operator ++ ()
	{
		if (p)
			p = p->pPrev;
		return *this;
	}

	// postfix increment
	reverse_iterator operator ++ (int postfix)
	{
		iterator tmp(*this);
		if (p)
			p = p->pPrev;
		return tmp;
	}

	//prefix decrement
	reverse_iterator& operator -- ()
	{
		if (p)
			p = p->pNext;
		return *this;
	}

	//postfix decrement
	reverse_iterator operator -- (int postfix)
	{
		iterator tmp(*this);
		if (p)
			p = p->pNExt;
		return tmp;
	}

	T getValue()
	{
		return p->data;
	}


	typename list <T> ::Node* p;
};

   
/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 *     advance by one. 
 * Author:      Br. Helfrich
 * Performance: O(log n) though O(1) in the common case
 *************************************************/
template <class T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
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
   BNode * pSave = pNode;

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

/*****************************************************
* BNode Class
******************************************************/
template <class T>
class BST<T>::BNode {
public:
	T data;
	BNode<T>* pLeft;
	BNode<T>* pRight;
	BNode<T>* pParent;

	BNode() : pLeft(NULL), pRight(NULL), pParent(NULL), data() {}

	// non-default. Initialize data as we create the node
	BNode(const T& data) : pLeft(NULL), pRight(NULL), pParent(NULL), data(data) {}
};

template<class T>
void addLeft(BNode<T>* pNode, const T& t)
{
	try
	{
		BNode<T>* pAdd = new BNode<T>(t);
		pAdd->pParent = pNode;
		pNode->pLeft = pAdd;
	}
	catch (std::bad_alloc)
	{
		throw "ERROR: Unable to allocate a node";
	}
}

template<class T>
void addLeft(BNode<T>* pNode, BNode<T>* pAdd)
{
	try
	{
		if (pAdd != NULL)
			pAdd->pParent = pNode;
		pNode->pLeft = pAdd;
	}
	catch (std::bad_alloc)
	{
		throw "ERROR: Unable to allocate a node";
	}
}

template<class T>
void addRight(BNode<T>* pNode, const T& t)
{
	try
	{
		BNode<T>* pAdd = new BNode<T>(t);
		pAdd->pParent = pNode;
		pNode->pRight = pAdd;
	}
	catch (std::bad_alloc)
	{
		throw "ERROR: Unable to allocate a node";
	}
}

template<class T>
void addRight(BNode<T>* pNode, BNode<T>* pAdd)
{
	try
	{
		if (pAdd != NULL)
			pAdd->pParent = pNode;
		pNode->pRight = pAdd;
	}
	catch (std::bad_alloc)
	{
		throw "ERROR: Unable to allocate a node";
	}
}

template<class T>
int sizeBTree(const BNode<T>* node)
{
	if (node == NULL)
	{
		return 0;
	}
	else
	{
		return sizeBTree(node->pLeft) + 1 + sizeBTree(node->pRight);
	}
}

template<class T>
void deleteBTree(BNode<T>*& node)
{
	if (node == NULL)
	{
		return;
	}

	deleteBTree(node->pLeft);
	deleteBTree(node->pRight);
	delete node;
	node = NULL;
}

template<class T>
BNode<T>* copyBTree(const BNode<T>* source)
{
	if (source == NULL)
	{
		return NULL;
	}
	BNode<T>* destination;

	try
	{
		destination = new BNode<T>(source->data);
	}
	catch (...)
	{
		throw "ERROR: Unable to allocate a node";
	}

	destination->pLeft = copyBTree(source->pLeft);
	if (destination->pLeft != NULL)
	{
		destination->pLeft->pParent = destination;
	}

	destination->pRight = copyBTree(source->pRight);
	if (destination->pRight != NULL)
	{
		destination->pRight->pParent = destination;
	}

	return destination;
}

template <class T>
std::ostream& operator << (std::ostream& out, const BNode <T>* pHead)
{
	// Left First
	if (pHead->pLeft != NULL)
	{
		out << pHead->pLeft;
	}

	// Parent
	out << pHead->data << " ";

	// Right Last
	if (pHead->pRight != NULL)
	{
		out << pHead->pRight;
	}

	return out;
}

} // namespace custom

#endif // BST_H
