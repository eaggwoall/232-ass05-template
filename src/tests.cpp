#ifndef UNITY_H
#define UNITY_H
#include "unity.h"
#endif
#include <string>
#include "code.hpp"
#include <sstream>
#include <iostream>

// ============================================================
// STAGE 0 Tests
// ============================================================

/// Create a LegacyData union with an int (42). Call printLegacyData.
/// Verify the returned string matches "42".
void test_printLegacyData_int(void) 
{
    LegacyData data;
    data.i = 42;
    std::string result = printLegacyData(data, 'i');
    TEST_ASSERT_TRUE_MESSAGE(result == "42", "Did not return 42.");
}

/// Create a LegacyData union with a double (3.14). Call printLegacyData.
/// Verify the returned string matches "3.14".
void test_printLegacyData_double(void) 
{
    LegacyData data;
    data.d = 3.14;
    std::string result = printLegacyData(data, 'd');
    TEST_ASSERT_TRUE_MESSAGE(result == "3.14", "Did not return 3.14.");
}

// ============================================================
// STAGE 1 Tests
// ============================================================

/// Call createTwoStructNodes().
/// Verify head->value contains int 5 with type 'i', and head->nextPtr->value contains double ~3.14 with type 'd'.
/// Clean up allocated memory.
void test_createTwoStructNodes_links_correctly(void) 
{
    structNode* head = createTwoStructNodes();
    TEST_ASSERT_NOT_NULL(head);
    TEST_ASSERT_NOT_NULL(head->nextPtr);

    TEST_ASSERT_EQUAL_CHAR('i', head->typeData);
    TEST_ASSERT_EQUAL_INT(5, head->value.i);

    TEST_ASSERT_EQUAL_CHAR('d', head->nextPtr->typeData);
    TEST_ASSERT_TRUE_MESSAGE(head->nextPtr->value.d > 3.139 && head->nextPtr->value.d < 3.14001, "Second node expected value approx. 3.14.");
    
    TEST_ASSERT_NULL(head->nextPtr->nextPtr);

    delete head->nextPtr;
    delete head;
}

// ============================================================
// STAGE 2 Tests
// ============================================================

/// Call createTwoClassNodes().
/// Verify head->value contains int 5 with type 'i', and head->nextPtr->value contains double ~3.14 with type 'd'.
/// Clean up allocated memory.
void test_createTwoClassNodes_links_correctly(void) 
{
    classNode* head = createTwoClassNodes();
    TEST_ASSERT_NOT_NULL(head);
    TEST_ASSERT_NOT_NULL(head->nextPtr);

    TEST_ASSERT_EQUAL_CHAR('i', head->typeData);
    TEST_ASSERT_EQUAL_INT(5, head->value.i);
    TEST_ASSERT_EQUAL_CHAR('d', head->nextPtr->typeData);
    TEST_ASSERT_TRUE_MESSAGE(head->nextPtr->value.d > 3.139 && head->nextPtr->value.d < 3.14001, "Second node expected value approx. 3.14.");
    TEST_ASSERT_NULL(head->nextPtr->nextPtr);

    delete head->nextPtr;
    delete head;
}

// ============================================================
// STAGE 3 Tests
// ============================================================

/// Call createTwoTemplateNodes().
/// Verify head->value is 5 and head->nextPtr->value is 3.
/// Clean up allocated memory.
void test_createTwoTemplateNodes_links_correctly(void) 
{
    classNodeT<int>* head = createTwoTemplateNodes();
    TEST_ASSERT_NOT_NULL(head);
    TEST_ASSERT_NOT_NULL(head->nextPtr);

    TEST_ASSERT_EQUAL_INT(5, head->value);
    TEST_ASSERT_EQUAL_INT(3, head->nextPtr->value);
    TEST_ASSERT_NULL(head->nextPtr->nextPtr);

    delete head->nextPtr;
    delete head;
}

// ============================================================
// STAGE 4: LinkedList Tests
// ============================================================

