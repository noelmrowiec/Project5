/*
* Linked List implementation
*
* author: Noel Mrowiec
* 11MAR2023
*/

#include "CSCI361Proj5.h"
#include <new>      //for memory allocation check

using namespace FHSULINKEDLIST;

//     Precondition: head_ptr is the head pointer of a linked list.
//     Postcondition: The value returned is the number of nodes in the linked
//     list. The list itself is unaltered.
size_t FHSULINKEDLIST::list_length(const Node* head_ptr)
{
    int length{};
    //check if at least one node
    if (head_ptr) {
        const Node* currentNode{ head_ptr };
        ++length;       //increment length count
        //keep counting while there is a next node
        while (currentNode->link != NULL) {
            ++length;
            currentNode = currentNode->link;
        }
    }

    return length;
}

//     Precondition: head_ptr is the head pointer of a linked list.
//     Postcondition: A new node containing the given entry has been added at
//     the head of the linked list; head_ptr now points to the head of the new,
//     longer linked list.
void FHSULINKEDLIST::list_head_insert(Node*& head_ptr, const Node::Item& entry)
{
    //(std::nothrow) will not throw exception so need to check if success
    Node* newHead{ new (std::nothrow) Node{entry, head_ptr} };     

    //check if memory allocation succeeded
    if(newHead)
        head_ptr = newHead;
}

//     Precondition: previous_ptr points to a node in a linked list.
//     Postcondition: A new node containing the given entry has been added
//     after the node that previous_ptr points to.
void FHSULINKEDLIST::list_insert(Node* previous_ptr, const Node::Item& entry)
{
    //(std::nothrow) will not throw exception so need to check if success
    Node* insertedNode{ new (std::nothrow) Node{entry, previous_ptr->link} };
    //check if memory allocation succeeded
    if(insertedNode)
        previous_ptr->link = insertedNode;

}

//   Node* list_search(Node* head_ptr, const Node::Item& target)
//     Precondition: head_ptr is the head pointer of a linked list.
//     Postcondition: The pointer returned points to the first node containing
//     the specified target in its data member. If there is no such node, the
//     null pointer is returned. The list itself is unaltered.
Node* FHSULINKEDLIST::list_search(Node* head_ptr, const Node::Item& target)
{
    Node* currentNode{ head_ptr };
    //check if at least one node
    if (currentNode) {
        //continue until end list
        while (currentNode) {
            //check if found target
            if (currentNode->data == target)
                return currentNode;             //return the node with data matching the target
            currentNode = currentNode->link;    //go to next node    
        }
    }

    return nullptr;
}

//     Precondition: head_ptr is the head pointer of a linked list, and
//     position > 0.
//     Postcondition: The pointer returned points to the node at the specified
//     position in the list. (The head node is position 1, the next node is
//     position 2, and so on). If there is no such position, then the null
//     pointer is returned. The list itself is unaltered.
Node* FHSULINKEDLIST::list_locate(Node* head_ptr, size_t position)
{
    Node* currentNode{ head_ptr };
    int currentPosition{ 1 };        //start at position 1
    //loop while currentNode hasn't reached the end or the 'position'
    while (currentNode && currentPosition < position) {
        currentNode = currentNode->link;
        ++currentPosition;
    }

    return currentNode;
}

//     Precondition: head_ptr is the head pointer of a linked list, with at
//     least one node.
//     Postcondition: The head node has been removed and returned to the heap;
//     head_ptr is now the head pointer of the new, shorter linked list.
void FHSULINKEDLIST::list_head_remove(Node*& head_ptr)
{
    //if 1 or more nodes, then remove 
    if (head_ptr) {
        Node* newHead{ head_ptr->link };
        delete head_ptr;
        head_ptr = newHead;
    }
}

//     Precondition: previous_ptr points to a node in a linked list, and this
//     is not the tail node of the list.
//     Postcondition: The node after previous_ptr has been removed from the
//     linked list.
void FHSULINKEDLIST::list_remove(Node* previous_ptr)
{
    //check that 'previous_ptr is not the end
    if (previous_ptr->link) {
        Node* newNext{ previous_ptr->link->link };      //get the new next node
        delete previous_ptr->link;
        previous_ptr->link = newNext;                   //link the new next node
    }
}

