# kuhllib Algorithms

## Checklist

Guideline | Description
--- | ---
[algorithm object](#algorithm-object) | make the algorithm an object
[`constexpr`](#constexpr-algorithm) | make the algorithm `constexpr` where possible
[return iterators](#return-iterators) | make sure all relevant iterators are returned
[projections](#projections) | supply all relevant projections
[end point](#end-point) | support use of end points
[range support](#range-support) | support ranges as well as iterators
[limited sequence](#limited sequence) | limit all sequences, not just the first
[stability](#stability) | prefer a stable choice where possible
[call variations](#call-variations) | deduce the variations used from the arguments

## Status

Algorithm | No. | Group | Done | Test | Perf
--- | --- | --- | --- | --- | ---
`accumalate` | 2 | `<numeric>` | - | - | -
`adjacent_difference` | 4 | `<numeric>` | - | - | -
`adjacent_find` | 4 | `<algorithm>` | - | - | -
`all_of` | 2 | `<algorithm>` | - | - | -
`any_of` | 2 | `<algorithm>` | - | - | -
`binary_search` | 2 | `<algorithm>` | - | - | -
`clamp` | 2 | `<algorithm>` | - | - | -
`copy_backward` | 1 | `<algorithm>` | - | - | -
`copy_if` | 2 | `<algorithm>` | - | - | -
`copy_n` | 2 | `<algorithm>` | - | - | -
`copy` | 2 | `<algorithm>` | - | - | -
`count_if` | 2 | `<algorithm>` | - | - | -
`count` | 2 | `<algorithm>` | - | - | -
`equal_range` | 2 | `<algorithm>` | - | - | -
`equal` | 8 | `<algorithm>` | - | - | -
`exclusive_scan` | 4 | `<numeric>` | - | - | -
`fill_n` | 2 | `<algorithm>` | - | - | -
`fill` | 2 | `<algorithm>` | - | - | -
`find_end` | 4 | `<algorithm>` | - | - | -
`find_first_of` | 4 | `<algorithm>` | - | - | -
`find_if_not` | 2 | `<algorithm>` | - | - | -
`find_if` | 2 | `<algorithm>` | - | - | -
`find` | 2 | `<algorithm>` | - | - | -
`for_each_n` | 2 | `<algorithm>` | - | - | -
`for_each` | 2 | `<algorithm>` | - | - | -
`generate_n` | 2 | `<algorithm>` | - | - | -
`generate` | 2 | `<algorithm>` | - | - | -
`includes` | 4 | `<algorithm>` | - | - | -
`inclusive_scan` | 6 | `<numeric>` | - | - | -
`inner_product` | 4 | `<numeric>` | - | - | -
`inplace_merge` | 4 | `<algorithm>` | - | - | -
`iota` | 1 | `<numeric>` | - | - | -
`is_heap_until` | 4 | `<algorithm>` | - | - | -
`is_heap` | 4 | `<algorithm>` | - | - | -
`is_partitioned` | 2 | `<algorithm>` | - | - | -
`is_permutation` | 4 | `<algorithm>` | - | - | -
`is_sorted_until` | 4 | `<algorithm>` | - | - | -
`is_sorted` | 4 | `<algorithm>` | - | - | -
`iter_swap` | 1 | `<algorithm>` | - | - | -
`lexicographical_compare` | 4 | `<algorithm>` | - | - | -
`lower_bound` | 2 | `<algorithm>` | - | - | -
`make_heap` | 2 | `<algorithm>` | - | - | -
`max_element` | 4 | `<algorithm>` | - | - | -
`max` | 4 | `<algorithm>` | - | - | -
`merge` | 4 | `<algorithm>` | - | - | -
`min_element` | 4 | `<algorithm>` | - | - | -
`min` | 4 | `<algorithm>` | - | - | -
`minmax_element` | 4 | `<algorithm>` | - | - | -
`minmax` | 4 | `<algorithm>` | - | - | -
`mismatch` | 8 | `<algorithm>` | - | - | -
`move_backward` | 1 | `<algorithm>` | - | - | -
`move` | 2 | `<algorithm>` | - | - | -
`next_permutation` | 2 | `<algorithm>` | - | - | -
`none_of` | 2 | `<algorithm>` | - | - | -
`nth_element` | 4 | `<algorithm>` | - | - | -
`partial_sort_copy` | 4 | `<algorithm>` | - | - | -
`partial_sort` | 4 | `<algorithm>` | - | - | -
`partial_sum` | 2 | `<numeric>` | - | - | -
`partition_copy` | 2 | `<algorithm>` | - | - | -
`partition_point` | 1 | `<algorithm>` | - | - | -
`partition` | 2 | `<algorithm>` | - | - | -
`pop_heap` | 2 | `<algorithm>` | - | - | -
`prev_permutation` | 2 | `<algorithm>` | - | - | -
`push_heap` | 2 | `<algorithm>` | - | - | -
`reduce` | 6 | `<numeric>` | - | - | -
`remove_copy_if` | 2 | `<algorithm>` | - | - | -
`remove_copy` | 2 | `<algorithm>` | - | - | -
`remove_if` | 2 | `<algorithm>` | - | - | -
`remove` | 2 | `<algorithm>` | - | - | -
`replace_copy_if` | 2 | `<algorithm>` | - | - | -
`replace_copy` | 2 | `<algorithm>` | - | - | -
`replace_if` | 2 | `<algorithm>` | - | - | -
`replace` | 2 | `<algorithm>` | - | - | -
`reverse_copy` | 2 | `<algorithm>` | - | - | -
`reverse` | 2 | `<algorithm>` | - | - | -
`reverse` | 2 | `<algorithm>` | - | - | -
`rotate_copy` | 2 | `<algorithm>` | - | - | -
`rotate` | 2 | `<algorithm>` | - | - | -
`sample` | 1 | `<algorithm>` | - | - | -
`search_n` | 4 | `<algorithm>` | - | - | -
`search` | 5 | `<algorithm>` | - | - | -
`set_difference` | 4 | `<algorithm>` | - | - | -
`set_intersection` | 4 | `<algorithm>` | - | - | -
`set_symmetric_difference` | 4 | `<algorithm>` | - | - | -
`set_union` | 4 | `<algorithm>` | - | - | -
`shuffle` | 1 | `<algorithm>` | - | - | -
`sort_heap` | 2 | `<algorithm>` | - | - | -
`sort` | 4 | `<algorithm>` | - | - | -
`stable_partition` | 2 | `<algorithm>` | - | - | -
`stable_sort` | 4 | `<algorithm>` | - | - | -
`swap_ranges` | 2 | `<algorithm>` | - | - | -
`transform_exclusive_scan` | 2 | `<numeric>` | - | - | -
`transform_inclusive_scan` | 3 | `<numeric>` | - | - | -
`transform_reduce` | 2 | `<numeric>` | - | - | -
`transform` | 4 | `<algorithm>` | - | - | -
`uninitialized_copy_n` | 1 | `<memory>` | - | - | -
`uninitialized_copy` | 1 | `<memory>` | - | - | -
`uninitialized_fill_n` | 1 | `<memory>` | - | - | -
`uninitialized_fill` | 1 | `<memory>` | - | - | -
`unique_copy` | 4 | `<algorithm>` | - | - | -
`unique` | 4 | `<algorithm>` | - | - | -
`upper_bound` | 2 | `<algorithm>` | - | - | -

## Design Rationale

### Algorithm Object

Making the algorithms objects with suitable function call operators
allows the use of algorithms as template parameters without the
need specifying template arguments. There are a few implications
of this design:

 1. The objects are _not_ found via ADL.
 2. When an object is found by name look-up ADL is inhibited by the name.
 3. There is no way to overload functions on the name.
 4. The class for the algorithm provides an obvious place for meta data and auxiliary definitions.
 5. Make an algorithm an object adds some necessary boilerplate.

### `constexpr` Algorithm

The algorithms generally do not depend on any state. Correspondingly,
many of them can be made `constexpr`. Where applicable the algorithms
should be made `constexpr`.

### Return Iterators

When using single-pass iterators, only the last used copy of the
iterator stays valid: a valid iterator can be copied and all copies
can be dereference until the first copy gets incremented at which
point only the incremented copy stays valid. The position in the
sequence cannot be recovered other than using the valid copy.

Some of the standard library algorithms use multiple sequences.
For example, `std::copy()` operates on both input and output iterators
and it is reasonably to have an end iterator which breaks out of
the sequence at a different position than the ultimate end.  However,
only the output iterator is returned. A better design is to return
all relevant iterators.

### Projections

Often algorithms should be applied to partial or derived objects.
For example, applying an algorithm to all keys of a `std::map` is
quite common. Using a projection amounts to calling a function
on the entity return from the iterator's dereference operator to
read or write a value.

Aside from the projection, the separation of reading and writing
operations provides additional benefit.

### End Point

Algorithms using only `ForwardIterator` or weaker iterator concepts
do no need to have the same type for indicating the begin and the end
of a range. In particular the end cannot be used for anything but
identifying the end. Using an _end point_ concept allowing only
equality comparison with an iterator for sequences at most requiring
forward iteration makes the algorithms a lot more flexible.

### Range Support

Instead of requiring sequences to be specified by a pair of a begin
and an end object, algorithms taking sequenes should support
individual objects providing a _range_. A range is essentially just
a combination of the begin and the end, possibly combined with
appropriate [projections](#projections).

### Limited Sequence

There are multiple algorithms in the standard C++ library making
assumptions about the size of some of the sequences. For example,
the destination sequence used with `std::copy()` has to have at
least the size of the first sequence. There is no way to limit the
destination sequence.

It is desirable to potentially limit all sequences. In cases where a
sequence is known to be big enough it is possible to provide a
suitable _unreachable_ object for which comparison to an iterator
is `constexpr` always returning `true`. For example

    template <typename InIt, typename InEndPoint,
              typename OutIt, typename OutEndPoint>
    std::pair<InIt, OutIt>
    copy(InIt fromBegin, InEndPoint fromEnd,
         OutIt toBegin, OutEndPoint toEnd) {
        while (fromBegin != fromEnd && toBegin != toEnd) {
            *toBegin = *fromBegin;
            ++fromBegin;
            ++toBegin;
       }
       return std::make_pair(fromBegin, toBegin);
    }
    template <typename InIt, typename InEndPoint,
              typename OutIt>
    std::pair<InIt, OutIt>
    copy(InIt fromBegin, InEndPoint fromEnd,
         OutIt toBegin) {
        return copy(fromBegin, fromEnd, toBegin, unreachable);
    }

### Stability

Some algorithms have a choice of the result. Where the result can
be freely chosen and the choice does not or only marginally impact
the performance, the _stable_ choice should be made: assuming the
sequence is ordered choose the result keeping candidate results in
the same order or choose the element among equivalent choice at the
appropriate end. For example, `std::min_element()` should return
the first minimal object and `std::max_element()` should return the
last maximal object.

### Call Variations

The algorithms have multiple variations like begin/end vs. range,
optional projections, optional execution policies, and possibly
more. In many cases it is reasonable to implement a very general
version to which all the other variations can delegate to. While
the most general version is probably the first one to be implemented
and the delegation can be aided by some components but it needs to
be applied
