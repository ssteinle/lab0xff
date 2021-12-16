#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <string.h>
#include <Windows.h>
#include <sys/time.h>   // for gettimeofday()
#include <unistd.h>     // for sleep()

#define PI 3.14159265
char test[1000];
struct Node* head = NULL;
int listCounter = 0;

void compareCircles(int **arr, int **holdPermutations, int numVertex)
{
    int list1[numVertex];
    int list2[numVertex];

    int verdict = 0;

    for (int i = 0; i < numVertex; i++)
        for (int j = 0; j < numVertex; j++)
            if(arr[i][j] < 9999)
                list1[j] = i;

    for (int i = 0; i < numVertex; i++)
        if(list1[i] != holdPermutations[1][i])
            verdict = 1;

    if(verdict == 1)
        printf("\nThese lists are Not Equal");
    else
        printf("\nThese lists are Equal");
}

void printTable(int **arr, int numVertex, int numElements)
{
    printf("\n\n");

    for (int i = 0; i < numVertex; i++)
    {
        for (int j = 0; j < numElements; j++)
            printf("%4d ", arr[i][j]);

        printf("\n\n");
    }
        printf("\n");
}

void generateRandomCostMatrix(int **arr, int numVertex, int maxCost)
{
    int count = 0;

    for (int i = 0; i < numVertex; i++)
        for (int j = 0; j < numVertex; j++)
            arr[i][j] = 0;

    for (int i = 0; i < numVertex; i++)
    {
        for (int j = 0; j < numVertex; j++)
        {
            if((i != j) && (arr[j][i] == 0))
            {
                arr[i][j] = (rand() % (maxCost + 1));
                arr[j][i] = arr[i][j];
            }
        }
    }
}

void generateRandomEuclideanMatrix(int **arr, int **holdPermutations, int numVertex)
{
    int realCost;

    for(int i=0; i<numVertex; i++)
    {
        holdPermutations[0][i] = rand() % 101;
        holdPermutations[1][i] = rand() % 101;
    }

    for (int i = 0; i < numVertex; i++)
    {
        for (int j = 0; j < numVertex; j++)
        {
            arr[i][j] = sqrt(pow(holdPermutations[0][j] - holdPermutations[0][i], 2) + pow(holdPermutations[1][j] - holdPermutations[1][i], 2));
        }
    }

    for (int i = 0; i < numVertex; i++)
    {
        for (int j = i; j < numVertex; j++)
        {
            arr[i][j] = 0;
        }
    }

    for (int i = 0; i < numVertex; i++)
        for (int j = 0; j < numVertex; j++)
            arr[i][j] = arr[j][i];
}

void generateRandomEuclideanCircle(int **arr, int numVertex, int radius)
{
    double sliceAngle = ((2 * PI) / numVertex);
    int sliceCost = 0;

    int sliceSize = pow(radius, 2) + pow(radius, 2) - 2 * pow(radius, 2) * cos(sliceAngle);

    sliceSize = sqrt(sliceSize);

    int first;
    int last;

    int previous = -1;
    int count = 0;

    for (int i = 0; i < numVertex; i++)
        for (int j = 0; j < numVertex; j++)
            arr[i][j] = 0;

    while (count < numVertex)
    {
        int selectFrom = previous;
        int selectTo = -1;

        if(count == 0)
        {
            do
            {
                first = rand() % numVertex;
                selectFrom = first;
                selectTo = rand() % numVertex;
            }
            while(selectFrom == selectTo);
        }
        else if(count < numVertex -1)
        {
            do
            {
                selectTo = rand() % numVertex;
                //printf("\n\nTest From: %d To: %d", selectFrom, selectTo);
                //printf("\nCannot be first: %d or previous: %d", first, previous);
            }
            while((previous == selectTo) || (selectTo == first)); 
        }
        else
        {
            selectTo = first;
        }

        int alreadySelected = 0;

        for (int j = 0; j < numVertex; j++)
        {
            //printf("\narr[%d][%d]: %d", count, j, arr[count][j]);

            if((arr[selectFrom][j] != 0) || (arr[j][selectTo] != 0))
                alreadySelected = 1;
        }

        //printf("\nAlready Selected: %d", alreadySelected);
            
        if(alreadySelected == 0)
        {
            printf("\n\nFrom: %d", selectTo);
            printf(" To: %d", selectFrom);

            arr[selectFrom][selectTo] = sliceSize;
            sliceCost = sliceCost + sliceSize;
            previous = selectTo;
            count++;
        }
        else
        {
            //printf("\nAlready Selected");
        }

    }

    //printf("\nTotal circut cost: %d\n", sliceCost);

    for (int i = 0; i < numVertex; i++)
        for (int j = 0; j < numVertex; j++)
        {
            if(arr[j][i] == arr[i][j])
                arr[i][j] = 9999;
        }

    for (int i = 0; i < numVertex; i++)
        for (int j = 0; j < numVertex; j++)
        {
            if((arr[i][j] == 0) && (i != j))
                arr[i][j] = 9999;

            if((arr[j][i] == 0) && (i != j))
                arr[j][i] = 9999;
        }
}

