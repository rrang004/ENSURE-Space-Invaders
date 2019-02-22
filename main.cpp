//Coded by Richard S. Rangel
#include <iostream>
#include <ctime>
#include <vector>
#include <conio.h>
#include <limits>
#include <stdexcept>
#include <windows.h>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <list>
#include <sstream>
#include <string>
using namespace std;

#define KEY_LEFT 75
#define KEY_RIGHT 77
#define TIMELIMIT 5
int score = 0; //user score (NOT HIGH SCORE)
int difficulty = 2; //starting number of layers of enemies
int enemyNumber = 4; //starting number of total enemies



void shoot(vector<vector<char>* > &board, int &enemies)
{
	//compare x coordinate, first one closest to line1, back of board
	//get player x position

	unsigned xPos;
	for(unsigned i = 0; i < board.back()->size(); i++)
	{
		if(board.back()->at(i) == '^')
		{
			xPos = i;
		}
	}
	for(unsigned i = board.size() - 2; i > 0; i--) //starts one past player row,
	{
		if(board.at(i)->at(xPos) == 'V')
		{

  			//cout << "\a" << endl;
  			board.at(i)->at(xPos) = ' ';
			score++;
			enemies--;
			return;
		}
	}
}
void move(char pos, vector<char> *line1) //FIX LEFTMOST CRASH
{
    //find way to read input without pressing enter
    //update position within line1 vector
    if(pos == KEY_LEFT)
    {
    	//cout << "entered left" << endl;

    	for(unsigned i = 0; i < line1->size(); i++)
		{
			if(line1->at(i) != ' ' && i > 0)
			{

					if(i > 0)
					{
						//cout << "move left" << endl;
						line1->at(i-1) = '^';
						line1->at(i) = ' ';

						return;
					}
					else
						return;


			}
		}
	}
    else if(pos == KEY_RIGHT)
    {
    	//cout << "entered right" << endl;
    	for(unsigned i = 0; i < line1->size(); i++)
		{
			if(line1->at(i) != ' ')
			{
				//cout << "Position: " << i << endl;
				if(!(i+ 1 >= line1->size()))
				{
					//cout << "move right" << endl;
					line1->at(i+ 1) = '^';
					line1->at(i) = ' ';
					return;
				}

			}

		}
	}
}
//USE BEFORE REDRAWING SCREEN
void clearScreen()
{
    //cout << endl;
    //cout << "\033c";
    //cout << endl;

	 cout << string( 100, '\n' );
    return;
}
void drawScreen(vector<vector<char> *> &x)
{
    //draw updated positions/remove aliens

    for(int i = 0; i < x.size(); i++)
    {
        for(int j = 0; j < x.at(i)->size(); j++)
        {
            cout << x.at(i)->at(j);
        }
        cout << endl;
    }

    return;
}

struct console
  {
  console( unsigned width, unsigned height )
    {
    SMALL_RECT r;
    COORD      c;
    hConOut = GetStdHandle( STD_OUTPUT_HANDLE );
    if (!GetConsoleScreenBufferInfo( hConOut, &csbi ))
      throw runtime_error( "You must be attached to a human." );

    r.Left   =
    r.Top    = 0;
    r.Right  = width -1;
    r.Bottom = height -1;
    SetConsoleWindowInfo( hConOut, TRUE, &r );

    c.X = width;
    c.Y = height;
    SetConsoleScreenBufferSize( hConOut, c );
    }

  ~console()
    {
    SetConsoleTextAttribute(    hConOut,        csbi.wAttributes );
    SetConsoleScreenBufferSize( hConOut,        csbi.dwSize      );
    SetConsoleWindowInfo(       hConOut, TRUE, &csbi.srWindow    );
    }

  void color( WORD color = 0x07 )
    {
    SetConsoleTextAttribute( hConOut, color );
    }

  HANDLE                     hConOut;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  };

//----------------------------------------------------------------------------
console con( 30, 10 ); //adjusts size

void generateLevel(vector<vector<char>* > &board, int &enemies)
{

	for(unsigned j = 0; j < enemyNumber; j++)
	{
		int level = rand()%difficulty + 1;
		int enemyIndex = rand()%board.at(level)->size();

		if(board.at(level)->at(enemyIndex) != 'V')
		{
				board.at(level)->at(enemyIndex) = 'V';
				enemies++;
		}
		else
			j--;

	}


	difficulty++;
	enemyNumber += 3;
}
class Timer {
public:
    Timer() { start = ::time(NULL); }
    virtual ~Timer() { }

