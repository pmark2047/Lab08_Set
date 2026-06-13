/***********************************************************************
* Header:
*    Set
* Summary:
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
*        set                 : A class that represents a Set
*        set::iterator       : An iterator through Set
* Author
*    <your names here>
************************************************************************/

#pragma once

#include <cassert>
#include <iostream>
#include "bst.h"
#include <memory>     // for std::allocator
#include <functional> // for std::less


namespace custom
{

//   class TestSet;

/************************************************
 * SET
 * A class that represents a Set
 ***********************************************/
template <typename T>
class set
{
public:
   
   // 
   // Construct
   //
   set() 
   {
   }
   set(const set &  rhs)
   {
       bst = rhs.bst;
   }
   set(set && rhs) 
    {
       bst = std::move(rhs.bst);
   }
   set(const std::initializer_list <T> & il) 
   {
       bst = il;
   }
   template <class Iterator>
   set(Iterator first, Iterator last) 
   {
       for (auto it = first; it != last; ++it)
           bst.insert(*it, true);
   }
  ~set() { }

   //
   // Assign
   //

   set & operator = (const set & rhs)
   {
       bst = rhs.bst;
      return *this;
   }
   set & operator = (set && rhs)
   {
       bst = std::move(rhs.bst);
      return *this;
   }
   set & operator = (const std::initializer_list <T> & il)
   {
       bst = il;
      return *this;
   }
   void swap(set& rhs) noexcept
   {
       bst.swap(rhs.bst);
   }

   //
   // Iterator
   //

   class iterator;
   iterator begin() const noexcept 
   { 
      return iterator(bst.begin());
   }
   iterator end() const noexcept 
   { 
      return iterator(bst.end());
   }

   //
   // Access
   //
   iterator find(const T& t) 
   { 
      return iterator(bst.find(t));
   }

   //
   // Status
   //
   bool empty() const noexcept 
   { 
      return bst.empty();
   }
   size_t size() const noexcept 
   { 
      return bst.size();
   }

   //
   // Insert
   //
   std::pair<iterator, bool> insert(const T& t)
   {
      std::pair<iterator, bool> p = bst.insert(t, true);
      return p;
   }
   std::pair<iterator, bool> insert(T&& t)
   {
      std::pair<iterator, bool> p = bst.insert(std::move(t), true);
      return p;
   }
   void insert(const std::initializer_list <T>& il)
   {
       for (T i : il)
           insert(i);
   }
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
       for(Iterator it = first; first != last; ++first)
           insert(*first);
   }


   //
   // Remove
   //
   void clear() noexcept 
   {
       bst.clear();
   }
   iterator erase(iterator &it)
   { 
      return iterator(bst.erase(it.it));
   }
   size_t erase(const T & t) 
   {
       iterator it = find(t);
       
       if (it == end())
           return 0;
    
       erase(it);
       return 1;   }
   iterator erase(iterator &itBegin, iterator &itEnd)
   {
      return iterator();
   }

#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif
   
   custom::BST <T> bst;
};


/**************************************************
 * SET ITERATOR
 * An iterator through Set
 *************************************************/
template <typename T>
class set <T> :: iterator
{
public:
   // constructors, destructors, and assignment operator
   iterator() 
   {
       it.pNode = nullptr;
   }
   iterator(const typename custom::BST<T>::iterator& itRHS) 
   {
       it = itRHS;
   }
   iterator(const iterator & rhs) 
   {
       it = rhs.it;
   }
   iterator & operator = (const iterator & rhs)
   {
       it = rhs.it;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator & rhs) const 
   { 
      return it != rhs.it;
   }
   bool operator == (const iterator & rhs) const 
   { 
      return it == rhs.it;
   }

   // dereference operator: by-reference so we can modify the Set
   const T & operator * () const 
   { 
      return *it;
   }

   // prefix increment
   iterator & operator ++ ()
   {
       ++it;
      return *this;
   }

   // postfix increment
   iterator operator++ (int postfix)
   {
       iterator temp(*this);
       ++it;
       return *temp;
   }
   
   // prefix decrement
   iterator & operator -- ()
   {
       --it;
      return *this;
   }
   
   // postfix decrement
   iterator operator-- (int postfix)
   {
       iterator temp(*this);
       --it;
      return *temp;
   }
   
#ifdef DEBUG // make this visible to the unit tests
public:
#else
private:
#endif

   typename custom::BST<T>::iterator it;
};

/***********************************************
 * SET : EQUIVALENCE
 * See if two sets are the same size
 ***********************************************/
template <typename T>
bool operator == (const set <T> & lhs, const set <T> & rhs)
{
   return true;
}

template <typename T>
inline bool operator != (const set <T> & lhs, const set <T> & rhs)
{
   return true;
}

/***********************************************
 * SET : RELATIVE COMPARISON
 * See if one set is lexicographically before the second
 ***********************************************/
template <typename T>
bool operator < (const set <T> & lhs, const set <T> & rhs)
{
   return true;
}

template <typename T>
inline bool operator > (const set <T> & lhs, const set <T> & rhs)
{
   return true;
}

}; // namespace custom



