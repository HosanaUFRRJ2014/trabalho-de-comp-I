#include "Monstros.c"

//#include "timer.c"
//#include "figura.c"

#define LinhasDoCampo 15
#define ColunasDoCampo 19

#define MultiplicadorDeX 40 // numero encontrado para poder dimensionar a tela corretamente: 41.5
#define MultiplicadorDeY 40 //numero encontrado para poder dimensionar a tela corretamente: 40

#define Constante_de_aumento 110
#define VPP 2 // Velocidade Por Pixel

#define TemBomba 7

#define TEMPOBOMBA 10
#define BOMBACOMUM 1
#define TEMPOEXPLOSAO 10
#define TempoHurted 15

#define NumeroMorcegos 6

#define CoracoesW 200
#define CoracoesH 30

#define PontoAcertouMonstro 100
#define PontoJogadorBurro -100
#define PontoAcertuDestrutivel 50

#define OxQuantidadeMorcego 255 //160
#define OyQuantidadeMorcego 40

#define OxBonus 100
#define OyBonus 70

#define BonusPontuacao 3000


// FASE2+
/*#define NumeroRobos 1
#define DistanciaGarraHorizontalNaTela 53*2
#define DistanciaGarraVerticalNaTela 40*3*/

#define OxRectLife 110
#define OyRectLife 5 //50
#define NumFase 1

#define CaminhoW 53
#define CaminhoH 40

#define VelocidadePersnagem 3
#define QtdTotalDeMosntros 6
#define TempoPAdraoAgarrado 30

#define OxPontuacao 620 //99
#define OyPontuacao 32 //32
#define OxTempo 395 //367
#define OyTempo 32 //32

#define NumeroDeDestrutiveis 9


//string de compilação:  gcc Main.c -o MainColisao -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer


typedef struct
{
	int TipoBomba;
	int TempoBomba;
	int Ativa;
	int Explodindo;
	int TempoExplosao;
	SDL_Rect Rect;

	SDL_Rect RectExplosaoEsquerda;
	SDL_Rect RectExplosaoEsquerdaFinal;
	SDL_Rect RectExplosaoDireita;
	SDL_Rect RectExplosaoDireitaFinal;
	SDL_Rect RectExplosaoCima;
	SDL_Rect RectExplosaoCimaFinal;
	SDL_Rect RectExplosaoBaixo;
	SDL_Rect RectExplosaoBaixoFinal;

	SDL_Rect CortedaExplosao;

	PosicaoDoMapa PosicaoAtual;
}Bombas;

typedef struct
{
	SDL_Rect Rect;
	int HP;
	PosicaoDoMapa PosicaoAtual;
	PosicaoDoMapa ProximaPosicao;
	int DirecaoQueVai;
	int VelX;
	int VelY;
	int Parado;
	int Preso;
	SDLKey UltimaHorizontalApertada;
	int Hurted;
}Personagem;

typedef struct
{
	Personagem P;
	int TempoEspera;
	int EhFigurante;
}Figurante;

typedef struct
{
	int EstahAtivo;
	SDL_Rect Rect;
	PosicaoDoMapa PosicaoAtual;
}Item;

void atualizacao_coracoes(int vida, SDL_Surface *imagem_coracao, SDL_Surface *screen)
{

    SDL_Rect posicao_HP;
    posicao_HP.x = OxRectLife;
    posicao_HP.y = OyRectLife;

    SDL_Rect corte_HP; // Corte feito no HP para possibilitar o aumento e a diminuição da vida.
    corte_HP.x = 0;
    corte_HP.y = 0;
    corte_HP.w = vida;
    corte_HP.h = CoracoesH;

    SDL_BlitSurface(imagem_coracao,&corte_HP,screen,&posicao_HP);
}


int AtaqueMonstros(Personagem P, Monstro M)
{
	if(!P.Hurted)
	{
		switch(M.DirecaoQueVeio)
		{
			case CIMA:
			{
				if(M.Rect.y + M.Rect.h/2 >= P.Rect.y && M.Rect.y - P.Rect.y < 0)
					if( (P.Rect.x - (M.Rect.x + M.Rect.w) < 5 && P.Rect.x - M.Rect.x > 0) || (P.Rect.x - M.Rect.x > -15 && P.Rect.x - M.Rect.x < 0) )
						return 1;
				break;
			}
			case DIREITA:
			{
				if( M.Rect.x <= P.Rect.x + P.Rect.w && P.Rect.x - M.Rect.x < 0 )
					if( (P.Rect.y - (M.Rect.y + M.Rect.h/2) < 5 && P.Rect.y - M.Rect.y > 0) || ( (P.Rect.y + P.Rect.h/2) - M.Rect.y > -10 && P.Rect.y - M.Rect.y < 0) )
						return 1;
				break;
			}
			case BAIXO:
			{
				if(M.Rect.y <= P.Rect.y + P.Rect.h/2 && P.Rect.y - M.Rect.y < 0)
					if( (P.Rect.x - (M.Rect.x + M.Rect.w) < 5 && P.Rect.x - M.Rect.x > 0) || (P.Rect.x - M.Rect.x > -15 && P.Rect.x - M.Rect.x < 0) )
						return 1;
				break;
			}
			case ESQUERDA:
			{
				if(M.Rect.x + M.Rect.w >= P.Rect.x && M.Rect.x - P.Rect.x < 0)
					if( (P.Rect.y - (M.Rect.y + M.Rect.h/2) < 5 && P.Rect.y - M.Rect.y > 0) || ( (P.Rect.y + P.Rect.h/2) - M.Rect.y > -10 && P.Rect.y - M.Rect.y < 0) )
						return 1;
				break;
			}
		}
	}
	return 0;
}


int VaiTrocarPosicaoMonstro(SDL_Rect M, SDL_Rect rect, int DirecaoQueVeio, int restricao)
{
	if(restricao == TemMonstro)
		return 0;
	else
	{
		switch(DirecaoQueVeio)
		{
			case CIMA:
			{
				if(M.y + M.h > rect.y)
				{
					if(M.y > rect.y)
						return 1;
					else
						return 2;
				}
				else
					return 0;

				break;
			}
			case DIREITA:
			{
				if(M.x < rect.x + rect.w)
				{
					if(M.x + M.w < rect.x + rect.w)
						return 1;
					else
						return 2;
				}
				else
					return 0;

				break;
			}
			case BAIXO:
			{
				if(M.y < rect.y + rect.h)
				{
					if(M.y + M.h < rect.y + rect.h)
						return 1;
					else
						return 2;
				}
				else
					return 0;

				break;
			}
			case ESQUERDA:
			{
				if(M.x + M.w > rect.x)
				{
					if(M.x > rect.x)
						return 1;
					else
						return 2;
				}
				else
					 return 0;

				break;
			}
		}
	}
}

int VaiTrocarPosicaoPlayer(SDL_Rect P, SDL_Rect rect, int DirecaoQueVai, int restricao)
{
	switch(DirecaoQueVai)
	{
		case CIMA:
		{
			if( (restricao == 0) || (restricao == TemMonstro) )
			{
				if(P.y + P.w <= rect.y + rect.h - 2)
					return 1;
			}
			else
				if(P.y <= rect.y + rect.h)
					return -1;
			break;
		}
		case DIREITA:
		{
			if(restricao == 0 || (restricao == TemMonstro))
			{
				if(P.x + P.w >= rect.x)
					return 1;
			}
			else
				if(P.x + P.w >= rect.x - 2)
					return -1;
			break;
		}
		case BAIXO:
		{
			if(restricao == 0 || (restricao == TemMonstro))
			{
				if(P.y + P.h >= rect.y)
					return 1;
			}
			else
				if(P.y + P.h >= rect.y - 2)
					return -1;
			break;
		}
		case ESQUERDA:
		{
			if(restricao == 0 || (restricao == TemMonstro))
			{
				if(P.x + P.w <= rect.x + rect.w)
					return 1;
			}
			else
				if(P.x <= rect.x + rect.w - 4)
					return -1;
			break;
		}
	}

	return 0;
}

