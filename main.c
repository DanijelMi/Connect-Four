#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>

#define MAIN_H
int backgroundColor = 15;   //  Sets the default background color to white
int textColor = 0;          //  Sets the default text color to black
char FILENAME[10] = {"Save.txt"};
typedef struct{
    char name[30];
    int color;
}player;
typedef struct{
    player P1;
    player P2;
}playersData;
typedef struct{
    int turnCounter;
    int startingPlayer;
    playersData PlayersData;
    int availableColumns[7];
    char grid[6][7];
}gameData;

playersData DPlayerData;
playersData *ptr_PlayerData = &DPlayerData;

//Custom SCANF THAT DOES ONLY ACCEPTS NUMBERS
float readInput(){
    float num;
    while (scanf("%f",&num) !=1){
        while((getchar()!='\n'));
        printf("Please enter a number\n");
    }
    return num;
}

//SAVES ALL THE GAME DATA INTO A FILE IN 1 LINE
int SaveGame(const char *Filename, gameData GameData){
    FILE *file;
    int i;
    file=fopen(Filename,"a"); // OPEN FILE IN APPEND MODE
    fprintf(file,"%d#",LargestID(FILENAME)+1);
    fprintf(file,"%s#%d#",GameData.PlayersData.P1.name,GameData.PlayersData.P1.color);  //  P1 Data
    fprintf(file,"%s#%d#",GameData.PlayersData.P2.name,GameData.PlayersData.P2.color);  //  P2 Data
    fprintf(file,"%d#%d#",GameData.startingPlayer, GameData.turnCounter);               //  Turn Data
    for(i=0;i<7;i++){fprintf(file,"%d",GameData.availableColumns[i]);}                  //  Grid array
    fprintf(file,"#");
    for(i=0;i<42;i++){fprintf(file,"%c", GameData.grid[i/7][i%7]);}                     //  availableColumns
    fprintf(file,"\n");
    fclose(file);
    return LargestID(FILENAME);
}

//RETURNS A GAMEDATA STRUCT FROM THE SPECIFIED LINE
gameData loadData(const char *Filename, int index){
    gameData GameData;
    FILE *file;
    int i,j;
    char line[100];
    file=fopen(Filename,"r");

    for(i=0;i<index;i++){
        fscanf(file, "%[^\n]", line);
        fgetc(file);
    }

    for(i=0;i<9;i++){
        fscanf(file, "%[^#\n]", line);
        fgetc(file);
        switch(i){
        case 1: strcpy(GameData.PlayersData.P1.name, line);     break;
        case 2: GameData.PlayersData.P1.color=atoi(line);       break;
        case 3: strcpy(GameData.PlayersData.P2.name, line);     break;
        case 4: GameData.PlayersData.P2.color=atoi(line);       break;
        case 5: GameData.startingPlayer=atoi(line);             break;
        case 6: GameData.turnCounter=atoi(line);                break;
        case 7:for(j=0;j<7;j++){GameData.availableColumns[j]=line[j]-'0';}  break;
        case 8:for(j=0;j<42;j++){GameData.grid[j/7][j%7]=line[j];}
        }
    }
    fclose(file);
    return GameData;
}

//PRINTS OUT ALL THE SAVED GAMES
void listSaves(const char *Filename){
    FILE *file;
    int i=0;
    gameData GameData;
    char line[100];
    ClearConsoleToColors(textColor,backgroundColor);
    file=fopen(Filename,"r");
    while ((fscanf(file, "%[^\n]", line)) != EOF ){
        fscanf(file, "%[^#\n]", line);
        fgetc(file);
        printf("ID:%d ",atoi(line));
        GameData=loadData(Filename,i);
        SetColor(GameData.PlayersData.P1.color);    printf("%s",GameData.PlayersData.P1.name);
        SetColor(textColor);     printf(" vs ");
        SetColor(GameData.PlayersData.P2.color);    printf("%s",GameData.PlayersData.P2.name);
        SetColor(textColor);     printf("      Empty fields: %d\n",42-GameData.turnCounter);
        i+=1;
    }
    fclose(file);
    fclose(file);
    int selection;
    int menu=1;
    int loop=1;
    while(menu){
        printf("\nSelect the save game by it's ID.\n");
        printf("0 - Back   \n  \nSelect an ID: ");
        do{
            selection=readInput();
            if(FindSaveByID(FILENAME,selection)==-1 && selection !=0){  //Does not break the loop unless there is a valid ID or 0 is pressed
                printf("ID does not exist. Try again:");
            }else{loop=0;}
        }while(loop);
        ClearConsoleToColors(textColor,backgroundColor);
        if(selection==0){menu=0;}
        else{
            GameData=loadData(FILENAME,FindSaveByID(FILENAME,selection));
            drawGrid(GameData);
            loadMenu2(selection,0);
            menu=0;
            }
    }
}

