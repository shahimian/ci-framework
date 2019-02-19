#include <iostream>

class Rec {
    int id;
    std::string name;
public:
    Rec(int i, std::string n): id(i), name(n) {};
    friend std::ostream& operator<<(std::ostream& os, const Rec r);
};
