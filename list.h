#include <iostream>

template <class T> class List {
    struct Link {
        Link *pre;
        Link *suc;
        T val;
        Link(const T& v, Link *p = 0, Link *s = 0): val(v), pre(p), suc(s) {};
    };
    Link *head;
public:
    List(const T& t): head(new Link(t)) {}
    void suc(const T& t);
    void print_all();
};