void listSavesByName(const char *Filename){
    FILE *file;
    int i=0;
    gameData GameData;
    char line[100];
    ClearConsoleToColors(textColor,backgroundColor);
    char playerName[30];
    printf("Enter Name: ");
    scanf("%s",&playerName);
    file=fopen(Filename,"r");
    while ((fscanf(file, "%[^\n]", line)) != EOF ){
        fscanf(file, "%[^#\n]", line);
        fgetc(file);
        GameData=loadData(Filename,i);
        if(strcmp(GameData.PlayersData.P1.name,playerName)==0 || strcmp(GameData.PlayersData.P2.name,playerName)==0){   //CHECKS IF EITHER NAMES IN EACH ITERATION MATCHES THE INPUT NAME
            printf("ID:%d ",atoi(line));
            SetColor(GameData.PlayersData.P1.color);    printf("%s",GameData.PlayersData.P1.name);
            SetColor(textColor);     printf(" vs ");
            SetColor(GameData.PlayersData.P2.color);    printf("%s",GameData.PlayersData.P2.name);
            SetColor(textColor);     printf("      Empty fields: %d\n",42-GameData.turnCounter);
        }
        i+=1;
    }
    fclose(file);
    int selection;
    int menu=1;
    int loop=1;
    while(menu){
        printf("\nSelect the save game by it's ID.\n");
        printf("0 - Back   \n  \nSelect an ID: ");
        do{
            selection=readInput();
            if(FindSaveByID(FILENAME,selection)==-1 && selection !=0){  //Does not break the loop unless there is a valid ID or 0 is pressed
                printf("ID does not exist. Try again:");
            }else{loop=0;}
        }while(loop);
        ClearConsoleToColors(textColor,backgroundColor);
        if(selection==0){menu=0;}
        else{
            GameData=loadData(FILENAME,FindSaveByID(FILENAME,selection));
            drawGrid(GameData);
            loadMenu2(selection,1);
            menu=0;
            }
    }
}

//  FINDS THE LARGEST ID IN THE SAVE FILE. USED FOR CONFLICT PREVENTION
int LargestID(const char* Filename){
    FILE *file;
    file=fopen(Filename,"r");
    char line[100];
    int i=0;
    int largestID=0;
    int largestIndex=0;
    while ((fscanf(file, "%[^\n]", line)) != EOF ){
            fscanf(file, "%[^#\n]", line);
            fgetc(file);
            if(atoi(line)>largestID){
                largestID=atoi(line);
                largestIndex=i;
            }
            fscanf(file, "%[^\n]", line);
    }
    fclose(file);
    return largestID;
}

//RETURNS THE LINE INDEX WHERE THE INPUT ID SAVE FILE IS FOUND
int FindSaveByID(const char *Filename, int ID){
    FILE *file;
    file=fopen(Filename,"r");
    char line[100];
    int i=0;
    int requestedIndex=-1; //IF NO SAVE IS FOUND THEN THIS IS WHAT GETS RETURNED
    while((fscanf(file, "%[^\n]", line)) != EOF ){
            fscanf(file, "%[^#\n]", line);
            fgetc(file);
            if(atoi(line)==ID){
                requestedIndex=i;
            }
        i+=1;
    }
    fclose(file);
    return requestedIndex;
}

//RETURNS THE TOTAL AMOUNT OF LINES IN THE FILE
int getFileLength(const char *Filename){
    FILE *file;
    char line[100];
    file=fopen(Filename,"r");
    int amount=0;
    while((fscanf(file, "%[^\n]", line)) != EOF ){
            amount++;
            fgetc(file);
        }
    fclose(file);
    rewind(file);
    return amount;
}

