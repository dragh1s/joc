#include <iostream> 
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;
bool gameOver;
const int width =30;
const int height =15;
int x,y,fruitX, fruitY , score, highScore;
int bonusX, bonusY;
bool hasBonus=false;
int bonusCounter=0;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

int  obsX[10],obsY[10];
int nObs=5;
int baseSpeed=100;
void LoadHighScore()
{
    ifstream file("highscore.txt");
    if (file.is_open()){
        file >>highScore;
        file.close();
    }
    else{
        highScore=0;
    }
}
void SaveHighScore()
{
    if(score> highScore){
        highScore=score;
        ofstream file("highscore.txt");
        if(file.is_open()){
            file<<highScore;
            file.close();
        }
    }
}

void Setup()
{
    srand(time(0));
    LoadHighScore();
    gameOver = false;
    dir = STOP;
    x=width/2;
    y=height/2;
    fruitX =rand()%width;
    fruitY=rand()%height;
    score =0;
    nTail=0;
    hasBonus=false;
    bonusCounter= 0;
    for(int i=0;i<nObs;i++)
    {
        obsX[i]=5+(i*4);
        obsY[i]=4+(i%3)*3;
    }
    }

    void Draw(){
        COORD coord;
        coord.X=0;
        coord.Y=0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
        string buffer="";
        buffer+="\033[37m";
        for(int i=0;i<width+2;i++)buffer +="#";
        buffer +="\n";
        for(int i=0;i<height;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(j==0) buffer+="\033[37m#";
                if(i==y&&j==x)
                {
                    buffer+="\033[32mO";
                }
                else
                if(i==fruitY && j==fruitX)
                {
                    buffer+="\033[31mF";
                }
                else if(hasBonus && i==bonusY && j==bonusX)
                {
                    buffer +="\033[33m*";
                }
                else{
                    bool isObstacle=false;
                    for(int o=0;o<nObs;o++)
                    {
                        if(obsX[o]==j&& obsY[o]==i){
                            buffer+="\033[36mX";
                            isObstacle =true;
                            break;
                        }
                    }
                    if(!isObstacle){
                        bool printTail =false;
                        for(int k=0;k<nTail;k++)
                        {
                            if(tailX[k]==j&&tailY[k]==i){
                                buffer+="\033[32mo";
                                printTail=true;
                                break;
                            }
                        }
                        if(!printTail) buffer +=" ";
                    }
                }
                if(j==width-1) buffer +="\033[37m#\033[0m";
            }
            buffer +="\n";
        }
        buffer +="\033[37m";
        for(int i=0;i<width+2;i++) buffer +="#";
        buffer +="\n";

        buffer +="\033[0mScore: " +to_string(score) + " | High Score: " + to_string(highScore) + "\n";
        buffer +="Controls: W,A,S,D to move | X to exit | P=pauza\n";
        cout<<buffer;
    }

    void Input(){
        if(_kbhit())
        {
            switch(_getch())
            {
                case 'a': case 'A': if(dir !=RIGHT) dir =LEFT; break;
                case 'd': case 'D': if(dir!=LEFT) dir=RIGHT; break;
                case 'w': case 'W':if(dir!=DOWN) dir =UP;break;
                case 's': case 'S': if(dir!=UP) dir=DOWN;break;
                case 'p': case 'P':
                cout<<"\n\033[33m[ PAUSE ] Press any key to continue  \033[0m\n";
                _getch();
                break;
                case 'x': case 'X' :
                gameOver=true; break;
                
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
            case LEFT: x--;break;
            case RIGHT: x++; break;
            case UP: y--;  break;
            case DOWN: y++; break;
            default:break;
        }
        //pierderea la pereti
        if(x>=width || x<0 || y>=height || y<0)
        gameOver =true;

        for(int i=0;i<nTail; i++)
        {
            if (tailX[i]==x && tailY[i]==y)
            gameOver=true;
        }

        for(int i=0;i< nObs;i++)
        {
            if (obsX[i] == x && obsY[i]==y)
            gameOver =true;
        }

        if(x==fruitX&& y==fruitY)
        {
            score=score+10;
            Beep(800, 40);
            fruitX=rand()%width;
            fruitY=rand()%height;
            nTail++;
            bonusCounter++;

            if (bonusCounter >=3){
                bonusX=rand() %width;
                bonusY=rand() %height;
                hasBonus=true;
                bonusCounter= 0;
            }
        }
        if (hasBonus && x==bonusX && y==bonusY){
            score +=30;
            Beep(1200, 70);
            hasBonus=false;
        }
     }

     int main(){
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode =0;
        if(GetConsoleMode(hConsole,&mode)){
            SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
        system("cls");
        cout<< "==========================================\n";
        cout<< "          My Snake Game                   \n";
        cout<< "         By Matei Dragos                  \n";
        cout<< "==========================================\n";
        cout<< "Choose difficulty level: \n";
        cout<< "1. Easy (Lower Speed)\n";
        cout<< "2. Medium (Normal Speed)\n";
        cout<< "3. Hard (Higher Speed)\n";
        cout<< "Enter yout choice (1 to 3): ";

        int choice;
        cin>>choice;
        if(choice==1) baseSpeed =140;
        else if(choice ==2) baseSpeed =90;
        else if(choice==3)baseSpeed=55;
        else baseSpeed=90; // in case bad vallue introdusa

        Setup();
        while (!gameOver) {
            Draw();
            Input();
            Logic();

            int currentSpeed =max(25, baseSpeed -(score /30 ) *8);
            Sleep(currentSpeed);
        }
        Beep(300,350);
        SaveHighScore();
        cout<< "\n\033[31m=== GAME OVER ===\033[0m\n";
        cout<< "Scor final: "<<score<<" | High Score: "<<highScore<<"\n";
        system("pause");
        return 0;
     }

