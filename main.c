#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 800

typedef enum
{
    NONE = 0,
    RIGHT,
    DOWN,
    LEFT,
    UP
} Direction;

typedef enum
{
    BACKGROUND_GRAY,
    BACKGROUND_BEIGE,
    BACKROUND_LIGHT_ORANGE,
    BACKGROUND_ORANGE,
    BACKGROUND_LIGTH_RED,
    BACKGROUND_RED,
    BACKGROUND_YELLOW,
} BackgroundColors;

static int grid[4][4];
static bool has_combined[4][4];
static Color background_colors[BACKGROUND_YELLOW + 1];

const int grid_rows = 4;
const int grid_cols = 4;
static Vector2 dirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

void place_random();
void display_grid();
void move_tiles(Direction dir);
void reset_combined();
bool can_move(Direction dir);

// UI
void draw_grid();

int main()
{
    srandom(time(NULL));
    InitWindow(WIDTH, HEIGHT, "2048");
    place_random();
    while (!WindowShouldClose())
    {
        int key = GetKeyPressed();
        Direction dir = NONE;

        switch (key)
        {
            case 'W': dir = UP; break;
            case 'A': dir = LEFT; break;
            case 'S': dir = DOWN; break;
            case 'D': dir = RIGHT; break;
            default: break;
        }

        if (can_move(dir))
        {
            move_tiles(dir);
            reset_combined();
            place_random();
        }

        BeginDrawing();
        draw_grid();
        EndDrawing();
    }

    CloseWindow();
    return 0;

    // Terminal version
    // bool playing = true;
    // place_random();
    // while (playing)
    // {
    //     printf("\e[1;1H\e[2J");

    //     display_grid();

    //     char input;
    //     scanf(" %c", &input);
    //     Direction dir = NONE;

    //     switch (input)
    //     {
    //         case 'W': dir = UP; break;
    //         case 'A': dir = LEFT; break;
    //         case 'S': dir = DOWN; break;
    //         case 'D': dir = RIGHT; break;
    //         default: break;
    //     }

    //     if (can_move(dir))
    //     {
    //         move_tiles(dir);
    //         reset_combined();
    //         place_random();
    //     }
    // }
    // return 0;
}

void place_random()
{
    do
    {
        int x = rand() % 4;
        int y = rand() % 4;

        if (grid[x][y] == 0) // if tile is empty
        {
            grid[x][y] = rand() % 5 == 0 ? 4 : 2; // tile either get a 2 or 4
            break;
        }
    } while (1);
}

void display_grid()
{
    for (int i = 0; i < grid_rows; ++i)
    {
        for (int j = 0; j < grid_cols; ++j)
        {
            if (grid[i][j] == 0) printf("  ");
            else printf("%d ", grid[i][j]);
        }
        printf("\n");
    }
}

