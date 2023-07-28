//******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "autobots.h"

#define NOTHING 0
#define UP 1
#define LEFT_UP 2
#define LEFT 3
#define LEFT_DOWN 4
#define DOWN 5
#define RIGHT_DOWN 6
#define RIGHT 7
#define RIGHT_UP 8

#define NIL '\0'

//******************************************************************************
// >>> validity

const char* validity(const char* const* board, int row, int col)
{
  static char valid[9];

  valid[NOTHING] = valid[UP] = valid[LEFT_UP] = valid[LEFT] = valid[LEFT_DOWN]
    = valid[DOWN] = valid[RIGHT_DOWN] = valid[RIGHT] = valid[RIGHT_UP] = 1;

  // borders
  if (row == 0)
    valid[UP] = valid[LEFT_UP] = valid[RIGHT_UP] = 0;
  if (row == ROWS - 1)
    valid[LEFT_DOWN] = valid[DOWN] = valid[RIGHT_DOWN] = 0;
  if (col == 0)
    valid[LEFT_UP] = valid[LEFT] = valid[LEFT_DOWN] = 0;
  if (col == COLS - 1)
    valid[RIGHT_DOWN] = valid[RIGHT] = valid[RIGHT_UP] = 0;

  // 8 primary neighbors
  if (row > 0)
    if (board[row - 1][col] == BOARD_ROBOT)
      valid[NOTHING] = valid[UP] = valid[LEFT_UP] = valid[LEFT]
	= valid[RIGHT] = valid[RIGHT_UP] = 0;
    else if (board[row - 1][col] == BOARD_JUNK)
      valid[UP] = 0;

  if (row > 0 && col > 0)
    if (board[row - 1][col - 1] == BOARD_ROBOT)
      valid[NOTHING] = valid[UP] = valid[LEFT_UP] = valid[LEFT] = 0;
    else if (board[row - 1][col - 1] == BOARD_JUNK)
      valid[LEFT_UP] = 0;

  if (col > 0)
    if (board[row][col - 1] == BOARD_ROBOT)
      valid[NOTHING] = valid[UP] = valid[LEFT_UP] = valid[LEFT]
	= valid[LEFT_DOWN] = valid[DOWN] = 0;
    else if (board[row][col - 1] == BOARD_JUNK)
      valid[LEFT] = 0;

  if (row < ROWS - 1 && col > 0)
    if (board[row + 1][col - 1] == BOARD_ROBOT)
      valid[NOTHING] = valid[LEFT] = valid[LEFT_DOWN] = valid[DOWN] = 0;
    else if (board[row + 1][col - 1] == BOARD_JUNK)
      valid[LEFT_DOWN] = 0;

  if (row < ROWS - 1)
    if (board[row + 1][col] == BOARD_ROBOT)
      valid[NOTHING] = valid[LEFT] = valid[LEFT_DOWN] = valid[DOWN]
	= valid[RIGHT_DOWN] = valid[RIGHT] = 0;
    else if (board[row + 1][col] == BOARD_JUNK)
      valid[DOWN] = 0;

  if (row < ROWS - 1 && col < COLS - 1)
    if (board[row + 1][col + 1] == BOARD_ROBOT)
      valid[NOTHING] = valid[DOWN] = valid[RIGHT_DOWN] = valid[RIGHT] = 0;
    else if (board[row + 1][col + 1] == BOARD_JUNK)
      valid[RIGHT_DOWN] = 0;

  if (col < COLS - 1)
    if (board[row][col + 1] == BOARD_ROBOT)
      valid[NOTHING] = valid[UP] = valid[DOWN] = valid[RIGHT_DOWN]
	= valid[RIGHT] = valid[RIGHT_UP] = 0;
    else if (board[row][col + 1] == BOARD_JUNK)
      valid[RIGHT] = 0;

  if (row > 0 && col < COLS - 1)
    if (board[row - 1][col + 1] == BOARD_ROBOT)
      valid[NOTHING] = valid[UP] = valid[RIGHT] = valid[RIGHT_UP] = 0;
    else if (board[row - 1][col + 1] == BOARD_JUNK)
      valid[RIGHT_UP] = 0;

  // 16 secondary neightbors
  if (row > 1
    && board[row - 2][col] == BOARD_ROBOT)
    valid[UP] = valid[LEFT_UP] = valid[RIGHT_UP] = 0;

  if (row > 1 && col > 0
    && board[row - 2][col - 1] == BOARD_ROBOT)
    valid[UP] = valid[LEFT_UP] = 0;

  if (row > 1 && col > 1
    && board[row - 2][col - 2] == BOARD_ROBOT)
    valid[LEFT_UP] = 0;

  if (row > 0 && col > 1
    && board[row - 1][col - 2] == BOARD_ROBOT)
    valid[LEFT_UP] = valid[LEFT] = 0;

  if (col > 1
    && board[row][col - 2] == BOARD_ROBOT)
    valid[LEFT_UP] = valid[LEFT] = valid[LEFT_DOWN] = 0;

  if (row < ROWS - 1 && col > 1
    && board[row + 1][col - 2] == BOARD_ROBOT)
    valid[LEFT] = valid[LEFT_DOWN] = 0;

  if (row < ROWS - 2 && col > 1
    && board[row + 2][col - 2] == BOARD_ROBOT)
    valid[LEFT_DOWN] = 0;

  if (row < ROWS - 2 && col > 0
    && board[row + 2][col - 1] == BOARD_ROBOT)
    valid[LEFT_DOWN] = valid[DOWN] = 0;

  if (row < ROWS - 2
    && board[row + 2][col] == BOARD_ROBOT)
    valid[LEFT_DOWN] = valid[DOWN] = valid[RIGHT_DOWN] = 0;

  if (row < ROWS - 2 && col < COLS - 1
    && board[row + 2][col + 1] == BOARD_ROBOT)
    valid[DOWN] = valid[RIGHT_DOWN] = 0;

  if (row < ROWS - 2 && col < COLS - 2
    && board[row + 2][col + 2] == BOARD_ROBOT)
    valid[RIGHT_DOWN] = 0;

  if (row < ROWS - 1 && col < COLS - 2
    && board[row + 1][col + 2] == BOARD_ROBOT)
    valid[RIGHT_DOWN] = valid[RIGHT] = 0;

  if (col < COLS - 2
    && board[row][col + 2] == BOARD_ROBOT)
    valid[RIGHT_DOWN] = valid[RIGHT] = valid[RIGHT_UP] = 0;

  if (row > 0 && col < COLS - 2
    && board[row - 1][col + 2] == BOARD_ROBOT)
    valid[RIGHT] = valid[RIGHT_UP] = 0;

  if (row > 1 && col < COLS - 2
    && board[row - 2][col + 2] == BOARD_ROBOT)
    valid[RIGHT_UP] = 0;

  if (row > 1 && col < COLS - 1
    && board[row - 2][col + 1] == BOARD_ROBOT)
    valid[UP] = valid[RIGHT_UP] = 0;

  return valid;
}