/// Create a LinkedList. Add two nodes using addFirst.
/// Verify listLength() returns 2 after insertions.
void test_linkedList_addFirst_updates_counter(void) 
{
    LinkedList list;
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.addFirst(nullptr));
    TEST_ASSERT_EQUAL_INT(0, list.listLength());

    classNodeVariant* first = new classNodeVariant(10);
    classNodeVariant* second = new classNodeVariant(20);
    TEST_ASSERT_NULL(first->nextPtr);
    TEST_ASSERT_EQUAL_INT(0, list.addFirst(first));
    TEST_ASSERT_EQUAL_INT(1, list.listLength());
    TEST_ASSERT_EQUAL_INT(0, list.addFirst(second));
    TEST_ASSERT_EQUAL_INT(2, list.listLength());
    TEST_ASSERT_EQUAL_PTR(first, second->nextPtr);
    TEST_ASSERT_NULL(first->nextPtr);
}

/// Create a LinkedList. Add nodes (10, then 20) using addLast.
/// Capture std::cout and verify elements appear in order ("10" before "20").
void test_linkedList_addLast_places_at_end(void) 
{
    LinkedList list;
    TEST_ASSERT_EQUAL_INT(-1, list.addLast(nullptr));
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
    TEST_ASSERT_EQUAL_INT(0, list.addLast(new classNodeVariant(10)));
    TEST_ASSERT_EQUAL_INT(0, list.addLast(new classNodeVariant(20)));
    TEST_ASSERT_EQUAL_INT(2, list.listLength());

    std::ostringstream output;
    std::streambuf* oldBuffer = std::cout.rdbuf(output.rdbuf());
    int result = list.printList();
    std::cout.rdbuf(oldBuffer);

    std::string printed = output.str();
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE_MESSAGE(printed.find("10") != std::string::npos, "Did not print 10.");
    TEST_ASSERT_TRUE_MESSAGE(printed.find("20") != std::string::npos, "Did not print 20.");
    TEST_ASSERT_TRUE_MESSAGE(printed.find("10") < printed.find("20"), "Nodes were not printed in insertion order.");
}

/// Create a LinkedList with an int, double, and string.
/// Call deleteValue() with the double value (3.14).
/// Verify list length decreases to 2 and second call returns -1.
void test_linkedList_deleteValue_removes_variant(void) 
{
    LinkedList list;
    TEST_ASSERT_EQUAL_INT(-1, list.deleteValue(10));
    list.addLast(new classNodeVariant(10));
    list.addLast(new classNodeVariant(3.14));
    list.addLast(new classNodeVariant(std::string("test")));

    TEST_ASSERT_EQUAL_INT(0, list.deleteValue(3.14));
    TEST_ASSERT_EQUAL_INT(2, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.deleteValue(3.14));
    TEST_ASSERT_EQUAL_INT(-1, list.deleteValue(10.0));
    TEST_ASSERT_EQUAL_INT(2, list.listLength());
    TEST_ASSERT_EQUAL_INT(0, list.deleteValue(std::string("test")));
    TEST_ASSERT_EQUAL_INT(1, list.listLength());
    TEST_ASSERT_EQUAL_INT(0, list.deleteValue(10));
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.deleteValue(10));

    list.addLast(new classNodeVariant(10));
    list.addLast(new classNodeVariant(10));
    list.addLast(new classNodeVariant(20));
    TEST_ASSERT_EQUAL_INT(0, list.deleteValue(10));
    TEST_ASSERT_EQUAL_INT(2, list.listLength());
    TEST_ASSERT_EQUAL_INT(0, list.deleteValue(10));
    TEST_ASSERT_EQUAL_INT(1, list.listLength());
    TEST_ASSERT_EQUAL_INT(0, list.deleteValue(20));
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
}

/// Create a LinkedList and insert three nodes.
/// Call destroyList().
/// Verify listLength() becomes 0.
void test_linkedList_destroyList_clears_all(void) 
{
    LinkedList list;
    list.destroyList();
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
    list.addLast(new classNodeVariant(10));
    list.addLast(new classNodeVariant(2.5));
    list.addLast(new classNodeVariant(std::string("test")));
    TEST_ASSERT_EQUAL_INT(3, list.listLength());

    list.destroyList();
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.printList());
    list.destroyList();
    TEST_ASSERT_EQUAL_INT(0, list.listLength());

    TEST_ASSERT_EQUAL_INT(0, list.addLast(new classNodeVariant(40)));
    TEST_ASSERT_EQUAL_INT(1, list.listLength());
    TEST_ASSERT_EQUAL_INT(0, list.deleteValue(40));
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
}

