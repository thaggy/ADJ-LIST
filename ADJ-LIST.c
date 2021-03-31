#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next;
    struct node* adj;
}Node;
//The complexity of this algorithim would be O(V+E) where V is the vertexes and E is the edges.
//The first While loop that is used to find the Min and Max is only searching the Edges so it is O(E)
//And then for the creation of the linked list, it takes O(V) time because we have the Max and Min amounts and can simply just create a list with the total amount of Vertices, V
//And then for the last FOR loop which adds the edges into the linked list, it takes O(E) time because it is looping through the list of edges V*E times which is just E because it is still linear.
//So by adding together our total complexity, we will get O(V+E)


Node* createNode(int);
Node* addToHead(Node* , int );;
Node* ADJList(Node*, int, int, int);
void PrintADJList(Node*);
void freeList(Node*);

int main(void)
{
    FILE* fp;
    //Two temporary ints used to find search through the list
    int traverseA;
    int traverseB;

    //Ints that will be storing the minimum and maximum ints found to make our list more efficient later
    //Max gets a random dummy value since we know that -1 can't be the max
    int max=-1;
    //Min is different since technically a negative number would be lower than any number such as 0 or 1 so we will have to take a specific measure to fix this
    int min = -1;
    //We create this value which will be used in our while loop to check if min has been initialized
    int mindummyvalue=0;

    //Finding file which I have named in this case mcf for Machine C file
    fp = fopen("mcf.txt", "r");
    
    //If its not found, exit the program to avoid seg faults or anything bad
    if (!fp)
    {
        printf("File not found");
        exit(0);
    }
    
    //Finding Min and Max by reading file until EOF is hit
    while (fscanf(fp,"%d %d\n", &traverseA, &traverseB) != EOF)
    {
        //Searching for lowest number
        if (min > traverseA)
        {
            min = traverseA;
            mindummyvalue++;
        }
        else if (min > traverseB)
        {
            min = traverseB;
            mindummyvalue++;
        }

        //Finding max number
        if (max < traverseA)
        {
            max = traverseA;
        }
        else if (max < traverseB)
        {
            max = traverseB;
        }

        //Since min == -1, unless there is a -2 or below vertex, min will not get changed, so to fix this, we don't compare min to a variable, we instead compare the variables and set min to the lower one of those
        if (mindummyvalue == 0)
        {
            if (traverseA < traverseB)
                min = traverseA;
            else
                min = traverseB;
        }
    }

    //Rewinding so that the file is usable
    rewind(fp);
    
    //Creating head of our list
    Node* list_ptr = NULL;

    //Making the list in a FOR loop, so now we have every value stored in the list
    for (int i = max; i > min - 1; i--)
    {
        list_ptr = addToHead(list_ptr, i);
    }

    //Reading Ints from file
    for(int i = max; i > min-1; i--)
    {
        while (fscanf(fp, "%d %d", &traverseA, &traverseB) != EOF)
        {
            list_ptr = ADJList(list_ptr, i, traverseA, traverseB);
        }
        rewind(fp);
    }
    PrintADJList(list_ptr);
    freeList(list_ptr);
    return 0;
}

Node* createNode(int data)
{
    //Creating Node
    Node* temp;
    temp = malloc(sizeof(Node));
    //If the node is empty, malloc did something wrong so we should exit the code
    if (!temp)
    {
        printf("Oops! Something went wrong!");
        exit(9);
    }
    //Setting all the values to the desired values
    temp->data = data;
    temp->next = NULL;
    temp->adj = NULL;
    return temp;
}

Node* addToHead(Node* Head, int info)
{
    //Simple insert to head function for Linked List
    Node* temp = createNode(info);
    //Setting Temps next pointer to be the head
    temp->next = Head;
    //Move the head up
    Head = temp;
    return Head;
}

void PrintADJList(Node* head)
{
    //Prints full adj list by using two pointers. One pointer points to the head and traverses the list normally.
    //However in the second FOR loop, we can set the second pointer to the first value in temps adj list and then traverse that list
    for (Node* temp = head; temp != NULL; temp = temp->next)
    {
        printf("%d - ",temp->data);
        for (Node* Alsotemp = temp->adj; Alsotemp != NULL; Alsotemp = Alsotemp->adj)
        {
            printf("%d ", Alsotemp->data);
        }
        printf("\n");
    }
}

Node* ADJList(Node* head, int check, int first, int second)
{
    //Creating Nodes to be inserted
    Node * insertfir = createNode(first);
    Node * insertsec = createNode(second);
    //Looping through the list
    for(Node * temp = head; temp != NULL; temp = temp->next)
    {
        //If TEMP->DATA == Check, we want nothing to be done
        if (temp->data == check)
        {
            continue;
        }
        //If the data, is in first or second, we want to insert the opposite
        if(temp->data == first || temp->data == second)
        {
            if(check == first || check == second)
            {
                if(check == first)
                {
                    insertfir->adj = temp->adj;
                    temp->adj = insertfir;
                    free(insertsec);
                }
                else
                {
                    insertsec->adj = temp->adj;
                    temp->adj = insertsec;
                    free(insertfir);
                }
            }
        }
    }
    return head;
}

void freeList(Node* head)
{
    Node * temp = head;
    Node * twotemp = head;
    Node * trail;
    while(temp->next!=NULL)
    {
        twotemp = temp->adj;
        while(twotemp->adj != NULL)
        {
            trail = twotemp;
            twotemp = twotemp->adj;
            free(trail);
        }
        free(twotemp);
        trail = temp;
        temp=temp->next;
        free(trail);
    }
    twotemp = temp->adj;
    while(twotemp->adj != NULL)
    {
        trail = twotemp;
        twotemp = twotemp->adj;
        free(trail);
    }
    free(twotemp);
    free(temp);
}