// <<<
//******************************************************************************
// >>> abovebelow

char abovebelow(const char* valid, int selfrow, int minrow, int maxrow,
  int selfcol, int avgcol0, int avgcol1)
{
  if (selfrow < minrow) // above
    if (selfcol < avgcol0)
    {
      if (valid[RIGHT_UP])
	return KEY_RIGHT_UP;
      if (valid[RIGHT])
	return KEY_RIGHT;
      if (valid[UP])
	return KEY_UP;
    }
    else if (selfcol > avgcol1)
    {
      if (valid[LEFT_UP])
	return KEY_LEFT_UP;
      if (valid[LEFT])
	return KEY_LEFT;
      if (valid[UP])
	return KEY_UP;
    }
    else // avgcol0 <= selfcol <= avgcol1
    {
      if (valid[UP])
	return KEY_UP;
      if (valid[LEFT_UP])
	return KEY_LEFT_UP;
      if (valid[RIGHT_UP])
	return KEY_RIGHT_UP;
    }
  else if (selfrow > maxrow) // below
    if (selfcol < avgcol0)
    {
      if (valid[RIGHT_DOWN])
	return KEY_RIGHT_DOWN;
      if (valid[RIGHT])
	return KEY_RIGHT;
      if (valid[DOWN])
	return KEY_DOWN;
    }
    else if (selfcol > avgcol1)
    {
      if (valid[LEFT_DOWN])
	return KEY_LEFT_DOWN;
      if (valid[LEFT])
	return KEY_LEFT;
      if (valid[DOWN])
	return KEY_DOWN;
    }
    else // avgcol0 <= selfcol <= avgcol1
    {
      if (valid[DOWN])
	return KEY_DOWN;
      if (valid[LEFT_DOWN])
	return KEY_LEFT_DOWN;
      if (valid[RIGHT_DOWN])
	return KEY_RIGHT_DOWN;
    }
  return NIL;
}

