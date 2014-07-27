#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>



int TelaMorte(SDL_Surface *screen, int EstadoOpcoes[2],int *pontuacao) //int main() //void TelaMorte(SDL_Surface *screen)
{


      SDL_Event event;

      int quit = 0;
      
      screen = SDL_SetVideoMode(800, 600,16,SDL_HWSURFACE | SDL_DOUBLEBUF);

      //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	Figura TelaMorte;
	TelaMorte.x = 0;
	TelaMorte.y = 0;
	TelaMorte.imagem = IMG_Load("Imagens/gameover.png");

	Mix_Chunk * SomMorte = Mix_LoadWAV("SonsJogo/risada.wav");

	TTF_Font *fonte;
	fonte = TTF_OpenFont("japonese.ttf",30);
	SDL_Color textColor = { 255, 255, 255 };

	
	Mix_PauseMusic();
	
	Nome_recorde(pontuacao,screen);


    if(EstadoOpcoes[0] == 0)
        Mix_Volume(-1, 0);
    else
        Mix_Volume(-1, 128);


	Mix_PlayChannel(-1,SomMorte,0);

      while(!quit)
      {
	  while (SDL_PollEvent(&event))
	  {
	      //if(event.type == SDL_QUIT)
            //quit = 1;

	      if(event.type == SDL_KEYDOWN)
	      {
		if(event.key.keysym.sym == SDLK_ESCAPE)
		    return -1;
		
		if(event.key.keysym.sym == SDLK_RETURN) //condição para retornar a Tela Inicial
		    quit = 1;
		
	      }

	  }
	    //SDL_BlitSurface(TelaMorte,NULL,screen,NULL);
	    apply_surface(TelaMorte,screen);
	    desenha_texto("Aperte ENTER para voltar ao menu inicial",screen,100,500,fonte);

	    SDL_Flip(screen);
      }


      Mix_FreeChunk(SomMorte);
      
      return 0;


}
