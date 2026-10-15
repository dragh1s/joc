#include <iostream> 
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;
bool gameOver;
const int width =30;
const int height =15;
int x,y,fruitX, fruitY , score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;


void Setup()
{
    srand(time(0));
    gameOver = false;
    dir = STOP;
    x=width/2;
    y=height/2;
    fruitX =rand()%width;
    fruitY=rand()%height;
    score =0;
    nTail=0;
    }

    void Draw(){
        COORD coord;
        coord.X=0;
        coord.Y=0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
        //marginea de sus
        for(int i=0;i<width+2;i++)
        cout<<"#";
        cout<<endl;
        for(int i=0;i<height;i++)
        { 
            for(int j=0;j<width;j++)
            {
                if(j==0)
                cout<<"#";
                if(i==y&&j==x)
                cout<<"O";
                else
                if(i==fruitY&&j==fruitX)
                cout<< "F";
                else 
                {
                    bool printTail = false;
                    for(int k=0;k<nTail;k++)
                    {
                        if(tailX[k]== j&& tailY[k]==i){
                            cout<<"o";
                            printTail =true;
                        }
                    }
                    if(!printTail)
                    cout<<" ";               
                 }
                 if(j== width-1)
                 cout<< "#";
            }
            cout<<endl;
        }
        //marginea de jos
        for (int i=0;i<width+2;i++)
        cout<<"#";
        cout<<endl;
        cout<<"Score: "<<score<<endl;
        cout<<"Controls: W(up), A(left), S(down), D(right), X(exit)"<<endl;
    }

    void Input(){
        if(_kbhit())
        {
            switch(_getch())
            {
                case 'a':
                if(dir !=RIGHT) dir =LEFT;
                break;
                case 'd':
                if(dir!=LEFT) dir=RIGHT;
                break;
                case 'w':
                if(dir!=DOWN) dir =UP;
                break;
                case 's':
                if(dir!=UP) dir=DOWN;
                break;
                case 'x': 
                gameOver = true;
                break;
            }
        }
     }

     void Logic(){
        int prevX=tailX[0];
        int prevY=tailY[0];
        int prev2X, prev2Y;
        tailX[0]=x;
        tailY[0]=y;
        for(int i=1;i<nTail;i++)
        {
            prev2X=tailX[i];
            prev2Y=tailY[i];
            tailX[i]=prevX;
            tailY[i]=prevY;
            prevX=prev2X;
            prevY=prev2Y;
        }
        switch (dir){
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
            default:
            break;
        }
        //pierderea la pereti
        if(x>=width || x<0 || y>=height || y<0)
        gameOver =true;

        for(int i=0;i<nTail; i++)
        {
            if (tailX[i]==x && tailY[i]==y)
            gameOver=true;
        }
        if(x==fruitX&& y==fruitY)
        {
            score=score+10;
            fruitX=rand()%width;
            fruitY=rand()%height;
            nTail++;
        }
     }

     int main(){
        Setup();
        while(!gameOver){
            Draw();
            Input();
            Logic();
            Sleep(100);
        }
        cout<< "Game Over! Finall score: "<< score<<endl;
        return 0;
     }

