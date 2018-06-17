// hazptr.t.cpp                                                       -*-C++-*-
// ----------------------------------------------------------------------------

#include "hazptr.hpp"
#include "hazard_list.hpp"

namespace NE = nstd::experimental;
namespace NP = nstd::p0566r5;

// ----------------------------------------------------------------------------

struct Node
    : NE::hazptr_obj_base<Node> {
};

// ----------------------------------------------------------------------------

int main()
{
    NE::hazptr_domain& domain = NE::default_hazptr_domain();
    (void)domain;

    (new Node())->retire();
    (new Node())->retire(NE::default_hazptr_domain());

    auto holder = NP::make_hazptr();
    bool b = holder.empty();
    (void)b;

    std::atomic<Node*> an = new Node();

    holder.reset_protected(nullptr);
    Node* n = holder.protect(an);
    (void)n;

    auto holder1 = NP::make_hazptr();
    holder.swap(holder1);
    b = holder.try_protect(n, an);
    holder.reset_protected(n);
}
