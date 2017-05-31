#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define nbre_cases_hauteur 100
#define nbre_cases_largeur 160

int rand_a_b(int a, int b);


void refresh_screen(TTF_Font *police, int taille_case, SDL_Rect position[nbre_cases_largeur][nbre_cases_hauteur], SDL_Surface* ecran, SDL_Surface* cases[nbre_cases_largeur][nbre_cases_hauteur], int etat[nbre_cases_largeur][nbre_cases_hauteur], unsigned int compteur);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int taille_case = 10;
    int hauteur = 1000;
    int largeur = 1600;
    int vitesse = 5;

    SDL_Surface *ecran = NULL, *cases[nbre_cases_largeur][nbre_cases_hauteur];
    SDL_Rect position[nbre_cases_largeur][nbre_cases_hauteur];
    TTF_Font *police;

    int etat[nbre_cases_largeur][nbre_cases_hauteur];
    int ancien_etat[nbre_cases_largeur][nbre_cases_hauteur];
    int nbr_voisins_vivants[nbre_cases_largeur][nbre_cases_hauteur];
    int pause = 0;
    unsigned int compteur;
    unsigned int temps_precedent = 0;

    for(int i = 0 ; i <= nbre_cases_largeur ; i++)
    {
      for(int y = 0 ; y <= nbre_cases_hauteur ; y++)
      {
        etat[i][y] = rand_a_b(0,2);
      }
    }

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    police = TTF_OpenFont("game_over.ttf", 65);
    ecran = SDL_SetVideoMode(largeur, hauteur, 32, SDL_SWSURFACE| SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Simulation simplifiée cellules", NULL);

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    int continuer = 1;
    while(continuer)
    {
      SDL_Event event;
      SDL_PollEvent(&event);
      switch(event.type)
      {
        case SDL_QUIT:
            continuer = 0;
            break;

        case SDL_MOUSEBUTTONDOWN:


            if (event.button.button == SDL_BUTTON_LEFT)
            {
              int x = (event.button.x - event.button.x % taille_case) / taille_case;
              int y = (event.button.y - event.button.y % taille_case) / taille_case;
              etat[x][y] = !etat[x][y];
              refresh_screen(police, taille_case, position, ecran, cases, etat, compteur);



            } else {

            }

            SDL_Delay(100);
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                  continuer = 0;
                  break;

                case SDLK_d:
                  for(int i = 0 ; i <= nbre_cases_largeur ; i++)
                  {
                    for(int y = 0 ; y <= nbre_cases_hauteur ; y++)
                    {
                      if(etat[i][y] == 1)
                        etat[i][y] = rand_a_b(0,2);
                    }
                  }
                  break;

                case SDLK_DELETE:
                  for(int i = 0 ; i <= nbre_cases_largeur ; i++)
                  {
                    for(int y = 0 ; y <= nbre_cases_hauteur ; y++)
                    {
                      etat[i][y] = 0;
                      compteur = 0;
                    }
                  }
                  break;

                case SDLK_r:
                  for(int i = 0 ; i <= nbre_cases_largeur ; i++)
                  {
                    for(int y = 0 ; y <= nbre_cases_hauteur ; y++)
                    {
                      etat[i][y] = rand_a_b(0,2);
                    }
                  }
                  break;

                case SDLK_c:
                  compteur = 0;
                  break;

                case SDLK_p:
                  pause = !pause;
                  break;
            }
            refresh_screen(police, taille_case, position, ecran, cases, etat, compteur);
            SDL_Delay(100);
            break;
      }
      if(pause)
        continue;

      if((SDL_GetTicks() - temps_precedent) > vitesse)
      {
        temps_precedent = SDL_GetTicks();
        for(int i = 0 ; i <= nbre_cases_largeur ; i++)
        {
          for(int y = 0 ; y <= nbre_cases_hauteur ; y++)
          {
            ancien_etat[i][y] = etat[i][y];
          }
        }

        for(int i = 0 ; i <= nbre_cases_largeur ; i++)
        {
          for(int y = 0 ; y <= nbre_cases_hauteur ; y++)
          {

            nbr_voisins_vivants[i][y] = ancien_etat[i+1][y+1] + ancien_etat[i+1][y] + ancien_etat[i+1][y-1] + ancien_etat[i][y+1] + ancien_etat[i][y-1] + ancien_etat[i-1][y+1] + ancien_etat[i-1][y] + ancien_etat[i-1][y-1];

          }
        }

        for(int i = 0 ; i <= nbre_cases_largeur ; i++)
        {
          for(int y = 0 ; y <= nbre_cases_hauteur ; y++)
          {
              if(ancien_etat[i][y] == 0)
                {
                  if(nbr_voisins_vivants[i][y] == 3)
                  {
                      etat[i][y] = 1;
                  }
                }
                if(ancien_etat[i][y] == 1)
                {
                  if((nbr_voisins_vivants[i][y] == 2) || (nbr_voisins_vivants[i][y] == 3))
                  {
                    etat[i][y] = 1;
                  } else {
                    etat[i][y] = 0;
                  }
                }
          }
        }
        compteur++;
        refresh_screen(police, taille_case, position, ecran, cases, etat, compteur);

      } else {
      //  SDL_Delay(vitesse-(SDL_GetTicks() - temps_precedent));
      }
    }
    TTF_CloseFont(police);
    SDL_Quit();
    TTF_Quit();
    return EXIT_SUCCESS;
}


void refresh_screen(TTF_Font *police, int taille_case, SDL_Rect position[nbre_cases_largeur][nbre_cases_hauteur], SDL_Surface* ecran, SDL_Surface* cases[nbre_cases_largeur][nbre_cases_hauteur], int etat[nbre_cases_largeur][nbre_cases_hauteur], unsigned int compteur)
{
  SDL_Rect position_texte ;
  SDL_Surface* texte = NULL;
  char generation[20] = "";
  sprintf(generation, "Generation : %d", compteur);
  SDL_Color couleur_noire = {255, 255, 255};
  texte = TTF_RenderText_Solid(police, generation, couleur_noire);
  position_texte.x = 0;
  position_texte.y = 0;

  for(int i = 0 ; i <= nbre_cases_largeur ; i++)
  {
    for(int y = 0 ; y <= nbre_cases_hauteur ; y++)
    {
      cases[i][y] = SDL_CreateRGBSurface(SDL_SWSURFACE, taille_case, taille_case, 32, 0, 0, 0, 0);
      position[i][y].y = y * taille_case;
      position[i][y].x = i * taille_case;

      if(etat[i][y])
      {
        SDL_FillRect(cases[i][y], NULL, SDL_MapRGB(ecran->format, 0, 190, 0));
      } else {
        SDL_FillRect(cases[i][y], NULL, SDL_MapRGB(ecran->format, 0, 0,0 ));
      }

      SDL_BlitSurface(cases[i][y], NULL, ecran, &position[i][y]);
      SDL_FreeSurface(cases[i][y]);
    }
  }

  SDL_BlitSurface(texte, NULL, ecran, &position_texte);
  SDL_Flip(ecran);
  SDL_FreeSurface(texte);
  SDL_FreeSurface(ecran);
  //TTF_CloseFont(police); /* Doit être avant TTF_Quit() */
}





int rand_a_b(int a, int b)
{

    return (rand()%(b-a) + a);
}
