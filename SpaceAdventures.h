#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define DIM_Y 640
#define DIM_X 860
#define SHIP_DIM 50
#define BULLET_DIM 10
#define ENEMIES_LINE 11
#define ENEMIES_COLUMN 5
#define POINTER_DIM 10
#define ASTEROIDS_COLUMN 5
#define ASTEROIDS_LINE 8

int KeyPressed(int key);//GetTheKeyboardState
int EndControl(SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE], int instructions);
int TimeSeconds(int Init);

void Move(int *x);
int HitMe(SDL_Rect bullet, SDL_Rect me);

void EnemiesCreation(SDL_Rect En[ENEMIES_COLUMN][ENEMIES_LINE]);//CreateTheEnemiesFleet
int EnemyHit(SDL_Rect bullet, SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE]);//HitsTheEnemies
int RightEnemies(SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE]);
int LeftEnemies(SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE]);
int DownEnemies(SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE]);
void EnemiesMove(SDL_Rect En[ENEMIES_COLUMN][ENEMIES_LINE]);
void EnemiesShot(SDL_Rect *bullet, SDL_Rect fleet[ENEMIES_COLUMN][ENEMIES_LINE], Mix_Chunk *shoot);

void CreateAsteroids(SDL_Rect AsteroidsRect[ASTEROIDS_COLUMN][ASTEROIDS_LINE]);
int MoveAsteroids(SDL_Rect AsteroidsRect[ASTEROIDS_COLUMN][ASTEROIDS_LINE], int regeneration);
int AsteroidInvested(SDL_Rect me, SDL_Rect Asteroids[ASTEROIDS_COLUMN][ASTEROIDS_LINE]);

void BackroundScrolling(int *y);

int UseMenu(int button);

void DrawGame(SDL_Rect MyRect, SDL_Rect EnemiesRect[ENEMIES_COLUMN][ENEMIES_LINE], SDL_Rect BulletRect, SDL_Rect EnemiesBullet, SDL_Rect AsteroidsRect[ASTEROIDS_COLUMN][ASTEROIDS_LINE]);
int DrawMenu(int Pointer);
void ShowM(char text[15]);
void ShowUpperBar();

SDL_Renderer *ren;
int score, level;

typedef enum Game{Menu, Invasion, Travel, Pause}Game;