// <<<
//******************************************************************************
// >>> leftrightof

char leftrightof(const char* valid, int selfcol, int mincol, int maxcol,
  int selfrow, int avgrow0, int avgrow1)
{
      if (selfcol < mincol) // leftofit
      if (selfrow < avgrow0)
      {
	if (valid[LEFT_DOWN])
	  return KEY_LEFT_DOWN;
	if (valid[DOWN])
	  return KEY_DOWN;
	if (valid[LEFT])
	  return KEY_LEFT;
      }
      else if (selfrow > avgrow1)
      {
	if (valid[LEFT_UP])
	  return KEY_LEFT_UP;
	if (valid[UP])
	  return KEY_UP;
	if (valid[LEFT])
	  return KEY_LEFT;
      }
      else // avgrow0 <= selfrow <= avgrow1
      {
	if (valid[LEFT])
	  return KEY_LEFT;
	if (valid[LEFT_UP])
	  return KEY_LEFT_UP;
	if (valid[LEFT_DOWN])
	  return KEY_LEFT_DOWN;
      }
    else if (selfcol > maxcol) // rightofit
      if (selfrow < avgrow0)
      {
	if (valid[LEFT_DOWN])
	  return KEY_LEFT_DOWN;
	if (valid[DOWN])
	  return KEY_DOWN;
	if (valid[LEFT])
	  return KEY_LEFT;
      }
      else if (selfrow > avgrow1)
      {
	if (valid[LEFT_UP])
	  return KEY_LEFT_UP;
	if (valid[UP])
	  return KEY_UP;
	if (valid[LEFT])
	  return KEY_LEFT;
      }
      else // avgrow0 <= selfrow <= avgrow1
      {
	if (valid[LEFT])
	  return KEY_LEFT;
	if (valid[LEFT_UP])
	  return KEY_LEFT_UP;
	if (valid[LEFT_DOWN])
	  return KEY_LEFT_DOWN;
      }
  return NIL;
}

// <<<
//******************************************************************************
// >>> junkrow

