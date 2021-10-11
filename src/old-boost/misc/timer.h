// -*-C++-*- timer.h
// <!!----------------------------------------------------------------------> 
// <!! Copyright (C) 1998 Dietmar Kuehl, Claas Solutions GmbH > 
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
// Title:  A class used for timing with higher precision than time()
// Version: $Id: timer.h,v 1.1.1.1 1999/08/19 23:18:24 kuehl Exp $ 

// -------------------------------------------------------------------------- 

#if !defined(BOOST_TIMER_H)
#define BOOST_TIMER_H 1

// --------------------------------------------------------------------------

#include <iosfwd>

namespace boost
{

// --------------------------------------------------------------------------

class timer
{
	struct rep;
public:
	timer();
	~timer();

	std::ostream& print(std::ostream&) const;

private:
	timer(timer const&);          // not to be used
	void operator=(timer const&); // not to be used
	rep *start;
};

#if 0
template <class charT, class traits>
std::basic_ostream<charT, traits>&
operator<< (std::basic_ostream<charT, traits>&, timer const&);
else
std::ostream&
operator<< (std::ostream&, timer const&);
#endif

// --------------------------------------------------------------------------

} // namespace boost

#include <sys/time.h>
#include <unistd.h>

namespace boost
{

struct timer::rep
{
	struct timeval time;
};

timer::timer()
{
  start = new rep;
	::gettimeofday(&(start->time), 0);
	//-dk:TODO use ftime()?
}

timer::~timer()
{
	delete start;
}

ostream &
timer::print(std::ostream& out) const
{
	struct timeval stop;
	::gettimeofday(&stop, 0);

	long usec = stop.tv_usec - start->time.tv_usec;
	long sec  = stop.tv_sec  - start->time.tv_sec;

	if (usec < 0)
		{
			usec += 1000000;
			sec  -= 1;
		}

	char fill = out.fill();
	if (out.width() > 7)
		out.width(out.width() - 7);
	else
		out.width(0);

	out << sec << ".";
	out.fill('0');
	out.width(6);
	out << usec;

	out.fill(fill);
	return out;
}

std::ostream&
operator<< (std::ostream& out, timer const& t)
{
	return t.print(out);
}

// --------------------------------------------------------------------------

} // namespace boost

#endif /* BOOST_TIMER_H */
