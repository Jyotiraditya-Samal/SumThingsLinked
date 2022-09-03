
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Trace() 
// printf("%s: %d\n", __FUNCTION__, __LINE__)

/**********************************************
Declared node blueprint
**********************************************/
struct Node
{
	int value;
  struct Node* prev;
	struct Node* next;
};
typedef struct Node Node;

struct LLInteger
{
	Node* head;
	int size;
};
typedef struct LLInteger LLInteger;

/**********************************************
Create a new node from item and returns pointer
to the newly created node
**********************************************/
Node* createNode(int num,Node* lastNode)
{
	Node* newNode = (Node *)malloc(sizeof(Node));

	if (newNode) // sanity check
	{
		newNode->value = num;
    newNode->prev = lastNode;
		newNode->next = NULL;
	}
	else
	{
		printf("createNode:: could not malloc node !\n");
	}

	return newNode;
}

/**********************************************
Free a node
**********************************************/
void destroyNode(Node* node)
{
	if (node) // sanity check
	{
		free(node);
		return;
	}
	printf("destroyNode:: Not the end of the world but you just tried to free a null pointer !\n");
}

/**********************************************
appends item to list
**********************************************/
void appendNodeToList(LLInteger* list, int value)
{
	assert(list != NULL && "Invalid List Head\n");  // idiot proofing
  Node* prevNode = NULL;
  Node* newNode = createNode(value,prevNode);
	assert(newNode != NULL && "Could not allocate Memory\n"); // sanity check

	if (!list->head) // Appending first node to empty list
	{
		list->head = newNode;
		return;
	}

	Node* iterator = list->head;

	while (iterator->next) // iterate until we find the last node (whose 'next' is null)
		iterator = iterator->next;

	iterator->next = newNode; // set the new node as the last node
}

/**********************************************
Creates a link list given the head...
**********************************************/
LLInteger* createLLInteger()
{
	LLInteger* list = (LLInteger*)malloc(sizeof(LLInteger));

	assert(list != NULL && "Failed to allocate LLInteger\n");  // sanity + mandate

	printf("Enter positive integer to be added: ");
	int number = 0;
	assert(scanf("%d", &number) != 0 && "Invalid user input. Must be an integer\n");
	assert(number >= 0 && "Thats not a positive integer\n");

	// calcualte the number of digits entered
	list->size = 1 + (int)log10(number);
	list->head = NULL;

	for (int fillPos = list->size - 1; fillPos >= 0; fillPos--)
	{
		int tempVal = (number / (int)pow(10, fillPos)) % 10; // extract the digit

		appendNodeToList(list, tempVal);
	}
	printf("\n");

	return list;
}

/**********************************************
Destroys the entire list
**********************************************/
void destroyLLInteger(LLInteger* list)
{
	if (!list)
		return;

	Node* freePos = list->head;
	for (; freePos != NULL;) // iterate through list until we reach null (which is end of list)
	{
		Node* next = freePos->next;
		destroyNode(freePos);
		freePos = next;
	}
	list->head = NULL;

	free(list);
}

/**********************************************
helper function: Reverse linkedlist
**********************************************/
void reverseList(LLInteger* list)
{
	if (!list) // sanity
		return;

	Node* current = list->head;

	while (current && current->next) {
		Node *temp = current->next;
		current->next = temp->next;
		temp->next = list->head;
		list->head = temp;
	}
}

/**********************************************
Add two linkedlists integers and return sum
**********************************************/
LLInteger* addTwoLLIntegers(LLInteger* num1, LLInteger* num2)
{
	LLInteger* sum = (LLInteger*)malloc(sizeof(LLInteger));

	assert(sum != NULL && "Failed to allocate LLInteger\n");  // sanity + mandate
	sum->head = NULL;

	if (num1)
		reverseList(num1); // for easier addition and carry

	if (num2)
		reverseList(num2); // for easier addition and carry

	Node* l1 = num1 ? num1->head : NULL;
	Node* l2 = num2 ? num2->head : NULL;

	int carry = 0;

	while (l1 || l2)
	{
		int digitSum = carry;

		if (l1)
		{
			digitSum += l1->value;
			l1 = l1->next;
		}

		if (l2)
		{
			digitSum += l2->value;
			l2 = l2->next;
		}

		carry = digitSum / 10;

		appendNodeToList(sum, digitSum % 10);

		sum->size++;
	}

	if (num1)
		reverseList(num1); // restore

	if (num2)
		reverseList(num2); // restore

	if (sum->size) // did we really add anything ?
		reverseList(sum);  // reverse result to make sense

	return sum;
}

/**********************************************
Add any number of lists
**********************************************/
LLInteger* addLLIntegers(int numItemsToAdd, ...)
{
	LLInteger* sum = NULL;

	// Declaring pointer to the
	// argument list
	va_list llIntArgumentPointer;

	// Initializing argument to the
	// list pointer
	va_start(llIntArgumentPointer, numItemsToAdd);

	for (int i = 0; i < numItemsToAdd; i++)
	{
		// Accessing current variable
		// and pointing to next one
		LLInteger* nextNumber = va_arg(llIntArgumentPointer, LLInteger*);

		LLInteger* result = addTwoLLIntegers(sum, nextNumber);

		destroyLLInteger(sum); // dont need the previous sum to remain allocated any more

		sum = result;
	}

	// Ending argument list traversal
	va_end(llIntArgumentPointer);

	return sum;
}

/**********************************************
Display List contents
**********************************************/
void showLLInteger(LLInteger* list)
{
	if (!list)
	{
		printf("showList:: Integer list is empty ! First create a new integer\n");
		return;
	}

	for (Node* pickPos = list->head; pickPos != NULL;)
	{
		printf("%d", pickPos->value);
		pickPos = pickPos->next;
	}
	printf("\n");
}


/**********************************************
Main Entry
**********************************************/
int main(void)
{
	printf("Program to add to linkedlist based integers\n");
	printf("-------------------------------------------\n");

	LLInteger* num1 = createLLInteger();

	LLInteger* num2 = createLLInteger();

	LLInteger* sum = addLLIntegers(2, num1, num2); // 2 represents we are adding two numbers

	showLLInteger(sum);

	// cleanup
	destroyLLInteger(num1);
	destroyLLInteger(num2);
	destroyLLInteger(sum);

	return 0;
}
