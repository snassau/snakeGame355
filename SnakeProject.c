#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>

#define WIDTH 30
#define HEIGHT 15

char direction = 'd';

void setupTerminal()
{
    struct termios t;

    tcgetattr(STDIN_FILENO, &t);

    t.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int KeyboardHit() 
{
    struct timeval tv = {0L, 0L};
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

int main()
{
    int x = WIDTH / 2;
    int y = HEIGHT / 2;

    int foodX;
    int foodY;

    srand(time(NULL));

    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;

    setupTerminal();

    while (1)
    {
        system("clear");

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
                    printf("##");
                }
                else if (r == y && c == x)
                {
                    printf("-D");
                }
                else if (r == foodY && c == foodX)
                {
                    printf("..");
                }
                else
                {
                    printf("  ");
                }
            }

            printf("\n");
        }

        printf("\nUse W(Up) A(Left) S(Down) D(Right) to move\n");

        /* Read key presses from the keyboard */
        while (KeyboardHit())  
        {
            char key = getchar();

            if (key == 'w')
                direction = 'w';

            else if (key == 's')
                direction = 's';

            else if (key == 'a')
                direction = 'a';

            else if (key == 'd')
                direction = 'd';
        }

        /* Move the head of the snake automatically */
        if (direction == 'w')
            y--;

        else if (direction == 's')
            y++;

        else if (direction == 'a')
            x--;

        else if (direction == 'd')
            x++;

        /* If you hit the wall, game over */
        if (x == 0 ||
            x == WIDTH - 1 ||
            y == 0 ||
            y == HEIGHT - 1)
        {
            system("clear");

            printf("\nGAME OVER!\n");

            break;
        }

        /* Snake eating the dot/food */
        if (x == foodX && y == foodY)
        {
            foodX = rand() % (WIDTH - 2) + 1;
            foodY = rand() % (HEIGHT - 2) + 1;
        }

        /* The speed for the snake */
        usleep(150000); 
    }

    return 0;
}