#include "list.h"

template <class T> void List<T>::print_all() {
    for(Link<T> *p = head; p; p=p->suc)
        std::cout << p->val << '\n';
}

template <class T> void List<T>::suc(const T& t) {
    head->suc->val = t;
}
