#ifndef D_LINKED_LIST
#define D_LINKED_LIST
#include <iostream>
#include "assert.h"

// template<class Data>// Data,Key
// struct Node{
//     Data data;
//     int key;
//     Node* next;
//     Node* prev;
//     Node(int key,Data data = Data()): data(data),key(key),next(nullptr),prev(nullptr){}
//     ~Node(){
//         delete data;
//     }
// };

template<class Data>
class DLinkedList{
public:
    struct node_t;
    typedef node_t* Node;

    DLinkedList() : list_size(0), head(nullptr), tail(nullptr) {}
    /**
     * @brief the copy c'tor, will make another list that points at the same data,
     * and has the same nodes, other than the head and tail.
     */
    DLinkedList(const DLinkedList& other) = default;
    DLinkedList& operator=( const DLinkedList& other) = delete;
    ~DLinkedList(); // Done
    void insertFirst(int key, Data data = Data()); 
    void insertOrdered(int key, Data data = Data());
    //assuming before is not a nullptr
    void insertBefore(int key,Data data, Node before);
    void insertAfter(int key,Data data, Node before);
    void insertLast(int key, Data data = Data()); 
    void deleteNodeKey(int key); 
    void deleteNode(Node todel);
    Node find_key(int key) const;
    void print_list() const;
    int getSize() const;
    Node GetTail() const;
    Node GetHead() const;
    class iterator{
        iterator(DLinkedList<Data>* list,Node some_node) : list(list), curr_node(some_node) {}
        friend class DLinkedList<Data>;
        DLinkedList<Data>* list;
        Node curr_node;
    public:
        Node operator*() const{
            return curr_node;
        }
        virtual iterator& operator++(){
            curr_node = curr_node->next;
            return *this;
        }
        bool operator==(const iterator& it) const{
            assert(list == it.list);
            return it.curr_node == curr_node;
        }
        bool operator!=(const iterator& it) const{
            return !(*this == it);
        }
    };
    iterator begin(){
        return iterator(this,head);
    } 
    iterator end(){
        return iterator(this,nullptr);
    }

    class reverse_iterator{
        reverse_iterator(DLinkedList<Data>* list,Node some_node) : list(list), curr_node(some_node) {}
        friend class DLinkedList<Data>;
        DLinkedList<Data>* list;
        Node curr_node;
    public:
        Node operator*() const{
            return curr_node;
        }
        virtual reverse_iterator& operator++(){
            curr_node = curr_node->prev;
            return *this;
        }
        bool operator==(const reverse_iterator& it) const{
            assert(list == it.list);
            return it.curr_node == curr_node;
        }
        bool operator!=(const reverse_iterator& it) const{
            return !(*this == it);
        }
    };

    reverse_iterator r_begin(){
        return reverse_iterator(this,tail);
    } 
    reverse_iterator r_end(){
        return reverse_iterator(this,nullptr);
    }
private:
    int list_size;
    Node head,tail;
};

template<class Data>
typename DLinkedList<Data>::Node DLinkedList<Data>::GetTail() const{
    return tail;
}

template<class Data>
typename DLinkedList<Data>::Node DLinkedList<Data>::GetHead() const{
    return head;
}

template<class Data>
struct DLinkedList<Data>::node_t{
    DLinkedList<Data>& list;
    Data data;
    int key;
    node_t* next;
    node_t* prev;
    node_t(int key, DLinkedList<Data>& list, Data data = Data())
        : list(list), data(data), key(key), next(nullptr),prev(nullptr){}
    void removeFromList();
};

template<class Data>
DLinkedList<Data>::~DLinkedList(){
    Node todel = nullptr;
    while(head){
        todel = head;
        head = head->next;
        delete todel;
    }
    head = nullptr;
}

template<class Data>
void DLinkedList<Data>::insertFirst(int key, Data data){
    Node new_node = new node_t(key, *this, data);
    if(head == nullptr){
        head = new_node;
        tail = new_node;
        list_size++;
        // new_node->next = nullptr;
        // new_node->prev = nullptr;
        return;
    }
    new_node->next = head;
    head = new_node;
    new_node->next->prev = head;
    new_node->prev = nullptr;
    list_size++;
}

