#include "view.h"
#include "linkedList.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STR_LEN 500
#define BUFFER_SIZE 100000

#define SUCCESS -2
#define ERROR -1

#define DECIMAL_BASE 10

#pragma warning(disable:4996)

/*
* Function which creates a new node
* input: head of the list
* output: head of the node
*/
FrameNode* createNode(FrameNode* head)
{
	// Initializing variables
	FILE* file = NULL;
	FrameNode* current = head;
	FrameNode* newNode = (FrameNode*)malloc(sizeof(FrameNode));
	char path[STR_LEN] = { '\0' };
	char name[STR_LEN] = { '\0' };
	unsigned int duration = 1;

	// Getting the input
	getInput(path, &duration, name, head);

	file = fopen(path, "rb");

	// If file couldn't be opened (path doesn't exist), free the memory and return
	if (!file)
	{
		free(newNode);
		printf("Can't find file! Frame will not be added\n");
		return head;
	}

	fclose(file);

	// Allocating memory to the struct
	allocateMemory(newNode);

	// Assigning the values into the new node
	newNode->frame->duration = duration;
	strcpy(newNode->frame->name, name);
	strcpy(newNode->frame->path, path);
	newNode->next = NULL;

	// If the head is null, return the newNode
	if (!head)
	{
		return newNode;
	}

	addNodeToList(head, newNode);

	return head;
}

/*
* Function which allocates memory to a new node
* input: pointer to new node
* output: none
*/
void allocateMemory(FrameNode* newNode)
{
	// Allocating the memory
	newNode->frame = (Frame*)malloc(sizeof(Frame));
	newNode->frame->name = (char*)malloc(STR_LEN);
	newNode->frame->path = (char*)malloc(STR_LEN);
}

/*
* Function which connects the new node to the list
* input: head, new node
* output: none
*/
void addNodeToList(FrameNode* head, FrameNode* newNode)
{
	FrameNode* current = head;

	while (current->next)
	{
		current = current->next;
	}
	current->next = newNode;

	return head;
}

/*
* Function which gets the input for the new node
* input: string to store path and name, variable to store duration, node head
* output: none
*/
void getInput(char* path, unsigned int* duration, char* name, FrameNode* head)
{
	char temp = '\n';
	// Getting the path
	printf("*** Creating new frame ***\n");
	printf("Please insert frame path:\n");
	fgets(path, STR_LEN, stdin);
	path[strcspn(path, "\n")] = '\0';

	// Getting the duration
	printf("Please insert frame duration(in miliseconds):\n");
	scanf("%d", duration);
	getchar();

	// Getting the frame's name
	printf("Please choose a name for that frame:\n");
	fgets(name, STR_LEN, stdin);
	name[strcspn(name, "\n")] = '\0';
	printf("\n");

	// Making sure the frame's name is not taken
	if (checkFrameName(name, head) == -1);
	{
		while (checkFrameName(name, head) == -1)
		{
			printf("The name is already taken, please enter another name\n");
			fgets(name, STR_LEN, stdin);
			name[strcspn(name, "\n")] = '\0';
		}
	}
}

/*
* Function which checks if the frame's name isn't taken
* input: frame's name, head node
* output: success or error
*/
int checkFrameName(char* name, FrameNode* head)
{
	// Initializing variables
	FrameNode* current = head;

	// Checking if the head is NULL;
	if (!head)
	{
		return SUCCESS;
	}

	// Checking if the name exists in the list
	while (current)
	{
		if (!strcmp(name, current->frame->name))
		{
			return ERROR;
		}

		current = current->next;
	}

	// If it doesn't, return success
	return SUCCESS;
}

/*
* Function which removes an item from the list
* input: head
* output: success or error 
*/
FrameNode* removeItem(FrameNode* head)
{
	// Initializing variables
	char name[STR_LEN] = { '\0' };
	FrameNode* previous = head;
	FrameNode* current = head;
	FrameNode* temp = NULL;

	// Getting the name of the frame
	printf("Enter the name of the frame you wish to erase\n");
	getString(name);

	// Checking if the name exists in the list
	if (head != NULL)
	{
		current = findName(head, name);

		// If frame wasn't found, return Error
		if (!current)
		{
			printf("The frame was not found\n");
			return head;
		}

		else if (!head->next)
		{
			removeHead(head, name);
			return NULL;
		}

		// Otherwise, remove the frame
		else
		{
			temp = current;

			// Connecting the frames if the removed frame wasn't the last
			if (current->next)
			{
				current->next = current->next->next;
			}

			// if the removed frame was the last
			else
			{
				while (previous->next != current)
				{
					previous = previous->next;
				}
				previous->next = current->next;
			}
		}

		// Freeing the memory
		if (temp)
		{
			// Free the allocated memory
			free(temp->frame->name);
			free(temp->frame->path);
			free(temp->frame);
			free(temp);
		}

		return head;
	}

	printf("The frame was not found\n");
	return head;

}

