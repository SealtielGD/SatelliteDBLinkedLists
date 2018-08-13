#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*
Iris Ponce Pinto
Sealtiel Garcia
Data Stuctures
Semester Project
05/08/18
*/


// struct that contains all the satellite points Each node will contain this struct.
typedef struct sat {
	int Date;
	int Time;
	float rmag;
	float NSmag;
	float azmag;
	float avgmag;
} Sate;

typedef struct node {
  Sate *data;
	struct node *next;
	struct node *prev;
} Node;

typedef struct {
	int size;
	Node *head;
	Node *tail;
} SortedLinkedList;

// The Usual print function that prints the data cointained in the List
// O(1) since its just going through the list and printing each node doesn't take long.
void printList(SortedLinkedList *list) {
	Node *current = list->head;
	while (current != NULL) {
	  printf("The Date is:%d ", current->data->Date);
		printf("Time:%d ", current->data->Time);
		printf("RMag:%6.3f ", current->data->rmag);
		printf("NSmag:%6.3f ", current->data->NSmag);
		printf("azmag:%6.3f ", current->data->azmag);
		printf("avgmag:%6.3f \n", current->data->avgmag);
		current = current->next;
	}
	printf("\n");
}

// Inserting new node into the list from the data that is imported every node contains
// date(in number format) time(in millisec) and 4 different magnetic field values depending on the orientation
// O(1) to insert since it is already sorted data and it is just added into the list
void insert(SortedLinkedList* list, int date,int Time,float rmag,float NSmag,float azmag,float avgmag) {
	Node *newPtr = (Node *)malloc(sizeof(Node));
	Sate *Sat = (Sate*) malloc(sizeof(Sate));
	Sat->Date = date;
	Sat->Time = Time;
	Sat->rmag = rmag;
	Sat->NSmag = NSmag;
	Sat->azmag= azmag;
	Sat->avgmag= avgmag;
	newPtr->data = Sat; 
	if(list->size == 0){
		newPtr->next = list->head;
		newPtr->prev = list->tail;
		list->head = newPtr;
		list->tail = newPtr;
	}else{
	  newPtr->next = list->head;
	  newPtr->prev = list->tail;
	  list->head = newPtr;
	}
	list->size++;
}

// Helper Function that prints the data for a specific time and date given
// O(N) because it has to go through every node to check for the exact date and time.
Sate *findTime(SortedLinkedList *list, int target,int date){
	Node *current = list->head;
	for (int i = 0; i < (list->size)+1 && current != NULL; i++) {
		if(current->data->Time == target && current->data->Date == date)
      return current->data;
    else{
      current = current->next;
    }
	}
}

// Function that takes in a range of times and dates then prints out all the data in between.
// O(N^2) in the worst case since first it has to find the first time and date and then iterate until it finds the second time and date.
void findRTime(SortedLinkedList *list, int target,int date1,int target2, int date2){
	Node *first = list->head;
	SortedLinkedList* temp = (SortedLinkedList*) malloc(sizeof(SortedLinkedList));
	for (int i = 0; i < list->size && first != NULL; i++) {
		if(first->data->Time == target && first->data->Date ==date1){
		  insert(temp,first->data->Date,first->data->Time,first->data->rmag,first->data->NSmag,first->data->azmag,first->data->avgmag);
		  first = first->next;
		  while(first->data->Time != target2 && (first->data->Date) == date2 ){
		    insert(temp,first->data->Date,first->data->Time,first->data->rmag,first->data->NSmag,first->data->azmag,first->data->avgmag);
		    first = first->next;
		  }
		}
    else{
      first = first->next;
    }
	}
	printf("The range you asked for contains:%d amount of data\n",temp->size);
  printList(temp);
  
  free(temp);
  
}

//  Function that takes in a list and runs a list of data that all have a noticible jump in magnetic field for this case we use the avg. magnetic field
//O(N) since we are just going through the list and finding big jumps in the avgmag in this case a big jump is 1.5 time bigger than the previous node.
SortedLinkedList *DisFind(SortedLinkedList *list){
  Node *current = list->head;
  Node *next = current->next;
  SortedLinkedList* rlist = (SortedLinkedList*) malloc(sizeof(SortedLinkedList));
  float mult = 1.5;
  for (int i = 0; i < list->size && current != NULL; i++) {
    if((fabs(current->data->avgmag) > fabs(next->data->avgmag)*1.5) || (fabs(current->data->avgmag)*1.5 < fabs(next->data->avgmag))) {
		  insert(rlist,current->data->Date,current->data->Time,current->data->rmag,current->data->NSmag,current->data->azmag,current->data->avgmag);
		  current = current->next;
		  rlist->size++;
	}else{
	  current = current->next;
	}
  }
  printf("This amount of data contains disturbances:%d\n",rlist->size);
  printList(rlist);
  return rlist;
  free(rlist);
}

// Funtion to search for any given field given in this case we could only do a simple find of time and date. 
// But implementation into the other data is fairly intuitive.
void Find(SortedLinkedList *list,int target,int date, char *search) {
	if(strcmp(search, "Time") == 0){
	  Sate *found = findTime(list, target,date);
	  printf("The Node with time:%d\n Is from the date:%d\n Contains the following:",found->Time,found->Date);
	  printf("RMag:%6.3f ", found->rmag);
		printf("NSmag:%6.3f ", found->NSmag);
		printf("azmag:%6.3f ", found->azmag);
		printf("avgmag:%6.3f \n", found->avgmag);
	}

}

int main(void) {
	SortedLinkedList* list1 = (SortedLinkedList*) malloc(sizeof(SortedLinkedList));
	// this code below with the while loop imports the data from the file that is read.
	FILE *file;
	file = fopen("Data1.txt", "r");
	char line[256];
	// this While loop goes through every line and inserts every line into the list. 
	while (fgets(line, sizeof(line), file) != NULL) {
		
		char *dateToken = strtok(line, "   ");
		float value[4];// stores the 4 magnetic values
		for (int i = 0; i < 4; i++) {
			char *token = strtok(NULL, "   ");
			if (token == NULL)
				return 0;
			if (sscanf(token, "%f", &value[i]) != 1)
				return 0;
		}

		// extract the components of the date and time
		int year, month, day, hour, minute, second, millisec;
		char t, z;
		sscanf(
			dateToken,
			"%d-%d-%d%c%d:%d:%d.%d%c",
			&year,
			&month,
			&day,
			&t,
			&hour,
			&minute,
			&second,
			&millisec,
			&z);

		// combine the components into a single number for the date and time
		int date = year * 10000 + month * 100 + day;
		int Time = hour * 3600000 + minute * 60000 + second * 1000 + millisec;
		if(value[0]>-1000)// when inserting it checks for lines that have no data avaible of the form -99999 and excludes it.
		  insert(list1,date,Time,value[0],value[1],value[2],value[3]);
	}
	//implementation of functions
  //printList(list1);
  //Find(list1,570000,19920126,"Time");
  //DisFind(list1);
  // When calling this function call the later date and time first and the first occuring date and time second
  // reason for this is couldnt figure out why I couldn't print the list in increasing date and time rather than decreasing date and time
  //findRTime(list1,43710000,19920125,8250000,19920125);
	return 0;
}