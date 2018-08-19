  #include <locale>
  #include <sstream>
  #include <iostream>
  #include <iterator>

  #if !defined(_CXXRT_STD)
  #  define STD std::
  #else
  #  define STD _CXXRT_STD
  #endif

  void printflags(STD ios_base::iostate flags)
  {
    if (flags == STD ios_base::goodbit)
      STD cout << "(good)";
    else
      {
        STD cout << "(";
        if ((flags & STD ios_base::failbit) != 0)
          STD cout << "fail";
        if ((flags & STD ios_base::eofbit) != 0)
          STD cout << ((flags & STD ios_base::failbit) != 0? " ": "") << "eof";
        if ((flags & STD ios_base::badbit) != 0)
          STD cout << ((flags & (STD ios_base::eofbit | STD ios_base::failbit)) != 0? " ": "") << "bad";
        STD cout << ")";
      }
  }

  void result(bool rc)
  {
    STD cout << (rc? "test OK": "test failed") << "\n";
  }

  template <class cT, class Val>
  void
  test(STD ios_base &b, char const *str, Val val)
  {
    STD ctype<cT> const& ct = STD use_facet< STD ctype<cT> >(b.getloc());
    cT  buf[1024];
    cT* it = buf;
    for (char const *tmp = str; *tmp != 0; ++tmp, ++it)
      *it = ct.widen(*tmp);
    *it = cT();

    typedef STD num_get<cT, STD istreambuf_iterator<cT> > num_get;
    num_get const& ng = STD use_facet<num_get>(b.getloc());
    STD basic_istringstream<cT> in(str);
    STD istreambuf_iterator<cT> end;
    typename STD basic_istream<cT>::pos_type start = in.tellg();

    Val rc = val;
    STD ios_base::iostate err(STD ios_base::goodbit);
    STD istreambuf_iterator<cT> last = ng.get(in, end, b, err, rc);
    STD cout << "\"" << str << "\": ";
    STD cout << static_cast<long>(in.tellg() - start) << " ";
    printflags(err);
    STD cout << " " << rc << "\n";
  }

  int main(int ac, char *av[])
  {
    if (ac < 2)
      {
        STD cerr << "wrong number of arguments\n";
        return 1;
      }

    STD istringstream in("a");

    switch (av[1][0])
      {
      case '0':
        {
          STD cout << STD boolalpha;
          test<char, bool>(in, "0 ", false);
          test<char, bool>(in, "1 ", true);
          test<char, bool>(in, "2 ", false);
          test<char, bool>(in, "2 ", true);
          test<char, bool>(in, "0123456789 ", false);
          test<char, bool>(in, "0123456789 ", true);
        }
        break;
      case '1':
        {
          STD cout << STD boolalpha;
          in >> STD boolalpha;
          test<char, bool>(in, "false ", false);
          test<char, bool>(in, "false ", true);
          test<char, bool>(in, "true ", true);
          test<char, bool>(in, "true ", false);
          test<char, bool>(in, "foo ", false);
          test<char, bool>(in, "foo ", true);
        }
        break;
      case '2':
        {
          in >> STD dec;
          test<char, long>(in, "+1 ", 17);
          test<char, long>(in, "1 ", 17);
          test<char, long>(in, "-1 ", 17);
          test<char, long>(in, "0 ", 17);
          test<char, long>(in, "010 ", 17);
          test<char, long>(in, "-2147483648 ", 17);
          test<char, long>(in, "-2147483649 ", 17);
          test<char, long>(in, "-21474836481 ", 17);
          test<char, long>(in, "2147483647 ", 17);
          test<char, long>(in, "2147483648 ", 17);
          test<char, long>(in, "21474836471 ", 17);
          test<char, long>(in, "x", 17);
          test<char, long>(in, "a", 17);
          test<char, long>(in, "", 17);
          test<char, long>(in, "+ ", 17);
          test<char, long>(in, "- ", 17);
          test<char, long>(in, "+", 17);
          test<char, long>(in, "-", 17);
          test<char, long>(in, "1", 17);
          test<char, long>(in, "21474836471", 17);
          test<char, long>(in, "0x1", 17);
          test<char, long>(in, "0X1", 17);
        }
        break;
      case '3':
        {
          in >> STD oct;
          test<char, long>(in, "+1 ", 17);
          test<char, long>(in, "1 ", 17);
          test<char, long>(in, "-1 ", 17);
          test<char, long>(in, "0 ", 17);
          test<char, long>(in, "010 ", 17);
          test<char, long>(in, "x", 17);
          test<char, long>(in, "9", 17);
          test<char, long>(in, "", 17);
          test<char, long>(in, "+ ", 17);
          test<char, long>(in, "- ", 17);
          test<char, long>(in, "+", 17);
          test<char, long>(in, "-", 17);
          test<char, long>(in, "1", 17);
          test<char, long>(in, "0x1", 17);
          test<char, long>(in, "0X1", 17);
        }
        break;
      case '4':
        {
          in >> STD hex;
          test<char, long>(in, "+1 ", 17);
          test<char, long>(in, "1 ", 17);
          test<char, long>(in, "-1 ", 17);
          test<char, long>(in, "0 ", 17);
          test<char, long>(in, "010 ", 17);
          test<char, long>(in, "0x10 ", 17);
          test<char, long>(in, "x", 17);
          test<char, long>(in, "g", 17);
          test<char, long>(in, "", 17);
          test<char, long>(in, "+ ", 17);
          test<char, long>(in, "- ", 17);
          test<char, long>(in, "+", 17);
          test<char, long>(in, "-", 17);
          test<char, long>(in, "1", 17);
          test<char, long>(in, "0x1", 17);
          test<char, long>(in, "0X1", 17);
        }
        break;
      case '5':
        {
          in.setf(STD ios_base::fmtflags(), STD ios_base::basefield);
          test<char, long>(in, "10 ", 17);
          test<char, long>(in, "+10 ", 17);
          test<char, long>(in, "-10 ", 17);
          test<char, long>(in, "010 ", 17);
          test<char, long>(in, "+010 ", 17);
          test<char, long>(in, "-010 ", 17);
          test<char, long>(in, "0x10 ", 17);
          test<char, long>(in, "+0x10 ", 17);
          test<char, long>(in, "-0x10 ", 17);
          test<char, long>(in, "0X10 ", 17);
          test<char, long>(in, "+0X10 ", 17);
          test<char, long>(in, "-0X10 ", 17);
        }
        break;
      case '6':
        {
          in >> STD dec;
          test<char, unsigned short>(in, "1 ", 17);
          test<char, unsigned short>(in, "-1 ", 17);
          test<char, unsigned short>(in, "0 ", 17);
          test<char, unsigned short>(in, "65535 ", 17);
          test<char, unsigned short>(in, "65536 ", 17);
          test<char, unsigned short>(in, "655351 ", 17);
          test<char, unsigned short>(in, "x", 17);
          test<char, unsigned short>(in, "a", 17);
        }
        break;
      case '7':
        {
          test<char, unsigned int>(in, "1 ", 17);
          test<char, unsigned int>(in, "-1 ", 17);
          test<char, unsigned int>(in, "0 ", 17);
          test<char, unsigned int>(in, "4294967295 ", 17);
          test<char, unsigned int>(in, "4294967296 ", 17);
          test<char, unsigned int>(in, "42949672951 ", 17);
          test<char, unsigned int>(in, "x", 17);
          test<char, unsigned int>(in, "a", 17);
        }
        break;
      case '8':
        {
          test<char, unsigned long>(in, "1 ", 17);
          test<char, unsigned long>(in, "-1 ", 17);
          test<char, unsigned long>(in, "0 ", 17);
          test<char, unsigned long>(in, "4294967295 ", 17);
          test<char, unsigned long>(in, "4294967296 ", 17);
          test<char, unsigned long>(in, "42949672951 ", 17);
          test<char, unsigned long>(in, "x", 17);
          test<char, unsigned long>(in, "a", 17);
        }
        break;

      case 'a':
        {
          test<char, float>(in, "e", 5.0);
          test<char, float>(in, "++", 5.0);
          test<char, float>(in, "--", 5.0);
          test<char, float>(in, "..", 5.0);
          test<char, float>(in, ".1.1", 5.0);
          test<char, float>(in, "1.1.1", 5.0);
          test<char, float>(in, "+.1.1", 5.0);
          test<char, float>(in, "+1.1.1", 5.0);

          test<char, float>(in, "x", 5.0);
          test<char, float>(in, " ", 5.0);
          test<char, float>(in, "+ ", 5.0);
          test<char, float>(in, "- ", 5.0);
          test<char, float>(in, "125 ", 5.0);
          test<char, float>(in, "+125 ", 5.0);
          test<char, float>(in, "-125 ", 5.0);
          test<char, float>(in, "125. ", 5.0);
          test<char, float>(in, "+125. ", 5.0);
          test<char, float>(in, "-125. ", 5.0);
          test<char, float>(in, ".125 ", 5.0);
          test<char, float>(in, "+.125 ", 5.0);
          test<char, float>(in, "-.125 ", 5.0);
          test<char, float>(in, "125e ", 5.0);
          test<char, float>(in, "+125e ", 5.0);
          test<char, float>(in, "-125e ", 5.0);
          test<char, float>(in, "125.e ", 5.0);
          test<char, float>(in, "+125.e ", 5.0);
          test<char, float>(in, "-125.e ", 5.0);
          test<char, float>(in, ".125e ", 5.0);
          test<char, float>(in, "+.125e ", 5.0);
          test<char, float>(in, "-.125e ", 5.0);
          test<char, float>(in, "125e2 ", 5.0);
          test<char, float>(in, "+125e2 ", 5.0);
          test<char, float>(in, "-125e2 ", 5.0);
          test<char, float>(in, "125.e2 ", 5.0);
          test<char, float>(in, "+125.e2 ", 5.0);
          test<char, float>(in, "-125.e2 ", 5.0);
          test<char, float>(in, ".125e2 ", 5.0);
          test<char, float>(in, "+.125e2 ", 5.0);
          test<char, float>(in, "-.125e2 ", 5.0);
          test<char, float>(in, "125e+2 ", 5.0);
          test<char, float>(in, "+125e+2 ", 5.0);
          test<char, float>(in, "-125e+2 ", 5.0);
          test<char, float>(in, "125.e+2 ", 5.0);
          test<char, float>(in, "+125.e+2 ", 5.0);
          test<char, float>(in, "-125.e+2 ", 5.0);
          test<char, float>(in, ".125e+2 ", 5.0);
          test<char, float>(in, "+.125e+2 ", 5.0);
          test<char, float>(in, "-.125e+2 ", 5.0);
          test<char, float>(in, "125e-2 ", 5.0);
          test<char, float>(in, "+125e-2 ", 5.0);
          test<char, float>(in, "-125e-2 ", 5.0);
          test<char, float>(in, "125.e-2 ", 5.0);
          test<char, float>(in, "+125.e-2 ", 5.0);
          test<char, float>(in, "-125.e-2 ", 5.0);
          test<char, float>(in, ".125e-2 ", 5.0);
          test<char, float>(in, "+.125e-2 ", 5.0);
          test<char, float>(in, "-.125e-2 ", 5.0);

          test<char, float>(in, "", 5.0);
          test<char, float>(in, "+", 5.0);
          test<char, float>(in, "-", 5.0);
          test<char, float>(in, "125", 5.0);
          test<char, float>(in, "+125", 5.0);
          test<char, float>(in, "-125", 5.0);
          test<char, float>(in, "125.", 5.0);
          test<char, float>(in, "+125.", 5.0);
          test<char, float>(in, "-125.", 5.0);
          test<char, float>(in, ".125", 5.0);
          test<char, float>(in, "+.125", 5.0);
          test<char, float>(in, "-.125", 5.0);
          test<char, float>(in, "125e", 5.0);
          test<char, float>(in, "+125e", 5.0);
          test<char, float>(in, "-125e", 5.0);
          test<char, float>(in, "125.e", 5.0);
          test<char, float>(in, "+125.e", 5.0);
          test<char, float>(in, "-125.e", 5.0);
          test<char, float>(in, ".125e", 5.0);
          test<char, float>(in, "+.125e", 5.0);
          test<char, float>(in, "-.125e", 5.0);
          test<char, float>(in, "125e2", 5.0);
          test<char, float>(in, "+125e2", 5.0);
          test<char, float>(in, "-125e2", 5.0);
          test<char, float>(in, "125.e2", 5.0);
          test<char, float>(in, "+125.e2", 5.0);
          test<char, float>(in, "-125.e2", 5.0);
          test<char, float>(in, ".125e2", 5.0);
          test<char, float>(in, "+.125e2", 5.0);
          test<char, float>(in, "-.125e2", 5.0);
          test<char, float>(in, "125e+2", 5.0);
          test<char, float>(in, "+125e+2", 5.0);
          test<char, float>(in, "-125e+2", 5.0);
          test<char, float>(in, "125.e+2", 5.0);
          test<char, float>(in, "+125.e+2", 5.0);
          test<char, float>(in, "-125.e+2", 5.0);
          test<char, float>(in, ".125e+2", 5.0);
          test<char, float>(in, "+.125e+2", 5.0);
          test<char, float>(in, "-.125e+2", 5.0);
          test<char, float>(in, "125e-2", 5.0);
          test<char, float>(in, "+125e-2", 5.0);
          test<char, float>(in, "-125e-2", 5.0);
          test<char, float>(in, "125.e-2", 5.0);
          test<char, float>(in, "+125.e-2", 5.0);
          test<char, float>(in, "-125.e-2", 5.0);
          test<char, float>(in, ".125e-2", 5.0);
          test<char, float>(in, "+.125e-2", 5.0);
          test<char, float>(in, "-.125e-2", 5.0);
        }
        break;
      case 'b':
        {
          test<char, double>(in, "e", 5.0);
          test<char, double>(in, "++", 5.0);
          test<char, double>(in, "--", 5.0);
          test<char, double>(in, "..", 5.0);
          test<char, double>(in, ".1.1", 5.0);
          test<char, double>(in, "1.1.1", 5.0);
          test<char, double>(in, "+.1.1", 5.0);
          test<char, double>(in, "+1.1.1", 5.0);

          test<char, double>(in, "x", 5.0);
          test<char, double>(in, " ", 5.0);
          test<char, double>(in, "+ ", 5.0);
          test<char, double>(in, "- ", 5.0);
          test<char, double>(in, "125 ", 5.0);
          test<char, double>(in, "+125 ", 5.0);
          test<char, double>(in, "-125 ", 5.0);
          test<char, double>(in, "125. ", 5.0);
          test<char, double>(in, "+125. ", 5.0);
          test<char, double>(in, "-125. ", 5.0);
          test<char, double>(in, ".125 ", 5.0);
          test<char, double>(in, "+.125 ", 5.0);
          test<char, double>(in, "-.125 ", 5.0);
          test<char, double>(in, "125e ", 5.0);
          test<char, double>(in, "+125e ", 5.0);
          test<char, double>(in, "-125e ", 5.0);
          test<char, double>(in, "125.e ", 5.0);
          test<char, double>(in, "+125.e ", 5.0);
          test<char, double>(in, "-125.e ", 5.0);
          test<char, double>(in, ".125e ", 5.0);
          test<char, double>(in, "+.125e ", 5.0);
          test<char, double>(in, "-.125e ", 5.0);
          test<char, double>(in, "125e2 ", 5.0);
          test<char, double>(in, "+125e2 ", 5.0);
          test<char, double>(in, "-125e2 ", 5.0);
          test<char, double>(in, "125.e2 ", 5.0);
          test<char, double>(in, "+125.e2 ", 5.0);
          test<char, double>(in, "-125.e2 ", 5.0);
          test<char, double>(in, ".125e2 ", 5.0);
          test<char, double>(in, "+.125e2 ", 5.0);
          test<char, double>(in, "-.125e2 ", 5.0);
          test<char, double>(in, "125e+2 ", 5.0);
          test<char, double>(in, "+125e+2 ", 5.0);
          test<char, double>(in, "-125e+2 ", 5.0);
          test<char, double>(in, "125.e+2 ", 5.0);
          test<char, double>(in, "+125.e+2 ", 5.0);
          test<char, double>(in, "-125.e+2 ", 5.0);
          test<char, double>(in, ".125e+2 ", 5.0);
          test<char, double>(in, "+.125e+2 ", 5.0);
          test<char, double>(in, "-.125e+2 ", 5.0);
          test<char, double>(in, "125e-2 ", 5.0);
          test<char, double>(in, "+125e-2 ", 5.0);
          test<char, double>(in, "-125e-2 ", 5.0);
          test<char, double>(in, "125.e-2 ", 5.0);
          test<char, double>(in, "+125.e-2 ", 5.0);
          test<char, double>(in, "-125.e-2 ", 5.0);
          test<char, double>(in, ".125e-2 ", 5.0);
          test<char, double>(in, "+.125e-2 ", 5.0);
          test<char, double>(in, "-.125e-2 ", 5.0);

          test<char, double>(in, "", 5.0);
          test<char, double>(in, "+", 5.0);
          test<char, double>(in, "-", 5.0);
          test<char, double>(in, "125", 5.0);
          test<char, double>(in, "+125", 5.0);
          test<char, double>(in, "-125", 5.0);
          test<char, double>(in, "125.", 5.0);
          test<char, double>(in, "+125.", 5.0);
          test<char, double>(in, "-125.", 5.0);
          test<char, double>(in, ".125", 5.0);
          test<char, double>(in, "+.125", 5.0);
          test<char, double>(in, "-.125", 5.0);
          test<char, double>(in, "125e", 5.0);
          test<char, double>(in, "+125e", 5.0);
          test<char, double>(in, "-125e", 5.0);
          test<char, double>(in, "125.e", 5.0);
          test<char, double>(in, "+125.e", 5.0);
          test<char, double>(in, "-125.e", 5.0);
          test<char, double>(in, ".125e", 5.0);
          test<char, double>(in, "+.125e", 5.0);
          test<char, double>(in, "-.125e", 5.0);
          test<char, double>(in, "125e2", 5.0);
          test<char, double>(in, "+125e2", 5.0);
          test<char, double>(in, "-125e2", 5.0);
          test<char, double>(in, "125.e2", 5.0);
          test<char, double>(in, "+125.e2", 5.0);
          test<char, double>(in, "-125.e2", 5.0);
          test<char, double>(in, ".125e2", 5.0);
          test<char, double>(in, "+.125e2", 5.0);
          test<char, double>(in, "-.125e2", 5.0);
          test<char, double>(in, "125e+2", 5.0);
          test<char, double>(in, "+125e+2", 5.0);
          test<char, double>(in, "-125e+2", 5.0);
          test<char, double>(in, "125.e+2", 5.0);
          test<char, double>(in, "+125.e+2", 5.0);
          test<char, double>(in, "-125.e+2", 5.0);
          test<char, double>(in, ".125e+2", 5.0);
          test<char, double>(in, "+.125e+2", 5.0);
          test<char, double>(in, "-.125e+2", 5.0);
          test<char, double>(in, "125e-2", 5.0);
          test<char, double>(in, "+125e-2", 5.0);
          test<char, double>(in, "-125e-2", 5.0);
          test<char, double>(in, "125.e-2", 5.0);
          test<char, double>(in, "+125.e-2", 5.0);
          test<char, double>(in, "-125.e-2", 5.0);
          test<char, double>(in, ".125e-2", 5.0);
          test<char, double>(in, "+.125e-2", 5.0);
          test<char, double>(in, "-.125e-2", 5.0);
        }
        break;
      case 'c':
        {
          test<char, long double>(in, "e", 5.0);
          test<char, long double>(in, "++", 5.0);
          test<char, long double>(in, "--", 5.0);
          test<char, long double>(in, "..", 5.0);
          test<char, long double>(in, ".1.1", 5.0);
          test<char, long double>(in, "1.1.1", 5.0);
          test<char, long double>(in, "+.1.1", 5.0);
          test<char, long double>(in, "+1.1.1", 5.0);

          test<char, long double>(in, "x", 5.0);
          test<char, long double>(in, " ", 5.0);
          test<char, long double>(in, "+ ", 5.0);
          test<char, long double>(in, "- ", 5.0);
          test<char, long double>(in, "125 ", 5.0);
          test<char, long double>(in, "+125 ", 5.0);
          test<char, long double>(in, "-125 ", 5.0);
          test<char, long double>(in, "125. ", 5.0);
          test<char, long double>(in, "+125. ", 5.0);
          test<char, long double>(in, "-125. ", 5.0);
          test<char, long double>(in, ".125 ", 5.0);
          test<char, long double>(in, "+.125 ", 5.0);
          test<char, long double>(in, "-.125 ", 5.0);
          test<char, long double>(in, "125e ", 5.0);
          test<char, long double>(in, "+125e ", 5.0);
          test<char, long double>(in, "-125e ", 5.0);
          test<char, long double>(in, "125.e ", 5.0);
          test<char, long double>(in, "+125.e ", 5.0);
          test<char, long double>(in, "-125.e ", 5.0);
          test<char, long double>(in, ".125e ", 5.0);
          test<char, long double>(in, "+.125e ", 5.0);
          test<char, long double>(in, "-.125e ", 5.0);
          test<char, long double>(in, "125e2 ", 5.0);
          test<char, long double>(in, "+125e2 ", 5.0);
          test<char, long double>(in, "-125e2 ", 5.0);
          test<char, long double>(in, "125.e2 ", 5.0);
          test<char, long double>(in, "+125.e2 ", 5.0);
          test<char, long double>(in, "-125.e2 ", 5.0);
          test<char, long double>(in, ".125e2 ", 5.0);
          test<char, long double>(in, "+.125e2 ", 5.0);
          test<char, long double>(in, "-.125e2 ", 5.0);
          test<char, long double>(in, "125e+2 ", 5.0);
          test<char, long double>(in, "+125e+2 ", 5.0);
          test<char, long double>(in, "-125e+2 ", 5.0);
          test<char, long double>(in, "125.e+2 ", 5.0);
          test<char, long double>(in, "+125.e+2 ", 5.0);
          test<char, long double>(in, "-125.e+2 ", 5.0);
          test<char, long double>(in, ".125e+2 ", 5.0);
          test<char, long double>(in, "+.125e+2 ", 5.0);
          test<char, long double>(in, "-.125e+2 ", 5.0);
          test<char, long double>(in, "125e-2 ", 5.0);
          test<char, long double>(in, "+125e-2 ", 5.0);
          test<char, long double>(in, "-125e-2 ", 5.0);
          test<char, long double>(in, "125.e-2 ", 5.0);
          test<char, long double>(in, "+125.e-2 ", 5.0);
          test<char, long double>(in, "-125.e-2 ", 5.0);
          test<char, long double>(in, ".125e-2 ", 5.0);
          test<char, long double>(in, "+.125e-2 ", 5.0);
          test<char, long double>(in, "-.125e-2 ", 5.0);

          test<char, long double>(in, "", 5.0);
          test<char, long double>(in, "+", 5.0);
          test<char, long double>(in, "-", 5.0);
          test<char, long double>(in, "125", 5.0);
          test<char, long double>(in, "+125", 5.0);
          test<char, long double>(in, "-125", 5.0);
          test<char, long double>(in, "125.", 5.0);
          test<char, long double>(in, "+125.", 5.0);
          test<char, long double>(in, "-125.", 5.0);
          test<char, long double>(in, ".125", 5.0);
          test<char, long double>(in, "+.125", 5.0);
          test<char, long double>(in, "-.125", 5.0);
          test<char, long double>(in, "125e", 5.0);
          test<char, long double>(in, "+125e", 5.0);
          test<char, long double>(in, "-125e", 5.0);
          test<char, long double>(in, "125.e", 5.0);
          test<char, long double>(in, "+125.e", 5.0);
          test<char, long double>(in, "-125.e", 5.0);
          test<char, long double>(in, ".125e", 5.0);
          test<char, long double>(in, "+.125e", 5.0);
          test<char, long double>(in, "-.125e", 5.0);
          test<char, long double>(in, "125e2", 5.0);
          test<char, long double>(in, "+125e2", 5.0);
          test<char, long double>(in, "-125e2", 5.0);
          test<char, long double>(in, "125.e2", 5.0);
          test<char, long double>(in, "+125.e2", 5.0);
          test<char, long double>(in, "-125.e2", 5.0);
          test<char, long double>(in, ".125e2", 5.0);
          test<char, long double>(in, "+.125e2", 5.0);
          test<char, long double>(in, "-.125e2", 5.0);
          test<char, long double>(in, "125e+2", 5.0);
          test<char, long double>(in, "+125e+2", 5.0);
          test<char, long double>(in, "-125e+2", 5.0);
          test<char, long double>(in, "125.e+2", 5.0);
          test<char, long double>(in, "+125.e+2", 5.0);
          test<char, long double>(in, "-125.e+2", 5.0);
          test<char, long double>(in, ".125e+2", 5.0);
          test<char, long double>(in, "+.125e+2", 5.0);
          test<char, long double>(in, "-.125e+2", 5.0);
          test<char, long double>(in, "125e-2", 5.0);
          test<char, long double>(in, "+125e-2", 5.0);
          test<char, long double>(in, "-125e-2", 5.0);
          test<char, long double>(in, "125.e-2", 5.0);
          test<char, long double>(in, "+125.e-2", 5.0);
          test<char, long double>(in, "-125.e-2", 5.0);
          test<char, long double>(in, ".125e-2", 5.0);
          test<char, long double>(in, "+.125e-2", 5.0);
          test<char, long double>(in, "-.125e-2", 5.0);
        }
        break;
      }

    return 0;
  }
