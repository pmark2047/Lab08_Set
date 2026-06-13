/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

namespace custom
{

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);

   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   //
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   //

   iterator erase(iterator& it);
   void   clear() noexcept;

   //
   // Status
   //

   bool   empty() const noexcept { return numElements == 0; }
   size_t size()  const noexcept { return numElements;   }
   

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif

   class BNode;
   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   //
   // Construct
   //
   BNode() : pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true)
   {
   }
   BNode(const T &  t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true)
   {
   }
   BNode(T && t)  : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true)
   {
   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);

   //
   // Status
   //
   bool isRightChild(BNode * pNode) const { return pRight == pNode; }
   bool isLeftChild( BNode * pNode) const { return pLeft == pNode; }

   // balance the tree
   void balance();

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
public:
   // constructors and assignment
   iterator(BNode * p = nullptr)
   {
       pNode = p;
   }
   iterator(const iterator & rhs)
   {
       pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
       pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return rhs.pNode == pNode;
   }
   bool operator != (const iterator & rhs) const
   {
      return rhs.pNode != pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const
   {
      return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
       iterator it = *this;
       ++(*this);
      return it;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
       iterator it = *this;
       --(*this);
      return it;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST()
{
   numElements = 0;
   root = nullptr;
}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs)
{
   this->numElements = rhs.numElements;
   this->root = nullptr;
   if(rhs.root == nullptr)
      return;
   
   this->root = new BNode(rhs.root->data);
   this->root->isRed = rhs.root->isRed;
   
   const BNode* pSrcCurrent = rhs.root;
   BNode*pDestCurrent = this->root;
   
   while(pSrcCurrent != nullptr)
   {
      if(pSrcCurrent->pLeft != nullptr && pDestCurrent->pLeft == nullptr)
      {
         BNode* pNew = new BNode(pSrcCurrent->pLeft->data);
         pNew->isRed = pSrcCurrent->pLeft->isRed;
         pNew->pParent = pDestCurrent;
         pDestCurrent->pLeft = pNew;
         pSrcCurrent = pSrcCurrent->pLeft;
         pDestCurrent = pDestCurrent->pLeft;
      }
      else if(pSrcCurrent->pRight != nullptr && pDestCurrent->pRight == nullptr)
      {
         BNode* pNew = new BNode(pSrcCurrent->pRight->data);
         pNew->isRed = pSrcCurrent->pRight->isRed;
         pNew->pParent = pDestCurrent;
         pDestCurrent->pRight = pNew;
         pSrcCurrent = pSrcCurrent->pRight;
         pDestCurrent = pDestCurrent->pRight;
      }
      else
      {
         pSrcCurrent = pSrcCurrent->pParent;
         pDestCurrent = pDestCurrent->pParent;
      }
   }
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs)
{
   this->numElements = rhs.numElements;
   this->root = rhs.root;
   rhs.numElements = 0;
   rhs.root = nullptr;
}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
  
    if (this == &rhs)
        return *this;
    BST <T> temp(rhs);
    swap(temp);
    return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
    // make sure it's empty
    clear();
    // loop through an add each item
    for (const auto & item : il)
        insert(item);
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
    // make sure it's empty first
    clear();
    // use swap method
    swap(rhs);
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T> & rhs)
{
    // swap root nodes
    std::swap(rhs.root, root);
    
    // swap number of elements
    std::swap(rhs.numElements, numElements);
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   std::pair<iterator, bool> pairReturn(end(), false);
    
    // if the tree is empty
    if (root == nullptr)
    {
        root = new BNode(t);
        root->isRed = false;                    // root is black
        numElements = 1;                        // only 1 element now
        pairReturn.first = iterator(root);
        pairReturn.second = true;               // it was added
        return pairReturn;
    }
    BNode* pNode = root;
    bool stop = false;
    
    while (!stop)
    {
        // if the data is already in there and it's meant to be unique
        if (keepUnique && t == pNode->data)
        {
            pairReturn.first = iterator(pNode);
            pairReturn.second = false;          // it wasn't added because it's meant to be unique
            return pairReturn;
        }
        
        // t is smaller than pNode, add it to the left
        if (t < pNode->data)
        {
            // is there a node alread on the left?
            if (pNode->pLeft)
                pNode = pNode->pLeft;           // go to the left
            // no node to left, so add a new one
            else
            {
                pNode->addLeft(t);               // create a new node to the left
                pairReturn.first = iterator(pNode->pLeft);
                pairReturn.second = true;        // node was added
                stop = true;
            }
        }
        // pNode is smaller, check the right
        else
        {
            if (pNode->pRight)
                pNode = pNode->pRight;              // go to the right
            else
            {
                pNode->addRight(t);               // create a new node to the right
                pairReturn.first = iterator(pNode->pRight);
                pairReturn.second = true;        // node was added
                stop = true;
            }
        }
    }
    
    numElements++;
   return pairReturn;
}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
    std::pair<iterator, bool> pairReturn(end(), false);
     
     // if the tree is empty
     if (root == nullptr)
     {
         root = new BNode(std::move(t));
         root->isRed = false;                    // root is black
         numElements = 1;                        // only 1 element now
         pairReturn.first = iterator(root);
         pairReturn.second = true;               // it was added
         return pairReturn;
     }
     BNode* pNode = root;
     bool stop = false;
     
     while (!stop)
     {
         // if the data is already in there and it's meant to be unique
         if (keepUnique && t == pNode->data)
         {
             pairReturn.first = iterator(pNode);
             pairReturn.second = false;          // it wasn't added because it's meant to be unique
             return pairReturn;
         }
         
         // t is smaller than pNode, add it to the left
         if (t < pNode->data)
         {
             // is there a node alread on the left?
             if (pNode->pLeft)
                 pNode = pNode->pLeft;           // go to the left
             // no node to left, so add a new one
             else
             {
                 pNode->addLeft(std::move(t));               // create a new node to the left
                 pairReturn.first = iterator(pNode->pLeft);
                 pairReturn.second = true;        // node was added
                 stop = true;
             }
         }
         // pNode is smaller, check the right
         else
         {
             if (pNode->pRight)
                 pNode = pNode->pRight;              // go to the right
             else
             {
                 pNode->addRight(std::move(t));               // create a new node to the right
                 pairReturn.first = iterator(pNode->pRight);
                 pairReturn.second = true;        // node was added
                 stop = true;
             }
         }
     }
     
     numElements++;
    return pairReturn;

}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{
   return end();
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{
   BNode* pCurrent = root;
   
   while(pCurrent != nullptr)
   {
      if(pCurrent->pLeft != nullptr)
         pCurrent = pCurrent->pLeft;
      else if(pCurrent->pRight != nullptr)
         pCurrent = pCurrent->pRight;
      else
      {
         BNode* pTemp = pCurrent;
         pCurrent = pCurrent->pParent;
         if(pCurrent != nullptr)
         {
            if (pCurrent->pLeft == pTemp)
               pCurrent->pLeft = nullptr;
            else
               pCurrent->pRight = nullptr;
         }
         else
            root = nullptr;
         
         delete pTemp;
      }
   }
   numElements = 0;
}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
    // is it empty?
    if (root == nullptr)
        return end();               // return the end, which is null
    
    BNode* p = root;
    // find the node all the way to the left
    while (p->pLeft)
        p = p->pLeft;
   return iterator(p);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
    BNode * p = root;
    while (p != nullptr)
    {
        if (p->data == t)
            return iterator(p);
        else if (t < p->data)
            p = p->pLeft;
        else
            p = p->pRight;
    }
   return end();
}

