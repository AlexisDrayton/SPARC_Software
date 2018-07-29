#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#define ROWS 5
#define COLS 5
enum ORIENTATION {
    North = 0,
    NorthEast = 1,
    East = 2,
    SouthEast = 3,
    South = 4,
    SouthWest = 5,
    West = 6,
    NorthWest = 7
};
//user defined types
struct myNode {
    int value;
    int isInitalized;
    int xCoord;
    int yCoord;
};
struct path {
    struct myNode start;
    struct myNode end;
};
struct stackObj
{
    int value;
    struct stackObj* next;
};
struct stack
{
    struct stackObj* head;
    struct stackObj* foot;
};
//global variables
struct myNode myArray[ROWS][COLS];
struct path thePath;
int position[2] = { COLS / 2, ROWS / 2 };
struct stack fringe;
//function definintions
void arrayPrint(int a, int b, int mode); //mode 0 is normal print, mode 1 is printing init, mode 2 is printing algorithm
void arrayPrint_Borders();
void addObstacle(int blockedNumber);
void manualObstacles();
void menu();
void initFn();
void algorithm();
main() {
    int i, j;

    //myNode junk = NULL;

    for (j = 0; j < ROWS; j++)
    {
        for (i = 0; i < COLS; i++)
        {
            myArray[i][j].xCoord = i;
            myArray[i][j].yCoord = j;
        }
    }

    thePath.start = myArray[2][2];
    thePath.end = myArray[2][4];

    myArray[2][2].value = 1;
    myArray[2][3].value = 1;
    myArray[2][4].value = 1;

    myArray[2][2].isInitalized = 1;
    myArray[2][3].isInitalized = 1;
    myArray[2][4].isInitalized = 1;

    //addObstacle(3);

    menu();
}
void addObstacle(int blockedNumber)
{
    int i, randX, randY;

    srand((unsigned int)time(NULL));

    for (i = 0; i < blockedNumber;)
    {
        randX = rand() % ROWS;
        randY = rand() % COLS; //will these actually get two different values?  Time too close?

        if (myArray[randX][randY].value != -1) //change to not equal to blocked in final
        {
            myArray[randX][randY].value = -1; //this loops infinitely
            i++;
        }
    }
}
void arrayPrint(int a, int b, int mode)
{
    int i, j, temp, sPlace, count2;

    arrayPrint_Borders();
    for (j = COLS - 1; j >= 0; j--)
    {
        for (i = 0; i < ROWS; i++)
        {
            printf("|");

            //sparc location printing
            if (a != -1 && b != -1)
            {
                if (i == a && j == b)
                {

                    //Initalization Printing
                    if (mode == 1)
                    {
                        myArray[i][j].isInitalized = 1;
                        printf(" S ");
                    }
                    else { //mode two, and mode zero printing
                        printf(" s ");
                    }



                    continue;
                }
            }

            temp = myArray[i][j].value;
            sPlace = 0;

            while (temp / 10 > 0)
            {
                sPlace++;
                temp = temp / 10;
            }

            if (sPlace == 0)
            {
                if (myArray[i][j].value < 0)
                {
                    printf("%d", myArray[i][j].value);

                    if (myArray[i][j].isInitalized == 1)
                    {
                        printf("*");
                    }
                    else {
                        printf(" ");
                    }


                    continue;
                }
                else {
                    printf(" %d", myArray[i][j].value);

                    if (myArray[i][j].isInitalized == 1)
                    {
                        printf("*");
                    }
                    else {
                        printf(" ");
                    }

                    continue;
                }


            }

            if (myArray[i][j].value < 0)
            {
                sPlace = 2;
            }

            if (myArray[i][j].isInitalized == 1)
            {
                sPlace++;
            }

            for (count2 = sPlace; count2 < 2; count2++) //print spaces before
            {
                printf(" ");
            }

            printf("%d", myArray[i][j].value);

            if (myArray[i][j].isInitalized == 1)
            {
                printf("*");
            }

            for (count2 = sPlace + 1; count2 < 2; count2++) //print spaces after
            {
                printf(" ");
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
    for (i = 0; i < COLS; i++)
    {
        printf("___");
        printf("|");
    }
    printf("\n");
}
void menu()
{
    int input = 0;
    int input2 = 0;


    while (input != -1)
    {
        input = 0;

        printf("Enter a command:\n  1: Initalize Path\n 2: Add Random Obstacles\n   3: Add Obstacles Manually\n 4: Pathfinding Subroutine\n 5: Print\n  -1: Exit\n"); //1 initalize, 2 Random Obstacles, 3 Manual Obstacles, 4 Subroutine, 5 display 

        scanf_s("%d", &input);

        printf("%d", input);

        switch (input)
        {
        case 1:
            system("cls");
            initFn();
            system("cls");
            break;

        case 2:

            printf("Enter number of random obstacles to be added\n");

            fflush(stdout);

            scanf_s("%d", &input2);

            switch (input2)
            {
            case 0:
                break;

            default:
                addObstacle(input2);
            }
            system("cls");
            break;

        case 3:
            manualObstacles();
            system("cls");
            break;

        case 4:

            system("cls");
            algorithm();
            system("cls");
            break;

        case 5:

            system("cls");
            arrayPrint(-1, -1, 0);

            break;

        case -1:
            break;

        default:

            printf("Invalid input.");
            system("pause");
            system("cls");

            break;
        }
    }
}
void manualObstacles()
{
    int x, y;

    printf("Enter an X coordinate\n");
    scanf_s("%d", &x);
    printf("Enter an Y coordinate\n");
    scanf_s("%d", &y);
    system("cls");

    switch (myArray[x][y].value)
    {
    case -1:
        printf("Location is already blocked.\n");
        break;
    default:
        printf("Location %d,%d blocked.\n", x, y);
        myArray[x][y].value = -1;
    }
}
void initFn()
{
    int input = 0;
    int direction = 0;
    int initalizing = 0;

    while (input != -1)
    {

        //print here
        if (initalizing == 0)
        {
            arrayPrint(position[0], position[1], 0);
        }
        else {
            arrayPrint(position[0], position[1], 1);
        }


        printf("Current Direction is ");
        switch (direction)
        {
        case 0:
            printf("North");
            break;
        case 1:
            printf("NorthEast");
            break;
        case 2:
            printf("East");
            break;
        case 3:
            printf("SouthEast");
            break;
        case 4:
            printf("South");
            break;
        case 5:
            printf("SouthWest");
            break;
        case 6:
            printf("West");
            break;
        case 7:
            printf("NorthWest");
            break;
        default:
            break;
        }
        printf("\n");
        printf("Coordinates: (%d, %d)\n", position[0], position[1]);
        printf("~Controls~\n");
        printf("1: Rotate 45 degrees\n");
        printf("2: Move Forward one cell\n");
        printf("3: Rotate clockwise 45 degrees\n");
        printf("0: Toggle Intializing ");

        if (initalizing == 1)
            printf("(Currently Enabled)");

        printf("\n");

        printf("-1: Exit\n");

        scanf_s("%d", &input);

        system("cls");

        switch (input)
        {

        case 1:

            if (direction == 0)
            {
                direction = 7;

                break;
            }
            direction = (direction - 1);

            break;

        case 2:
            switch (direction)
            {
            case 0:
                if (position[1] == ROWS - 1) //south case
                    printf("Out of Bounds");
                else
                {
                    position[1] = position[1] + 1; //move north
                }
                break;

            case 1:
                if (position[1] == ROWS - 1 || position[0] == COLS - 1)
                    printf("Out of Bounds");
                else
                {
                    position[1] = position[1] + 1; //move north
                    position[0] = position[0] + 1; //move East
                }
                break;

            case 2:
                if (position[0] == COLS - 1)
                    printf("Out of Bounds");
                else
                {
                    position[0] = position[0] + 1; //move East
                }
                break;

            case 3:
                if (position[0] == COLS - 1 || position[1] == 0)
                    printf("Out of Bounds");
                else
                {
                    position[0] = position[0] + 1; //move East
                    position[1] = position[1] - 1; //move south
                }
                break;

            case 4:
                if (position[1] == 0)
                    printf("Out of Bounds");
                else
                {
                    position[1] = position[1] - 1; //move south
                }
                break;

            case 5:
                if ((position[1] == 0) || position[0] == 0)
                    printf("Out of Bounds");
                else
                {
                    position[1] = position[1] - 1; //move south
                    position[0] = position[0] - 1; //move West
                }
                break;

            case 6:
                if (position[0] == 0)
                    printf("Out of Bounds");
                else
                {
                    position[0] = position[0] - 1; //move West
                }
                break;

            case 7:
                if (position[0] == 0 || position[1] == ROWS - 1)
                    printf("Out of Bounds");
                else
                {
                    position[1] = position[1] + 1; //move north
                    position[0] = position[0] - 1; //move West
                }
                break;

            default:
                break;
            }

            myArray[position[0]][position[1]].value = 1;
            //we moved here

            break;

        case 3:
            direction = (direction + 1) % 8;

            break;

        case 0:
            if (initalizing == 0)
            {
                initalizing = 1;
                thePath.start = myArray[position[0]][position[1]];
                myArray[position[0]][position[1]].value = 1;
            }
            else {
                initalizing = 0;
                thePath.end = myArray[position[0]][position[1]];
            }

            break;
        }
    }

}
void algorithm()
{
    //if not in starting, cry about it;
    struct myNode location = thePath.start;
    int localX = thePath.start.xCoord;
    int localY = thePath.start.yCoord;
    int i;
    struct myNode* explored[100] = { NULL };
    int exploredIndex = 0;
    struct myNode maxVal;
    int blackSpot = -1, tempBS = -1;

    maxVal.value = 0; //Dangerous?

    arrayPrint(localX, localY, 2);
    Sleep(1500);
    system("cls");

    while (localX != thePath.end.xCoord || localY != thePath.end.yCoord)
    {
        //normal functionlity
        if (localX != 0) //West...
        {
            if (blackSpot != West)
            {
                maxVal = myArray[localX - 1][localY];
                tempBS = East;
            }

            if (localY != 0 && blackSpot != SouthWest) //SouthWest
            {
                if (maxVal.value < myArray[localX - 1][localY - 1].value)
                {
                    maxVal = myArray[localX - 1][localY - 1];
                    tempBS = NorthEast;
                }
            }

            if (localY != ROWS - 1 && blackSpot != NorthWest) //NorthWest
            {
                if (maxVal.value < myArray[localX - 1][localY + 1].value)
                {
                    maxVal = myArray[localX - 1][localY + 1];
                    tempBS = SouthEast;
                }
            }

        }

        if (localX != COLS - 1) //East...
        {
            if (maxVal.value < myArray[localX + 1][localY].value && blackSpot != East)
            {
                maxVal = myArray[localX + 1][localY];
                tempBS = West;
            }

            if (localY != 0) //SouthEast
            {
                if (maxVal.value < myArray[localX + 1][localY - 1].value && blackSpot != SouthEast)
                {
                    maxVal = myArray[localX + 1][localY - 1];
                    tempBS = NorthWest;
                }
            }

            if (localY != ROWS - 1) //NorthEast
            {
                if (maxVal.value < myArray[localX + 1][localY + 1].value && blackSpot != NorthEast)
                {
                    maxVal = myArray[localX + 1][localY + 1];
                    blackSpot = SouthWest;
                }
            }
        }

        if (localY != 0) //South
        {
            if (maxVal.value < myArray[localX][localY - 1].value && blackSpot != South)
            {
                maxVal = myArray[localX][localY - 1];
                tempBS = North;
            }
        }

        if (localY != ROWS - 1) //North
        {
            if (maxVal.value < myArray[localX][localY + 1].value && blackSpot != North)
            {
                maxVal = myArray[localX][localY + 1];
                tempBS = South;
            }
        }

        if (maxVal.value == 0)
        {
            printf("we stuck, my dude.");
            system("Pause");
        }

        //if this would be a collision
        {

            //backspot gets this instead

            //check for final case
            //if so exit, else do the stuff below
        }

        localX = maxVal.xCoord;
        localY = maxVal.yCoord;
        blackSpot = tempBS;

        arrayPrint(localX, localY, 2);
        Sleep(1500);
        system("cls");
    }
}
int manhattenDifference(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}
void frontier()
{
    short tragectory;
    int N, i;

    fringe.foot = NULL;
    fringe.head = NULL;

    //This logic block determines which way the SPARC wants to head in its initial exploration

    while (1) { //this needs to be not 1?

        if (position[0] < thePath.end.xCoord) //East If Statement
        {
            if (position[1] == thePath.end.yCoord) //East
            {
                N = East;
            }

            if (position[1] < thePath.end.yCoord) //North-East
            {
                N = NorthEast;
            }

            if (position[1] > thePath.end.yCoord) //South-East
            {
                N = SouthEast;
            }

        }
        else if (position[0] > thePath.end.xCoord) //West cases
        {
            if (position[1] == thePath.end.yCoord) //West
            {
                N = West;

            }

            if (position[1] < thePath.end.yCoord)//North-West
            {
                N = NorthWest;

            }

            if (position[1] > thePath.end.yCoord)//South-West
            {
                N = SouthWest;

            }
        } 
        else if (position[1] < thePath.end.yCoord)//North
        {
            N = North;
        }
        else if (position[1] > thePath.end.yCoord)//South
        {
            N = South;
        }

    }

    //stacker fn; orders so that most direct direction to dest is first
    push((N + 4) % 8);

    for (i = 3; i > 0; i--)
    {
        push(abs((N - i) % 8));
        push((N + i) % 8);
    }
    push(N);

    //check if pop is obstacle, if not then and move in that direction

    while (tragectory != N)
        rotate(45degrees);



    while (fringesize >= 1)
    {
        temp = pop();

        if (poppedValue == isTempBlocked)
            continue;

        move();

        if (moved) //if we don't encounter collision in this direction at X distance
        {
            clearStack;

            if (current location is already in graph)
                return;
            else
                break;
        }
    }


    //corner case; fringe emptied

    ~change this coord to blocked temporarily ? ~

    //if stack is all but one, then mark as obstacle.  If that one is an obstacle, then terminate all movement

    move(;

    if (IR_Voltage >= High)
        system("pause");
}
void push(int x)
{
    struct stackObj* newObject;

    newObject = calloc(1, sizeof(struct stackObj));

    newObject->next = fringe.head;
    newObject->value = x;
    fringe.head = &newObject;
}
struct stackObj pop()
{
    struct stackObj result = *fringe.head;
    struct stackObj *foo = fringe.head->next;

    fringe.head->next = NULL;
    fringe.head = foo;
    
    return result;
}
