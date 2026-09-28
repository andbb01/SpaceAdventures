#include "SpaceAdventures.h"

int vel = 2;
extern SDL_Renderer *ren;
float backvel = 0.1;

int KeyPressed(int key){
	const Uint8 *Key = SDL_GetKeyboardState(NULL);
	if (Key[key])
		return 1;
	else return 0;
}

void Move(int *x){
	if (KeyPressed(SDL_SCANCODE_RIGHT) == 1 && *x + vel < DIM_X - SHIP_DIM)
		*x += vel;
	if (KeyPressed(SDL_SCANCODE_LEFT) == 1 && *x - vel > 0)
		*x -= vel;
}

int EndControl(SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE], int instructions){
	static int End = 0;
	int cont = 0;
	if (instructions != 0)
		End = instructions;
	else if (End > 0)
		return -End;
	for (int i = 0; i < ENEMIES_COLUMN; i++)
				for (int j = 0; j < ENEMIES_LINE; j++)
					if (enemies[i][j].x < 0)
						cont++;
	return (cont == 55 ? 1 : 0);
}

int HitMe(SDL_Rect bullet, SDL_Rect me){
	static SDL_Surface *Explosion;
	static SDL_Texture *ExplosionTexture;

	if (!Explosion){
		Explosion = IMG_Load("images/ShipExplosion.png");
		ExplosionTexture = SDL_CreateTextureFromSurface(ren, Explosion);
	}
	
	if ((bullet.y > me.y && bullet.y < me.y + SHIP_DIM) && (bullet.x > me.x && bullet.x < me.x + SHIP_DIM)){
		SDL_RenderCopy(ren, ExplosionTexture, NULL, &me);
		return 1;
	}
	else return 0;
}

int UseMenu(int button){
	SDL_Point MousePosition;
	SDL_GetMouseState(&MousePosition.x, &MousePosition.y);
	if (MousePosition.x > DIM_X/2 - 50 && MousePosition.x < DIM_X/2 + 50 && MousePosition.y > DIM_Y/3 + 150 && MousePosition.y < DIM_Y/3 + 170){
		if (button == SDL_BUTTON_LEFT)
			return 1;
		else return 2;
	}
	else if (MousePosition.x > DIM_X/2 - 40 && MousePosition.x < DIM_X/2 + 40 && MousePosition.y > DIM_Y/3 + 180 && MousePosition.y < DIM_Y/3 + 200){
		if (button == SDL_BUTTON_LEFT)
			return -1;
		else return -2;
	}
	else return 0;
}

void BackroundScrolling(int *y){
	if (*y + backvel < 0)
		*y += backvel;
	else *y = -DIM_Y;
}

int TimeSeconds(int Init){
	static unsigned int StartTime;
	if (Init == 0)
		StartTime = SDL_GetTicks();

	return (SDL_GetTicks() - StartTime)/1000;
}
