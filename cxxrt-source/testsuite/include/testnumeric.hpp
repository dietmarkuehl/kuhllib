// -*-C++-*- testnumeric.hpp
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
// Author: Dietmar Kuehl <http://www.dietmar-kuehl.de/>
// Title:  a type used to test instantiation of the numeric classes
// Version: $Id: testnumeric.hpp,v 1.1.1.1 2002/06/05 01:03:52 dietmar_kuehl Exp $ 
// -------------------------------------------------------------------------- 

#if !defined(TESTNUMERIC_HPP)
#define TESTNUMERIC_HPP 1

#if !defined(_CXXRT_STD)
#  define TTSTD std::
#else
#  define TTSTD _CXXRT_STD
#endif

namespace tst
{

// --------------------------------------------------------------------------

  class number
  {
  public:
    number(): m_val(0.0) {}
    explicit number(double v): m_val(v) {}
    number(number const& n): m_val(n.m_val) {}
    ~number() {}
    number& operator= (number const& n) { m_val = n.m_val; return *this; }
    number& operator= (double const& n) { m_val = n; return *this; }

    number& operator+= (number const& n) { m_val += n.m_val; return *this; }
    number& operator-= (number const& n) { m_val -= n.m_val; return *this; }
    number& operator*= (number const& n) { m_val *= n.m_val; return *this; }
    number& operator/= (number const& n) { m_val /= n.m_val; return *this; }

    number operator+ () const { return *this; }
    number operator- () const { return number(-m_val); }

    bool operator== (number const& n) const { return m_val == n.m_val; }
    bool operator!= (number const& n) const { return m_val != n.m_val; }
    bool operator<= (number const& n) const { return m_val <= n.m_val; }
    bool operator<  (number const& n) const { return m_val <  n.m_val; }
    bool operator>= (number const& n) const { return m_val >= n.m_val; }
    bool operator>  (number const& n) const { return m_val >  n.m_val; }

    double value() const { return m_val; }
    double& value() { return m_val; }

  private:
    double m_val;
  };

  number operator+ (number n1, number const& n2) { return n1 += n2; }
  number operator- (number n1, number const& n2) { return n1 -= n2; }
  number operator* (number n1, number const& n2) { return n1 *= n2; }
  number operator/ (number n1, number const& n2) { return n1 /= n2; }

  TTSTD ostream& operator<< (TTSTD ostream& os, number const& n) { return os << n.value(); }
  TTSTD istream& operator>> (TTSTD istream& is, number& n) { return is >> n.value(); }

  template <class T>
  bool feq(T const& v1, T const& v2)
    {
      T diff = (v1 - v2) < T(0.0)? -(v1 - v2): (v1 - v2);
      if (v1 < T(0.0000001))
	return diff < T(0.0000001)? true: false;
      return diff < (v1 < T(0.0)? -v1: v1) / T(100000.0)? true: false;
    }

// --------------------------------------------------------------------------

} // namespace tst

#endif /* TESTNUMERIC_HPP */
