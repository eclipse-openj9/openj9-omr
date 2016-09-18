/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 1996, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 ******************************************************************************/

/***************************************************************************/
/*                                                                         */
/*  File name:  allocator.h                                                */
/*  Purpose:    Definition of the CS2 base allocator                       */
/*                                                                         */
/***************************************************************************/

#ifndef CS2_ALLOCATOR_H
#define CS2_ALLOCATOR_H

#include <stdio.h>
#include "cs2/cs2.h"
#include <memory.h>
#include "cs2/hashtab.h"
#include "cs2/arrayof.h"

namespace CS2 {

  typedef exception allocator_exception;

  // Basic CS2 allocator class
  // CS2 allocators are per-instance, and should not have any non-static
  // data. Only exception is that they may have a reference to an actual
  // memory pool. Copying an allocator only copies that reference and both
  // objects will share the same memory pool
  class malloc_allocator {
  public:
      void *allocate(size_t size, const char *name=NULL) {
      void *ret = (void *) malloc(size);
      //      if (ret==0) throw allocator_exception(); disable for now
      return ret;
    }
    void deallocate(void *pointer, size_t size, const char *name=NULL) {
      free(pointer);
    }
    void *reallocate(size_t newsize, void *pointer, size_t size, const char *name=NULL) {
      return realloc(pointer, newsize);
    }

    template <class ostr, class allocator> ostr& stats(ostr &o, allocator &a) { return o;}
  };

  template <class base_allocator = CS2::malloc_allocator>
  class trace_allocator: private base_allocator {
  public:
    void *allocate(size_t size, const char *name=NULL ) {
      void *ret = (void *) base_allocator::allocate(size);
      printf ("%s %ld a %p\n", name?name:"", size, ret);
      return ret;
    }
    void deallocate(void *pointer, size_t size, const char *name=NULL) {
      printf ("%s %ld d %p\n", name?name:"", size, pointer);
      base_allocator::deallocate(pointer,size);
    }
    void *reallocate(size_t newsize, void *pointer, size_t size, const char *name=NULL) {
      void *ret = base_allocator::reallocate(newsize,pointer,size);
      printf ("%s %ld r %p->%p\n", name?name:"", newsize, pointer, ret);
      return ret;
    }

    template <class ostr, class allocator> ostr& stats(ostr &o, allocator &a) { return base_allocator::stats(o, a);}
    trace_allocator(const base_allocator &a = base_allocator()) : base_allocator(a) {}
  };

  template <class base_allocator = CS2::malloc_allocator, bool base_stats = false>
  class account_allocator: private base_allocator {
  private:
    struct AllocationRecord {
      uint64_t _count;
      uint64_t _size;

      void bump(size_t size) { _count+=1; _size += size; }
      void debump(size_t size) { _count-=1; _size -= size; }

      AllocationRecord(size_t size=0) : _count(1), _size(size) {}

      template <class ostr> friend
      ostr & operator<< (ostr &o, const struct AllocationRecord &a) {
        return o << "\t" << a._size << " bytes / " << a._count << "";
        }
    };

    HashTable<const char *, AllocationRecord, base_allocator> accounting;
  public:
    void *allocate(size_t size, const char *name=NULL ) {
      void *ret = (void *) base_allocator::allocate(size, name);

      HashIndex hi;
      if (!name) name="**unknown**";

      if (accounting.Locate((const char *)name, hi)) {
        accounting[hi].bump(size);
      } else {
        accounting.Add((const char *)name, AllocationRecord(size));
      }
      return ret;
    }
    void deallocate(void *pointer, size_t size, const char *name=NULL) {

      HashIndex hi;
      if(!name) name="**unknown**";
      if (accounting.Locate((const char *)name, hi)) {
        accounting[hi].debump(size);
        if (accounting[hi]._count==0) accounting.Remove(hi);
      }

      base_allocator::deallocate(pointer,size, name);
    }

    void *reallocate(size_t newsize, void *pointer, size_t size, const char *name=NULL) {
      HashIndex hi;
      if (!name) name="**unknown**";
      if (accounting.Locate(name, hi)) {
        accounting[hi].debump(size);
        accounting[hi].bump(newsize);
      }

      return base_allocator::reallocate(newsize,pointer,size, name);
    }


