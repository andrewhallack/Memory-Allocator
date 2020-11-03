/* 
 File: free_list.cpp
 
 Author: Andrew Hallack
 Department of Computer Science
 Texas A&M University
 Date  : 8/29/2020
 
 Modified:
 
 This file contains the implementation of the class FreeList.
 
 */

#include <iostream>
#include <assert.h>
#include "free_list.hpp"


using namespace std;

SegmentHeader::SegmentHeader(size_t _length, bool _is_free) {
  length = _length;
  is_free = _is_free;
  cookie = COOKIE_VALUE;
  next = nullptr;
  prev = nullptr;
}

SegmentHeader::~SegmentHeader() {
}

// verifies segmentheaders by using cookies.
// if segment header were to be erroneously allocated, it would alert the user.
void SegmentHeader::CheckValid() {
  if (cookie != COOKIE_VALUE) {
    cout << "INVALID SEGMENT HEADER!!" << endl;
    assert(false);
  }
}

SegmentHeader* SegmentHeader::Split(size_t seg_length) {
  SegmentHeader* segNew = new((void*)((char*)this + seg_length)) SegmentHeader(this->length - seg_length, true);
  this->length = seg_length; // sets old segment to shorter length
  return segNew;
}

FreeList::FreeList() {
  head = nullptr;
  tail = nullptr;
}

FreeList::~FreeList() {

}

bool FreeList::Add(SegmentHeader * _segment) {
  if (!_segment->GetIsFree()) return false; // cannot free memory necessary for allocation

  // if list is empty, set head and tail to the segment parameter passed
  if (head == nullptr) {
    head = _segment;
    tail = _segment;
    return true;
  }

  SegmentHeader* newHead = _segment; // creates head replacement
  newHead->next = head; // pushes newHead to front of list
  newHead->prev = nullptr; 
  head->prev = newHead;
  head = newHead;

  return true;
}

bool FreeList::Remove(SegmentHeader * _segment) {
  if (head == nullptr) return false; // can't remove from empty list
  if (_segment == nullptr) return false; // cannot remove nullptr from list
  if (_segment->GetIsFree()) return false; // cannot remove memory that is supposed to be free
  
  // edge cases with head and tail removal
  if (head == _segment && head->next == nullptr) {
    head = nullptr;
    return true;
  } else if (head == _segment) {
    head = head->next;
    head->prev = nullptr;
    return true;
  } else if (tail == _segment) {
    tail = tail->prev;
    tail->next = nullptr;
    return true;
  }

  // traverses through list to find necessary segment to remove
  SegmentHeader* current = head;
  while (current != nullptr && current->next != _segment) {
    current = current->next;
  }

  // redirects pointers to remove segment
  SegmentHeader* current_next = _segment->next;
  current->next = current_next;
  current_next->prev = current;

  return true;
}

// function for debugging
void FreeList::Print() {
  SegmentHeader* current = head;

  cout << "------------------------------------------------" << endl;

  cout << "Printing list:" << endl;
  while (current != nullptr) {
    cout << "Length: " << current->length << " Cookie: " << current->cookie << "\n";
    current = current->next;
  }

  cout << "------------------------------------------------" << endl;
}
