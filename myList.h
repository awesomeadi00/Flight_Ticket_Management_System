#ifndef LIST_H
#define LIST_H

#include <iostream>
using namespace std;

template <typename T>
class Node
{
    private:
        T elem;             //Data element 
        Node* next;         //Link (pointer) to the next Node
        Node* prev;         //Link (pointer) to the previous Node
        template <typename T2> friend class MyList;
        
    public:
        Node(): elem(), next(NULL), prev(NULL) {}                   //Default Constructor
        Node(T elem) : elem(elem), next(NULL), prev(NULL) {}        //Non-default Constructor
};

template <typename T>
class MyList
{
    private:
        Node<T> *head;   //Pointer to the head of List
        Node<T> *tail;   //Pointer to the tail of List
    
    public:
        MyList();                              //Constructor to create dummy head and tail nodes
        ~MyList();                             //Destructor to clean all nodes
        int size() const;                      //Returns the size of the Linked List
        bool empty() const;                    //Return true if the list is empty
        const T& front() const;                //Return the element from the first valid Node of list
        const T& back() const;                 //Return the element from last valid Node of the list
        void push_front(const T& elem);        //Add a new node to the front of list (after head)
        void push_back(const T& elem);         //Add a new node to the back of the list (before tail)
        void pop_front();                      //Remove front node from list
        void pop_back();                       //Remove last node from list
        void erase(Node<T> *ptr);              //Remove a Node that an iterator points to
        void addBefore(Node<T>* ptr, const T& elem);    //Adds an element before an iterator 'ptr'    
    
        class iterator;   //Forwarding declarion of iterator class
        iterator begin() {return(iterator(head->next)); } //Returns the first element (head->next)
        iterator end() {return iterator(tail); } //Returns the next of the end of the list (tail)


        class iterator {
            private:
                Node<T> *it; //Pointer Iterator

            public: //Iteartor Traits
                using iterator_category = bidirectional_iterator_tag;
                using difference_type = ptrdiff_t;
                using value_type = T;
                using pointer = Node<T>*; //Here can put: T*
                using reference = T&;

            public:
                //Default Constructor: Iterator points to NULL
                iterator() {this->it = nullptr; }
                
                //Constructor: Initializes iterator pointer.
                iterator(Node<T> *node) {it = node; }

                //Dereferencing operator: Returns the element that iterator is pointing to.
                T operator*() {return it->elem; }
                
                //Get Iterator: This returns the pointer iterator (used for deletion of a node)
                Node<T>* getIt() {return this->it; }
            
                //Pre-Incremement operator: Increments the iterator to the next node.
                iterator& operator++(){
                    it = it->next;
                    return *this;
                }

                //Pre-Decremement operator: Decrements the iterator to the previous node.
                iterator& operator--() {
                    it = it->prev;
                    return *this;
                }

                //Not-Equal operator: Returns true if iterator is not the same as what pointer p is pointing to.
                bool operator!=(const iterator &p) const {
                    return (it != p.it);
                }

                //Equal operator: Returns true if iterator is the same as what pointer p is pointing to.
                bool operator==(const iterator &p) const {
                    return (it == p.it);
                }
        };
};

#endif