//REMOVES A SAVE FROM THE FILE BY INDEX. (TARGETFILE,BUFFERFILE,INDEX)
void SaveRemove(const char *Filename, const char *TempFile, int index){
    FILE *file1, *file2;
    char line[100];
    int j,i = 0;
    for(j=0;j<2;j++){
        if(j==0){
            file1 = fopen(Filename,"r");
            file2 = fopen(TempFile,"w");
        }else{
            file1 = fopen(TempFile,"r");
            file2 = fopen(Filename,"w");
        }
        while ((fscanf(file1, "%[^\n]", line)) != EOF ){
                if(index != i){
                    fprintf(file2,"%s\n", line);
                }
                fgetc(file1);
                i++;
        }
        fclose(file1);
        fclose(file2);
        index=-1;
    }
    //remove(TempFile);
}

//A function found online for setting text color
void SetColor(int ForgC){
     WORD wColor;

      HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO csbi;

                       //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
                 //Mask out all but the background attribute, and add in the forgournd     color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
 }

//A function found online for clearing the console
void ClearConsoleToColors(int ForgC, int BackC){
 WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
               //Get the handle to the current output buffer...
 HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
                     //This is used to reset the carat/cursor to the top left.
 COORD coord = {0, 0};
                  //A return value... indicating how many chars were written
                    //   not used but we need to capture this since it will be
                      //   written anyway (passing NULL causes an access violation).
  DWORD count;

                               //This is a structure containing all of the console info
                      // it is used here to find the size of the console.
 CONSOLE_SCREEN_BUFFER_INFO csbi;
                 //Here we will set the current color
 SetConsoleTextAttribute(hStdOut, wColor);
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
                          //This fills the buffer with a given character (in this case 32=space).
      FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

      FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
                          //This will set our cursor position for the next print statement.
      SetConsoleCursorPosition(hStdOut, coord);
 }
 return;
}

//SMALL DIALOG FOR EXITING THE PROGRAM
int promptExit(){
    int selection;
    do{
        ClearConsoleToColors(textColor,backgroundColor);
        printf("Are you sure you wish to exit?\n");
        printf("Yes - 1\nNo - 0\n");
        printf("Answer: ");
        selection=readInput();
    }while(selection<0 || selection>1);
    return selection;
}

//SMALL DIALOG FOR RE-PLAYING THE GAME
void promptPlayAgain(){
    int selection;
    do{
            printf("Play Again?\n");
            printf("Yes - 1\n");
            printf("No - 0\n");
            selection=readInput();
        }while(selection<0 || selection>1);
        switch(selection){
        case 0:
            main();
        case 1:
            gameController(1,0);
            break;
        }
}

//SMALL DIALOG FOR CHOOSING WHO STARTS FIRST
int promptStartFirst(playersData PData){
    int selection;
    do{                         //Loop handles user input on who starts first
        ClearConsoleToColors(textColor,backgroundColor);
        printf("Who starts first?\n");
        SetColor(PData.P1.color);     printf("1 - %s\n", PData.P1.name);
        SetColor(PData.P2.color);     printf("2 - %s\n", PData.P2.name);
        SetColor(textColor);          printf("3 - Random\n");
        printf("Selection: ");
        selection=readInput();
    }while(selection<1 || selection>3);
    if(selection==3){           //Translates user input to starting turn advantage
        srand (time(NULL));
        selection = (rand()%2) + 1;
    }
    return selection;
}

//ASKS AND GATHERS BASIC INFO FROM THE 2 PLAYERS
playersData GatherPlayerInfo(){
    playersData playersdata;
    int colorCodes[7]={5,6,9,10,11,12,13};  //Translates indexed selection to proper color code
    int selection;
    //GET NAME OF Player1
    printf("Player 1 name: ");
    scanf("%s",&playersdata.P1.name);
    //Get color of Player1
    do{
        ClearConsoleToColors(textColor,backgroundColor);
        SetColor(colorCodes[0]);    printf("Magenta-1 ");
        SetColor(colorCodes[1]);    printf("Brown-2 ");
        SetColor(colorCodes[2]);    printf("Blue-3 ");
        SetColor(colorCodes[3]);    printf("Green-4 ");
        SetColor(colorCodes[4]);    printf("Cyan-5 ");
        SetColor(colorCodes[5]);    printf("Red-6 ");
        SetColor(colorCodes[6]);    printf("Light Magenta-7\n");
        SetColor(textColor);
        printf("Choose your color, %s: ",playersdata.P1.name);
        selection=readInput();
    }while(selection < 1 || selection > 7);
    playersdata.P1.color=colorCodes[selection-1];

    //Get name of Player 2, make sure names don't match
    ClearConsoleToColors(textColor,backgroundColor);
    do{
        printf("Name cannot be the same\n");
        printf("Player 2 name: ");
        scanf("%s",&playersdata.P2.name);
        ClearConsoleToColors(textColor,backgroundColor);
    }while(!strcmp(playersdata.P1.name, playersdata.P2.name)); // Breaks when the two names are not equal
    //Get the color of Player 2, make sure colors don't match
    do{
        printf("Color cannot be the same\n");
        SetColor(colorCodes[0]);    printf("Magenta-1 ");
        SetColor(colorCodes[1]);    printf("Brown-2 ");
        SetColor(colorCodes[2]);    printf("Blue-3 ");
        SetColor(colorCodes[3]);    printf("Green-4 ");
        SetColor(colorCodes[4]);    printf("Cyan-5 ");
        SetColor(colorCodes[5]);    printf("Red-6 ");
        SetColor(colorCodes[6]);    printf("Light Magenta-7\n");
        SetColor(textColor);
        printf("Choose your color, %s: ",playersdata.P2.name);
        selection=readInput();
        playersdata.P2.color=colorCodes[selection-1];
        ClearConsoleToColors(textColor,backgroundColor);
    }while(playersdata.P1.color==playersdata.P2.color || selection < 1 || selection > 7);

    return playersdata;
}