char junkrow(const char* const* board, const char* valid,
  int selfrow, int selfcol, int robrow, int robcol)
{
  int num = COLS; 
  char key = NIL;

  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
      if (board[row][col] == BOARD_JUNK)
      {
	if (selfcol < robcol - 1 && col < robcol)
	{
	  int robnum = robcol - (col + 1);
	  if (abs(robrow - row) <= robnum)
	  {
	    if (selfcol < col)
	    {
	      if (selfrow < row)
	      {
		int selfnum = row - selfrow;
		if (selfnum <= robnum && selfnum < num)
		  if (selfcol < col - 1 && valid[RIGHT_DOWN])
		  {
		    num = selfnum;
		    key = KEY_RIGHT_DOWN;
		  }
		  else if (valid[DOWN])
		  {
		    num = selfnum;
		    key = KEY_DOWN;
		  }
	      }
	      else if (selfrow > row)
	      {
		int selfnum = selfrow - row;
		if (selfnum <= robnum && selfnum < num)
		  if (selfcol < col - 1 && valid[RIGHT_UP])
		  {
		    num = selfnum;
		    key = KEY_RIGHT_UP;
		  }
		  else if (valid[UP])
		  {
		    num = selfnum;
		    key = KEY_UP;
		  }
	      }
	      else // selfrow == row
		return KEY_WAIT;
	    }
	    else // selfcol >= col
	    {
	      int selfnumcol = selfcol - (col - 1);
	      if (selfrow < row)
	      {
		int selfnumrow = row - selfrow;
		if (selfnumcol > selfnumrow)
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[LEFT])
		  {
		    num = selfnumcol;
		    key = KEY_LEFT;
		  }
		}
		else // selfnumcol <= selfnumrow
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[LEFT_DOWN])
		  {
		    num = selfnumrow;
		    key = KEY_LEFT_DOWN;
		  }
		}
	      }
	      else if (selfrow > row)
	      {
		int selfnumrow = selfrow - row;
		if (selfnumcol > selfnumrow)
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[LEFT])
		  {
		    num = selfnumcol;
		    key = KEY_LEFT;
		  }
		}
		else // selfnumcol <= selfnumrow
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[LEFT_UP])
		  {
		    num = selfnumrow;
		    key = KEY_LEFT_UP;
		  }
		}
	      }
	      else // selfrow == row
	      {
		int selfnum = selfcol - (col - 1);
		if (selfnum <= robnum && selfnum < num)
		{
		  if (valid[LEFT_UP])
		  {
		    num = selfnum;
		    key = KEY_LEFT_UP;
		  }
		  else if (valid[LEFT_DOWN])
		  {
		    num = selfnum;
		    key = KEY_LEFT_DOWN;
		  }
		}
	      } 
	    }
	  }
	}
	else if (selfcol > robcol + 1 && col > robcol)
	{
	  int robnum = (col - 1) - robcol;
	  if (abs(robrow - row) <= robnum)
	  {
	    if (selfcol > col)
	    {
	      if (selfrow < row)
	      {
		int selfnum = row - selfrow;
		if (selfnum <= robnum && selfnum < num)
		  if (selfcol > col + 1 && valid[LEFT_DOWN])
		  {
		    num = selfnum;
		    key = KEY_LEFT_DOWN;
		  }
		  else if (valid[DOWN])
		  {
		    num = selfnum;
		    key = KEY_DOWN;
		  }
	      }
	      else if (selfrow > row)
	      {
		int selfnum = selfrow - row;
		if (selfnum <= robnum && selfnum < num)
		  if (selfcol > col + 1 && valid[LEFT_UP])
		  {
		    num = selfnum;
		    key = KEY_LEFT_UP;
		  }
		  else if (valid[UP])
		  {
		    num = selfnum;
		    key = KEY_UP;
		  }
	      }
	      else // selfrow == row
		return KEY_WAIT;
	    }
	    else // selfcol <= col
	    {
	      int selfnumcol = (col + 1) - selfcol;
	      if (selfrow < row)
	      {
		int selfnumrow = row - selfrow;
		if (selfnumcol > selfnumrow)
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[RIGHT])
		  {
		    num = selfnumcol;
		    key = KEY_RIGHT;
		  }
		}
		else // selfnumcol <= selfnumrow
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[RIGHT_DOWN])
		  {
		    num = selfnumrow;
		    key = KEY_RIGHT_DOWN;
		  }
		}
	      }
	      else if (selfrow > row)
	      {
		int selfnumrow = selfrow - row;
		if (selfnumcol > selfnumrow)
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[RIGHT])
		  {
		    num = selfnumcol;
		    key = KEY_RIGHT;
		  }
		}
		else // selfnumcol <= selfnumrow
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[RIGHT_UP])
		  {
		    num = selfnumrow;
		    key = KEY_RIGHT_UP;
		  }
		}
	      }
	      else // selfrow == row
	      {
		int selfnum = (col + 1) - selfcol;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (valid[RIGHT_UP])
		  {
		    num = selfnum;
		    key = KEY_RIGHT_UP;
		  }
		  else if (valid[RIGHT_DOWN])
		  {
		    num = selfnum;
		    key = KEY_RIGHT_DOWN;
		  }
		}
	      }
	    }
	  }
	}
      }

  if (num < COLS)
    return key;

  return NIL;
}

// <<<
//******************************************************************************
// >>> junkcol