int Fase1(SDL_Surface *screen, int EstadoOpcoes[2], int *Pontos)
//vetor de opções com {SFX(1/0), Music(1/0)
{

	int quit = 0;
	SDL_Event evento;
	screen = SDL_SetVideoMode(760,600+Constante_de_aumento,16,SDL_HWSURFACE | SDL_DOUBLEBUF);//800
	
	int quitHistoria = 0;
        int ContadorHistoria = 0;
        int ContadorAnimacaoHistoria = 0;
	
        SDL_Rect dest;
        dest.x = 100;
        dest.y = 50;
	SDL_Surface  *surQuadro[21];
	SDL_Event eventohistoria;

	
	TTF_Font *fonte = TTF_OpenFont( "japonese.ttf", 25);
	
	
	//============================================
	//Gritos das pessoas.
	Mix_Chunk *grito_2,*grito_4,*grito_5,*grito_6,*explosao;
	grito_2 = Mix_LoadWAV( "SonsJogo/Grito2.wav");
	grito_4 = Mix_LoadWAV( "SonsJogo/Grito4.wav");
	grito_5 = Mix_LoadWAV( "SonsJogo/Grito5.wav");
	grito_6 = Mix_LoadWAV( "SonsJogo/Grito6.wav");
	explosao = Mix_LoadWAV( "SonsJogo/explosao.wav" );
	
	//============================================
	
	
	surQuadro[0] = IMG_Load("Imagens/Historinha/quadrinho1.png");
	surQuadro[1] = IMG_Load("Imagens/Historinha/quadrinho2.png");
	surQuadro[2] = IMG_Load("Imagens/Historinha/quadrinho3.png");
	surQuadro[3] = IMG_Load("Imagens/Historinha/quadrinho4.png");
	surQuadro[4] = IMG_Load("Imagens/Historinha/quadrinho5.png");
	surQuadro[5] = IMG_Load("Imagens/Historinha/quadrinho6.png");
	surQuadro[6] = IMG_Load("Imagens/Historinha/quadrinho7.png");
	surQuadro[7] = IMG_Load("Imagens/Historinha/quadrinho8.png");
	surQuadro[8] = IMG_Load("Imagens/Historinha/quadrinho9.png");
	surQuadro[9] = IMG_Load("Imagens/Historinha/quadrinho10.png");
	surQuadro[10] = IMG_Load("Imagens/Historinha/quadrinho11.png");
	surQuadro[11] = IMG_Load("Imagens/Historinha/quadrinho21.png");
	//surQuadro[12] = IMG_Load("Imagens/Historinha/quadrinho13.png");
	surQuadro[13] = IMG_Load("Imagens/Historinha/quadrinho14.png");
	surQuadro[14] = IMG_Load("Imagens/Historinha/quadrinho15.png");
	surQuadro[15] = IMG_Load("Imagens/Historinha/quadrinho16.png");
	surQuadro[16] = IMG_Load("Imagens/Historinha/quadrinho16b.png");
	surQuadro[17] = IMG_Load("Imagens/Historinha/quadrinho18.png");
	surQuadro[18] = IMG_Load("Imagens/Historinha/quadrinho19.png");

	
	desenha_texto("Aperte ENTER para pular a historia",screen,170,0,fonte);
	desenha_texto("Use as setas esquerda e direita para passar a historia",screen,70,650,fonte);
	SDL_BlitSurface(surQuadro[0], NULL, screen, &dest);
	SDL_Flip(screen);

	while(!quitHistoria)
	{
		while (SDL_PollEvent(&eventohistoria))
	  {
     		if(eventohistoria.type == SDL_KEYDOWN)
     		{	
            if(eventohistoria.key.keysym.sym == SDLK_RIGHT)
            	ContadorHistoria++;
           
            if(eventohistoria.key.keysym.sym == SDLK_LEFT && ContadorHistoria > 0)
            {
            	ContadorHistoria--;
            	ContadorAnimacaoHistoria = 0;
            }	
            	
            //if(eventohistoria.key.keysym.sym == SDLK_ESCAPE)
                 //quitHistoria = 1;

            if(eventohistoria.key.keysym.sym == SDLK_RETURN)
            	ContadorHistoria = 20;

            if(ContadorHistoria == 0)
                SDL_BlitSurface(surQuadro[0], NULL, screen, &dest);
            if(ContadorHistoria == 1)
                SDL_BlitSurface(surQuadro[1], NULL, screen, &dest);
            if(ContadorHistoria == 2)
                SDL_BlitSurface(surQuadro[2], NULL, screen, &dest);
            if(ContadorHistoria == 3)
                SDL_BlitSurface(surQuadro[3], NULL, screen, &dest);
            if(ContadorHistoria == 4)
                SDL_BlitSurface(surQuadro[4], NULL, screen, &dest);
            if(ContadorHistoria == 5)
                SDL_BlitSurface(surQuadro[5], NULL, screen, &dest);
            if(ContadorHistoria == 6)
                SDL_BlitSurface(surQuadro[6], NULL, screen, &dest); 
            if(ContadorHistoria == 7)
                SDL_BlitSurface(surQuadro[7], NULL, screen, &dest);

            //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& AQUI COMEÇAM OS GRITOS DAS PESSOAS &&&&&&&&&&&&&&&&&&&&&&
            if(ContadorHistoria == 8)
                SDL_BlitSurface(surQuadro[8], NULL, screen, &dest);
	    
            if(ContadorHistoria == 9)
	    {
                SDL_BlitSurface(surQuadro[9], NULL, screen, &dest);
		Mix_PlayChannel(-1,grito_5,0);
		
	    }
            if(ContadorHistoria == 10)
	    {
                SDL_BlitSurface(surQuadro[10], NULL, screen, &dest);
		Mix_PlayChannel(-1,grito_2,0);
		
	    }
            if(ContadorHistoria == 11 || ContadorHistoria == 13 || ContadorHistoria == 15)
            	SDL_BlitSurface(surQuadro[11], NULL, screen, &dest);
		
            if(ContadorHistoria == 12)
	    {
                SDL_BlitSurface(surQuadro[13], NULL, screen, &dest);
		Mix_PlayChannel(-1,grito_6,0);
	    }
            if(ContadorHistoria == 14)
	    {
                SDL_BlitSurface(surQuadro[14], NULL, screen, &dest);
		Mix_PlayChannel(-1,grito_4,0);
	    }
		 
            if(ContadorHistoria == 16)
	    {
                SDL_BlitSurface(surQuadro[15], NULL, screen, &dest);
		
	    }
            if(ContadorHistoria == 17)
                SDL_BlitSurface(surQuadro[16], NULL, screen, &dest);
	    

            //AQUI TERMINAM OS GRITOS
            if(ContadorHistoria == 18)
                SDL_BlitSurface(surQuadro[17], NULL, screen, &dest);
            if(ContadorHistoria == 19)
	    {  
                SDL_BlitSurface(surQuadro[18], NULL, screen, &dest);
		Mix_PlayChannel(-1,explosao,0);
	    }
            if(ContadorHistoria == 20)
                quitHistoria = 1;
            }

            if(eventohistoria.key.keysym.sym == SDL_KEYUP)
            	ContadorHistoria += 0;

            SDL_Flip(screen);

	  }    	

	}
  
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,100,0,0));
    SDL_Flip(screen);
     
	 Mix_FreeChunk(grito_2);
	 Mix_FreeChunk(grito_4);
	 Mix_FreeChunk(grito_5);
	 Mix_FreeChunk(grito_6);
	
	 
    if(quitHistoria == 1)
    {
	//Músicas e efeitos de som
	//===========================================
	Mix_Chunk *por_bomba, *iten,*iten_coracao;
	Mix_Music *Parte1_1;

	//=======================================
	//explosao = Mix_LoadWAV( "SonsJogo/explosao.wav" );
	por_bomba = Mix_LoadWAV( "SonsJogo/Por_bomba.wav" );
	Parte1_1 = Mix_LoadMUS("SonsJogo/Parte1_1.ogg");
	iten = Mix_LoadWAV("SonsJogo/iten.wav");
	iten_coracao = Mix_LoadWAV("SonsJogo/coracao.wav");
	//=========================================

	//Condições referidas as opções do som.

	if(EstadoOpcoes[0] == 0)
	  Mix_Volume(-1, 0);
	else
	  Mix_Volume(-1, 128);

	if(EstadoOpcoes[1] != 0)
	  Mix_PlayMusic(Parte1_1, -1);
	//======================================


	//lê as imagens para os blocos do campo
	SDL_Surface *surCaminho = IMG_Load("Imagens/Cenario/rua.png");
	SDL_Surface *surCerca = IMG_Load("Imagens/Cenario/cerca.png");
	SDL_Surface *surCasa1 = IMG_Load("Imagens/Cenario/casa_amarela.png");
	SDL_Surface *surCasa2 = IMG_Load("Imagens/Cenario/casa_azul.png");
	SDL_Surface *surCasa3 = IMG_Load("Imagens/Cenario/casa_branca.png");
	SDL_Surface *surCasa4 = IMG_Load("Imagens/Cenario/casa_rosa.png");
	SDL_Surface *surCasa5 = IMG_Load("Imagens/Cenario/casa_roxa.png");
	SDL_Surface *surCasa6 = IMG_Load("Imagens/Cenario/casa_verde.png");
	SDL_Surface *surGrama = IMG_Load("Imagens/Cenario/grama.png");

//------------------------------------IMAGENS BARRA SUPERIOR-----------------------
	//Func HP
	SDL_Surface *surLife = IMG_Load("Imagens/BarraSuperior/Vida.png");

//------------------------------------RECTS DA BARRA SUPERIOR----------------------
	SDL_Rect rectBarraSuperior; //barra superior onde irá ficar as informações do jogo em si.
    rectBarraSuperior.x = 0;
    rectBarraSuperior.y = 0;
    rectBarraSuperior.w = 800; //tamanho da largura da tela.
    rectBarraSuperior.h = Constante_de_aumento;


	SDL_Rect rectLife;
	rectLife.x = OxRectLife;
	rectLife.y = OyRectLife;//Func HP    onde a barra de vida fica

//-----------------------------------IMAGENS PERSONAGEM-----------------------------
	SDL_Surface *surPlayer[4][3];
	surPlayer[CIMA][0] = IMG_Load("Imagens/Personagem/boyParCos.png");
	surPlayer[CIMA][1] = IMG_Load("Imagens/Personagem/boyMovCos1.png");
	surPlayer[CIMA][2] = IMG_Load("Imagens/Personagem/boyMovCos2.png");
	surPlayer[DIREITA][0] = IMG_Load("Imagens/Personagem/boyParDir.png");
	surPlayer[DIREITA][1] = IMG_Load("Imagens/Personagem/boyMovDir1.png");
	surPlayer[DIREITA][2] = IMG_Load("Imagens/Personagem/boyMovDir2.png");
	surPlayer[BAIXO][0] = IMG_Load("Imagens/Personagem/boyParFren.png");
	surPlayer[BAIXO][1] = IMG_Load("Imagens/Personagem/boyMovFren1.png");
	surPlayer[BAIXO][2] = IMG_Load("Imagens/Personagem/boyMovFren2.png");
	surPlayer[ESQUERDA][0] = IMG_Load("Imagens/Personagem/boyParEsq.png");
	surPlayer[ESQUERDA][1] = IMG_Load("Imagens/Personagem/boyMovEsq1.png");
	surPlayer[ESQUERDA][2] = IMG_Load("Imagens/Personagem/boyMovEsq2.png");

//------------------------------------CRIANDO PLAYER-------------------------------

	Personagem Player;
	Player.HP = CoracoesW;
	Player.DirecaoQueVai = DIREITA;
	Player.PosicaoAtual.i = 1;
	Player.PosicaoAtual.j = 1;
	Player.Parado = 1;
	Player.VelX = 0;
	Player.VelY = 0;
	Player.ProximaPosicao.i = Player.PosicaoAtual.i;
	Player.ProximaPosicao.j = Player.PosicaoAtual.j + 1;
	Player.Preso = 0;
	Player.UltimaHorizontalApertada = SDLK_SPACE;
	Player.Hurted = 0;

//------------------------------------IMAGENS MONSTROS------------------------------
	SDL_Surface *surMorcego[4][2];

	//estado 0 e estado 1 para a animação
	//defines indexadoras definidas em Monstros.c
	surMorcego[CIMA][0] = IMG_Load("Imagens/Monstros/Morcegos/MorCos1.png");
	surMorcego[CIMA][1] = IMG_Load("Imagens/Monstros/Morcegos/MorCos3.png");
	surMorcego[DIREITA][0] = IMG_Load("Imagens/Monstros/Morcegos/MorDir1.png");
	surMorcego[DIREITA][1] = IMG_Load("Imagens/Monstros/Morcegos/MorDir3.png");
	surMorcego[BAIXO][0] = IMG_Load("Imagens/Monstros/Morcegos/MorFren1.png");
	surMorcego[BAIXO][1] = IMG_Load("Imagens/Monstros/Morcegos/MorFren3.png");
	surMorcego[ESQUERDA][0] = IMG_Load("Imagens/Monstros/Morcegos/MorEsq1.png");
	surMorcego[ESQUERDA][1] = IMG_Load("Imagens/Monstros/Morcegos/MorEsq3.png");

	SDL_Surface *surMorcegoAtaque[4][4];
	surMorcegoAtaque[CIMA][CIMA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParCosMorCos.png");
	surMorcegoAtaque[CIMA][DIREITA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParCosMorDir.png");
	surMorcegoAtaque[CIMA][ESQUERDA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParCosMorEsq.png");
	surMorcegoAtaque[CIMA][BAIXO] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParCosMorFren.png");
	surMorcegoAtaque[DIREITA][CIMA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParDirMorCos.png");
	surMorcegoAtaque[DIREITA][DIREITA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParDirMorDir.png");
	surMorcegoAtaque[DIREITA][ESQUERDA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParDirMorEsq.png");
	surMorcegoAtaque[DIREITA][BAIXO] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParDirMorFren.png");
	surMorcegoAtaque[ESQUERDA][CIMA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParEsqMorCos.png");
	surMorcegoAtaque[ESQUERDA][DIREITA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParEsqMorDir.png");
	surMorcegoAtaque[ESQUERDA][ESQUERDA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParEsqMorEsq.png");
	surMorcegoAtaque[ESQUERDA][BAIXO] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParEsqMorFren.png");
	surMorcegoAtaque[BAIXO][CIMA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParFrenMorCos.png");
	surMorcegoAtaque[BAIXO][DIREITA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParFrenMorDir.png");
	surMorcegoAtaque[BAIXO][ESQUERDA] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParFrenMorEsq.png");
	surMorcegoAtaque[BAIXO][BAIXO] = IMG_Load("Imagens/Monstros/Morcegos/Ataque/boyParFrenMorFren.png");


//--------------------------CHAMANDO VAMPIRO PARA A ANIMAÇÃO INICIAL----------------------------------------

	SDL_Surface *surVampiro[4][3];

	surVampiro[CIMA][0] = IMG_Load("Imagens/Monstros/Vampiro/VampParCos.png");
	surVampiro[CIMA][1] = IMG_Load("Imagens/Monstros/Vampiro/VampMovCos1.png");
	surVampiro[CIMA][2] = IMG_Load("Imagens/Monstros/Vampiro/VampMovCos2.png");
	surVampiro[DIREITA][0] = IMG_Load("Imagens/Monstros/Vampiro/VampParDir.png");
	surVampiro[DIREITA][1] = IMG_Load("Imagens/Monstros/Vampiro/VampMovDir1.png");
	surVampiro[DIREITA][2] = IMG_Load("Imagens/Monstros/Vampiro/VampMovDir2.png");
	surVampiro[BAIXO][0] = IMG_Load("Imagens/Monstros/Vampiro/VampParFren.png");
	surVampiro[BAIXO][1] = IMG_Load("Imagens/Monstros/Vampiro/VampMovFren1.png");
	surVampiro[BAIXO][2] = IMG_Load("Imagens/Monstros/Vampiro/VampMovFren2.png");
	surVampiro[ESQUERDA][0] = IMG_Load("Imagens/Monstros/Vampiro/VampParEsq.png");
	surVampiro[ESQUERDA][1] = IMG_Load("Imagens/Monstros/Vampiro/VampMovEsq1.png");
	surVampiro[ESQUERDA][2] = IMG_Load("Imagens/Monstros/Vampiro/VampMovEsq2.png");


//----------------------------------CRIANDO MONSTROS-------------------------------------------
	Monstro Morcegos[NumeroMorcegos];

	//PARA FASE2+
	//Mestre Robo;

//----------------------------------IMAGENS BOMBA-----------------------------------------------
	SDL_Surface *surBomba;
	SDL_Surface *surExplosao[5];
	surBomba = IMG_Load("Imagens/Bombas/bomba_simples0.png");

	surExplosao[0] = IMG_Load("Imagens/Bombas/explosao1teste.png");
	surExplosao[1] = IMG_Load("Imagens/Bombas/explosao2teste.png");
	surExplosao[2] = IMG_Load("Imagens/Bombas/explosao3teste.png");
	surExplosao[3] = IMG_Load("Imagens/Bombas/explosao4teste.png");
	surExplosao[4] = IMG_Load("Imagens/Bombas/explosao5teste.png");

	Bombas BombaComum;
	BombaComum.TempoBomba = TEMPOBOMBA;
	BombaComum.Ativa = 0;
	BombaComum.Explodindo = 0;
	BombaComum.TipoBomba = BOMBACOMUM;
	BombaComum.TempoExplosao = TEMPOEXPLOSAO;

//------------------------------------IMAGENS DO ITEM DE LIFE---------------------------------
	SDL_Surface *surItem[2];

	surItem[0] = IMG_Load("Imagens/Itens/Life0.png"); //caminho do coração pequeno
	surItem[1] = IMG_Load("Imagens/Itens/Life1.png"); //caminho do coração maior

	Item Itens[NumeroDeDestrutiveis];
	int NumDestrutiveisDestruidos = 0;

	int d;
	for(d=0;d<NumeroDeDestrutiveis;d++)
		Itens[d].EstahAtivo = 0;

//----------------------------------CRIANDO MAPA E CAMPO---------------------------------------
	 //defines indexadoras definidas em Monstro.c
	//define a matriz que vai mapear a tela. Sendo 0 = caminho; 1 = indestrutivel; 2 = destrutivel
	int mapa[LinhasDoCampo][ColunasDoCampo] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
									     		{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 0, 2, 1},
									     		{1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1},
									     		{1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 2, 1},
									     		{1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1},
									     		{1, 0, 1, 2, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
									     		{1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
									     		{1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
									     		{1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1},
									     		{1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1},
									     		{1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1},
									     		{1, 0, 1, 1, 1, 1, 0, 1, 2, 0, 1, 1, 0, 0, 1, 1, 2, 0, 1},
									     		{1, 0, 2, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 2, 1, 0, 1},
									     		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 0, 0, 1},
									     		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };



	//define a matriz de rect que vai dimensionar a tela...
	SDL_Rect campo[LinhasDoCampo][ColunasDoCampo];
	int i = 0, j = 0;

	//define as posições dos rects do campo
	for(i=0;i<LinhasDoCampo;i++)
		for(j=0;j<ColunasDoCampo;j++)
		{
			campo[i][j].x = MultiplicadorDeX*j;
			campo[i][j].y = MultiplicadorDeY*i + Constante_de_aumento;
		}


//----------------------------------------CONFIGURA MONSTROS----------------------------------

	srand(time(NULL));

	int m, n, cont = 0;
	int DiminuiResistencia = 0;
	for(m=0;m<NumeroMorcegos;m++)
	{
		Morcegos[m].TipoDoMonstro = MORCEGO;
		Morcegos[m].Ataque = 0;
		Morcegos[m].DanoQueCausa = 1;
		Morcegos[m].Resistencia = 200;
		Morcegos[m].EstahVivo = 1;
		Morcegos[m].TempoAtaque = TempoPAdraoAgarrado;

		int a = 0; int b = 0;
		PosicaoDoMapa Temp;
		while(mapa[a][b]!=0)
		{
    		a = rand() % 14;
    		b = rand() % 18;
    		Temp.i = a;
    		Temp.j = b;

    		if(PosicaoDoMapaEhIgual(Player.PosicaoAtual, Temp))
    			continue;

    		if(a <= 2)
    			continue;

    		if(cont!=0)
    		{
    			for(n=0;n<cont;n++)
    			{
    				if(Morcegos[n].PosicaoAtual.i == Temp.i)
    					a = 0;
    				if(Morcegos[n].PosicaoAtual.j == Temp.j)
    					b = 0;
    			}
    		}
		}
		cont++;
		Morcegos[m].PosicaoAtual = Temp;
		Morcegos[m].Rect.x = campo[a][b].x;
		Morcegos[m].Rect.y = campo[a][b].y;
		mapa[a][b] = TemMonstro;
		Morcegos[m].DirecaoQueVeio = -1;
	}

//-----------------------------------------PRIMEIRA BLITAGEM DA TELA----------------------
	//blita a barra superior (no caso só pinta de cinza)
	SDL_FillRect(screen,&rectBarraSuperior, SDL_MapRGB(screen->format,107,51,102));
	SDL_BlitSurface(surLife, NULL, screen, &rectLife); //Func HP    primeira blitagem na tela

	//insere a grama ao redor
	i=0;
	while(1==1)
	{
		for(j=0;j<ColunasDoCampo;j++)
			SDL_BlitSurface(surGrama, NULL, screen, &campo[i][j]);
		if(i==14)
			break;
		else
			i=14;
	}
	j = 0;
	while(1==1)
	{
		for(i=0;i<LinhasDoCampo;i++)
			SDL_BlitSurface(surGrama, NULL, screen, &campo[i][j]);
		if(j==18)
			break;
		else
			j=18;
	}

	//insere as demais imagens nas demais posições
	for (i=1;i<LinhasDoCampo-1;i++)
	{
		int cont=0;
		for(j=1;j<ColunasDoCampo-1;j++)
		{
			SDL_BlitSurface(surCaminho, NULL, screen, &campo[i][j]);

			if(mapa[i][j] == 1)
			{
				cont++;
				if(cont == 7)
					cont = 1;
				switch(cont)
				{
					case 1:
					{
						SDL_BlitSurface(surCasa1, NULL, screen, &campo[i][j]);
						break;
					}
					case 2:
					{
						SDL_BlitSurface(surCasa2, NULL, screen, &campo[i][j]);
						break;
					}
					case 3:
					{
						SDL_BlitSurface(surCasa3, NULL, screen, &campo[i][j]);
						break;
					}
					case 4:
					{
						SDL_BlitSurface(surCasa4, NULL, screen, &campo[i][j]);
						break;
					}
					case 5:
					{
						SDL_BlitSurface(surCasa5, NULL, screen, &campo[i][j]);
						break;
					}
					case 6:
					{
						SDL_BlitSurface(surCasa6, NULL, screen, &campo[i][j]);
						break;
					}
				}
			}
			else
				if(mapa[i][j] == 2)
					SDL_BlitSurface(surCerca, NULL, screen, &campo[i][j]);
		}
	}

	//blita personagem
	Player.Rect.x = campo[Player.PosicaoAtual.i][Player.PosicaoAtual.j].x + 10;
	Player.Rect.y = campo[Player.PosicaoAtual.i][Player.PosicaoAtual.j].y + 10;
	SDL_BlitSurface(surPlayer[Player.DirecaoQueVai][0], NULL, screen, &Player.Rect);

	//blita monstros
	srand(time(NULL));
	for(m=0;m<NumeroMorcegos;m++)
	{
		PosicaoDoMapa PosicoesDoMapaParaSeguir[3]; //esse vetor conterá as posições do mapa que pode ir;
		int DirecaoQueVai[3];
		int seguir; //essa variável vai receber um valor aleatório que vai indicar para qual lado o monstro deve seguir
		int LimiteParaSeguir = NumeroDeDirecoesPraSeguir(Morcegos[m].PosicaoAtual, PosicoesDoMapaParaSeguir, mapa, Morcegos[m].DirecaoQueVeio, DirecaoQueVai, MORCEGO);

		seguir = rand() % LimiteParaSeguir;

		if(seguir==3)
			seguir=2;

		Morcegos[m].ProximaPosicao = PosicoesDoMapaParaSeguir[seguir];
		Morcegos[m].DirecaoQueVeio = DirecaoQueVai[seguir];

		switch(Morcegos[m].DirecaoQueVeio)
		{
			case CIMA:
			{
				SDL_BlitSurface(surMorcego[BAIXO][0], NULL, screen, &Morcegos[m].Rect);
				break;
			}
			case DIREITA:
			{
				SDL_BlitSurface(surMorcego[ESQUERDA][0], NULL, screen, &Morcegos[m].Rect);
				break;
			}
			case BAIXO:
			{
				SDL_BlitSurface(surMorcego[CIMA][0], NULL, screen, &Morcegos[m].Rect);
				break;
			}
			case ESQUERDA:
			{
				SDL_BlitSurface(surMorcego[DIREITA][0], NULL, screen, &Morcegos[m].Rect);
				break;
			}
		}
	}


//------------------------------------- INICIO DO LOOP -----------------------------
	int EstadoAnimacaoMonstros = 0, ControladorDeTempoAnimacaoMonstros = 0;
	int EstadoAnimacaoBomba = 0, ControladorDeTempoAnimacaoBomba = 0;
	int EstadoAnimacaoPlayer = 1, ControladorDeTempoAnimacaoPlayer = 0;


	int AumentaExplosaoEsquerda = 0, AumentaExplosaoEsquerdaFinal = 0;
	int AumentaExplosaoDireita = 0, AumentaExplosaoDireitaFinal = 0;
	int AumentaExplosaoCima = 0, AumentaExplosaoCimaFinal = 0;
	int AumentaExplosaoBaixo = 0, AumentaExplosaoBaixoFinal = 0;

	int EstadoAnimacaoExplosao = 0;


	int AtingiuMonstro = 0;
	int AtingiuDestrutivel = 0;
	int AtingiuPlayer = 0;


	SDL_Rect CorteExplosao;
	SDL_Rect MioloExplosao;
	SDL_Rect ExplosaoEsquerda, ExplosaoEsquerdaFinal;
	SDL_Rect ExplosaoDireita, ExplosaoDireitaFinal;
	SDL_Rect ExplosaoCima, ExplosaoCimaFinal;
	SDL_Rect ExplosaoBaixo, ExplosaoBaixoFinal;


	SDL_Event event;
	mapa[Player.PosicaoAtual.i][Player.PosicaoAtual.j] = TemPersonagem;

	int EstadoAnimacaoPlayerHurt = 0, ControladorDeTempoAnimacaoPlayerHurt = 0;

//-----------------------------VARIAVEIS DE CONTROLE TEMPO/SCORE---------------------

      time_t t;
     srand((unsigned) time(&t));

    // coisas  de vsync
    Timer fps;
    fps.startTicks = 0;
    fps.pausedTicks = 0;
    fps.paused = 0;
    fps.started = 0;

    //Para poder blitar o score na tela.
    TTF_Font *font =TTF_OpenFont( "japonese.ttf", 20);
    SDL_Color textColor = { 255, 139, 51 };
    char pontos_texto[7];

    sprintf(pontos_texto,"%06d",(*Pontos) );
    Figura pontuacao;
    pontuacao.imagem = TTF_RenderText_Solid( font, pontos_texto , textColor );
    pontuacao.x = OxPontuacao;
    pontuacao.y = OyPontuacao;

    //tempo para terminar a fase
    //Variáveis necessárias para poder blitar o tempo na tela.
    int tempo = 0; // o tempo sempre começará do zero(o tempo que irá se modificar)
    int tempo_fps = 0; //serve para controlar a velocidade do tempo nos loops
    char tempo_texto[6]; //necessário para armazenar o texto do tempo.
    sprintf(tempo_texto,"%02d:%02d",tempo/60,tempo%60);

    //Necessário para blitar a figura do tempo na tela!
    Figura tempo_figura;
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    tempo_figura.imagem = TTF_RenderText_Solid( font, tempo_texto , textColor );
    tempo_figura.x = OxTempo;
    tempo_figura.y = OyTempo;

    int morcegos = NumeroMorcegos;
    char QuantidadeMorcegosRestantes[2];
    sprintf(QuantidadeMorcegosRestantes,"%01d",morcegos);

    Figura quantidade_morcego;
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    quantidade_morcego.imagem = TTF_RenderText_Solid(font,QuantidadeMorcegosRestantes,textColor);
    quantidade_morcego.x = OxQuantidadeMorcego; //90
    quantidade_morcego.y = OyQuantidadeMorcego; //40

    int PontuacaoBonus = BonusPontuacao;
    char Bonus[5];
    sprintf(Bonus,"%04d",PontuacaoBonus);

    Figura bonus;
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    bonus.imagem = TTF_RenderText_Solid(font,Bonus,textColor);
    bonus.x = OxBonus;
    bonus.y = OyBonus;

    //SNK





    //ESCREVER    OS INDICADORES DE PONTUAÇÃO E TEMPO
    apply_texto(&pontuacao,screen);
    apply_texto(&tempo_figura,screen);
    apply_texto(&quantidade_morcego,screen);
    apply_texto(&bonus,screen);

    //==============================================
	//Escrever "tempo" e "socre" na tela.

	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	desenha_texto("TEMPO", screen, 390, OyRectLife,font);
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	desenha_texto("SCORE", screen, 630, OyRectLife,font);
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	desenha_texto("PLAYER:", screen, 10,OyRectLife,font);
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	desenha_texto("FALTAM MORCEGOS:",screen,10,OyQuantidadeMorcego,font);
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	desenha_texto("BONUS:",screen,10,OyBonus,font);

    //==============================================



    SDL_Flip(screen);
    SDL_Delay(2000);

	while(!quit)
	{
		int ContadorDeMonstrosMortos = 0;
//--------------------------------CONTROLADORES DE ANIMAÇÃO--------------------------
		//CONTROLE DE ANIMAÇÃO DOS MONSTROS
		if(ControladorDeTempoAnimacaoMonstros == 2)
		{
			if(EstadoAnimacaoMonstros == 1)
				EstadoAnimacaoMonstros = 0;
			else
				EstadoAnimacaoMonstros = 1;

			ControladorDeTempoAnimacaoMonstros = 0;
		}
		else
			ControladorDeTempoAnimacaoMonstros++;

		//CONTROLE DA ANIMAÇÃO DA BOMBA
		if(BombaComum.Ativa)
		{
			if(ControladorDeTempoAnimacaoBomba == BombaComum.TempoBomba)
			{
				if(EstadoAnimacaoBomba == 1)
					EstadoAnimacaoBomba = 0;
				else
					EstadoAnimacaoBomba = 1;

				if(BombaComum.TempoBomba > 0)
					BombaComum.TempoBomba--;
				else
				{
					BombaComum.Ativa = 0;
					BombaComum.Explodindo = 1;

					//A bomba vai explodir!!!!!!!
					//==============================
					  Mix_PlayChannel(-1,explosao,0);
					//==============================

					BombaComum.TempoBomba = TEMPOBOMBA;
					mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j] = 0;
				}

				ControladorDeTempoAnimacaoBomba = 0;
			}
			else
				ControladorDeTempoAnimacaoBomba++;
		}

		//CONTROLE DA ANIMACAO DO PERSONAGEM
		if(!Player.Parado)
		{
			if(ControladorDeTempoAnimacaoPlayer == 3)
			{
				if(EstadoAnimacaoPlayer == 1)
					EstadoAnimacaoPlayer = 2;
				else
					if(EstadoAnimacaoPlayer == 2)
						EstadoAnimacaoPlayer = 1;

				ControladorDeTempoAnimacaoPlayer = 0;
			}
			else
				ControladorDeTempoAnimacaoPlayer++;
		}

		if(Player.Hurted == 1)
		{
			if(ControladorDeTempoAnimacaoPlayerHurt != TempoHurted)
			{
				if(EstadoAnimacaoPlayerHurt == 0)
					EstadoAnimacaoPlayerHurt = 1;
				else
					EstadoAnimacaoPlayerHurt = 0;

				ControladorDeTempoAnimacaoPlayerHurt++;
			}
			else
			{
				Player.Hurted = 0;
				EstadoAnimacaoPlayerHurt = 0;
				ControladorDeTempoAnimacaoPlayerHurt = 0;
			}
		}




//------------------------------------------INICIO DOS EVENTOS---------------------------
		while (SDL_PollEvent(&event))
     	{
     		if(event.type == SDL_KEYDOWN)
     		{
     			switch(event.key.keysym.sym)
     			{
     				case SDLK_UP:
     				{
     					if(!Player.Preso)
     					{
     						Player.VelY = - VelocidadePersnagem;
	     					Player.VelX = 0;
	     					Player.Parado = 0;
	     				}
	     				else
	     				{
	     					Player.VelY = 0;
     						Player.VelX = 0;
	     				}

     					Player.DirecaoQueVai = CIMA;
	     				Player.ProximaPosicao.i = Player.PosicaoAtual.i - 1;
	     				Player.ProximaPosicao.j = Player.PosicaoAtual.j;
     					break;
     				}
     				case SDLK_DOWN:
     				{
     					if(!Player.Preso)
 						{
 							Player.VelY = VelocidadePersnagem;
	     					Player.VelX = 0;
	     					Player.Parado = 0;
	     				}
	     				else
	     				{
	     					Player.VelY = 0;
     						Player.VelX = 0;
	     				}
	     				Player.DirecaoQueVai = BAIXO;
     					Player.ProximaPosicao.i = Player.PosicaoAtual.i + 1;
     					Player.ProximaPosicao.j = Player.PosicaoAtual.j;
     					break;
     				}
     				case SDLK_LEFT:
     				{
     					if(!Player.Preso)
     					{
	     					Player.VelY = 0;
	     					Player.VelX = - VelocidadePersnagem;
	     					Player.Parado = 0;
	     				}
	     				else
	     				{
	     					if(Player.UltimaHorizontalApertada != SDLK_LEFT)
	     					{
	     						DiminuiResistencia = 20;
	     						Player.UltimaHorizontalApertada = SDLK_LEFT;
	     					}
	     					Player.VelX = 0;
     						Player.VelY = 0;
	     				}

     					Player.DirecaoQueVai = ESQUERDA;
     					Player.ProximaPosicao.i = Player.PosicaoAtual.i;
     					Player.ProximaPosicao.j = Player.PosicaoAtual.j - 1;
     					break;
     				}
     				case SDLK_RIGHT:
     				{
     					if(!Player.Preso)
     					{
	     					Player.VelY = 0;
	     					Player.VelX = VelocidadePersnagem;
	     					Player.Parado = 0;
	     				}
	     				else
	     				{
	     					if(Player.UltimaHorizontalApertada != SDLK_RIGHT)
	     					{
	     						DiminuiResistencia = 20;
	     						Player.UltimaHorizontalApertada = SDLK_RIGHT;
	     					}
	     					Player.VelX = 0;
     						Player.VelY = 0;
	     				}
     					Player.DirecaoQueVai = DIREITA;
     					Player.ProximaPosicao.i = Player.PosicaoAtual.i;
     					Player.ProximaPosicao.j = Player.PosicaoAtual.j + 1;
     					break;
     				}

     				case SDLK_SPACE:
     				{



     					if(!Player.Preso)
     					{
	     					if(!BombaComum.Ativa && !BombaComum.Explodindo)
	     					{
	     						BombaComum.Ativa = 1;
	     						BombaComum.PosicaoAtual = Player.PosicaoAtual;
	     						mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j] = TemBomba;
	     						//Som que ele irá fazer colocando a bomba!
								//========================================
								Mix_PlayChannel(-1,por_bomba,0);
								//==========================================
	     					}
	     				}
     					break;
     				}
     				case SDLK_ESCAPE:
     				{
     					return -1;
     					break;
     				}
     				case SDLK_p:
     				{
     					int q = 0;

     					while(!q)
     					{
     						while (SDL_PollEvent(&event))
					     	{
					     		if(event.type == SDL_KEYDOWN)
					     		{
					     			if(event.key.keysym.sym == SDLK_p)
					     				q = 1;
					     		}

					     		desenha_texto("PAUSE", screen, 393, OyRectLife + 60,font);
								SDL_Flip(screen);
							//SNK
					     	}
     					}
     					break;
     				}
     			}
     		}
     		if(event.type == SDL_KEYUP)
     		{
     			switch(event.key.keysym.sym)
     			{
     				case SDLK_UP:
     				{
     					Player.VelY = 0;
     					if(Player.VelX == 0)
     						Player.Parado = 1;
     					break;
     				}
     				case SDLK_DOWN:
     				{
     					Player.VelY = 0;
     					if(Player.VelX == 0)
     						Player.Parado = 1;
     					break;
     				}
     				case SDLK_LEFT:
     				{
     					Player.VelX = 0;
     					if(Player.VelY == 0)
     						Player.Parado = 1;
     					break;
     				}
     				case SDLK_RIGHT:
     				{
     					Player.VelX = 0;
     					if(Player.VelY == 0)
     						Player.Parado = 1;
     					break;
     				}
     			}
     		}
     	}

     	//RESTRIÇÃO DO PLAYER
 		int VaiTrocar = VaiTrocarPosicaoPlayer(Player.Rect, campo[Player.ProximaPosicao.i][Player.ProximaPosicao.j], Player.DirecaoQueVai, mapa[Player.ProximaPosicao.i][Player.ProximaPosicao.j]);
     	if(VaiTrocar == 1)
 		{
 			if(mapa[Player.PosicaoAtual.i][Player.PosicaoAtual.j] == TemPersonagem)
 				mapa[Player.PosicaoAtual.i][Player.PosicaoAtual.j] = 0;

 			Player.PosicaoAtual.i = Player.ProximaPosicao.i;
 			Player.PosicaoAtual.j = Player.ProximaPosicao.j;

 			switch(Player.DirecaoQueVai)
 			{
 				case CIMA:
 				{
 					Player.ProximaPosicao.i--;
 					break;
 				}
 				case DIREITA:
 				{
 					Player.ProximaPosicao.j++;
 					break;
 				}
 				case BAIXO:
 				{
 					Player.ProximaPosicao.i++;
 					break;
 				}
 				case ESQUERDA:
 				{
 					Player.ProximaPosicao.j--;
 					break;
 				}
 			}
 		}
 		else
 			if(VaiTrocar == -1)
 			{
				Player.VelX = 0;
				Player.VelY = 0;
 			}

 		Player.Rect.x += Player.VelX;
 		Player.Rect.y += Player.VelY;

		//CONTROLA OS MORCEGOS
		for(m=0;m<NumeroMorcegos;m++)
		{
			if(!Morcegos[m].EstahVivo)
			{
				ContadorDeMonstrosMortos++;
				continue;
			}

			if(!Player.Preso)
				Morcegos[m].Ataque = AtaqueMonstros(Player, Morcegos[m]);

			if(!Morcegos[m].Ataque)
			{
				PosicaoDoMapa PosicoesDoMapaParaSeguir[3]; //esse vetor conterá as posições do mapa que pode ir;
				int DirecaoQueVai[3];
				int seguir; //essa variável vai receber um valor aleatório que vai indicar para qual lado o monstro deve seguir
				int VaitrocarNumMapa = 0;


				if(PosicaoDoRectEhIgual(Morcegos[m].Rect, campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j]))
				{
					mapa[Morcegos[m].PosicaoAtual.i][Morcegos[m].PosicaoAtual.j] = 0;
					Morcegos[m].PosicaoAtual = Morcegos[m].ProximaPosicao;
					mapa[Morcegos[m].PosicaoAtual.i][Morcegos[m].PosicaoAtual.j] = TemMonstro;
				}

				if(PosicaoDoMapaEhIgual(Morcegos[m].ProximaPosicao, Morcegos[m].PosicaoAtual))
				{
					int LimiteParaSeguir = NumeroDeDirecoesPraSeguir(Morcegos[m].PosicaoAtual, PosicoesDoMapaParaSeguir, mapa, Morcegos[m].DirecaoQueVeio, DirecaoQueVai, MORCEGO);

					seguir = rand() % LimiteParaSeguir;

					if(seguir==3)
						seguir=2;

					Morcegos[m].ProximaPosicao = PosicoesDoMapaParaSeguir[seguir];
					Morcegos[m].DirecaoQueVeio = DirecaoQueVai[seguir];
				}
				else
				{
					if( (Morcegos[m].Rect.x <= campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].x + campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].w)
						&& (Morcegos[m].Rect.x >= campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].x) )
						{
							if(Morcegos[m].Rect.y < campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].y)
							{
								if(campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].y - Morcegos[m].Rect.y == 1)
									Morcegos[m].Rect.y++;
								else
									Morcegos[m].Rect.y+=VPP;
							}
							else
								if(Morcegos[m].Rect.y > campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].y)
								{
									if(Morcegos[m].Rect.y - campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].y == 1)
										Morcegos[m].Rect.y--;
									else
										Morcegos[m].Rect.y-=VPP;
								}
						}

						if( (Morcegos[m].Rect.y <= campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].y + campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].h)
							&& (Morcegos[m].Rect.y >= campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].y) )
							{
								if(Morcegos[m].Rect.x < campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].x)
								{
									if(campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].x - Morcegos[m].Rect.x == 1)
										Morcegos[m].Rect.x++;
									else
										Morcegos[m].Rect.x+=VPP;
								}
								if(Morcegos[m].Rect.x > campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].x)
								{
									if(Morcegos[m].Rect.x - campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j].x == 1)
										Morcegos[m].Rect.x--;
									else
										Morcegos[m].Rect.x-=VPP;
								}
							}
				}


				VaitrocarNumMapa = VaiTrocarPosicaoMonstro(Morcegos[m].Rect, campo[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j], Morcegos[m].DirecaoQueVeio, mapa[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j]);

				switch(VaitrocarNumMapa)
				{
					case 0:
					{
						break;
					}
					case 1:
					{
						mapa[Morcegos[m].ProximaPosicao.i][Morcegos[m].ProximaPosicao.j] = TemMonstro;
						break;
					}
					case 2:
					{
						mapa[Morcegos[m].PosicaoAtual.i][Morcegos[m].PosicaoAtual.j] = 0;
						break;
					}
				}
			}
			else
			{
				Player.Preso = 1;
				Player.Parado = 1;
				Morcegos[m].Resistencia -= DiminuiResistencia;
				DiminuiResistencia = 0;
				Morcegos[m].TempoAtaque--;
				Player.HP -= Morcegos[m].DanoQueCausa;
				Player.VelY = 0;
				Player.VelX = 0;

				if(Morcegos[m].Resistencia <= 0 || Morcegos[m].TempoAtaque <= 0)
				{
					Morcegos[m].Ataque = 0;
					Morcegos[m].Resistencia = 200;
					Morcegos[m].TempoAtaque = TempoPAdraoAgarrado;
					Player.Hurted = 1;
					Player.Preso = 0;
					Morcegos[m].Ataque = 0;
					Player.UltimaHorizontalApertada = SDLK_SPACE;
				}
			}


		}


		//DEFINE AONDE A BOMBA ESTARÁ
		if(BombaComum.Ativa)
		{
			if(EstadoAnimacaoBomba == 1)
			{
				BombaComum.Rect.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j].x;
				BombaComum.Rect.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j].y;
			}
			else
			{
				BombaComum.Rect.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j].x + 4;
				BombaComum.Rect.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j].y + 4;
			}
		}


		PosicaoDoMapa DestrutiveisAcertados[3];
		int PosicaoVetorMorcegoAcertado[6];

		if(BombaComum.Explodindo)
	    {


            MioloExplosao.x = 79;
            MioloExplosao.y = 80;
            MioloExplosao.w = 45;
            MioloExplosao.h = 40;

            BombaComum.Rect.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j].x;
		    BombaComum.Rect.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j].y;


            ExplosaoEsquerda.x = 41;
            ExplosaoEsquerda.y = 80;
            ExplosaoEsquerda.w = 41;
            ExplosaoEsquerda.h = 40;

            ExplosaoEsquerdaFinal.x = 0;
            ExplosaoEsquerdaFinal.y = 80;
            ExplosaoEsquerdaFinal.w = 45;
            ExplosaoEsquerdaFinal.h = 40;

            ExplosaoDireita.x = 123;
            ExplosaoDireita.y = 80;
            ExplosaoDireita.w = 45;
            ExplosaoDireita.h = 40;

            ExplosaoDireitaFinal.x = 164;
            ExplosaoDireitaFinal.y = 80;
            ExplosaoDireitaFinal.w = 41;
            ExplosaoDireitaFinal.h = 40;

            ExplosaoCima.x = 82;
            ExplosaoCima.y = 40;
            ExplosaoCima.w = 41;
            ExplosaoCima.h = 40;

            ExplosaoCimaFinal.x = 82;
            ExplosaoCimaFinal.y = 10;
            ExplosaoCimaFinal.w = 41;
            ExplosaoCimaFinal.h = 40;

            ExplosaoBaixo.x = 82;
            ExplosaoBaixo.y = 120;
            ExplosaoBaixo.w = 41;
            ExplosaoBaixo.h = 40;

            ExplosaoBaixoFinal.x = 82;
            ExplosaoBaixoFinal.y = 160;
            ExplosaoBaixoFinal.w = 41;
            ExplosaoBaixoFinal.h = 40;



            AumentaExplosaoEsquerda = 0;
            AumentaExplosaoEsquerdaFinal = 0;
	        AumentaExplosaoDireita = 0;
	        AumentaExplosaoDireitaFinal = 0;
	        AumentaExplosaoCima = 0;
	        AumentaExplosaoCimaFinal = 0;
	        AumentaExplosaoBaixo = 0;
	        AumentaExplosaoBaixoFinal = 0;
	        AtingiuMonstro = 0;
	        AtingiuDestrutivel = 0;
	        AtingiuPlayer = 0;

	        BombaComum.TempoExplosao--;
	        EstadoAnimacaoExplosao++;
	        if(EstadoAnimacaoExplosao == 5)
	        	EstadoAnimacaoExplosao = 2;


	//restrição do rect
            //p/ direita
            if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1] != 100)
            {
                if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1] == 0)   //caminho
                {
                    AumentaExplosaoDireita = 1;
                    BombaComum.RectExplosaoDireita = BombaComum.Rect;
                    BombaComum.RectExplosaoDireita.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1].x;
		            BombaComum.RectExplosaoDireita.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1].y;
                }


                if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1] == 2)         //destrutivel
                {
                	DestrutiveisAcertados[AtingiuDestrutivel].i = BombaComum.PosicaoAtual.i;
                	DestrutiveisAcertados[AtingiuDestrutivel].j = BombaComum.PosicaoAtual.j+1;

                    AumentaExplosaoDireita = 1;
                    AtingiuDestrutivel += 1;

		    		Mix_PlayChannel(-1,iten,0);

                    BombaComum.RectExplosaoDireita = BombaComum.Rect;
                    BombaComum.RectExplosaoDireita.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1].x;
		            BombaComum.RectExplosaoDireita.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1].y;


                }
                for(m=0;m<NumeroMorcegos;m++)
                {
	                if(BombaComum.PosicaoAtual.i == Morcegos[m].PosicaoAtual.i &&
	                	BombaComum.PosicaoAtual.j+1 == Morcegos[m].PosicaoAtual.j)//monstro
	                {
	                	PosicaoVetorMorcegoAcertado[AtingiuMonstro] = m;

	                    AumentaExplosaoDireita = 1;
	                    AtingiuMonstro += 1;
	                    BombaComum.RectExplosaoDireita = BombaComum.Rect;
	                    BombaComum.RectExplosaoDireita.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1].x;
			            BombaComum.RectExplosaoDireita.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1].y;
	                }
	            }

                if(BombaComum.PosicaoAtual.i == Player.PosicaoAtual.i && BombaComum.PosicaoAtual.j+1 == Player.PosicaoAtual.j)//player
                {
                    AumentaExplosaoDireita = 1;
                    AtingiuPlayer = 1;
                    BombaComum.RectExplosaoDireita = BombaComum.Rect;
                    BombaComum.RectExplosaoDireita.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1].x;
		            BombaComum.RectExplosaoDireita.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1].y;
                }

            }



            //p/ esq
            if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1] != 100)
            {
                if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1] == 0)
                {
                    AumentaExplosaoEsquerda = 1;
                    BombaComum.RectExplosaoEsquerda = BombaComum.Rect;
                    BombaComum.RectExplosaoEsquerda.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1].x;
		            BombaComum.RectExplosaoEsquerda.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1].y;
                }


                if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1] == 2)
                {
                	DestrutiveisAcertados[AtingiuDestrutivel].i = BombaComum.PosicaoAtual.i;
                	DestrutiveisAcertados[AtingiuDestrutivel].j = BombaComum.PosicaoAtual.j-1;

                    AumentaExplosaoEsquerda = 1;
                    AtingiuDestrutivel += 1;

		    		Mix_PlayChannel(-1,iten,0);

                    BombaComum.RectExplosaoEsquerda = BombaComum.Rect;
                    BombaComum.RectExplosaoEsquerda.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1].x;
		            BombaComum.RectExplosaoEsquerda.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1].y;


                }
                for(m=0;m<NumeroMorcegos;m++)
                {
	                if(BombaComum.PosicaoAtual.i == Morcegos[m].PosicaoAtual.i &&
	                	BombaComum.PosicaoAtual.j-1 == Morcegos[m].PosicaoAtual.j)
	                {
	                	PosicaoVetorMorcegoAcertado[AtingiuMonstro] = m;

	                    AumentaExplosaoEsquerda = 1;
	                    AtingiuMonstro += 1;
	                    BombaComum.RectExplosaoEsquerda = BombaComum.Rect;
	                    BombaComum.RectExplosaoEsquerda.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1].x;
			            BombaComum.RectExplosaoEsquerda.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1].y;
	                }
	            }

                if(BombaComum.PosicaoAtual.i == Player.PosicaoAtual.i && BombaComum.PosicaoAtual.j-1 == Player.PosicaoAtual.j)
                {
                    AumentaExplosaoEsquerda = 1;
                    AtingiuPlayer = 1;
                    BombaComum.RectExplosaoEsquerda = BombaComum.Rect;
                    BombaComum.RectExplosaoEsquerda.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1].x;
		            BombaComum.RectExplosaoEsquerda.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1].y;
                }


            }

            //p/ baixo
             if(mapa[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j] != 100)
            {
            	if(mapa[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j] == 0)
                {
                    AumentaExplosaoBaixo = 1;
                    BombaComum.RectExplosaoBaixo = BombaComum.Rect;
                    BombaComum.RectExplosaoBaixo.x = campo[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j].x;
		            BombaComum.RectExplosaoBaixo.y = campo[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j].y;
                }

                if(mapa[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j] == 2)
                {
                	DestrutiveisAcertados[AtingiuDestrutivel].i = BombaComum.PosicaoAtual.i+1;
                	DestrutiveisAcertados[AtingiuDestrutivel].j = BombaComum.PosicaoAtual.j;

                    AumentaExplosaoBaixo = 1;
                    AtingiuDestrutivel += 1;

		    		Mix_PlayChannel(-1,iten,0);

                    BombaComum.RectExplosaoBaixo = BombaComum.Rect;
                    BombaComum.RectExplosaoBaixo.x = campo[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j].x;
		            BombaComum.RectExplosaoBaixo.y = campo[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j].y;
                }

                for(m=0;m<NumeroMorcegos;m++)
                {
	                if(BombaComum.PosicaoAtual.i+1 == Morcegos[m].PosicaoAtual.i &&
	                	BombaComum.PosicaoAtual.j == Morcegos[m].PosicaoAtual.j)
	                {
	                	PosicaoVetorMorcegoAcertado[AtingiuMonstro] = m;

	                    AumentaExplosaoBaixo = 1;
	                    AtingiuMonstro += 1;
	                    BombaComum.RectExplosaoBaixo = BombaComum.Rect;
	                    BombaComum.RectExplosaoBaixo.x = campo[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j].x;
			            BombaComum.RectExplosaoBaixo.y = campo[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j].y;
	                }
	            }

                if(BombaComum.PosicaoAtual.i+1 == Player.PosicaoAtual.i && BombaComum.PosicaoAtual.j == Player.PosicaoAtual.j)
                {
                    AumentaExplosaoBaixo = 1;
                    AtingiuPlayer = 1;
                    BombaComum.RectExplosaoBaixo = BombaComum.Rect;
                    BombaComum.RectExplosaoBaixo.x = campo[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j].x;
		            BombaComum.RectExplosaoBaixo.y = campo[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j].y;
                }

            }


            //p/ cima
            if(mapa[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j] != 100)
            {
                if(mapa[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j] == 0)
                {
                    AumentaExplosaoCima = 1;
                    BombaComum.RectExplosaoCima = BombaComum.Rect;
                    BombaComum.RectExplosaoCima.x = campo[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j].x;
		            BombaComum.RectExplosaoCima.y = campo[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j].y;
                }

                if(mapa[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j] == 2)
                {
                	DestrutiveisAcertados[AtingiuDestrutivel].i = BombaComum.PosicaoAtual.i-1;
                	DestrutiveisAcertados[AtingiuDestrutivel].j = BombaComum.PosicaoAtual.j;

                    AumentaExplosaoCima = 1;
                    AtingiuDestrutivel += 1;

		   			Mix_PlayChannel(-1,iten,0);

                    BombaComum.RectExplosaoCima = BombaComum.Rect;
                    BombaComum.RectExplosaoCima.x = campo[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j].x;
		            BombaComum.RectExplosaoCima.y = campo[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j].y;
                }

                for(m=0;m<NumeroMorcegos;m++)
                {
	                if(BombaComum.PosicaoAtual.i-1 == Morcegos[m].PosicaoAtual.i &&
	                 	BombaComum.PosicaoAtual.j == Morcegos[m].PosicaoAtual.j )
	                {
	                	PosicaoVetorMorcegoAcertado[AtingiuMonstro] = m;

	                    AumentaExplosaoCima = 1;
	                    AtingiuMonstro += 1;
	                    BombaComum.RectExplosaoCima = BombaComum.Rect;
	                    BombaComum.RectExplosaoCima.x = campo[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j].x;
			            BombaComum.RectExplosaoCima.y = campo[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j].y;
	                }
	            }

                if(BombaComum.PosicaoAtual.i-1 == Player.PosicaoAtual.i && BombaComum.PosicaoAtual.j == Player.PosicaoAtual.j)
                {
                    AumentaExplosaoCima = 1;
                    AtingiuPlayer = 1;
                    BombaComum.RectExplosaoCima = BombaComum.Rect;
                    BombaComum.RectExplosaoCima.x = campo[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j].x;
		            BombaComum.RectExplosaoCima.y = campo[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j].y;
                }
         }


   // Tratando exceções :    [vazio] seguido de um [indestrutível]

           //p/ direita
            if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+1] != 1)
            {

            	if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+2] == 1)
            		AumentaExplosaoDireitaFinal = 0;

                else
                {
            		if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+2] == 2)
					{
						DestrutiveisAcertados[AtingiuDestrutivel].i = BombaComum.PosicaoAtual.i;
                		DestrutiveisAcertados[AtingiuDestrutivel].j = BombaComum.PosicaoAtual.j+2;

					    AtingiuDestrutivel += 1;

					    Mix_PlayChannel(-1,iten,0);
					}

					for(m=0;m<NumeroMorcegos;m++)
					{
		            	if(BombaComum.PosicaoAtual.i == Morcegos[m].PosicaoAtual.i &&
		            		BombaComum.PosicaoAtual.j+2 == Morcegos[m].PosicaoAtual.j)
		                {
		                	PosicaoVetorMorcegoAcertado[AtingiuMonstro] = m;
		                    AtingiuMonstro += 1;
		                }
		            }

	            	if(BombaComum.PosicaoAtual.i == Player.PosicaoAtual.i && BombaComum.PosicaoAtual.j+2 == Player.PosicaoAtual.j)
	                    AtingiuPlayer = 1;


                    AumentaExplosaoDireitaFinal = 1;
                    BombaComum.RectExplosaoDireitaFinal = BombaComum.Rect;
                    BombaComum.RectExplosaoDireitaFinal.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+2].x;
		            BombaComum.RectExplosaoDireitaFinal.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j+2].y;
                }

            }

            //p/ esquerda
            if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-1] != 1)
            {
            	if (mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-2] == 1)
            	   AumentaExplosaoEsquerdaFinal = 0;

                else
                {
	            	if(mapa[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-2] == 2)
					{
						DestrutiveisAcertados[AtingiuDestrutivel].i = BombaComum.PosicaoAtual.i;
	                	DestrutiveisAcertados[AtingiuDestrutivel].j = BombaComum.PosicaoAtual.j-2;

					    AtingiuDestrutivel += 1;

					    Mix_PlayChannel(-1,iten,0);
					}

					for(m=0;m<NumeroMorcegos;m++)
					{
		            	if(BombaComum.PosicaoAtual.i == Morcegos[m].PosicaoAtual.i &&
		            		BombaComum.PosicaoAtual.j-2 == Morcegos[m].PosicaoAtual.j)
		            	{
		            		PosicaoVetorMorcegoAcertado[AtingiuMonstro] = m;
		            		AtingiuMonstro += 1;
		            	}
		            }


	            	if(BombaComum.PosicaoAtual.i == Player.PosicaoAtual.i && BombaComum.PosicaoAtual.j-2 == Player.PosicaoAtual.j)
	            		AtingiuPlayer = 1;


	            	AumentaExplosaoEsquerdaFinal = 1;
	                BombaComum.RectExplosaoEsquerdaFinal = BombaComum.Rect;
	                BombaComum.RectExplosaoEsquerdaFinal.x = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-2].x;
			        BombaComum.RectExplosaoEsquerdaFinal.y = campo[BombaComum.PosicaoAtual.i][BombaComum.PosicaoAtual.j-2].y;
	            }


            }

            // para baixo
            if(mapa[BombaComum.PosicaoAtual.i+1][BombaComum.PosicaoAtual.j] != 1)
            {
            	if(mapa[BombaComum.PosicaoAtual.i+2][BombaComum.PosicaoAtual.j] == 1)
                   AumentaExplosaoBaixoFinal = 0;

                else
                {
	            	if(mapa[BombaComum.PosicaoAtual.i+2][BombaComum.PosicaoAtual.j] == 2)
					{
						DestrutiveisAcertados[AtingiuDestrutivel].i = BombaComum.PosicaoAtual.i+2;
                		DestrutiveisAcertados[AtingiuDestrutivel].j = BombaComum.PosicaoAtual.j;

					    AtingiuDestrutivel += 1;

					    Mix_PlayChannel(-1,iten,0);
					}

					for(m=0;m<NumeroMorcegos;m++)
					{
		            	if(BombaComum.PosicaoAtual.i+2 == Morcegos[m].PosicaoAtual.i &&
		            		BombaComum.PosicaoAtual.j == Morcegos[m].PosicaoAtual.j)
		                {
		                	PosicaoVetorMorcegoAcertado[AtingiuMonstro] = m;

		                    AtingiuMonstro +=1;
		                }
		            }


	            	if(BombaComum.PosicaoAtual.i+2 == Player.PosicaoAtual.i && BombaComum.PosicaoAtual.j == Player.PosicaoAtual.j)
	                    AtingiuPlayer = 1;



	                AumentaExplosaoBaixoFinal = 1;
	                BombaComum.RectExplosaoBaixoFinal = BombaComum.Rect;
	                BombaComum.RectExplosaoBaixoFinal.x = campo[BombaComum.PosicaoAtual.i+2][BombaComum.PosicaoAtual.j].x;
			        BombaComum.RectExplosaoBaixoFinal.y = campo[BombaComum.PosicaoAtual.i+2][BombaComum.PosicaoAtual.j].y;
                }

            }

            // para cima
            if(mapa[BombaComum.PosicaoAtual.i-1][BombaComum.PosicaoAtual.j] != 1)
            {
                if(mapa[BombaComum.PosicaoAtual.i-2][BombaComum.PosicaoAtual.j] == 1)
                	AumentaExplosaoCimaFinal = 0;
                else
                {
            		if(mapa[BombaComum.PosicaoAtual.i-2][BombaComum.PosicaoAtual.j] == 2)
					{
						DestrutiveisAcertados[AtingiuDestrutivel].i = BombaComum.PosicaoAtual.i-2;
                		DestrutiveisAcertados[AtingiuDestrutivel].j = BombaComum.PosicaoAtual.j;

					    AtingiuDestrutivel +=1;

					    Mix_PlayChannel(-1,iten,0);
					}

					for(m=0;m<NumeroMorcegos;m++)
					{
		            	if(BombaComum.PosicaoAtual.i-2 == Morcegos[m].PosicaoAtual.i &&
		            		BombaComum.PosicaoAtual.j == Morcegos[m].PosicaoAtual.j)
		            	{
		            		PosicaoVetorMorcegoAcertado[AtingiuMonstro] = m;

		            		AtingiuMonstro += 1;
		            	}
		            }


	            	if(BombaComum.PosicaoAtual.i-2 == Player.PosicaoAtual.i && BombaComum.PosicaoAtual.j == Player.PosicaoAtual.j)
	                    AtingiuPlayer = 1;


	                AumentaExplosaoCimaFinal = 1;
	                BombaComum.RectExplosaoCimaFinal = BombaComum.Rect;
	                BombaComum.RectExplosaoCimaFinal.x = campo[BombaComum.PosicaoAtual.i-2][BombaComum.PosicaoAtual.j].x;
			        BombaComum.RectExplosaoCimaFinal.y = campo[BombaComum.PosicaoAtual.i-2][BombaComum.PosicaoAtual.j].y;

            	}

      		}
      		if(PosicaoDoMapaEhIgual(Player.PosicaoAtual, BombaComum.PosicaoAtual))
      			AtingiuPlayer = 1;

    	}