//UNNECESARY COSMETIC EFFECT, MAKES THE PLACED SYMBOL FALL DOWN TO ITS PLACE
void animatePlace(int selection,char symbol, gameData GameData){
    int i;
    for(i=0;i<availableDot(GameData.grid,selection-1);i++){  //This animates the placed dot towards its position
        ClearConsoleToColors(textColor,backgroundColor);
        GameData.grid[i][selection-1]=symbol;
        printf(" 0-Save Game  8-Exit\n");
        drawGrid(GameData);   //Sets symbol (only used for animation)
        Sleep(30);
        GameData.grid[i][selection-1]=' ';  //Removes the trailing symbol
    }
}

//DRAWS THE GRID
void drawGrid(gameData GameData){
    int i,j;
    printf(" _____________ \n");
    for(i=0;i<6;i++){       //DRAWS THE TABLE FROM THE 2D ARRAY grid
        printf("|");
        for(j=0;j<7;j++){
            if (!GameData.grid[i][j]){
                GameData.grid[i][j]=' ';
            }else if(GameData.grid[i][j]=='X'){
                SetColor(GameData.PlayersData.P1.color);
            }else if(GameData.grid[i][j]=='O'){
                SetColor(GameData.PlayersData.P2.color);
            }
            printf("%c",GameData.grid[i][j]);
            SetColor(textColor);
            printf("|",GameData.grid[i][j]);
        }
        printf("\n");
    }
    for(i=0;i<7;i++){       //PRINTS THE AVAILABLE COLUMNS BELOW THE TABLE
        if(GameData.availableColumns[i]){
            printf(" %d",GameData.availableColumns[i]);
        }else{
            printf("  ");
        }
    }
    printf("\n");
}

//CHECKS IF THERE ARE ANY 4 CONSECUTIVE ELEMENTS
int checkResult(char grid[6][7]){
    int row,column,i,count;

    //checks horizontal
    for(row=0;row<6;row++){
        for(column=0;column<4;column++){
            count = 0;
            for(i=0;i<3;i++){
                if(!(grid[row][column+i]==' ' || grid[row][column+i]=='_') && grid[row][column+i]==grid[row][column+i+1]){
                    count+=1;
                    if(count==3){
                        return 100+(row*10)+column;
                    }
                }
            }
        }
    }
    //checks vertical
    for(row=0;row<3;row++){
        for(column=0;column<7;column++){
            count = 0;
            for(i=0;i<3;i++){
                if(!(grid[row+i][column]==' ' || grid[row+i][column]=='_') && grid[row+i][column]==grid[row+i+1][column]){
                    count+=1;
                    if(count==3){
                        return 200+(row*10)+column;
                    }
                }
            }
        }
    }
    //checks diagonal \ direction
    for(row=0;row<3;row++){
        for(column=0;column<4;column++){
            count = 0;
            for(i=0;i<3;i++){
                if(!(grid[row+i][column+i]==' ' || grid[row+i][column+i]=='_') && grid[row+i][column+i]==grid[row+i+1][column+i+1]){
                    count+=1;
                    if(count==3){
                        return 300+(row*10)+column;
                    }
                }
            }
        }
    }
    //checks diagonal /
    for(row=0;row<3;row++){
        for(column=0;column<4;column++){
            count = 0;
            for(i=0;i<3;i++){
                if(!(grid[row+3-i][column+i]==' ' || grid[row+3-i][column+i]=='_') && grid[row+3-i][column+i]==grid[row+3-i-1][column+i+1]){
                    count+=1;
                    if(count==3){
                        return 400+(row*10)+column;
                    }
                }
            }
        }
    }
    return 0;
}