char junkcol(const char* const* board, const char* valid,
  int selfrow, int selfcol, int robrow, int robcol)
{
  int num = COLS; 
  char key = NIL;

  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
      if (board[row][col] == BOARD_JUNK)
      {
	if (selfrow < robrow - 1 && row < robrow)
	{
	  int robnum = robrow - (row + 1);
	  if (abs(robcol - col) <= robnum)
	  {
	    if (selfrow < row)
	    {
	      if (selfcol < col)
	      {
		int selfnum = col - selfcol;
		if (selfnum <= robnum && selfnum < num)
		  if (selfrow < row - 1 && valid[RIGHT_DOWN])
		  {
		    num = selfnum;
		    key = KEY_RIGHT_DOWN;
		  }
		  else if (valid[RIGHT])
		  {
		    num = selfnum;
		    key = KEY_RIGHT;
		  }
	      }
	      else if (selfcol > col)
	      {
		int selfnum = selfcol - col;
		if (selfnum <= robnum && selfnum < num)
		  if (selfrow < row - 1 && valid[LEFT])
		  {
		    num = selfnum;
		    key = KEY_LEFT_DOWN;
		  }
		  else if (valid[LEFT])
		  {
		    num = selfnum;
		    key = KEY_LEFT;
		  }
	      }
	      else // selfcol == col
		return KEY_WAIT;
	    }
	    else // selfrow >= row
	    {
	      int selfnumrow = selfrow - (row - 1);
	      if (selfcol < col)
	      {
		int selfnumcol = col - selfcol;
		if (selfnumrow > selfnumcol)
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[UP])
		  {
		    num = selfnumrow;
		    key = KEY_UP;
		  }
		}
		else // selfnumrow <= selfnumcol
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[RIGHT_UP])
		  {
		    num = selfnumcol;
		    key = KEY_RIGHT_UP;
		  }
		}
	      }
	      else if (selfcol > col)
	      {
		int selfnumcol = selfcol - col;
		if (selfnumrow > selfnumcol)
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[UP])
		  {
		    num = selfnumrow;
		    key = KEY_UP;
		  }
		}
		else // selfnumrow <= selfnumcol
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[LEFT_UP])
		  {
		    num = selfnumcol;
		    key = KEY_LEFT_UP;
		  }
		}
	      }
	      else // selfcol == col
	      {
		int selfnum = selfrow - (row - 1);
		if (selfnum <= robnum && selfnum < num)
		{
		  if (valid[LEFT_UP])
		  {
		    num = selfnum;
		    key = KEY_LEFT_UP;
		  }
		  else if (valid[RIGHT_UP])
		  {
		    num = selfnum;
		    key = KEY_RIGHT_UP;
		  }
		}
	      } 
	    }
	  }
	}
	else if (selfrow > robrow + 1 && row > robrow)
	{
	  int robnum = (row - 1) - robrow;
	  if (abs(robcol - col) <= robnum)
	  {
	    if (selfrow > row)
	    {
	      if (selfcol < col)
	      {
		int selfnum = col - selfcol;
		if (selfnum <= robnum && selfnum < num)
		  if (selfrow > row + 1 && valid[RIGHT_UP])
		  {
		    num = selfnum;
		    key = KEY_RIGHT_UP;
		  }
		  else if (valid[RIGHT])
		  {
		    num = selfnum;
		    key = KEY_RIGHT;
		  }
	      }
	      else if (selfcol > col)
	      {
		int selfnum = selfcol - col;
		if (selfnum <= robnum && selfnum < num)
		  if (selfrow > row + 1 && valid[LEFT_UP])
		  {
		    num = selfnum;
		    key = KEY_LEFT_UP;
		  }
		  else if (valid[LEFT])
		  {
		    num = selfnum;
		    key = KEY_LEFT;
		  }
	      }
	      else // selfcol == col
		return KEY_WAIT;
	    }
	    else // selfrow <= row
	    {
	      int selfnumrow = (row + 1) - selfrow;
	      if (selfcol < col)
	      {
		int selfnumcol = col - selfcol;
		if (selfnumrow > selfnumcol)
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[DOWN])
		  {
		    num = selfnumrow;
		    key = KEY_DOWN;
		  }
		}
		else // selfnumrow <= selfnumcol
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[RIGHT_DOWN])
		  {
		    num = selfnumcol;
		    key = KEY_RIGHT_DOWN;
		  }
		}
	      }
	      else if (selfcol > col)
	      {
		int selfnumcol = selfcol - col;
		if (selfnumrow > selfnumcol)
		{
		  if (selfnumrow <= robnum && selfnumrow < num && valid[DOWN])
		  {
		    num = selfnumrow;
		    key = KEY_DOWN;
		  }
		}
		else // selfnumrow <= selfnumcol
		{
		  if (selfnumcol <= robnum && selfnumcol < num && valid[LEFT_DOWN])
		  {
		    num = selfnumcol;
		    key = KEY_LEFT_DOWN;
		  }
		}
	      }
	      else // selfcol == col
	      {
		int selfnum = (row + 1) - selfrow;
		if (selfnum <= robnum && selfnum < num)
		{
		  if (valid[RIGHT_UP])
		  {
		    num = selfnum;
		    key = KEY_RIGHT_DOWN;
		  }
		  else if (valid[RIGHT_DOWN])
		  {
		    num = selfnum;
		    key = KEY_LEFT_DOWN;
		  }
		}
	      }
	    }
	  }
	}
      }

  if (num < COLS)
    return key;

  return NIL;
}