/// Create a LinkedList with nodes (10, 20, 30).
/// Call deleteFirst().
/// Verify listLength() becomes 2 and operation returns 0.
void test_linkedList_deleteFirst(void) 
{
    LinkedList list;
    TEST_ASSERT_EQUAL_INT(-1, list.deleteFirst());
    list.addLast(new classNodeVariant(10));
    list.addLast(new classNodeVariant(20));
    list.addLast(new classNodeVariant(30));

    TEST_ASSERT_EQUAL_INT(0, list.deleteFirst());
    TEST_ASSERT_EQUAL_INT(2, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.deleteValue(10));
    TEST_ASSERT_EQUAL_INT(0, list.deleteFirst());
    TEST_ASSERT_EQUAL_INT(1, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.deleteValue(20));
    TEST_ASSERT_EQUAL_INT(0, list.deleteFirst());
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.deleteFirst());
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
    TEST_ASSERT_EQUAL_INT(0, list.addLast(new classNodeVariant(40)));
    TEST_ASSERT_EQUAL_INT(0, list.deleteValue(40));
}

/// Create a LinkedList with nodes (10, 20, 30).
/// Call deleteLast().
/// Verify listLength() becomes 2 and operation returns 0.
void test_linkedList_deleteLast(void) 
{
    LinkedList list;
    TEST_ASSERT_EQUAL_INT(-1, list.deleteLast());
    list.addLast(new classNodeVariant(10));
    list.addLast(new classNodeVariant(20));
    list.addLast(new classNodeVariant(30));

    TEST_ASSERT_EQUAL_INT(0, list.deleteLast());
    TEST_ASSERT_EQUAL_INT(2, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.deleteValue(30));
    TEST_ASSERT_EQUAL_INT(0, list.deleteLast());
    TEST_ASSERT_EQUAL_INT(1, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.deleteValue(20));
    TEST_ASSERT_EQUAL_INT(0, list.deleteLast());
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
    TEST_ASSERT_EQUAL_INT(-1, list.deleteLast());
    TEST_ASSERT_EQUAL_INT(0, list.listLength());
    TEST_ASSERT_EQUAL_INT(0, list.addLast(new classNodeVariant(40)));
    TEST_ASSERT_EQUAL_INT(0, list.deleteValue(40));
}

/// Create a LinkedList with nodes (1, 2.5, "test").
/// Redirect std::cout buffer and call printList().
/// Verify printed output contains "1", "2.5" (or "2.50"), and "test".
void test_linkedList_printList(void) 
{
    LinkedList list;
    std::ostringstream output;
    std::streambuf* oldBuffer = std::cout.rdbuf(output.rdbuf());
    int result = list.printList();
    std::cout.rdbuf(oldBuffer);
    TEST_ASSERT_EQUAL_INT(-1, result);
    TEST_ASSERT_TRUE_MESSAGE(output.str().empty(), "Empty list should not print values.");

    list.addLast(new classNodeVariant(1));
    list.addLast(new classNodeVariant(2.5));
    list.addLast(new classNodeVariant(std::string("test")));
    oldBuffer = std::cout.rdbuf(output.rdbuf());
    result = list.printList();
    std::cout.rdbuf(oldBuffer);

    std::string printed = output.str();
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE_MESSAGE(printed.find("1") != std::string::npos, "Did not print 1.");
    TEST_ASSERT_TRUE_MESSAGE(printed.find("2.5") != std::string::npos, "Did not print 2.5.");
    TEST_ASSERT_TRUE_MESSAGE(printed.find("test") != std::string::npos, "Did not print test.");
    TEST_ASSERT_EQUAL_INT(3, list.listLength());
}
