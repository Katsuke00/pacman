#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/* flags that defines what moves can be done
 * in a block */
#define MU 0x01
#define MD 0x02
#define ML 0x04
#define MR 0x08

/* flags that defines when a block is obrigatory
 * and in what direction */
#define MFU 0x01
#define MFD 0x02
#define MFL 0x04
#define MFR 0x08

#define BLOCKS 64
#define HEIGHT 513
#define WIDTH  513

#define WINNAME  "pacman"
#define MAPPATH  "img/map.bmp"
#define CHARPATH "img/pacman.bmp"

struct moveblock {
	int x[2];
	int y[2];
	int dirs;
	int type;
};

struct speed {
	int x;
	int y;
};

/*{.x={000,000}, .y={111,111}, .dirs=MU|MD|ML|MR, .type=},*/
static struct moveblock blocks[] = {
	/* COLUMN 01 */
	{.x={15,57},   .y={11,45},   .dirs=MD|MR,       .type=MFU|MFL},
	{.x={104,140}, .y={11,45},   .dirs=MD|ML|MR,    .type=MFU},
	{.x={210,245}, .y={11,45},   .dirs=MD|ML,       .type=MFU|MFR},
	{.x={265,302}, .y={11,45},   .dirs=MD|MR,       .type=MFU|MFL},
	{.x={372,408}, .y={11,45},   .dirs=MD|ML|MR,    .type=MFU},
	{.x={455,495}, .y={11,45},   .dirs=MD|ML,       .type=MFU|MFR},
	/* COLUMN 02 */
	{.x={15,57},   .y={77,110},  .dirs=MU|MD|MR,    .type=MFL},
	{.x={104,140}, .y={77,110},  .dirs=MU|MD|ML|MR, .type=0},
	{.x={156,194}, .y={77,110},  .dirs=MD|ML|MR,    .type=MFU},
	{.x={210,246}, .y={77,110},  .dirs=MU|ML|MR,    .type=MFD},
	{.x={265,302}, .y={77,110},  .dirs=MU|ML|MR,    .type=MFD},
	{.x={318,356}, .y={77,110},  .dirs=MD|ML|MR,    .type=MFU},
	{.x={372,407}, .y={77,110},  .dirs=MU|MD|ML|MR, .type=0},
	{.x={455,497}, .y={77,110},  .dirs=MU|MD|ML,    .type=MFR},
	/* COLUMN 03 */
	{.x={15,57},   .y={126,159}, .dirs=MU|MR,       .type=MFD|MFL},
	{.x={104,140}, .y={126,159}, .dirs=MU|MD|ML,    .type=MFR},
	{.x={156,194}, .y={126,159}, .dirs=MU|MR,       .type=MFD|MFL},
	{.x={210,246}, .y={126,159}, .dirs=MD|ML,       .type=MFU|MFR},
	{.x={265,302}, .y={126,159}, .dirs=MD|MR,       .type=MFU|MFL},
	{.x={318,356}, .y={126,159}, .dirs=MU|ML,       .type=MFD|MFR},
	{.x={372,407}, .y={126,159}, .dirs=MU|MD|MR,    .type=MFL},
	{.x={455,497}, .y={126,159}, .dirs=MU|ML,       .type=MFD|MFR},
	/* COLUMN 04 */
	{.x={156,194}, .y={174,206}, .dirs=MD|MR,       .type=MFU|MFL},
	{.x={210,245}, .y={174,206}, .dirs=MU|ML|MR,    .type=MFD},
	{.x={265,302}, .y={174,206}, .dirs=MU|ML|MR,    .type=MFD},
	{.x={318,356}, .y={174,206}, .dirs=MD|ML,       .type=MFU|MFR},
	/* COLUMN 05 */
	{.x={104,140}, .y={224,256}, .dirs=MU|MD|ML|MR, .type=0},
	{.x={156,194}, .y={224,256}, .dirs=MU|MD|ML,    .type=MFR},
	{.x={318,356}, .y={224,256}, .dirs=MU|MD|MR,    .type=MFL},
	{.x={372,408}, .y={224,256}, .dirs=MU|MD|ML|MR, .type=0},
	/* COLUMN 06 */
	{.x={104,140}, .y={273,305}, .dirs=MU|MD|MR,    .type=MFL},
	{.x={318,356}, .y={273,305}, .dirs=MU|MD|ML,    .type=MFR},
	/* COLUMN 07 */
	{.x={15,57},   .y={321,353}, .dirs=MD|MR,       .type=MFU|MFL},
	{.x={104,140}, .y={321,353}, .dirs=MU|MD|ML|MR, .type=0},
	{.x={156,194}, .y={321,353}, .dirs=MU|ML|MR,    .type=MFD},
	{.x={210,246}, .y={321,353}, .dirs=MD|ML,       .type=MFU|MFR},
	{.x={265,302}, .y={321,353}, .dirs=MD|MR,       .type=MFU|MFL},
	{.x={318,356}, .y={321,353}, .dirs=MU|ML|MR,    .type=MFD},
	{.x={372,407}, .y={321,353}, .dirs=MU|MD|ML|MR, .type=0},
	{.x={455,497}, .y={321,353}, .dirs=MD|ML,       .type=MFU|MFR},
	/* COLUMN 08 */
	{.x={15,57},   .y={371,403}, .dirs=MU|MR,       .type=MFD|MFL},
	{.x={51,87},   .y={371,403}, .dirs=MD|ML,       .type=MFR},
	{.x={104,140}, .y={371,403}, .dirs=MU|MD|MR,    .type=MFL},
	{.x={156,194}, .y={371,403}, .dirs=MD|ML|MR,    .type=MFU},
	{.x={210,246}, .y={371,403}, .dirs=MD|ML|MR,    .type=MFD},
	{.x={265,302}, .y={371,403}, .dirs=MD|ML|MR,    .type=MFD},
	{.x={318,356}, .y={371,403}, .dirs=MD|ML|MR,    .type=MFU},
	{.x={372,407}, .y={371,403}, .dirs=MU|MD|ML,    .type=MFR},
	{.x={424,461}, .y={371,403}, .dirs=MD|MR,       .type=MFL},
	{.x={455,497}, .y={371,403}, .dirs=MU|ML,       .type=MFD|MFR},
	/* COLUMN 09 */
	{.x={15,57},   .y={418,450}, .dirs=MD|MR,       .type=MFU|MFL},
	{.x={51,87},   .y={418,450}, .dirs=MU|MR|ML,    .type=MFD},
	{.x={104,140}, .y={418,450}, .dirs=MU|ML,       .type=MFD|MFR},
	{.x={156,194}, .y={418,450}, .dirs=MU|MR,       .type=MFD|MFL},
	{.x={210,246}, .y={418,450}, .dirs=MD|ML,       .type=MFU|MFR},
	{.x={265,302}, .y={418,450}, .dirs=MD|MR,       .type=MFU|MFL},
	{.x={318,356}, .y={418,450}, .dirs=MU|ML,       .type=MFD|MFR},
	{.x={372,407}, .y={418,450}, .dirs=MU|MR,       .type=MFD|MFL},
	{.x={424,461}, .y={418,450}, .dirs=MU|ML|MR,    .type=MFD},
	{.x={455,497}, .y={418,450}, .dirs=MD|ML,       .type=MFU|MFR},
	/* COLUMN 10 */
	{.x={15,57},   .y={468,500}, .dirs=MU|MR,       .type=MFD|MFL},
	{.x={210,246}, .y={468,500}, .dirs=MU|ML|MR,    .type=MFD},
	{.x={265,302}, .y={468,500}, .dirs=MU|ML|MR,    .type=MFD},
	{.x={455,495}, .y={468,500}, .dirs=MU|ML,       .type=MFD|MFR},
};

