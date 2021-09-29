#include <iostream>
#include "LinkedList.h"
#include "LinkedList.cpp"

#define LOG(s) std::cout << s << std::endl

int main() {
    LinkedList<int> list = LinkedList<int>();
    LOG("Add front: 20");
    list.add_front(20);
    list.print();
    LOG("Add back: 30");
    list.add_back(30);
    list.print();
    LOG("Add back: 10");
    list.add_back(10);
    list.print();
    LOG("Add front: 42");
    list.add_front(42);
    list.print();
    LOG("Delete back");
    list.del_back();
    list.print();
    LOG("Delete front");
    list.del_front();
    list.print();
    return 0;
}
