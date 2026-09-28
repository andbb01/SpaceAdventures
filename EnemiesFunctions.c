#include "SpaceAdventures.h"


int bevel = 7, avel = 3;
float evel;
extern SDL_Renderer *ren;


void EnemiesCreation(SDL_Rect En[ENEMIES_COLUMN][ENEMIES_LINE]){
	int i, j;
	for (i = 0; i < ENEMIES_COLUMN; i++)
		for (j = 0; j < ENEMIES_LINE; j++){
			En[i][j].x = SHIP_DIM*j + (SHIP_DIM/2) + j*10 + DIM_X;
			En[i][j].y = SHIP_DIM*i + (SHIP_DIM/2);
			En[i][j].w = SHIP_DIM;
			En[i][j].h = SHIP_DIM;
		}
}
	
int EnemyHit(SDL_Rect bullet, SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE]){
	int i, j;
	extern int Score;
	static SDL_Rect ExplosionRect = {0, 0, SHIP_DIM, SHIP_DIM};
	static SDL_Surface *Explosion;
	static SDL_Texture *ExplosionTexture;

	if (!Explosion){
		Explosion = IMG_Load("images/ShipExplosion.png");
		ExplosionTexture = SDL_CreateTextureFromSurface(ren, Explosion);
	}

	for (i = 0; i < ENEMIES_COLUMN; i++)
		for (j = 0; j < ENEMIES_LINE; j++)
			if ((bullet.x + (BULLET_DIM/2) >= enemies[i][j].x) && (bullet.x + (BULLET_DIM/2) <= enemies[i][j].x + SHIP_DIM) && (bullet.y >= enemies[i][j].y) && (bullet.y <= enemies[i][j].y + SHIP_DIM)){
				Score += 200;
				ExplosionRect.x = enemies[i][j].x;
				ExplosionRect.y = enemies[i][j].y;
				enemies[i][j].x = -SHIP_DIM;
				enemies[i][j].y = -SHIP_DIM;
				SDL_RenderCopy(ren, ExplosionTexture, NULL, &ExplosionRect);
				return 1;
			}
	return 0;
}

void EnemiesShot(SDL_Rect *bullet, SDL_Rect fleet[ENEMIES_COLUMN][ENEMIES_LINE], Mix_Chunk *shoot){
	int ry, rx;
	if (bullet->y + bevel < DIM_Y){
			bullet->y += bevel;	
		}
	else {
		do{
			rx = rand() % ENEMIES_COLUMN;
			ry = rand() % ENEMIES_LINE;
			bullet->y = fleet[rx][ry].y + SHIP_DIM;
			bullet->x = fleet[rx][ry].x + SHIP_DIM/2;
		}while (fleet[rx][ry].y < 0);
		Mix_PlayChannel(-1, shoot, 0);
	}
}

/*******************************************************************Menage enemies' movements***************************************************************************************/

void EnemiesMove(SDL_Rect En[ENEMIES_COLUMN][ENEMIES_LINE]){
	static int verse = -1;
	if (verse == 1)
		verse = RightEnemies(En);
	else if (verse == -1)
		verse = LeftEnemies(En);
}

int RightEnemies(SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE]){
	int maior = 0;
	for (int i = 0; i < ENEMIES_COLUMN; i++)
		for (int j = 0; j < ENEMIES_LINE; j++)
			maior = (enemies[i][j].x > maior ? enemies[i][j].x : maior);


	if (maior + evel < DIM_X - SHIP_DIM - SHIP_DIM/2){
		for (int i = 0; i < ENEMIES_COLUMN; i++)
			for (int j = 0; j < ENEMIES_LINE; j++)
					enemies[i][j].x = (enemies[i][j].x > 0 ? enemies[i][j].x + evel + 0.5: enemies[i][j].x);
		return 1;
	}
	else return -1;
}

