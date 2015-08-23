#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "test_utilities.h"
#include "mtm_set.h"

#define IF_NULL_RETURN_NULL(var) { \
		if ( ((var) == NULL)) return NULL; }
#define TO_STRING(str) ((char *)(str))

SetElement copyInt(SetElement x) {
	assert(x != NULL);
	int *a = malloc(sizeof(*a));
	IF_NULL_RETURN_NULL(a)
	*a = *((int *) x);
	return a;
}

int compareInt(SetElement num1, SetElement num2) {
	assert(num1 != NULL);
	assert(num2 != NULL);
	return *(int*) num2 - *(int*) num1;
}

SetElement copyString(SetElement inputStr) {
	char *inputString = TO_STRING(inputStr);
	assert(inputString != NULL);
	char *newString = malloc(sizeof(*newString) * (strlen(inputString) + 1));
	IF_NULL_RETURN_NULL(newString)
	strcpy(newString, inputString);
	return newString;
}

static bool testSetCreate() {
	Set set;
	ASSERT_TEST(setCreate(NULL,NULL,NULL) == NULL); // Check NULL set
	set = setCreate(copyInt, free, compareInt); // Check int set
	ASSERT_TEST(set != NULL);
	setDestroy(set);
	set = setCreate(copyString, free, (int (*)(SetElement, SetElement)) strcmp); // Check string set
	ASSERT_TEST(set != NULL);
	SetResult result;
	char *lyrics[5] = { "All", "You", "Need", "Is", "Love" };
	int i;
	for (i = 0; i < 5; i++) {
		result = setAdd(set, lyrics[i]); // Inserting backward
		ASSERT_TEST(result == SET_SUCCESS);
	}
	for (SetElement iterator = setGetFirst(set); iterator != NULL; iterator =
			setGetNext(set)) {
		printf("%s", (char *) iterator);
		//ASSERT_TEST(!(strcmp(iterator, lyrics[--i]))); // strcmp = 0 when equal
	}
	printf("\n%d\n", setGetSize(set));
	for (i = 0; i < 5; i++) {
		result = setRemove(set, lyrics[2 * i % 4]); // Inserting backward

	}
	for (SetElement iterator = setGetFirst(set); iterator != NULL; iterator =
			setGetNext(set)) {
		printf("%s", (char *) iterator);
		//ASSERT_TEST(!(strcmp(iterator, lyrics[--i]))); // strcmp = 0 when equal
	}
	printf("\n%d\n", setGetSize(set));
	for (i = 0; i < 5; i++) {
		result = setAdd(set, lyrics[i]); // Inserting backward
		ASSERT_TEST(0 == strcmp(lyrics[i], setContains(set,lyrics[i])));
	}
	for (SetElement iterator = setGetFirst(set); iterator != NULL; iterator =
			setGetNext(set)) {
		printf("%s", (char *) iterator);

	}
	ASSERT_TEST(setGetSize(set) == 5);
	setClear(set);
	ASSERT_TEST(setGetSize(set) == 0);
	setDestroy(set);
	return true;
}

static bool testSetCopy() {
	Set set, cpySet;
	set = setCreate(copyInt, free, compareInt);
	ASSERT_TEST(set != NULL);
	cpySet = setCopy(set);
	ASSERT_TEST(cpySet != NULL);
	setDestroy(cpySet);
	setDestroy(set);
	// Test string set copy
	set = setCreate(copyString, free, (int (*)(SetElement, SetElement)) strcmp);
	ASSERT_TEST(set != NULL);
	ASSERT_TEST(cpySet != NULL);
	/*char *favouriteHWs[4]={"MTM_HW1", "MTM_HW2", "MTM_HW3", "MTM_HW0"};
	 int i;
	 SetResult result;
	 for (i=0; i<4; i++) {
	 result=setInsertFirst(set,favouriteHWs[i]); // Inserting backward
	 ASSERT_TEST(result == SET_SUCCESS);
	 }*/
	cpySet = setCopy(set);
	/*for (SetElement iterator = setGetFirst(cpySet); iterator != NULL;
	 iterator = setGetNext(cpySet)) { // Go through the words
	 ASSERT_TEST(!(strcmp(iterator, favouriteHWs[--i]))); // strcmp = 0 when equal
	 }
	 Set cpySetToCheckIterator=setCopy(cpySet);
	 // Check that copied set will save the iterator location (currently at "MTM_HW1")
	 ASSERT_TEST(!(strcmp(setGetCurrent(cpySetToCheckIterator), "MTM_HW1")));
	 ASSERT_TEST(!(strcmp(setGetCurrent(cpySet), setGetCurrent(cpySetToCheckIterator))));
	 */setDestroy(set);
	setDestroy(cpySet);
	//setDestroy(cpySetToCheckIterator);
	return true;
}

