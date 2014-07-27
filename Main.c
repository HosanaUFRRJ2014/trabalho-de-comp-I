#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL.h>
#include <stdio.h>

#include "timer.c"
#include "figura.c"
#include "recorde.c"
#include "NomeRecorde.c"
#include "telaInicial2.c"
#include "Fase1hh.c" //Fase21



#include "Morte.c"

#define FONTSIZE 21

//string de compilação:
//gcc Main.c -o MainTesteUniao -lSDL -lSDL_image -lSDL_ttf -lSDL -lSDL_mixer

int main(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//=======================================
	//Inicia o TTF.
	TTF_Init();

	//=========================================

	SDL_Surface *screen, *surPersonagem;
	screen = SDL_SetVideoMode(800,600,16,SDL_HWSURFACE | SDL_DOUBLEBUF);
	SDL_keysym Controles[5];
	Controles[0].sym = SDLK_UP;
	Controles[1].sym = SDLK_DOWN;
	Controles[2].sym = SDLK_RIGHT;
	Controles[3].sym = SDLK_LEFT;
	Controles[4].sym = SDLK_SPACE;


	//===============================================
	/*Necessário para inicializar a biblioteca do SDL_mixer(só pode ser
	 iniciado sempre depois do SDL_Init.
	 */
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	//===============================================

	SDL_Surface*TelaApresentacao = IMG_Load("Imagens/Capa_jogo2.png");
	
	SDL_Surface*TelaFim = IMG_Load("Imagens/Fim.png");
	
	
	
	SDL_BlitSurface(TelaApresentacao,NULL,screen,NULL);

	SDL_Flip(screen);

	SDL_Delay(3000);

	int Opcoes [2] = {1, 1} ;

	int ProximaTela = 0;
	int Pontos = 0;

	int quit = 0;
	
	while(!quit)
	{
		switch(ProximaTela)
		{
			case -2:
			{
			    ProximaTela = TelaMorte(screen,Opcoes,&Pontos);
			    Pontos = 0;
			    break;
			}
		  
			case -1:
			{
				quit = 1;
				break;
			}
			case 0:
			{
				ProximaTela = TelaInicial(screen, Opcoes);
				break;
			}
			case 1:
			{
				//SDL_BlitSurface(TelaInicio,NULL,screen,NULL);
				//SDL_Flip(screen);
				//SDL_Delay(4000);
				//Introducao(screen,ProximaTela,Opcoes);
				//ProximaTela = Fase2(screen,Opcoes,&Pontos);
				ProximaTela = Fase1(screen, Opcoes, &Pontos);
				break;
			}
			case 2:
			{
			    //ProximaTela = Fase1(screen,Opcoes,&Pontos);
			    //ProximaTela = Fase2(screen,Opcoes,&Pontos);
			     //ProximaTela = FimJogo(screen,Opcoes,&Pontos);
			     screen = SDL_SetVideoMode(800,600,16,SDL_HWSURFACE | SDL_DOUBLEBUF);
			     SDL_BlitSurface(TelaFim,NULL,screen,NULL);
			     SDL_Flip(screen);
			     SDL_Delay(4000);
			     Nome_recorde(&Pontos,screen);
			     ProximaTela = 0;
			     Pontos = 0;
			     break;
			}
			
			/*
			case numeroquenãoseiainda:
			{
			    ProximaTela = FimJogo(screen,Opcoes,&Pontos);
			    Pontos = 0;
			    break;
			}
			*/
		}
		 
	    SDL_Flip(screen);
	}

	printf("%d \n", ProximaTela);

	//Mix_CloseAudio(); ---------------->Serve para fechar a bibliotece mixer(isso será usado apenas no fim do jogo).

	SDL_Quit();
	return 0;
}
