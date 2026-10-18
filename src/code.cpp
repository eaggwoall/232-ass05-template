// ============================================================
// CSCI 232 Assignment 05 – Evolution of Data Structures
// Student Implementation
// ============================================================
// Author: King Batawe
// ============================================================

#include "code.hpp"
#include <iostream>
#include <format>

// ============================================================
// STAGE 0: Legacy C Union
// ============================================================

/// Converts a LegacyData union to a formatted string.
/// Format specs:
/// 'i' -> integer value as string (e.g., "42")
/// 'd' -> double value formatted to 2 decimal places (e.g., "3.14")
/// 'c' -> string pointer content (or "nullptr" if cPtr is null)
/// default -> "unknown"

std::string printLegacyData(LegacyData data, char type) {
    std::string result = "unknown";
    if(type == 'i'){
        result = std::format("{}", data.i);
    }
    if(type == 'd'){
        result = std::format("{:.2f}", data.d);
    }
    if(type == 'c'){
        if(data.cPtr != NULL){
            result = std::format("{}", data.cPtr);
        }
    }
    return result;
}

// ============================================================
// STAGE 1: C-Style Struct
// ============================================================

/// Initializes a structNode with value, type indicator, and nullptr nextPtr.
void initStructNode(structNode* nPtr, LegacyData val, char type) {
    // TODO: Check if nPtr is nullptr before assigning fields
    if (nPtr == nullptr) {
        return;
    }
    nPtr->value = val;
    nPtr->typeData = type;
    nPtr->nextPtr = nullptr;
}

/// Dynamically allocates two structNodes.
/// Node 1: int 5 ('i')
/// Node 2: double 3.14 ('d')
/// Links Node 1 -> Node 2 -> nullptr
/// Returns pointer to Node 1.
structNode* createTwoStructNodes() {
    // TODO: Allocate dynamically using new, initialize both nodes, link them, and return head]
    LegacyData firstVal;
    firstVal.i = 5;

    LegacyData secondVal;
    secondVal.d = 3.14;
    
    structNode* first = new structNode;
    structNode* second = new structNode;

    initStructNode(first, firstVal, 'i');
    initStructNode(second, secondVal, 'd');

    first->nextPtr=second;
    return first;
}

// ============================================================
// STAGE 2: Early C++ Class (Classic Constructor)
// ============================================================

/// Constructor: Assign fields inside curly braces.
/// DO NOT use member initializer lists here!

classNode::classNode(LegacyData val, char type) {
    value = val;
    typeData = type;
    nextPtr = nullptr;
}

/// Dynamically allocates two classNodes (int 5, double 3.14) and links them.
classNode* createTwoClassNodes() {
    LegacyData firstVal;
    firstVal.i = 5;
    LegacyData secondVal;
    secondVal.d = 3.14;

    classNode* first = new classNode(firstVal, 'i');
    classNode* second = new classNode(secondVal, 'd');

    first->nextPtr = second;
    return first;
}

// ============================================================
// STAGE 3: C++98 Templates
// ============================================================

/// Dynamically allocates two classNodeT<int> objects (int 5, int 3) and links them.
classNodeT<int>* createTwoTemplateNodes() {
    classNodeT<int>* first = new classNodeT<int>(5);
    classNodeT<int>* second = new classNodeT<int>(3);

    first->nextPtr = second;
    return first;
}

// ============================================================
// STAGE 4: C++17 Variant and LinkedList Manager
// ============================================================

LinkedList::LinkedList() {
    headPtr = nullptr;
    counter = 0;
}

LinkedList::~LinkedList() {
    destroyList();
}

void LinkedList::destroyList() {
    while (headPtr != nullptr) {
        classNodeVariant* tempPtr = headPtr;
        headPtr = headPtr->nextPtr;
        delete tempPtr;
    }
    counter = 0;
}

int LinkedList::addFirst(classNodeVariant* newNodePtr) {
    if (newNodePtr == nullptr) {
        return -1;
    }
    newNodePtr->nextPtr = headPtr;
    headPtr = newNodePtr;
    counter++;
    return 0;
}

int LinkedList::addLast(classNodeVariant* newNodePtr) {
    if (newNodePtr == nullptr) {
        return -1;
    }
    newNodePtr->nextPtr = nullptr;
    if (headPtr == nullptr) {
        headPtr = newNodePtr;
    } else {
        classNodeVariant* currentPtr = headPtr;
        while (currentPtr->nextPtr != nullptr) {
            currentPtr = currentPtr->nextPtr;
        }
        currentPtr->nextPtr = newNodePtr;
    }
    counter++;
    return 0;
}

int LinkedList::deleteFirst() {
    if (headPtr == nullptr) {
        return -1;
    }
    classNodeVariant* tempPtr = headPtr;
    headPtr = headPtr->nextPtr;
    delete tempPtr;
    counter--;
    return 0;
}

// int LinkedList::deleteLast() {
//     // TODO: Find second-to-last node, delete last node, decrement counter
//     // Return -1 if list is empty, 0 on success
//     return -1;
// }

int LinkedList::deleteValue(ModernData targetValue) {
    classNodeVariant* currentPtr = headPtr;
    classNodeVariant* previousPtr = nullptr;
    while (currentPtr != nullptr) {
        if (currentPtr->value == targetValue) {
            if (previousPtr == nullptr) {
                headPtr = currentPtr->nextPtr;
            } else {
                previousPtr->nextPtr = currentPtr->nextPtr;
            }
            delete currentPtr;
            counter--;
            return 0;
        }
        previousPtr = currentPtr;
        currentPtr = currentPtr->nextPtr;
    }
    return -1;
}

int LinkedList::printList() {
    if (headPtr == nullptr) {
        return -1;
    }
    classNodeVariant* currentPtr = headPtr;
    while (currentPtr != nullptr) {
        if (std::holds_alternative<int>(currentPtr->value)) {
            std::cout << std::get<int>(currentPtr->value);
        } else if (std::holds_alternative<double>(currentPtr->value)) {
            std::cout << std::get<double>(currentPtr->value);
        } else {
            std::cout << std::get<std::string>(currentPtr->value);
        }
        std::cout << '\n';
        currentPtr = currentPtr->nextPtr;
    }
    return 0;
}

int LinkedList::listLength() {
    return counter;
}
