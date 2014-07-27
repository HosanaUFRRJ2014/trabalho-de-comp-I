#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


#define LinhasDoCampo 15
#define ColunasDoCampo 19

#define CIMA 0
#define DIREITA 1
#define BAIXO 2
#define ESQUERDA 3

#define JaPassou 4 // são os valores que vo ser utilizados pra evitar loop infinito na função de caminho do mob
#define TemMonstro 5

#define MORCEGO 0
#define ROBO 3
#define MESTRE -1
#define TemPersonagem 6
#define DanoRobo 30

#define ConstReducaoImagemMorcego 15
#define ConstReducaoImagemPersonagem 0

#define BarrasDeLifePadraoRobo 3

typedef struct
{
	int i;
	int j;
}PosicaoDoMapa;

typedef struct
{
	int TipoDoMonstro;
	int Ataque; //varia de 0 para não agarrado e 1 para agarrado... se refere ao ataque do morcego
	int TempoAtaque;
	int DanoQueCausa; // o dano que a agarrada causará por milisegundo (caso morcego) ou por hit
	int Resistencia; // vai ser o número usado para medir a resistência que o morcego terá as setas direcionais
	int EstahVivo; // 0 para o morcego morre, e 1 para o morcego esta vivo
	PosicaoDoMapa UltimaPosicao; //guarda o ultimo rect pelo qual o mob passou (em campo e em mapa)
	PosicaoDoMapa PosicaoAtual; // talvez UltimaPosicaoMorra
	PosicaoDoMapa ProximaPosicao;
	SDL_Rect Rect;
	int DirecaoQueVeio; // 0 = direita, 1 = esquerda, 2 = cima ou 3 = baixo em relação ao rect anterior que se encontra
}Monstro;

typedef struct
{
    Monstro Padrao;
    PosicaoDoMapa Caminho[500];
    int DirecaoQueVai[500];
    int LimiteCaminho;
    Uint16 *HP;
    int BarrasDeLife;
    int HorizontalAndar;
    int VerticalAndar;
    //terminar as outras caracteristicas dos mestres
}Mestre;


void AjudaTestar (SDL_Rect rect, SDL_Surface *screen)//pinta de verde o rect (pra teste)
{
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 255, 0));
	SDL_Flip(screen);
	//SDL_Delay(80);
}

void AjudaTestar2(SDL_Rect rect, SDL_Surface *screen)//pinta de branco o rect (pra teste)
{
	SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 255,255,255));
	SDL_Flip(screen);
	//SDL_Delay(20);
}


int PersonagemAoLado(int mapa[LinhasDoCampo][ColunasDoCampo], PosicaoDoMapa PosicaoMonstro, PosicaoDoMapa *PosicaoComPersonagem)
{
	PosicaoDoMapa P;
	if(mapa[PosicaoMonstro.i-1][PosicaoMonstro.j] == TemPersonagem)
	{
		P = PosicaoMonstro;
		P.i -= 1;
		PosicaoComPersonagem = &P;
		return 1;
	}
	else if(mapa[PosicaoMonstro.i+1][PosicaoMonstro.j] == TemPersonagem)	
		{
			P = PosicaoMonstro;
			P.i += 1;
			PosicaoComPersonagem = &P;
			return 1;
		}
	else if(mapa[PosicaoMonstro.i][PosicaoMonstro.j-1] == TemPersonagem)
		{
			P = PosicaoMonstro;
			P.j -= 1;
			PosicaoComPersonagem = &P;
			return 1;
		}
	else if(mapa[PosicaoMonstro.i][PosicaoMonstro.j+1] == TemPersonagem)
		{
			P = PosicaoMonstro;
			P.j += 1;
			PosicaoComPersonagem = &P;
			return 1;
		}
	else
	{
		PosicaoComPersonagem = NULL;
		return 0;
	}
}

