# Sender/Receiver Experiment

The directory contains some code to experiment with the
[P0443](http://wg21.link/p0443) proposal. The main purpose of the
code is to see if I can get something to work. There is a [review
repository](https://github.com/atomgalaxy/review-executor-sendrecv)
where comments get collected. Some of the code over here is also
informed by the [Networking TS](http://wg21.link/n4771) and Facebook's
[libunifex](https://github.com/facebookexperimental/libunifex).

The code in this directory is compiled on Mac OS using
[gcc](http://gcc.gnu.org) 10.1.0 and [clang](http://clang.llvm.org)
10.0.0, in both cases using the `-std=c++2a` flag.

**Note**: This code is used for my personal experiments and it
certainly **not** ready for any actual use. It is essentially
untested and, thus, almost certainly riddled with bugs. Also, the
"design" is lacking at is what mostly driven to get something off
the ground on a very short notice.