// <<<
//******************************************************************************
// >>> junkesc

char junkesc(const char* const* board, const char* valid,
  int selfrow, int selfcol, int robrow, int robcol)
{
  int junkrow = 0;
  int junknum = 0;
  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
      if (board[row][col] == BOARD_JUNK)
      {
	junkrow += row;
	junknum++;
      }
  junkrow /= junknum;

  char key = NIL;
  if (selfcol <= robcol)
    if (selfrow < junkrow)
    {
      if (valid[LEFT_DOWN])
	key = KEY_LEFT_DOWN;
      else if (valid[LEFT])
	key = KEY_LEFT;
    }
    else if (selfrow > junkrow)
    {
      if (valid[LEFT_UP])
	key = KEY_LEFT_UP;
      else if (valid[LEFT])
	key = KEY_LEFT;
    }
    else // selfrow == junkrow
    {
      if (valid[LEFT])
	key = KEY_LEFT;
      else if (valid[LEFT_UP])
	key = KEY_LEFT_UP;
      else if (valid[LEFT_DOWN])
	key = KEY_LEFT_DOWN;
    }
  else if (selfcol > robcol)
    if (selfrow < junkrow)
    {
      if (valid[RIGHT_DOWN])
	key = KEY_RIGHT_DOWN;
      else if (valid[RIGHT])
	key = KEY_RIGHT;
    }
    else if (selfrow > junkrow)
    {
      if (valid[RIGHT_UP])
	key = KEY_RIGHT_UP;
      else if (valid[RIGHT])
	key = KEY_RIGHT;
    }
    else // selfrow == junkrow
    {
      if (valid[RIGHT])
	key = KEY_RIGHT;
      else if (valid[RIGHT_UP])
	key = KEY_RIGHT_UP;
      else if (valid[RIGHT_DOWN])
	key = KEY_RIGHT_DOWN;
    }
  return key;
}

// <<<
//******************************************************************************
// >>> inbetween

