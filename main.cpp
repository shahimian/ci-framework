#include <iostream>
#include "list.h"
#include "rec.h"

using namespace std;

void f(List<int>&, List<Rec>&);

int main()
{
    List<Rec> lr(Rec(1, "ali"));
    List<int> li(8);
    f(li, lr);
    return 0;
}

void f(List<int>& li, List<Rec>& lr)
{
    li.print_all();
    lr.print_all();
};
