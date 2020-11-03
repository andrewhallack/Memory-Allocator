/* 
 File: my_allocator.cpp
 
 Author: Andrew Hallack
 Department of Computer Science
 Texas A&M University
 Date  : 8/29/2020
 
 Modified:
 
 This file contains the implementation of the class MyAllocator.
 
 */

#include <cstdlib>
#include "my_allocator.hpp"
#include <assert.h>
#include <math.h>
#include <iostream>

using namespace std;


MyAllocator::MyAllocator(size_t _basic_block_size, size_t _size) {
    cout << "Reserving memory of size = " << _size << endl;
    start = std::malloc(_size); // reserves specific amount of memory
    current = (char*)start; // stores current mem address
    block_size = _basic_block_size;
    remaining = _size; // sets remaining memory

    SegmentHeader* seg1 = new(start) SegmentHeader(_size, true); // places segment header at desired location
    list.Add(seg1); // adds segment header to free list    
}

MyAllocator::~MyAllocator() {
    std::free(start);
}

void* MyAllocator::Malloc(size_t _length) {
    // This empty implementation just uses C standard library malloc
    cout << "MyAllocator::Malloc called with length = " << _length << endl;
    
    double multiplier = double(_length + sizeof(SegmentHeader)) / block_size; // creates multiplier to 
                                                                              // find necessary amount of blocks
    size_t len = ceil(multiplier) * block_size; // finds amount of memory necessary to allocate

    // traverses through list to find long-enough segment
    SegmentHeader* seg = list.Head();
    while (seg != nullptr && seg->GetLength() < len) {
        seg = seg->Next();
    }
    
    // if no segments are long enough, we "ran out" of available memory
    if (seg == nullptr) {
        cout << "Malloc failed: not enough free memory left to allocate." << endl;
        return nullptr;
    }

    seg->SetIsFree(false); // sets segment header to denote whether memory is free or not
    list.Remove(seg); // removes allocated memory from free list

    // if segment in free list is far too large,
    // split into 2 segments to not waste memory
    if (seg->GetLength() - len >= block_size) {
        SegmentHeader* splitSeg = seg->Split(len);
        list.Add(splitSeg);
    }

    // list.Print();    

    // returns address to allocated memory after segment header
    return (void*) ((char*)seg + sizeof(SegmentHeader));
}

bool MyAllocator::Free(void* _a) {
    cout << "MyAllocator::Free called" << endl;

    // find the address of the segment header to return back to free list
    SegmentHeader* seg = (SegmentHeader*)((char*)_a - sizeof(SegmentHeader));

    if (seg->GetIsFree()) return false; // cannot free memory that is already free

    seg->SetIsFree(true); // denotes that segment header is free
    list.Add(seg); // adds segment header to free list

    return true;
}
