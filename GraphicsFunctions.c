#include "SpaceAdventures.h"

extern void BackroundScrolling(int *y);
TTF_Font *font;

int DrawMenu(int Pointer){
	SDL_Rect StartRect = {DIM_X/2 - 50, DIM_Y/3 + 150, 100, 20}, ExitRect = {StartRect.x + 10, StartRect.y + 30, 80, 20};
	static SDL_Surface *Start, *Exit, *Title;
	static SDL_Texture *StartTexture, *ExitTexture, *TitleTexture;
	int delay = 0;
	SDL_Color green = {0x00, 0xff, 0x00, SDL_ALPHA_OPAQUE};//green
	SDL_Color white = {0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE};//white
	SDL_Color red = {0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE};//red

	if (!font){
		font = TTF_OpenFont("font/arcadeclassic.ttf", 20);
	}

	if (!Title){
		Title = IMG_Load("images/MenuIcon.png");
		TitleTexture = SDL_CreateTextureFromSurface(ren, Title);
	}

	if (Pointer == 1){
		Start = TTF_RenderText_Solid(font, "Start", red);
		Exit = TTF_RenderText_Solid(font, "Exit", white);
		delay = 500;
	}
	else if (Pointer == -1){
		Start = TTF_RenderText_Solid(font, "Start", white);
		Exit = TTF_RenderText_Solid(font, "Exit", red);
		delay = 500;
	}
	else {		
		Start = TTF_RenderText_Solid(font, "Start", white);
		Exit = TTF_RenderText_Solid(font, "Exit", white);
	}

	StartTexture = SDL_CreateTextureFromSurface(ren, Start);
	ExitTexture = SDL_CreateTextureFromSurface(ren, Exit);
	
	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(ren);	
	SDL_RenderCopy(ren, TitleTexture, NULL, NULL);
	SDL_RenderCopy(ren, StartTexture, NULL, &StartRect);
	SDL_RenderCopy(ren, ExitTexture, NULL, &ExitRect);
	SDL_SetRenderDrawColor(ren, 0x8a, 0x8a, 0x8a, SDL_ALPHA_OPAQUE);
	if (Pointer == 2 || Pointer == 1){
		SDL_RenderDrawRect(ren, &StartRect);
	}
	else if (Pointer == -2 || Pointer == -1){
		SDL_RenderDrawRect(ren, &ExitRect);
	}	
	SDL_RenderPresent(ren);
	SDL_Delay(delay);
}

void DrawGame(SDL_Rect MyRect, SDL_Rect EnemiesRect[ENEMIES_COLUMN][ENEMIES_LINE], SDL_Rect BulletRect, SDL_Rect EnemiesBullet,SDL_Rect AsteroidsRect[ASTEROIDS_COLUMN][ASTEROIDS_LINE]){
	extern Game GameState;
	static SDL_Surface *MyShip, *EnemyShip, *Background, *Asteroid;
	static SDL_Texture *MyTexture,	*EnemyTexture, *BackgroundTexture, *AsteroidTexture;
	static SDL_Rect BackgroundRect = {0, -DIM_Y, DIM_X, DIM_Y*2};
	static unsigned int Init = 0;
	static float angle = 0;
	
	if (Init == 0){
		Background = IMG_Load("images/SpaceBackground.png");
		BackgroundTexture = SDL_CreateTextureFromSurface(ren, Background);

		MyShip = IMG_Load("images/MyShip.png");
		MyTexture = SDL_CreateTextureFromSurface(ren, MyShip);

		EnemyShip = IMG_Load("images/EnemyShip.png");
		EnemyTexture = SDL_CreateTextureFromSurface(ren, EnemyShip);

		Asteroid = IMG_Load("images/Asteroid.png");
		AsteroidTexture = SDL_CreateTextureFromSurface(ren, Asteroid);
		Init = 1;
	}

	BackroundScrolling(&BackgroundRect.y);
	
	SDL_RenderClear(ren);
	SDL_SetRenderDrawColor(ren, 0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, BackgroundTexture, NULL, &BackgroundRect);
	SDL_RenderCopy(ren, MyTexture, NULL, &MyRect);
	
	if (GameState == Travel){//DrawAsteroids
		for (int i = 0; i < ASTEROIDS_COLUMN; i++)
			for (int j = 0; j < ASTEROIDS_LINE; j++)
				SDL_RenderCopyEx(ren, AsteroidTexture, NULL, &AsteroidsRect[i][j], angle, NULL, SDL_FLIP_HORIZONTAL);
		angle+= 0.1;
	}
		
	else if (GameState == Invasion){//DrawEnemies&Bullets
		for (int i = 0; i < ENEMIES_COLUMN; i++)
			for (int j = 0; j < ENEMIES_LINE; j++)
				if (EnemiesRect[i][j].x > 0 && EnemiesRect[i][j].y > 0)
					SDL_RenderCopy(ren, EnemyTexture, NULL, &EnemiesRect[i][j]);
		SDL_RenderFillRect(ren, &EnemiesBullet);
		SDL_RenderFillRect(ren, &BulletRect);
	}
}

