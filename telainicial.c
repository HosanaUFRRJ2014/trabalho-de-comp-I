#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


#include <SDL/SDL_ttf.h>

#include <SDL/SDL_mixer.h>

//#include "NomeRecorde.c"
//#include "figura.c"

#define FONTSIZE 21


#define OxPrimeiroBotao 500 // para os botões: Inciar, Continuar, SFX
#define OyPrimeiroBotao 100

#define OxSegundoBotao 500 // para os botões: Novo Jogo, Musica
#define OySegundoBotao 180

#define OxTerceiroBotao 500 // para o botão Controles
#define OyTerceiroBotao 260

#define OxQuartoBotao 500 // para o botão AutoSave
#define OyQuartoBotao 340

#define OxQuintoBotao 500 // para o botão Save
#define OyQuintoBotao 420

//#define OxSextoBotao 500      o 6º botão sumiu, mas talvez isso ainda seja util
#define OySextoBotao 500

#define OxBotaoLimpar 500
#define OyBotaoLimpar 600

#define IncrementoXdoVotlarHighScore 0 // diz quanto pro lado está o botão voltar na high score

#define IncrementoDeSelecaoX -80 // vai colocar o botão mais para o lado, dando ideia de seleção

//#define OxBarraSelecao 0

#define AcrescimoXnoBotaoVoltar 0

#define AcrescimoYnoBotaoVoltar 80

#define AcrescimoDeXemOnOff -300

#define AcrescimoEmX_Sim -200

#define AcrescimoEmX_Nao -315

#define AcrescimoEmXMorcego 15

#define OxMostrarControles 150







void BlitarBotaoSelecionado(SDL_Surface *surBotaoSelecionado, SDL_Rect rectBotaoSelecionado, SDL_Surface *screen)
{
	rectBotaoSelecionado.x += IncrementoDeSelecaoX; /*pra dar uma ideia de seleção dá um acrescimo x na coordenada da opção x para parecer destaque*/
	SDL_BlitSurface(surBotaoSelecionado, NULL, screen, &rectBotaoSelecionado);
}

//=====================================================X=============================================
//CHECAR AS FUNCIONALIDADES DA TELA, COMPLETANDO A PARTE DE CONTROLES, SOBRE, SAIR E FAZER AS IMAGENS
//=====================================================X=============================================

//=============== CHECADO AS FUNÇÕES DOS BOTÕES/FALTANDO SÓ OS DE INICIO E SAIDA DE JOGO (att)============