void move_tiles(Direction dir)
{
    switch (dir)
    {
        case RIGHT:
            for (int i = 0; i < grid_rows; ++i)
            {
                for (int j = grid_cols - 1; j >= 0; --j)
                {
                    while (j >= 0 && j + 1 < grid_cols)
                    {
                        if (grid[i][j] == 0)
                        {
                            --j;
                            continue;
                        }

                        if (grid[i][j] == grid[i][j + 1] && !has_combined[i][j + 1]) // if they are equal, combine them;
                        {
                            grid[i][j + 1] = grid[i][j] + grid[i][j + 1];
                            grid[i][j] = 0;
                            has_combined[i][j + 1] = true; // mark that this tile has combined for the curr turn.
                            --j;
                        }
                        else
                        {
                            if (grid[i][j + 1] == 0) // if right tile is empty move curr tile
                            {
                                grid[i][j + 1] = grid[i][j];
                                grid[i][j] = 0;
                                ++j;
                            }
                            else --j;
                        }
                    }
                }
            }
            break;
        case DOWN:
            for (int j = 0; j < grid_cols; ++j)
            {
                for (int i = grid_rows - 1; i >= 0; --i)
                {
                    while (i >= 0 && i + 1 < grid_rows)
                    {
                        if (grid[i][j] == 0)
                        {
                            --i;
                            continue;
                        }

                        if (grid[i][j] == grid[i + 1][j] && !has_combined[i + 1][j]) // if they are equal, combine them;
                        {
                            grid[i + 1][j] = grid[i][j] + grid[i + 1][j];
                            grid[i][j] = 0;
                            has_combined[i + 1][j] = true; // mark that this tile has combined for the curr turn.
                            --i;
                        }
                        else
                        {
                            if (grid[i + 1][j] == 0) // if down tile is empty move curr tile
                            {
                                grid[i + 1][j] = grid[i][j];
                                grid[i][j] = 0;
                                ++i;
                            }
                            else --i;
                        }
                    }
                }
            }
            break;
        case LEFT:
            for (int i = 0; i < grid_rows; ++i)
            {
                for (int j = 0; j < grid_cols; ++j)
                {
                    while (j < grid_cols && j - 1 >= 0)
                    {
                        if (grid[i][j] == 0)
                        {
                            ++j;
                            continue;
                        }

                        if (grid[i][j] == grid[i][j - 1] && !has_combined[i][j - 1]) // if they are equal, combine them;
                        {
                            grid[i][j - 1] = grid[i][j] + grid[i][j - 1];
                            grid[i][j] = 0;
                            has_combined[i][j - 1] = true; // mark that this tile has combined for the curr turn.
                            ++j;
                        }
                        else
                        {
                            if (grid[i][j - 1] == 0) // if left tile is empty move curr tile
                            {
                                grid[i][j - 1] = grid[i][j];
                                grid[i][j] = 0;
                                --j;
                            }
                            else ++j;
                        }
                    }
                }
            }
            break;
        case UP:
            for (int j = 0; j < grid_cols; ++j)
            {
                for (int i = 0; i < grid_rows; ++i)
                {
                    while (i < grid_rows && i - 1 >= 0)
                    {
                        if (grid[i][j] == 0)
                        {
                            ++i;
                            continue;
                        }

                        if (grid[i][j] == grid[i - 1][j] && !has_combined[i - 1][j]) // if they are equal, combine them;
                        {
                            grid[i - 1][j] = grid[i][j] + grid[i - 1][j];
                            grid[i][j] = 0;
                            has_combined[i - 1][j] = true; // mark that this tile has combined for the curr turn.
                            ++i;
                        }
                        else
                        {
                            if (grid[i - 1][j] == 0) // if up tile is empty move curr tile
                            {
                                grid[i - 1][j] = grid[i][j];
                                grid[i][j] = 0;
                                --i;
                            }
                            else ++i;
                        }
                    }
                }
            }
            break;
        case NONE: return;
        default: return;
    }
}

void reset_combined()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            has_combined[i][j] = false;
        }
    }
}

bool can_move(Direction dir)
{
    switch (dir)
    {
        case RIGHT:
            for (int i = 0; i < grid_rows; ++i)
            {
                for (int j = grid_cols - 1; j >= 0; --j)
                {
                    if (grid[i][j] == 0) continue;
                    if (j + 1 < grid_cols && grid[i][j + 1] == 0 || grid[i][j] == grid[i][j + 1]) return true;
                }
            }
            return false;
            break;
        case DOWN:
            for (int j = 0; j < grid_cols; ++j)
            {
                for (int i = grid_rows - 1; i >= 0; --i)
                {
                    if (grid[i][j] == 0) continue;
                    if (i + 1 < grid_rows && grid[i + 1][j] == 0 || grid[i][j] == grid[i + 1][j]) return true;
                }
            }
            return false;
            break;
        case LEFT:
            for (int i = 0; i < grid_rows; ++i)
            {
                for (int j = 0; j < grid_cols; ++j)
                {
                    if (grid[i][j] == 0) continue;
                    if (j - 1 >= 0 && grid[i][j - 1] == 0 || grid[i][j] == grid[i][j - 1]) return true;
                }
            }
            return false;
            break;
        case UP:
            for (int j = 0; j < grid_cols; ++j)
            {
                for (int i = 0; i < grid_rows; ++i)
                {
                    if (grid[i][j] == 0) continue;
                    if (i - 1 >= 0 && grid[i - 1][j] == 0 || grid[i][j] == grid[i - 1][j]) return true;
                }
            }
            return false;
            break;
        case NONE: return false;
        default: return false;
    }
}

void draw_grid()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            Rectangle rec = {j * 200 + 10, i * 200 + 10, 180, 180};
            DrawRectangleRounded(rec, 0.20f, 20, WHITE);
            if (grid[i][j] != 0)
            {
                const char* txt = TextFormat("%d", grid[i][j]);
                DrawText(txt, rec.x + (rec.width / 2), rec.y + (rec.height / 2), 64, BLUE);
            }
        }
    }
}