int LeftEnemies(SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE]){
	int minor = DIM_X;
	for (int i = 0; i < ENEMIES_COLUMN; i++)
		for (int j = 0; j < ENEMIES_LINE; j++)
			minor = ((minor > enemies[i][j].x && enemies[i][j].x != -SHIP_DIM) ? enemies[i][j].x : minor);


	if (minor - evel > SHIP_DIM/2){
		for (int i = 0; i < ENEMIES_COLUMN; i++)
			for (int j = 0; j < ENEMIES_LINE; j++)
					enemies[i][j].x = (enemies[i][j].x > 0 ? enemies[i][j].x - evel : enemies[i][j].x);
		return -1;	
	}
	else  {
		if (DownEnemies(enemies) == 0)
			EndControl(enemies, 1);
		return 1;
	}
}


int DownEnemies(SDL_Rect enemies[ENEMIES_COLUMN][ENEMIES_LINE]){
	int mayor = 0;
	for (int i = 0; i < ENEMIES_COLUMN; i++)
		for (int j = 0; j < ENEMIES_LINE; j++)
			mayor = (mayor < enemies[i][j].y ? enemies[i][j].y : mayor);

	if (mayor + SHIP_DIM/2 + SHIP_DIM < DIM_Y - SHIP_DIM){
		for (int i = 0; i < ENEMIES_COLUMN; i++)
			for (int j = 0; j < ENEMIES_LINE; j++)
				enemies[i][j].y = (enemies[i][j].y > 0 ? enemies[i][j].y + SHIP_DIM/2 : enemies[i][j].y);
		return 1;
	}
	else return 0;
}

/****************************************************************************Asteroids Functions*************************************************************************************/

void CreateAsteroids(SDL_Rect AsteroidsRect[ASTEROIDS_COLUMN][ASTEROIDS_LINE]){
	for (int i = 0; i < ASTEROIDS_COLUMN; i++)
		for (int j = 0; j < ASTEROIDS_LINE; j++){
			AsteroidsRect[i][j].x = rand() % (DIM_X - SHIP_DIM);
			AsteroidsRect[i][j].y =  - (rand() % DIM_Y) - SHIP_DIM;
			AsteroidsRect[i][j].w = SHIP_DIM;
			AsteroidsRect[i][j].h = SHIP_DIM;
		}
}

int MoveAsteroids(SDL_Rect AsteroidsRect[ASTEROIDS_COLUMN][ASTEROIDS_LINE], int regeneration){
	int cnt = 0;
	for (int i = 0; i < ASTEROIDS_COLUMN; i++)
		for (int j = 0; j < ASTEROIDS_LINE; j++){
			AsteroidsRect[i][j].y += avel;
			if (AsteroidsRect[i][j].y > DIM_Y)
				cnt++;
			if (AsteroidsRect[i][j].y >= DIM_Y && regeneration == 1){
				AsteroidsRect[i][j].x = rand() % (DIM_X - SHIP_DIM);
				AsteroidsRect[i][j].y =  - (rand() % DIM_Y);
			}
		}
	if (cnt == 40)
		return 1;
	else return 0;
}

int AsteroidInvested(SDL_Rect me, SDL_Rect Asteroids[ASTEROIDS_COLUMN][ASTEROIDS_LINE]){
	static SDL_Surface *Explosion;
	static SDL_Texture *ExplosionTexture;

	if (!Explosion){
		Explosion = IMG_Load("images/ShipExplosion.png");
		ExplosionTexture = SDL_CreateTextureFromSurface(ren, Explosion);
	}

	for (int i = 0; i < ASTEROIDS_COLUMN; i++)
		for (int j = 0;j < ASTEROIDS_LINE; j ++)
			if ((me.x + SHIP_DIM/2 > Asteroids[i][j].x) && (me.x + SHIP_DIM/2 < Asteroids[i][j].x + SHIP_DIM) && (me.y > Asteroids[i][j].y) && (me.y < Asteroids[i][j].y + SHIP_DIM)){
				SDL_RenderCopy(ren, ExplosionTexture, NULL, &me);
				return 1;
			}
	return 0;
}

/*************************************************************************************************************************************************************************************/