void generateRandomEuclideanCircle2(int numVertex, int radius)
{
    double sliceAngle = ((2 * PI) / numVertex);
    int sliceCost = 0;
    int ySlice;
    int xSlice;

    int sliceSize = pow(radius, 2) + pow(radius, 2) - 2 * pow(radius, 2) * cos(sliceAngle);

    sliceSize = sqrt(sliceSize);

    int coordMax = 2*radius;

    int** holdCircle = (int**)malloc(coordMax * sizeof(int*));

    for (int i = 0; i < coordMax; i++)
        holdCircle[i] = (int*)malloc(coordMax * sizeof(int));


    for (int i = 0; i < numVertex; i++)
        for (int j = 0; j < numVertex; j++)
            holdCircle[i][j] = 0;

    printTable(holdCircle, coordMax, coordMax);
    

    int center = coordMax / 2;

    int x=center;
    int y=center;

    holdCircle[y][x] = 0;

    holdCircle[y-sliceSize][x] = 0;
/*
    for(int i=1; i<numVertex; i++)
    {
        ySlice = y-sin(sliceAngle*i);
        xSlice = x+cos(sliceAngle*i);
        holdCircle[ySlice][xSlice] = i;
    }*/


    for (int i = 0; i < numVertex; i++)
        free(holdCircle[i]);
 
    free(holdCircle);  
}

void printNumArray(int toBuild[], int numVertex)
{
    printf("\n");
    for(int i=0; i<numVertex; i++)
    {
        printf("%d\t", toBuild[i]);
    }
    printf("\n");
}

void buildString(int toBuild[], int numVertex)
{
    int i;
    for(i=0;i<numVertex;i++)
        toBuild[i] = i;
}

int factorial(int number)
{
    if(number == 0)
        return 1;

    int outNum = 1;

    for(int i=1; i<=number; i++)
        outNum = outNum * i;

    return outNum;
}

void bruteForce(int **holdVerticies, int **holdPermutations, int numVertex)
{

    if(holdPermutations[1][numVertex+1] < holdPermutations[0][numVertex+1])
    {
        for(int j=0; j<numVertex+2; j++)
            holdPermutations[0][j] = holdPermutations[1][j];
    }
}

void bruteAssign(int arr[], int size, int **holdPermutations, int **holdVerticies, int numVertex)
{
    int i,j;
    //printf("\nArr: ");
    for(i=1; i<size; i++)
    {
        holdPermutations[1][i] = arr[i];
        //printf("\t%d", arr[i]);
    }
    //printf("\n");

    holdPermutations[1][numVertex+1] = 0;

    for(int j=0; j<numVertex; j++)
    {
        holdPermutations[1][numVertex+1] = holdPermutations[1][numVertex+1] + holdVerticies[holdPermutations[2][j]][holdPermutations[2][j+1]];
        //printf("\n%d %d %d %d\n", holdPermutations[1][numVertex+1], holdVerticies[holdPermutations[2][j]][holdPermutations[2][j+1]], holdPermutations[2][j], holdPermutations[2][j+1]);
    }
/*
    for(int i=0; i<3; i++)
    {
        printf("\nArray %d: ", i);
        for(int j=0; j<numVertex+2; j++)
        {
            printf("\t%d", holdPermutations[i][j]);
        }
    }
*/
    bruteForce(holdVerticies, holdPermutations, numVertex);
}

