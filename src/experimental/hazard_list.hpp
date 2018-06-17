#ifndef INCLUDED_HAZARD_LIST
#define INCLUDED_HAZARD_LIST

#include "hazptr.hpp"
#include <atomic>

namespace hazard {

template <typename CONTENT>
class ListNode : nstd::experimental::hazptr_obj_base<ListNode<CONTENT>> {
  public:
    std::atomic<ListNode *> d_next;
    CONTENT                 d_content;
};

}

#endif
