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
    head = Node(-1, &dummy_node, &tail);
    head.id = "HEAD";
    tail = Node(-1, &head, &dummy_node);
    tail.id = "TAIL";
}

template<typename T>
bool LinkedList<T>::add_front(T item) {
    /* Create a new node on the heap and assign pointers as needed */
    if (len < MAX_LEN) {
        Node* new_node = new Node(item, &dummy_node, &dummy_node);
        new_node->prev = &head;
        new_node->next = head.next;
        head.next->prev = new_node;
        head.next = new_node;
        len++;
        return true;
    }
    return false;
}

template<typename T>
bool LinkedList<T>::add_back(T item) {
    /* Create a new node on the heap and assign pointers as needed */
    if (len < MAX_LEN) {
        Node* new_node = new Node(item, &dummy_node, &dummy_node);
        new_node->next = &tail;
        new_node->prev = tail.prev;
        tail.prev->next = new_node;
        tail.prev = new_node;
        len++;
    }
    return false;
}

template<typename T>
bool LinkedList<T>::del_front() {
    if (is_empty()) {
        std::cout << "The list is empty! Cannot delete from the back." << std::endl;
        return false;
    }
    else {
        Node* first = head.next; // get the node at the front
        head.next = first->next; // set the pointers such that the head points to the node after the first node
        head.next->prev = &head; // and the new first node points to the head
        delete first; // free the memory, so we don't get a memory leak
        len--;
        return true;
    }
}

template<typename T>
bool LinkedList<T>::del_back() {
    if (is_empty()) {
        std::cout << "The list is empty! Cannot delete from the back." << std::endl;
        return false;
    }
    else {
        Node* last = tail.prev; // get the node at the back
        tail.prev = last->prev; // set the pointers such that tail points to the node in front of the last one
        tail.prev->next = &tail; // and the new last node points to the tail
        delete last; // free the memory, so we don't get a memory leak
        len--;
        return true;
    }
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
    return head.next->data;
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
bool LinkedList<T>::is_empty() {
    return len == 0;
}

template<typename T>
LinkedList<T>::Node::Node(T d, LinkedList::Node *previous_node, LinkedList::Node *next_node) {
    data = d;
    prev = previous_node;
    next = next_node;
}

template<typename T>
LinkedList<T>::Node::Node() {

}

