//g++ test_sdl.cpp $(sdl2-config --cflags --libs)

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    int statut = EXIT_FAILURE;
    SDL_Rect rect = {100, 100, 100, 100}, dst = {0, 0, 0, 0};
    
    SDL_Surface *tmp = NULL;

    SDL_Event event;
    SDL_bool quit = SDL_FALSE;
    

    if(0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }
    if(0 != SDL_CreateWindowAndRenderer(260, 615, SDL_WINDOW_SHOWN, &window, &renderer))
    {
        fprintf(stderr, "Erreur SDL_CreateWindowAndRenderer : %s", SDL_GetError());
        goto Quit;
    }


    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderTarget(renderer, NULL); /* Le renderer est la cible de rendu. */


    tmp = SDL_LoadBMP("1110i-v2.bmp");//260*615pixel
    if(NULL == tmp)
    {
        fprintf(stderr, "Erreur SDL_LoadBMP : %s\n", SDL_GetError());
        goto Quit;
    }
    texture = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp); //On libère la surface, on n’en a plus besoin 
    if(NULL == texture)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s", SDL_GetError());
        goto Quit;
    }

    /* On récupère les dimensions de la texture, on la copie sur le renderer
       et on met à jour l’écran. */
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_RenderPresent(renderer);
    statut = EXIT_SUCCESS;


    while(!quit)
    {
        SDL_WaitEvent(&event);
        if(event.type == SDL_QUIT)
            quit = SDL_TRUE;
    }       
    /* On ferme notre fenêtre et on quitte le programme */


Quit:
    if(NULL != texture)
        SDL_DestroyTexture(texture);
    if(NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if(NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    return statut;
}