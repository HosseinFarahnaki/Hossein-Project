#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include <bits/stdc++.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include<SDL2/SDL_mixer.h>
using namespace std;
#define Radius 10
#define main_shooted_ball 25
#define second_shooted_ball 15
#define canon_center_x 640
#define canon_center_y 400
#define max_ball_counter 80
double dx, dy, map_coord[2][10000];
SDL_Event* e = new SDL_Event();
const char* qq[4];
SDL_Window* m_window;

SDL_Renderer* m_renderer;
int canon_w, canon_h, img_w, img_h, ice_w, ice_h, fire_w, fire_h, BG_h, BG_w, bomb_w, bomb_h;
int login_w, login_h,  logo_w, logo_h,  music_w, music_h,   quit_w, quit_h,  settings_w, settings_h,   sound_w, sound_h,   start_w, start_h;
bool map_menu = 0, setting = 0, exite = 0,help;
bool sound = 1, music = 1;
SDL_Rect canon_rect;
SDL_Rect BG_rect;
SDL_Rect ice_rect;
SDL_Rect img_rect;
SDL_Rect pause_rect;
SDL_Rect fire_rect;
SDL_Rect login_rect;
SDL_Rect logo_rect;
SDL_Rect music_rect;
SDL_Rect help_rect;
SDL_Rect settingwood_rect;
SDL_Rect quit_rect;
SDL_Rect setting_rect;
SDL_Rect sound_rect;
SDL_Rect bomb_rect;
SDL_Rect start_rect;
SDL_Rect map1wood_rect;
SDL_Rect map2wood_rect;
SDL_Rect map3wood_rect;
SDL_Rect map4wood_rect;


struct moving_balls
{
    int r, g, b;
    double x, y;
} ball[1000];

struct shooted_ball
{
    int r, g, b;
    double x = canon_center_x, y = canon_center_y;
    bool fired_ball = false, passed_ball = false;
};

SDL_Rect recc = {100, 100, 100, 100};
SDL_Point p = {65,65};
SDL_Point pfire = { 24, 24 };
SDL_Point bfire = { 30, 45 };

SDL_Texture* ice_img = NULL;
SDL_Texture* m_img = NULL;
SDL_Texture* pause_img = NULL;

SDL_Texture* login_img = NULL;
SDL_Texture* bomb_img = NULL;
SDL_Texture* logo_img = NULL;
SDL_Texture* music_img = NULL;
SDL_Texture* quit_img = NULL;
SDL_Texture* setting_img = NULL;
SDL_Texture* sound_img = NULL;
SDL_Texture* start_img = NULL;
SDL_Texture* canon = NULL;
SDL_Texture* BG_img = NULL;
SDL_Texture* help_img = NULL;
SDL_Texture* settingwood_img = NULL;

SDL_Texture* map4wood_img = NULL;
SDL_Texture* map3wood_img = NULL;
SDL_Texture* map2wood_img = NULL;
SDL_Texture* map1wood_img = NULL;
Mix_Chunk* pop;
Mix_Chunk* win;
Mix_Chunk* lose;
Mix_Chunk* fireball;
Mix_Chunk* ignite;
Mix_Chunk* ice;
Mix_Chunk* explosion;

SDL_Texture* fire_img = NULL;

void menu (SDL_Renderer* m_renderer);

void betterText(
        SDL_Renderer *renderer, string S,
        int x, int y,
        int r, int g, int b, int a,
        int size, string Font /*def = arial */);

void textRGBA2(SDL_Renderer*m_renderer, int x, int y, const char* text,int f_size,int r, int g, int b, int a);

void setting_menu(SDL_Renderer* m_renderer, Mix_Music *backgroundsound);

void menu_map ( SDL_Renderer* m_renderer);

void draw(SDL_Renderer* m_renderer, SDL_Texture* m_img, SDL_Rect img_rect, int x, int y, int w, int h);

int check_color_ball (int r, vector <moving_balls> toop, int counter );

void slow_mo_power(int &delay, bool &slow_mo_bool, bool back_move_power, bool ice_power_bool, int &tedad_slow_mo, int &counter_slow_mo);

void ice_power(int &i, bool &ice_power_bool, bool back_move_power, bool slow_mo_bool, int &tedad_icepower, int &counter_ice );

void backward_power(int &i, bool &back_move_power, bool ice_power_bool, bool slow_mo_bool, int &tedad_backwardpower, int &counter_backward);

double ball_angle(double xnxx, double ynyy);

void draw_bg(SDL_Renderer* m_renderer, SDL_Texture* m_img, SDL_Rect img_rect );

void random_color(int &r, int &g, int &b);

void open_map_coordinates(SDL_Renderer* m_renderer, ifstream& coord);

void move_ball( SDL_Renderer* m_renderer, SDL_Texture* m_img, SDL_Texture* canon, SDL_Rect img_rect, vector <moving_balls> toop);

void draw_curve(SDL_Renderer* m_renderer);

double dist(double x1, double y1, double x2, double y2);

void get_mouse_angle(int &x, int &y, SDL_Event* e);

void remove_ball(int k, vector <moving_balls> &toop, int counter, int r, int g, int b, int &popping_ball_num, bool new_shooted_ball_bool, int &removed_ball_counter);

int checkpress( int x_min, int x_max, int y_min, int y_max);

int xnxx, ynyy ;

vector <moving_balls> toop;


Mix_Music *backgroundsound = NULL;
Mix_Music *backgroundsound2 = NULL;

Mix_Music *shot_sound = NULL;

