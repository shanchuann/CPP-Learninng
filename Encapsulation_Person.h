#ifndef ENCAPSULATION_PERSON_H
#define ENCAPSULATION_PERSON_H
#include <string>
using namespace std;

class Person {
public:
    void setName(const string& n);
    string getName() const;
private:
    string name;
};
#endif // !ENCAPSULATION_PERSON_H