    struct stat_sort {
      stat_sort(size_t s=0, size_t c=0, const char *n=NULL) : size(s), count(c), name(n){}

      bool operator< (const struct stat_sort s2) {
        return size > s2.size;
      }
      size_t size, count;
      const char *name;

      stat_sort &operator=(const stat_sort &s){
        size = s.size;
        count = s.count;
        name = s.name;
        return *this;
      }
      template <class ostr> friend
      ostr & operator<< (ostr &o, const struct stat_sort &a) {
        return o << a.size << " (" << a.count << ")" << "\t" << (a.name?a.name:"");
      }
    };

    template <class ostr, class allocator> ostr& stats(ostr &o, allocator &a) {
      ArrayOf<struct stat_sort, allocator> sstats(a);

      uint32_t count=0;
      typename HashTable<const char *, AllocationRecord, base_allocator>::Cursor ac(accounting);
      for (ac.SetToFirst(); ac.Valid(); ac.SetToNext()){
        count++;
      }
      sstats.GrowTo(count);
      count=0;
      for (ac.SetToFirst(); ac.Valid(); ac.SetToNext()){
        sstats[count++] = stat_sort(accounting[ac]._size, accounting[ac]._count, accounting.KeyAt(ac));
      }
      sstats.Sort();

      o << "Allocator stats:\n"
        << sstats;
      if (base_stats) return base_allocator::stats(o, a);
      return o;
    }

    account_allocator(const base_allocator &a = base_allocator()) : base_allocator(a), accounting(a) {}
    account_allocator(const account_allocator &a) : base_allocator(a), accounting(a) {}
  };

  template <class base_allocator = CS2::malloc_allocator>
  class stat_allocator: private base_allocator {
  public:
    void *allocate(size_t size, const char *name = NULL) {
      void *ret = (void *) base_allocator::allocate(size,name);
      if (collect_stats) {
        alloc_cnt+=1; alloc_size += size;
        watermark += size;
        if (watermark > high_watermark) high_watermark = watermark;
      }
      return ret;
    }
    void deallocate(void *pointer, size_t size, const char *name = NULL) {
      base_allocator::deallocate(pointer,size,name);
      if (collect_stats){
        dealloc_cnt+=1; dealloc_size += size;
        watermark -= size;
      }
    }
    void *reallocate(size_t newsize, void *pointer, size_t size, const char *name = NULL) {
      if (collect_stats) {
        realloc_cnt+=1; realloc_size += size;
        watermark += (newsize-size);
        if (watermark > high_watermark) high_watermark = watermark;
      }
      return base_allocator::reallocate(newsize,pointer,size,name);
    }

    template <class ostr, class allocator> ostr& stats(ostr &o, allocator &a) { return base_allocator::stats(o, a);}

    stat_allocator(const base_allocator &a = base_allocator(), bool _stats=false ) :
      base_allocator(a),
      collect_stats(_stats),
      alloc_cnt(0),
      dealloc_cnt(0),
      realloc_cnt(0),
      alloc_size(0),
      realloc_size(0),
      dealloc_size(0),
      watermark(0),
      high_watermark(0)
    {}

    ~stat_allocator() {
      if (collect_stats && alloc_cnt!=0) {
        printf("  ALLOC= %llu SIZE=%llu AVG=%llu\n", (long long unsigned int)alloc_cnt, (long long unsigned int)alloc_size, (long long unsigned int)(alloc_cnt==0?0:alloc_size/alloc_cnt));
        printf("DEALLOC= %llu SIZE=%llu AVG=%llu\n", (long long unsigned int)dealloc_cnt, (long long unsigned int)dealloc_size, (long long unsigned int)(dealloc_cnt==0?0:dealloc_size/alloc_cnt));
        printf("REALLOC= %llu SIZE=%llu AVG=%llu\n", (long long unsigned int)realloc_cnt, (long long unsigned int)realloc_size, (long long unsigned int)(realloc_cnt==0?0:realloc_size/alloc_cnt));

        printf("FINAL SIZE=%lld\n", (long long unsigned int)watermark);
        printf("HIGH WATER MARK=%lld\n", (long long unsigned int)high_watermark);
      }
    }
  private:
  bool collect_stats;
  uint64_t alloc_cnt;
  uint64_t dealloc_cnt;
  uint64_t realloc_cnt;