/*int ColidiuComPersonagem(SDL_Rect RectMonstro, SDL_Rect RectPersonagem, int DirecaoQueVeioMonstro, int TipoDoMonstro)
{
	int ConstanteDeImagem;
	if(TipoDoMonstro == MORCEGO)
		ConstanteDeImagem = ConstReducaoImagemMorcego;

	switch(DirecaoQueVeioMonstro)
	{
		case CIMA:
		{
			//TERMINAR O CALCULO DA COLISÃO DO MONSTRO COM O PERSONAGEM
			if(RectMonstro.x + ConstanteDeImagem == RectPersonagem.x + ConstReducaoImagemPersonagem)
		}
	}
}*/

int NumeroDeDirecoesPraSeguir( PosicaoDoMapa posicaoAtual, PosicaoDoMapa LinhaColunaParaSeguir[3],
							   int mapa[LinhasDoCampo][ColunasDoCampo], int DirecaoQueVeio, int DirecaoQueVai[3], int TipoDeMonstro)
{
    //não precisa se preocupar com a ordem dos pontos no vetor pq o vetor "DirecaoQueVai" irá definir que...
    //a posicao n de linha/coluna tem sua respectiva direcao na msm posicao n de "DirecaoQueVai"
	int acumulador = 0;
	int i;
	for (i = 0; i < 3; ++i)
	{
		LinhaColunaParaSeguir[i].i = 0;
		LinhaColunaParaSeguir[i].j = 0;
	}

	switch(DirecaoQueVeio)
	{
		case DIREITA:
		{
			if( (mapa[posicaoAtual.i-1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i-1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].i -= 1;
				DirecaoQueVai[acumulador] = BAIXO;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i+1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i + 1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].i += 1;
				DirecaoQueVai[acumulador] = CIMA;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i][posicaoAtual.j-1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j+1] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].j -= 1;
				DirecaoQueVai[acumulador] = DIREITA;
				acumulador++;
			}
			if(acumulador==0)
			{
                if( (mapa[posicaoAtual.i][posicaoAtual.j+1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j+1] == TemMonstro && TipoDeMonstro == MESTRE) )
                {
                    LinhaColunaParaSeguir[acumulador] = posicaoAtual;
                    LinhaColunaParaSeguir[acumulador].j += 1;
                    DirecaoQueVai[acumulador] = ESQUERDA;
                    acumulador++;
                }
			}
			break;
		}
		case ESQUERDA:
		{
			if( (mapa[posicaoAtual.i-1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i-1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].i -= 1;
				DirecaoQueVai[acumulador] = BAIXO;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i][posicaoAtual.j+1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j+1] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].j += 1;
				DirecaoQueVai[acumulador] = ESQUERDA;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i+1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i+1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].i += 1;
				DirecaoQueVai[acumulador] = CIMA;
				acumulador++;
			}
			if(acumulador==0)
			{
                if( (mapa[posicaoAtual.i][posicaoAtual.j-1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j-1] == TemMonstro && TipoDeMonstro == MESTRE) )
                {
                   LinhaColunaParaSeguir[acumulador] = posicaoAtual;
                   LinhaColunaParaSeguir[acumulador].j -= 1;
                   DirecaoQueVai[acumulador] = DIREITA;
                   acumulador++;
                }
			}
			break;
		}
		case CIMA:
		{
			if( (mapa[posicaoAtual.i][posicaoAtual.j+1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j+1] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].j += 1;
				DirecaoQueVai[acumulador] = ESQUERDA;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i+1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i+1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].i += 1;
				DirecaoQueVai[acumulador] = CIMA;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i][posicaoAtual.j-1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j-1] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].j -= 1;
				DirecaoQueVai[acumulador] = DIREITA;
				acumulador++;
			}
			if(acumulador==0)
			{
                if( (mapa[posicaoAtual.i-1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i-1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
                {
                    LinhaColunaParaSeguir[acumulador] = posicaoAtual;
                    LinhaColunaParaSeguir[acumulador].i -= 1;
                    DirecaoQueVai[acumulador] = CIMA;
                    acumulador++;
                }
			}
			break;
		}
		case BAIXO:
		{
			if( (mapa[posicaoAtual.i-1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i-1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].i -= 1;
				DirecaoQueVai[acumulador] = BAIXO;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i][posicaoAtual.j+1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j+1] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].j += 1;
				DirecaoQueVai[acumulador] = ESQUERDA;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i][posicaoAtual.j-1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j-1] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].j -= 1;
				DirecaoQueVai[acumulador] = DIREITA;
				acumulador++;
			}
			if(acumulador==0)
			{
                if( (mapa[posicaoAtual.i+1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i+1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
                {
                    LinhaColunaParaSeguir[acumulador] = posicaoAtual;
                    LinhaColunaParaSeguir[acumulador].i += 1;
                    DirecaoQueVai[acumulador] = CIMA;
                    acumulador++;
                }
			}
			break;
		}
		default:
		{
			if( (mapa[posicaoAtual.i-1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i-1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].i -= 1;
				DirecaoQueVai[acumulador] = BAIXO;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i][posicaoAtual.j+1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j+1] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].j += 1;
				DirecaoQueVai[acumulador] = ESQUERDA;
				acumulador++;
			}
			if( (mapa[posicaoAtual.i+1][posicaoAtual.j] == 0) || (mapa[posicaoAtual.i+1][posicaoAtual.j] == TemMonstro && TipoDeMonstro == MESTRE) )
			{
				LinhaColunaParaSeguir[acumulador] = posicaoAtual;
				LinhaColunaParaSeguir[acumulador].i += 1;
				DirecaoQueVai[acumulador] = CIMA;
				acumulador++;
			}
			if(acumulador < 3)
			{
			   if( (mapa[posicaoAtual.i][posicaoAtual.j-1] == 0) || (mapa[posicaoAtual.i][posicaoAtual.j-1] == TemMonstro && TipoDeMonstro == MESTRE) )
               {
                   LinhaColunaParaSeguir[acumulador] = posicaoAtual;
                   LinhaColunaParaSeguir[acumulador].j -= 1;
                   DirecaoQueVai[acumulador] = DIREITA;
                   acumulador++;
               }
			}
	        break;
		}
	}
	return acumulador;
}