    unsigned long Lapse() {
        //edit: this is wrong
        //return (::time(NULL)-start)/1000;
        return ::time(NULL)-start;
    }
private:
    unsigned long start;
};
int main()
{
    //6 lines of enemies in pyramid
 	srand(time(NULL));

  	vector<char> *line10 = new vector<char>(11, ' ');
    vector<char> *line9 = new vector<char>(11, ' ');
    vector<char> *line8 = new vector<char>(11, ' ');
    vector<char> *line7 = new vector<char>(11, ' ');
    vector<char> *line6 = new vector<char>(11, ' ');
    vector<char> *line5 = new vector<char>(11, ' ');
    vector<char> *line4 = new vector<char>(11, ' ');
    vector<char> *line3 = new vector<char>(11, ' ');
    vector<char> *line2 = new vector<char>(11, ' ');
    vector<char> *line1 = new vector<char>(11, ' '); //row containing player
    line1->at(5) = '^';

    vector<vector<char>* > board;
    board.push_back(line10);
    board.push_back(line9);
    board.push_back(line8);
    board.push_back(line7);
    board.push_back(line6);
    board.push_back(line5);
    board.push_back(line4);
    board.push_back(line3);
    board.push_back(line2);
    board.push_back(line1); //contains player

	int enemies;

	enemies = 0;
   	generateLevel(board, enemies);
	//drawScreen(board);


	cout << "Welcome to Space Invaders" << endl;
	cout << "You have 10 seconds to \nclear each level." << endl;
	cout << "Use the left and right \narrow keys to move and \nspace to shoot." << endl;
	cout << "Press space to begin." << endl;

	char x = getch();


	bool GameRunning = true;

	//code to play a level with a 5 second timer (working OK)
	while(GameRunning)
	{
		Timer t;
		while(t.Lapse() < TIMELIMIT)
		{
			while(enemies > 0)
			{
				if(t.Lapse() > TIMELIMIT) //waits for action to break out
					break;

				char input = getch();

				if(t.Lapse() > TIMELIMIT) //waits for action to break out
					break;

				if(input == KEY_LEFT || input == KEY_RIGHT)
		  		{
		  			if(t.Lapse() > TIMELIMIT) //waits for action to break out
					break;

		  			move(input, line1);

					clearScreen();
		  	 		drawScreen(board);
				}
		  		else if(input == ' ')
		  		{
		  			if(t.Lapse() > TIMELIMIT) //waits for action to break out
					break;

		  			if(enemies == 0)
		  				break;
		  			else
		  			{
		  				shoot(board, enemies);
 						clearScreen();
		  	 			drawScreen(board);
					}

				}

			}
			break;
		}
		if(enemies == 0)
			generateLevel(board, enemies);
		else
			GameRunning = false;
	}
	system("PAUSE");
	cout << "Game Over" << endl;
	cout << "Score: " << score << endl;

	clearScreen();
	ifstream scoresList;
	scoresList.open("invadersScores.txt");

	if(!scoresList.is_open())
		cout << "Error opening scores list." << endl;

    cout << "Your score: " << score << endl;
	cout << "Scores List: " << endl;
	int scoresPlace = 1;
	string nameScore;

	vector<int> scoreBoard;
	int readScore;
	while(scoresList >> readScore)
    {
        scoreBoard.push_back(readScore);
    }
    int l = 1;
    for(int k = 0; k < scoreBoard.size(); k++)
    {
         cout << l << ": " << scoreBoard.at(k) << endl;
        l++;

    }
    cout << "We will now update the list\nwith your score." << endl;

    system("PAUSE");
    bool abc;
    for(int k = 0; k < scoreBoard.size(); k++)
    {
        if(scoreBoard.at(k) <= score)
        {
            scoreBoard.insert(scoreBoard.begin() + k,  score);
            abc = false;
            break;
        }
    }
    if(abc == true)
        scoreBoard.push_back(score);
    cout << "Updated Score List: " << endl;

    l = 1;
    for(int k = 0; k < scoreBoard.size(); k++)
    {
         cout << l << ": " << scoreBoard.at(k) << endl;
        l++;

    }

    scoresList.close();
    ofstream scoresListNew("invadersScores.txt");
    for(int k = 0; k < scoreBoard.size(); k++)
    {
        scoresListNew << scoreBoard.at(k) << endl;
    }
    cout << "Thank you for playing! " << endl;

    scoresListNew.close();
    system("PAUSE");

    return 0;
}
