#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 800

#define TXT_FONTSIZE 64
#define TXT_FONTSIZE_SMALL 48

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
    BACKGROUND_GRAY = 0,
    BACKGROUND_WHITE,
    BACKGROUND_BEIGE,
    BACKROUND_LIGHT_ORANGE,
    BACKGROUND_ORANGE,
    BACKGROUND_LIGTH_RED,
    BACKGROUND_RED,
    BACKGROUND_YELLOW,
    BACKGROUND_YELLOW_BRIGHT,
    BACKGROUND_BLACK,
} BackgroundColors; // background of each individual square

typedef enum
{
	GAMESTATUS_NONE = 0,
	GAMESTATUS_PLAYING,
	GAMESTATUS_GAMEOVER
} GameStatus;

static int grid[4][4];
static bool has_combined[4][4];
static Color background_colors[BACKGROUND_BLACK + 1] = {
    {0x44, 0x44, 0x44, 0x59}, // 0
    {0xfa, 0xf8, 0xef, 0xff}, // 2
    {0xed, 0xe0, 0xc8, 0xff}, // 4
    {0xf2, 0xb1, 0x79, 0xff}, // 8
    {0xf5, 0x95, 0x63, 0xff}, // 16
    {0xf6, 0x7c, 0x5f, 0xff}, // 32
    {0xf6, 0x5e, 0x3b, 0xff}, // 64
    {0xed, 0xcf, 0x72, 0xff}, // 128
    {0xed, 0xcc, 0x61, 0xff}, // 256
    {0x00, 0x00, 0x00, 0xff}, // > 256
};

const int grid_rows = 4;
const int grid_cols = 4;
// static Vector2 dirs[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

void place_random();
void display_grid();
void move_tiles(Direction dir);
void reset_combined(); // resets has_combined array
bool can_move(Direction dir);
bool is_full(); // checks if grid is full

// UI
void draw_grid();
void draw_end_screen();

int main()
{
    srandom(time(NULL));
    InitWindow(WIDTH, HEIGHT, "2048");
    SetTargetFPS(60);

    GameStatus status = GAMESTATUS_PLAYING;

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
        else
        {
       		if (is_full()) // if you can't move and it is full, you lose
         	{
          		status = GAMESTATUS_GAMEOVER;
          	}
        }

        BeginDrawing();

        switch (status)
        {
        	case GAMESTATUS_PLAYING: draw_grid(); break;
         	case GAMESTATUS_GAMEOVER: ClearBackground((Color){ 0, 255, 0, 0});draw_end_screen(); break;
          	case GAMESTATUS_NONE: break;
          	default: break;
        }

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
                    if ((j + 1 < grid_cols && grid[i][j + 1] == 0) || grid[i][j] == grid[i][j + 1]) return true;
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
                    if ((i + 1 < grid_rows && grid[i + 1][j] == 0) ||grid[i][j] == grid[i + 1][j]) return true;
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
                    if ((j - 1 >= 0 && grid[i][j - 1] == 0) || grid[i][j] == grid[i][j - 1]) return true;
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
                    if ((i - 1 >= 0 && grid[i - 1][j] == 0) || grid[i][j] == grid[i - 1][j]) return true;
                }
            }
            return false;
            break;
        case NONE: return false;
        default: return false;
    }
}

bool is_full()
{
	for (int i = 0; i < grid_rows; ++i)
	{
		for (int j = 0; j < grid_cols; ++j)
		{
			if (grid[i][j] == 0)
			{
				return false;
			}
		}
	}
	return true;
}

void draw_grid()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            int n = grid[i][j];
            Rectangle rec = {j * 200 + 10, i * 200 + 10, 180, 180};
            int idx = n == 0 ? 0 : (n > 256 ? BACKGROUND_BLACK : (int)log(n)); // used to map n to idx in background_colors array
            Color background = background_colors[idx];

            DrawRectangleRounded(rec, 0.20f, 20, background);
            if (n != 0)
            {
                const char* txt = TextFormat("%d", n);
                unsigned int txt_length = MeasureText(txt, TXT_FONTSIZE);
                Color txt_color = (background.r + background.g + background.b) > 382 ? BLACK : WHITE;

                DrawText(txt, rec.x + (rec.width / 2) - ((float)txt_length / 2), rec.y + (rec.height / 2) - 20, TXT_FONTSIZE, txt_color);
            }
        }
    }
}

void draw_end_screen()
{
	const char* txt = "Game Over";
	int txt_size = MeasureText(txt, TXT_FONTSIZE);

	DrawText(txt, (WIDTH / 2) - (txt_size / 2), HEIGHT / 2 - 50, TXT_FONTSIZE, WHITE);

	const char* txt_2 = "Try Again";
	int txt_size_2 = MeasureText(txt_2, TXT_FONTSIZE_SMALL);

	Rectangle button = {(int)(WIDTH / 2) - (int)(txt_size_2 / 2), (int)(HEIGHT / 2) + 50, txt_size_2 + 20, TXT_FONTSIZE_SMALL * 1.3};
	DrawRectangleRounded(button, 0.20f, 20, BROWN);
	DrawText(txt_2, button.x + 10, button.y + 5, TXT_FONTSIZE_SMALL, WHITE);
}
