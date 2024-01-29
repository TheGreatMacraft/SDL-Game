#include <iostream>
#include <ctime>
#include <cstdlib>

void random(char map[20][20], int n)
{
    int x,y,x1,y1;
    for(int j=0;j<50;j++)
    {
        for(int i=0;i<50;i++)
            map[j][i] = '0';   
    }
    for(int i=0;i<3;i++)
        {
            x = rand()%(20);
            y = rand()%(20);
            if(map[x][y] != '#')
                map[x][y] = '#';
            else
                i--;
        }
    x = 0;
    y = 0;
    for (int i=0;i<8;i++)
    {
        int j = rand()%(3);
        if(j==0)
            {
                x1 = x;
                if(i==9)
                    y1 = 19;
                else
                y1 = rand()%(20);
            }
        else if (j==1)
        {
            y1 = y;
            if(i==9)
                x1 = 19;
            else
                x1 = rand()%(20);
        }
        else
        {
            if(i==9)
            {
                y1 = 19;
                x1 = 19;
            }
            else
            {
                x1 = rand()%(20);
                y1 = rand()%(20);
            }
        }
        while(x!=x1 || y!=y1)
        {
            map[x][y] = '+';
            if(x1>x)
                x++;
            else if(x1<x)
                x--;
            if(y1>y)
                y++;
            else if(y1<y)
                y--;
        }
        
    }
}

void izpis(char map[20][20], int n)
{
    for(int j=0;j<n;j++)
    {
        for(int i=0;i<n;i++)
            std::cout << map[j][i] << "|";
        std::cout << std::endl;
    }
}

int main()
{
    srand(time(0));
    char map[20][20];
    random(map,20);
    izpis(map,20);
}