//Returns the first available row (INT) in the specified column
int availableDot(char grid[6][7], int column){
    int i,available=-1; //  if unmodified, is returned as -1 to be interpreted as "no results found"
    for(i=6;i>=0;i--){
        if(grid[i][column]==' ' || grid[i][column]=='_'){
            available=i;    //returns the first empty or _ field, starting from the bottom up
            i=0;            //Exits the for loop prematurely
        }
    }
    return available;
}

//Main FN handling the entire game
void gameController(int isRerun, int loadIndex){   //  If "isRerun" is == 1 then it starts the game with PlayersData imported
    int i;  //Used for iteration
    int selection=0;    /*For Choice Making*/
    int checkRes;   //      Win condition check results are stored here
    char symbol;    //      Used for marking X or O
    int saveID=0;
    int gameRunning=1;  //  Breaks the gameplay loop when set to 0
    gameData GameData;
    GameData.turnCounter=0;
    for(i=0;i<7;i++){GameData.availableColumns[i]=i+1;} //  Sets the array: {1,2,3,4,5,6,7}: Defines the starting available columns
    for(i=0;i<42;i++){GameData.grid[i/7][i%7]=' ';}
    for(i=0;i<7;i++){   //  Sets the last line of the grid to '_', (for a better looking table)
        GameData.grid[5][i]='_';
    }

    if(isRerun==1){GameData.PlayersData=*ptr_PlayerData;}  //  If the game is being re-run from the "play again" prompt, then load in the Player Data
    else if(isRerun==2){
        GameData=loadData(FILENAME,loadIndex);
        *ptr_PlayerData = GameData.PlayersData;
    }else{
        GameData.PlayersData = GatherPlayerInfo();       //Gets players' names and color preferences if the game is run from the main menu.
        *ptr_PlayerData = GameData.PlayersData;
    }
    if(isRerun!=2){GameData.startingPlayer=promptStartFirst(GameData.PlayersData);}   //  Handles who will start first IF it is not a loaded game
    ClearConsoleToColors(textColor,backgroundColor);
    drawGrid(GameData);
    //START THE ACTUAL GAME
    while(gameRunning){
        do{
            //  Draws the grid if a dot wasn't succesfuly placed
            if(selection==0 || GameData.availableColumns[selection-1]==0 || selection<0 || selection>7){
                    ClearConsoleToColors(textColor,backgroundColor);
                    printf(" 0-Save Game  8-Exit\n");
                    drawGrid(GameData);
            }if(saveID){printf("The game has been saved! Game ID: %d\n",saveID);
                saveID=0;
            }
            //HANDLES THE SEQUENTIAL "MAKE A TURN" PROMPT
            if(((GameData.turnCounter+(GameData.startingPlayer-1))  %2)==0){
                SetColor(GameData.PlayersData.P1.color);    printf("%s's Turn (X): ",GameData.PlayersData.P1.name);
                SetColor(textColor);
                symbol='X';
            }else if(((GameData.turnCounter+(GameData.startingPlayer-1))  %2)==1){
                SetColor(GameData.PlayersData.P2.color);    printf("%s's Turn (O): ",GameData.PlayersData.P2.name);
                SetColor(textColor);
                symbol='O';
            }
            selection=readInput();
            if(selection==0){saveID=SaveGame(FILENAME,GameData);}  //SAVES THE GAME IF 0 IS PRESSED
            if(selection==8){//PROMPTS TO EXIT THE GAME (TO MAIN MENU) IF 8 IS PRESSED
                if(promptExit()){
                    main();
                }
            }
        }while(GameData.availableColumns[selection-1]==0 || selection<1 || selection>7); // Checks if the selection is out of bounds or selecting an unavailable index

        animatePlace(selection,symbol,GameData);   //Animated unnecessary effect
        ClearConsoleToColors(textColor,backgroundColor);
        GameData.grid[availableDot(GameData.grid,selection-1)][selection-1]=symbol;   //Sets the final X or O in the 2d array grid
        //Checks if the placement is the last one possible in the column. If so, remove that index from being available.
        if(availableDot(GameData.grid,selection-1)==-1){GameData.availableColumns[selection-1]=0;}
        printf(" 0-Save Game  8-Exit\n");
        drawGrid(GameData);
        int checkRes = checkResult(GameData.grid);
        //HIGHLIGHTS THE 4 SYMBOLS IN A ROW IF THEY EXIST
        if((int)checkRes/100==1){
            for(i=0;i<4;i++){
                GameData.grid[(checkRes%100)/10][(checkRes%10)+i]='@';
            }
            gameRunning = 0;
        }else if((int)checkRes/100==2){
            for(i=0;i<4;i++){
                GameData.grid[((checkRes%100)/10)+i][(checkRes%10)]='@';
            }
            gameRunning = 0;
        }else if((int)checkRes/100==3){
            for(i=0;i<4;i++){
                GameData.grid[((checkRes%100)/10)+i][(checkRes%10)+i]='@';
            }
            gameRunning = 0;
        }else if((int)checkRes/100==4){
            for(i=0;i<4;i++){
                GameData.grid[((checkRes%100)/10)+3-i][(checkRes%10)+i]='@';
            }
            gameRunning = 0;
        }else if(GameData.turnCounter>=42){gameRunning = 0;}

        GameData.turnCounter+=1;    //  Increments to the next turn
    }
    ClearConsoleToColors(textColor,backgroundColor);
    printf("Game Over!\n");
    drawGrid(GameData);
    if(((GameData.turnCounter+GameData.startingPlayer) %2==0) && (int)checkRes/100>0){
        SetColor(GameData.PlayersData.P1.color);    printf("%s Wins!\n",GameData.PlayersData.P1.name);  SetColor(textColor);
    }else if(((GameData.turnCounter+GameData.startingPlayer) %2==1) && (int)checkRes/100>0){
        SetColor(GameData.PlayersData.P2.color);    printf("%s Wins!\n",GameData.PlayersData.P2.name);  SetColor(textColor);
    }else if(GameData.turnCounter==42){
        printf("Nobody Wins!\n");
    }
    promptPlayAgain();
}

