# Old Boost Submission

In the early days of Boost I helped kicking it off with
three simple libraries:

- `array.h` which provids a consistent way to get begin/end
  iterators for built-in arrays and STL containers. Part of
  that turned into `std::begin()`/`std::end()` although much
  refined.
- `dir_it` providing an iterator for directories. This was
  the starting point of what eventually became the file
  system library although at some point it was entirely
  rewritten.
- `heaps` which is a collection of various priority queue
  like implementations. This library got dropped from Boost
  and later an entirely different implementation emerged.

In all three cases I wasn't involved in any way in the
refinement. I'm still proud having been involved in kicking
off Boost. The files in this directory are not the original
submission, primarily because I haven't located that, yet.
If I manage to find the original submission I'll replace
(remove) the files accordingly.

## Note

The code isn't compilable for various reasons, e.g., because some
included files are missing. I may provide a compilable version if I
locate the original submission (and maybe a modernized form to see how
things have evolved).