static int nextev;

static void
key(int sym, struct speed *speed)
{
	if      (sym == SDLK_UP)
		speed->y -= 1;
	else if (sym == SDLK_DOWN)
		speed->y += 1;
	else if (sym == SDLK_LEFT)
		speed->x -= 1;
	else if (sym == SDLK_RIGHT)
		speed->x += 1;
}

static int
moves(int flags, int sym, struct speed *speed)
{
	int rval = 0;
	if (sym == SDLK_UP) {
		if (flags & MU)
			speed->x = 0;
		else
			rval++;
	}
	if (sym == SDLK_DOWN) {
		if (flags & MD)
			speed->x = 0;
		else
			rval++;
	}
	if (sym == SDLK_LEFT) {
		if (flags & ML)
			speed->y = 0;
		else
			rval++;
	}
	if (sym == SDLK_RIGHT) {
		if (flags & MR)
			speed->y = 0;
		else
			rval++;
	}
	return rval;
}

static int
colision(SDL_Rect *dest, struct speed *speed, int sym)
{
	int i;

	for (i = 0; i < BLOCKS; i++) {
		if (dest->x >= blocks[i].x[0]         &&
		    dest->x <= blocks[i].x[1]-dest->h &&
		    dest->y >= blocks[i].y[0]         &&
		    dest->y <= blocks[i].y[1]-dest->w) {
			nextev = moves(blocks[i].dirs, sym, speed);
			if ((blocks[i].type & MFU && speed->y < 0) ||
			    (blocks[i].type & MFD && speed->y > 0) ||
			    (blocks[i].type & MFL && speed->x < 0) ||
			    (blocks[i].type & MFR && speed->x > 0))
				nextev++;
			if (nextev)
				speed->y = speed->x = 0;
		}
	}

	return 0;
}