char inbetween(const char* valid, int selfrow, int avgrow0, int avgrow1,
  int selfcol, int avgcol0, int avgcol1, int minrow, int maxrow, int mincol, int maxcol)
{
  if (selfcol < avgcol0)
    if (selfrow < avgrow0)
    {
      if (valid[RIGHT_DOWN])
	return KEY_RIGHT_DOWN;
      if (valid[RIGHT])
	return KEY_RIGHT;
      if (valid[DOWN])
	return KEY_DOWN;
    }
    else if (selfrow > avgrow1)
    {
      if (valid[RIGHT_UP])
	return KEY_RIGHT_UP;
      if (valid[RIGHT])
	return KEY_RIGHT;
      if (valid[UP])
	return KEY_UP;
    }
    else // avgrow0 <= selfrow <= avgrow1
    {
      if (valid[RIGHT])
	return KEY_RIGHT;
      if (valid[RIGHT_UP])
	return KEY_RIGHT_UP;
      if (valid[RIGHT_DOWN])
	return KEY_RIGHT_DOWN;
    }
  else if (selfcol > avgcol1)
    if (selfrow < avgrow0)
    {
      if (valid[LEFT_DOWN])
	return KEY_LEFT_DOWN;
      if (valid[LEFT])
	return KEY_LEFT;
      if (valid[DOWN])
	return KEY_DOWN;
    }
    else if (selfrow > avgrow1)
    {
      if (valid[LEFT_UP])
	return KEY_LEFT_UP;
      if (valid[LEFT])
	return KEY_LEFT;
      if (valid[UP])
	return KEY_UP;
    }
    else // avgrow0 <= selfrow <= avgrow1
    {
      if (valid[LEFT])
	return KEY_LEFT;
      if (valid[LEFT_UP])
	return KEY_LEFT_UP;
      if (valid[LEFT_DOWN])
	return KEY_LEFT_DOWN;
    }
  else // avgcol0 <= selfcol <= avgcol1
    if (selfrow < avgrow0)
    {
      if (valid[DOWN])
	return KEY_DOWN;
      if (valid[LEFT_DOWN])
	return KEY_LEFT_DOWN;
      if (valid[RIGHT_DOWN])
	return KEY_RIGHT_DOWN;
    }
    else if (selfrow > avgrow1)
    {
      if (valid[UP])
	return KEY_UP;
      if (valid[LEFT_UP])
	return KEY_LEFT_UP;
      if (valid[RIGHT_UP])
	return KEY_RIGHT_UP;
    }

  if (selfcol <= avgcol1)
  {
    if (valid[RIGHT])
      return KEY_RIGHT;
    if (valid[RIGHT_UP])
      return KEY_RIGHT_UP;
    if (valid[RIGHT_DOWN])
      return KEY_RIGHT_DOWN;
  }
  else if (selfcol >= avgcol0)
  {
    if (valid[LEFT])
      return KEY_LEFT;
    if (valid[LEFT_UP])
      return KEY_LEFT_UP;
    if (valid[LEFT_DOWN])
      return KEY_LEFT_DOWN;
  }

  if (selfrow <= avgrow1)
  {
    if (valid[DOWN])
      return KEY_DOWN;
    if (valid[LEFT_DOWN])
      return KEY_LEFT_DOWN;
    if (valid[RIGHT_DOWN])
      return KEY_RIGHT_DOWN;
  }
  else if (selfrow >= avgrow0)
  {
    if (valid[UP])
      return KEY_UP;
    if (valid[LEFT_UP])
      return KEY_LEFT_UP;
    if (valid[RIGHT_UP])
      return KEY_RIGHT_UP;
  }

  if (selfcol <= maxcol)
  {
    if (valid[RIGHT])
      return KEY_RIGHT;
    if (valid[RIGHT_UP])
      return KEY_RIGHT_UP;
    if (valid[RIGHT_DOWN])
      return KEY_RIGHT_DOWN;
  }
  else if (selfcol >= mincol)
  {
    if (valid[LEFT])
      return KEY_LEFT;
    if (valid[LEFT_UP])
      return KEY_LEFT_UP;
    if (valid[LEFT_DOWN])
      return KEY_LEFT_DOWN;
  }

  if (selfrow <= maxrow)
  {
    if (valid[DOWN])
      return KEY_DOWN;
    if (valid[LEFT_DOWN])
      return KEY_LEFT_DOWN;
    if (valid[RIGHT_DOWN])
      return KEY_RIGHT_DOWN;
  }
  else if (selfrow >= minrow)
  {
    if (valid[UP])
      return KEY_UP;
    if (valid[LEFT_UP])
      return KEY_LEFT_UP;
    if (valid[RIGHT_UP])
      return KEY_RIGHT_UP;
  }

  return NIL;
}

// <<<
//******************************************************************************
// >>> strategy