  uint64_t alloc_size;
  uint64_t realloc_size;
  uint64_t dealloc_size;

  uint64_t watermark;
  uint64_t high_watermark;
  };

  template <class base_allocator = CS2::malloc_allocator>
  class check_allocator: private base_allocator {
    struct allocation_record {
      size_t size;
      const char *name;

      allocation_record(size_t s, const char *n) {
        size =s; name = n;
      }
    };

    HashTable<void *, allocation_record, base_allocator> record_table;
  public:

    check_allocator(const base_allocator &a = base_allocator()) : base_allocator(a), record_table(a) {}
    check_allocator(const check_allocator &c) : base_allocator(base_allocator(c)), record_table(c) {}

    ~check_allocator() {
      typename HashTable<void *, allocation_record, base_allocator>::Cursor rc(record_table);
      for (rc.SetToFirst(); rc.Valid(); rc.SetToNext()) {
        allocation_record &r = record_table[rc];
        printf("Leftover pointer: %p[%ld] %s\n", record_table.KeyAt(rc),r.size, r.name?r.name:"");
      }
      printf("Allocator check complete\n");
    }

    void *allocate(size_t size, const char *name=NULL) {
      void *ret = base_allocator::allocate(size+2*sizeof(void *), name);
      memcpy(ret, &ret, sizeof(void *));
      memcpy((char *)ret+size+sizeof(void *), &ret, sizeof(void *));
      ret = (void *)((char *)ret + sizeof(void *));

      HashIndex hi;
      if (record_table.Locate(ret, hi)){
        struct allocation_record x = record_table[hi];

        printf("duplicate return: %p\n*alloc1=%s[%ld]\n*alloc2=%s[%ld]\n",
               ret,  x.name?x.name:"", x.size,name?name:"",  size);
      }
      record_table.Add(ret, allocation_record(size, name), hi);

      return ret;
    }
    void deallocate(void *pointer, size_t size, const char *name=NULL) {
      HashIndex hi;
      if (!record_table.Locate(pointer, hi)) {
        printf("not found: %p %ld (%s)\n", pointer, size, name?name:"");
        return;
      }
      struct allocation_record x = record_table[hi];
      record_table.Remove(hi);
      if (size != x.size) {
        printf("mismatched size: %ld!=%ld\n*alloc=%s\n*dealloc=%s\n",
               size, x.size, name?name:"", x.name?x.name:"");
        CS2Assert(false, ("Overflow: %p", pointer));
      }
      pointer = (void *)((char *)pointer - sizeof(void *));
      if (memcmp(pointer, &pointer, sizeof(void *)))
        printf("buffer underflow: %p!=%p\n*alloc=%s[%ld]\n*dealloc=%s[%ld]\n",
               pointer, *(void **)pointer, x.name?x.name:"", x.size,name?name:"",  size);
      if (memcmp((char *)pointer+size+sizeof(void *), &pointer, sizeof(void *))) {
        printf("buffer overflow: %p!=%p\n*alloc=%s[%ld]\n*dealloc=%s[%ld]\n",
               pointer, *(void **)((char *)pointer+size+4), x.name?x.name:"", x.size,name?name:"", size);
        CS2Assert(false, ("Overflow: %p", pointer));
      }

      base_allocator::deallocate(pointer, size+2*sizeof(void *), name);
    }
    void *reallocate(size_t newsize, void *pointer, size_t size, const char *name = NULL) {
      HashIndex hi;
      if (!record_table.Locate(pointer, hi)) {
        printf("not found: %p %d (%s)\n", pointer, hi, name?name:NULL);
        return NULL;
      }

      struct allocation_record x = record_table[hi];
      record_table.Remove(hi);

      pointer = (void *)((char *)pointer - sizeof(void *));

      if (size != x.size)
        printf("mismatched size: %ld!=%ld\n*alloc=%s\n*dealloc=%s\n",
               size, x.size, x.name, name);
      if (memcmp(pointer, &pointer, sizeof(void *)))
        printf("buffer underflow: %p!=%p\n*alloc=%s[%ld]\n*dealloc=%s[%ld]\n",
               pointer, *(void **)pointer, x.name?x.name:"", x.size,name?name:"", size);
      if (memcmp((char *)pointer+size+sizeof(void *), &pointer, sizeof(void *))){
        printf("buffer overflow: %p!=%p\n*alloc=%s[%ld]\n*dealloc=%s[%ld]\n",
               pointer, *(void **)((char *)pointer+size+4), x.name?x.name:"", x.size,name?name:"", size);
      }
      void *ret = base_allocator::reallocate(newsize+2*sizeof(void *), pointer, size+2*sizeof(void *), name);
      memcpy(ret, &ret, sizeof(void *));
      memcpy((char *)ret+newsize+sizeof(void *), &ret, sizeof(void *));
      ret = (void *)((char *)ret + sizeof(void *));

      record_table.Add(ret, allocation_record(newsize, name), hi);
      return ret;
    }