void ShowM(char text[15]){
	static SDL_Rect MessageRect = {DIM_X/2 - 180, DIM_Y/2 - 40, 360, 80};
	SDL_Color red = {0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE};//red
	SDL_Surface *Text;
	SDL_Texture *TextTexture;
	static Mix_Chunk *GameOver;

	Mix_HaltMusic();

	if (!GameOver){
		GameOver = Mix_LoadWAV("sounds/GameOver.wav");
	}
	
	Text = TTF_RenderText_Solid(font, text, red);
	TextTexture = SDL_CreateTextureFromSurface(ren, Text);

	SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);	
	
	for (int i = 0; i <= DIM_Y; i++){
		SDL_RenderDrawLine(ren, 0, i, DIM_X, i);
		SDL_RenderPresent(ren);
		SDL_Delay(2);
	}
	Mix_PlayChannel(1, GameOver, 0);
	SDL_RenderCopy(ren, TextTexture, NULL, &MessageRect);
	SDL_RenderPresent(ren);
	SDL_Delay(4000);
}

void ShowUpperBar(){
	extern Game GameState;
	extern int bevel;
	extern int Score;
	const SDL_Rect scoreRect = {30, 10, 100, 20}, levelRect = {DIM_X - 130, 10, 100, 20}, gameRect = {DIM_X/2 - 100, 10, 200, 20};
	SDL_Color white = {0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE};
	SDL_Color red = {0xff, 0x00, 0x00, SDL_ALPHA_OPAQUE};
	SDL_Surface *score, *level, *game;
	SDL_Texture *scoreTexture, *levelTexture, *gameTexture;
	char scoreSTR[15], levelSTR[15];
	
	sprintf(levelSTR, "Level  %d", bevel - 6);
	sprintf(scoreSTR, "Score  %d", Score);
	if (GameState == Travel)
		game = TTF_RenderText_Solid(font, "change  sector" , white);
	else if (GameState == Invasion)
		game = TTF_RenderText_Solid(font, "allert  enemies" , red);
	else if (GameState == Pause)
		game = TTF_RenderText_Solid(font, "are  you  ready" , white);

	score = TTF_RenderText_Solid(font, scoreSTR, white);
	level = TTF_RenderText_Solid(font, levelSTR, white);
	scoreTexture = SDL_CreateTextureFromSurface(ren, score);
	levelTexture = SDL_CreateTextureFromSurface(ren, level);
	gameTexture = SDL_CreateTextureFromSurface(ren, game);
	SDL_RenderCopy(ren, scoreTexture, NULL, &scoreRect);
	SDL_RenderCopy(ren, levelTexture, NULL, &levelRect);
	SDL_RenderCopy(ren, gameTexture, NULL, &gameRect);
}
