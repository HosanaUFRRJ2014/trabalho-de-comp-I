#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>


    void Nome_recorde(int *pontuacao, SDL_Surface *screen);
    void mostra_recorde(SDL_Surface *screen);


void Nome_recorde(int *pontuacao, SDL_Surface *screen)
{

  SDL_Event event;
  TTF_Font *fonte;
  fonte = TTF_OpenFont("japonese.ttf",25);
  SDL_Color textColor = {0, 255, 0};
  
    screen = SDL_SetVideoMode(800,600,16,SDL_HWSURFACE | SDL_DOUBLEBUF);

    fps.startTicks = 0;
    fps.pausedTicks = 0;
    fps.paused = 0;
    fps.started = 0;

  //Palavra da livia para teste
    char recorde[4];
    sprintf(recorde,"A__");
    Figura nome_figura;
    nome_figura.imagem = TTF_RenderText_Solid(fonte,recorde,textColor);
    int parte = 0;
    nome_figura.x = 400;//400
    nome_figura.y = 250;//200

    Figura pontuacao_figura;
    pontuacao_figura.x = 50; //50
    pontuacao_figura.y = 250; //200
    char pontuacao_vetor[8];
    sprintf(pontuacao_vetor,"%07d",*pontuacao);
    pontuacao_figura.imagem = TTF_RenderText_Solid(fonte,pontuacao_vetor,textColor);

    int quit = 0;

    char *nome_arquivo = "Recordes.dat";
    FILE *p_arquivo;

    ARRAY array;
    array.n = 0;

    if((p_arquivo = fopen(nome_arquivo, "r")) != NULL)
    {
        fread(&array, sizeof(ARRAY), 1, p_arquivo);
        fclose(p_arquivo);
    }
    else
    {
        printf("DEU ERRO\n");
        SDL_Quit();
        return ;
    }


     RECORDE rec1;

     if(array.n == MAX_ARRAY)
     {
        rec1 = elemento(&array,MAX_ARRAY - 1);

        if(*pontuacao < rec1.pontuacao)
        {
            //fclose(p_arquivo);
            return;
        }

    }

  while(!quit)
  {

     timer_start(&fps); // começa a contagem do vsync

      while (SDL_PollEvent(&event))
      {
	if(event.type == SDL_KEYUP)
        {
	  if(event.key.keysym.sym == SDLK_UP)
	  {
		if(parte < 4)
		  recorde[parte]++;

		if(recorde[parte] > 'Z')
		  recorde[parte] = 'A';
	  }

	  if(event.key.keysym.sym == SDLK_DOWN)
	  {
		if(parte < 4)
		  recorde[parte]--;

		if(recorde[parte] < 'A')
		  recorde[parte] = 'Z';
	  }
		if(event.key.keysym.sym == SDLK_RETURN)
		{
		    parte++;

		    if(parte >= 3 )
		    {
			if(recorde[0] == 'A' && recorde[1] == 'A' && recorde[2] == 'A')
			{
			    sprintf(recorde,"SNK");
			}

			  parte = 4;
			  quit = 1;
		    }
		    else
		      recorde[parte] = 'A';
		}
	}

      }

      //atualiza o nome do sujeito
      nome_figura.imagem = TTF_RenderText_Solid(fonte,recorde,textColor);

      //parte vai botar na tela
      SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00,0x00,0x00 ) ); //limpa a tela

      apply_surface(nome_figura,screen);
      apply_surface(pontuacao_figura,screen);
      desenha_texto("Selecione seu nome(Apenas 3 letras):",screen,200,100,fonte);

       SDL_Flip(screen) ; // atualiza a tela

        if( (timer_get_ticks(&fps) < 1000 / FPS ) )
        {
                SDL_Delay( ( 1000 / FPS ) - timer_get_ticks(&fps)  );
        }

  }

     rec1.nome[0] = recorde[0];
     rec1.nome[1] = recorde[1];
     rec1.nome[2] = recorde[2];
     rec1.nome[3] = '\0';

     rec1.pontuacao = *pontuacao;

     insere(&array,rec1);

    if((p_arquivo = fopen(nome_arquivo, "w")) == NULL)
    {
        perror("fopen:");
        SDL_Quit();
        return ;
    }

    fwrite(&array, sizeof(ARRAY), 1, p_arquivo);
    fclose(p_arquivo);

  return;
}


void mostra_recorde(SDL_Surface *screen)
{

  int i;
  SDL_Event event;
  TTF_Font *fonte;
  fonte = TTF_OpenFont("japonese.ttf",25);
  SDL_Color textColor = { 0, 0, 0 };

  SDL_Surface *FundoRecorde = IMG_Load("Imagens/fundoHS.png");


    fps.startTicks = 0;
    fps.pausedTicks = 0;
    fps.paused = 0;
    fps.started = 0;


    char *nome_arquivo = "Recordes.dat";
    FILE *p_arquivo;
    ARRAY array;
    array.n = 0;

    if((p_arquivo = fopen(nome_arquivo, "r")) != NULL)
    {
        fread(&array, sizeof(ARRAY), 1, p_arquivo);
        fclose(p_arquivo);
    }
    else
    {
        printf("DEU ERRO\n");//Recordes.dat
        SDL_Quit();
        return ;
    }


    RECORDE rec1;

    Figura figura_recorde[MAX_ARRAY];
    char temp[50];
    for(i = 0;i < MAX_ARRAY;i++)
    {
        figura_recorde[i].x = 100;
        figura_recorde[i].y = 100 + i * 50;
        rec1 = elemento(&array,i);
        sprintf(temp,"%d - %07d %s",i+1,rec1.pontuacao,rec1.nome);
        figura_recorde[i].imagem = TTF_RenderText_Solid( fonte, temp , textColor );
    }

    int roda = 1;


  while(roda)
  {

     timer_start(&fps); // começa a contagem do vsync

      while (SDL_PollEvent(&event))
      {
	if(event.type == SDL_KEYDOWN)
        {

		if(event.key.keysym.sym == SDLK_RETURN)
		{
			  roda = 0;
		}
	}

      }



            SDL_BlitSurface(FundoRecorde,NULL,screen,NULL);



            for(i=0;i<tamanho(&array);i++)
            {
                apply_surface(figura_recorde[i],screen);
            }

            SDL_Flip(screen) ; // atualiza a tela


        if( (timer_get_ticks(&fps) < 1000 / FPS ) )
        {
                SDL_Delay( ( 1000 / FPS ) - timer_get_ticks(&fps)  );
        }

  }

  return;