//FIRST LOAD MENU
void loadMenu(){
    int selection;
    int menu=1;
    while(menu){
        do{
            ClearConsoleToColors(textColor,backgroundColor); // Sets the background to white
            printf("Select the operation that you would like to do.\n");
            printf("1 - All Saved Games   \n2 - Saved Games with a specified player   \n3 - Back   \n   \nSelection: ");
            selection=readInput();
        }while(selection<1 || selection > 4);
        switch(selection){
        case 1: listSaves(FILENAME); break;
        case 2: listSavesByName(FILENAME); break;
        case 3: menu=0; break;
        }
    }
}

void loadMenu2(int index, int returning){
    int selection;
    gameData GameData;
        do{
            printf("\nWhat do you want to do?.\n");
            printf("0 - Back   \n1 - Load Game  \n2 - Delete Game   \n   \nSelection: ");
            selection=readInput();
        }while(selection<0 || selection > 2);
        if(selection==0){
            if(returning==0){listSaves(FILENAME);}
            else if(returning==1){listSavesByName(FILENAME);}
        }
        else if(selection==1){gameController(2,FindSaveByID(FILENAME,index));}
        else{
            SaveRemove(FILENAME,"TEMP.txt",FindSaveByID(FILENAME,index));
            if(returning==0){listSaves(FILENAME);}
            else if(returning==1){listSavesByName(FILENAME);}
        }
}


//MAIN MENU
int main(){
    int selection=0;
    while(1){
        do{
            ClearConsoleToColors(textColor,backgroundColor); // Sets the background to white
            printf("Select the operation that you would like to do.\n");
            printf("1 - New game   \n2 - Load game   \n3 - Exit\n   \nSelection: ");
            selection=readInput();
            if(selection < 1 || selection > 3){
                printf("Invalid input, try again\n");
                selection=0;
            }
        }while(selection<1 || selection >3);
        ClearConsoleToColors(textColor,backgroundColor);
        switch(selection){
        case 1: gameController(0,0);  break;
        case 2: loadMenu();      break;
        case 3:
            selection=promptExit();
            if(selection==1){
                ClearConsoleToColors(textColor,backgroundColor);
                printf("\nGoodbye!\n");
                exit(0);
            }
        }
    }
    return 0;
}