int ahang = 0;
bool next_ahang = 1;
int main(int argc, char* argv[])
{

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
    backgroundsound = Mix_LoadMUS("main music.mp3");
    backgroundsound2 = Mix_LoadMUS("main music2.mp3");
    ice = Mix_LoadWAV("ice.wav");
    pop = Mix_LoadWAV("pop.wav");
    win = Mix_LoadWAV("win.wav");
    lose = Mix_LoadWAV("lose.wav");
    fireball = Mix_LoadWAV("fireball.wav");
    ignite = Mix_LoadWAV("ignite.wav");
    explosion = Mix_LoadWAV("explosion.wav");

    //SDL_Delay(10000);
    //Mix_FreeMusic(backgroundsound);
    Mix_PlayMusic(backgroundsound, -1);

    srand(time(NULL));
    //Initialization of SDL windows
    Uint32 SDL_flags = SDL_INIT_VIDEO | SDL_INIT_TIMER;
    Uint32 WND_flags = SDL_WINDOW_SHOWN;//SDL_WINDOW_BORDERLESS ;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    //Texture for loading image

    SDL_Init(SDL_flags);
    SDL_CreateWindowAndRenderer(1280, 800, WND_flags, &m_window, &m_renderer);
    //Pass the focus to the drawing window
    SDL_RaiseWindow(m_window);
    //Get screen resolution
    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int W = DM.w;
    int H = DM.h;
    // Clear the window with a black background
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // Show the window
    SDL_RenderPresent(m_renderer);
    BG_img = IMG_LoadTexture(m_renderer, "BG.jpg");

    map4wood_img = IMG_LoadTexture(m_renderer, "map4wood.png");
    map3wood_img = IMG_LoadTexture(m_renderer, "map3wood.png");
    map2wood_img = IMG_LoadTexture(m_renderer, "map2wood.png");
    map1wood_img = IMG_LoadTexture(m_renderer, "map1wood.png");
    bomb_img = IMG_LoadTexture(m_renderer, "bomb.png");
    pause_img = IMG_LoadTexture(m_renderer, "pause.png");
    SDL_Texture* m_img = NULL;

    settingwood_img = IMG_LoadTexture(m_renderer, "settingwood.png");

    help_img = IMG_LoadTexture(m_renderer, "help.png");

    login_img = IMG_LoadTexture(m_renderer, "login.png");

    logo_img = IMG_LoadTexture(m_renderer, "logo.png");

    music_img = IMG_LoadTexture(m_renderer, "music.png");

    quit_img = IMG_LoadTexture(m_renderer, "quit.png");

    setting_img = IMG_LoadTexture(m_renderer, "setting.png");

    sound_img = IMG_LoadTexture(m_renderer, "sound.png");

    start_img = IMG_LoadTexture(m_renderer, "start.png");

    BG_img = IMG_LoadTexture(m_renderer, "BG.jpg");

    canon = IMG_LoadTexture(m_renderer, "canon.png");

    SDL_QueryTexture(canon, NULL, NULL, &canon_w, &canon_h);
    canon_rect.x = 640 - 65;
    canon_rect.y = 400 - 65;
    canon_rect.w = 130;
    canon_rect.h = 130;
    ice_img = IMG_LoadTexture(m_renderer, "ice_pic.png");
    SDL_QueryTexture(ice_img, NULL, NULL, &ice_w, &ice_h);

    fire_img = IMG_LoadTexture(m_renderer, "fire_pic.png");
    SDL_QueryTexture(fire_img, NULL, NULL, &fire_w, &fire_h);

    //Wait for a key to be pressed
    //Loading image


    //Get the width and height of the texture
    SDL_QueryTexture(m_img, NULL, NULL, &img_w, &img_h);
    img_rect.x = 0;
    img_rect.y = 0;
    img_rect.w = 1280;
    img_rect.h = 800;

    ifstream coord("map 1.txt", ios::in);
    open_map_coordinates(m_renderer, coord); //keshidane masir

    for(int i = 0; i < 1000; i++)
    {
        int r, g, b;
        random_color(r, g, b);
        ball[i].r = r;
        ball[i].g = g;
        ball[i].b = b;
        ball[i].x = map_coord[0][0];
        ball[i].y = map_coord[1][0];
        toop.push_back(ball[i]);
    }

    // copy the texture to the rendering context
//    SDL_RenderCopy(m_renderer, m_img, NULL, &img_rect);

    //SDL_RenderPresent(m_renderer);

    // move_ball(m_renderer, m_img,canon , img_rect, toop);
    e->type = 0;
    SDL_PollEvent(e);

    bool menu_bool=true;
    bool map_bool=false;


    while (true)
    {
        if( e -> type == SDL_QUIT)
        {
            SDL_DestroyTexture(m_img);
            SDL_DestroyWindow(m_window);
            SDL_DestroyRenderer(m_renderer);
            IMG_Quit();
            SDL_Quit();
        }

        SDL_PollEvent(e);
        SDL_ShowCursor(SDL_ENABLE);
        xnxx = e->button.x;
        ynyy = e->button.y;
        menu(m_renderer);

        map_menu = 0;
        setting = 0;

        if((xnxx>70)&&(xnxx<235)&&(ynyy>580)&&(ynyy<800) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
        {
            e ->type = 0 ;
            exite = 1;
        }

        if((xnxx>540)&&(xnxx<740)&&(ynyy>400)&&(ynyy<505) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
        {
            map_menu = 1;
            draw(m_renderer, BG_img, BG_rect, 0, 0, 1280, 800);
            draw(m_renderer, map1wood_img, map1wood_rect, 300, 150, 250, 200);
            draw(m_renderer, map2wood_img, map2wood_rect, 730, 150, 250, 200);
            draw(m_renderer, map3wood_img, map3wood_rect, 300, 450, 250, 200);
            draw(m_renderer, map4wood_img, map4wood_rect, 730, 450, 250, 200);
            SDL_RenderPresent(m_renderer);
        }

        if((xnxx>1045)&&(xnxx<1210)&&(ynyy>580)&&(ynyy<800) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
        {
            setting = 1;
            menu(m_renderer);
            draw(m_renderer, settingwood_img, settingwood_rect, 1045, 450, 150, 150);
            draw(m_renderer, settingwood_img, settingwood_rect, 900, 450, 150, 150);
            draw(m_renderer, settingwood_img, settingwood_rect, 900, 570, 150, 165);
            draw(m_renderer, music_img, music_rect, 1085, 490, 70, 70);
            draw(m_renderer, sound_img, sound_rect, 945, 490, 70, 70);
            draw(m_renderer, help_img, help_rect, 935, 610, 85, 80);
            SDL_RenderPresent(m_renderer);
        }
        while(map_menu)
        {
            menu_map(m_renderer);
            exite = 0;
        }

        while(setting)
        {
            setting_menu(m_renderer, backgroundsound);
        }

        if(exite == 1)
            break;
////               move_ball(m_renderer, m_img,canon , img_rect, toop);

        e->type = 0;
    }

    SDL_DestroyTexture(m_img);
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    IMG_Quit();
    SDL_Quit();
    return 0;

}
void random_color(int &r, int &g, int &b)
{

    int rc = rand() % 4;
    if (rc == 0)
    {
        //soolati
        r = 255;
        g = 59;
        b = 139;
    }
    else if (rc == 1)
    {
        //cyan
        r = 30;
        g = 240;
        b = 206;
    }
    else if (rc == 2)
    {
        //abi_oili
        r = 0;
        g = 4;
        b = 122;
    }
    else if (rc == 3)
    {
        //puprle
        r = 172;
        g = 72;
        b = 252;
    }
}

void open_map_coordinates(SDL_Renderer* m_renderer, ifstream& coord)
{

    for (int i = 0; i < 10000; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            coord >> map_coord[j][i];
        }
    }
    coord.close();
}

double ball_angle(double xnxx, double ynyy)
{

    double tetha = atan((ynyy - 400) / (xnxx - 640));
    if ((ynyy - 400 > 0) && (xnxx - 640 > 0))
    {
        tetha = (3.1415 / 2) - tetha;
    }
    if ((ynyy - 400 < 0) && (xnxx - 640 > 0))
    {
        tetha = (3.1415 / 2) - tetha;
    }
    if ((ynyy - 400 < 0) && (xnxx - 640 < 0))
    {
        tetha = (3 * 3.1415 / 2) - tetha;
    }
    if ((ynyy - 400 > 0) && (xnxx - 640 < 0))
    {
        tetha = (3 * 3.1415 / 2) - tetha;
    }
    return tetha;
}

void move_ball( SDL_Renderer* m_renderer, SDL_Texture* m_img, SDL_Texture* canon, SDL_Rect img_rect, vector <moving_balls> toop)
{

    int counter_ice = 0, counter_backward = 0, counter_slow_mo = 0;
    int tedad_icepower = 3, tedad_backwardpower = 3, tedad_slow_mo = 3, tedad_fire_ball = 3, tedad_bomb = 1;
    srand(time(NULL));
    int score = 0 ;
    bool fire_ball_bool = false ;
    bool pause_bool = false;
    bool bomb_bool = false ;
    bool back_move_power = false ;
    bool ice_power_bool = false ;
    bool slow_mo_bool = false;
    int delay = 20 ;
    int fireballangle = 0;
    int bombballangle = 0 ;
    int counter = 0, transparent = 255;    //tedad tup - 1
    int xxxx, yyyy, popping_ball_num = 0 ;
    double mouse_angle, shooting_angle, dx, dy;
    bool shooted_ball_bool = false, game_finish = false ;
    shooted_ball shooted_ball;
    int r, g, b, rk, gk, bk ;
    random_color(r, g, b);
    random_color(rk, gk, bk);
    shooted_ball.r = r;
    shooted_ball.g = g;
    shooted_ball.b = b;
    bool eighty_counter = true;
    for(int i = 0 ; i < 10000 ; i++)
    {
        if(ahang %2 == 0 && next_ahang == 0)
        {
            next_ahang = 1;
            Mix_PlayMusic(backgroundsound,-1);
        }

        else if(ahang %2 == 1&& next_ahang == 0)
        {
            next_ahang = 1;
            Mix_PlayMusic(backgroundsound2,-1);
        }

        if(map_menu == 0)
            break;

        SDL_PollEvent(e);
        SDL_ShowCursor(SDL_ENABLE);
        xnxx = e->button.x;
        ynyy = e->button.y;
        if( e -> type == SDL_QUIT)
        {
            SDL_DestroyTexture(m_img);
            SDL_DestroyWindow(m_window);
            SDL_DestroyRenderer(m_renderer);
            IMG_Quit();
            SDL_Quit();
        }
        slow_mo_power(delay, slow_mo_bool, back_move_power, ice_power_bool, tedad_slow_mo, counter_slow_mo);
        ice_power( i, ice_power_bool, back_move_power, slow_mo_bool, tedad_icepower, counter_ice ) ;
        if(tedad_fire_ball > 0 && (e->key.keysym.sym == SDLK_f && e ->type == SDL_KEYDOWN))
        {
            Mix_VolumeChunk(fireball, 128);
            Mix_PlayChannel(-1, fireball, -1);
            shooted_ball.r = 250;
            shooted_ball.g = 150;
            shooted_ball.b = 0;
            tedad_fire_ball--;
            fire_ball_bool = true;
        }
        if(tedad_bomb > 0)
        {
            if((e->key.keysym.sym == SDLK_e && e ->type == SDL_KEYDOWN) )
            {
                Mix_VolumeChunk(ignite, 128);
                Mix_PlayChannel(-1, ignite, -1);
                shooted_ball.r = 1;
                shooted_ball.g = 0;
                shooted_ball.b = 0;
                tedad_bomb--;
                bomb_bool = true;
            }
        }

        backward_power( i, back_move_power,   ice_power_bool, slow_mo_bool, tedad_backwardpower, counter_backward) ;
        draw_bg(m_renderer, m_img, img_rect );
        SDL_PollEvent(e);
        mouse_angle = 180 - ( ball_angle ( xnxx, ynyy ) * 180 / 3.1415 );
        if (shooted_ball_bool == false )
        {
            shooted_ball.x = 640 + 50 * sin ( 3.1415 - ( mouse_angle * 3.1415 / 180 ) );
            shooted_ball.y = 400 +  50 * cos ( 3.1415 - ( mouse_angle * 3.1415 / 180 ) );
        }
        xxxx = 640 - 27 * sin ( 3.1415 - ( mouse_angle * 3.1415 / 180 ) );
        yyyy = 400 -  27 * cos ( 3.1415 - ( mouse_angle * 3.1415 / 180 ) );
        if(shooted_ball.x < 0 || shooted_ball.y < 0 || shooted_ball.x > 1280 || shooted_ball.y > 800)
        {
            Mix_VolumeChunk(fireball, 0);
            Mix_VolumeChunk(ignite, 0);
            bomb_bool = false;
            fire_ball_bool = false;
            dx = 0;
            dy = 0;
            shooted_ball.x = canon_center_x;
            shooted_ball.y = canon_center_y;
            shooted_ball_bool = false;
            //  random_color(r, g, b);
            shooted_ball.r = rk;
            shooted_ball.g = gk;
            shooted_ball.b = bk;
            srand(time(NULL));
            while (true)
            {
                random_color(rk, gk, bk);
                if ( check_color_ball(rk,toop,counter) == 1 )
                {
                    break;
                }
            }

        }
        if(counter >= max_ball_counter - 1)
        {
            eighty_counter = false;
            //tedad_fire_ball = 3;
        }
        int removed_ball_counter = 0;
        if(popping_ball_num != 0)
        {
            remove_ball(popping_ball_num, toop, counter, toop[popping_ball_num].r, toop[popping_ball_num].g, toop[popping_ball_num].b, popping_ball_num, false, removed_ball_counter);
            if(removed_ball_counter == 0)
                remove_ball(popping_ball_num + 1, toop, counter, toop[popping_ball_num].r, toop[popping_ball_num].g, toop[popping_ball_num].b, popping_ball_num, false, removed_ball_counter);
            score+=2*removed_ball_counter;
        }
        for( int k = 0 ; k < counter + 1 ; k++ )
        {
            if ( dist( shooted_ball.x, shooted_ball.y, toop[k].x, toop[k].y ) < 34 )
            {
                Mix_VolumeChunk(fireball, 0);
                Mix_VolumeChunk(ignite, 0);
                dx = 0;
                dy = 0;
                shooted_ball.x = canon_center_x;
                shooted_ball.y = canon_center_y;
                shooted_ball_bool = false;
                int same_color_counter = 0;
                Mix_PlayChannel(-1, pop, 0);
                if(shooted_ball.r == 250)
                {
                    toop.erase(toop.begin() + k, toop.begin() + k + 1);
                    popping_ball_num = k - 1;
                    removed_ball_counter = 1;
                    shooted_ball.r = rk ;
                    shooted_ball.g = gk;
                    shooted_ball.b = bk ;
                    fire_ball_bool = false;
                }
                else if(shooted_ball.r == 1)
                {
                    Mix_PlayChannel(-1, explosion, 0);
                    int first_removing_ball;
                    int last_removing_ball;
                    if(k - 4 < 0 && k + 5 > counter)
                    {
                        first_removing_ball = 0;
                        last_removing_ball = counter;
                    }
                    else if(k - 4 < 0 && k + 5 <= counter)
                    {
                        first_removing_ball = 0;
                        last_removing_ball = k + 5;
                    }
                    else if(k - 4 >= 0 && k + 5 > counter)
                    {
                        first_removing_ball = k - 4;
                        last_removing_ball = counter;
                    }
                    else if(k - 4 >= 0 && k + 5 <= counter)
                    {
                        first_removing_ball = k - 4;
                        last_removing_ball = k + 5;
                    }
                    toop.erase(toop.begin() + first_removing_ball, toop.begin() + last_removing_ball );
                    popping_ball_num = last_removing_ball ;
                    removed_ball_counter = 9;
                    shooted_ball.r = rk ;
                    shooted_ball.g = gk;
                    shooted_ball.b = bk ;
                    bomb_bool = false;
                }
                else
                {
                    if(dist(shooted_ball.x, shooted_ball.y, toop[k + 1].x, toop[k + 1].y) < dist(shooted_ball.x, shooted_ball.y, toop[k - 1].x, toop[k - 1].y))
                    {
                        moving_balls new_ball;
                        new_ball.r = shooted_ball.r;
                        new_ball.g = shooted_ball.g;
                        new_ball.b = shooted_ball.b;
                        new_ball.x = toop[k].x;
                        new_ball.y = toop[k].y;
                        for(int j = 0; j < k; j++)
                        {
                            toop[j].x = map_coord[0][i + 34 - j * 34];
                            toop[j].y = map_coord[1][i + 34 - j * 34];
                        }
                        toop.insert(toop.begin() + k, new_ball);
                        if(counter < max_ball_counter)
                            counter ++;
                        remove_ball(k, toop, counter, shooted_ball.r, shooted_ball.g, shooted_ball.b, popping_ball_num, true, removed_ball_counter);
                        score+=removed_ball_counter;
                        shooted_ball.r = rk ;
                        shooted_ball.g = gk;
                        shooted_ball.b = bk ;
                        while (true)
                        {
                            random_color(rk, gk, bk);
                            if ( check_color_ball(rk,toop,counter) == 1 )
                            {
                                break;
                            }
                        }
                    }
                    else
                    {
                        moving_balls new_ball;
                        new_ball.r = shooted_ball.r;
                        new_ball.g = shooted_ball.g;
                        new_ball.b = shooted_ball.b;
                        new_ball.x = toop[k].x;
                        new_ball.y = toop[k].y;
                        for(int j = 0; j < k; j++)
                        {
                            toop[j].x = map_coord[0][i + 34 - j * 34];
                            toop[j].y = map_coord[1][i + 34 - j * 34];
                        }
                        toop.insert(toop.begin() + k, new_ball);
                        if(counter < max_ball_counter)
                            counter ++;
                        remove_ball(k, toop, counter, shooted_ball.r, shooted_ball.g, shooted_ball.b, popping_ball_num, true, removed_ball_counter);
                        score+=removed_ball_counter;
                        shooted_ball.r = rk ;
                        shooted_ball.g = gk;
                        shooted_ball.b = bk ;
                        while (true)
                        {
                            random_color(rk, gk, bk);
                            if ( check_color_ball(rk,toop,counter) == 1 )
                            {
                                break;
                            }
                        }
                    }


                    break;
                }

            }
        }
        // cout << toop.size() << endl;
        check_color_ball(255,toop,counter);
        if(eighty_counter == false)
        {
            counter -= removed_ball_counter;
        }

        if ((xnxx>1200)&&(xnxx<1280)&&(ynyy>0)&&(ynyy<80)&&(e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
        {
            e->type = 0 ;
            pause_bool = true;
        }
        if(pause_bool == true )
        {
            i--;
            SDL_Rect play_rect;
            SDL_Texture* play_img = NULL;
            play_img = IMG_LoadTexture(m_renderer, "play.png");
            draw(m_renderer,settingwood_img,setting_rect,140,100,1000,600);
            draw(m_renderer, play_img,play_rect,610,350,100,100);
            draw(m_renderer, music_img, music_rect, 880, 350, 100, 100);
            draw(m_renderer, sound_img, sound_rect, 340, 350, 100, 100);
            draw(m_renderer, quit_img, quit_rect, 70, 580, 165, 220);

            SDL_RenderPresent(m_renderer);
            while(1)
            {
                SDL_PollEvent(e);
                SDL_ShowCursor(SDL_ENABLE);
                xnxx = e->button.x;
                ynyy = e->button.y;
                if ((xnxx>610)&&(xnxx<710)&&(ynyy>350)&&(ynyy<450) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN) &&(pause_bool==true))
                {
                    e->type = 0 ;
                    SDL_SetRenderDrawColor(m_renderer,0,0,0,255);
                    SDL_RenderClear(m_renderer);
                    pause_bool = false ;
                    break;
                }

                if((xnxx>70)&&(xnxx<235)&&(ynyy>580)&&(ynyy<800) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
                {
                    e ->type = 0 ;
                    exite = 1;
                    pause_bool = 0;
                    map_menu = 0;
                    break;
                }
                if ((xnxx>880)&&(xnxx<980)&&(ynyy>350)&&(ynyy<450) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN) &&(music == true))
                {
                    e->type = 0 ;
                    Mix_VolumeMusic(0);
                    music = false ;
                }

                if ((xnxx>880)&&(xnxx<980)&&(ynyy>350)&&(ynyy<450) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN)&&(music == false ))
                {
                    e->type = 0 ;
                    Mix_VolumeMusic(128);
                    music = true ;
                }
                if ((xnxx>340)&&(xnxx<440)&&(ynyy>350)&&(ynyy<450) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN) &&(sound == true))
                {
                    e->type = 0 ;
                    Mix_VolumeChunk(ice,0);
                    Mix_VolumeChunk(pop,0);
                    Mix_VolumeChunk(explosion,0);
                    Mix_VolumeChunk(fireball,0);
                    Mix_VolumeChunk(win,0);
                    Mix_VolumeChunk(lose,0);
                    Mix_VolumeChunk(ignite,0);
                    sound = false ;
                }
                if ((xnxx>340)&&(xnxx<440)&&(ynyy>350)&&(ynyy<450) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN)&&(sound == false ))
                {
                    e->type = 0 ;
                    Mix_VolumeChunk(ice,128);
                    Mix_VolumeChunk(pop,128);
                    Mix_VolumeChunk(explosion,128);
                    Mix_VolumeChunk(fireball,128);
                    Mix_VolumeChunk(win,128);
                    Mix_VolumeChunk(lose,128);
                    Mix_VolumeChunk(ignite,128);
                    sound = true ;
                }
                if(e->key.keysym.sym == SDLK_n && e ->type == SDL_KEYDOWN)
                {
                    e->type = 0;
                    ahang++;
                    ahang %= 2;
                    next_ahang = 0;
                }
                if(ahang %2 == 0 && next_ahang == 0)
                {
                    next_ahang = 1;
                    Mix_PlayMusic(backgroundsound,-1);
                }

                if(ahang %2 == 1 && next_ahang == 0)
                {
                    next_ahang = 1;
                    Mix_PlayMusic(backgroundsound2,-1);
                }
                e->type = 0;
            }


        }

        if ((e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN) && ( shooted_ball_bool == false ) && (game_finish == false) &&(pause_bool==false) )
        {
            // checkpress(xnxx,ynyy,0,100,0,100);
            e->type = 0 ;
            shooting_angle = ball_angle(xnxx,ynyy);
            dx = 8 * sin(shooting_angle);
            dy = 8 * cos(shooting_angle);
            shooted_ball_bool = true ;
        }
        if ((e->button.button == SDL_BUTTON_RIGHT && e->type == SDL_MOUSEBUTTONDOWN) && ( shooted_ball_bool == false ))
        {
            e->type = 0 ;
            int rr,gg,bb;
            rr=rk;
            gg=gk;
            bb=bk;
            rk = shooted_ball.r ;
            gk = shooted_ball.g ;
            bk = shooted_ball.b ;
            shooted_ball.r = rr ;
            shooted_ball.g = gg ;
            shooted_ball.b = bb ;
        }

        shooted_ball.x += dx;
        shooted_ball.y += dy;
        filledCircleRGBA(m_renderer, shooted_ball.x, shooted_ball.y, 17, shooted_ball.r, shooted_ball.g, shooted_ball.b, 255);
        filledCircleRGBA(m_renderer, xxxx, yyyy, 20, rk, gk, bk, 255);
        if(toop[counter].x == map_coord[0][34] && counter < max_ball_counter - 1)
        {
            counter ++;
        }
        for(int j = 0; j < counter + 1; j++)
        {
            if((i - 34 * j)>=0)
            {
                filledCircleRGBA(m_renderer, map_coord[0][i - 34 * j], map_coord[1][i - 34 * j], 17, toop[j].r, toop[j].g, toop[j].b, transparent );
                toop[j].x = map_coord[0][i - 34 * j];
                toop[j].y = map_coord[1][i - 34 * j];
            }
        }
        if(toop[0].x == map_coord[0][3499])
        {
            delay = 1 ;
            game_finish=true;
        }
        if(toop[counter].x == map_coord[0][3499])
        {
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
            SDL_RenderClear(m_renderer);
            SDL_Rect img_rectt;
            SDL_Texture* m_imgg = NULL;
            m_imgg = IMG_LoadTexture(m_renderer, "game_over_pic.jpg");
            SDL_QueryTexture(m_imgg, NULL, NULL, &img_w, &img_h);
            img_rectt.x = 0;
            img_rectt.y = 0;
            img_rectt.w = 1280;
            img_rectt.h = 800;
            SDL_RenderCopy(m_renderer, m_imgg, NULL, &img_rectt);
            SDL_RenderPresent(m_renderer);
            break;
        }
        if(counter <= -1)
        {
            SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
            SDL_RenderClear(m_renderer);
            SDL_Rect win_rect;
            SDL_Texture* win_img = NULL;
            win_img = IMG_LoadTexture(m_renderer, "win.jpg");
            SDL_QueryTexture(win_img, NULL, NULL, &img_w, &img_h);
            win_rect.x = 0;
            win_rect.y = 0;
            win_rect.w = 1280;
            win_rect.h = 800;
            SDL_RenderCopy(m_renderer, win_img, NULL, &win_rect);
            textRGBA2(m_renderer,10,10,"score :",40,255,255,2,255);
            textRGBA2(m_renderer,150,10,to_string(score).c_str(),40,255,255,2,255);
            SDL_RenderPresent(m_renderer);
            break;
        }

        if (ice_power_bool == true)
        {
            for (int s = 0; s <= counter; s++)
            {
                draw(m_renderer, ice_img, ice_rect, toop[s].x - 24, toop[s].y - 24, 48, 48);
            }
        }
        if (fire_ball_bool == true)
        {
            fireballangle = fireballangle + 5;
            fire_rect.x = shooted_ball.x - 24;
            fire_rect.y = shooted_ball.y - 24;
            fire_rect.w = 48;
            fire_rect.h = 48;
            SDL_RenderCopyEx(m_renderer, fire_img, NULL, &fire_rect, fireballangle, &pfire, SDL_FLIP_NONE);
        }
        SDL_RenderCopyEx(m_renderer, canon, NULL, &canon_rect, mouse_angle, &p, SDL_FLIP_NONE);
        if (bomb_bool == true)
        {
            bombballangle = bombballangle + 5;
            bomb_rect.x = shooted_ball.x - 30;
            bomb_rect.y = shooted_ball.y - 45;
            bomb_rect.w = 70;
            bomb_rect.h = 70;
            SDL_RenderCopyEx(m_renderer, bomb_img, NULL, &bomb_rect, bombballangle, &bfire, SDL_FLIP_NONE);
        }

        if ((e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN) &&(pause_bool==true))
        {
            e->type = 0 ;
            if((xnxx>610)&&(xnxx<710)&&(ynyy>350)&&(ynyy<450))
            {
                cout<<"erbe";
                SDL_RenderClear(m_renderer);
                pause_bool = false ;

            }
        }
        textRGBA2(m_renderer,10,10,"score :",40,255,255,2,255);
        textRGBA2(m_renderer,150,10,to_string(score).c_str(),40,255,255,2,255);
        draw(m_renderer,pause_img,pause_rect,1200,0,80,80);
        SDL_RenderPresent(m_renderer);
        SDL_Delay(delay);
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
        SDL_RenderClear(m_renderer);
    }

}