/* static bool testSetGetSize() {
 ASSERT_TEST(setGetSize(NULL) == -1); // Check if NULL set returns -1
 Set set=setCreate(copyInt,free);
 SetResult createResult;
 ASSERT_TEST(setGetSize(set) == 0); // Check if empty set is 0
 for (int elementInt=0; elementInt<15;elementInt++) {
 createResult=setInsertFirst(set,&elementInt);
 ASSERT_TEST(createResult == SET_SUCCESS);
 ASSERT_TEST(setGetSize(set) == elementInt+1); // if size is OK
 }
 setClear(set);
 ASSERT_TEST(setGetSize(set) == 0); // if set after being cleared is 0
 for (int elementInt=0; elementInt<99;elementInt++) {
 createResult=setInsertLast(set,&elementInt);
 ASSERT_TEST(createResult == SET_SUCCESS);
 ASSERT_TEST(setGetSize(set) == elementInt+1); // if size is OK
 }
 setDestroy(set);
 set=NULL;
 ASSERT_TEST(setGetSize(set) == -1); // check if NULL again
 return true;
 }

 static bool testSetGetFirst() {
 Set set=setCreate(copyInt,free);
 SetResult result;
 int elementInt;
 for (elementInt=0; elementInt < 500; elementInt++) {
 result=setInsertFirst(set,&elementInt); // Add node to the beginning
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(*(int *)setGetFirst(set) == elementInt); // check if
 // the node has been added successfully using setGetFirst
 }
 ASSERT_TEST(setGetSize(set) == 500);
 result=setClear(set);
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(setGetFirst(set) == NULL); // check for empty set
 ASSERT_TEST(setGetFirst(NULL) == NULL); // check for NULL parameter
 setDestroy(set);
 return true;
 }

 static bool testSetGetNext() {
 Set set=setCreate(copyInt,free);
 SetResult result;
 int elementInt;
 result=setInsertFirst(set,&elementInt); // Add node to the beginning
 ASSERT_TEST(result == SET_SUCCESS);
 setGetFirst(set);
 for (elementInt=1; elementInt < 250; elementInt++) {
 result=setInsertAfterCurrent(set,&elementInt); // Add node
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(*(int *)setGetNext(set) == elementInt); // check if
 // the node has been added successfully using setGetFirst
 }
 ASSERT_TEST(setGetSize(set) == 250);
 result=setClear(set);
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(setGetNext(set) == NULL); // check for empty set
 ASSERT_TEST(setGetNext(NULL) == NULL); // check for NULL parameter
 setDestroy(set);
 return true;
 }

 static bool testSetInsertFirst() {
 Set set=setCreate(copyInt,free);
 SetResult result;
 int elementInt;
 for (elementInt=0; elementInt<3;elementInt++) {
 result=setInsertFirst(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(*(int *)setGetFirst(set) == elementInt);
 }
 setClear(set);
 for (elementInt=0; elementInt<10;elementInt++) {
 result=setInsertFirst(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(*(int *)setGetFirst(set) == elementInt);
 }
 int fifteen=15;
 result=setInsertLast(set,&fifteen);
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(*(int *)setGetFirst(set) == elementInt-1);
 int twenty=20;
 result=setInsertFirst(set,&twenty);
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(*(int *)setGetFirst(set) == 20);
 ASSERT_TEST(setInsertFirst(set,NULL) == SET_NULL_ARGUMENT);
 ASSERT_TEST(setInsertFirst(NULL,&fifteen) == SET_NULL_ARGUMENT);
 setDestroy(set);
 return true;
 }

 static bool testSetInsertLast() {
 Set set=setCreate(copyInt,free);
 SetResult result;
 int elementInt;
 for (elementInt=0; elementInt<121;elementInt++) {
 result=setInsertLast(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 }
 ASSERT_TEST(setGetSize(set) == 121);
 setClear(set);
 int i=0;
 result=setInsertLast(set,&i);
 ASSERT_TEST(result == SET_SUCCESS);
 SetElement lastElement=setGetFirst(set);
 for (elementInt=1; elementInt<10;elementInt++) {
 result=setInsertLast(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 lastElement=setGetNext(set);
 ASSERT_TEST(*(int *)lastElement == elementInt);
 }
 ASSERT_TEST(setGetSize(set) == 10);
 ASSERT_TEST(setInsertLast(set,NULL) == SET_NULL_ARGUMENT);
 ASSERT_TEST(setInsertLast(NULL,&i) == SET_NULL_ARGUMENT);
 setDestroy(set);
 return true;
 }

 static bool testSetInsertBeforeCurrent() {
 Set set=setCreate(copyInt,free); // Check int set
 int a=4;
 // Check for empty set
 ASSERT_TEST(setInsertBeforeCurrent(set,&a) == SET_INVALID_CURRENT);
 SetResult result;
 int setNewItems=5;
 for (int elementInt=setNewItems; elementInt>0;elementInt--) {
 result=setInsertFirst(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 }
 // Set is now 1,2,3,4,5
 int x;
 for (SetElement iterator = setGetFirst(set); iterator != NULL;
 iterator = setGetNext(set)) { // Go through the numbers
 x=0;
 result=setInsertBeforeCurrent(set,&x);
 ASSERT_TEST(result == SET_SUCCESS);
 }
 // Set is now 0,1,0,2,0,3,0,4,0,5. Let's check !
 for (SetElement iterator = setGetFirst(set); iterator != NULL;
 iterator = setGetNext(set)) { // Go through the numbers
 if (x%2 == 0) { // Even index of set, data will be 0
 ASSERT_TEST(*(int *)setGetCurrent(set) == 0);
 }
 else { 			// Odd index of set, data will be 1,2,3,4,5,...
 ASSERT_TEST(*(int *)setGetCurrent(set) == x/2+1);
 }
 x++;
 }
 // Check for sending NULL data
 ASSERT_TEST(setInsertBeforeCurrent(set,NULL) == SET_NULL_ARGUMENT);
 setDestroy(set);
 return true;
 }
 //
 static bool testSetInsertAfterCurrent() {
 Set set=setCreate(copyInt,free); // Check int set
 int a=4;
 // Check for empty set
 ASSERT_TEST(setInsertAfterCurrent(set,&a) == SET_INVALID_CURRENT);
 SetResult result;
 int setNewItems=5;
 for (int elementInt=setNewItems; elementInt>0;elementInt--) {
 result=setInsertFirst(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 }
 // Set is now 1,2,3,4,5
 int x;
 for (SetElement iterator = setGetFirst(set); iterator != NULL;
 iterator = setGetNext(set)) { // Go through the numbers
 x=0;
 result=setInsertAfterCurrent(set,&x);
 iterator = setGetNext(set); // I have to skip the added item to
 ASSERT_TEST(result == SET_SUCCESS); // avoid infinite loop
 }
 // Set is now 1,0,2,0,3,0,4,0,5,0. Let's check !
 for (SetElement iterator = setGetFirst(set); iterator != NULL;
 iterator = setGetNext(set)) { // Go through the numbers
 if (x%2 != 0) { // Odd index of set, data will be 0
 ASSERT_TEST(*(int *)setGetCurrent(set) == 0);
 }
 else { 			// Even index of set, data will be 1,2,3,4,5,...
 ASSERT_TEST(*(int *)setGetCurrent(set) == x/2+1);
 }
 x++;
 }
 // Check for sending NULL data
 ASSERT_TEST(setInsertAfterCurrent(set,NULL) == SET_NULL_ARGUMENT);
 setDestroy(set);
 return true;
 }

 static bool testSetRemoveCurrent() {
 Set set=setCreate(copyInt,free); // Check int set
 SetResult result;
 int setNewItems=10;
 for (int elementInt=setNewItems; elementInt>0;elementInt--) {
 result=setInsertFirst(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(setGetSize(set) == 1); // Added 1 object
 setGetFirst(set);
 setRemoveCurrent(set);
 ASSERT_TEST(setGetSize(set) == 0); // Deleted it
 }
 ASSERT_TEST(setRemoveCurrent(set) == SET_INVALID_CURRENT); // Empty set
 ASSERT_TEST(setRemoveCurrent(NULL) == SET_NULL_ARGUMENT); // NULL argument
 setDestroy(set);
 return true;
 }


 static bool testSetSort() {
 Set set=setCreate(copyInt,free); // Check int set
 SetResult result;
 int setNewItems=10;
 for (int elementInt=setNewItems; elementInt>0;elementInt--) {
 if (elementInt % 2 == 0) {
 result=setInsertFirst(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 }
 else {
 result=setInsertLast(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 }
 } //// Set now is {2,4,6,8,10,9,7,5,3,1}
 result=setSort(set, compareInt);
 ASSERT_TEST(result == SET_SUCCESS);
 // Set is now sorted from 1 to 10
 int i=1;
 for (SetElement iterator = setGetFirst(set); iterator != NULL;
 iterator = setGetNext(set)) { // Go through the numbers
 // make sure all the numbers are sorted
 ASSERT_TEST(*(int *)iterator == i++);
 }
 setDestroy(set);
 return true;
 }


 static bool testSetClear() {
 ASSERT_TEST(setGetSize(NULL) == -1); // Check if NULL set returns -1
 Set set=setCreate(copyInt,free);
 SetResult createResult;
 ASSERT_TEST(setGetSize(set) == 0); // Check if empty set is 0
 for (int elementInt=0; elementInt<15;elementInt++) {
 createResult=setInsertFirst(set,&elementInt);
 ASSERT_TEST(createResult == SET_SUCCESS);
 ASSERT_TEST(setGetSize(set) == elementInt+1); // if size is OK
 }
 setClear(set);
 ASSERT_TEST(setGetSize(set) == 0); // if set after being cleared is 0
 setDestroy(set);
 return true;
 }


 static bool testSetFilter() {
 Set set=setCreate(copyInt,free); // Check int set
 SetResult result;
 int setNumbers=1000;
 // Make a set from 1 to setNumbers
 for (int elementInt=1; elementInt<=setNumbers;elementInt++) {
 result=setInsertFirst(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 }
 int key = 10;
 // filteredSet will contain numbers x between 1 to setNumbers x % key ==0
 Set filteredSet=setFilter(set, isFactor, &key);
 // filteredSet size will be exactly setNumbers/key
 ASSERT_TEST(setGetSize(filteredSet) == setNumbers/key);
 int factors=setNumbers-setNumbers%key;
 for (SetElement iterator = setGetFirst(filteredSet); iterator != NULL;
 iterator = setGetNext(filteredSet)) { // Go through the numbers
 // make sure all the numbers that devide in the set
 ASSERT_TEST(*(int *)setGetCurrent(filteredSet) == factors);
 factors-=key;
 }
 // now let's take those numbers from filteredSet1 and filter by lowerThan
 Set filteredSet2=setFilter(set, isLowerThan, &key);
 ASSERT_TEST(setGetSize(filteredSet2) == key-1);
 int tempKey=key;
 for (SetElement iterator = setGetFirst(filteredSet2); iterator != NULL;
 iterator = setGetNext(filteredSet2)) { // Go through the numbers
 // make sure numbers are right
 ASSERT_TEST(*(int *)setGetCurrent(filteredSet2) == --tempKey);
 }
 // Check for NULL argument as set
 ASSERT_TEST(setFilter(NULL,isLowerThan,&key) == NULL);
 // Check for NULL argument as filter function
 ASSERT_TEST(setFilter(set,NULL,&key) == NULL);
 setDestroy(set);
 setDestroy(filteredSet);
 setDestroy(filteredSet2);

 // Filter strings set
 set=setCreate(copyString,free); // Check string set
 ASSERT_TEST(set != NULL);
 char *frequentWords[8]={"a", "an", "then", "have", "or", "had", "on", "if"};
 int i;
 for (i=0; i<8; i++) {
 result=setInsertFirst(set,frequentWords[i]); // Inserting backward
 ASSERT_TEST(result == SET_SUCCESS);
 }
 ASSERT_TEST(setGetSize(set) == 8); // just to be sure length is good
 key=2; // We'll filter the set to get the 1 or 2 letters words
 Set shortWordsSet=setFilter(set, isShorterThan, &key);
 ASSERT_TEST(setGetSize(shortWordsSet) == 5); // "a", "an", "or", "on", "if"
 setDestroy(set);
 setDestroy(shortWordsSet);
 return true;
 }

 static bool testSetGetCurrent() {
 ASSERT_TEST(setGetCurrent(NULL) == NULL); // Check for NULL argument as set
 Set set=setCreate(copyInt,free); // Check int set
 ASSERT_TEST(setGetCurrent(NULL) == NULL); // Check for empty set
 SetResult result;
 ASSERT_TEST(set != NULL);
 for (int elementInt=0; elementInt<1000;elementInt++) {
 result=setInsertFirst(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(*(int *)setGetFirst(set) == elementInt);
 }
 for (SetElement iterator = setGetFirst(set); iterator != NULL;
 iterator = setGetNext(set)) { // Go through the words
 ASSERT_TEST(!(strcmp(iterator,setGetCurrent(set)))); // Check if
 }								// every node's data is equal to iterator
 setDestroy(set);
 return true;
 }


 static bool testSetDestroy() {
 setDestroy(NULL); // check if i can destroy NULL set
 Set set=setCreate(copyInt,free); // Check int set
 SetResult result;
 ASSERT_TEST(set != NULL);
 for (int elementInt=0; elementInt<1000;elementInt++) {
 result=setInsertFirst(set,&elementInt);
 ASSERT_TEST(result == SET_SUCCESS);
 ASSERT_TEST(*(int *)setGetFirst(set) == elementInt);
 }
 setDestroy(set); // destroying HUGE int set
 set=setCreate(copyString,free); // Check string set
 ASSERT_TEST(set != NULL);
 char *abc[9]={"abc", "cde", "fgh", "ijk", "lmn", "opq", "rst", "uvw", "xyz"};
 int i;
 for (int j=0; j<250; j++) {
 for (i=0; i<9; i++) {
 result=setInsertFirst(set,abc[i]); // Inserting backward
 ASSERT_TEST(result == SET_SUCCESS);
 }
 }
 ASSERT_TEST(setGetSize(set) == 9*250); // check size again
 setDestroy(set); // destroying HUGE string set
 return true;
 }*/

int main() {
	RUN_TEST(testSetCreate);
	RUN_TEST(testSetCopy);
	/*RUN_TEST(testSetGetSize);
	 RUN_TEST(testSetGetFirst);
	 RUN_TEST(testSetGetNext);
	 RUN_TEST(testSetInsertFirst);
	 RUN_TEST(testSetInsertLast);
	 RUN_TEST(testSetInsertBeforeCurrent);
	 RUN_TEST(testSetInsertAfterCurrent);
	 RUN_TEST(testSetRemoveCurrent);
	 RUN_TEST(testSetSort);
	 RUN_TEST(testSetClear);
	 RUN_TEST(testSetFilter);
	 RUN_TEST(testSetGetCurrent);
	 RUN_TEST(testSetDestroy);*/
	return 0;
}
