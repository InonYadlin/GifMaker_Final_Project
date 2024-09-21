/*********************************
* Class: MAGSHIMIM C2			 *
* Week:                			 *
* Name:                          *
* Credits:                       *
**********************************/

#include <stdio.h>
#include "view.h"
#include "linkedList.h"

#pragma warning(disable:4996)

#define STR_LEN 100

int printMenu(void);
FrameNode* directRequest(int choice, struct FrameNode* head);
FrameNode* getProjectData(void);

int main(void)
{
	// Initializing variables
	int choice = 0;
	FrameNode* head = getProjectData();
	
	// Loop for the gifmaker
	do
	{
		choice = printMenu();
		head = directRequest(choice, head);
	} while (choice);

	// Printing the goodbye message
	printf("Bye!");

	return 0;
}

/*
* Function which prints the starting menu
* input: none
* output: user's choice
*/
int printMenu(void)
{
	// Initializing variable
	int choice = -1;
	char temp[STR_LEN] = { '\0' };

	// Printing menu
	printf("What would you like to do?\n");
	printf(" [0] Exit\n");
	printf(" [1] Add new Frame\n");
	printf(" [2] Remove a Frame\n");
	printf(" [3] Change frame index\n");
	printf(" [4] Change frame duration\n");
	printf(" [5] Change duration of all frames\n");
	printf(" [6] List frames\n");
	printf(" [7] Play movie!\n");
	printf(" [8] Save project\n");

	// Getting the user's choice
	fgets(temp, 100, stdin);
	choice = (int)(temp[0]) - '0';
	
	return choice;
}

/* 
* Function which directs the user's request
* input: user's choic
* output: pointer to the head
*/
FrameNode* directRequest(int choice, FrameNode* head)
{
	FrameNode* temp = NULL;

	switch (choice)
	{
		case 0:
			freeFrames(head);
			break;
		case 1:
			temp = createNode(head);
			head = temp;
			break;
		case 2:
			temp = removeItem(head);
			head = temp;
			break;
		case 3:
			changeFrameIndex(head);
			break;
		case 4:
			changeItemDuration(head);
			break;
		case 5:
			changeAllDurations(head);
			break;
		case 6:
			printList(head);
			break;
		case 7:
			play(head);
			break;
		case 8:
			saveList(head);
			break;
		default: 
			break;
	}

	return head;

}

/*
* Function which gets the project data 
* input: none
* output: head node
*/
FrameNode* getProjectData(void)
{
	// Initializing variables
	int choice = 0;
	char temp[STR_LEN] = { '\0' };
	FrameNode* head = NULL;

	printf("Welcome to Magshimim Movie Maker! what would you like to do?\n");
	printf(" [0] Create a new project\n");
	printf(" [1] Load existing project\n");

	// Getting the user's choice
	fgets(temp, 100, stdin);
	choice = (int)(temp[0]) - '0';

	// If the option was 0, returning NULL
	if (!choice)
	{
		printf("Working on a new project.\n\n");
		return NULL;
	}

	// Otherwise, returning the head
	return(openFile());
}