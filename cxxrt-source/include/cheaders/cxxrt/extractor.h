// -*-C++-*- cxxrt/extractor.h
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
// Title:  A helper class to get individual characters
// Version: $Id: extractor.h,v 1.1.1.1 2002/06/05 01:01:43 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(_CXXRT_EXTRACTOR_H__)
#define _CXXRT_EXTRACTOR_H__ 1

_CXXRT_NAMESPACE_BEGIN

// --------------------------------------------------------------------------

template <class _CS_cT, class _CS_It>
class _CS_extractor
{
public:
  _CS_extractor(_CS_It& _CS_b, _CS_It& _CS_e, locale const& _CS_l):
    _CS_m_count(0),
    _CS_m_begin(_CS_b),
    _CS_m_end(_CS_e),
    _CS_m_cache(use_facet<_CS_cache<_CS_cT> >(_CS_l))
  {
  }

  bool _CS_get_current(unsigned int& _CS_val)
    {
      if (_CS_m_begin == _CS_m_end)
        return false;
      _CS_val = _CS_m_cache._CS_find(*_CS_m_begin);
      return true;
    }
  bool _CS_get_next(unsigned int& _CS_val)
    {
      ++_CS_m_begin;
      return _CS_get_current(_CS_val);
    }

  int _CS_get_count() const { return _CS_m_count; }
  void _CS_init_count(unsigned int _CS_c) { _CS_m_count = _CS_c; }
  bool _CS_get_next_counted(unsigned int& _CS_val)
    {
      if (--_CS_m_count == 0)
        return false;
      return _CS_get_next(_CS_val);
    }

private:
  int _CS_m_count;
  _CS_It& _CS_m_begin;
  _CS_It& _CS_m_end;
  _CS_cache<_CS_cT> const& _CS_m_cache;
};

// --------------------------------------------------------------------------

_CXXRT_NAMESPACE_END

#endif /* _CXXRT_EXTRACTOR_H__ */