//------------------------------------------------------------------------------------------------------------------------------//


//-------------------------------REBLITAGEM DO CENARIO----------------------------------------

		SDL_FillRect(screen,&rectBarraSuperior, SDL_MapRGB(screen->format,107,51,102));
		atualizacao_coracoes(Player.HP, surLife, screen);




		 //==============================================
		//Escrever "tempo" e "socre" na tela.

		TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		desenha_texto("TEMPO", screen, 390, OyRectLife,font);
		TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		desenha_texto("SCORE", screen, 630, OyRectLife,font);
		TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		desenha_texto("PLAYER:", screen, 10,OyRectLife,font);
		TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		desenha_texto("FALTAM MORCEGOS:", screen,10,OyQuantidadeMorcego,font);
		TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		desenha_texto("BONUS:",screen,10,OyBonus,font);


	      //==============================================



		i=0;
		while(1==1)
		{
			for(j=0;j<ColunasDoCampo;j++)
				SDL_BlitSurface(surGrama, NULL, screen, &campo[i][j]);
			if(i==14)
				break;
			else
				i=14;
		}


		j = 0;
		while(1==1)
		{
			for(i=0;i<LinhasDoCampo;i++)
				SDL_BlitSurface(surGrama, NULL, screen, &campo[i][j]);
			if(j==18)
				break;
			else
				j=18;
		}


		for (i=1;i<LinhasDoCampo-1;i++)
		{
			int cont=0;
			for(j=1;j<ColunasDoCampo-1;j++)
			{
				SDL_BlitSurface(surCaminho, NULL, screen, &campo[i][j]);

				if(mapa[i][j] == 1)
				{
					cont++;
					if(cont == 7)
						cont = 1;
					switch(cont)
					{
						case 1:
						{
							SDL_BlitSurface(surCasa1, NULL, screen, &campo[i][j]);
							break;
						}
						case 2:
						{
							SDL_BlitSurface(surCasa2, NULL, screen, &campo[i][j]);
							break;
						}
						case 3:
						{
							SDL_BlitSurface(surCasa3, NULL, screen, &campo[i][j]);
							break;
						}
						case 4:
						{
							SDL_BlitSurface(surCasa4, NULL, screen, &campo[i][j]);
							break;
						}
						case 5:
						{
							SDL_BlitSurface(surCasa5, NULL, screen, &campo[i][j]);
							break;
						}
						case 6:
						{
							SDL_BlitSurface(surCasa6, NULL, screen, &campo[i][j]);
							break;
						}
					}
				}
				else
					if(mapa[i][j] == 2)
						SDL_BlitSurface(surCerca, NULL, screen, &campo[i][j]);
			}

		}



		//REBLITA MORCEGOS
		for(m=0;m<NumeroMorcegos;m++)
		{
			if(Morcegos[m].EstahVivo)
			{
				if(!Morcegos[m].Ataque)
				{
					switch(Morcegos[m].DirecaoQueVeio)
					{
						case CIMA:
						{
							SDL_BlitSurface(surMorcego[BAIXO][EstadoAnimacaoMonstros], NULL, screen, &Morcegos[m].Rect);
							break;
						}
						case DIREITA:
						{
							SDL_BlitSurface(surMorcego[ESQUERDA][EstadoAnimacaoMonstros], NULL, screen, &Morcegos[m].Rect);
							break;
						}
						case BAIXO:
						{
							SDL_BlitSurface(surMorcego[CIMA][EstadoAnimacaoMonstros], NULL, screen, &Morcegos[m].Rect);
							break;
						}
						case ESQUERDA:
						{
							SDL_BlitSurface(surMorcego[DIREITA][EstadoAnimacaoMonstros], NULL, screen, &Morcegos[m].Rect);
							break;
						}
					}
				}
				else
				{
					switch(Morcegos[m].DirecaoQueVeio)
					{
						case CIMA:
						{
							Player.Rect.y -= 5;
							Player.Rect.x -= 5;
							SDL_BlitSurface(surMorcegoAtaque[Player.DirecaoQueVai][BAIXO], NULL, screen, &Player.Rect);
							Player.Rect.y += 5;
							Player.Rect.x += 5;
							break;
						}
						case DIREITA:
						{
							Player.Rect.y -= 5;
							Player.Rect.x -= 5;
							SDL_BlitSurface(surMorcegoAtaque[Player.DirecaoQueVai][ESQUERDA], NULL, screen, &Player.Rect);
							Player.Rect.y += 5;
							Player.Rect.x += 5;
							break;
						}
						case BAIXO:
						{
							Player.Rect.y -= 5;
							Player.Rect.x -= 5;
							SDL_BlitSurface(surMorcegoAtaque[Player.DirecaoQueVai][CIMA], NULL, screen, &Player.Rect);
							Player.Rect.y += 5;
							Player.Rect.x += 5;
							break;
						}
						case ESQUERDA:
						{
							Player.Rect.y -= 5;
							Player.Rect.x -= 5;
							SDL_BlitSurface(surMorcegoAtaque[Player.DirecaoQueVai][DIREITA], NULL, screen, &Player.Rect);
							Player.Rect.y += 5;
							Player.Rect.x += 5;
							break;
						}
					}

				}
			}
		}


		///BLITAR PERSONAGEM
		if(!Player.Preso)
		{
			if(!Player.Hurted)
			{
				if(Player.Parado)
					SDL_BlitSurface(surPlayer[Player.DirecaoQueVai][0], NULL, screen, &Player.Rect);
				else
					SDL_BlitSurface(surPlayer[Player.DirecaoQueVai][EstadoAnimacaoPlayer], NULL, screen, &Player.Rect);
			}
			else
			{
				if(EstadoAnimacaoPlayerHurt == 0)
				{
					if(Player.Parado)
						SDL_BlitSurface(surPlayer[Player.DirecaoQueVai][0], NULL, screen, &Player.Rect);
					else
						SDL_BlitSurface(surPlayer[Player.DirecaoQueVai][EstadoAnimacaoPlayer], NULL, screen, &Player.Rect);
				}
			}
		}

		// TERMINAR A LÓGICA PRA BLITAGEM DA BOMBA
		if(BombaComum.Ativa)
			SDL_BlitSurface(surBomba, NULL, screen, &BombaComum.Rect);
		else
		{
			if(BombaComum.Explodindo)
			{

			      	SDL_BlitSurface(surExplosao[EstadoAnimacaoExplosao],&MioloExplosao,screen, &BombaComum.Rect);

                if(AumentaExplosaoDireita == 1)
                {
                    SDL_BlitSurface(surExplosao[EstadoAnimacaoExplosao],&ExplosaoDireita,screen, &BombaComum.RectExplosaoDireita);



                }

                //p/ esq
                if(AumentaExplosaoEsquerda == 1)
                {
                    SDL_BlitSurface(surExplosao[EstadoAnimacaoExplosao],&ExplosaoEsquerda,screen, &BombaComum.RectExplosaoEsquerda);

                }

                //p/ baixo
                if(AumentaExplosaoBaixo == 1)
                {
            	    SDL_BlitSurface(surExplosao[EstadoAnimacaoExplosao],&ExplosaoBaixo,screen, &BombaComum.RectExplosaoBaixo);


                }


                //p/ cima
                if(AumentaExplosaoCima == 1)
                {
                    SDL_BlitSurface(surExplosao[EstadoAnimacaoExplosao],&ExplosaoCima,screen, &BombaComum.RectExplosaoCima);

                }


                if(AumentaExplosaoDireitaFinal == 1)
                {
                    SDL_BlitSurface(surExplosao[EstadoAnimacaoExplosao],&ExplosaoDireitaFinal,screen, &BombaComum.RectExplosaoDireitaFinal);



                }

                //p/ esq
                if(AumentaExplosaoEsquerdaFinal == 1)
                {
                    SDL_BlitSurface(surExplosao[EstadoAnimacaoExplosao],&ExplosaoEsquerdaFinal,screen, &BombaComum.RectExplosaoEsquerdaFinal);

                }

                //p/ baixo
                if(AumentaExplosaoBaixoFinal == 1)
                {
            	    SDL_BlitSurface(surExplosao[EstadoAnimacaoExplosao],&ExplosaoBaixoFinal,screen, &BombaComum.RectExplosaoBaixoFinal);


                }


                //p/ cima
                if(AumentaExplosaoCimaFinal == 1)
                {
                    SDL_BlitSurface(surExplosao[EstadoAnimacaoExplosao],&ExplosaoCimaFinal,screen, &BombaComum.RectExplosaoCimaFinal);

                }

	//---------------------------------------------------------------------------------------------------------------------

				//código para blitar a explosão
			}
		}

		if(BombaComum.TempoExplosao <=0)
	    {
	     	BombaComum.Explodindo = 0;
	     	BombaComum.TempoExplosao = TEMPOEXPLOSAO;
	     	EstadoAnimacaoExplosao = 0;


	  //-------------------- modificações de hit na explosçao--------------------------------
	     	//verifica se acertou o player
	     	if(AtingiuPlayer)
	 		{
	 			Player.HP -= 60;
	 			(*Pontos)+=PontoJogadorBurro;
	 			if((*Pontos)<=0)
	 				(*Pontos) = 0;
	 			Player.Hurted = 1;
	 		}
	 		//verifica os monstros atingidos

 			for(m=0;m<AtingiuMonstro;m++)
 			{
				Morcegos[PosicaoVetorMorcegoAcertado[m]].EstahVivo = 0;
				mapa[Morcegos[PosicaoVetorMorcegoAcertado[m]].PosicaoAtual.i][Morcegos[PosicaoVetorMorcegoAcertado[m]].PosicaoAtual.j] = 0;
				mapa[Morcegos[PosicaoVetorMorcegoAcertado[m]].ProximaPosicao.i][Morcegos[PosicaoVetorMorcegoAcertado[m]].ProximaPosicao.j] = 0;
				Morcegos[PosicaoVetorMorcegoAcertado[m]].PosicaoAtual.i = -1;
				Morcegos[PosicaoVetorMorcegoAcertado[m]].PosicaoAtual.j = -1;

				(*Pontos)+=PontoAcertouMonstro;
				morcegos--;
				//SNK
				if(Player.Preso)
				{
					Morcegos[PosicaoVetorMorcegoAcertado[m]].Ataque = 0;
					Player.Preso = 0;
				}
			}

		   if(morcegos >= 0) //para evitar bug do score do morcego.
		   {
		    	sprintf(QuantidadeMorcegosRestantes,"%01d",morcegos);
			    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
				quantidade_morcego.imagem = TTF_RenderText_Solid(font,QuantidadeMorcegosRestantes,textColor);
		   }

	 		//acha quantos destrutiveis foram atingidos
	 		for(i=0;i<AtingiuDestrutivel;i++)
	 		{
				int Temp = rand() % 99;
				Temp++;
				if(Temp<=10)
				{
					Itens[NumDestrutiveisDestruidos].EstahAtivo = 1;
					Itens[NumDestrutiveisDestruidos].PosicaoAtual.i = DestrutiveisAcertados[i].i;
					Itens[NumDestrutiveisDestruidos].PosicaoAtual.j = DestrutiveisAcertados[i].j;
					Itens[NumDestrutiveisDestruidos].Rect.x = campo[Itens[NumDestrutiveisDestruidos].PosicaoAtual.i][Itens[NumDestrutiveisDestruidos].PosicaoAtual.j].x + 10;
					Itens[NumDestrutiveisDestruidos].Rect.y = campo[Itens[NumDestrutiveisDestruidos].PosicaoAtual.i][Itens[NumDestrutiveisDestruidos].PosicaoAtual.j].y + 10;
				}
				NumDestrutiveisDestruidos++;
				mapa[DestrutiveisAcertados[i].i][DestrutiveisAcertados[i].j] = 0;
				(*Pontos)+=PontoAcertuDestrutivel;
	 		}
	 	}

	 	//controle do Item
	 	for(i=0;i<NumDestrutiveisDestruidos;i++)
	 	{
	 		if(Itens[i].EstahAtivo)
	 		{
	 			//sim estou reutilizando a varivel só pra não criar outro controlador de animação
	 			SDL_BlitSurface(surItem[EstadoAnimacaoMonstros], NULL, screen, &Itens[i].Rect);
	 			if(PosicaoDoMapaEhIgual(Player.PosicaoAtual, Itens[i].PosicaoAtual))
	 			{
	 				Itens[i].EstahAtivo = 0;
					
					//=================================
					Mix_PlayChannel(-1,iten_coracao,0);
					//=================================
					
	 				if(Player.HP + 28 > surLife->w )
	 					Player.HP = surLife->w;
					else
	 					Player.HP += 28;
	 			}
	 		}
	 	}


		//CONTROLE E REBLITAGEM DO TEMPO/SCORE
		sprintf(pontos_texto,"%07d",(*Pontos) );
		TTF_SetFontStyle(font, TTF_STYLE_BOLD);
		pontuacao.imagem = TTF_RenderText_Solid( font, pontos_texto , textColor );

		tempo_fps++;
        if(tempo_fps >= 40)
        {
            tempo_fps = 0;
            tempo++;

			PontuacaoBonus--;
			sprintf(Bonus,"%04d",PontuacaoBonus);
			TTF_SetFontStyle(font, TTF_STYLE_BOLD);
			bonus.imagem = TTF_RenderText_Solid(font,Bonus,textColor);

			//SNK

            sprintf(tempo_texto,"%02d:%02d",tempo/60,tempo%60);
            TTF_SetFontStyle(font, TTF_STYLE_BOLD);
            tempo_figura.imagem = TTF_RenderText_Solid( font, tempo_texto , textColor );
            tempo_fps = 0;
        }


        //ESCREVER    OS INDICADORES DE PONTUAÇÃO E TEMPO
        apply_texto(&pontuacao,screen);
    	apply_texto(&tempo_figura,screen);
		apply_texto(&quantidade_morcego,screen);
		apply_texto(&bonus,screen);


    	/*if( (timer_get_ticks(&fps) < 1000 / FPS ) )
        {
           SDL_Delay( ( 1000 / FPS ) - timer_get_ticks(&fps)  );
        }*/



		if(ContadorDeMonstrosMortos == QtdTotalDeMosntros)
			quit = 1;

		if(Player.HP<=0)
		{
		    return -2;
		    quit = 1;

		}

		SDL_Flip(screen);
	}

	//SAÍDA DA FUNÇÃO
	int MatouGeral = 1;

	for(i=0;i<NumeroMorcegos;i++)
	{
		if(Morcegos[i].EstahVivo)
		{
			MatouGeral = 0;
			break;
		}
	}

	if(MatouGeral)
	{
	    SDL_Delay(2000);


	    (*Pontos) = (*Pontos) + BonusPontuacao;
	    return NumFase+1;
	}
	else
	{
		if(Player.HP==0)
		  return -2;

	}


	 //Para liberar os arquivos de música e eles pararem de tocar de vez.
	 Mix_FreeMusic(Parte1_1);
	 Mix_FreeChunk(explosao);
	 Mix_FreeChunk(por_bomba);
	 Mix_FreeChunk(iten);
	 Mix_FreeChunk(iten_coracao);
}	 
	 


//----------------------------- parte da história ou introdução do jogo ------------------------------

// talvez a repetição acima entre nessa parte....

//----------------------------- início do e movimento livre do personagem ---------------------------

}
