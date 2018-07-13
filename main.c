#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef Node {
	int coords[2];
	bool inital;
	short value;
	//int averageTime;
	bool blocked;
	Node nodeNext;
	Node Branches
} newtype;
typedef branchTrunk{
    Node *branchPath
    Node *nextTrunk
} newtype;
typedef Path {
	Node[] nodePath; //make LL, not array
	int averageTime;
} newtype;
enum ORIENT {
    North = 0,
    NorthEast = 1,
    East = 2,
    SouthEast = 3,
    South = 4,
    SouthWest = 5,
    West = 6,
    NorthWest = 7
};
#define ROWS 5
#define COLS 5
Node* NodeCoordList = null;

int main()
{
short sparcOrient = 0;
Node currentPath = NULL;
Node currentNode = NULL;
int sparcGPSx = 0;
int sparcGPSy = 0;
int i, j;

for(i = 0; i < ARRAYSIZE; i++)
{

}

//setup

//make 20x20 array

while(currentNode != dest) //exit upon exceptions
{
	//move(cardinalDirection);

	if(didn't move)~~~
        collision

	if(collision)
	{
		checkAlternates~~~
		if currentNode.branching == null
            frontier
        else
			make it alternate~~~
			gotomove
	}
}
obstalceGenerator(Node **Array){


}
Path* setup(OriginNode){
	Path* newPath = {...}; //POINTING NEEDED DYNAMIC ALLOCATION

	char input;
	int newCoords[2] = {0,0};
	currentNode = ~~~~~~~~~~~~~

	do{
        //print array to console
        scanf(%c, &input);
        //enter to confirm, f to quit
        //some gotos

        switch(input){
            case w:
                newCoords[1] = currentNode.coords[1] + 1; //north
                break;
            case e:
                newCoords[0] = currentNode.coords[0] + 1; //northeast
                newCoords[1] = currentNode.coords[1] + 1;
                break;
            case d:
                newCoords[0] = currentNode.coords[0] + 1; //east
                break;
            case c:
                newCoords[0] = currentNode.coords[0] + 1; //southeast
                newCoords[1] = currentNode.coords[1] - 1;
                break;
            case x:
                newCoords[1] = currentNode.coords[1] - 1; //south
                break;
            case z:
                newCoords[0] = currentNode.coords[0] - 1; //southwest
                newCoords[1] = currentNode.coords[1] - 1;
                break;
            case a:
                newCoords[0] = currentNode.coords[0] - 1; //west
                break;
            case q:
                newCoords[0] = currentNode.coords[0] - 1; //northwest
                newCoords[1] = currentNode.coords[1] + 1;
                break;
            default:
                //some gotos
            currentNode.next = bigArray[newCoords[0]][newCoords[1]];
            currentNode = bigArray[newCoords[0]][newCoords[1]];
            currentNode.inital = true;
        }

	}while(input != 'f')

	destination = currentNode;



    return newPath;
}
fontierFn(currentNode, destination){//recursive?

	short tragectory;
	int N, i;

	//This logic block determines which way the SPARC wants to head in its initial exploration

	while(){

        if(currentNode.coords[0] < destination.coords[0]) //East If Statement
        {
            if(currentNode.coords[1] == destination.coords[1]) //East
            {
                N = East;
            }

            if(currentNode.coords[1] < destination.coords[1]) //North-East
            {
                N = Northeast;
            }

            if(currentNode.coords[1] > destination.coords[1]) //South-East
            {
                N = Southeast;
            }

        }else{ //western cases

            if(currentNode.coords[0] > destination.coords[0]) //West cases
            {
                if(currentNode.coords[1] == destination.coords[1]) //West
                {
                    N = West;

                }

                if(currentNode.coords[1] < destination.coords[1])//North-West
                {
                    N = Northwest;

                }

                if(currentNode.coords[1] > destination.coords[1])//South-West
                {
                    N = Southwest;

                }
            }

        }else{ //North and South

            if(currentNode.coords[1] < destination.coords[1])//North
            {
                N = North;
            } else {

                if(currentNode.coords[1] > destination.coords[1])//South
                   {
                       N = South;
                   }
            }

        }else{
            break; //we done
        }

        //stacker?
        push((N+4) % 8);

        for(i = 3; i > 0; i--)
        {
            push(abs((N-i)%8));
            push((N+i)%8);
        }
        push(N);

        //check if pop is obstacle, if not then and move in that direction
        //if stack is all but one, then mark as obstacle.  If that one is an obstacle, then terminate all movement

        if(currentPath)
        {

        }

    }
	if obstacle spotted in way~~~~~~
		fontierFn
		if no fontier
    }
}

void arrayPrint(Node **Array)
{
    int i, j

    arrayPrint_Borders();

    for(j = 0; j < COLS; j++)
    {
        for(i = 0; i < ROWS; i++)
        {
            printf("|");
            if(Array[i][j] == NULL) //if not substantied, print 0
            {
                printf("  ");
                continue;
            }

            if(Array[i][j].blocked = true) //if blocked, print an X
            {
                printf("X ");

            } else {
                printf("%d", Array[i][j].value); //otherwise print its value

                if(Array[i][j].inital == true)
                {
                    printf(".");
                } else {
                    printf(" ");
                }
            }
        }
        printf("|\n");
        arrayPrint_Borders();
    }
}
void arrayPrint_Borders()
{
    int i;

    printf("|");
    for(i = 0; i < COLS; i++)
    {
        printf("-");
        printf("|");
    }
    printf("\n");
}