    template <class ostr, class allocator> ostr& stats(ostr &o, allocator &a) { return base_allocator::stats(o, a);}
  };

  template <size_t segmentsize = 65536, uint32_t segmentcount= 10, class base_allocator = CS2::malloc_allocator>
  class heap_allocator : private base_allocator {
  private:

    class Segment {
      Segment *next, *prev;
      void *freelist;
      uint32_t alloc;
      uint32_t freed;

    public:
      Segment(Segment *n) : next(n), prev(NULL), freelist(NULL), alloc(0), freed(0) {
        if (n) n->prev=this;
      }

      void *allocate(uint32_t index) {
        if (freelist) {
          void *ret = (void *)freelist;
          freelist = *(void **)ret;
          freed-=1;

          CS2Assert(holds_address(ret),
                    ("Found pointer outside of segment: %p", ret));
          return ret;
        }

        if (!is_full(index)) {
          void *ret = (char *)this + sizeof(Segment) + element_size(index)*alloc;
          alloc+=1;
          return ret;
        }
        return NULL;
      }
      void deallocate(void *pointer) {
        *(void **)pointer = freelist;
        freelist=pointer;
        freed+=1;
      }
      bool holds_address(void *pointer) {
        return (this <= pointer && pointer < ((char *)this)+segmentsize);
      }
      bool is_full(uint32_t index ) {
        return alloc == element_count(index);
      }
      bool is_empty() {
        return alloc == freed;
      }

      static uint32_t segment_index(size_t size) {
        if (size<=sizeof(void *)) return 1;
        if (size<= element_size(segmentcount-1)) {

          uint32_t halfsegment=segmentcount/2;

          if (size<=element_size(halfsegment)) {
            uint32_t i=2;
            for (; i<halfsegment; i++) {
              if (size<=element_size(i)) return i;
            }
            return i;
          } else {
            uint32_t i=halfsegment+1;
            for (; i<segmentcount-1; i++) {
              if (size<=element_size(i)) return i;
            }
            return i;
          }
        }
        return 0;
      }
      static size_t element_size(uint32_t index) {
        // Ensure allocation granule can hold a pointer for free list
        return size_t(sizeof(void *))<<(index-1);
      }
      static size_t element_count(uint32_t index) {
        return (segmentsize-sizeof(Segment)) / element_size(index);
      }
      Segment *next_segment() {
        return next;
      }

      size_t freelist_size(uint32_t index) {
        return freed*element_size(index);
      }

      uint32_t get_alloc(uint32_t index) { return alloc*element_size(index); }
      uint32_t get_free(uint32_t index)  { return (element_count(index)-alloc)*element_size(index);}

      Segment *move_to_head(Segment *head) {
        if (prev!=NULL) {              // already at the head?
          prev->next = next;           // unlink myself
          if (next) next->prev = prev;

          next=head;                // link at the head
          if (head)head->prev=this;
          prev=NULL;
        }
        return this;
      }
      Segment *unlink(Segment *head) {
        if (prev!=NULL) {              // already at the head?
          prev->next = next;           // unlink myself
          if (next) next->prev = prev;
          return head;
        } else {
          if (next) next->prev = NULL;
          return next;
        }
      }

