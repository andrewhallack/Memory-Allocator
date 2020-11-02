/* 
    File: free_list.hpp

    Author: <your name>
            Department of Computer Science and Engineering
            Texas A&M University
    Date  : <date>

    Modified:

*/

#ifndef _free_list_hpp_                   // include file only once
#define _free_list_hpp_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cstdlib>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

class SegmentHeader {

private:

  static const unsigned COOKIE_VALUE = 0xBADB00;
  unsigned int cookie; /* To check whether this is a genuine header! */
  size_t length;
  bool is_free;

  SegmentHeader* next = nullptr;
  SegmentHeader* prev = nullptr;

  friend class FreeList;
  
public:
  

  SegmentHeader(size_t _length, bool _is_free = true);
  
  ~SegmentHeader();
  /* We probably won't need the destructor. */

  void CheckValid();
  /* Check if the cookie is valid. */

  void SetIsFree(bool setFree) { is_free = setFree; } // sets whether memory is free
  bool GetIsFree() { return is_free; } // finds if memory is free
  void SetLength(size_t len) { length = len; } // sets length of segment
  size_t GetLength() { return length; } // returns length of segment

  SegmentHeader* Next() { return next; } // allows public access to next
  SegmentHeader* Prev() { return prev; } // allows public access to prev
  SegmentHeader* Split(size_t seg_length);
};

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS  FreeList */
/*--------------------------------------------------------------------------*/

class FreeList {

 private:

  SegmentHeader* head;
  SegmentHeader* tail;

public:

  FreeList(); 
  /* This function initializes a new free-list. */

  ~FreeList(); 
  /* We probably don't need a destructor. */ 

  bool Remove(SegmentHeader * _segment); 
  /* Remove the given segment from the given free list. 
     Returns true if the function succeeds.
  */ 

  bool Add(SegmentHeader * _segment); 
  /* Add the segment to the given free list. */

  void Print();

  SegmentHeader* Head() { return head; }
  SegmentHeader* Tail() { return tail; }
  
};

#endif 