char strategy(const char* const* board)
{
  int selfrow = 0, selfcol = 0;
  int minrow = ROWS, mincol = COLS;
  int maxrow = 0, maxcol = 0;

  for (int row = 0; row < ROWS; row++)
    for (int col = 0; col < COLS; col++)
    {
      if (board[row][col] == BOARD_SELF)
      {
	selfrow = row;
	selfcol = col;
      }
      else if (board[row][col] == BOARD_ROBOT)
      {
	if (row > maxrow)
	  maxrow = row;
	if (row < minrow)
	  minrow =  row;
	if (col > maxcol)
	  maxcol = col;
	if (col < mincol)
	  mincol =  col;
      }
    }

  int avgrow0 = (maxrow + minrow) / 2;
  int avgrow1 = (maxrow + minrow + 1) / 2;
  int avgcol0 = (maxcol + mincol) / 2;
  int avgcol1 = (maxcol + mincol + 1) / 2;

  const char* valid = validity(board, selfrow, selfcol);

  if (minrow == maxrow && mincol == maxcol) // one robot
  {
    char key = junkrow(board, valid, selfrow, selfcol, minrow, mincol);
    if (key)
      return key;

    key = junkcol(board, valid, selfrow, selfcol, minrow, mincol);
    if (key)
      return key;

    key = junkesc(board, valid, selfrow, selfcol, minrow, mincol);
    if (key)
      return key;
  }
  else if (minrow == maxrow && mincol < maxcol) // one row of robots
  {
    if (selfcol < mincol)
    {
      char key = junkrow(board, valid, selfrow, selfcol, minrow, mincol);
      if (key)
	return key;
    }
    else if (selfcol > maxcol)
    {
      char key = junkrow(board, valid, selfrow, selfcol, maxrow, maxcol);
      if (key)
	return key;
    }

    char key = abovebelow(valid, selfrow, minrow, maxrow, selfcol, avgcol0, avgcol1);
    if (key)
      return key;

    if (selfcol > mincol && selfcol < maxcol)
    {
      if (valid[LEFT])
	return KEY_LEFT;
      if (valid[RIGHT])
	return KEY_RIGHT;
      if (valid[LEFT_UP])
	return KEY_LEFT_UP;
      if (valid[LEFT_DOWN])
	return KEY_LEFT_DOWN;
      if (valid[RIGHT_UP])
	return KEY_RIGHT_UP;
      if (valid[RIGHT_DOWN])
	return KEY_RIGHT_DOWN;
    }
  }
  else if (minrow < maxrow && mincol == maxcol) // one column of robots
  {
    if (selfrow < minrow)
    {
      char key = junkcol(board, valid, selfrow, selfcol, minrow, mincol);
      if (key)
	return key;
    }
    else if (selfrow > maxcol)
    {
      char key = junkcol(board, valid, selfrow, selfcol, maxrow, maxcol);
      if (key)
	return key;
    }

    char key = leftrightof(valid, selfcol, mincol, maxcol, selfrow, avgrow0, avgrow1);
    if (key)
      return key;

    if (selfrow > minrow && selfrow < maxrow)
    {
      if (valid[UP])
	return KEY_UP;
      if (valid[DOWN])
	return KEY_DOWN;
      if (valid[LEFT_UP])
	return KEY_LEFT_UP;
      if (valid[RIGHT_UP])
	return KEY_RIGHT_UP;
      if (valid[LEFT_DOWN])
	return KEY_LEFT_DOWN;
      if (valid[RIGHT_DOWN])
	return KEY_RIGHT_DOWN;
    }
  }
  else // spread out
  {
    char key = leftrightof(valid, selfcol, mincol, maxcol, selfrow, avgrow0, avgrow1);
    if (key)
      return key;

    key = abovebelow(valid, selfrow, minrow, maxrow, selfcol, avgcol0, avgcol1);
    if (key)
      return key;

    key = inbetween(valid, selfrow, avgrow0, avgrow1, selfcol, avgcol0, avgcol1,
      minrow, maxrow, mincol, maxcol);
    if (key)
      return key;
  }
   
  if (valid[NOTHING])
    return KEY_NOTHING;

  return KEY_TELEPORT;
}

// <<<
//******************************************************************************