//assuming key doesnt already exist, can check it with "find_key" function before use.
//use only when insertFirst/insertLast are not used at all
template<class Data>
void DLinkedList<Data>::insertOrdered(int key, Data data){
    Node new_node = new node_t(key, *this, data);
    if(head == nullptr){
        head = new_node;
        tail = new_node;
        list_size++;
        new_node->next = nullptr;
        new_node->prev = nullptr;
        return;
    }
    Node tmp = head;
    while(tmp != nullptr){
        if(tmp->key < key && tmp->next == nullptr){
            tmp->next = new_node;
            new_node->next = nullptr;
            new_node->prev = tmp;
            tail = new_node;
            list_size++;
            return;
        }
        else if(tmp->key < key && tmp->next != nullptr && tmp->next->key > key){
            new_node->prev = tmp;
            new_node->next = tmp->next;
            tmp->next->prev = new_node;
            tmp->next = new_node;
            list_size++;
            return;

        }
        else if(tmp->key > key && tmp->prev == nullptr){
            tmp->prev = new_node;
            new_node->prev = nullptr;
            new_node->next = tmp;
            head = new_node;
            list_size++;
            return;

        }
        tmp=tmp->next;
    }
    delete new_node;
}

template<class Data>
void DLinkedList<Data>::node_t::removeFromList(){
    if(next == nullptr && prev == nullptr){
        list.list_size--;
        list.head = nullptr;
        list.tail = nullptr;
        delete this;
        return;
    }
    else if(next == nullptr){
        prev->next = nullptr;
        list.tail = prev;
        list.list_size--;
        delete this;
        return;
    }
    else if(prev == nullptr){
        next->prev = nullptr;
        list.head = next;
        list.list_size--;
        delete this;
        return;
    }
    else{
        next->prev = prev;
        prev->next = next;
        list.list_size--;
        delete this;
        return;
    }
}

template<class Data>
void DLinkedList<Data>::insertAfter(int key,Data data, Node after){
    if(after == nullptr){
        return;
    }
    Node new_node = new node_t(key, *this, data);
    if(after->next == nullptr){//before points to 1st element
        after->next = new_node;
        new_node->prev = after;
        new_node->next = nullptr;
        tail = new_node;
        list_size++;
        return;
    }
    new_node->prev = after;
    after->next->prev = new_node;
    new_node->next = after->next;
    after->next = new_node;
    list_size++;
}

template<class Data>
void DLinkedList<Data>::insertBefore(int key,Data data, Node before){
    if(before == nullptr){
        return;
    }
    Node new_node = new node_t(key, *this, data);
    if(before->prev == nullptr){//before points to 1st element
        before->prev = new_node;
        new_node->prev = nullptr;
        new_node->next = before;
        head = new_node;
        list_size++;
        return;
    }
    new_node->prev = before->prev;
    before->prev->next = new_node;
    new_node->next = before;
    before->prev = new_node;
    list_size++;
}

template<class Data>
void DLinkedList<Data>::insertLast(int key, Data data){
    if(tail == nullptr){
        insertFirst(key,data);
    }
    else if(tail->next == nullptr){
        Node tmp = tail;
        Node new_node = new node_t(key, *this, data);
        tail->next = new_node;
        tail = new_node;
        tail->prev = tmp;
        tail->next = nullptr;
        list_size++;
    } 
}
 
//assuming keys are ordered from low to high
//
template<class Data>
typename DLinkedList<Data>::Node DLinkedList<Data>::find_key(int key) const{
    Node tmp = head;
    while(tmp != nullptr){
        if(tmp->key == key){
            return tmp;
        }
        tmp = tmp->next;
    }
    return nullptr;//in case key was not found
}

template<class Data>
void DLinkedList<Data>::print_list() const{
    // int counter = 0;
    Node tmp = head;
    while(tmp != nullptr){
        std::cout<<tmp->key<<" ";
        tmp = tmp->next;
        // counter++;
    }
    std::cout<<""<<std::endl;
}

template<class Data>
void DLinkedList<Data>::deleteNodeKey(int key){
    Node tmp = find_key(key);
    if(tmp==nullptr)
        return;
    tmp->removeFromList();
}

template<class Data>
int DLinkedList<Data>::getSize() const{
    return list_size;
}

#endif 