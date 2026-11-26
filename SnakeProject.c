#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>
#include <signal.h>
#include <curses.h>

#define WIDTH 30
#define HEIGHT 15

char direction = 'd';

int main()
{
    int x = WIDTH / 2;
    int y = HEIGHT / 2;

    int foodX;
    int foodY;

    int score = 0; // This is where score is initilized to start at 0

    int snakeLength = 1; // start the snake body with one(head)

    int snakeX[100]; //array for the body 
    int snakeY[100];

    snakeX[0] = x; //setting each position to 0.
    snakeY[0] = y;

    srand(time(NULL));

    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;

    /* curses setup: replaces setupTerminal(), resetTerminal(), and the signal handlers */
    initscr();
    cbreak();               // no line buffering
    noecho();               // don't echo keys
    keypad(stdscr, TRUE);   // enables KEY_UP, KEY_DOWN, etc.
    curs_set(0);            // hide the cursor
    nodelay(stdscr, TRUE);  // getch() returns ERR instead of waiting

    while (1)
    {
        erase();

        /* Draw  the board on terminal */
        for (int r = 0; r < HEIGHT; r++)
        {
            for (int c = 0; c < WIDTH; c++)
            {
                if (r == 0 ||
                    r == HEIGHT - 1 ||
                    c == 0 ||
                    c == WIDTH - 1)
                {
                    mvaddstr(r, c * 2, "##");
                }
                else
                {
                    int bodyPart= 0;
                    for (int i = 1; i < snakeLength; i++)
                    {
                        if (r == snakeY[i] && c == snakeX[i])
                        {
                            mvaddstr(r, c * 2, "O");
                            bodyPart = 1;
                            break;
                        }
                    }
                    if (!bodyPart)
                    {
                        if (r == y && c == x)
                        {
                            mvaddstr(r, c * 2, "O");
                        }
                        else if ( r == foodY && c == foodX)
                        {
                            mvaddstr(r, c * 2, "..");
                        }
                    }
                }
                    
                
                
                /*empty cells since erase() has blanked them already*/
            }
        }

        mvprintw(HEIGHT + 1, 0, "Your Score: %d     Length: %d", score, snakeLength);
        mvprintw(HEIGHT + 3, 0, "Use W(Up) A(Left) S(Down) D(Right) to move, Q to quit");


        refresh(); //nothing appears until this is called

        int key;
        while ((key = getch()) != ERR){
            if (key == 'w' || key == KEY_UP)        direction = 'w';
            else if (key == 's' || key == KEY_DOWN) direction = 's';
            else if (key == 'a' || key == KEY_LEFT) direction = 'a';
            else if (key == 'd' || key == KEY_RIGHT)direction = 'd';
            else if (key == 'q')                    direction = 'q';
        }

        if (direction == 'q')
            break;

        int oldX = x;
        int oldY = y;

        /* Move the head of the snake automatically */
        if (direction == 'w')
            y--;

        else if (direction == 's')
            y++;

        else if (direction == 'a')
            x--;

        else if (direction == 'd')
            x++;
        

        for (int i = snakeLength - 1; i > 0; i--) // MOVES each body segment forwars so it follows the head
        {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
        snakeX[0] = x; // UPDATE head position
        snakeY[0] = y;

        /* If you hit the wall, game over */
        if (x == 0 ||
            x == WIDTH - 1 ||
            y == 0 ||
            y == HEIGHT - 1)
        {
            break;
        }

        /* Snake eating the dot/food */
        if (x == foodX && y == foodY)
        {
            
            score++; // Here is where the score it counted

            snakeLength++;
            
            foodX = rand() % (WIDTH - 2) + 1;
            foodY = rand() % (HEIGHT - 2) + 1;
        }
        
        napms(150); //repalces usleep(150000);
    }

    mvprintw(HEIGHT / 2, 5, "GAME OVER! Final Score: %d  (press any key to exit)", score);
    refresh();

    nodelay(stdscr, FALSE);  // make getch() wait
    flushinp();              // discard leftover movement keypresses
    getch();                 // wait for a key

    endwin();
    return 0;
}