int Colidiu(SDL_Rect colisor, int mapa[LinhasDoCampo][ColunasDoCampo], SDL_Rect campo[LinhasDoCampo][ColunasDoCampo],
SDL_Rect *colidido, int *TipoColidido, int *Linha, int *Coluna)
{
	int i, j;
	for(i=0;i<LinhasDoCampo;i++)
		for(j=0;j<ColunasDoCampo;j++)
		{
			if( !(colisor.x + colisor.w>= campo[i][j].x) && (colisor.x <= campo[i][j].x + campo[i][j].w) )
				if( !(colisor.y + colisor.h >= campo[i][j].y) && (colisor.y <= campo[i][j].y + campo[i][j].h))
				{
					colidido = &campo[i][j];
					TipoColidido = &mapa[i][j];
					Linha = &i;
					Coluna = &j;
					return 1;
				}
		}
	return 0;
}


//função que vê rects iguais... pra não escrever um if grande onde já tem vários
int PosicaoDoRectEhIgual(SDL_Rect R1, SDL_Rect R2)
{
    if( (R1.x == R2.x) && (R1.y == R2.y) )
        return 1;
    else
        return 0;
}

//verifica se um monstro está dentro de um determinado rect do campo
int RectEstaContidoEmOutro(SDL_Rect R1, SDL_Rect R2)
{
	if( (R1.x + R1.w < R2.x + R2.w) && (R1.x >= R2.x) )
		if( (R1.y + R1.h < R2.y + R2.h) && (R1.y >= R2.y) )
			return 1;
		else
			return 0;
	else
		return 0;
}

int PosicaoDoMapaEhIgual(PosicaoDoMapa P1, PosicaoDoMapa P2)
{
	if( (P1.i == P2.i) && (P1.j == P2.j) )
		return 1;
	else
		return 0;
}

