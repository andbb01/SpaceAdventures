#include "SpaceAdventures.h"


int delay = 10, bvel = 12;
int ShootState = 0;
int Score = 0;
Game GameState = Menu;

int main(void){
	extern SDL_Renderer *ren;
	SDL_Window *win;
	SDL_Event event;	
	extern int bevel;
	extern float evel;

	int Pointer = 0;
	int regenerate = 1;
	int next = 0;

	SDL_Rect MyRect = {DIM_X/2, DIM_Y - (SHIP_DIM * 2), SHIP_DIM, SHIP_DIM}, BulletRect = {0, - BULLET_DIM, BULLET_DIM/2, BULLET_DIM},  EnemiesBullet = {DIM_X, DIM_Y, BULLET_DIM/2, BULLET_DIM};
	SDL_Rect EnemiesRect[ENEMIES_COLUMN][ENEMIES_LINE], AsteroidsRect[ASTEROIDS_COLUMN][ASTEROIDS_LINE];


	Mix_Chunk *shoot, *EnemyKilled, *KilledMe;
	Mix_Music *music, *MenuMusic, *TravelMusic;

	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);

	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	

	MenuMusic = Mix_LoadMUS("sounds/MenuMusic.mp3");
	music = Mix_LoadMUS("sounds/Music.mp3");
	TravelMusic = Mix_LoadMUS("sounds/SpaceTravelMusic.ogg");
	EnemyKilled = Mix_LoadWAV("sounds/EnemyKilled.wav");
	shoot = Mix_LoadWAV("sounds/shoot.wav");
	KilledMe = Mix_LoadWAV("sounds/KilledMe.wav");

	SDL_CreateWindowAndRenderer(DIM_X, DIM_Y, 0, &win, &ren);
	SDL_SetWindowTitle(win, "SpaceAdventures");
	
	srand((unsigned) 30);
	Mix_PlayMusic(MenuMusic, -1);
	while(1){
		while (GameState == Menu){
			SDL_PollEvent(&event);
			switch(UseMenu(event.button.button)){
				case 1:
					ShootState = 0;
					Score = 0;
					EnemiesCreation(EnemiesRect);
					CreateAsteroids(AsteroidsRect);
					EndControl(EnemiesRect, -1);
					MyRect.x = DIM_X/2;
					MyRect.y =  DIM_Y - (SHIP_DIM * 2);
					Mix_PlayMusic(music, -1);
	
					GameState = Pause;
					TimeSeconds(0);
					next = -2;
	
					bevel = 7;
					evel = 1;
					Pointer = 1;
					break;
				case -1:
					Pointer = -1;
					DrawMenu(Pointer);
					exit(0);
					break;
				case 2:
					Pointer = 2;
					break;
				case -2:
					Pointer = -2;
					break;
			}
			DrawMenu(Pointer);
			Pointer = 0;
			if (KeyPressed(SDL_SCANCODE_ESCAPE) == 1 || event.type == SDL_QUIT)
			exit(0);
		}

		SDL_PollEvent(&event);
		
		DrawGame(MyRect, EnemiesRect, BulletRect, EnemiesBullet, AsteroidsRect);
		ShowUpperBar();
		Move(&MyRect.x);

		if (GameState == Invasion){//TheInvasionIsComing
			EnemiesMove(EnemiesRect);
			//MenagesEnemiesShots
			EnemiesShot(&EnemiesBullet, EnemiesRect, shoot);
			//HitsTheEnemies 
			if (EnemyHit(BulletRect, EnemiesRect) == 1){
				ShootState = 0;
				Mix_PlayChannel(-1, EnemyKilled, 0);
					BulletRect.x = DIM_X;
				BulletRect.y = DIM_Y;
			}
			//MenagesTheShots
			if (KeyPressed(SDL_SCANCODE_SPACE) == 1 && ShootState == 0){
				Mix_PlayChannel(-1, shoot, 0);
				ShootState = 1;
				BulletRect.x = MyRect.x + (SHIP_DIM/2) - 10;
				BulletRect.y = MyRect.y;
			}
			else if (ShootState == 1){
				BulletRect.y -= bvel;
				if (BulletRect.y == 0){
					ShootState = 0;
					BulletRect.x = DIM_X;
					BulletRect.y = DIM_Y;
				}
			}
			//HitsMe
			if (HitMe(EnemiesBullet, MyRect) == 1){
				Mix_PlayChannel(-1, KilledMe, 0);
				ShowM("Game  over");
				Mix_PlayMusic(MenuMusic, -1);
				EnemiesBullet.x = DIM_X;
				EnemiesBullet.y = DIM_Y;
				BulletRect.x = DIM_X;
				BulletRect.y = DIM_Y;
				GameState = Menu;
			}
			switch (EndControl(EnemiesRect, 0)){
			case 1:
				GameState = Pause;
				next = -1;
				bevel += 1;
				evel += 0.5;
				Mix_PlayMusic(TravelMusic, -1);
				BulletRect.x = DIM_X;
				BulletRect.y = DIM_Y;
				TimeSeconds(0);
				regenerate = 1;
				CreateAsteroids(AsteroidsRect);
				break;
			case -1:
				ShowM("Game  over");
				GameState = Menu;
				Mix_PlayMusic(MenuMusic, -1);
				BulletRect.x = DIM_X;
				BulletRect.y = DIM_Y;
				break;
			}
		}
		else if (GameState == Travel){//TravelState
			if (MoveAsteroids(AsteroidsRect, regenerate) == 1){
				EnemiesCreation(EnemiesRect);
				GameState = Pause;
				TimeSeconds(0);
				next = -2;
				ShootState = 0;
				Mix_PlayMusic(music, -1);
			}
			if (AsteroidInvested(MyRect, AsteroidsRect) == 1){
				Mix_PlayChannel(-1, KilledMe, 0);
				ShowM("Game  over");
				GameState = Menu;
				Mix_PlayMusic(MenuMusic, -1);
				EnemiesBullet.x = DIM_X;
				EnemiesBullet.y = DIM_Y;
				BulletRect.x = DIM_X;
				BulletRect.y = DIM_Y;
			}
			if (TimeSeconds(1) >= 30){
				regenerate = 0;
			}

		}
		else if (GameState == Pause && TimeSeconds(1) == 5){
			GameState += next;
			TimeSeconds(0);
		}

		if (KeyPressed(SDL_SCANCODE_ESCAPE) == 1 || event.type == SDL_QUIT)
			exit(0);

		SDL_RenderPresent(ren);
		SDL_Delay(delay);
	}
	Mix_CloseAudio();
	SDL_DestroyWindow(win);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
}	
