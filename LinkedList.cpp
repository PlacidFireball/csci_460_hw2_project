//
// Created by placidfireball on 9/29/21.
//

#include <iostream>
#include "LinkedList.h"
template<typename T>
LinkedList<T>::~LinkedList() = default;

template<typename T>
LinkedList<T>::LinkedList() {
    len = 0;
    for (auto & node : nodes) node = Node();
    head = Node(-1, &dummy_node, &tail);
    head.id = "HEAD";
    tail = Node(-1, &head, &dummy_node);
    tail.id = "TAIL";
}

template<typename T>
bool LinkedList<T>::add_front(T item) {
    if (len < MAX_LEN) {
        nodes[num_nodes] = Node(item, &dummy_node, &dummy_node);;
        nodes[num_nodes].prev = &head;
        nodes[num_nodes].next = head.next;
        head.next->prev = &nodes[num_nodes];
        head.next = &nodes[num_nodes];
        num_nodes++;
        if (num_nodes > 998) num_nodes = 0;
        len++;
        return true;
    }
    return false;
}

template<typename T>
bool LinkedList<T>::add_back(T item) {
    if (len < MAX_LEN) {
        nodes[num_nodes] = Node(item, &dummy_node, &dummy_node);
        nodes[num_nodes].next = &tail;
        nodes[num_nodes].prev = tail.prev;
        tail.prev->next = &nodes[num_nodes];
        tail.prev = &nodes[num_nodes];
        num_nodes++;
        if (num_nodes > 998) num_nodes = 0;
        len++;
        return true;
    }
    return false;
}

template<typename T>
bool LinkedList<T>::del_front() {
    Node* first = head.next;
    head.next = first->next;
    head.next->prev = &head;
    len--;
    return true;
}

template<typename T>
bool LinkedList<T>::del_back() {
    Node* last = tail.prev;
    tail.prev = last->prev;
    tail.prev->next = &tail;
    len--;
    return true;
}

template<typename T>
void LinkedList<T>::print() {
    Node* curr_node = head.next;
    std::cout << "[";
    while(curr_node->data != -1) {
        std::cout << curr_node->data;
        if (curr_node->next->data != -1) std::cout << ", ";
        curr_node = curr_node->next;
    }
    std::cout << "]" << std::endl;
}

template<typename T>
T LinkedList<T>::front() {
    return tail.next->data;
}

template<typename T>
T LinkedList<T>::back() {
    return tail.prev->data;
}

template<typename T>
[[maybe_unused]] void LinkedList<T>::test() {
    LinkedList<T> list = LinkedList<T>();
    for (int i = 0; i < 30; i++) {
        if (i % 2 == 0) {
            LOG("Add back"); LOG(i);
            list.add_back((T)i);
        }
        else {
            LOG("Add front"); LOG(i);
            list.add_front((T)i);
        }
        list.print();
    }
    for (unsigned long i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            LOG("Delete back");
            list.del_back();
        }
        else {
            LOG("Delete front");
            list.del_front();
        }
        list.print();
    }
}

template<typename T>
LinkedList<T>::Node::Node(T d, LinkedList::Node *previous_node, LinkedList::Node *next_node) {
    data = d;
    prev = previous_node;
    next = next_node;
}

template<typename T>
LinkedList<T>::Node::Node() {
    data = -1;
}