//colocar um vetor de opções com o seguinte padrão {1, 1};
int TelaInicial(SDL_Surface *screen, int EstadoOpcoes[3])
// -1 = SAIR DO JOGO; 1 = ENTRAR NOVO JOGO;
{
	//Controles[0] = Cima; Controles[1] = Baixo; Controles[2] = Direita; Controles[3] = Esquerda; Controles[4] = Bomba;

	/*Menu: 1-Iniciar;        2-High Score;    3-Opções;                 4- Sobre;            5- Sair;
	          1.1-Continuar     2.1-Limpar       3.1-SFX(on/off)           4.1- Voltar          *Sim
	          1.2-Novo Jogo     2.2- Voltar      3.2- Musica(on/off)                             *Não
	          1.3- Voltar                        3.3-Auto save(on/off)
	                                             3.4-Save
	                                             3.5-Controles
	                                             3.6 Voltar
*/
	int MenuSelecionado = 0, SubMenuSelecionado = -1, Pagina = 0;
	int Retorno = -2;
	int HaSave = 0;
	int MostrarSimNao = 0, SimOuNaoSelecionado = 0, ApagaRecordes = 0;
	int ControladorMorcego = 0, ControladorDeTempo = 0; // vai fazer a animação entre a imagem 1 e 2
	int Sair = 0;

	//EstadoOpcoes[3]; opção[0] = SFX; opção[1] = Music; opção[2] = AutoSave


	//======================================================

	TTF_Font* fonte;

	fonte = TTF_OpenFont("japonese.ttf",FONTSIZE * 2);

	//=====================================================






	//=========================================================

	/*Variáveis que serão usadas para carregar as músicas
	 Mix_Chunk ------> mais usado para carregar arquivos de sons pequenos(tipo o efeito de seleção);
	 Mix_Music -------> usado para carregar os arquivos de música; */

	Mix_Music * musica_fundo = Mix_LoadMUS("SonsJogo/menu.mp3");
	Mix_Chunk * som_selecao_enter = Mix_LoadWAV("SonsJogo/button16.wav");
	Mix_Chunk * som_selecao_movimento = Mix_LoadWAV("SonsJogo/button15.wav");

	//===========================================================



	//======================================

	 SDL_Surface *FundoControles = IMG_Load("Imagens/fundoHS.png");
	 
	//======================================


	SDL_Surface *Fundo = IMG_Load("Imagens/Background.jpg");


	//botões da tela principal
	SDL_Surface *surBotoesIniciais[5][2];

	surBotoesIniciais[0][0] = IMG_Load("Imagens/Menu/BotaoIniciar.png");
	surBotoesIniciais[1][0] = IMG_Load("Imagens/Menu/BotaoRecordes.png");
	surBotoesIniciais[2][0] = IMG_Load("Imagens/Menu/BotaoOpcoes.png");
	surBotoesIniciais[3][0] = IMG_Load("Imagens/Menu/BotaoSobre.png");
	surBotoesIniciais[4][0] = IMG_Load("Imagens/Menu/BotaoSair.png");
	//botões da tela incial selecionados
	surBotoesIniciais[0][1] = IMG_Load("Imagens/Menu/BotaoIniciar_Selecionado.png");
	surBotoesIniciais[1][1] = IMG_Load("Imagens/Menu/BotaoRecordes_Selecionado.png");
	surBotoesIniciais[2][1] = IMG_Load("Imagens/Menu/BotaoOpcoes_Selecionado.png");
	surBotoesIniciais[3][1] = IMG_Load("Imagens/Menu/BotaoSobre_Selecionado.png");
	surBotoesIniciais[4][1] = IMG_Load("Imagens/Menu/BotaoSair_Selecionado.png");

	//botões dentro da tag INICIAR
	SDL_Surface *surBotoesSubMenuIniciar[2][2];

	surBotoesSubMenuIniciar[0][0] = IMG_Load("Imagens/Menu/SubMenuIniciar/BotaoContinuar.png");
	surBotoesSubMenuIniciar[1][0] = IMG_Load("Imagens/Menu/SubMenuIniciar/BotaoNovoJogo.png");
	//botões selecionados
	surBotoesSubMenuIniciar[0][1] = IMG_Load("Imagens/Menu/SubMenuIniciar/BotaoContinuar_Selecionado.png");
	surBotoesSubMenuIniciar[1][1] = IMG_Load("Imagens/Menu/SubMenuIniciar/BotaoNovoJogo_Selecionado.png");

	//botões dentro da tag de OPÇÕES
	SDL_Surface *surBotoesSubMenuOpcoes[3][2];
	SDL_Surface *surBotaoOnOff[2][2]; //auxiliar que vai mostrar o estado da opção pro jogador

	surBotoesSubMenuOpcoes[0][0] = IMG_Load("Imagens/Menu/SubMenuOpcoes/BotaoSFX.png");
	surBotoesSubMenuOpcoes[1][0] = IMG_Load("Imagens/Menu/SubMenuOpcoes/BotaoMusica.png");
	surBotoesSubMenuOpcoes[2][0] = IMG_Load("Imagens/Menu/SubMenuOpcoes/BotaoControles.png");
	//botões selecionados
	surBotoesSubMenuOpcoes[0][1] = IMG_Load("Imagens/Menu/SubMenuOpcoes/BotaoSFX_Selecionado.png");
	surBotoesSubMenuOpcoes[1][1] = IMG_Load("Imagens/Menu/SubMenuOpcoes/BotaoMusica_Selecionado.png");
	surBotoesSubMenuOpcoes[2][1] = IMG_Load("Imagens/Menu/SubMenuOpcoes/BotaoControles_Selecionado.png");
		//auxiliares on/off
	surBotaoOnOff[0][0] = IMG_Load("Imagens/Menu/SubMenuOpcoes/OnOff_Off.png");
	surBotaoOnOff[1][0] = IMG_Load("Imagens/Menu/SubMenuOpcoes/OnOff_On.png");
	//botões selecionados
	surBotaoOnOff[0][1] = IMG_Load("Imagens/Menu/SubMenuOpcoes/OnOff_Off_Selecionado.png");
	surBotaoOnOff[1][1] = IMG_Load("Imagens/Menu/SubMenuOpcoes/OnOff_On_Selecionado.png");



	//Rect dos botões
	SDL_Rect rectBotoes[5];
	//posicao dos botoes
	rectBotoes[0].x = OxPrimeiroBotao;
	rectBotoes[0].y = OyPrimeiroBotao;

	rectBotoes[1].x = OxSegundoBotao;
	rectBotoes[1].y = OySegundoBotao;

	rectBotoes[2].x = OxTerceiroBotao;
	rectBotoes[2].y = OyTerceiroBotao;

	rectBotoes[3].x = OxQuartoBotao;
	rectBotoes[3].y = OyQuartoBotao;

	rectBotoes[4].x = OxQuintoBotao;
	rectBotoes[4].y = OyQuintoBotao;

	//posição do botão On/off
	SDL_Rect rectBotoesOnOff[3];

	int i;
	for(i=0;i<3;i++)
	{
			rectBotoesOnOff[i].x = rectBotoes[i].x + AcrescimoDeXemOnOff;
			rectBotoesOnOff[i].y = rectBotoes[i].y;
	}

	//botões que aparecem ao lado do botão sair para escolher sim ou não
	SDL_Surface *surBotaoSimNao[2][2];
	surBotaoSimNao[0][0] = IMG_Load("Imagens/Menu/SubMenuSair/BotaoNao.png");
	surBotaoSimNao[1][0] = IMG_Load("Imagens/Menu/SubMenuSair/BotaoSim.png");
	//botões selecionados
	surBotaoSimNao[0][1] = IMG_Load("Imagens/Menu/SubMenuSair/BotaoNao_Selecionado.png");
	surBotaoSimNao[1][1] = IMG_Load("Imagens/Menu/SubMenuSair/BotaoSim_Selecionado.png");

	//rect dos botões sim/não
	SDL_Rect rectBotaoSimNao[2];
	//posição botão não
	rectBotaoSimNao[0].x = OxQuintoBotao + AcrescimoEmX_Nao;
	rectBotaoSimNao[0].y = OyQuintoBotao - 4;
	//posição botão sim
	rectBotaoSimNao[1].x = OxQuintoBotao + AcrescimoEmX_Sim;
	rectBotaoSimNao[1].y = OyQuintoBotao + 2;

	//o que terá na tela de sobre
	SDL_Surface *surMenuSobre = IMG_Load("Imagens/Menu/MenuSobre/menu_sobre.png");
	SDL_Rect rectMenuSobre;

	rectMenuSobre.x = 30;
	rectMenuSobre.y = 30;

	//botão padrão de subtela... voltar
	SDL_Surface *surBotaoVoltar[2];
	surBotaoVoltar[0] = IMG_Load("Imagens/Menu/BotaoVoltar.png");
	surBotaoVoltar[1] = IMG_Load("Imagens/Menu/BotaoVoltar_Selecionado.png");
	//rect do botão voltar
	SDL_Rect rectBotaoVoltar;
	//posição do botão será definida em relação ao ultimo botão da tela

	//botão dentro da tag HIGH SCORE
	SDL_Surface *surBotaoLimpar[2];
	surBotaoLimpar[0] = IMG_Load("Imagens/Menu/SubMenuRecordes/BotaoLimparRecordes.png");
	surBotaoLimpar[1] = IMG_Load("Imagens/Menu/SubMenuRecordes/BotaoLimparRecordes_Selecionado.png");
	//rect do botão limpar
	SDL_Rect rectBotaoLimpar;
	rectBotaoLimpar.x = OxBotaoLimpar;
	rectBotaoLimpar.y = OyBotaoLimpar;

	//imagens para as setas nos controles
	SDL_Surface *surMostrarControles[5];
	surMostrarControles[0] = IMG_Load("Imagens/Menu/SubMenuOpcoes/Controles/Cima.png");
	surMostrarControles[1] = IMG_Load("Imagens/Menu/SubMenuOpcoes/Controles/Direita.png");
	surMostrarControles[2] = IMG_Load("Imagens/Menu/SubMenuOpcoes/Controles/Baixo.png");
	surMostrarControles[3] = IMG_Load("Imagens/Menu/SubMenuOpcoes/Controles/Esquerda.png");
	surMostrarControles[4] = IMG_Load("Imagens/Menu/SubMenuOpcoes/Controles/Bomba.png");

	//botoes para as setas nos controles
	SDL_Rect rectMostraControles[5];

	rectMostraControles[0].x = OxMostrarControles;
	rectMostraControles[0].y = OyPrimeiroBotao;

	rectMostraControles[1].x = OxMostrarControles;
	rectMostraControles[1].y = OySegundoBotao;

	rectMostraControles[2].x = OxMostrarControles;
	rectMostraControles[2].y = OyTerceiroBotao;

	rectMostraControles[3].x = OxMostrarControles;
	rectMostraControles[3].y = OyQuartoBotao;

	rectMostraControles[4].x = OxMostrarControles - (surMostrarControles[4]->w/2);
	rectMostraControles[4].y = OyQuintoBotao;


	//a ideia aqui é criar um pequeno morcego que ficará "voando" ao lado do menu selecionado ...
	SDL_Surface *surMorcegoSelect[2];
	surMorcegoSelect[0] = IMG_Load("Imagens/Menu/Selecao/MorcegoSelect0.png");
	surMorcegoSelect[1] = IMG_Load("Imagens/Menu/Selecao/MorcegoSelect1.png");

	SDL_Rect rectMorcegoSelect;
	//a posição do morcego será definida a partir do botão que for selecionado, ou seja, só na hora de blitar na tela
	//....porém, devido a problemas com a primeira blitagem ele será definido como no 1º botão aqui
	rectMorcegoSelect.x = OxPrimeiroBotao + 70 + IncrementoDeSelecaoX + AcrescimoEmXMorcego;
	rectMorcegoSelect.y = OyPrimeiroBotao;

//----------------------------INICIANDO TELA--------------------------------------------------
	SDL_BlitSurface(Fundo, NULL, screen, NULL);

	for(i=0;i<5;i++)
	{
		if(i!=MenuSelecionado)
			SDL_BlitSurface(surBotoesIniciais[i][0], NULL, screen, &rectBotoes[i]);
		else
		{
			BlitarBotaoSelecionado(surBotoesIniciais[i][1], rectBotoes[i], screen);
			SDL_BlitSurface(surMorcegoSelect[i], NULL, screen, &rectMorcegoSelect);
		}


	}


	SDL_Flip(screen);

//----------------------------- EVENTOS E CONTROLE INTERNO DO MENU-----------------------------

	SDL_Event evento;
	int quit = 0;


	//==================================
	/*Serve para começar a tocar a musica desejada
	 Obs: O parâmetro "-1" na função indica que a musica
	 irá ser repetida infinitamente ou até fechar o programa.*/

	Mix_PlayMusic(musica_fundo, -1);
	//===================================




	while (!quit)
	{
		if(ControladorDeTempo >= 5)
		{
			if(ControladorMorcego == 1)
				ControladorMorcego = 0;
			else
				ControladorMorcego = 1;

			ControladorDeTempo = 0;
		}
		else
			ControladorDeTempo++;

		while(SDL_PollEvent(&evento))
		{


		  //=============================================
		  /*Como existe a opção de desligar os sons e a música do jogo,
		   foi necessária a criação dessas condições:

		   Mix_Volume-------->Meche com o volume dos sons(parametro
		   -1 utilizado para indicar que isso servirá para todos os tipos
		   de Mix_Chunk que vc esta utilizando.

		   Mix_PauseMusic(pausar a musica) e Mix_ResumeMusic(voltar a tocar a musica)------->
		   Creio que não seja necessário explicar...
		   pois a propria função diz o que ela faz.*/

		    if(EstadoOpcoes[0] == 0)
		      Mix_Volume(-1, 0);
		    else
		      Mix_Volume(-1, 128);

		    if(EstadoOpcoes[1] == 0)
		      Mix_PauseMusic();
		    else
		      Mix_ResumeMusic();

		    //==============================================


			if(evento.type == SDL_KEYDOWN)
			{
				switch(evento.key.keysym.sym)
				{
					case SDLK_DOWN:
					//se apertar a tecla pra baixo desce, mas depende da página que esta e do indice
					{


					  //===========================================
					  //Tocará o som indicado quando ocorrer o evento de movimentar as opções.

					    Mix_PlayChannel(-1,som_selecao_movimento,0);
					  //===========================================

						switch (Pagina) //PAGINA
						{
							case 0: //PAGINA PRINCIPAL
							{
								if( (MenuSelecionado==4) && (MostrarSimNao == 1) )
								{
									MostrarSimNao = 0;
									SimOuNaoSelecionado = 0;
								}

								MenuSelecionado ++; //AUMENTA O MENU
								if(MenuSelecionado == 5) //IMPEDE QUE PASSE DO LIMITE
									MenuSelecionado = 0;

								break; //SAI DO SWITCH PÁGINA
							}
							case 1: //PAGINA 1 (QUALQUER SUBMENU)
							{
								switch (MenuSelecionado)
								{
									case 2: //OPÇÕES
									{
										SubMenuSelecionado++; //AUMENTA O SUBMENU
										if(SubMenuSelecionado == 4)
										//IMPEDE QUE PASSE DO LIMITE
											SubMenuSelecionado = 0;
										break;//SAI DO SWITCH DE ESCOLHER MENU
									}
								}
								break;
							}
						}
						break;
					}
					case SDLK_UP:
					// se apertar a tecla pra cima sobe
					{


					  //==========================================
					  Mix_PlayChannel(-1,som_selecao_movimento,0);
					  //==========================================

						switch (Pagina)
						{
							case 0:
							{
								if( (MenuSelecionado==4) && (MostrarSimNao == 1) )
								{
									MostrarSimNao = 0;
									SimOuNaoSelecionado = 0;
								}
								MenuSelecionado--; //DIMINUI O MENU
								if(MenuSelecionado == -1) //IMPEDE QUE PASSE DO LIMITE
									MenuSelecionado = 4;

								break;
							}
							case 1:
							{
								switch(MenuSelecionado)
								{
									case 2:
									{
										SubMenuSelecionado--; //AUMENTA O SUBMENU
										if(SubMenuSelecionado == -1)//IMPEDE QUE PASSE DO LIMITE
											SubMenuSelecionado = 3;
										break;
									}
								}
							}
						}

						break;
					}

					case SDLK_RIGHT:
					//FALTANDO HS

					//apertar para direita só irá acontecer na high score e nas opções
					{

					  //============================================
					    Mix_PlayChannel(-1,som_selecao_movimento,0);
					  //============================================



						if( (Pagina == 0) && (MenuSelecionado == 4) && (MostrarSimNao == 1))
						{
							SimOuNaoSelecionado++;
							if(SimOuNaoSelecionado > 1)
								SimOuNaoSelecionado--;
						}
						if((Pagina == 1) && (MenuSelecionado == 1))
						{
							SubMenuSelecionado++;
							if (SubMenuSelecionado==1)
								SubMenuSelecionado--;
						}
						//talvez precise criar algo pra confirmação do apagar recordes
						if( (Pagina == 1) && (MenuSelecionado == 2) )
						{
							switch (SubMenuSelecionado)
							{
								case 0:
								{
									EstadoOpcoes[0] = 0;
									break;
								}
								case 1:
								{
									EstadoOpcoes[1] = 0;
									break;
								}
							}
						}

						break;
					}
					case SDLK_LEFT:
					//FALTANDO HS
					//apertar para esquerda só irá acontecer na high score e nas opções
					{


					   //=========================================
					    Mix_PlayChannel(-1,som_selecao_movimento,0);
					    //=========================================



						if( (Pagina == 0) && (MenuSelecionado == 4) && (MostrarSimNao == 1) )
						{
							SimOuNaoSelecionado--;
							if(SimOuNaoSelecionado < 0)
								SimOuNaoSelecionado++;
						}
						if((Pagina == 1) && (MenuSelecionado == 1))
						{
							SubMenuSelecionado--;
							if (SubMenuSelecionado == -1)
								SubMenuSelecionado++;
						}
						if( (Pagina == 1) && (MenuSelecionado == 2) )
						{
							switch (SubMenuSelecionado)
							{
								case 0:
								{
									EstadoOpcoes[0] = 1;
									break;
								}
								case 1:
								{
									EstadoOpcoes[1] = 1;
									break;
								}
							}
						}

						break;
					}
					case SDLK_RETURN:
					// se apertar enter seleciona e entra na proxima tela ****DESENVOLVER UM MECANISMO PARA FINALIZAR****
					{

					   //=====================================
					    Mix_PlayChannel(-1,som_selecao_enter,0);
					    //======================================


						switch (Pagina)
						{
							case 0:
							{
								if(MenuSelecionado == 4)//MENU SAIR
								{
									if(MostrarSimNao == 1)//ESTÁ MOSTRANDO SIM/NÃO
									{
										if(SimOuNaoSelecionado == 1)//SE SIM PG++ E SAI DA TELA
										{
											Pagina++;
											Sair = 1;
										}
										else
											MostrarSimNao = 0; //PARA DE MOSTRAR O SIM/NÃO
									}
									else
									{
										MostrarSimNao = 1;// SE NÃO ESTÁ MOSTRANDO, MOSTRA
										SimOuNaoSelecionado = 1;//SELECIONA O SIM
									}
								}
								else
								{
									Pagina++; // QUALQUER OUTRO MENU AUMENTA A PÁGINA
									if(MenuSelecionado == 0)
										Sair = 1;
									else
										SubMenuSelecionado = 0;//E COMEÇA NO INICIO DA TELA
								}

								break;
							}
							case 1:
							{
								switch(MenuSelecionado)
								{
									case 1://SUBMENU HIGHSCORES
									{

                                    //=================================================
                                     //SNK
                                    //====================================================


									  //====================================
									  mostra_recorde(screen);
									  //=====================================
									  Pagina = 0;
									  MenuSelecionado = 0;


										switch (SubMenuSelecionado)
										{
											case 0:
											{
												if(MostrarSimNao == 0)
													MostrarSimNao == 1;
												else
												{
													if(SimOuNaoSelecionado == 1)
													{
														//FALTANDO
														//código pra zerar o HS
													}
													else
														MostrarSimNao = 0;
												}
												break;
											}
											case 1:
											{
												Pagina--;
												if(Pagina == -1)
													Pagina++;
												SubMenuSelecionado = 0;
												break;
											}
										}
										break;
									}
									case 2://SUBMENU OPÇÕES
									{
										if(SubMenuSelecionado == 2)
										{
											//FALTANDO
											Pagina++;//TELA QUE MOSTRA CONTROLES PG 2, MENU 2, SUB MENU 2
										}
										if(SubMenuSelecionado == 3)
											Pagina --;

										break;
									}
									case 3:
									{
										if(SubMenuSelecionado == 0)
										{
											Pagina--;
										}
										break;
									}
								}

								break;
							}
							case 2:
							{
								switch(MenuSelecionado)
								{
									case 2:
									{
										if(SubMenuSelecionado == 2)
											Pagina--;
										break;
									}
								}
								break;
							}
						}

						break;
					}
					case SDLK_ESCAPE:
					{

					  //=======================================
					    Mix_PlayChannel(-1,som_selecao_enter,0);
					  //======================================


						if(Pagina > 0)
						{
							Pagina--;
							if(Pagina == 0)
								SubMenuSelecionado = -1;
							else
								SubMenuSelecionado = 0;
							break;
						}
						if(Pagina==0)
						{
							if( (MenuSelecionado == 4) && (MostrarSimNao == 1) )
								MostrarSimNao = 0;
							else
							{
								MostrarSimNao = 1;
								SimOuNaoSelecionado = 1;
								MenuSelecionado = 4;
							}
						}

						break;
					}
					default: break;

				}
			}
		}

		//----------------------------------------------VISUALIZAÇÃO DO MENU BASEADA NOS EVENTOS-----------------
		SDL_BlitSurface(Fundo, NULL, screen, NULL);

		switch (Pagina)
		{
			case 0: //TELA INICIAL
			{
				for(i=0; i<5; i++)
				{
					if(i != MenuSelecionado)
						SDL_BlitSurface(surBotoesIniciais[i][0], NULL, screen, &rectBotoes[i]);
					else
					{
						BlitarBotaoSelecionado(surBotoesIniciais[i][1], rectBotoes[i], screen);
						rectMorcegoSelect.x = rectBotoes[i].x + surBotoesIniciais[i][1]->w + IncrementoDeSelecaoX + AcrescimoEmXMorcego;
						rectMorcegoSelect.y = rectBotoes[i].y;
					}
				}

				if(MostrarSimNao == 1)
				{
					if(SimOuNaoSelecionado == 1)
					{
						SDL_BlitSurface(surBotaoSimNao[1][1], NULL, screen, &rectBotaoSimNao[1]);

						rectMorcegoSelect.x = rectBotaoSimNao[1].x + surBotaoSimNao[1][1]->w + AcrescimoEmXMorcego;
						rectMorcegoSelect.y = rectBotaoSimNao[1].y;

						SDL_BlitSurface(surBotaoSimNao[0][0], NULL, screen, &rectBotaoSimNao[0]);
					}
					else
					{
						SDL_BlitSurface(surBotaoSimNao[1][0], NULL, screen, &rectBotaoSimNao[1]);
						SDL_BlitSurface(surBotaoSimNao[0][1], NULL, screen, &rectBotaoSimNao[0]);

						rectMorcegoSelect.x = rectBotaoSimNao[0].x + surBotaoSimNao[0][1]->w + AcrescimoEmXMorcego;
						rectMorcegoSelect.y = rectBotaoSimNao[0].y;
					}
				}
				break;
			}
			case 1: //ALGUM SUBMENU
			{
				switch (MenuSelecionado) // VER QUAL É O SUBMENU EM RELAÇÃO À OPÇÃO ESCOLHIDA
				{
					case 0: //MENU INICIAR
					{
						if(Sair == 1)
							return 1;

						break;
					}
					case 1: // MENU HIGH SCORE
					{
						//****função que coloca o highscore na tela****
						mostra_recorde(screen);
						Pagina = 0;
						/*rectBotaoVoltar.x = rectBotaoLimpar.x + AcrescimoXnoBotaoVoltar;
						rectBotaoVoltar.y = rectBotaoLimpar.y;

						if(SubMenuSelecionado == 0)
						{
							BlitarBotaoSelecionado(surBotaoLimpar[1], rectBotaoLimpar, screen);
							rectMorcegoSelect.x = rectBotaoLimpar.x + surBotaoLimpar[1]->w + AcrescimoEmXMorcego;
							rectMorcegoSelect.y = rectBotaoLimpar.y;

							SDL_BlitSurface(surBotaoVoltar[0], NULL, screen, &rectBotaoVoltar);
						}
						if(SubMenuSelecionado == 1)
						{
							SDL_BlitSurface(surBotaoLimpar[0], NULL, screen, &rectBotaoLimpar);

							//rectSelecao.y = rectBotaoVoltar.y
							BlitarBotaoSelecionado(surBotaoVoltar[1], rectBotaoVoltar, screen);
							rectMorcegoSelect.x = rectBotaoVoltar.x + surBotaoVoltar[1]->w + AcrescimoEmXMorcego;
							rectMorcegoSelect.y = rectBotaoVoltar.y;
						}
						if(MostrarSimNao == 1)
						{
							//FALTANDO
							//definir uma posição pro sim/não e blitar na tela
						}*/

						break;
					}
					case 2: //OPÇÕES
					{
						for(i=0;i<3;i++)
						{
							if(i!=SubMenuSelecionado)
							{
								SDL_BlitSurface(surBotoesSubMenuOpcoes[i][0], NULL, screen, &rectBotoes[i]);
							}
							else
							{
								//rectSelecao.y = rectBotoes[i].y
								SDL_BlitSurface(surBotoesSubMenuOpcoes[i][1], NULL, screen, &rectBotoes[i]);
								rectMorcegoSelect.x = rectBotoes[i].x + surBotoesSubMenuOpcoes[i][1]->w + AcrescimoEmXMorcego;
								rectMorcegoSelect.y = rectBotoes[i].y;
							}
						}

						rectBotaoVoltar.x = rectBotoes[2].x;
						rectBotaoVoltar.y = rectBotoes[2].y + AcrescimoYnoBotaoVoltar;

						if(SubMenuSelecionado != 3)
						{
							SDL_BlitSurface(surBotaoVoltar[0], NULL, screen, &rectBotaoVoltar);
						}
						else
						{
							SDL_BlitSurface(surBotaoVoltar[1], NULL, screen, &rectBotaoVoltar);
							rectMorcegoSelect.x = rectBotaoVoltar.x + surBotaoVoltar[1]->w + AcrescimoEmXMorcego;
							rectMorcegoSelect.y = rectBotaoVoltar.y;
						}

						for(i=0;i<2;i++)
						{
							if(EstadoOpcoes[i] == 0)
							{
								if(i!=SubMenuSelecionado)
									SDL_BlitSurface(surBotaoOnOff[0][0], NULL, screen, &rectBotoesOnOff[i]);
								else
								{
									SDL_BlitSurface(surBotaoOnOff[0][1], NULL, screen, &rectBotoesOnOff[i]);
								}
							}
							else
							{
									if(i!=SubMenuSelecionado)
										SDL_BlitSurface(surBotaoOnOff[1][0], NULL, screen, &rectBotoesOnOff[i]);
									else
									{
										SDL_BlitSurface(surBotaoOnOff[1][1], NULL, screen, &rectBotoesOnOff[i]);
									}
							}
						}
						break;
					}
					case 3: //MENU SOBRE
					{
						SDL_BlitSurface(surMenuSobre, NULL, screen, &rectMenuSobre);

						rectBotaoVoltar.x = rectMenuSobre.x + ((surMenuSobre->w)/2);
						rectBotaoVoltar.y = rectMenuSobre.y + surMenuSobre->h + 50;

						rectMorcegoSelect.x = rectBotaoVoltar.x + surBotaoVoltar[1]->w -60;
						rectMorcegoSelect.y = rectBotaoVoltar.y;
						BlitarBotaoSelecionado(surBotaoVoltar[1], rectBotaoVoltar, screen);
						ControladorDeTempo++;
						break;//SAI DO SWITCH DE ESCOLHER MENU
					}
					case 4:
					{
						if(Sair == 1)
						{
							return -1;
						}
					}
				}
				break;
			}
			case 2:
			{
				switch (MenuSelecionado)
				{
					case 0:
					{
						if(SubMenuSelecionado == 0)
						{
							//programar para mostrar a tela de escolha de saves...
						}
						if(SubMenuSelecionado == 1)
						{
							//programar para mostrar a tela de escolha de personagens...
						}
						break;
					}
					case 2:
					{
						if(SubMenuSelecionado == 2)
						{
							//==================================================
							SDL_BlitSurface(FundoControles,NULL,screen,NULL);
							//=================================================

							for(i=0;i<5;i++)
							{
								SDL_BlitSurface(surMostrarControles[i], NULL, screen, &rectMostraControles[i]);
							}

							//================================================================================




							TTF_SetFontStyle(fonte, TTF_STYLE_ITALIC);
							desenha_texto("Movimento para cima", screen, 220,100,fonte);
							TTF_SetFontStyle(fonte, TTF_STYLE_ITALIC);
							desenha_texto("Movimento para direita", screen, 220,180,fonte);
							TTF_SetFontStyle(fonte, TTF_STYLE_ITALIC);
							desenha_texto("Movimento para baixo", screen, 220,260,fonte);
							TTF_SetFontStyle(fonte, TTF_STYLE_ITALIC);
							desenha_texto("Movimento para esquerda", screen, 220, 340,fonte);
							TTF_SetFontStyle(fonte, TTF_STYLE_ITALIC);
							desenha_texto("Colocar bomba", screen, 230,410,fonte);


							//================================================================================

							rectBotaoVoltar.x = rectMenuSobre.x + ((surMenuSobre->w)/2);
							rectBotaoVoltar.y = OySextoBotao;

							rectMorcegoSelect.x = rectBotaoVoltar.x + surBotaoVoltar[1]->w + AcrescimoEmXMorcego;
							rectMorcegoSelect.y = rectBotaoVoltar.y;
							SDL_BlitSurface(surBotaoVoltar[1], NULL, screen, &rectBotaoVoltar);
							ControladorDeTempo++;

						}
					}
					case 4:
					{
						if(MostrarSimNao == 1)
						{
							if(SimOuNaoSelecionado == 1)
								return -1;
							else
							{
								Pagina = 0;
								SimOuNaoSelecionado = 0;
								MostrarSimNao = 0;
							}
						}
						break;
					}

				}
				// e a de escolha de controles...
				break;
			}
			case 3:
			{
			//*PROGRAMAR AS AÇÕES BASEADO QUE PAGINA 3 SIGNIFICA QUE APERTOU O ENTER E VAI SAIR PARA DA FUNCAO
				if( (MenuSelecionado == 0) && (SubMenuSelecionado == 0) )
				 //DEFINE QUE VAI CONTINUAR DE ALGUM JOGO SALVO
				{
					quit = 1;
					/*switch ()    <<< vai definir qual é o continuar que vai ser acessado
					{
						case :
						{
							return 1;
						}
						case :
						{
							return 2;
						}
					}*/
				}
				if( (MenuSelecionado == 0) && (SubMenuSelecionado == 1) )
				//DEFINE QUE VAI COMEÇAR UM NOVO JOGO
				{
					//return 0;
				}
				if( (MenuSelecionado == 2) && (SubMenuSelecionado == 2) )
				// TALVEZ ISSO SUMA, MAS É PARA SALVAR AS CONFIGURAÇÕES DE CONTROLE
				{

				}
				break;
			}
		}
		SDL_BlitSurface(surMorcegoSelect[ControladorMorcego], NULL, screen, &rectMorcegoSelect);
		SDL_Flip(screen);
	}


	//=====================================
	//Serve para liberar os sons(melhor dizendo, "descartá-los"...)

	Mix_FreeChunk(som_selecao_enter);
	Mix_FreeChunk(som_selecao_movimento);
	Mix_FreeMusic(musica_fundo);

	//======================================

}