void draw_bg(SDL_Renderer* m_renderer, SDL_Texture* m_img, SDL_Rect img_rect)
{
    //Get the width and height of the texture
    //SDL_QueryTexture(m_img, NULL, NULL, &img_w, &img_h);
    img_rect.x = 0;
    img_rect.y = 0;
    img_rect.w = 1280;
    img_rect.h = 800;
    // copy the texture to the rendering context
    SDL_RenderCopy(m_renderer, m_img, NULL, &img_rect);
}

void draw(SDL_Renderer* m_renderer, SDL_Texture* m_img, SDL_Rect img_rect, int x, int y, int w, int h)
{
    //Get the width and height of the texture
    //SDL_QueryTexture(m_img, NULL, NULL, &img_w, &img_h);
    img_rect.x = x;
    img_rect.y = y;
    img_rect.w = w;
    img_rect.h = h;
    // copy the texture to the rendering context
    SDL_RenderCopy(m_renderer, m_img, NULL, &img_rect);
}

void draw_curve(SDL_Renderer* m_renderer)
{
    for(int i = 0; i < 3500; i++)
    {
        filledCircleRGBA(m_renderer, map_coord[0][i], map_coord[1][i], 20, 255, 255, 0, 255);
    }
}

double dist(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void get_mouse_angle(int &x, int &y, SDL_Event* e)
{
    SDL_ShowCursor(SDL_ENABLE);
    x = e->button.x;
    y = e->button.y;
}

void remove_ball(int k, vector <moving_balls> &toop, int counter, int r, int g, int b, int &popping_ball_num, bool new_shooted_ball_bool, int &removed_ball_counter)
{
    if(new_shooted_ball_bool == true)
    {
        int same_color_low,same_color_high;
        int same_color_counter = 0;
        for(int j = k - 1; j >= 0; j--)
        {
            if(r == toop[j].r)
            {
                same_color_counter++;
            }
            else
                break;
        }
        same_color_high = k - same_color_counter ;
        same_color_counter = 0 ;
        for(int j = k + 1; j < counter + 1; j++)
        {
            if(r == toop[j].r)
            {
                same_color_counter++;
            }
            else
                break;
        }
        same_color_low = k + same_color_counter ;
        same_color_counter = 0 ;
        //cout<<same_color_low<<"      "<<same_color_high<<endl;
        if ( same_color_low - same_color_high >= 2)
        {
            toop.erase( toop.begin() + same_color_high, toop.begin() + same_color_low + 1);
            removed_ball_counter = same_color_low - same_color_high + 1;
        }
        popping_ball_num = same_color_high ;
    }
    else
    {
        int same_color_low,same_color_high;
        int same_color_counter = 0;
        for(int j = k - 1; j >= 0; j--)
        {
            if(r == toop[j].r)
            {
                same_color_counter++;
            }
            else
                break;
        }
        same_color_high = k - same_color_counter ;
        same_color_counter = 0 ;
        for(int j = k + 1; j < counter + 1; j++)
        {
            if(r == toop[j].r)
            {
                same_color_counter++;
            }
            else
                break;
        }
        same_color_low = k + same_color_counter ;
        same_color_counter = 0 ;
        //cout<<same_color_low<<"      "<<same_color_high<<endl;
        if ( same_color_low - same_color_high >= 2 && r == toop[k + 1].r)
        {
            toop.erase( toop.begin() + same_color_high, toop.begin() + same_color_low + 1);
            removed_ball_counter = same_color_low - same_color_high + 1;
        }
        popping_ball_num = same_color_high ;
    }
}
int check_color_ball (int r, vector <moving_balls> toop, int counter )
{
    int x = 0 ;
    for ( int i = 0 ; i<=counter ; i++)
    {
        if(r!=toop[i].r)
        {
            x++;
        }
    }
    if(x==counter+1)
    {
        return 0 ;
    }
    else
    {
        return 1 ;
    }
}

void ice_power(int &i, bool &ice_power_bool, bool back_move_power, bool slow_mo_bool, int &tedad_icepower, int &counter_ice )
{

    if(back_move_power == false && slow_mo_bool == false)
    {
        if(tedad_icepower > 0 )
        {
            if((e->key.keysym.sym == SDLK_i && e ->type == SDL_KEYDOWN) && ( ice_power_bool == false ))
            {
                e ->type = 0 ;
                ice_power_bool = true ;
            }
            if((e->key.keysym.sym == SDLK_i && e ->type == SDL_KEYDOWN) && ( ice_power_bool == true ))
            {
                e ->type = 0 ;
                ice_power_bool = false ;
            }
            if(( ice_power_bool == true ) &&( counter_ice < 100))
            {
                i = i - 1 ;
                counter_ice ++;
                Mix_VolumeChunk(ice, 128);
                Mix_PlayChannel(-1, ice, 0);
            }
        }
        if ( counter_ice >= 100 )
        {
            tedad_icepower --;
            counter_ice = 0 ;
            ice_power_bool = false ;
            Mix_VolumeChunk(ice, 0);
        }
    }

}

void backward_power(int &i, bool &back_move_power, bool ice_power_bool, bool slow_mo_bool, int &tedad_backwardpower, int &counter_backward)
{

    if( ice_power_bool == false && slow_mo_bool == false)
    {
        if(tedad_backwardpower > 0)
        {
            if((e->key.keysym.sym == SDLK_b && e ->type == SDL_KEYDOWN) && ( back_move_power == false ))
            {
                e ->type = 0 ;
                back_move_power = true ;
            }
            if((e->key.keysym.sym == SDLK_b && e ->type == SDL_KEYDOWN) && ( back_move_power == true ))
            {
                e ->type = 0 ;
                back_move_power = false ;
            }
            if( back_move_power == true &&( counter_backward < 100))
            {
                i = i - 2 ;
                counter_backward ++;
            }
        }
        if ( counter_backward >= 100 )
        {
            tedad_backwardpower --;
            counter_backward = 0 ;
            back_move_power = false ;

        }
    }
}

void slow_mo_power(int &delay, bool &slow_mo_bool, bool back_move_power, bool ice_power_bool, int &tedad_slow_mo, int &counter_slow_mo)
{
    if(back_move_power == false && ice_power_bool == false)
    {
        if(tedad_slow_mo > 0)
        {
            if((e->key.keysym.sym == SDLK_s && e ->type == SDL_KEYDOWN) && (slow_mo_bool == false ))
            {
                e ->type = 0 ;
                slow_mo_bool = true ;
            }
            if((e->key.keysym.sym == SDLK_s && e ->type == SDL_KEYDOWN) && ( slow_mo_bool == true ))
            {
                e ->type = 0 ;
                slow_mo_bool = false ;
            }
            if( slow_mo_bool == true &&( counter_slow_mo < 100))
            {
                delay = 40;
                counter_slow_mo ++;
            }
        }
        if ( counter_slow_mo >= 100 )
        {
            tedad_slow_mo --;
            counter_slow_mo = 0 ;
            slow_mo_bool = false ;
            delay = 20 ;
        }
    }
}

void textRGBA2(SDL_Renderer*m_renderer, int x, int y, const char* text,int f_size,int r, int g, int b, int a)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("arial.ttf", f_size);
    int textWidth, textHeight;
    TTF_SizeText(font, text, &textWidth, &textHeight);

    SDL_Rect rectText{x, y, 0, 0};
    Uint8 rr = r;    Uint8 gg = g;    Uint8 bb = b;    Uint8 aa = a;
    SDL_Color color{rr,gg,bb,aa};

    SDL_Surface *textSur = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *textTex = SDL_CreateTextureFromSurface(m_renderer, textSur);
    SDL_FreeSurface(textSur);
    SDL_QueryTexture(textTex, nullptr, nullptr, &rectText.w, &rectText.h);
    SDL_RenderCopy(m_renderer, textTex, nullptr, &rectText);
    SDL_DestroyTexture(textTex);
    TTF_CloseFont(font);
}

