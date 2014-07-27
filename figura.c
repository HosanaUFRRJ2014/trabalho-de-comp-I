typedef struct figura{                   //struct que facilita a inserção de informações das imagens na tela!
    int x;    //posição X da imagem
    int y;    //posição Y da imagem
    SDL_Surface *imagem; //a imagem!
}Figura;



void desenha_texto(char* texto, SDL_Surface* dst, int x, int y, TTF_Font *fonte)
{
  SDL_Color cor = {0, 255, 0};
  SDL_Surface* src = TTF_RenderText_Blended(fonte, texto, cor);
  SDL_Rect dst_rect = {x, y, 0, 0};
  
  SDL_BlitSurface(src, NULL, dst, &dst_rect);
  
  SDL_FreeSurface(src);
}

void desenha_texto2(char* texto, SDL_Surface* dst, int x, int y, TTF_Font *fonte)
{
  SDL_Color cor = {255, 255, 255};
  SDL_Surface* src = TTF_RenderText_Blended(fonte, texto, cor);
  SDL_Rect dst_rect = {x, y, 0, 0};
  
  SDL_BlitSurface(src, NULL, dst, &dst_rect);
  
  SDL_FreeSurface(src);
}


void apply_surface(Figura figura,SDL_Surface* destination) //função facilitadora de colocar as imagens na tela.
{
    SDL_Surface *source = figura.imagem;
    SDL_Rect offset;
     //Give the offsets to the rectangle
    offset.x = figura.x;
    offset.y = figura.y;


    SDL_BlitSurface( source, NULL, destination, &offset );
}

void apply_texto(Figura *figura,SDL_Surface* destination)
{
    SDL_Surface *source = figura->imagem;
    SDL_Rect offset;
     //Give the offsets to the rectangle
    offset.x = figura->x;
    offset.y = figura->y;


    SDL_BlitSurface( source, NULL, destination, &offset );
}