/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
    pLeft = pNode;                  // make it left of this
    if (pNode)
        pNode->pParent = this;      // make this it's parent
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
    pRight = pNode;                 // make it right of this
    // if it's not null...
    if (pNode)
        pNode->pParent = this;      // make this it's parent
    
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
    // create a node with given data
    BNode* pNode = new BNode(t);
    // add it to the left with method
    addLeft(pNode);
    pNode->balance();               // balance it!
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{
    // create a node with given data and move function
    BNode* pNode = new BNode(std::move(t));
    // add it to the left with method
    addLeft(pNode);
    pNode->balance();               // balance it!

}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
    // new node with t
    BNode * pNode = new BNode(t);
    // add it to the right
    addRight(pNode);
    pNode->balance();               // balance

}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
    // new node with t
    BNode * pNode = new BNode(std::move(t));
    // add it to the right
    addRight(pNode);
    pNode->balance();               // balance
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: balance()
{
   // Case 1: if we are the root, then color ourselves black and call it a day.
    if (!pParent)
    {
        isRed = false;   // root is black
        return;
    }
   // Case 2: if the parent is black, then there is nothing left to do
    if (!pParent->isRed)
        return;
    
    // figure out the family members
    BNode* pGranny = pParent->pParent;
    BNode* pAunt = new BNode;               // this one depends
    if (pGranny->isRightChild(pParent))
        pAunt = pGranny->pLeft;
    else
        pAunt = pGranny->pRight;
    BNode * pSibling = new BNode;           // this one depends
    if (pParent->isRightChild(this))
        pSibling = pParent->pLeft;
    else
        pSibling = pParent->pRight;
    
   // Case 3: if the aunt is red, then just recolor
    if (pAunt != nullptr && pAunt->isRed == true)
    {
        pParent->isRed = false;             // parent is black
        pAunt->isRed = false;               // right of root is black
        pGranny->isRed = true;              // root is red
        pGranny->balance();
        return;
    }
        

   // Case 4: if the aunt is black or non-existant, then we need to rotate

   // Case 4a: We are mom's left and mom is granny's left
    // temporary top
    BNode * pHead = nullptr;
    if (pParent->isLeftChild(this) && pGranny->isLeftChild(pParent))
    {
        pParent->addRight(pGranny);         // granny is rotated to the right
        pGranny->addLeft(pSibling);         // and siblings are added to granny's right
        pHead = pParent;
        
        pParent->isRed = false;             // Parent is black
        pGranny->isRed = true;              // granny is red
    }
   // case 4b: We are mom's right and mom is granny's right
    else if (pParent->isRightChild(this) && pGranny->isRightChild(pParent))
    {
        pParent->addLeft(pGranny);          // granny rotated to left
        pGranny->addRight(pSibling);        // siblings roatated to granny's left
        pHead = pParent;
        
        pParent->isRed = false;             // Parent is black
        pGranny->isRed = true;              // granny is red
    }
   // Case 4c: We are mom's right and mom is granny's left
    else if (pParent->isRightChild(this) && pGranny->isLeftChild(pParent))
    {
        pGranny->addLeft(this->pRight);
        pParent->addRight(this->pLeft);
        this->addRight(pGranny);
        this->addLeft(pParent);
        pHead = this;
        
//        if (pGranny->pParent == nullptr)
//            this->pParent = nullptr;
//        else if (pGranny->pParent->pRight == pGranny)
//            pGranny->pParent->pRight = this;
//        else
//            pGranny->pParent->pLeft = this;
//
        this->isRed = false;
        pGranny->isRed = true;
        
    }
    
   // case 4d: we are mom's left and mom is granny's right
    else if (pParent->isLeftChild(this) && pGranny->isRightChild(pParent))
    {
        pGranny->addRight(this->pLeft);
        pParent->addLeft(this->pRight);
        
        this->addLeft(pGranny);
        this->addRight(pParent);
        pHead = this;
//        if (pGranny->pParent == nullptr)
//            this->pParent = nullptr;
//        else if (pGranny->pParent->pLeft == pGranny)
//            pGranny->pParent->pLeft = this;
//        else
//            pGranny->pParent->pRight = this;
        
        this->isRed = false;
        pGranny->isRed = true;
        
    }
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   if (pNode == nullptr)
      return *this;
   
   if (pNode->pRight != nullptr)
   {
      pNode = pNode->pRight;
      while(pNode->pLeft != nullptr)
         pNode = pNode->pLeft;
   }
   else
   {
      BNode* pTemp = pNode;
      pNode = pNode->pParent;
      while(pNode != nullptr && pTemp == pNode->pRight)
      {
         pTemp = pNode;
         pNode = pNode->pParent;
      }
   }
   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   if (pNode == nullptr)
      return *this;
   
   if (pNode->pLeft != nullptr)
   {
      pNode = pNode->pLeft;
      while(pNode->pRight != nullptr)
         pNode = pNode->pRight;
   }
   else
   {
      BNode* pTemp = pNode;
      pNode = pNode->pParent;
      while(pNode != nullptr && pTemp == pNode->pLeft)
      {
         pTemp = pNode;
         pNode = pNode->pParent;
      }
   }
   return *this;
}


} // namespace custom


