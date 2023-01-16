# Sender/Receiver Tutorial

This directory aims to provide a tutorial on how to use and implement
sender/receiver components (see the
[`std::execution`](http://wg21.link/p2300) proposal). The examples
in the tutorial start substantially simplified compared to a full
generic version. The simplification is intended to avoid distraction
while getting familiar with the basics. The hope is to get eventually
get to full-fledged generic components.

## Getting Started

The key abstraction of the sender/receiver consists of three parts:

1. A _receiver_ is an entity which can receive the results of
    some possibly asynchronous work. When some work is done it
    completes by calling a suitable receiver function called a
    _completion signal_. To be useful receivers will have multiple
    completion signals but the first few examples will just use the
    successful completion:

        receiver.set_value(result)

2. Work to be done is represented using _senders_. Each sender
    provides information on how it can complete and a way to
    `connect()` it to a receiver:

        decltype(sender)::result_t
        auto state = sender.connect(receiver)

3. When a sender gets connected to a receiver the result is an
    _operation state_ object repesenting work ready to be to be
    executed: the sender represents what work should be executed
    and the receiver represents where the results are send. The
    only function applicable to an operation state object is to
    `start()` it:

        state.start();

    Until the operation state object is `start()`ed the work isn't
    pursued and the opertion state object can be destroyed. Once
    the `state` is started it needs to stay alive until one
    completion signal is invoked on the object. There is always
    exactly one completion signal invoked.

### First Example of a Sender: `just`

The basic concepts are demonstrated by the [first
program](getting-started-1.cpp) implementing a simple sender `just`
which "just" provides its values synchronously as soon as it gets
started.

The class `tut::just` is straight forward:

- The constructor just stores a copy of the value passed in the
    member variable `d_value`. As the constructor takes the template
    paremeter as its own parameter it also support [class template
    argument deduction
    (CTAD)](https://en.cppreference.com/w/cpp/language/class_template_argument_deduction):

- To support introspection the result type is made available as 
    `result_t`.

- The `connect()` member function is templatized on a `Receiver`
    type and used to construct a `state<Receiver>` storing copies
    of the `receiver` and `d_value`. The members of the state are
    initialized using [aggregate
    initialization](https://en.cppreference.com/w/cpp/language/aggregate_initialization).

- The `state<Receiver>` class template has a member function `start()`
    which simply calls `d_receiver.set_value(d_value)` with the stored
    value.

To demonstrate the operation of this sender, a rather base `receiver`
is implemented: it only provides a `set_value()` method printing
the received parameter. The `main()` function creates `tut::just`
object, and `connect()`s a `receiver` to produce a `state` object.
Finally, the `state` object is `start()`ed.

This example isn't meant to be a useful program. Its entire intent is
to show a rather basic sender/receiver example. The benefit of these
abstractions start working out when some of the operations can be
asynchronous and are composed into workflows by algorithms operating
on the sender/receiver abstraction.

## Chaining Work on the Completion of Other Sender: `then`

## Asynchronously Scheduling Work: Schedulers

So far the examples were just a fairly complicated way of executing
work sequentially. To actually manage asynchronous work _schedulers_
are used. A scheduler provides a `schedule` operation which provides
a sender. After the sender is `connect()`ed and the operation state
got `start()`, the sender completes on on the scheduler's computation
resource. When that happens is controlled by the scheduler.

Schedulers are copyable and are, thus, typically lightweight handles
to some execution context. The execution context may manage an event
queue from a UI, outstanding I/O operations when dealing with
external source, a thread pool for work, the necessary resources
to off-load work to a GPU, etc.

The [manual scheduling example](getting-started-3.cpp) demonstrates
the operation of scheduler using a context managing a stack of
outstanding work:

- The class `manual_context` uses a singly linked, intrusive list
    of work items. The work `submit()`ed to the context is represented
    by a pointer to a `state_base` which holds a pointer `d_next`
    to the next work item or `nullptr` if there is no more work.
    The downstream work is scheduled by `complete()`ing the
    `schedule()` sender's operation state.

    The public operation of the `manual_context` are:

    1. An `empty()` function indicating that there is no outstanding
        work.
    2. A `get_scheduler()` function yielding an object of type
        `manual_context::scheduler` which is used to schedule work
        on the context.
    3. A function `run_next()` is running the next scheduled work
        item by completing the sender on the current thread.

- The `manual_context::scheduler` returned from `get_scheduler()`
    supports just one operation: `schedule()`. The main functionality
    of the scheduler abstraction to clearly indicate what operations
    are run where. Also, unlike senders, schedulers are always
    copyable.

    The `schedule()` operation yields a `manual_context::sender`
    used to chain work on the completion of the scheduling operation.

- The `manual_context::sender` declares that it doesn't produce any
    useful result by defining `result_t` to be `tut::none`. When
    the model gets refined sender not yielding a value will eventually
    just call `set_value()` without any arguments but for now having
    `set_value` always yield exactly one value is a huge simplification.

    As all senders, `manual_context::sender` provides a `connect()`
    function, yielding a `manual_context::state<Receiver>.

- The `manual_context::state<Receiver>`'s main operation is `start()`
    which `submit()`s work with the context. As the `state` object is
    the actual work, it inherits from `sender_base` and overrides the
    `virtual` function `complete()`. The `complete()` function just
    calls `set_value(tut::none{})` on the `connect()`ed receiver.

    One important aspect of the operation states is that it can't
    be copied or moved: this way the `state_base` can be pointed
    to directly and an intrusive list can be used. This technique
    is quite common to avoid allocations. As operation states are
    returned from `connect()` and won't have copy or move constructor
    the entirely model heavely depends on [mandatory copy
    elision](https://en.cppreference.com/w/cpp/language/copy_elision).

The `main()` function of this example exercises the scheduling
operation of the `manual_context`: it `schedule()`s and `connect()`s
two piece of work using `then()` senders from the previous section.
Each corresponding state object gets `start()`ed which, of course,
just registered the work to be done with the scheduler. Once the
work is submitted the loop at the end just executes all work of the
`context`.