//function to swap the variables
void swap(int *a, int *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

//permutation function
void permutation(int *arr, int start, int end, int **holdPermutations, int **holdVerticies, int numVertex)
{
    if(start == 1)
    {
        for(int i=0;i<numVertex+1;i++)
        {
            holdPermutations[0][i] = i;
            holdPermutations[1][i] = i;
        }
        holdPermutations[0][numVertex] = 0;
        holdPermutations[0][numVertex+1] = 9999;
        holdPermutations[1][numVertex] = 0;
        holdPermutations[1][numVertex+1] = 0;
        holdPermutations[2][0] = 0;
        holdPermutations[2][numVertex] = 0;
        holdPermutations[2][numVertex+1] = 0;
    }
    if(start==end)
    {
        bruteAssign(arr, end+1, holdPermutations, holdVerticies, numVertex);
        return;
    }
    int i;
    for(i=start;i<=end;i++)
    {
        swap((arr+i), (arr+start));
        permutation(arr, start+1, end, holdPermutations, holdVerticies, numVertex);
        swap((arr+i), (arr+start));
    }
}

void greedyAlg(int **holdVerticies, int **holdPermutations, int numVertex)
{
    /*
    holdPermutations[0][j] -- For Final
    holdPermutations[1][j] -- For Swap
    holdPermutations[2][j] -- For Test
    */
    //printf("Shortest Path:\n");

    for(int i=0; i<3; i++)
        for(int j=0; j<numVertex+2; j++)
        {
            holdPermutations[i][j] = 9999;
        }

    for(int i=0; i<numVertex; i++)
    {
        holdPermutations[0][i] = i;
    }

    int allCheck = numVertex;
    holdPermutations[0][numVertex] = 1;
    holdPermutations[2][numVertex+2] = 0;

    while(allCheck > 0)
    {
        for(int i=numVertex - allCheck; i<numVertex; i++)
        {
            if((holdPermutations[1][i] == 9999))
                for(int j=0; j<numVertex; j++)
                {
                    if(allCheck == 1)
                    {
                        if((holdVerticies[j][i] < holdPermutations[2][numVertex+1]) && (holdPermutations[1][j] != i) && (i!=j))
                        {
                            //printf("\nPlacing %d and %d\n", i, holdVerticies[j][i]);
                            holdPermutations[1][numVertex] = holdPermutations[0][i];
                            holdPermutations[0][numVertex+1] = holdPermutations[0][j];
                            holdPermutations[1][numVertex+1] = i;
                            holdPermutations[2][numVertex+1] = holdVerticies[j][i];
                            holdPermutations[2][numVertex] = holdPermutations[0][i];
                        }
                    }
                    else
                        if((holdVerticies[j][i] < holdPermutations[2][numVertex+1]) && (holdPermutations[0][j] != holdPermutations[0][i]) && (holdPermutations[1][j] != i) && (i!=j))
                        {
                            //printf("\nPlacing %d and %d\n", i, holdVerticies[j][i]);
                            holdPermutations[1][numVertex] = holdPermutations[0][i];
                            holdPermutations[0][numVertex+1] = holdPermutations[0][j];
                            holdPermutations[1][numVertex+1] = i;
                            holdPermutations[2][numVertex+1] = holdVerticies[j][i];
                            holdPermutations[2][numVertex] = holdPermutations[0][i];
                        }
                }
        }
        printf("\n%d->%d cost: %d", holdPermutations[1][numVertex+1], holdPermutations[0][numVertex+1], holdPermutations[2][numVertex+1]);
        holdPermutations[2][numVertex+2] = holdPermutations[2][numVertex+2] + holdPermutations[2][numVertex+1];
        holdPermutations[0][holdPermutations[0][numVertex+1]] = holdPermutations[1][numVertex];
        holdPermutations[1][holdPermutations[1][numVertex+1]] = holdPermutations[0][numVertex+1];
        holdPermutations[2][holdPermutations[1][numVertex+1]] = holdPermutations[2][numVertex+1];

        for(int k=0; k<numVertex; k++)
            if(holdPermutations[0][k] == holdPermutations[0][numVertex+1])
                holdPermutations[0][k] = holdPermutations[1][numVertex];

        holdPermutations[0][numVertex+1] = 9999;
        holdPermutations[1][numVertex+1] = 9999;
        holdPermutations[2][numVertex+1] = 9999;

        allCheck--;
    }

    printf("\n\nPath Cost: %d", holdPermutations[2][numVertex+2]);
}

struct Node
{
    int data;
    char id[1000];
    int pathTread;
    struct Node* next;
};
 
void deleteList(struct Node** head_ref)
{
   struct Node* current = *head_ref;
   struct Node* next;
 
   while (current != NULL)
   {
       next = current->next;
       free(current);
       current = next;
   }
   
   *head_ref = NULL;
}
 
void push(struct Node** head_ref, int new_data)
{
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
 
    new_node->data = new_data;
    strcpy(new_node->id, test);
    new_node->pathTread = 1;
    
    new_node->next = (*head_ref);
    
    (*head_ref) = new_node;
}

void printList(struct Node* n)
{
    while (n != NULL) {
        if(n->pathTread > 20)
        {
        printf("\nNext List: ");
        printf("id: %s ", n->id);
        printf("data: %d ", n->data);
        printf("tread: %d ", n->pathTread);
        }
        n = n->next;
    }
}

int search(struct Node* head, int pathValue)
{
    struct Node* current = head;  // Initialize current

    //printf("\nNew Compare===============");

    while (current != NULL)
    {
        /*
        printf("\nNode: %d", current);
        printf("\nid: %s ", current->id);
        printf("\ndata: %d ", current->data);
        printf("\ntread: %d ", current->pathTread);
*/
        //printf("\n\nComparing %s and %s", current->id, test);
        //printf("\nstrcmp val: %d", strcmp(current->id,test));

        if (strcmp(current->id,test) == 0)
        {
            //printf("\nFound Equal\n");
            current->pathTread++;
            return 1;
        }
        current = current->next;
    }

    return 0;
}

void pathWeight(int **holdVerticies, int **holdPermutations, int numVertex)
{
    float whereIsTheFood;
    float thisHouse;
    float crumbs[numVertex+1];
    int badHouse = 1;
    int nibble;
    int bite;
    int ant;

    holdPermutations[1][numVertex+1] = 0;
    int count = numVertex;

    printf("\nNew Sandwich");

    while(count > 0)
    {
        holdPermutations[0][numVertex] = 0;
        holdPermutations[0][numVertex+1] = 0;
        nibble = 0;
        bite = 0;
        ant = holdPermutations[1][numVertex+1];
        crumbs[numVertex] = 0;

        if(count > 1)
        {

            for(int i=0; i<numVertex; i++)
            {
                holdPermutations[0][i] = holdVerticies[0][i];
                crumbs[i] = 0;
            }
            
            //printTable(holdPermutations, 4, numVertex+2);

            //printf("\nCrumbs: ");
            //for(int i=0; i<numVertex+1; i++)
            //printf("%f\t", crumbs[i]);
            
            for(int i=0; i<numVertex; i++)
                if(i != ant)
                {
                    holdPermutations[0][numVertex] = holdPermutations[0][numVertex] + holdVerticies[0][i];
                }

            for(int i=0; i<numVertex; i++)
            {
                for(int j=0; j<numVertex; j++)
                    if((holdVerticies[0][i] != 0) && (holdPermutations[3][i] == 9999))
                    {
                        crumbs[i] = ((float)holdPermutations[0][numVertex] / (float)holdVerticies[0][i]);
                    }
                    else
                    {
                        crumbs[i] = 0;
                    }
                //printf("\nCrumbs: %f", crumbs[i]);
            }

            /*printf("\nCrumbs: ");
                for(int i=0; i<numVertex+1; i++)
                    printf("%f\t", crumbs[i]);*/

            for(int i=0; i<numVertex; i++)
            {
                if(crumbs[i] != 0)
                crumbs[numVertex] = crumbs[numVertex] + crumbs[i];
            }

                           /* printf("\nCrumbs: ");
                for(int i=0; i<numVertex+1; i++)
                    printf("%f\t", crumbs[i]);*/

            for(int i=0; i<numVertex; i++)
            {
                if(crumbs[i] != 0)
                    crumbs[i] = (crumbs[numVertex] / crumbs[i]);
                //printf("\nCrumbs: %f", crumbs[i]);
            }

                            /*printf("\nCrumbs: ");
                for(int i=0; i<numVertex+1; i++)
                    printf("%f\t", crumbs[i]);*/
            crumbs[numVertex] = 0;

            for(int i=0; i<numVertex; i++)
                crumbs[numVertex] = crumbs[numVertex] + crumbs[i];
            //printTable(holdPermutations, 4, numVertex+2);

                      /*                  printf("\nCrumbs: ");
                for(int i=0; i<numVertex+1; i++)
                    printf("%f\t", crumbs[i]);*/

            for(int i=0; i<numVertex; i++)
            {
                crumbs[i] = (crumbs[i] / crumbs[numVertex]) * 10000;
                //printf("\nCrumbs: %f", crumbs[i]);
            }

            for(int i=1; i<numVertex; i++)
            {
                crumbs[i] = crumbs[i] + crumbs[i-1];
                //printf("\nCrumbs: %f", crumbs[i]);
            }
            crumbs[numVertex] = crumbs[numVertex] * 10000;

            whereIsTheFood = rand() % 10000;
            //printf("\nwhereIsTheFood: %f\n", whereIsTheFood);

           /* printf("\nCrumbs: ");
                for(int i=0; i<numVertex+1; i++)
                    printf("%f\t", crumbs[i]);*/

            while(badHouse==1)
            {
                nibble++;
                bite = nibble % numVertex;
                thisHouse = crumbs[bite]; 

                //printf("\nAnt %d investigating Random House: %d\n", ant, bite);
                //printf("\nChecking If %f < %f\n", whereIsTheFood, crumbs[bite]);

                if(whereIsTheFood < crumbs[bite])
                {
                    if((bite!=ant) && (holdPermutations[1][bite] != ant) && (holdPermutations[2][bite] == 9999))
                        badHouse = 0;
                }
            }

            badHouse = 1;

            printf("\nAnt %d found a sandwich at Random House: %d at Distance: %d\n", ant, bite, holdVerticies[ant][bite]);

            holdPermutations[3][numVertex+1] = holdPermutations[3][numVertex+1] + holdVerticies[ant][bite];
            holdPermutations[1][ant] = bite;
            holdPermutations[2][bite] = ant;
            holdPermutations[3][bite] = holdVerticies[ant][bite];
            holdPermutations[1][numVertex+1] = bite;
        }
        else
        {
            printf("\nAnt %d found a sandwich at Random House: %d at Distance: %d\n", ant, 0, holdVerticies[bite][ant]);

            holdPermutations[3][numVertex+1] = holdPermutations[3][numVertex+1] + holdVerticies[ant][0];
            holdPermutations[1][ant] = 0;
            holdPermutations[2][0] = ant;
            holdPermutations[3][0] = holdVerticies[ant][0];
            holdPermutations[1][numVertex+1] = 0;
        }

        //printTable(holdPermutations, 4, numVertex+2);
        //printf("\nCount is at: %d", count);
        count--;
    }

    //printf("\nShortest Path:\n");

    int i = 0;
    while(count < numVertex)
    {
        //printf("\n%d->%d cost %d", i, holdPermutations[1][i], holdPermutations[3][holdPermutations[1][i]]);
        holdPermutations[0][count] = holdPermutations[1][i];
        i = holdPermutations[1][i];
        count++;
    }
    //printf("\n\nTotal Cost: %d", holdPermutations[3][numVertex+1]);

    //printTable(holdPermutations, 4, numVertex+2);
}

void catInt(int **holdPermutations, int numVertex)
{
    int converted = holdPermutations[0][0];
    //printf("\nconverted: %d", converted);

    char tmpShell[1000];
    char stringShell[1000];

    itoa(converted, stringShell, 10);

    //printf("\nUsing converted: %d, stringShell: %s", converted, stringShell);

    for(int i=1; i<numVertex; i++)
    {
        converted = holdPermutations[0][i];
        itoa(converted, tmpShell, 10);
        strcat(stringShell, tmpShell);
        //printf("\nUsing converted: %d, stringShell: %s, tmpShell: %s", converted, stringShell, tmpShell);
    }

    //printf("\nAssigning: %s", stringShell);

        listCounter++;

    strcpy(test, stringShell);
        //printf("\n\ntest: %s", test);
}

void antColony(int **holdVerticies, int **holdPermutations, int numVertex)
{
    int allCheck = numVertex*10;

    while(allCheck > 0)
    {
        for(int i=0; i<4; i++)
            for(int j=0; j<numVertex+2; j++)
            {
                holdPermutations[i][j] = 9999;
            }

        holdPermutations[3][numVertex+1] = 0;

        //printTable(holdPermutations, 4, numVertex+2);

        pathWeight(holdVerticies, holdPermutations, numVertex);

        catInt(holdPermutations, numVertex);

        int pathValue = holdPermutations[3][numVertex+1];
        
        //printf("\n\ntest: %s", test);
        //printf("\npathValue: %d", pathValue);

        if(search(head, pathValue) == 0)
        {
            //printf("\nNot found");
            push(&head, pathValue);
        }
 //push(&head, pathValue);

        allCheck--;
    }

    //printTable(holdPermutations, 4, numVertex+2);

    //printf("\nFinal Colony:\n");
}

int main(int argc, char **argv) // Number of Verticies, Generation Type, Variable Unit
{
    strcpy(test, "0000");
    push(&head, 100);

    clock_t start, end;
    double total = 0.0;

    int numVertex = atoi(argv[1]);
    int generationType = atoi(argv[2]);
    int variableUnit = atoi(argv[3]);
    int pathType = atoi(argv[4]);

    int permutationRows = 4;

    time_t seconds = time(NULL);
    srand(seconds);

    // Makes array for loading edges
    int** holdVerticies = (int**)malloc(numVertex * sizeof(int*));

    for (int i = 0; i < numVertex; i++)
        holdVerticies[i] = (int*)malloc(numVertex * sizeof(int));

    // Makes array for permutations

    int** holdPermutations = (int**)malloc(permutationRows * sizeof(int*));

    for (int i = 0; i < permutationRows; i++)
        holdPermutations[i] = (int*)malloc((numVertex+2) * sizeof(float));

    // Switch for generation types
    if(generationType == 0)
        generateRandomCostMatrix(holdVerticies, numVertex, variableUnit);
    else if(generationType == 1)
        generateRandomEuclideanMatrix(holdVerticies, holdPermutations, numVertex);
    else if(generationType == 2)
    {
        printf("\nRandom Euclidean Circle Path");
        generateRandomEuclideanCircle(holdVerticies, numVertex, 100);

        //generateRandomEuclideanCircle2(numVertex, 20);
    }
    else
    {
        printf("\nManual\n");
        holdVerticies[0][0] = 0;
        holdVerticies[0][1] = 1000;
        holdVerticies[0][2] = 10;
        holdVerticies[1][0] = 1000;
        holdVerticies[1][1] = 0;
        holdVerticies[1][2] = 20;
        holdVerticies[2][0] = 10;
        holdVerticies[2][1] = 20;
        holdVerticies[2][2] = 0;
    }
    printTable(holdVerticies, numVertex, numVertex);

    // Switch for shortest path
    if(pathType == 0)
    {
        for(int i=1;i<numVertex;i++)
            holdPermutations[2][i] = i;

                struct timeval start, end;
 
    gettimeofday(&start, NULL);
 
        permutation(holdPermutations[2], 1, numVertex-1, holdPermutations, holdVerticies, numVertex);

 
    gettimeofday(&end, NULL);
 
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
 

        printf("\nBrute Force\n");

        printf("\nShortest Path:\n");
        for(int i=0; i<numVertex; i++)
        {
            printf("\n%d->%d", holdPermutations[0][i], holdPermutations[0][i+1]);
            printf(" cost: %d", holdVerticies[holdPermutations[0][i]][holdPermutations[0][i+1]]);
        }
        printf("\n\nPath Cost: %d", holdPermutations[0][numVertex+1]);

    printf("\n\nThe elapsed time is %d microseconds\n", micros);

        
    }
    else if(pathType == 1)
    {

        printf("\nPrim's Greedy\n");

        struct timeval start, end;
 
        gettimeofday(&start, NULL);
        greedyAlg(holdVerticies, holdPermutations, numVertex);
        gettimeofday(&end, NULL);
 
        long seconds = (end.tv_sec - start.tv_sec);
        long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
 
        printf("\nThe elapsed time is %d microseconds\n", micros);
    }
    else if(pathType == 2)
    {
        printf("\nForgetful Ant Colony\n");

        struct timeval start, end;
 
        gettimeofday(&start, NULL);
        antColony(holdVerticies, holdPermutations, numVertex);
        gettimeofday(&end, NULL);
 
        long seconds = (end.tv_sec - start.tv_sec);
        long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
 
        printf("\nThe elapsed time is %d microseconds\n", micros);
    }
    else
        printf("\n\nBoop\n");


    //printTable(holdPermutations, 4, numVertex+2);

    //Test Fluff
    //printf("\n\n");
        printf("\n\n");
    //compareCircles(holdVerticies, holdPermutations, numVertex);

    //printTable(holdVerticies, numVertex, numVertex);

    //printf("\ncrash 1");

    //Test Fluff
    printList(head);

    deleteList(&head);

    for (int i = 0; i < numVertex; i++)
        free(holdVerticies[i]);
 
    free(holdVerticies);    
    
    //printf("\ncrash 2");

    for (int i = 0; i < permutationRows; i++)
        free(holdPermutations[i]);
 
    //printf("\ncrash 3");

    free(holdPermutations);

    printf("\n\nMade %d lists\n", listCounter);

    Sleep(500); 

    printf("\nSafe Exit\n");

    return 0;
}