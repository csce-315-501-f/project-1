#include <string>

template <class T>
class Attribute {
    T value;
    int length;
  public:
    Attribute() : length(0) {};
    Attribute(int len) : length(len) {};
    T getValue() { return value; }
};
