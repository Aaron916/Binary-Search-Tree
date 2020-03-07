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

	// Helper functions to use recursively
	void privateInsert(T t, BNode*& root);
	BNode* copyBTree(const BNode* root);
	BNode* privateFind(T value, BNode* root);

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
	void deleteBTree(BNode*& node);

	void clear()
	{
		if (root)
			deleteBTree(root);
		numElements = 0;
	}

	BNode* getRoot() { return root; }

	void insert(const T& t);

	class iterator;
	iterator begin();
	iterator end() { return iterator(NULL); }
	iterator find(const T& value);
	iterator rbegin();
	iterator rend() { return iterator(NULL); }

	void erase(iterator & it);
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

  // Check if it is a right node or left
	bool isRight(BNode* right)
	{
		if (right->pParent != nullptr)
			return( right == right->pParent->pRight );
		return false;
	}

	bool isLeft(BNode* left)
	{
		if (left->pParent != nullptr)
			return(left == left->pParent->pLeft);
		return false;
	}

public:
	// constructors, destructors, and assignment operator
	iterator() : pNode(NULL) {}
	iterator(BNode* it) : pNode(it) {}
	iterator(const iterator& rhs) { *this = rhs; }
	iterator& operator = (const iterator& rhs)
	{
		this->pNode = rhs.pNode;
		return *this;
	}

	iterator & operator = (const BNode* node)
	{
		this->pNode = node;
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
		// do nothing if we have nothing
		if (pNode == nullptr)
			return *this;

		// go to the right node and from there to the farthest left (closest value that is greater)
		else if (pNode->pRight != nullptr)
		{
			pNode = pNode->pRight;

			while (pNode->pLeft != nullptr)
				pNode = pNode->pLeft;
		}

		// check what type of child it is
		else if (isLeft(pNode))
		{
			//The next greatest node for a left child is always it's parent
			pNode = pNode->pParent;
		}
		else if (isRight(pNode))
		{
			// The next greatest node for a right child is always the parent
			// of the first ancestor that is a left child
			while (isRight(pNode))
			{
				pNode = pNode->pParent;
			}
			pNode = pNode->pParent;
		}
		else
		{
			pNode = nullptr;
		}

		return *this;
	}

	// postfix increment
	iterator operator ++ (int postfix)
	{
		iterator tmp(*this);

		// do nothing if we have nothing
		if (pNode == nullptr)
			return *this;

		else if (pNode->pRight != nullptr)
		{
			pNode = pNode->pRight;

			while (pNode->pLeft != nullptr)
				pNode = pNode->pLeft;
		}


		else if (isLeft(pNode))
		{
			pNode = pNode->pParent;
		}

		else if (isRight(pNode))
		{
			while (isRight(pNode))
			{
				pNode = pNode->pParent;
			}
			pNode = pNode->pParent;
		}
		else
		{
			pNode = nullptr;
		}

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
* BST find / Public & Private
*************************************************/
template <class T>
typename BST<T> ::iterator BST<T>::find(const T& value) {
	BNode* temp = privateFind(value, this->root);
	iterator tempIt(temp);
	return tempIt;
}

template<class T>
typename BST<T>::BNode* BST<T>::privateFind(T value, BNode* root)
{
	if (root == nullptr || value == root->data)
	{
		return root;
	}

	else if (value < root->data)
	{
		return privateFind(value, root->pLeft);
	}

	else if (value > root->data)
	{
		return privateFind(value, root->pRight);
	}

	return root;
}


/**************************************************
* BST begin
*************************************************/
template <class T>
typename BST <T> ::iterator BST<T>::begin() {
	if (root == NULL) { return iterator(NULL); }

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
* BST rbegin
*************************************************/
template <class T>
typename BST <T> ::iterator BST<T>::rbegin() {
	if (root == NULL) { return iterator(NULL); }

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
		this->numElements = rhs.numElements;
 		this->root = copyBTree(rhs.root);
}

/*****************************************************
* BST Assignment Operator
******************************************************/
template<class T>
BST<T>& BST<T> :: operator = (const BST<T>& rhs)
{
	this->numElements = rhs.numElements;
	this->root = copyBTree(rhs.root);

	return *this;
}


/*****************************************************
* BST Delete Binary Tree
******************************************************/
template<class T>
void BST<T>::deleteBTree(BNode*& node)
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

/*****************************************************
* BST Erase
******************************************************/
template<class T>
void BST<T>:: erase(BST<T>::iterator & it)
{
	BST<T>::BNode* pNode = it;

	//Case 1: No Children
	if (pNode->pRight == NULL && pNode->pLeft == NULL)
	{
		if (pNode->pParent != NULL && pNode->pParent->pRight == pNode)
		{
			pNode->pParent->pRight = NULL;
		}
		if (pNode->pParent != NULL && pNode->pParent->pLeft == pNode)
		{
			pNode->pParent->pLeft = NULL;
		}

		delete pNode;
	}

	//Case 2: One Child
	if (pNode->pRight == NULL && pNode->pLeft != NULL)
	{
		pNode->pLeft->pParent = pNode->pParent;

		if (pNode->pParent != NULL && pNode->pParent->pRight == pNode )
		{
			pNode->pParent->pRight = pNode->pLeft;
		}
		if (pNode->pParent != NULL && pNode->pParent->pLeft == pNode)
		{
			pNode->pParent->pLeft = pNode->pLeft;
		}

		delete pNode;
	}

	if (pNode->pLeft == NULL && pNode != NULL)
	{
		pNode->pRight->pParent = pNode->pParent;

		if (pNode->pParent != NULL && pNode->pParent->pRight == pNode)
		{
			pNode->pParent->pRight = pNode->pRight;
		}
		if (pNode->pParent != NULL && pNode->pParent->pLeft == pNode)
		{
			pNode->pParent->pLeft = pNode->pRight;
		}
		delete pNode;
	}

}

/*****************************************************
* BST copy Tree
******************************************************/

template<class T>
typename BST<T>::BNode* BST<T>::copyBTree(const BNode * root)
{
	if (root != nullptr)
	{
		BST<T>::BNode* newBNode = new BST<T>::BNode(root->data);
		if (root->pLeft != nullptr)
		{
			newBNode->pLeft = copyBTree(root->pLeft);

			if (newBNode->pLeft != nullptr)
				newBNode->pLeft->pParent = newBNode;
		}

		if (root->pRight != nullptr)
		{
			newBNode->pRight = copyBTree(root->pRight);

			if (newBNode->pRight != nullptr)
				newBNode->pRight->pParent = newBNode;
		}

		return newBNode;
	}
	return nullptr;

}

} // namespace custom

#endif // BST_H
