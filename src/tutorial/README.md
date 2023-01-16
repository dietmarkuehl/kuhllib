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
