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
  Node* tail;
	int size;
};
typedef struct LLInteger LLInteger;

/**********************************************
Create a new node from item and returns pointer
to the newly created node
**********************************************/
Node* createNode(int num)
{
	Node* newNode = (Node *)malloc(sizeof(Node));

	if (newNode) // sanity check
	{
		newNode->value = num;
    newNode->prev =  NULL;
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
appends item to list head
**********************************************/
void appendNodeToListHead(LLInteger* list, int value)
{
	assert(list != NULL && "Invalid List Head\n");  // idiot proofing
  // this is also not needed - Node* prevNode = NULL; sorry for this again!! :(
  Node* newNode = createNode(value); // litu:: not needed -> ,prevNode);
	assert(newNode != NULL && "Could not allocate Memory\n"); // sanity check

  newNode->next = list->head;
  if (list->head) // if list was not empty
    list->head->prev = newNode;

  list->head = newNode; // update head

	if (! list->tail) // Appending first node to empty list
		list->tail = newNode; // update tail
  
}

/**********************************************
appends item to list tail
**********************************************/
void appendNodeToListTail(LLInteger* list, int value)
{
	assert(list != NULL && "Invalid List Head\n");  // idiot proofing
  // this is also not needed - Node* prevNode = NULL; sorry for this again!! :(
  Node* newNode = createNode(value); // litu:: not needed -> ,prevNode);
	assert(newNode != NULL && "Could not allocate Memory\n"); // sanity check

	if (! list->head) // Appending first node to empty list
	{
		list->head = newNode;
    list->tail = list->head;
		return;
	}

	list->tail->next = newNode;
  newNode->prev = list->tail;

  list->tail = newNode;  // set the new node as the last node
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
	list->size = 1 + (number ? (int)log10(number) : 0); 
	list->head = NULL;
  list->tail = NULL;

	for (int fillPos = list->size - 1; fillPos >= 0; fillPos--)
	{
		int tempVal = (number / (int)pow(10, fillPos)) % 10; // extract the digit

		appendNodeToListTail(list, tempVal); //sets tail to last added node
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

#if 0
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
#endif

/**********************************************
Add two linkedlists integers and return sum
**********************************************/
LLInteger* addTwoLLIntegers(LLInteger* num1, LLInteger* num2)
{
	LLInteger* sum = (LLInteger*)malloc(sizeof(LLInteger));

	assert(sum != NULL && "Failed to allocate LLInteger\n");  // sanity + mandate
	sum->head = NULL;
  sum->tail = NULL;

	Node* l1 = num1 ? num1->tail : NULL;
	Node* l2 = num2 ? num2->tail : NULL;

	int carry = 0;

	while (l1 || l2)
	{
		int digitSum = carry;

		if (l1)
		{
			digitSum += l1->value;
			l1 = l1->prev;
		}

		if (l2)
		{
			digitSum += l2->value;
			l2 = l2->prev;
		}

		carry = digitSum / 10;

    appendNodeToListHead(sum, digitSum % 10);

		sum->size++;
	}

  if (carry) // dont forget about last carry
  {
    appendNodeToListHead(sum, carry);
  }

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
	printf("\nProgram to add to linkedlist based integers\n");
	printf("-------------------------------------------\n");

	LLInteger* num1 = createLLInteger();

	LLInteger* num2 = createLLInteger();

	LLInteger* sum = addTwoLLIntegers(num1, num2);

  showLLInteger(sum);

	// cleanup
	destroyLLInteger(num1);
	destroyLLInteger(num2);
	destroyLLInteger(sum);
  
	return 0;
}
