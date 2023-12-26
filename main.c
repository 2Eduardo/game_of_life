#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

struct point
{
  int x, y;
};

struct cell
{
  struct point position;
  int body;
  bool live;
};

void
print_grid (size_t len, const struct cell grid[][len])
{
  for (size_t i = 0; i < len; i++)
    {
      for (size_t j = 0; j < len; j++)
        {
          const struct cell *cell = &grid[i][j];
          if (cell->live)
            printf ("%c", cell->body);
          else
            printf (" ");
        }
      puts ("");
    }
}

bool
random_boolean ()
{
  return rand () % 2 == 0;
}

void
init_grid (size_t len, struct cell grid[][len])
{
  for (size_t i = 0; i < len; i++)
    {
      for (size_t j = 0; j < len; j++)
        {
          struct cell *cell = &grid[i][j];
          cell->position.y = i;
          cell->position.x = j;
          cell->body = 'c';
          cell->live = random_boolean ();
        }
    }
}

void
apply (size_t len, const struct cell grid[][len], struct cell *cell)
{
  struct point pos = cell->position;
  int living_neighbors = 0;

  int x_right = pos.x + 1;
  int x_left = pos.x - 1;
  int y_down = pos.y + 1;
  int y_up = pos.y - 1;

  bool has_right_neigh = x_right < len;
  bool has_left_neigh = x_left >= 0;
  bool has_down_neigh = y_down < len;
  bool has_up_neigh = y_up >= 0;

  if (has_up_neigh)
    {
      const struct cell *up_neigh = &grid[y_up][pos.x];
      if (up_neigh->live)
        living_neighbors++;
    }

  if (has_down_neigh)
    {
      const struct cell *down_neigh = &grid[y_down][pos.x];
      if (down_neigh->live)
        living_neighbors++;
    }

  if (has_right_neigh)
    {
      const struct cell *right_neigh = &grid[pos.y][x_right];
      if (right_neigh->live)
        living_neighbors++;

      if (has_up_neigh)
        {
          const struct cell *right_up_neigh = &grid[y_up][x_right];
          if (right_up_neigh->live)
            living_neighbors++;
        }

      if (has_down_neigh)
        {
          const struct cell *right_down_neigh = &grid[y_down][x_right];
          if (right_down_neigh->live)
            living_neighbors++;
        }
    }

  if (has_left_neigh)
    {
      const struct cell *left_neigh = &grid[pos.y][x_left];
      if (left_neigh->live)
        living_neighbors++;

      if (has_up_neigh)
        {
          const struct cell *left_up_neigh = &grid[y_up][x_left];
          if (left_up_neigh->live)
            living_neighbors++;
        }

      if (has_down_neigh)
        {
          const struct cell *left_down_neigh = &grid[y_down][x_left];
          if (left_down_neigh->live)
            living_neighbors++;
        }
    }

  // Rules: 1 and 3
  if (living_neighbors < 2 || living_neighbors > 3)
    cell->live = false;

  // Rules: 2 and 4
  if (living_neighbors == 2 || living_neighbors == 3
      || (!cell->live && living_neighbors == 3))
    cell->live = true;
}

int
main (int argc, char const *argv[])
{
  const int N = 10;
  struct cell grid[N][N];

  srand (time (NULL));
  init_grid (N, grid);
  print_grid (N, grid);

  for (int i = 0; i < 10; i++)
    {
      for (size_t i = 0; i < N; i++)
        {
          for (size_t j = 0; j < N; j++)
            apply (N, grid, &grid[i][j]);
        }
      printf ("\nAfter Iteration %d\n", i + 1);
      print_grid (N, grid);
    }

  return 0;
}
