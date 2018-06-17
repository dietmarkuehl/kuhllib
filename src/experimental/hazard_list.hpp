#ifndef INCLUDED_HAZARD_LIST
#define INCLUDED_HAZARD_LIST

#include "hazptr.hpp"
#include <atomic>
#include <functional>

namespace hazard {

template <typename CONTENT>
class ListNode : nstd::experimental::hazptr_obj_base<ListNode<CONTENT>> {
  public:
    std::atomic<ListNode *> d_next;
    CONTENT                 d_content;
};

    namespace p0566r5 {
        using namespace nstd::p0566r5;

        template <typename CONTENT>
        struct ProtectedElement {
            hazptr_holder  d_sourceProtection;
            CONTENT       *d_source = nullptr;
        };

        template <typename CONTENT>
        auto listFind(
                std::atomic<ListNode<CONTENT> *>& head,
                std::function<bool(CONTENT)> const& pred
            ) -> ProtectedElement<CONTENT>
        {
            using AtomicNodePtr = std::atomic<ListNode<CONTENT> *>;

            auto h1 = make_hazptr();
            auto h2 = make_hazptr();

            AtomicNodePtr *current = &head;
            for (;;) {
                auto *const node = h1.protect(*current);
                if (!node)
                    return {};
                if (pred(node->d_content))
                    return {std::move(h1), node};
                
                current = node;
                swap(h1, h2);
            }
            return {};
        }


        template <typename CONTENT>
        void removeNextListItem(
                ProtectedElement<CONTENT> previous
            )
        {
            auto hdel = make_hazptr();
            auto hafter = make_hazptr();

            ListNode<CONTENT> *toDelete;
            for (;;) {
                auto *const toDelete = hdel.protect(previous.d_source->d_next);
                if (!toDelete)
                    return;

                auto *const after = hafter.protect(toDelete->d_next);
                auto tmp = toDelete;
                if (previous.d_source->d_next.compare_exchange_strong(tmp, after)) {
                    toDelete->retire(
                        [h = std::move(hafter)](auto *p) mutable {
                            delete p;
                            h.reset_protected();
                        });
                    return;
                }
            }
        }

    }
}

#endif
