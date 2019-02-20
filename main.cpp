#include "list.h"
#include <iostream>
#include "rec.h"

using namespace std;

int main()
{
    List<Rec> *lr = new List<Rec>(Rec(1, "Manager"));
    lr->suc(Rec(2, "Employee"));
    lr->suc(Rec(3, "Worker"));
    lr->print_all();
    return 0;
}