      void *operator new (size_t, void *ptr) {
        return ptr;
      }
    };

    Segment *segments[segmentcount];
  public:
    heap_allocator(const base_allocator &a=base_allocator()) : base_allocator(a) {
      for (uint32_t i=0; i<segmentcount; i++)
        segments[i]=NULL;
    }
    heap_allocator(const heap_allocator &c) : base_allocator(c) {
      for (uint32_t i=0; i<segmentcount; i++)
        segments[i]=NULL;
    }

    template <class ostr, class allocator> ostr& stats(ostr &o, allocator &a) {
      o << "CS2 heap allocator\n"
        << "Segment size= " << segmentsize << " bytes\n";

      for (uint32_t i=1; i<segmentcount; i++) {
        Segment *s = segments[i];
        size_t numsegs=0, totalsize=0, allocsize=0, freesize=0;
        while (s) {
           numsegs++;
           totalsize += segmentsize;
           allocsize += s->get_alloc(i);
           freesize += s->get_free(i);
           s = s->next_segment();
        }

        if (totalsize)
        o << " segment[" <<i << "](" << s->element_size(i) << ")"
          << " count=" << numsegs
          << " size(alloc,free)=(" << allocsize << "," << freesize  << ":" << (allocsize*100/totalsize) << "%) "
          << " pad=(" << totalsize-allocsize-freesize << ":" << (totalsize-allocsize-freesize)*100/totalsize << "%)\n";
      }
      return base_allocator::stats(o, a);
    }

    ~heap_allocator() {
      for (uint32_t i=0; i<segmentcount; i++) {
        Segment *s = segments[i];
        while (s) {
          Segment *next = s->next_segment();
          base_allocator::deallocate(s, segmentsize);
          s = next;
        }
        segments[i]=NULL;
      }
    }

    Segment *new_segment(Segment *next, const char *name) {
      void *ret = base_allocator::allocate(segmentsize, name);
      return new (ret) Segment(next);
    }

    void *allocate(size_t size, const char *name=NULL) {
      uint32_t ix = Segment::segment_index(size);
      if (ix==0) {
        return base_allocator::allocate(size, name);
      }

      for (Segment *s = segments[ix]; s; s=s->next_segment()) {
        void *ret = s->allocate(ix);
        if (ret) {
          if (s!=segments[ix])
            segments[ix]= s->move_to_head(segments[ix]);
          return ret;
        }
      }
      segments[ix] = new_segment(segments[ix], name);
      return segments[ix]->allocate(ix);
    }
    void deallocate(void *pointer, size_t size, const char *name = NULL) {
      uint32_t ix = Segment::segment_index(size);
      if (ix==0) {
        return base_allocator::deallocate(pointer, size, name);
      }

      for (Segment *s = segments[ix]; s; s=s->next_segment()) {
        if (s->holds_address(pointer)) {
          s->deallocate(pointer);
          if (s->is_empty()) {
            segments[ix] = s->unlink(segments[ix]);
            base_allocator::deallocate(s, segmentsize, name);
          } else if (s!=segments[ix])
            segments[ix]= s->move_to_head(segments[ix]);
          return;
        }
      }
      CS2Assert(false, ("Could not find pointer to delete: %p", pointer));
    }
    void *reallocate(size_t newsize, void *pointer, size_t size, const char *name = NULL) {
      uint32_t ix = Segment::segment_index(size);
      uint32_t nix = Segment::segment_index(newsize);

      if (ix==nix)  {
        if (ix==0) {
          return base_allocator::reallocate(newsize, pointer, size, name);
        }
        return pointer;
      }

      void * npointer = allocate(newsize, name);
      memcpy(npointer, pointer, newsize<size?newsize:size);
      deallocate(pointer, size, name);

      return npointer;
    }
  };

  template <class base_allocator>
  class shared_allocator {
    base_allocator &base;
  public:
    shared_allocator(base_allocator &b = base_allocator::instance()) : base(b) {}

    void *allocate(size_t size, const char *name = NULL) {
        return base.allocate(size, name);
    }