int
main(void)
{
	int ksym;
	SDL_Event    event;
	SDL_Rect     src, dest;
	SDL_Renderer *rend;
	SDL_Surface  *surf;
	SDL_Texture  *map, *chr;
	SDL_Window   *win;
	struct speed speed = {0};

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		errx(1, "SDL_Init: %s", SDL_GetError());

	if (!(win  = SDL_CreateWindow(WINNAME, SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0)))
		errx(1, "SDL_CreateWindow: %s", SDL_GetError());
	if (!(rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED)))
		errx(1, "SDL_CreateRenderer: %s", SDL_GetError());

	if (!(surf = SDL_LoadBMP(MAPPATH)))
		errx(1, "SDL_LoadBMP: %s", SDL_GetError());
	if (!(map = SDL_CreateTextureFromSurface(rend, surf)))
		errx(1, "SDL_CreateTextureFromSurface: %s", SDL_GetError());
	SDL_FreeSurface(surf);
	surf = NULL;

	if (!(surf = SDL_LoadBMP(CHARPATH)))
		errx(1, "SDL_LoadBMP: %s", SDL_GetError());
	if (!(chr = SDL_CreateTextureFromSurface(rend, surf)))
		errx(1, "SDL_CreateTextureFromSurface: %s", SDL_GetError());
	SDL_FreeSurface(surf);
	surf = NULL;

	src.x = 0;
	src.y = 0;
	src.w = 31;
	src.h = 34;

	dest.x = 15;
	dest.y = 11;
	dest.w = 31;
	dest.h = 34;

	while (1) {
		SDL_PollEvent(&event);
		ksym = event.key.keysym.sym;
		switch (event.type) {
		case SDL_KEYUP:
			key(ksym, &speed);
			break;
		case SDL_QUIT:
			goto end;
		}

		colision(&dest, &speed, ksym);

		if ((dest.x+speed.x) < WIDTH-dest.w && (dest.x+speed.x) > 0)
			dest.x += speed.x;
		if ((dest.y+speed.y) < HEIGHT-dest.h && (dest.y+speed.y) > 0)
			dest.y += speed.y;

		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, map, NULL, NULL);
		SDL_RenderCopy(rend, chr, &src, &dest);
		SDL_RenderPresent(rend);
		event.key.keysym.sym = event.type = 0;
	}

end:
	SDL_DestroyTexture(map);
	SDL_DestroyTexture(chr);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
