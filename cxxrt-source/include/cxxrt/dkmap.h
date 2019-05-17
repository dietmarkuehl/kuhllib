// <!!-*-C++-*- file: _CS_map.h --->
// -------------------------------------------------------------------------- 
// Copyright (c) 2002 Dietmar Kuehl

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject
// to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
// KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// -------------------------------------------------------------------------- 
// Author:  Dietmar Kuehl <http://www.dietmar-kuehl.de/>
// Title:   Until the IOStream stuff is implemented, I can't use the standard classes...
// Version: $Id: dkmap.h,v 1.4 2006/04/07 07:07:13 dietmar_kuehl Exp $
// -----------------------------------------------------------------------------

#if !defined(__DKMAP_H__)
#define __DKMAP_H__

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -----------------------------------------------------------------------------

template <class _Key, class _Value>
class _CS_map
{
public:
  struct item // modeled after 'std::pair'
  {
    _Key const first;
    _Value     second;
    item*     next;

    ~item() {}
    item(_Key const &f, _Value const &s = _Value()):
      first(f),
      second(s),
      next(0)
    {
    }
  private:
    item(item const &);
    void operator= (item const &);
  };

  class iterator
  {
    friend class _CS_map;
  public:
    iterator(): it(0) {}
    iterator(iterator const &i): it(i.it) {}
    explicit iterator(item *i): it(i) {}

    iterator &operator= (iterator const &i) { it = i.it; return *this; }
    ~iterator() {}

    item& operator* () const { return *it; }
    item* operator-> () { return it; }
    item* operator-> () const { return it; }
    iterator &operator++ () { it = it->next; return *this; }

    bool operator== (iterator const& i) const { return it == i.it; }
    bool operator!= (iterator const& i) const { return it != i.it; }

#if !defined(_MSC_VER)
  private:
#endif
    item *it;
  };

  typedef iterator const_iterator;

  _CS_map():
    _begin(0)
  {
  }
  ~_CS_map()
  {
                clear();
  }

  iterator begin() { return iterator(_begin); }
  iterator end() { return iterator(); }
  const_iterator begin() const { return iterator(_begin); }
  const_iterator end() const { return const_iterator(); }

  iterator find(_Key const &_key)
  {
    for (typename _CS_map::iterator it(_begin); it != typename _CS_map::iterator(); ++it)
      if ((*it).first == _key)
        return it;
    return typename _CS_map::iterator();
  }
  const_iterator find(_Key const &_key) const
  {
    for (const_iterator it(_begin); it != iterator(); ++it)
      if ((*it).first == _key)
        return const_iterator(it);
    return const_iterator();
  }

  _Value &operator[] (_Key const &_key)
  {
    iterator it(this->find(_key));
    if (it == iterator())
      {
        item *i = new item(_key);
        i->next = _begin;
        _begin = i; 
        it = iterator(i);
      }
    return (*it).second;
  }

        void clear()
        {
    for (iterator it(_begin); it != iterator(); )
      {
        iterator tmp = it;
        ++it;
        delete tmp.it;
      }

                _begin = 0;
        }
private:
  _CS_map(_CS_map const &);
  void operator= (_CS_map const &);

  item *_begin;
};

// -----------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* __DKMAP_H__ */