    void deallocate(void *pointer, size_t size, const char *name = NULL) {
      return base.deallocate(pointer, size, name);
    }

    void *reallocate(size_t newsize, void *pointer, size_t size, const char *name=NULL) {
      return base.reallocate(newsize, pointer, size, name);
    }

    template <class ostr, class allocator> ostr& stats(ostr &o, allocator &a) { return base.stats(o, a);}

    shared_allocator & operator = (const shared_allocator & a2 ) {
      // no need to copy the allocator being shared
      return *this;
    }
    friend bool operator ==(const shared_allocator &left, const shared_allocator &right) { return &left.base == &right.base; }

    friend bool operator !=(const shared_allocator &left, const shared_allocator &right) { return !(operator ==(left, right)); }
  };

  template <class base_allocator>
    class named_allocator: private base_allocator {
    const char *allocator_name;
  public:
    named_allocator(base_allocator b = base_allocator(), const char *name = NULL) : base_allocator(b), allocator_name(name) {}

    void *allocate(size_t size, const char *name = NULL) {
      return base_allocator::allocate(size, name?name:allocator_name);
    }

    void deallocate(void *pointer, size_t size, const char *name = NULL) {
      return base_allocator::deallocate(pointer, size, name?name:allocator_name);
    }

    void *reallocate(size_t newsize, void *pointer, size_t size, const char *name=NULL) {
      return base_allocator::reallocate(newsize, pointer, size, name?name:allocator_name);
    }

    template <class ostr, class allocator> ostr& stats(ostr &o, allocator &a) { return base_allocator::stats(o, a);}

  };

  template <size_t segmentsize = 65536, class base_allocator = CS2::allocator>
  class arena_allocator : private base_allocator {

    struct Segment {
      struct Segment *next;
      size_t size;
    };

  public:
    arena_allocator(base_allocator b = base_allocator() ) : base_allocator(b), segment(NULL), allocated(0) {}
    ~arena_allocator() {
      Segment *s = segment;

      while (s) {
        Segment *next = s->next;
        base_allocator::deallocate(s, s->size);
        s = next;
      }
    }

    static size_t arena_size() { return segmentsize - sizeof(Segment);}

    void *allocate(size_t size, const char *name=NULL) {
      if (size % sizeof(size_t)) size = (size/sizeof(size_t)+1)*sizeof(size_t);

      void *ret;
      if (segment && size>=arena_size()) {
        Segment *new_segment = (Segment *)base_allocator::allocate(sizeof(Segment)+size, name);
        new_segment->size = sizeof(Segment)+size;
        new_segment->next = segment->next;
        segment->next=new_segment;

        ret = (void *)((char *)new_segment + sizeof(Segment));
      } else if (segment==NULL || allocated+size>arena_size()) {

        Segment *new_segment = (Segment *)base_allocator::allocate(segmentsize, name);
        new_segment->size = segmentsize;
        new_segment->next = segment;

        segment = new_segment;
        ret = (void*) ((char *)new_segment + sizeof(Segment));
        allocated = size;
      } else {
        ret = (void*) ((char *)segment + sizeof(Segment) + allocated);
        allocated+=size;
      }
      return ret;
    }

    void deallocate(void *pointer, size_t size, const char *name=NULL) {
      // no deallocation
    }

    void *reallocate(size_t newsize, void *pointer, size_t size, const char *name=NULL) {
      if (newsize<=size) return pointer;
      void *ret = allocate(newsize, name);
      memcpy(ret, pointer, size);
      return ret;
    }

    arena_allocator & operator = (const arena_allocator & a2 ) {
      // no need to copy the allocator being shared
      return *this;
    }

    template <class ostr, class allocator> ostr& stats(ostr &o, allocator &a) {
      uint32_t c = 0;
      for (Segment *s = segment; s; s=s->next) c+=1;

      o << "Arena: Segments allocated=" << c << "\n"
        << "Arena: Top segment allocation: " << allocated << "/" << segmentsize << "\n";

      return base_allocator::stats(o, a);
    }

  private:
    Segment *segment;
    size_t allocated;
  };

  class allocator : public malloc_allocator { };
}

#endif // CS2_ALLOCATOR_H