int MenorCaminho(int mapa[LinhasDoCampo][ColunasDoCampo], PosicaoDoMapa *VetorCaminho,
                 PosicaoDoMapa *GuardaCaminhoA, int LimiteVetor, PosicaoDoMapa PosicaoAtualMestre,
                 int *VetorDirecaoQueVai, int *GuardaDirecaoQueVaiA, int DirecaoQueVeio,
                 PosicaoDoMapa PosicaoDestino, int *EhSemSaida)
{
    int NumeroDeCaminhos = 0; // retorno da função numero de caminhos e condicionador para qual caso seguir
    PosicaoDoMapa AuxCaminho[3];//retorno da funcao numero de caminhos que guardará as possibilidades
    int ContadorMenorCaminho = 0;
    int AuxDirecaoQueVai[3];
    PosicaoDoMapa Zero;
    Zero.i = 0;
    Zero.j = 0;

    if( PosicaoDoMapaEhIgual(PosicaoAtualMestre, PosicaoDestino) )
    {
        return ContadorMenorCaminho;
    }
    else
    {
        NumeroDeCaminhos = NumeroDeDirecoesPraSeguir(PosicaoDestino, AuxCaminho, mapa, DirecaoQueVeio, AuxDirecaoQueVai, MESTRE);

        switch(NumeroDeCaminhos)
        {
            case 0:
            {
                *EhSemSaida = 1;
                break;
            }
            case 1:
            {
                mapa[AuxCaminho[0].i][AuxCaminho[0].j] = JaPassou;
                VetorCaminho[0] = AuxCaminho[0];
                VetorDirecaoQueVai[0] = AuxDirecaoQueVai[0];
                int a = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoA[1], LimiteVetor-1,
                        PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiA[1],
                        AuxDirecaoQueVai[0], AuxCaminho[0], EhSemSaida);

                ContadorMenorCaminho++;
                ContadorMenorCaminho += a;
                break;
            }
            case 2:
            {
                int i;
                mapa[AuxCaminho[0].i][AuxCaminho[0].j] = JaPassou;
                VetorCaminho[0] = AuxCaminho[0];
                VetorDirecaoQueVai[0] = AuxDirecaoQueVai[0];
                int a = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoA[1], LimiteVetor-1,
                        PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiA[1],
                        AuxDirecaoQueVai[0], AuxCaminho[0], EhSemSaida);


                if(*EhSemSaida == 1)
                {
                    *EhSemSaida = 0;
                    for(i=0;i<=a;i++)
                    {
                        mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                        VetorCaminho[i] = Zero;
                        VetorDirecaoQueVai[i] = -1;
                    }

                    mapa[AuxCaminho[1].i][AuxCaminho[1].j] = JaPassou;
                    VetorCaminho[0] = AuxCaminho[1];
                    VetorDirecaoQueVai[0] = AuxDirecaoQueVai[1];
                    int b = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoA[1], LimiteVetor-1,
                            PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiA[1],
                            AuxDirecaoQueVai[1], AuxCaminho[1], EhSemSaida);

                    ContadorMenorCaminho++;
                    ContadorMenorCaminho+=b;
                }
                else
                {
                    for(i=0;i<=a;i++)
                    {
                        mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                        GuardaCaminhoA[i] = VetorCaminho[i];
                        GuardaDirecaoQueVaiA[i] = AuxDirecaoQueVai[i];
                        VetorCaminho[i] = Zero;
                        VetorDirecaoQueVai[i] = -1;
                    }

                    PosicaoDoMapa GuardaCaminhoB[LimiteVetor];
                    int GuardaDirecaoQueVaiB[LimiteVetor];
                    mapa[AuxCaminho[1].i][AuxCaminho[1].j] = JaPassou;
                    VetorCaminho[0] = AuxCaminho[1];
                    VetorDirecaoQueVai[0] = AuxDirecaoQueVai[1];
                    int b = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoB[0], LimiteVetor-1,
                            PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiB[1],
                            AuxDirecaoQueVai[1], AuxCaminho[1], EhSemSaida);

                    if(*EhSemSaida==1)
                    {
                        *EhSemSaida = 0;
                        for(i=0;i<=b;i++)
                        {
                            mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                            VetorCaminho[i] = Zero;
                            VetorDirecaoQueVai[i] = -1;
                        }

                        for(i=0;i<=a;i++)
                        {
                            VetorCaminho[i] = GuardaCaminhoA[i];
                            VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiA[i];
                            mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
                        }

                        ContadorMenorCaminho++;
                        ContadorMenorCaminho+=a;
                    }
                    else
                    {
                        if(a<b)
                        {
                        	for(i=0;i<=b;i++)
                        	{
                        		mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                        		VetorCaminho[i] = Zero;
                        		VetorDirecaoQueVai[i] = -1;
                        	}
                            for(i=0;i<=a;i++)
                            {
                                VetorCaminho[i] = GuardaCaminhoA[i];
                                VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiA[i];
                                mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
                            }

                            ContadorMenorCaminho++;
                            ContadorMenorCaminho+=a;
                        }
                        else
                        {
                            ContadorMenorCaminho++;
                            ContadorMenorCaminho+=b;
                            //não precisa fazer de novo pois b foi o ultimo a ser feito
                        }
                    }
                }

                break;
            }
            case 3:
            {
                int i;
                mapa[AuxCaminho[0].i][AuxCaminho[0].j] = JaPassou;
                VetorCaminho[0] = AuxCaminho[0];
                VetorDirecaoQueVai[0] = AuxDirecaoQueVai[0];
                int a = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoA[1], LimiteVetor-1,
                        PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiA[1],
                        AuxDirecaoQueVai[0], AuxCaminho[0], EhSemSaida);

                if(*EhSemSaida == 1)
                {
                    *EhSemSaida = 0;
                    for(i=0;i<=a;i++)
                    {
                        mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                        VetorCaminho[i] = Zero;
                        VetorDirecaoQueVai[i] = -1;
                    }

                    mapa[AuxCaminho[1].i][AuxCaminho[1].j] = JaPassou;
                    VetorCaminho[0] = AuxCaminho[1];
                    VetorDirecaoQueVai[0] = AuxDirecaoQueVai[1];
                    int b = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoA[1], LimiteVetor-1,
                            PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiA[1],
                            AuxDirecaoQueVai[1], AuxCaminho[1], EhSemSaida);

                    if(*EhSemSaida == 1)
                    {
                        *EhSemSaida = 0;
                        for(i=0;i<=b;i++)
                        {
                            mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                            VetorCaminho[i] = Zero;
                            VetorDirecaoQueVai[i] = -1;
                        }

                        mapa[AuxCaminho[2].i][AuxCaminho[2].j] = JaPassou;
                        VetorCaminho[0] = AuxCaminho[2];
                        VetorDirecaoQueVai[0] = AuxDirecaoQueVai[2];
                        int c = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoA[1], LimiteVetor-1,
                                PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiA[1],
                                AuxDirecaoQueVai[2], AuxCaminho[2], EhSemSaida);

                        ContadorMenorCaminho++;
                        ContadorMenorCaminho+=c;
                    }
                    else
                    {

						for(i=0;i<=b;i++)
						{
							mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
							GuardaCaminhoA[i] = VetorCaminho[i];
							VetorCaminho[i] = Zero;

						}
                        PosicaoDoMapa GuardaCaminhoC[LimiteVetor];
                        int GuardaDirecaoQueVaiC[LimiteVetor];
                        mapa[AuxCaminho[2].i][AuxCaminho[2].j] = JaPassou;
                        VetorCaminho[0] = AuxCaminho[2];
                        VetorDirecaoQueVai[0] = AuxDirecaoQueVai[2];
                        int c = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoC[0], LimiteVetor-1,
                                PosicaoAtualMestre, &VetorDirecaoQueVai[2], &GuardaDirecaoQueVaiC[0],
                                AuxDirecaoQueVai[2], AuxCaminho[2], EhSemSaida);

                        if(*EhSemSaida == 1)
                        {
                            *EhSemSaida = 0;
                            for(i=0;i<=c;i++)
                            {
                                mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                                VetorCaminho[i] = Zero;
                                VetorDirecaoQueVai[i] = -1;
                            }

                            for(i=0;i<=b;i++)
                            {
                                VetorCaminho[i] = GuardaCaminhoA[i];
                                VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiA[i];
                                mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
                            }
                            ContadorMenorCaminho++;
                            ContadorMenorCaminho+=b;
                        }
                        else
                        {
                            if(b<c)
                            {

                            	for(i=0;i<=c;i++)
								{
									mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
									VetorCaminho[i] = Zero;
									VetorDirecaoQueVai[i] = -1;
								}
                                for(i=0;i<=b;i++)
                                {
                                    VetorCaminho[i] = GuardaCaminhoA[i];
                                    VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiA[i];
                                    mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
                                }

                                ContadorMenorCaminho++;
                                ContadorMenorCaminho+=b;
                            }
                            else
                            {
                                ContadorMenorCaminho++;
                                ContadorMenorCaminho+=c;
                            }
                        }
                    }
                }
                else
                {
                    for(i=0;i<=a;i++)
                    {
                    	mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                        GuardaCaminhoA[i] = VetorCaminho[i];
                        GuardaDirecaoQueVaiA[i] = VetorDirecaoQueVai[i];
                        VetorCaminho[i] = Zero;
                        VetorDirecaoQueVai[i] = -1;
                    }

                    PosicaoDoMapa GuardaCaminhoB[LimiteVetor];
                    int GuardaDirecaoQueVaiB[LimiteVetor];
                    mapa[AuxCaminho[1].i][AuxCaminho[1].j] = JaPassou;
                    VetorCaminho[0] = AuxCaminho[1];
                    VetorDirecaoQueVai[0] = AuxDirecaoQueVai[1];
                    int b = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoB[0], LimiteVetor-1,
                            PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiB[1],
                            AuxDirecaoQueVai[1], AuxCaminho[1], EhSemSaida);

                    if(*EhSemSaida==1)
                    {
                        *EhSemSaida = 0;
                        for(i=0;i<=b;i++)
                        {
                            mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                            VetorCaminho[i] = Zero;
                            VetorDirecaoQueVai[i] = -1;
                        }

                        mapa[AuxCaminho[2].i][AuxCaminho[2].j] = JaPassou;
                        VetorCaminho[0] = AuxCaminho[2];
                        VetorDirecaoQueVai[0] = AuxDirecaoQueVai[2];
                        int c = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoB[0], LimiteVetor-1,
                                PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiB[0],
                                AuxDirecaoQueVai[2], AuxCaminho[2], EhSemSaida);

                        if(*EhSemSaida == 1)
                        {
                            *EhSemSaida = 0;
                            for(i=0;i<=c;i++)
                            {
                                mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                                VetorCaminho[i] = Zero;
                                VetorDirecaoQueVai[i] = -1;
                            }

                            for(i=0;i<=a;i++)
                            {
                                VetorCaminho[i] = GuardaCaminhoA[i];
                                VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiA[i];
                                mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
                            }
                            ContadorMenorCaminho++;
                            ContadorMenorCaminho+=a;
                        }
                        else
                        {
                            if(a<c)
                            {
                            	for(i=0;i<=c;i++)
								{
									mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
									VetorCaminho[i] = Zero;
									VetorDirecaoQueVai[i] = -1;
								}
                                for(i=0;i<=a;i++)
                                {
                                    VetorCaminho[i] = GuardaCaminhoA[i];
                                    VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiA[i];
                                    mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
                                }

                                ContadorMenorCaminho++;
                                ContadorMenorCaminho+=a;
                            }
                            else
                            {
                                ContadorMenorCaminho++;
                                ContadorMenorCaminho+=c;
                            }
                        }
                    }
                    else
                    {
                        if(a<b)
                        {
                            for(i=0;i<=b;i++)
                            {
                                mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                                VetorCaminho[i] = Zero;
                                VetorDirecaoQueVai[i] = -1;
                                GuardaCaminhoB[i] = Zero;
                                GuardaDirecaoQueVaiB[i] = -1;
                            }

                            mapa[AuxCaminho[2].i][AuxCaminho[2].j] = JaPassou;
                            VetorCaminho[0] = AuxCaminho[2];
                            VetorDirecaoQueVai[0] = AuxDirecaoQueVai[2];
                            int c = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoB[0], LimiteVetor-1,
                                    PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiB[0],
                                    AuxDirecaoQueVai[2], AuxCaminho[2], EhSemSaida);

                            if(*EhSemSaida == 1)
                            {
                                *EhSemSaida = 0;
                                for(i=0;i<=c;i++)
                                {
                                    mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                                    VetorCaminho[i] = Zero;
                                    VetorDirecaoQueVai[i] = -1;
                                }

                                for(i=0;i<=a;i++)
                                {
                                    VetorCaminho[i] = GuardaCaminhoA[i];
                                    VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiA[i];
                                    mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
                                }

                                ContadorMenorCaminho++;
                                ContadorMenorCaminho+=a;
                            }
                            else
                            {
                                if(a<c)
                                {
                                	for(i=0;i<=c;i++)
									{
										mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
										VetorCaminho[i] = Zero;
										VetorDirecaoQueVai[i] = 0;
									}
                                    for(i=0;i<=a;i++)
                                    {
                                        VetorCaminho[i] = GuardaCaminhoA[i];
                                        VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiA[i];
                                        mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
                                    }

                                    ContadorMenorCaminho++;
                                    ContadorMenorCaminho+=a;
                                }
                                else
                                {
                                    ContadorMenorCaminho++;
                                    ContadorMenorCaminho+=c;
                                }
                            }
                        }
                        else
                        {
                        	for(i=0;i<=a;i++)
                        	{
                        		GuardaCaminhoA[i] = Zero;
                        		GuardaDirecaoQueVaiA[i] = -1;
							}
                        	for(i=0;i<=b;i++)
                        	{
								mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
								GuardaCaminhoB[i] = VetorCaminho[i];
								VetorCaminho[i] = Zero;
								VetorDirecaoQueVai[i] = -1;
                        	}

                            mapa[AuxCaminho[2].i][AuxCaminho[2].j] = JaPassou;
                            VetorCaminho[0] = AuxCaminho[2];
                            VetorDirecaoQueVai[0] = AuxDirecaoQueVai[2];
                            int c = MenorCaminho(mapa, &VetorCaminho[1], &GuardaCaminhoA[1], LimiteVetor-1,
                                    PosicaoAtualMestre, &VetorDirecaoQueVai[1], &GuardaDirecaoQueVaiA[1],
                                    AuxDirecaoQueVai[2], AuxCaminho[2], EhSemSaida);

							if(*EhSemSaida == 1)
							{
                                *EhSemSaida = 0;
                                for(i=0;i<=c;i++)
                                {
                                    mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
                                    VetorCaminho[i] = Zero;
                                    VetorDirecaoQueVai[i] = -1;
                                }

                                for(i=0;i<=b;i++)
                                {
                                    VetorCaminho[i] = GuardaCaminhoB[i];
                                    VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiB[i];
                                    mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
                                }

                                ContadorMenorCaminho++;
                                ContadorMenorCaminho+=b;
							}
							else
							{
								if(b<c)
								{
									for(i=0;i<=c;i++)
									{
										mapa[VetorCaminho[i].i][VetorCaminho[i].j] = 0;
										VetorCaminho[i] = Zero;
										VetorDirecaoQueVai[i] = 0;
									}
									for(i=0;i<=b;i++)
									{
										VetorCaminho[i] = GuardaCaminhoB[i];
										VetorDirecaoQueVai[i] = GuardaDirecaoQueVaiB[i];
										mapa[VetorCaminho[i].i][VetorCaminho[i].j] = JaPassou;
									}

									ContadorMenorCaminho++;
									ContadorMenorCaminho+=b;
								}
								else
								{
									ContadorMenorCaminho++;
									ContadorMenorCaminho+=c;
								}
							}
						}
					}
				}

				break;
			}
		}
		return ContadorMenorCaminho;
	}
}
