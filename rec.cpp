#include "rec.h"

std::ostream& operator<<(std::ostream& os, const Rec r){
    os << r.id << '\t' << r.name;
    return os;
}