/*
* Function which removes the head
* input: head, name
* output: none
*/
void removeHead(FrameNode* head, char* name)
{
	// Freeing the allocated memory
	if (!strcmp(head->frame->name, name))
	{
		free(head->frame->name);
		free(head->frame->path);
		free(head->frame);
		free(head);
	}
}

/* 
* Function which shows the frames in the list
* input: head
* output: none
*/
void printList(FrameNode* head)
{
	// Initializing variables
	struct FrameNode* current = head;

	// Printing the values
	printf("\t\tName\t\tDuration\t\tPath\n");

	if (head)
	{
		while (current)
		{
			printf("\t\t%-8s\t%-8d ms\t\t%-8s\n", current->frame->name, current->frame->duration, current->frame->path);

			current = current->next;
		}
	}

	printf("\n\n\n");
}

/*
* Function which changes the duration of the selected frame
* input: head
* output: success/error
*/
int changeItemDuration(FrameNode* head)
{
	// Initializing variables
	char name[STR_LEN] = { '\0' };
	unsigned int newDuration = 1;
	FrameNode* current = head;
	FrameNode* temp = NULL;

	// Getting the name of the frame
	printf("enter the name of the frame\n");
	getString(name);

	// Checking if the name exists in the list
	if (head != NULL)
	{
		current = findName(head, name);

		// If frame wasn't found, return Error
		if (!current)
		{
			printf("The frame was not found\n");
			return ERROR;
		}

		// Get the new duration
		printf("Enter the new duration\n");
		scanf("%d", &newDuration);
		getchar();

		// Put the duration in the head
		if(!head->next)
		{
			head->frame->duration = newDuration;
			return SUCCESS;
		}

		else
		{
			current->frame->duration = newDuration;
			return SUCCESS;
		}
	}

	else
	{
		printf("The frame was not found\n");
		return ERROR;
	}
}

/*
* Function which changes the duration of all the frames
* input: head node
* output: success or error
*/
int changeAllDurations(FrameNode* head)
{
	// Initializing variables
	FrameNode* current = head;
	unsigned int newDuration = 0;

	// Getting the new duration
	printf("Enter the duration for all frames:\n");
	scanf("%d", &newDuration);
	getchar();

	// Changing all durations
	while (current)
	{
		current->frame->duration = newDuration;
		current = current->next;
	}

	return SUCCESS;
}

/*
* Function which changes the index of 2 frames
* input: head
* output: Success or error
*/
int changeFrameIndex(FrameNode* head)
{
	// Initializing variables
	FrameNode* firstNode = head;
	FrameNode* secondNode = head;
	int newIndex = 1;
	int i = 1;
	char name[STR_LEN] = { '\0' };

	// Getting the input
	printf("Enter the name of the frame\n");
	getString(name);

	// Checking if the name exists in the list
	if (head != NULL)
	{
		// Getting the first node
		firstNode = findName(head, name);

		if (!firstNode)
		{
			printf("The frame was not found\n");
			return ERROR;
		}
		
		// Getting the new index
		printf("Enter the new index in the movie you wish to place the frame\n");
		scanf("%d", &newIndex);
		getchar();
		
		// Getting the second node
		while ((secondNode->next) && (i < newIndex))
		{
			i++;
			secondNode = secondNode->next;
		}
		
		// Swapping the index
		swapIndex(firstNode, secondNode);
		return SUCCESS;
	}

	else
	{
		printf("The frame was not found\n");
		return ERROR;
	}
}

/* 
* Function which swaps the data of 2 nodes
* input: 2 nodes to swap
* output: none
*/
void swapIndex(FrameNode* firstNode, FrameNode* secondNode)
{
	// Initializing variables
	char name[STR_LEN] = { '\0' };
	char path[STR_LEN] = { '\0' };
	unsigned int duration = 0;

	// Swapping the nodes
	duration = secondNode->frame->duration;
	strcpy(name, secondNode->frame->name);
	strcpy(path, secondNode->frame->path);

	secondNode->frame->duration = firstNode->frame->duration;
	strcpy(secondNode->frame->name, firstNode->frame->name);
	strcpy(secondNode->frame->path, firstNode->frame->path);

	firstNode->frame->duration = duration;
	strcpy(firstNode->frame->name, name);
	strcpy(firstNode->frame->path, path);
}

/*
* Function which frees all the frames (recursivly)
* input: node to free
* output: none
*/
void freeFrames(FrameNode* node)
{
	// If the node exists
	if (node)
	{
		freeFrames(node->next);
		free(node->frame->path);
		free(node->frame->name);
		free(node->frame);
		free(node);
	}
}

