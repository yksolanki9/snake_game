#include<iostream>
// #include "keybit"
// #include<ncurses.h>

/*Used for sleep function */
#include <chrono>
#include <thread>

using namespace std;


/* keybit function in linux */
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);    
    ch = getchar(); 
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf); 
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }   
    return 0;
}

/*getch() function in linux */
#include <termios.h>
#include <stdio.h>

static struct termios old, new_char;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); //grab old terminal i/o settings
  new_char = old; //make new settings same as old settings
  new_char.c_lflag &= ~ICANON; //disable buffered i/o
  new_char.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
  tcsetattr(0, TCSANOW, &new_char); //apply terminal io settings
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* 
Read 1 character without echo 
getch() function definition.
*/
char getch(void) 
{
  return getch_(0);
}




// /*Main program starts here */
bool isGameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100], nTail;
enum eDirection {STOP, LEFT, RIGHT, UP, DOWN};
eDirection dir; 

void setup(){
    // /* Initialize the curses environment*/
    // initscr();
    // cbreak();
    // noecho();
    // clear();
    // curs_set(false);

    /* Setup the game*/
    isGameOver = false;
    dir = STOP;
    x = width/2;
    y = (height-2)/2;
    fruitX = rand() % width; //% width makes sure that value of fruitX does not                                 go beyond width
    fruitY = rand() % height;
    score = 0;
}

void draw_map(){
    system("clear");
    for(int i = 0; i<width+1; i++)
        cout<<"#";
    cout<<endl;

    for(int i = 0; i<height; i++){
        for(int j = 0; j<width; j++){
            if(j == 0 || j == width-1)        //borders left and right
                cout<<"#";

            if(i == y && j == x)
                cout<<"@";              //head of snake
            else if(i == fruitY && j == fruitX)
                cout<<"O";              //fruit position
            else{
                bool isPrint = false;
               for(int k = 0; k<nTail; k++){
                   if(tailX[k] == j && tailY[k] == i){
                       cout<<"*";
                       isPrint = true;
                   }
               }
               if(!isPrint)
                cout<<" ";                    

            }
        }
        cout<<endl;
    }
    for(int i = 0; i<width+1; i++)
        cout<<"#";
    cout<<endl;
    cout<<"Score: "<<score<<endl;
}

void user_input(){
    if(kbhit()){
        int c = 0;
        switch(c = getch()){
            case '75':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                isGameOver = true;
                break;
        }
        // sleep(10);
    }
}

void controller(){
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;

    tailX[0] = x;
    tailY[0] = y;
    for(int i = 1; i<nTail; i++){
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;

    }

    switch(dir){
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
    }

    // if(x> (width-1) || x<0 || y>(height-1) || y<0)
    //     isGameOver = true;

    if(x >= width)
        x = 0;
    else if(x < 0)
        x = width -1;
    if(y >= height)
        y = 0;
    else if(y < 0)
        y = height -1;

    for(int i = 0; i<nTail; i++){
        if(tailX[i] == x && tailY[i] == y)
            isGameOver = true;
    }

    if(x == fruitX && y == fruitY){
        score++;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main(){
    setup();
    // draw_map();
    while(!isGameOver){
        draw_map();
        user_input();
        controller();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    // endwin();
    return 0;
}