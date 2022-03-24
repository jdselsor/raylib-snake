#include "raylib.h"

#define BOOL char
#define TRUE 1
#define FALSE 0

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

#define SNAKE_LENGTH 256
#define CELL_SIZE 31

typedef struct Snake {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    Color color;
} Snake;

typedef struct Food {
    Vector2 position;
    Vector2 size;
    BOOL isActive;
    Color color;
} Food;

int framesCounter = 0;
BOOL gameOver = FALSE;
BOOL pause = FALSE;

Food fruit = { 0 };
Snake snake [SNAKE_LENGTH] = { 0 };
Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
BOOL allowMove = FALSE;
Vector2 offset = { 0 };
int counterTail = 0;

void init ();
void update ();
void render ();
void update_draw_frame ();

int main () {
    InitWindow (SCREEN_WIDTH, SCREEN_HEIGHT, "classic game: snake");

    init ();

    SetTargetFPS (60);

    while (!WindowShouldClose()) {
        update_draw_frame();
    }

    CloseWindow ();

    return 0;
}

void init () {
    framesCounter = 0;
    gameOver = FALSE;
    pause = FALSE;

    counterTail = 1;
    allowMove = FALSE;

    offset.x = SCREEN_WIDTH % CELL_SIZE;
    offset.y = SCREEN_HEIGHT % CELL_SIZE;

    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snake[i].position = (Vector2) { offset.x / 2, offset.y / 2 };
        snake[i].size = (Vector2) { CELL_SIZE, CELL_SIZE };
        snake[i].velocity = (Vector2) { CELL_SIZE, 0 };

        if (i == 0) {
            snake[i].color = (Color) { 0, 0, 170, 255 };
        }
        else {
            snake[i].color = (Color) { 0, 136, 255, 255 };
        }
    }

    for (int i = 0; i < SNAKE_LENGTH; i++) {
        snakePosition[i] = (Vector2) { 0.0f, 0.0f };
    }

    fruit.size = (Vector2) { CELL_SIZE, CELL_SIZE };
    fruit.color = (Color) { 136, 0, 0, 255 };
    fruit.isActive = FALSE;
}

void update () {
    if (!gameOver) {
        if (IsKeyPressed('P')) {
            pause = !pause;
        }

        if (!pause) {
            if (IsKeyPressed(KEY_RIGHT) && (snake[0].velocity.x == 0) && allowMove) {
                snake[0].velocity = (Vector2) { CELL_SIZE, 0 };
                allowMove = FALSE;
            }
            if (IsKeyPressed (KEY_LEFT) && (snake[0].velocity.x == 0) && allowMove) {
                snake[0].velocity = (Vector2) { -CELL_SIZE, 0 };
                allowMove = FALSE;
            }
            if (IsKeyPressed(KEY_UP) && (snake[0].velocity.y == 0) && allowMove) {
                snake[0].velocity = (Vector2) { 0, -CELL_SIZE };
                allowMove = FALSE;
            }
            if (IsKeyPressed (KEY_DOWN) && (snake[0].velocity.y == 0) && allowMove) {
                snake[0].velocity = (Vector2) { 0, CELL_SIZE };
                allowMove = FALSE;
            }

            for (int i = 0; i < counterTail; i++) {
                snakePosition[i] = snake[i].position;
            }

            if ((framesCounter % 5) == 0) {
                for (int i = 0; i < counterTail; i++) {
                    if (i == 0) {
                        snake[0].position.x += snake[0].velocity.x;
                        snake[0].position.y += snake[0].velocity.y;
                        allowMove = TRUE;
                    }
                    else {
                        snake[i].position = snakePosition[i-1];
                    }
                }
            }

            if (((snake[0].position.x) > (SCREEN_WIDTH - offset.x)) || ((snake[0].position.y) > (SCREEN_HEIGHT - offset.y)) || (snake[0].position.x < 0) || (snake[0].position.y < 0)) {
                gameOver = TRUE;
            }

            for (int i = 1; i < counterTail; i++) {
                if ((snake[0].position.x == snake[i].position.x) && (snake[0].position.y == snake[i].position.y)) {
                    gameOver = TRUE;
                }
            }

            if (fruit.isActive == FALSE) {
                fruit.isActive = TRUE;
                fruit.position = (Vector2) { GetRandomValue(0, (SCREEN_WIDTH / CELL_SIZE) - 1) * CELL_SIZE + offset.x / 2, GetRandomValue(0, (SCREEN_HEIGHT / CELL_SIZE) - 1) * CELL_SIZE + offset.y / 2 };

                for (int i = 0; i < counterTail; i++) {
                    while ((fruit.position.x == snake[i].position.x) &&  (fruit.position.y == snake[i].position.y)) {
                        fruit.position = (Vector2){ GetRandomValue(0, (SCREEN_HEIGHT/CELL_SIZE) - 1)*CELL_SIZE + offset.x/2, GetRandomValue(0, (SCREEN_HEIGHT/CELL_SIZE) - 1)*CELL_SIZE + offset.y/2 };
                        i = 0;
                    }
                }
            }

            if ((snake[0].position.x < (fruit.position.x + fruit.size.x) && (snake[0].position.x + snake[0].size.x) > fruit.position.x) && (snake[0].position.y < (fruit.position.y + fruit.size.y) && (snake[0].position.y + snake[0].size.y) > fruit.position.y)) {
                snake[counterTail].position = snakePosition[counterTail - 1];
                counterTail += 1;
                fruit.isActive = FALSE;
            }

            framesCounter ++;
        }
    }
    else {
        if (IsKeyPressed (KEY_ENTER)) {
            init();
            gameOver = FALSE;
        }
    }
}

void render () {
    BeginDrawing();

    ClearBackground((Color) {255, 255, 255, 255});

    if (!gameOver) {
        for (int i = 0; i < SCREEN_WIDTH / CELL_SIZE + 1; i++) {
            DrawLineV((Vector2){ CELL_SIZE * i + offset.x / 2, offset.y / 2}, (Vector2){ CELL_SIZE * i + offset.x / 2, SCREEN_HEIGHT - offset.y / 2 }, LIGHTGRAY);
        }

        for (int i = 0; i < SCREEN_HEIGHT / CELL_SIZE + 1; i++) {
            DrawLineV((Vector2){ offset.x / 2, CELL_SIZE * i + offset.y / 2}, (Vector2){ SCREEN_WIDTH - offset.x / 2, CELL_SIZE * i + offset.y / 2}, LIGHTGRAY);
        }

        for (int i = 0; i < counterTail; i++) {
            DrawRectangleV (snake[i].position, snake[i].size, snake[i].color);
        }

        DrawRectangleV (fruit.position, fruit.size, fruit.color);

        if (pause) {
            DrawText("GAME PAUSED", SCREEN_WIDTH / 2 - MeasureText ("GAME PAUSED", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, (Color) { 119, 119, 119, 255 });
        }
    }
    else {
        DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, GetScreenHeight() / 2 - 50, 20, (Color) { 119, 119, 119, 255 });
    }

    EndDrawing ();
}

void update_draw_frame () {
    update();
    render();
}