void menu ( SDL_Renderer* m_renderer )
{
    SDL_RenderClear(m_renderer);
    draw(m_renderer, BG_img, BG_rect, 0, 0, 1280, 800);
    draw(m_renderer, logo_img, logo_rect, 415, 150, 450, 250 );
    draw(m_renderer, start_img, start_rect, 540, 400, 200, 105);
    draw(m_renderer, login_img, login_rect, 560, 510, 160, 45);
    draw(m_renderer, quit_img, quit_rect, 70, 580, 165, 220);
    draw(m_renderer, setting_img, setting_rect, 1045, 580, 165, 220 );

    SDL_RenderPresent(m_renderer);
}


void menu_map (SDL_Renderer* m_renderer)
{
    SDL_PollEvent(e);
    SDL_ShowCursor(SDL_ENABLE);
    xnxx = e->button.x;
    ynyy = e->button.y;
    if((e->key.keysym.sym == SDLK_ESCAPE && e ->type == SDL_KEYDOWN) )
    {
        e ->type = 0 ;
        map_menu = 0;
    }
    if ((xnxx>300)&&(xnxx<550)&&(ynyy>150)&& (ynyy<350) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
    {
        e->type = 0 ;
        m_img = IMG_LoadTexture(m_renderer, "back map 1.png");
        ifstream coord("map 1.txt", ios::in);
        open_map_coordinates(m_renderer, coord);
        move_ball(m_renderer, m_img,canon, img_rect, toop);
    }
    if ((xnxx>730)&&(xnxx<980)&&(ynyy>150)&&(ynyy<350) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
    {
        e->type = 0 ;
        m_img = IMG_LoadTexture(m_renderer, "back map 2.png");
        ifstream coord("map 2.txt", ios::in);
        open_map_coordinates(m_renderer, coord);
        move_ball(m_renderer, m_img,canon, img_rect, toop);
    }
    if ((xnxx>300)&&(xnxx<550)&&(ynyy>450)&&(ynyy<650) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
    {
        e->type = 0 ;

        m_img = IMG_LoadTexture(m_renderer, "back map 3.png");
        ifstream coord("map 3.txt", ios::in);
        open_map_coordinates(m_renderer, coord);
        move_ball(m_renderer, m_img,canon, img_rect, toop);

    }
    if ((xnxx>730)&&(xnxx<980)&&(ynyy>450)&&(ynyy<650) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
    {
        e->type = 0 ;

        m_img = IMG_LoadTexture(m_renderer, "back map 4.png");
        ifstream coord("map 4.txt", ios::in);
        open_map_coordinates(m_renderer, coord);
        move_ball(m_renderer, m_img,canon, img_rect, toop);

    }
    e->type = 0 ;
}
void setting_menu(SDL_Renderer* m_renderer, Mix_Music *backgroundsound)
{
    e->type = 0;

    SDL_PollEvent(e);
    SDL_ShowCursor(SDL_ENABLE);
    xnxx = e->button.x;
    ynyy = e->button.y;


    if((e->key.keysym.sym == SDLK_ESCAPE && e ->type == SDL_KEYDOWN) || ((xnxx>1045)&&(xnxx<1210)&&(ynyy>580)&&(ynyy<800) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN)))
    {
        e ->type = 0 ;
        setting = 0;
    }

    if((xnxx>70)&&(xnxx<235)&&(ynyy>580)&&(ynyy<800)  && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
    {
        e ->type = 0 ;
        exite = 1;
        setting = 0;
    }
    if((xnxx>935)&&(xnxx<1020)&&(ynyy>610)&&(ynyy<690)  && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN))
    {
        draw(m_renderer,help_img,help_rect,300,200,540,540);
        SDL_RenderPresent(m_renderer);
        help = 1;
        setting = 0;
        e->type = 0;
    }
    while(help)
    {
        e->type = 0;
        SDL_PollEvent(e);
        if(e->key.keysym.sym == SDLK_ESCAPE && e ->type == SDL_KEYDOWN)
        {
            e->type = 0;
            help = 0;
        }
    }
    if ((xnxx>1085)&&(xnxx<1155)&&(ynyy>490)&&(ynyy<560) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN) &&(music == true))
    {
        e->type = 0 ;
        Mix_VolumeMusic(0);
        music = false ;
    }

    if ((xnxx>1085)&&(xnxx<1155)&&(ynyy>490)&&(ynyy<560) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN)&&(music == false ))
    {
        e->type = 0 ;
        Mix_VolumeMusic(128);
        music = true ;
    }
    if ((xnxx>945)&&(xnxx<1015)&&(ynyy>490)&&(ynyy<560) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN) &&(sound == true))
    {
        e->type = 0 ;
        Mix_VolumeChunk(ice,0);
        Mix_VolumeChunk(pop,0);
        Mix_VolumeChunk(explosion,0);
        Mix_VolumeChunk(fireball,0);
        Mix_VolumeChunk(win,0);
        Mix_VolumeChunk(lose,0);
        Mix_VolumeChunk(ignite,0);
        sound = false ;
    }
    if ((xnxx>945)&&(xnxx<1015)&&(ynyy>490)&&(ynyy<560) && (e->button.button == SDL_BUTTON_LEFT && e->type == SDL_MOUSEBUTTONDOWN)&&(sound == false ))
    {
        e->type = 0 ;
        Mix_VolumeChunk(ice,128);
        Mix_VolumeChunk(pop,128);
        Mix_VolumeChunk(explosion,128);
        Mix_VolumeChunk(fireball,128);
        Mix_VolumeChunk(win,128);
        Mix_VolumeChunk(lose,128);
        Mix_VolumeChunk(ignite,128);
        sound = true ;
    }



    e ->type = 0 ;

}

// better function to render ttf text

