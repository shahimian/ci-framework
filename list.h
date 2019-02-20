#include <iostream>

template <class T> struct Link {
    Link<T> *pre;
    Link<T> *suc;
    T val;
    Link(const T& v, Link *p = 0, Link *s = 0): val(v), pre(p), suc(s) {};
};

template <class T> class List {
    Link<T> *head;
public:
    List(const T& t): head(new Link<T>(t)) {}
    void suc(const T& t){
        Link<T> *p;
        for(p = head; p; p = p->suc);
        p = new Link<T>(t);
    }
    void print_all(){
        for(Link<T> *p = head; p; p = p->suc)
        std::cout << p->val << '\n';
    }
};
