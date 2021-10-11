// -*-C++-*- directory.hpp
// <!!----------------------------------------------------------------------> 
// <!! Copyright (C) 1998 - 2000 Dietmar Kuehl, Claas Solutions GmbH > 
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
// Title:  An input iterator used to list the entries in a directory 
// Version: $Id: directory.hpp,v 1.3 2000/07/15 07:51:15 kuehl Exp $

// -------------------------------------------------------------------------- 

#if !defined(BOOST_DIRECTORY_HPP)
#define BOOST_DIRECTORY_HPP 1

// --------------------------------------------------------------------------

#include <iterator>
#include <string>
#include <ctime>
#include <stdexcept>
#include <iostream>
#include <boost/config.hpp>

#if defined(_CXXRT_STD)
#  define BOOST_CSTD _CXXRT_STD
#else
#  define BOOST_CSTD
#endif

namespace boost
{
#if defined(__GNUG__) && !defined(_CXXRT_STD)
  template <class Category, class T, class Distance = BOOST_CSTD ptrdiff_t,
    class Pointer = T*, class Reference = T&>
  struct iterator
  {
    typedef T         value_type;
    typedef Distance  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
    typedef Category  iterator_category;
  };
#else
  template <class Category, class T, class Distance = BOOST_CSTD ptrdiff_t,
    class Pointer = T*, class Reference = T&>
    struct BOOST_DECL iterator:
#  if defined(_MSC_VER)
    std::iterator<Category, T, Distance>
#  else
    std::iterator<Category, T, Distance, Pointer, Reference>
#  endif
    {
    };
  // using std::iterator;
#endif
}

// --------------------------------------------------------------------------

namespace boost
{
  namespace filesystem
  {
    struct BOOST_DECL pimpl_base
    {
      pimpl_base(): m_count(0) {}
      virtual ~pimpl_base() {}
      int m_count;
    };

    template <class Pimpl>
      class BOOST_DECL pimpl_ptr
      {
      public:
	pimpl_ptr(Pimpl* ptr): m_ptr(ptr) { ++(m_ptr->m_count); }
	pimpl_ptr(pimpl_ptr const& ptr): m_ptr(ptr.m_ptr) { ++(m_ptr->m_count); }
	~pimpl_ptr() { if (--(m_ptr->m_count) == 0) delete m_ptr; }
	pimpl_ptr& operator= (pimpl_ptr const& ptr)
	  {
	    pimpl_base* del = m_ptr;
	    m_ptr = ptr.m_ptr;
	    ++(m_ptr->m_count);
	    if (--(del->m_count) == 0)
	    delete del;
	    return *this;
	  }
	
	Pimpl& operator*() const { return *static_cast<Pimpl*>(m_ptr); }
	Pimpl* operator->() const { return static_cast<Pimpl*>(m_ptr); }
	Pimpl* get() const { return static_cast<Pimpl*>(m_ptr); }
	
	template <class Base>
	operator pimpl_ptr<Base> () const { return pimpl_ptr<Base>(static_cast<Pimpl*>(m_ptr)); }
	
      private:
	pimpl_base* m_ptr;
      };
    
    template <class cT>
    class BOOST_DECL basic_file
    {
    public:
      struct BOOST_DECL representation;
      typedef basic_file const* const_iterator;
      
      static const_iterator roots_begin();
      static const_iterator roots_end();

      basic_file(pimpl_ptr<representation> const&);
      basic_file(std::basic_string<cT> const&);
      basic_file(basic_file<cT> const&, std::basic_string<cT> const&);

      std::basic_string<cT> str() const;
      cT const*             c_str() const;
      
      // the following stuff is for internal use only:
      pimpl_ptr<representation> const& get_rep() const { return m_rep; }
    private:
      pimpl_ptr<representation> m_rep;
    };

    template <class cT>
    class BOOST_DECL basic_dir_it:
      public boost::iterator<std::input_iterator_tag, std::basic_string<cT> >
    {
    public:
      typedef ptrdiff_t               difference_type;
      typedef std::basic_string<cT>   value_type;
      typedef std::basic_string<cT> * pointer;
      typedef std::basic_string<cT> & reference;
      typedef std::input_iterator_tag iterator_category;
      
      class proxy
      {
        friend basic_dir_it<cT>;
        proxy(std::basic_string<cT> const &ent): entry(ent) {}
      public:
        std::basic_string<cT> operator*() const { return entry; }
      private:
        std::basic_string<cT> entry;
      };
      
      basic_dir_it();
      basic_dir_it(std::basic_string<cT> const&);
      
      std::basic_string<cT> operator* () const;
      basic_dir_it<cT>&     operator++ ();
      proxy                 operator++ (int);
      
      bool operator== (basic_dir_it<cT> const&) const;
      bool operator!= (basic_dir_it<cT> const&) const;

      // the following stuff is for internal use only:
      pimpl_ptr<basic_file<cT>::representation> get_rep() const;
    private:
      struct representation;
      pimpl_ptr<representation> m_rep;
    };
    
    template <class Property, class cT>
      class property
      {
      public:
	typedef typename Property::value_type value_type;

