# Cursors and Sentinels

Cursors and sentinels provide an abstraction of positions within
a sequence. They [deliberately] do not provide access to any data.
Data at a position is accessed through
[_projections_](../projection/README.md) (formerly projections were
referred to as _property maps_). The traditional C++
[_iterators_](../iterator/README) become refinements of cursors.

## Overview

Cursors are used to traverse the positions within a sequence.
Depending on the capabilities of a cursor a sequence may be traversed
just once ([_single pass cursor_](#single-pass-cursor)), traversed
multiple times ([_multi pass cursor_](#multi-pass-cursor)), traversed
in both directions ([_bidirectional cursor_](#bidirectional-cursor)),
or allow direct access to the positions in the range
([_random access cursor_](#random-access-cursor)). The cursor
categories above form a hierarchy of refinements:

 1. [`SinglePassCursor`](#single-pass-cursor)
 2. [`MultiPassCursor`](#multi-pass-cursor)
 3. [`BidirectionalCursor`](#bidirectional-cursor)
 4. [`RandomAccessCursor`](#random-access-cursor)

At each position within a sequence a _key_ can be accessed. The key
is generally considered to be an opaque entity to be used with a
[projection](../projection/README.md) to obtain access to a _value_.
The value access is required to be efficient. Some categories of
cursors, notably [_iterators_](../iterator/README.md), may allow
direct access to the value at a position without involving a
projection.

Sequences are delimted using a begin cursor and an end. The end
doesn't have to be a cursor for [`SinglePassCursor`](#single-pass-cursor)
and [`MultiPassCursor`](#multi-pass-cursor).  Instead it can be an
object of a suitable potentially different type. The concept for
the end is [`Sentinel`](#sentinel).

For [`BidirectionalCursor`](#bidirectional-cursor) and beyond the
begin and the end need to be the same type: while forward movements
can always produce a suitable end, when moving backwards the correct
type is needed.

## Concept Details

### Sentinel

An `Sentinel` is used to delimit a single-pass or a multi-pass
sequence. `Sentinel`s can be either stateless or stateful. For
example, an _unreachable_ `Sentinel` probably won't need any state
and operations involving a corresponding `Sentinel` most like yield
`constexpr` results.  An `Sentinel` indicating a position or a
condition on a value is likely to contain data identifying the
relevant position or data used during evaluation of the condition.

A type `S` satisfies the `Sentinel` requirement if `S` satisfied
the `CopyConstructible`, `CopyAssignable`, and `Destructible`
requirements and lvalues of `S` are swappable.

### Single Pass Cursor

A `SinglePassCursor` can traverse a sequence just once. It is not
required that a `SinglePassCursor` can recover a location or traverse
a sequence multiple times. For example, reading bytes from an
external source or writing bytes to an external destination would
allow only a single pass. When `c` is an object of type `C` satisfying
the `SinglePassCursor` it can can be in three different states:

 1. When `c` is at the sentinel, i.e., the sequence was exhausted, it is _invalid_.
 2. Otherwise `c` can be in a _valid_ state. How a valid cursor is produced is up to the implementer of the cursor. No constraints are imposed on how a valid cursor is created except that a copy of valid cursor shall be valid, too.
 3. If `c` is a copy of a valid cursor `d` and `d` gets incremented, `c` (as well as any other copies)  gets into _indeterminate_ state.

Algorithms expect that arguments are **not** in indeterminate state,
i.e., all cursor arguments shall be either valid or invalid. Note, that
these are the same constraints as those imposed by `InputIterators`.

An object `c` of a `SinglePassCursor` type `C` interoperates with
an object `s` of a `Sentinel` type `S` to determine the end of a
sequence. The types `C` and `S` are not rquired to be the same
although they are allowed to be the same (and they are required to
be the same for `BidirectionalCursor` or stronger concepts). The
operations in the following table are required to be valid with their
respective semantics:

Expression | Return Type | Semantics | Assertion
--- | --- | --- | ---
`++c` | `C&` | advance the cursor to the next element | **pre**: `c` is not at the end; **post**: other copies of `c` are no longer required either to be dereferencable or to be in the domain of `==`.
`(void)c++` | | advance the cursor to the next element | same as `(void)++c`
`*c` | *key-type* | access to data at a position | **pre**: `c` is not at the end
`*c++` | *key-type* | combined access and advance | same as `{ auto r = *c; ++c; return r; }`
`c == s` | `bool` | determine if the cursor is at the sentinel|
`s == c` | `bool` | determine if the cursor is at the sentinel| same as `c == s`
`c != s` | `bool` | determine if the cursor is not at the sentinel| same as `!(c ==s)`
`s != c` | `bool` | determine if the cursor is not at the sentinel| same as `!(c ==s)`

There are a few things to note:

 1. If the `SinglePassCursor` and the `Sentinel` types are different,
    there is no requirement that objects of the respective type can
    be compared to each other.
 2. The *key-type* is considered to be an opaque type. The only
    requirements on the *key-type* are that is `MoveConstructible`,
    `MoveAssignable`, and `Destructible`. Specific types modeling
    the `SinglePassCursor` can, of course, make stronger guarantees
    for the *key-type*.
 3. The *key-type* is **not** required to be a reference type. The
    *key-type* is used with a [projection](../projection/README.md)
    providing access to the corresponding data.
 4. When a cursor is advanced other objects have to be considered
    invalidated for `SinglePassCursors`:
    1. All copies of a cursor are invalided when one of the cursors
        (the original or any of the copies) is advanced.
    2. All values of *key-type* are invalidated when the cursor the
        value was obtained from is advanced or gets invalidated.

### Multi Pass Cursor

-dk:TODO

### Bidirectional Cursor

-dk:TODO

### Random Accessor Cursor

-dk:TODO

### Segmented Cursor

-dk:TODO
