//******************************************************************************

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <assert.h>

#include "autobots.h"

#define WIDTH 80
#define HEIGHT 24

#define LEN 2048

char strategy(const char* const* board);

//******************************************************************************

int autobots(char (*strategy)(const char* const*), int rounds, int advance, int verbose)
{
  int master = posix_openpt(O_RDWR | O_NOCTTY);
  if (master < 0)
  {
    perror("openpt failed");
    return 1; // error
  }
  if (grantpt(master))
  {
    perror("grantpt failed");
    return 1; // error
  }
  if (unlockpt(master))
  {
    perror("unlockpt failed");
    return 1; // error
  }
  pid_t pid = fork();
  if (pid < 0)
  {
    perror("fork failed");
    return 1; // error
  }

//******************************************************************************

  if (pid) // parent
  {
    char screen[HEIGHT][WIDTH];
    memset(screen, ' ', HEIGHT * WIDTH);
    int row = 0;
    int col = 0;

    char *board[ROWS] =
    {
      &screen[1][1], &screen[2][1], &screen[3][1], &screen[4][1],
      &screen[5][1], &screen[6][1], &screen[7][1], &screen[8][1],
      &screen[9][1], &screen[10][1], &screen[11][1], &screen[12][1],
      &screen[13][1], &screen[14][1], &screen[15][1], &screen[16][1],
      &screen[17][1], &screen[18][1], &screen[19][1], &screen[20][1],
      &screen[21][1], &screen[22][1]
    };

    for (;;) // games
    {
      for (row = 0; row < ROWS; row++)
	memset(board[row], ' ', COLS);

      int turn;
      for (turn = 0;; turn++) // turns
      {
	for (;;) // chunks
	{
	  char str[LEN];
	  char *p = str;
	  ssize_t len = 0;

	  fd_set set;
	  FD_ZERO(&set);
	  FD_SET(master, &set);
	  struct timeval timeout = { 0L, 100000L }; // 100ms
	  for (fd_set s = set;
	    select(master + 1, &s, NULL, NULL, &timeout);
	    s = set)
	  {
	    ssize_t l = read(master, p, LEN - len);
	    p += l;
	    len += l;
	  }
	  str[len] = '\0';

	  if (*str == '\a')
	  {
	    fprintf(stderr, "invalid move\n");
	    kill(pid, SIGTERM);
	    return 1; // error
	  }

	  if (len > 13 && !strcmp(str + len - 13, "Another game?"))
	    goto DONE;

	  for (p = str; *p; p++)
	  {
	    // https://en.wikipedia.org/wiki/VT52
	    if (*p == '\033')
	    {
	      p++;
	      assert(*p);

	     if (*p == 'A')
		row--;
	      else if (*p == 'B')
		row++;
	      else if (*p == 'C')
		col++;
	      else if (*p == 'D')
		col--;
	      else if (*p == 'G')
		;
	      else if (*p == 'H')
		row = col = 0;
	      else if (*p == 'J')
		memset(screen, ' ', HEIGHT * WIDTH);
	      else if (*p == 'K')
		;
	      else if (*p == 'Y')
	      {
		p++;
		assert(*p);
		row = *p - ' ';	
		p++;
		assert(*p);
		col = *p - ' ';	
	      }
	      else
		assert(0);
	    }
	    else if (*p == '\r')
	      col = 0;
	    else
	    {
	      assert(*p >= ' ' && *p <= '~');
	      assert(row >= 0 && row < HEIGHT);
	      assert(col >= 0 && col < WIDTH);
	      screen[row][col] = *p;
	      col++;
	    }
	  }
	  if (col <= COLS && screen[row][col] == BOARD_SELF)
	    break;

	  if (!strncmp(&screen[22][62], "Teleport!", 9))
	  {
	    kill(pid, SIGCONT);
	    memset(&screen[22][62], ' ', 9);
	  }  
	}

	if (verbose)
	  for (int row = 0; row < HEIGHT; row++)
	  {
	    assert(screen[row][COLS + 2] = ' ');
	    screen[row][COLS + 2] = '\0';
	    printf("%s\n", &screen[row][0]);
	    screen[row][COLS + 2] = ' ';
	  }

        char key = (*strategy)((const char *const*)board);

	if (verbose)
	  printf("[%c]\n\n", key);
	write(master, &key, 1);
      }

    DONE:
      {
	int score = atoi(&screen[21][COLS + 10]);
	printf("turns=%d score=%d\n", turn, score);
      }

      rounds--;
      if (!rounds)
	break;
      write(master, "y", 1);

      if (verbose)
	printf("\n\n");
    }
    write(master, "n", 1);
    sleep(1);
    kill(pid, SIGTERM);
    wait(NULL);
  }

//******************************************************************************

  else // child
  {
    const char *name = ptsname(master);
    if (!name)
    {
      perror("ptsname failed");
      exit(EXIT_FAILURE);
    }
    int slave = open(name, O_RDWR | O_NOCTTY);
    if (slave < 0)
    {
      perror("open pts  failed");
      exit(EXIT_FAILURE);
    }
    close(master);
    close(STDIN_FILENO);
    if (dup(slave) != STDIN_FILENO)
    {
      perror("dup stdin failed");
      exit(EXIT_FAILURE);
    }
    close(STDOUT_FILENO);
    if (dup(slave) != STDOUT_FILENO)
    {
      perror("dup stdout failed");
      exit(EXIT_FAILURE);
    }
    close(slave);

    setenv("TERM", "vt52", 1);
    if (advance)
      execl("/usr/games/robots", "robots", "-a", NULL);
    else
      execl("/usr/games/robots", "robots", NULL);

    perror("exec failed");
    exit(EXIT_FAILURE);
  }
  return 0;
}

//******************************************************************************

int main(int ac, char *av[])
{
  int advance = 0;
  int verbose = 0;
  int rounds = 1;
  for (int ai = 1; ai < ac; ai++)
  {
    if (!strcmp(av[ai], "-a"))
      advance = 1;
    else if (!strcmp(av[ai], "-v"))
      verbose = 1;
    else if (isdigit(av[ai][0]))
    {
      rounds = atoi(av[ai]);
      if (!rounds)
      {
	fprintf(stderr, "invalid rounds: %s\n", av[ai]);
	return EXIT_FAILURE;
      }
    }
    else
    {
      fprintf(stderr, "invalid argument: %s\n", av[ai]);
      return EXIT_FAILURE;
    }
  }

  if (autobots(strategy, rounds, advance, verbose))
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

//******************************************************************************