	static bool is_meaningful();
	static value_type get(pimpl_ptr<typename basic_file<cT>::representation> const&);
	static void set(pimpl_ptr<typename basic_file<cT>::representation> const&, value_type const&);
      };

#if defined(_MSC_VER)
    template <class Property>
      class BOOST_DECL get
      {
      public:
	get(basic_file const& f): m_rep(f.get_rep()) {}
	get(basic_dir_it const& d): m_rep(d.get_rep()) {}
	
	operator typename Property::value_type() const
	  {
	    return property<Property>::get(m_rep);
	  }

      private:
	get(get const&): m_rep(0) {}
	void operator= (get const&) {}

	pimpl_ptr<basic_file::representation> m_rep;
      };

    template <class Property, class cT, class traits>
      inline std::basic_ostream<cT, traits>&
      operator<< (std::basic_ostream<cT, traits>& os, get<Property> const& prop)
      {
	return os << static_cast<typename Property::value_type>(prop);
      }

    template <class Property>
      class BOOST_DECL set
      {
      public:
	set(basic_file const& f,
	    typename Property::value_type val)
	{
	  property<Property>::set(f.get_rep(), val);
	}
	set(basic_dir_it const& d,
	    typename Property::value_type val)
	{
	  property<Property>::set(d.get_rep(), val);
	}
      };
#else
    template <class Property, class cT>
      typename Property::value_type get(basic_file<cT> const& f)
      {
	return property<Property, cT>::get(f.get_rep());
      }
    template <class Property, class cT>
      typename Property::value_type get(basic_dir_it<cT> const& d)
      {
	return property<Property, cT>::get(d.get_rep());
      }
    template <class Property, class cT>
      void set(basic_file<cT> const& f, typename Property::value_type const& v)
      {
	return property<Property, cT>::set(f.get_rep(), v);
      }
    template <class Property, class cT>
      void set(basic_dir_it<cT> const& d, typename Property::value_type const& v)
      {
	return property<Property, cT>::set(d.get_rep(), v);
      }
#endif

    template<class T> struct property_type { typedef T value_type; };

    struct size: property_type<BOOST_CSTD size_t> {};
    struct mtime: property_type<BOOST_CSTD time_t const*> {};
    struct atime: property_type<BOOST_CSTD time_t const*> {};
    struct ctime: property_type<BOOST_CSTD time_t const*> {};
    struct links: property_type<BOOST_CSTD size_t> {};
    struct blocks: property_type<BOOST_CSTD size_t> {};
    struct blksize: property_type<BOOST_CSTD size_t> {};

    struct is_hidden: property_type<bool> {};
    struct is_regular: property_type<bool> {};
    struct is_directory: property_type<bool> {};
    struct is_link: property_type<bool> {};
    struct is_char_device: property_type<bool> {};
    struct is_block_device: property_type<bool> {};
    struct is_fifo: property_type<bool> {};
    struct is_socket: property_type<bool> {};

    struct user_read: property_type<bool> {};
    struct user_write: property_type<bool> {};
    struct user_execute: property_type<bool> {};

    struct group_read: property_type<bool> {};
    struct group_write: property_type<bool> {};
    struct group_execute: property_type<bool> {};

    struct other_read: property_type<bool> {};
    struct other_write: property_type<bool> {};
    struct other_execute: property_type<bool> {};

    struct set_uid: property_type<bool> {};
    struct set_gid: property_type<bool> {};
    struct sticky: property_type<bool> {};

    struct uid: property_type<unsigned long> {};
    struct gid: property_type<unsigned long> {};
    struct uname: property_type<std::string> {};
    struct gname: property_type<std::string> {};

    typedef basic_file<char>      file;
    typedef basic_dir_it<char>    dir_it;
    typedef basic_file<wchar_t>   wfile;
    typedef basic_dir_it<wchar_t> wdir_it;

  } // namespace filesystem
} // namespace boost

namespace std
{
  template<>
  struct iterator_traits<boost::filesystem::basic_dir_it<char> >
  {
    typedef boost::filesystem::basic_dir_it<char>::difference_type   difference_type;
    typedef boost::filesystem::basic_dir_it<char>::value_type        value_type;
    typedef boost::filesystem::basic_dir_it<char>::pointer           pointer;
    typedef boost::filesystem::basic_dir_it<char>::reference         reference;
    typedef boost::filesystem::basic_dir_it<char>::iterator_category iterator_category;
  };

  template<>
  struct iterator_traits<boost::filesystem::basic_dir_it<wchar_t> >
  {
    typedef boost::filesystem::basic_dir_it<wchar_t>::difference_type   difference_type;
    typedef boost::filesystem::basic_dir_it<wchar_t>::value_type        value_type;
    typedef boost::filesystem::basic_dir_it<wchar_t>::pointer           pointer;
    typedef boost::filesystem::basic_dir_it<wchar_t>::reference         reference;
    typedef boost::filesystem::basic_dir_it<wchar_t>::iterator_category iterator_category;
  };
} // namespace std

// --------------------------------------------------------------------------

#endif /* BOOST_DIRECTORY_HPP */
