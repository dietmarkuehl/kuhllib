// hazptr.hpp                                                         -*-C++-*-
// ----------------------------------------------------------------------------

#ifndef INCLUDED_HAZPTR
#define INCLUDED_HAZPTR

#include <memory>
#include <cstddef>

namespace std {
    namespace pmr {
        template <typename T>
        using polymorphic_allocator = std::allocator<T>;
    }
}

// ----------------------------------------------------------------------------

namespace nstd {
    namespace experimental {
        class hazptr_domain;
        hazptr_domain& default_hazptr_domain() noexcept;
        void hazptr_cleanup();
        void hazptr_cleanup(hazptr_domain& domain);
        template <typename T, typename D = std::default_delete<T>>
        class hazptr_obj_base;
    }
    namespace p0566r5 {
        class hazptr_holder;
        hazptr_holder make_hazptr();
        hazptr_holder make_hazptr(experimental::hazptr_domain& domain);
        void swap(hazptr_holder&, hazptr_holder&) noexcept;
    }
}

// ----------------------------------------------------------------------------

class nstd::experimental::hazptr_domain {
public:
    explicit hazptr_domain( std::pmr::polymorphic_allocator<std::byte> poly_alloc = {});
    hazptr_domain(const hazptr_domain&) = delete;
    hazptr_domain(hazptr_domain&&) = delete;
    hazptr_domain& operator=(const hazptr_domain&) = delete;
    hazptr_domain& operator=(hazptr_domain&&) = delete;
    ~hazptr_domain();
private:
    std::pmr::polymorphic_allocator<std::byte> alloc_; // exposition only
};

// ----------------------------------------------------------------------------

#endif