/*
* Function which looks for a name inside the list
* input: head node, name to look for
* output: the node, or NULL if it wasn't found
*/
FrameNode* findName(FrameNode* head, char name[])
{
	// Initializing variable
	FrameNode* current = head;

	// Finding the name
	if (head)
	{
		while (current)
		{
			// If the name is in the list, return the node
			if (!strcmp(current->frame->name, name))
			{
				return current;
			}

			current = current->next;
		}

		if ((!(head->next)) && (!strcmp(head->frame->name, name)))
		{
			return head;
		}
	}

	// If the name isn't in the list, return NULLs
	return NULL;
}

/*
* Function which saves the list to a file
* input: file path, head node
* output: success or error
*/
int saveList(FrameNode* head)
{
	// Initializing variables
	char path[STR_LEN] = { '\0' };
	char buffer[STR_LEN] = { '\0' };
	FILE* file = NULL;
	FrameNode* current = head;

	// Getting the path
	printf("Where to save the project? enter a full path and file name\n");
	getString(path);

	// Creating the path
	file = fopen(path, "w");

	// If file couldn't be created, return Error
	if (!file)
	{
		printf("Error! couldn't create file\n");
		return ERROR;
	}

	// Loop to write to the  file
	while (current)
	{
		// Writing the frame's path
		fwrite(current->frame->path, strlen(current->frame->path), 1, file);
		fwrite(",", 1, 1, file);

		// Writing the frame's duration using itoa (integer to ascii)
		itoa(current->frame->duration, buffer, DECIMAL_BASE);
		fwrite(buffer, strlen(buffer), 1, file);
		fwrite(",", 1, 1, file);

		// Writing the frame's name
		fwrite(current->frame->name, strlen(current->frame->name), 1, file);
		current = current->next;
		fwrite(",\n", 2, 1, file);
	}

	// Closing the file
	fclose(file);

	return SUCCESS;
}

/*
* Function which get the file's path
* input: string to store the path
* output: none
*/
void getString(char string[])
{
	// Getting the path from the user
	fgets(string, STR_LEN, stdin);
	string[strcspn(string, "\n")] = '\0';
}

/*
* Function which opens the file the user provided
* input: none
* output: head node
*/
FrameNode* openFile(void)
{
	// Initializing variables
	char path[STR_LEN] = { '\0' };
	FrameNode* head = NULL;
	int length = 0;
	char buffer[BUFFER_SIZE] = { '\0' };
	FILE* file = NULL;

	// Allocating memory to the head

	// Getting the path
	printf("Enter the path of the project (including project name):\n");
	getString(path);

	// Opening the file
	file = fopen(path, "r");

	// If file couldn't be opened, return NULL;
	if (!file)
	{
		printf("Error!- cant open file, creating a new project");
		free(head);
		return NULL;
	}

	// Read the data from the file
	length = fread(buffer, sizeof(char), BUFFER_SIZE, file);
	buffer[length] = '\0';

	head = assembleList(buffer);

	fclose(file);
	return head;
}

/*
* Function which assembles the list from the buffer
* input: buffer containing all the data
* output: head node
*/
FrameNode* assembleList(char buffer[])
{
	// Initializing variables
	FrameNode* head = NULL;
	FrameNode* previous = NULL;
	char temp[STR_LEN] = { '\0' };
	char temp1[STR_LEN] = { '\0' };
	int length = 0;
	int startIndex = 0;
	int listLength = 0;

	// Loop to get all assets in the buffer
	while (strcspn(buffer, "\n") != strlen(buffer))
	{
		// Creating the new node
		FrameNode* newNode = (FrameNode*)malloc(sizeof(FrameNode));
		allocateMemory(newNode);

		// Getting the indexes
		length = strcspn(buffer, "\n"); - startIndex + 1;

		// Copying the string
		strncpy(temp, buffer + startIndex, length);

		// Getting the new start index
		startIndex = strcspn(buffer, "\n"); + 1;

		// Clearing the previous \n in the buffer
		buffer[strcspn(buffer, "\n")] = ' ';

		// Getting all the data for the new node
		strcpy(newNode->frame->path, strtok(temp, ","));
		strcpy(temp1, strtok(NULL, ","));
		newNode->frame->duration = atoi(temp1);
		temp[strcspn(temp, ",")] = " ";
		strcpy(newNode->frame->name, strtok(NULL, ","));
		newNode->next = NULL;

		listLength++;

		if (!head)
		{
			head = newNode;
			previous = head;
		}

		else if (listLength >= 2)
		{
			previous->next = newNode;
			previous = previous->next;
		}
	}

	return head;
}
