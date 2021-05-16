# Execution

The header `"nstd/execution.hpp"` (`<execution>` ) defines facilities
relating to execution policies. Execution policies are used to
specify what amount of concurrency an algorithm can employ.

## Execution Policy as Executor

The execution policy is an object passed to the algorithm. The
standard execution policies are `constexpr` objects. This implementation
uses the execution policies as an Executor concept. Note that this
Executor concept is *not* aligned with the ideas of the Concurrency
SG although it is believed that an eventually emerging Executor
concept could be used (the primary reason is that Currency SG doesn't
seem to have finalized their concept).

## Executor Operations

The following operations are used with an execution policy `ep`:

* `execute(ep, begin, end, fun)`

    Call the function `fun()` with the range `[begin, end)` possibly
    covering the entire range by multiple calls. Although `fun()`
    needs to be called with the entire range it may be called with
    different subranges from different threads.
