# kuhllib

This repository is intended to contain yet another implementation
of the standard C++ library. This is work in progress, intended
primarily to provide an improved interface for the standard algorithms
and a fast implementation of the I/O streams and locales library.
While prototypes for most of the components planned to be implemented
exist, many of them aren't in the repository.

## Build

You can just use `make` in the current directory. It will invoke `cmake`,
build the code, and run tests. To have more control over the build-process
you can use `cmake` with the `CMakeLists.txt` in the `src` directory.

## Concept Overview

This section is intended to yield an overview of the used concepts.
The primary purpose is to serve as a quick reference of what needs
to be implemented rather than necessarily capturing all details.

- `operation_state`

        template <typename S>
        concept operation_state
            =  destructible<S>
            && is_object_v<S>
            && requires(S& s) { { <a href="#start-cpo">execution::start(s)</a> } noexcept; }
            ;

- `receiver`

        templatete <typename R>
        concept receiver
            =  move_constructible<remove_cvref_t<R>>
            && constructible_from<remove_cvref_t<R>, R>
            && requires(const remove_cvref_t<R>& r) { <a href="#get_env-cpo">execution::get_env(rec)</a>; }
            ;

- `receiver_of`

        template <typename R, typename Completions>
        concept receiver_of
            =  receiver<R>
            && requires(Completions* comps) {
                []<valid_completion_for<R>... Sigs>(completion_signatures<Sigs...>*){}(comps);
            }
	    ;

- `scheduler`

        template <typename S>
        concept scheduler
            =  copy_constructible<remove_cvref_t<S>>
            && equality_comparable<remove_cvref_t<S>>
            && requires(S&& s, get_completion_scheduler_t<set_value_t> const& cpo) {
                { schedule(forward<S>(s)) } -> sender;
	            { tag_invoke(cpo, schedule(forward<S>(s))) } -> same_as<remove_cvref_t<S>>;
            }
            ;

- `sender_base` (exposition-only)

        template <typename S, typename E>
        concept sender_base
            = requires (S&& s, E&& e) {
                {
                    get_completion_signatures(forward<S>(s), forward<E>(e))
                } -> valid_completion_signatures<E>;
            }
            ;

- `sender`

        template <typename S, typename E = no_env>
        concept sender
            =  sender_base<S, E>
            && sender_base<S, no_env>
            && move_constructible<remove_cvref_t<Sender>>
            ;

- `sender_to`

        template <typename S, typename R>
        concept sender_to
            =  sender<S, env_of_t<R>>
            && receiver_of<R, completion_signatures_of_t<S, env_of_t<R>>>
            && requires(S&& s, R&& r) {
                connect(forward<S>(s), forward<R>(r));
            }
            ;

- `valid_completion_for` (exposition-only)


        template <typename Sig, typename R>
        concept valid_completion_for
            = requires(Sig* s) {
                []<typename C, typename...A>(C(*)(A...)) requires nothrow_tag_invocable<C, R, A...>{}(s);
            }
            ;

## Customization Point Object Overview

This section is intended to yield and overview of the user customization
point objects.  The notation resembles how the customization points
can be called. It doesn't represent their respective implementation.
This section is intend as quick reference when implementing the
corresponding customization rather than necessarily capturing all
details.

- <a name="get_env-cpo">`get_env(auto&& receiver) -> void`</a>
- <a name="set_error-cpo">`set_error(auto&& receiver, auto&& error) noexcept -> void`</a>
- <a name="set_stopped-cpo">`set_stopped(auto&& receiver) noexcept -> void`</a>
- <a name="set_value-cpo">`set_value(auto&& receiver, auto&&... args) noexcept -> void`</a>
- <a name="start-cpo">`start(auto& state) noexcept -> void`</a>
