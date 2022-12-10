#include "flighthashtable.h"
#include "flightticket.h"
#include "myList.h"

//========================================================================
//Constructor: Creates dummy head and tail
template <typename T> MyList<T>::MyList() {
    head = new Node<T>;
    tail = new Node<T>;
    head->next = tail;
    tail->prev = head;
}

//========================================================================
//Destructor: Cleans up all nodes
template <typename T> MyList<T>::~MyList() {
    while(!empty()) {pop_front(); }
    delete head;
    delete tail;
}

//========================================================================
//Size: Returns the number of elements in the list
template <typename T> int MyList<T>::size() const {
    Node<T> *iterator = head;
    int count(0);
    
    for(int i = 0; iterator != tail; i++) {
        iterator = iterator->next;
        count++;
    }
    
    return count;
}

//========================================================================
//Empty: Returns true if the list is empty
template <typename T> bool MyList<T>::empty() const {
    return(head->next == tail and tail->prev == head);
}

//========================================================================
//Front: Return the element from the first valid Node of list, throw an exception if the list is empty
template <typename T> const T& MyList<T>::front() const {
    if(empty()) {
        throw runtime_error("List is empty! No Front!");
    }
    
    else
        return (head->next->elem);
}

//========================================================================
//Back: Return the element from last valid Node of the list, throw an exception if the list is empty
template <typename T> const T& MyList<T>::back() const {
    if(empty()) {
        throw runtime_error("List is empty! No Back!");
    }
    
    else
        return (tail->prev->elem);
}

//========================================================================
//Push Front: This will insert node at the front of the list
template <typename T> void MyList<T>::push_front(const T& elem) {
    Node<T> *u = new Node<T>(elem);
    u->next = head->next;
    u->prev = head;
    head->next = u;
    head->next->prev = u;
}

//========================================================================
//Push Back: This will insert node at the back of the list
template <typename T> void MyList<T>::push_back(const T& elem) {
    Node<T> *u = new Node<T>(elem);
    u->next = tail;
    u->prev = tail->prev;
    tail->prev->next = u;
    tail->prev = u;
}

//========================================================================
//Pop Front: Remove front node from list, throw an exception if the list is empty
template <typename T> void MyList<T>::pop_front() {
    if(empty()) {
        throw runtime_error("List is empty! No Front!");
    }
    
    else {
        Node<T> *v = head->next;
        Node<T> *w = head->next->next;
        head->next = w;
        w->prev = head;
        delete v;
    }
}

//========================================================================
//Pop Back: Remove last node from list, throw an exception if the list is empty
template <typename T> void MyList<T>::pop_back() {
    if(empty()) {
        throw runtime_error("List is empty! No Back!");
    }
    
    else {
        Node<T> *v = tail->prev;
        Node<T> *w = tail->prev->prev;
        tail->prev = w;
        w->next = tail;
        delete v;
    }
}

//========================================================================
//Erase: Remove a Node that an iterator points to.
template <typename T> void MyList<T>::erase(Node<T> *ptr) {
    Node<T> *prev = ptr->prev;
    Node<T> *next= ptr->next;
    prev->next = next;
    next->prev = prev;
    delete ptr;
}


//========================================================================
//Add Before: Adds an element before an iterator 'ptr'
template <typename T> void MyList<T>::addBefore(Node<T>* ptr, const T& elem) {
    Node<T> *temp = new Node<T>(elem);
    temp->next = ptr;
    temp->prev = ptr->prev;
    ptr->prev->next = temp;
    ptr->prev = temp;
}
//========================================================================

template class MyList<HashNode>;

