#include "../include/chip8.h"


void render_init(Rendering* screen)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)  
    {
        fprintf(stderr, "Failed to initialize SDL 2.\n");
        return;
    }

    strcpy(screen->name, "chip 8 emulator");

    screen->wpixel = 64;
    screen->hpixel = 32;
    screen->scale = 20;

    screen->width = screen->wpixel * screen->scale;
    screen->height = screen->hpixel * screen->scale;

    screen->window = SDL_CreateWindow(screen->name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen->width, screen->height, 0);

    if(screen->window == NULL)
    {
        fprintf(stderr, "Failed to create a window.\n");
        SDL_Quit();
        return;
    }

    screen->render = SDL_CreateRenderer(screen->window, -1, 0);

    if(screen->render == NULL)
    {
        fprintf(stderr, "Failed to create a render.\n");
        SDL_DestroyWindow(screen->window);
        SDL_Quit();
        return;
    }
}

void process_keyboard(Rendering* screen, Chip8_t* c)
{

    while(SDL_PollEvent(&screen->event))
    {
        switch (screen->event.type)
        {
        case SDL_QUIT:
            screen->isExit = 1;
            break;

        case SDL_KEYDOWN:
            fprintf(stdout, "Key pressed (%d).\n", screen->event.key.keysym.scancode);
            switch (screen->event.key.keysym.sym)
            {
            case SDLK_1: c->KEYBOARD[0x1] = 1; break;
            case SDLK_2: c->KEYBOARD[0x2] = 1; break;
            case SDLK_3: c->KEYBOARD[0x3] = 1; break;
            case SDLK_a: c->KEYBOARD[0x4] = 1; break;

            case SDLK_z: c->KEYBOARD[0x5] = 1; break;
            case SDLK_e: c->KEYBOARD[0x6] = 1; break;
            case SDLK_q: c->KEYBOARD[0x7] = 1; break;
            case SDLK_s: c->KEYBOARD[0x8] = 1; break;

            case SDLK_d: c->KEYBOARD[0x9] = 1; break;
            case SDLK_w: c->KEYBOARD[0xA] = 1; break;
            case SDLK_x: c->KEYBOARD[0x0] = 1; break;
            case SDLK_c: c->KEYBOARD[0xB] = 1; break;

            case SDLK_4: c->KEYBOARD[0xC] = 1; break;
            case SDLK_r: c->KEYBOARD[0xD] = 1; break;
            case SDLK_f: c->KEYBOARD[0xE] = 1; break;
            case SDLK_v: c->KEYBOARD[0xF] = 1; break;

            default: break;
            }
            break;

        case SDL_KEYUP:
            fprintf(stdout, "Key released (%d).\n", screen->event.key.keysym.scancode);
            switch (screen->event.key.keysym.sym)
            {
            case SDLK_1: c->KEYBOARD[0x1] = 0; break;
            case SDLK_2: c->KEYBOARD[0x2] = 0; break;
            case SDLK_3: c->KEYBOARD[0x3] = 0; break;
            case SDLK_a: c->KEYBOARD[0x4] = 0; break;

            case SDLK_z: c->KEYBOARD[0x5] = 0; break;
            case SDLK_e: c->KEYBOARD[0x6] = 0; break;
            case SDLK_q: c->KEYBOARD[0x7] = 0; break;
            case SDLK_s: c->KEYBOARD[0x8] = 0; break;

            case SDLK_d: c->KEYBOARD[0x9] = 0; break;
            case SDLK_w: c->KEYBOARD[0xA] = 0; break;
            case SDLK_x: c->KEYBOARD[0x0] = 0; break;
            case SDLK_c: c->KEYBOARD[0xB] = 0; break;

            case SDLK_4: c->KEYBOARD[0xC] = 0; break;
            case SDLK_r: c->KEYBOARD[0xD] = 0; break;
            case SDLK_f: c->KEYBOARD[0xE] = 0; break;
            case SDLK_v: c->KEYBOARD[0xF] = 0; break;

            default: break;
            }
            break;
        
        default:
            break;
        }
    }
}

void clear_screen(Rendering* screen)
{
    SDL_SetRenderDrawColor(screen->render, 50, 50, 50, 0);
    SDL_RenderClear(screen->render);
}

void update_screen(Rendering* screen)
{
    SDL_RenderPresent(screen->render);
}

void draw_pixel(Rendering* screen, Chip8_t* c)
{
    SDL_Rect pixel;
    pixel.h = screen->scale;
    pixel.w = screen->scale;


    for(uint32_t i = 0; i < RESOLUTION; ++i)
    {
        if(c->PIXELS[i])
        {
            pixel.x = (i % screen->wpixel) * screen->scale;
            pixel.y = (i / screen->wpixel) * screen->scale;
            SDL_SetRenderDrawColor(screen->render, 255, 0, 0, 0);
            SDL_RenderFillRect(screen->render, &pixel);

            SDL_SetRenderDrawColor(screen->render, 50, 50, 50, 0);
            SDL_RenderDrawRect(screen->render, &pixel);
        }
    }
    

}

void loop(Rendering* screen, Chip8_t* c)
{
    screen->isExit = 0;
    
    while(!screen->isExit)
    {
        uint16_t cycles = 500 / 60;

        clear_screen(screen);

        process_keyboard(screen, c);

        uint64_t debut = SDL_GetPerformanceCounter();

        while (cycles > 0)
        {
            Chip8_process_instruction(c);
            --cycles;
        }

        uint64_t end = SDL_GetPerformanceCounter();

        double time_elapsed = (double)((end - debut) * 1000)/SDL_GetPerformanceFrequency();

        SDL_Delay(16.67 > time_elapsed ? 16.67 - time_elapsed : 0);

        draw_pixel(screen, c);

        update_screen(screen);

        if(c->DELAY_TIMER > 0)
            --c->DELAY_TIMER;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       }
}

void clear_render(Rendering* screen)
{
    SDL_DestroyRenderer(screen->render);
    SDL_DestroyWindow(screen->window);
    SDL_Quit();
}