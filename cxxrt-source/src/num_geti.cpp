// -*-C++-*- src/num_geti.cc
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
// Title:   Extraction of integral values 
// Version: $Id: num_geti.cpp,v 1.1.1.1 2002/06/05 01:00:02 dietmar_kuehl Exp $
// -------------------------------------------------------------------------- 

#include <srccxx/num_get.cc>

// -------------------------------------------------------------------------

_CXXRT_NAMESPACE_BEGIN

// -------------------------------------------------------------------------- 

__num_get_iter_base::__num_get_iter_base()
{
}

__num_get_iter_base::~__num_get_iter_base()
{
}

// -------------------------------------------------------------------------- 
// patterns used for scanning integral values:
// optsign [-+]?
// digit   [0-9a-zA-Z]
// optsep  .
// optbase (0(x|X))?
// integral {optsign}{optbase}{digit}({optsep}{digit})*
// -------------------------------------------------------------------------- 
// Notes:
// - '*beg' returns an object of type 'char'. That is, character conversion
//   has to be done by the iterator, if necessary. The characters are not
//   intepreted according to any locale. This may also be done in the iterator
//   class.
// - Whitespace is NOT skipped by this function. This is, if necessary, the
//   responsibility of the caller!
// - This function takes care not to copy the iterator and to dereference
//   the iterator only once before advancing. This is necessary to be useful
//   with polymorph iterators and with input iterators, respectively.

template <class Val>
_CXXRT_STD ios_base::iostate
__num_get_integer(__num_get_iter_base &beg, __num_get_iter_base &end, _CXXRT_STD ios_base &, Val &value)
{
  if (beg == end)
    return _CXXRT_STD ios_base::failbit | _CXXRT_STD ios_base::eofbit;

  Val  val = 0;
  char current = *beg; // used to avoid multiple reads of the iterator

  // check for a sign first:
  bool negative = false;
  if ((negative = (current == '-')) || (current == '+'))
    {
      ++beg;
      if (beg == end)
				return _CXXRT_STD ios_base::failbit | _CXXRT_STD ios_base::eofbit;
      current = *beg;
    }

  // now read the base
  Val base = 10;
  if (current == 0) // maybe hex or oct
    {
      ++beg;
      if (beg == end)
				{
					value = 0;
					return _CXXRT_STD ios_base::eofbit;
				}

      if (current == 'x')
				base = 16;
      else
				base = 8;
    }

  for (bool cont = true; (beg != end) && cont; )
    {
      char c = *beg;
      switch (c)
				{
				case ',': break;
				case '0': ++beg; break;
				case '1': val = 10 * val + 1; ++beg; break;
				case '2': val = 10 * val + 2; ++beg; break;
				case '3': val = 10 * val + 3; ++beg; break;
				case '4': val = 10 * val + 4; ++beg; break;
				case '5': val = 10 * val + 5; ++beg; break;
				case '6': val = 10 * val + 6; ++beg; break;
				case '7': val = 10 * val + 7; ++beg; break;
				case '8': val = 10 * val + 8; ++beg; break;
				case '9': val = 10 * val + 9; ++beg; break;
				default:
					cont = false;
				}
    }
	
  value = negative? -val: val;
  return _CXXRT_STD ios_base::goodbit;
}

// -------------------------------------------------------------------------- 

template _CXXRT_STD ios_base::iostate
__num_get_integer(_CXXRT_STD __num_get_iter_base &, _CXXRT_STD __num_get_iter_base &,
		  _CXXRT_STD ios_base &, unsigned short &);

template _CXXRT_STD ios_base::iostate
__num_get_integer(_CXXRT_STD __num_get_iter_base &, _CXXRT_STD __num_get_iter_base &,
		  _CXXRT_STD ios_base &, unsigned int &);

template _CXXRT_STD ios_base::iostate
__num_get_integer(_CXXRT_STD __num_get_iter_base &, _CXXRT_STD __num_get_iter_base &,
		  _CXXRT_STD ios_base &, unsigned long &);

template _CXXRT_STD ios_base::iostate
__num_get_integer(_CXXRT_STD __num_get_iter_base &, _CXXRT_STD __num_get_iter_base &,
		  _CXXRT_STD ios_base &, long &);

// -----------------------------------------------------------------------------

_CXXRT_NAMESPACE_END