//     Precondition: head_ptr is the head pointer of a linked list.
//     Postcondition: All nodes of the list have been returned to the heap,
//     and the head_ptr is now NULL.
void FHSULINKEDLIST::list_clear(Node*& head_ptr)
{
    size_t length{ list_length(head_ptr) };
    for (size_t i = 0; i < length; ++i)
        list_tail_remove(head_ptr);
}

//     Precondition: source_ptr is the head pointer of a linked list.
//     Postcondition: head_ptr is the head pointer for
//     a new list that contains the same items as the list pointed to by
void FHSULINKEDLIST::list_copy(Node* source_ptr, Node*& head_ptr)
{
    if (source_ptr) {
        size_t length{ list_length(head_ptr) };

        head_ptr = list_copy_front(source_ptr, length);
    }
}

//     Precondition: head_ptr is the head pointer of a linked list.
//     Postcondition: The return value is the count of the number of times
//     target appears as the data portion of a node on the linked list.
//     The linked list itself is unchanged.
size_t FHSULINKEDLIST::list_occurrences(Node* head_ptr, const Node::Item& target)
{
    size_t occurences{};

    Node* currentNode{ head_ptr };
    //continue while nodes in list
    while (currentNode) {
        currentNode = list_search(currentNode, target);        //search for target
        //check if target found (not nullptr)
        if (currentNode) {
            ++occurences;
            currentNode = currentNode->link;                        //go to next node
        }
    }

    return occurences;
}

//     Precondition: head_ptr is the head pointer of a linked list.
//     Postcondition: A new node containing the given entry has been added at
//     the tail of the linked list; if this happens to be the first node of
//     the linked list, then head_ptr now points to this node (otherwise
//     head_ptr is unchanged).
void FHSULINKEDLIST::list_tail_attach(Node*& head_ptr, const Node::Item& entry)
{
    if (head_ptr == nullptr) {
        //add head
        list_head_insert(head_ptr, entry);
    }
    else {
        //add tail, therefore, 'Link' is nullptr
        Node* newTail{ new(std::nothrow) Node{entry, nullptr} };

        //check if memory allocation succeeded
        if (newTail) {
            size_t length = list_length(head_ptr);
            Node* currentTail{ list_locate(head_ptr, length) };              //get current tail
            currentTail->link = newTail;
        }
    }
}

//   void list_tail_remove(Node*& head_ptr)
//     Precondition: head_ptr is the head pointer of a linked list, with at
//     least one node.
//     Postcondition: The tail node has been removed and returned to the heap;
//     if the list is now empty, then head_ptr is null; otherwise head_ptr
//     is unchanged.
void FHSULINKEDLIST::list_tail_remove(Node*& head_ptr)
{
    size_t length{ list_length(head_ptr) };
    if (length == 1) {
        //delete head
        list_head_remove(head_ptr);
    }
    else {
        //delete tail
        Node* tailPrevious{ list_locate(head_ptr, length - 1) };
        delete tailPrevious->link;                                  //delete tail 
        tailPrevious->link = nullptr;                               //set new tail's next node to nullptr             
    }
}

//     Precondition: source_ptr is the head pointer of a linked list
//     Postcondition: The value returned is the head pointer for
//     a new list that contains copies of the first n nodes from the list
//     that source_ptr points to. If there less than n nodes in source list,
//     just copy all nodes in list. 
Node* FHSULINKEDLIST::list_copy_front(Node* source_ptr, size_t n)
{
    Node* head_ptr{ nullptr };

    //check that source_ptr is not empty
    if (source_ptr) {
        list_head_insert(head_ptr, source_ptr->data);   //create new head of the copy

        //start a i=2 because head already added. continue for n items
        for (int i = 2; i <= n; ++i) {
            Node* currentNode{ list_locate(source_ptr, i) };
            //check if node exits 
            if (currentNode) {
                list_tail_attach(head_ptr, currentNode->data);
            }
            else
                break;      //node is nullptr, so reached end of list
        }
    }

    return head_ptr;
}




