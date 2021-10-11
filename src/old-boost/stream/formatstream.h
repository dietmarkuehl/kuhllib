// -*-C++-*- formatstream.h
// <!!----------------------------------------------------------------------> 
// <!! Copyright (C) 1999 Dietmar Kuehl, Claas Solutions GmbH > 
// <!!> 
// <!! Permission to use, copy, modify, distribute and sell this > 
// <!! software for any purpose is hereby granted without fee, provided > 
// <!! that the above copyright notice appears in all copies and that > 
// <!! both that copyright notice and this permission notice appear in > 
// <!! supporting documentation. Dietmar Kuehl and Claas Solutions make no > 
// <!! representations about the suitability of this software for any > 
// <!! purpose. It is provided "as is" without express or implied warranty. > 
// <!!----------------------------------------------------------------------> 

// Author: Dietmar Kuehl dietmar.kuehl@claas-solutions.de 
// Title:  A class allow format strings to be used with IOStreams
// Version: $Id: formatstream.h,v 1.2 1999/10/18 12:17:46 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(FORMATSTREAM_H)
#define FORMATSTREAM_H 1

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

// -------------------------------------------------------------------------- 

namespace boost
{
#if 0
} // for emacs indentation
#endif

// --------------------------------------------------------------------------

extern void format_specs_set(std::ostream& out, std::string const& spec);

template <typename T>
struct format_specs
{
  static inline void set(std::ostream& out, std::string const& spec)
    {
      format_specs_set(out, spec);
    }
};

template <typename T>
struct format_print
{
  static inline void do(std::ostream& out, T const& val)
    {
      out << val;
    }
};

class format
{
  class format_rep;

public:
  class too_few_arguments: public std::logic_error
  {
  public:
    too_few_arguments();
  };

  format(std::ostream&, std::string const&);
  ~format();

  template <typename T>
  format const& operator<< (T const& val)
    {
      format_specs<T>::set(...);
    }
private:
  format(format const&);         // prohibit use of copy ctor
  void operator=(format const&); // prohibit use of copy assignment
  
private:
  format_rep* m_rep;
};

// --------------------------------------------------------------------------

#if 0
{ // for emacs indentation
#endif
}

#endif /* FORMATSTREAM_H */
