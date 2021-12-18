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

void printTable(float **arr, int numVertex, int numElements)
{
    printf("\n\n");

    for (int i = 0; i < numVertex; i++)
    {
        for (int j = 0; j < numElements; j++)
            printf("%4.0f ", arr[i][j]);

        printf("\n\n");
    }
        printf("\n");
}

void generateRandomCostMatrix(float **arr, int numVertex, int maxCost)
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

void generateRandomEuclideanMatrix(float **arr, float **holdPermutations, int numVertex)
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

void generateRandomEuclideanCircle(float **arr, float **holdPermutations, int numVertex, int radius) // test numVertex = 10, Radius = 100
{
    ///

    double radianSlice = (PI / numVertex);
    int sliceCost = 0;
    double ySlice;
    double xSlice;

    printf("\nGenerate Random Euclidean Circle");

    //printf("\n\nRadian Slice: %f", radianSlice);

    // Start Series Generation=======================================================================

    int numSeries[numVertex];
    int count = 0;
    int nextNumber;

    for(int i=0; i<numVertex; i++)
        numSeries[i] = -1;
        
    numSeries[0];

    int assignNum = 0;

    while(count < numVertex)
    {
        nextNumber = (rand() % (numVertex-1)) + 1;

        //printf("\n\nnextNumber: %d", nextNumber);

        int gate = 0;
        int isPresent = 0;

        while(gate < numVertex)
        {
            //printf("\nChecking if: %d == %d", numSeries[gate], nextNumber);

            if(numSeries[gate] == nextNumber)
            {
                //printf("\n%d == %d", numSeries[gate], nextNumber);
                gate = numVertex;
                isPresent = 1;
            }
            else
            {
                gate++;
            }
        }

        if(isPresent == 0)
        {
            //printf("\nAssigning: numSeries[%d] == %d", assignNum, nextNumber);
            numSeries[assignNum] = nextNumber;
            //printf("\nnumSeries[%d]: %d", assignNum, numSeries[assignNum]);
            assignNum = numSeries[assignNum];
            count++;
        }

        if(count == numVertex - 1)
        {
            numSeries[assignNum] = 0;
            count++;
        }
    }

    int orderedSeries[numVertex];

    orderedSeries[0] = numSeries[0];

    nextNumber = numSeries[0];

    for(int i=1; i<numVertex; i++)
    {
        orderedSeries[i] = numSeries[nextNumber];
        nextNumber = orderedSeries[i];
    }

    for(int i=0; i< numVertex; i++)
    {
        holdPermutations[2][i+1] = orderedSeries[i];
    }

    holdPermutations[2][0] = 0;

    //printf("\n\nChecking Series\n");

    //for(int i=0; i<numVertex; i++)
        //printf("\nnumSeries[%d]: %d", i, numSeries[i]);

    // End Series Generation==========================================================================

    // Start Circle Test==============================================================================

    double slicePie = 2*PI / numVertex;

    for(int i=0; i<=numVertex; i++)
    {
        ySlice = radius * sin((slicePie)*i);
        xSlice = radius * cos((slicePie)*i);
        //printf("\nID: %d, ySlice: %f, xSlice: %f", i, ySlice, xSlice);
    }

    // End Circle Test================================================================================

    // Start Circle Generation========================================================================

    for(int i=0; i<=numVertex; i++)
    {
        holdPermutations[0][i] = radius * sin((slicePie)*i);
        //printf("\n %f == %f", holdPermutations[0][i], radius * sin((slicePie)*i));
        holdPermutations[1][i] = radius * cos((slicePie)*i);
        //printf("\n %f == %f", holdPermutations[1][i], radius * sin((slicePie)*i));
    }

    for(int i=0; i<numVertex; i++)
        numSeries[i+1] = orderedSeries[i];

    numSeries[0] = 0;

    for(int i=0; i<numVertex; i++)
        orderedSeries[numSeries[i]] = i;

    for(int i=0; i<numVertex; i++)
        for(int j=0; j<numVertex; j++)
            arr[i][j]=0;

    for(int i=0; i<numVertex; i++)
        for(int j=0; j<numVertex; j++)
        {
            arr[numSeries[i]][j] = sqrt(pow(holdPermutations[0][orderedSeries[j]] - holdPermutations[0][i], 2) + pow(holdPermutations[1][orderedSeries[j]] - holdPermutations[1][i], 2));
        }

    holdPermutations[2][numVertex+1] = 0;
    
    printf("\n\nShortest Path\n");
    for(int i=0; i<numVertex; i++)
    {
        int j = holdPermutations[2][i];
        int k = holdPermutations[2][i+1];
        printf("\nFrom: %0.f To: %0.f. Cost: %.0f", holdPermutations[2][i], holdPermutations[2][i+1], arr[j][k]);
        holdPermutations[2][numVertex+1] = holdPermutations[2][numVertex+1] + arr[j][k];
    }
    
    printf("\n\nTotal Cost: %.0f", holdPermutations[2][numVertex+1]);

    /*
    printf("\n");
    for(int i=0; i<numVertex; i++)
        printf("\norderedSeries[%d]: %d", i, orderedSeries[i]);
    
    printTable(holdPermutations, 4, numVertex+2);

    printf("\n\n");

        printTable(arr, numVertex, numVertex);
  */
    // End Circle Generation========================================================================
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

//**************************************************************************************************************************************
// Brute Force

//function to swap the variables
void swap(float *a, float *b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

//bruteForce function
void bruteForce(float *arr, int start, int end, float **holdPermutations, float **holdVerticies, int numVertex)
{
    if(start == 1)
    {
        for(int i=0; i< 4; i++)
            for(int j=0; j<numVertex+2; j++)
                holdPermutations[i][j] = j;

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
        holdPermutations[1][numVertex+1] = 0;

        for(int j=0; j<numVertex; j++)
        {
            int k = holdPermutations[2][j];
            int l = holdPermutations[2][j+1];
            holdPermutations[1][numVertex+1] = holdPermutations[1][numVertex+1] + holdVerticies[k][l];
            holdPermutations[1][j] = holdPermutations[2][j];
        }

        if(holdPermutations[1][numVertex+1] < holdPermutations[0][numVertex+1])
        {
            for(int j=0; j<numVertex+2; j++)
                holdPermutations[0][j] = holdPermutations[1][j];
        }
        return;
    }
    int i;
    for(i=start;i<=end;i++)
    {
        swap((arr+i), (arr+start));
        bruteForce(arr, start+1, end, holdPermutations, holdVerticies, numVertex);
        swap((arr+i), (arr+start));
    }
}

//**************************************************************************************************************************************

//**************************************************************************************************************************************
// Greedy (Prim's)

void greedyAlg(float **holdVerticies, float **holdPermutations, int numVertex)
{
    //printf("\n**************************************************************************************************************");

    for(int i=0; i<numVertex+2; i++)
    {
        holdPermutations[0][i] = 0;
        holdPermutations[1][i] = 0;
        holdPermutations[2][i] = 0;
        holdPermutations[3][i] = 9999;
    }

    //printf("\n\nFirst Tables");
    //printTable(holdVerticies, numVertex, numVertex);
    //printTable(holdPermutations, 4, numVertex+2);

    int remaining = numVertex;
    int nextNumber = 0;

    while(remaining > 0)
    {
        for(int i=0; i<numVertex; i++)
        {

            if(remaining != 1)
            {
                if((nextNumber != i) && (holdVerticies[nextNumber][i] < holdPermutations[3][numVertex]) && (holdPermutations[0][i] != 1))
                {
                    holdPermutations[0][numVertex] = i;
                    holdPermutations[1][numVertex] = nextNumber;
                    holdPermutations[2][numVertex] = i;
                    holdPermutations[3][numVertex] = holdVerticies[nextNumber][i];
                }
            }
            else
            {
                holdPermutations[0][numVertex] = i;
                holdPermutations[1][numVertex] = nextNumber;
                holdPermutations[2][numVertex] = 0;
                holdPermutations[3][numVertex] = holdVerticies[nextNumber][0];
            }
        }

        //int k = holdPermutations[0][numVertex];
        holdPermutations[0][nextNumber] = 1;
        holdPermutations[1][nextNumber] = holdPermutations[1][numVertex];
        holdPermutations[2][nextNumber] = holdPermutations[2][numVertex];
        holdPermutations[3][nextNumber] = holdPermutations[3][numVertex];
        //printf("\nStoring: %.0f", holdPermutations[3][numVertex]);

        holdPermutations[2][numVertex+1] = holdPermutations[2][numVertex+1] + holdPermutations[3][numVertex];

        holdPermutations[0][numVertex] = 0;
        holdPermutations[1][numVertex] = 0;
        holdPermutations[2][numVertex] = 0;
        holdPermutations[3][numVertex] = 9999;

        nextNumber = holdPermutations[2][nextNumber];
        remaining--;
    }

    //printf("\n\nFinal Table\n");
    //printTable(holdPermutations, 4, numVertex+2);
    //printf("\n**************************************************************************************************************");
}

//**************************************************************************************************************************************

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
    char bestID[1000];
    int bestData = 0;
    int bestPathTread = 9999;
    int totalAnts = 0;
    int totalData = 0;

    while (n != NULL) {
        totalAnts = totalAnts + n->pathTread;
        totalData = totalData + (n->data*n->pathTread);
        if(n->pathTread < bestPathTread)
        {
            bestData = n->data;
            bestPathTread = n->pathTread;
            strcpy(bestID, n->id);
        }
        n = n->next;
    }

    float averageData = totalData / totalAnts;

    printf("\nBest Path: %s at cost %d. Chosen by %d of %d ants! The average path cost was %.0f", bestID, bestData, bestPathTread, totalAnts, averageData);
}

int search(struct Node* head, int pathValue)
{
    struct Node* current = head;  // Initialize current

    //printf("\nNew Compare===============");

    while (current != NULL)
    {
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

void pathWeight(float **holdVerticies, float **holdPermutations, int numVertex)
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

    //printf("\nNew Sandwich");

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

            //printf("\nAnt %d found a sandwich at Random House: %d at Distance: %d\n", ant, bite, holdVerticies[ant][bite]);

            holdPermutations[3][numVertex+1] = holdPermutations[3][numVertex+1] + holdVerticies[ant][bite];
            holdPermutations[1][ant] = bite;
            holdPermutations[2][bite] = ant;
            holdPermutations[3][bite] = holdVerticies[ant][bite];
            holdPermutations[1][numVertex+1] = bite;
        }
        else
        {
            //printf("\nAnt %d found a sandwich at Random House: %d at Distance: %d\n", ant, 0, holdVerticies[bite][ant]);

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

void catInt(float **holdPermutations, int numVertex)
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
        strcat(stringShell, "->");
        strcat(stringShell, tmpShell);
        //printf("\nUsing converted: %d, stringShell: %s, tmpShell: %s", converted, stringShell, tmpShell);
    }

    //printf("\nAssigning: %s", stringShell);

        listCounter++;

    strcpy(test, stringShell);
        //printf("\n\ntest: %s", test);
}

void forgetfulAntColony(float **holdVerticies, float **holdPermutations, int numVertex)
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


//************************************************************************************************************************************************************




//************************************************************************************************************************************************************

void antColony(float **holdVerticies, float **holdPermutations, float **holdSmells, float **holdNewSmells, int ants, float smellFactor, float smellDecay, int requiredConsecutiveSteps, int numVertex)
{
    // holdPermutations[0][< numvertex] = path
    // holdPermutations[1][< numvertex] = visited
    // holdPermutations[2][< numvertex] = minPath

    //printTable(holdVerticies, numVertex, numVertex);
    //printTable(holdPermutations, 4, numVertex+2);


    int homeNode;
    int currentNode;
    int potNext;
    float totalAttraction;
    float takeSniff;
    float cumulProb;
    float edgeSelectionProb;
    float minPathCostSoFar;
    float previousPathCost;
    int attempts = 0;

    int consecutiveSteps = 0;

    for(int i=0; i<4; i++)
        for(int j=0; j<numVertex+2; j++)
            holdPermutations[i][j] = 0;

    //printTable(holdVerticies, numVertex, numVertex);



    //printf("\nCrash AC1");

    while(consecutiveSteps < requiredConsecutiveSteps)
    {
        for(int i=0; i<numVertex; i++)
        {
            for(int j=0; j<numVertex; j++)
            {
                holdSmells[i][j] = 0;
                holdNewSmells[i][j] = 0;
            }   
        }

        //printf("\nCrash AC2");


        for(int ant=0; ant<ants; ant++)
        {
            //printf("\nCrash AC3");

            holdPermutations[0][numVertex] = 0; // Path Cost
            homeNode = 0; // Home Node

            for(int j=0; j<numVertex; j++)
            {
                holdPermutations[1][j] = 0;
            }

            holdPermutations[1][0] = 1; // home visited

                //printf("\nAnt: %d", ant);


            for(int step=1; step<numVertex; step++)
            {

                currentNode = (int)holdPermutations[0][step-1];

                totalAttraction = 0;
                //printf("\nCrash AC5");

                //for(int i=0; i<numVertex; i++)
                        //printf("\nVisited: %d:%f", i, holdPermutations[1][i]);

                for(potNext=0; potNext<numVertex-1; potNext++)
                {
                    //printf("\nPotNext: %d", potNext);
                    //printf("\n\n%f == 0 ?\n%d != %d", holdPermutations[1][potNext], currentNode, potNext);

                        //printf("\n\nTest Gopher");
                    

                    if((holdPermutations[1][potNext] == 0) && (currentNode != potNext))
                    {
                        //printf("\n\nTotalAttraction: %f holdSmells: %f holdVerticies %f", totalAttraction, holdSmells[currentNode][potNext], holdVerticies[currentNode][potNext]);
                        totalAttraction = totalAttraction + ((1 + holdSmells[currentNode][potNext])  / holdVerticies[currentNode][potNext]);
                        //printf("\nCrash AC5.5");
                    }

                    //printf("\nCrash AC6");
                        
                }
                //printf("\nCrash AC7");

                int randomNumber = (rand() % 1001);

                //printf("\n\nrandomNumber: %d", randomNumber);

                takeSniff = (float)randomNumber / 1000;

                cumulProb = 0;

                        //printf("\n\ntakeSniff: %f cumulProb: %f edgeSelectionProb: %f\n\nVisited: ", takeSniff, cumulProb, edgeSelectionProb);

                //for(int i=0; i<numVertex; i++)
                    //printf(" %.0f", holdPermutations[1][i]);

                for(potNext=0; potNext<numVertex-1; potNext++)
                {
                     //hold
                    //printf("\n\nPotential Moving To: %d", potNext);

                    if((holdPermutations[1][potNext] == 0) && (currentNode != potNext))
                    {
                        edgeSelectionProb = ((1 + holdSmells[currentNode][potNext])  / holdVerticies[currentNode][potNext]) / totalAttraction;

                        cumulProb = cumulProb + edgeSelectionProb;

                        //printf("\n\nCheck me takeSniff: %f cumulProb: %f edgeSelectionProb: %f\n\n", takeSniff, cumulProb, edgeSelectionProb);

                        if(takeSniff < cumulProb)
                        {
                            break;
                        }
                    }
                }

                    //printf("\n\nMoving To: %d", potNext);


                holdPermutations[0][step] = potNext;


                holdPermutations[1][potNext] = 1;

                holdPermutations[0][numVertex] = holdPermutations[0][numVertex] + holdVerticies[currentNode][potNext];

                //printf("\nCrash AC7.5");


            }

                //printf("\nCrash AC8");
                //printf("\n\nholdVerticies[homeNode][potNext]: %f", holdVerticies[homeNode][potNext]);

            holdPermutations[0][numVertex] = holdPermutations[0][numVertex] + holdVerticies[homeNode][potNext];

            //printf("\n\nSANITY CHECK1 holdPermutations[0][numVertex]: %f", holdPermutations[0][numVertex]);

            //printf("\n\nAnt: %d", ant);

            if(ant == 0)
            {
            //printf("\n\nSANITY CHECK2 holdPermutations[0][numVertex]: %f", holdPermutations[0][numVertex]);

                //printf("\n\nSanity Check");
                minPathCostSoFar = holdPermutations[0][numVertex];

                //printf("\n\nminPathCostSoFar: %f", minPathCostSoFar);

                for(int i=0; i<numVertex; i++)
                    holdPermutations[2][i] = holdPermutations[0][i];
            }
            else if (holdPermutations[0][numVertex] < minPathCostSoFar)
            {
                minPathCostSoFar = holdPermutations[0][numVertex];

                for(int i=0; i<numVertex; i++)
                    holdPermutations[2][i] = holdPermutations[0][i];
            }

            for(int step=1; step<numVertex; step++)
            {
                homeNode = holdPermutations[0][step];
                potNext = holdPermutations[0][(step + 1) % numVertex];
                holdNewSmells[homeNode][potNext] = holdNewSmells[homeNode][potNext] + (smellFactor / holdPermutations[0][numVertex]);
            }

            //for(int i=1; i<numVertex; i++)
                //printf("\nPath: %.0f", holdPermutations[0][i]);

            //printf("\n================================================================================================================================");
        }

        for(int i=0; i<numVertex; i++)
        {
            for(int j=0; j<numVertex; j++)
            {
                holdSmells[i][j] = holdSmells[i][j]*smellDecay;

                holdSmells[i][j] = holdSmells[i][j] + holdNewSmells[i][j];
            }
        }

        //printf("\n\nPreviousPathCost: %f minPathCostSoFar: %f", previousPathCost, minPathCostSoFar);

        if(previousPathCost == minPathCostSoFar)
        {
            consecutiveSteps++;
        }
        else
        {
            consecutiveSteps = 0;
        }

        attempts++;

        previousPathCost = minPathCostSoFar;
        holdPermutations[2][numVertex+1] = previousPathCost;
    }
        //printf("\nAttempted Steps: %d",attempts);

}
//************************************************************************************************************************************************************





//************************************************************************************************************************************************************

//Main Program
int main(int argc, char **argv) // Generation Type, Variable Unit, Path Type
{
    time_t seconds = time(NULL);
    srand(seconds);

    int generationType = atoi(argv[1]);
    int pathType = atoi(argv[2]);
    int N_min = atoi(argv[3]);
    int N_max = atoi(argv[4]);

//************************************************************************************************************************************************************
    double trialsTime_max = .250; // in seconds
    long long int trialsCount_max = 10000,
                  n, trial;
    clock_t splitTimeStamp, trialSetStart;
    double splitTime, trialSetCount, trialSetTime, dummySetCount, dummySetTime, averageTrialTime, averageDummyTrialTime, estimatedTimePerTrial;
	
	// For each size N of input we want to test -- typically start N at 1 and double each repetition
    if(pathType == 0)
        for ( n=N_min; n<=N_max; n++ ) {

            Sleep(1000); 

            printf("\n\nBrute Force for N: %d\n", n);

            int permutationRows = 4;

                // Makes array for loading edges
            float** holdVerticies = (float**)malloc(n * sizeof(float*));

            for (int i = 0; i < n; i++)
                holdVerticies[i] = (float*)malloc(n * sizeof(float));

            // Makes array for permutations

            float** holdPermutations = (float**)malloc(permutationRows * sizeof(float*));

            for (int i = 0; i < permutationRows; i++)
                holdPermutations[i] = (float*)malloc((n+2)* sizeof(float));

            // Switch for generation types
            if(generationType == 0)
            {
                generateRandomCostMatrix(holdVerticies, n, 100);
                printTable(holdVerticies, n, n);
            }
            else if(generationType == 1)
            {
                generateRandomEuclideanMatrix(holdVerticies, holdPermutations, n);
                printTable(holdVerticies, n, n);
            }
            else if(generationType == 2)
                generateRandomEuclideanCircle(holdVerticies, holdPermutations, n, 100);

            printf("\n\nGenerate Brute Force");
            
            splitTime=0.0;
            // get timestamp before set of trials are run:
            trialSetStart = clock();
            // For each trial trialNumber=1 to Number of Trials per input size:
            for ( trial=0; trial < trialsCount_max && splitTime < trialsTime_max; trial++) {

                bruteForce(holdPermutations[2], 1, n-1, holdPermutations, holdVerticies, n);

                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            trialSetCount = trial; // value of trial when loop ends is how many we did
            trialSetTime = splitTime; // total time for trial set is the last split time
            averageTrialTime = trialSetTime / trialSetCount; // this is the average tiem per trial, including any prep/overhead

            splitTime=0.0;
            // get timestamp before set of dummy trials are run:
            trialSetStart = clock();
            for ( trial=0; trial < trialSetCount  && splitTime < trialsTime_max; trial++) {

                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            dummySetCount = trial; // value of trial when loop ends is how many we did, should be same as trialsSetCount
            dummySetTime = splitTime; // total time for dummy trial set is the last split time
            averageDummyTrialTime = dummySetTime / dummySetCount; // this is the average tiem per dummy trial, including any prep/overhead

            
            estimatedTimePerTrial = averageTrialTime - averageDummyTrialTime; // should be a decent measurement of time taken to run your algorithm
            
            //printTable(holdVerticies, n, n);
            //printTable(holdPermutations, 4, n+2);

            printf("\n\nShortest Path\n");
            for(int i=0; i<=n-1; i++)
            {
                int j = holdPermutations[0][i];
                int k = holdPermutations[0][i+1];
                printf("\nFrom: %.0f To: %.0f. Cost: %.0f", holdPermutations[0][i], holdPermutations[0][i+1], holdVerticies[j][k]);
            }

            printf("\n\nTotal Cost: %.0f", holdPermutations[0][n+1]);

            printf("\n\nEstimated Time: %lf\n", estimatedTimePerTrial);

            for (int i = 0; i < n; i++)
                free(holdVerticies[i]);
    
            free(holdVerticies);

            for (int i = 0; i < permutationRows; i++)
                free(holdPermutations[i]);

            free(holdPermutations);
        }

//************************************************************************************************************************************************************
// For each size N of input we want to test -- typically start N at 1 and double each repetition
    if(pathType == 1)
        for ( n=N_min; n<=N_max; n=n*2 ) {

            Sleep(1000); 

            printf("\n\nGreedy (Prim's) for N: %d\n", n);

            int permutationRows = 4;

                // Makes array for loading edges
            float** holdVerticies = (float**)malloc(n * sizeof(float*));

            for (int i = 0; i < n; i++)
                holdVerticies[i] = (float*)malloc(n * sizeof(float));

            // Makes array for permutations

            float** holdPermutations = (float**)malloc(permutationRows * sizeof(float*));

            for (int i = 0; i < permutationRows; i++)
                holdPermutations[i] = (float*)malloc((n+2)* sizeof(float));

            // Switch for generation types
            if(generationType == 0)
            {
                generateRandomCostMatrix(holdVerticies, n, 100);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 1)
            {
                printf("\n\nUsing Euclidean Matrix");
                generateRandomEuclideanMatrix(holdVerticies, holdPermutations, n);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 2)
                generateRandomEuclideanCircle(holdVerticies, holdPermutations, n, 100);

            printf("\n\nGenerate Greedy");
            
            splitTime=0.0;
            // get timestamp before set of trials are run:
            trialSetStart = clock();
            // For each trial trialNumber=1 to Number of Trials per input size:
            for ( trial=0; trial < trialsCount_max && splitTime < trialsTime_max; trial++) {

                greedyAlg(holdVerticies, holdPermutations, n);

                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            trialSetCount = trial; // value of trial when loop ends is how many we did
            trialSetTime = splitTime; // total time for trial set is the last split time
            averageTrialTime = trialSetTime / trialSetCount; // this is the average tiem per trial, including any prep/overhead

            
            splitTime=0.0;
            // get timestamp before set of dummy trials are run:
            trialSetStart = clock();
            for ( trial=0; trial < trialSetCount  && splitTime < trialsTime_max; trial++) {


                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            dummySetCount = trial; // value of trial when loop ends is how many we did, should be same as trialsSetCount
            dummySetTime = splitTime; // total time for dummy trial set is the last split time
            averageDummyTrialTime = dummySetTime / dummySetCount; // this is the average tiem per dummy trial, including any prep/overhead

            
            estimatedTimePerTrial = averageTrialTime - averageDummyTrialTime; // should be a decent measurement of time taken to run your algorithm
            
            //printTable(holdVerticies, n, n);
            //printTable(holdPermutations, 4, n+2);

            /*
            printf("\n\nShortest Path\n");

            int remaining = n;
            int nextNumber = 0;

            while(remaining > 0)
            {
                printf("\nFrom: %.0f To: %.0f. Cost: %.0f", holdPermutations[1][nextNumber], holdPermutations[2][nextNumber], holdPermutations[3][nextNumber]);

                nextNumber = holdPermutations[2][nextNumber];
                remaining--;
            }
            */
            printf("\n\nTotal Cost: %.0f", holdPermutations[2][n+1]);
            printf("\n\nEstimated Time: %lf\n", estimatedTimePerTrial);

            for (int i = 0; i < n; i++)
                free(holdVerticies[i]);
    
            free(holdVerticies);

            for (int i = 0; i < permutationRows; i++)
                free(holdPermutations[i]);

            free(holdPermutations);

        }


//************************************************************************************************************************************************************
// For each size N of input we want to test -- typically start N at 1 and double each repetition
for(int i=0; i<10; i++)
    if(pathType == 2)
        for ( n=N_min; n<=N_max; n++ ) {

            Sleep(1000); 

            printf("\n\nGreedy / Brute Force for N: %d\n", n);

            int permutationRows = 4;

                // Makes array for loading edges
            float** holdVerticies = (float**)malloc(n * sizeof(float*));

            for (int i = 0; i < n; i++)
                holdVerticies[i] = (float*)malloc(n * sizeof(float));

            // Makes array for permutations

            float** holdPermutations = (float**)malloc(permutationRows * sizeof(float*));

            for (int i = 0; i < permutationRows; i++)
                holdPermutations[i] = (float*)malloc((n+2)* sizeof(float));

            // Switch for generation types
            if(generationType == 0)
            {
                generateRandomCostMatrix(holdVerticies, n, 100);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 1)
            {
                //printf("\n\nUsing Euclidean Matrix");
                generateRandomEuclideanMatrix(holdVerticies, holdPermutations, n);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 2)
                generateRandomEuclideanCircle(holdVerticies, holdPermutations, n, 100);

        //**********************************************************************************************
            //printf("\n\nGenerate Brute Force");
            
            splitTime=0.0;
            // get timestamp before set of trials are run:
            trialSetStart = clock();
            // For each trial trialNumber=1 to Number of Trials per input size:
            for ( trial=0; trial < trialsCount_max && splitTime < trialsTime_max; trial++) {

                bruteForce(holdPermutations[2], 1, n-1, holdPermutations, holdVerticies, n);

                splitTimeStamp = clock(); // 
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            trialSetCount = trial; // value of trial when loop ends is how many we did
            trialSetTime = splitTime; // total time for trial set is the last split time
            averageTrialTime = trialSetTime / trialSetCount; // this is the average tiem per trial, including any prep/overhead

           
            splitTime=0.0;
            // get timestamp before set of dummy trials are run:
            trialSetStart = clock();
            for ( trial=0; trial < trialSetCount  && splitTime < trialsTime_max; trial++) {


                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            dummySetCount = trial; // value of trial when loop ends is how many we did, should be same as trialsSetCount
            dummySetTime = splitTime; // total time for dummy trial set is the last split time
            averageDummyTrialTime = dummySetTime / dummySetCount; // this is the average tiem per dummy trial, including any prep/overhead

            
            estimatedTimePerTrial = averageTrialTime - averageDummyTrialTime; // should be a decent measurement of time taken to run your algorithm
            
            //printTable(holdVerticies, n, n);
            //printTable(holdPermutations, 4, n+2);
/*
            printf("\n\nShortest Path\n");
            for(int i=0; i<=n-1; i++)
            {
                int j = holdPermutations[0][i];
                int k = holdPermutations[0][i+1];
                printf("\nFrom: %.0f To: %.0f. Cost: %.0f", holdPermutations[0][i], holdPermutations[0][i+1], holdVerticies[j][k]);
            }*/

            printf("\n\nTotal Cost: %.0f", holdPermutations[0][n+1]);

            //printf("\n\nEstimated Time: %lf\n", estimatedTimePerTrial);
        //**********************************************************************************************

        //**********************************************************************************************
            //printf("\n\nGenerate Greedy");
            
            splitTime=0.0;
            // get timestamp before set of trials are run:
            trialSetStart = clock();
            // For each trial trialNumber=1 to Number of Trials per input size:
            for ( trial=0; trial < trialsCount_max && splitTime < trialsTime_max; trial++) {


                greedyAlg(holdVerticies, holdPermutations, n);

                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            trialSetCount = trial; // value of trial when loop ends is how many we did
            trialSetTime = splitTime; // total time for trial set is the last split time
            averageTrialTime = trialSetTime / trialSetCount; // this is the average tiem per trial, including any prep/overhead

            
            splitTime=0.0;
            // get timestamp before set of dummy trials are run:
            trialSetStart = clock();
            for ( trial=0; trial < trialSetCount  && splitTime < trialsTime_max; trial++) {

                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            dummySetCount = trial; // value of trial when loop ends is how many we did, should be same as trialsSetCount
            dummySetTime = splitTime; // total time for dummy trial set is the last split time
            averageDummyTrialTime = dummySetTime / dummySetCount; // this is the average tiem per dummy trial, including any prep/overhead

            
            estimatedTimePerTrial = averageTrialTime - averageDummyTrialTime; // should be a decent measurement of time taken to run your algorithm
            
            //printTable(holdVerticies, n, n);
            //printTable(holdPermutations, 4, n+2);

            /*
            printf("\n\nShortest Path\n");

            int remaining = n;
            int nextNumber = 0;

            while(remaining > 0)
            {
                printf("\nFrom: %.0f To: %.0f. Cost: %.0f", holdPermutations[1][nextNumber], holdPermutations[2][nextNumber], holdPermutations[3][nextNumber]);

                nextNumber = holdPermutations[2][nextNumber];
                remaining--;
            }
            */
            printf("\n\nTotal Cost: %.0f", holdPermutations[2][n+1]);
            //printf("\n\nEstimated Time: %lf\n", estimatedTimePerTrial);
        //**********************************************************************************************

            for (int i = 0; i < n; i++)
                free(holdVerticies[i]);
    
            free(holdVerticies);

            for (int i = 0; i < permutationRows; i++)
                free(holdPermutations[i]);

            free(holdPermutations);

        }



//************************************************************************************************************************************************************
	// For each size N of input we want to test -- typically start N at 1 and double each repetition
    if(pathType == 4)
        for ( n=N_min; n<=N_max; n=n*2 ) {

            Sleep(1000); 

            printf("\n\nForgetful Ant for N: %d\n", n);

            int permutationRows = 4;

                // Makes array for loading edges
            float** holdVerticies = (float**)malloc(n * sizeof(float*));

            for (int i = 0; i < n; i++)
                holdVerticies[i] = (float*)malloc(n * sizeof(float));

            // Makes array for permutations

            float** holdPermutations = (float**)malloc(permutationRows * sizeof(float*));

            for (int i = 0; i < permutationRows; i++)
                holdPermutations[i] = (float*)malloc((n+2)* sizeof(float));

            // Switch for generation types
            if(generationType == 0)
            {
                generateRandomCostMatrix(holdVerticies, n, 100);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 1)
            {
                generateRandomEuclideanMatrix(holdVerticies, holdPermutations, n);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 2)
                generateRandomEuclideanCircle(holdVerticies, holdPermutations, n, 100);

            printf("\n\nGenerate Forgetful Ant");

            strcpy(test, "0000");
            push(&head, 100);
            
            splitTime=0.0;
            // get timestamp before set of trials are run:
            trialSetStart = clock();
            // For each trial trialNumber=1 to Number of Trials per input size:
            for ( trial=0; trial < trialsCount_max && splitTime < trialsTime_max; trial++) {


                forgetfulAntColony(holdVerticies, holdPermutations, n);

                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            trialSetCount = trial; // value of trial when loop ends is how many we did
            trialSetTime = splitTime; // total time for trial set is the last split time
            averageTrialTime = trialSetTime / trialSetCount; // this is the average tiem per trial, including any prep/overhead
            
            splitTime=0.0;
            // get timestamp before set of dummy trials are run:
            trialSetStart = clock();
            for ( trial=0; trial < trialSetCount  && splitTime < trialsTime_max; trial++) {

                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            dummySetCount = trial; // value of trial when loop ends is how many we did, should be same as trialsSetCount
            dummySetTime = splitTime; // total time for dummy trial set is the last split time
            averageDummyTrialTime = dummySetTime / dummySetCount; // this is the average tiem per dummy trial, including any prep/overhead

            
            estimatedTimePerTrial = averageTrialTime - averageDummyTrialTime; // should be a decent measurement of time taken to run your algorithm
            
            //printTable(holdVerticies, n, n);
            //printTable(holdPermutations, 4, n+2);
/*
            printf("\n\nShortest Path\n");
            for(int i=0; i<=n-1; i++)
            {
                int j = holdPermutations[0][i];
                int k = holdPermutations[0][i+1];
                printf("\nFrom: %.0f To: %.0f. Cost: %.0f", holdPermutations[0][i], holdPermutations[0][i+1], holdVerticies[j][k]);
            }

            printf("\n\nTotal Cost: %.0f", holdPermutations[0][n+1]);
*/
            printf("\n\nEstimated Time: %lf\n", estimatedTimePerTrial);

            for (int i = 0; i < n; i++)
                free(holdVerticies[i]);
    
            free(holdVerticies);

            for (int i = 0; i < permutationRows; i++)
                free(holdPermutations[i]);

            free(holdPermutations);

            printList(head);

            deleteList(&head);

            //printf("\n\nMade %d lists\n", listCounter);
        }

//************************************************************************************************************************************************************


//************************************************************************************************************************************************************
	// For each size N of input we want to test -- typically start N at 1 and double each repetition
    if(pathType == 5)
        for ( n=N_min; n<=N_max; n++ ) {

            Sleep(1000); 

            printf("\n\nForgetful Ant for N: %d\n", n);

            int permutationRows = 4;

                // Makes array for loading edges
            float** holdVerticies = (float**)malloc(n * sizeof(float*));

            for (int i = 0; i < n; i++)
                holdVerticies[i] = (float*)malloc(n * sizeof(float));

            // Makes array for permutations

            float** holdPermutations = (float**)malloc(permutationRows * sizeof(float*));

            for (int i = 0; i < permutationRows; i++)
                holdPermutations[i] = (float*)malloc((n+2)* sizeof(float));

            // Switch for generation types
            if(generationType == 0)
            {
                generateRandomCostMatrix(holdVerticies, n, 100);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 1)
            {
                generateRandomEuclideanMatrix(holdVerticies, holdPermutations, n);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 2)
                generateRandomEuclideanCircle(holdVerticies, holdPermutations, n, 100);


                        //**********************************************************************************************
            printf("\n\nGenerate Brute Force");
            
            splitTime=0.0;
            trialSetStart = clock();
            for ( trial=0; trial < trialsCount_max && splitTime < trialsTime_max; trial++) {

                bruteForce(holdPermutations[2], 1, n-1, holdPermutations, holdVerticies, n);

                splitTimeStamp = clock(); // 
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            trialSetCount = trial; // value of trial when loop ends is how many we did
            trialSetTime = splitTime; // total time for trial set is the last split time
            averageTrialTime = trialSetTime / trialSetCount; // this is the average tiem per trial, including any prep/overhead
            
            splitTime=0.0;
            // get timestamp before set of dummy trials are run:
            trialSetStart = clock();
            for ( trial=0; trial < trialSetCount  && splitTime < trialsTime_max; trial++) {

                splitTimeStamp = clock(); // 
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            dummySetCount = trial; // value of trial when loop ends is how many we did, should be same as trialsSetCount
            dummySetTime = splitTime; // total time for dummy trial set is the last split time
            averageDummyTrialTime = dummySetTime / dummySetCount; // this is the average tiem per dummy trial, including any prep/overhead

            
            estimatedTimePerTrial = averageTrialTime - averageDummyTrialTime; // should be a decent measurement of time taken to run your algorithm
            
            //printTable(holdVerticies, n, n);
            //printTable(holdPermutations, 4, n+2);
/*
            printf("\n\nShortest Path\n");
            for(int i=0; i<=n-1; i++)
            {
                int j = holdPermutations[0][i];
                int k = holdPermutations[0][i+1];
                printf("\nFrom: %.0f To: %.0f. Cost: %.0f", holdPermutations[0][i], holdPermutations[0][i+1], holdVerticies[j][k]);
            }*/

            printf("\n\nTotal Cost: %.0f", holdPermutations[0][n+1]);

            printf("\n\nEstimated Time: %lf\n", estimatedTimePerTrial);
        //**********************************************************************************************

            printf("\n\nGenerate Forgetful Ant");

            strcpy(test, "0000");
            push(&head, 100);
            
            splitTime=0.0;
            // get timestamp before set of trials are run:
            trialSetStart = clock();
            // For each trial trialNumber=1 to Number of Trials per input size:
            for ( trial=0; trial < trialsCount_max && splitTime < trialsTime_max; trial++) {

                forgetfulAntColony(holdVerticies, holdPermutations, n);

                splitTimeStamp = clock(); // 
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            trialSetCount = trial; // value of trial when loop ends is how many we did
            trialSetTime = splitTime; // total time for trial set is the last split time
            averageTrialTime = trialSetTime / trialSetCount; // this is the average tiem per trial, including any prep/overhead
            
            splitTime=0.0;
            // get timestamp before set of dummy trials are run:
            trialSetStart = clock();
            for ( trial=0; trial < trialSetCount  && splitTime < trialsTime_max; trial++) {
                splitTimeStamp = clock(); // 
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            dummySetCount = trial; // value of trial when loop ends is how many we did, should be same as trialsSetCount
            dummySetTime = splitTime; // total time for dummy trial set is the last split time
            averageDummyTrialTime = dummySetTime / dummySetCount; // this is the average tiem per dummy trial, including any prep/overhead

            
            estimatedTimePerTrial = averageTrialTime - averageDummyTrialTime; // should be a decent measurement of time taken to run your algorithm
            
            //printTable(holdVerticies, n, n);
            //printTable(holdPermutations, 4, n+2);
/*
            printf("\n\nShortest Path\n");
            for(int i=0; i<=n-1; i++)
            {
                int j = holdPermutations[0][i];
                int k = holdPermutations[0][i+1];
                printf("\nFrom: %.0f To: %.0f. Cost: %.0f", holdPermutations[0][i], holdPermutations[0][i+1], holdVerticies[j][k]);
            }

            printf("\n\nTotal Cost: %.0f", holdPermutations[0][n+1]);
*/
            printf("\n\nEstimated Time: %lf\n", estimatedTimePerTrial);

            for (int i = 0; i < n; i++)
                free(holdVerticies[i]);
    
            free(holdVerticies);

            for (int i = 0; i < permutationRows; i++)
                free(holdPermutations[i]);

            free(holdPermutations);

            printList(head);

            deleteList(&head);

            //printf("\n\nMade %d lists\n", listCounter);

        }

//************************************************************************************************************************************************************

//************************************************************************************************************************************************************
// For each size N of input we want to test -- typically start N at 1 and double each repetition
    if(pathType == 6)
        for ( n=N_min; n<=N_max; n++ ) {

            Sleep(1000); 

            int ants = 20;
            float smellFactor = 0.7; // 0.7
            float smellDecay = 0.9; //0.9
            int requiredConsecutiveSteps = 4;

            printf("\n\nAnt Colony for N: %d\n", n);

            int permutationRows = 4;

                // Makes array for loading edges
            float** holdVerticies = (float**)malloc(n * sizeof(float*));

            for (int i = 0; i < n; i++)
                holdVerticies[i] = (float*)malloc(n * sizeof(float));

            // Makes array for permutations

            float** holdPermutations = (float**)malloc(permutationRows * sizeof(float*));

            for (int i = 0; i < permutationRows; i++)
                holdPermutations[i] = (float*)malloc((n+2)* sizeof(float));


            // Makes array for pheromones
            float** holdSmells = (float**)malloc(n * sizeof(float*));

            for (int i = 0; i < n; i++)
                holdSmells[i] = (float*)malloc(n * sizeof(float));

            float** holdNewSmells = (float**)malloc(n * sizeof(float*));

            for (int i = 0; i < n; i++)
                holdNewSmells[i] = (float*)malloc(n * sizeof(float));

            // Switch for generation types
            if(generationType == 0)
            {
                generateRandomCostMatrix(holdVerticies, n, 100);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 1)
            {
                printf("\n\nUsing Euclidean Matrix");
                generateRandomEuclideanMatrix(holdVerticies, holdPermutations, n);
                //printTable(holdVerticies, n, n);
            }
            else if(generationType == 2)
                generateRandomEuclideanCircle(holdVerticies, holdPermutations, n, 100);

            printf("\n\nGenerate Ant Colony");
            
            splitTime=0.0;
            // get timestamp before set of trials are run:
            trialSetStart = clock();
            // For each trial trialNumber=1 to Number of Trials per input size:
            for ( trial=0; trial < trialsCount_max && splitTime < trialsTime_max; trial++) {

                antColony(holdVerticies, holdPermutations, holdSmells, holdNewSmells, ants, smellFactor, smellDecay, requiredConsecutiveSteps, n);


                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            trialSetCount = trial; // value of trial when loop ends is how many we did
            trialSetTime = splitTime; // total time for trial set is the last split time
            averageTrialTime = trialSetTime / trialSetCount; // this is the average tiem per trial, including any prep/overhead

            
            splitTime=0.0;
            // get timestamp before set of dummy trials are run:
            trialSetStart = clock();
            for ( trial=0; trial < trialSetCount  && splitTime < trialsTime_max; trial++) {


                splitTimeStamp = clock(); // 
                // split time is the difference between current split timestamp and the starting time stamp for trial set
                splitTime = (splitTimeStamp-trialSetStart) / (double)CLOCKS_PER_SEC; // CLOCK_PER_SEC define time.h 
            }
            dummySetCount = trial; // value of trial when loop ends is how many we did, should be same as trialsSetCount
            dummySetTime = splitTime; // total time for dummy trial set is the last split time
            averageDummyTrialTime = dummySetTime / dummySetCount; // this is the average tiem per dummy trial, including any prep/overhead

            
            estimatedTimePerTrial = averageTrialTime - averageDummyTrialTime; // should be a decent measurement of time taken to run your algorithm
            
            //printTable(holdVerticies, n, n);
            //printTable(holdPermutations, 4, n+2);
            //printTable(holdSmells, n, n);


            
            printf("\n\nShortest Path\n");
            for(int i=0; i<n; i++)
            {
                int y = holdPermutations[2][i];
                int x = holdPermutations[2][i+1];
                printf("\nFrom: %.0f To: %.0f. Cost: %.0f", holdPermutations[2][i], holdPermutations[2][i+1], holdVerticies[y][x]);
            }
            
            printf("\n\nTotal Cost: %.0f", holdPermutations[2][n+1]);
            
            printf("\n\nEstimated Time: %lf\n", estimatedTimePerTrial);

            for (int i = 0; i < n; i++)
                free(holdVerticies[i]);
    
            free(holdVerticies);

            for (int i = 0; i < permutationRows; i++)
                free(holdPermutations[i]);

            free(holdPermutations);

            for (int i = 0; i < n; i++)
                free(holdSmells[i]);

            free(holdSmells);

            for (int i = 0; i < n; i++)
                free(holdNewSmells[i]);

            free(holdNewSmells);

        }


//************************************************************************************************************************************************************

    Sleep(500); 

    printf("\nSafe Exit\n");

    return 0;
}