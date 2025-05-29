#include "window.h"
#include "../resources/resources.h"
#include "sprite.h"
#include "game.h"


void CreateGameWindow(Game* game, HINSTANCE hInstance, int nShowCmd)
{
    game->window->CLASS_NAME = "SnakeGameClass";
    game->window->wc = (WNDCLASS){ };
    game->window->wc.lpszClassName = game->window->CLASS_NAME;
    game->window->wc.lpfnWndProc = game->window->gameProc;
    game->window->wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE_ICON));
    game->window->wc.hCursor = LoadCursor(hInstance, (LPSTR)IDC_ARROW);
    game->window->wc.hInstance = hInstance;
    game->window->wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
    if(!RegisterClass(&game->window->wc))
    {
        MessageBoxW(NULL, L"Error registering class", L"Registration error", MB_OK | MB_ICONERROR);
        game->destroy(game);
        exit(EXIT_FAILURE);
    }

    
    game->window->hwnd = CreateWindowExA(
        0,
        game->window->CLASS_NAME,
        "Snake Game",
        WS_OVERLAPPEDWINDOW,
        CW_CENTERED_X,
        CW_CENTERED_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        game
    );
    
    if(!game->window->hwnd)
    {
        MessageBoxW(NULL, L"Error creating window", L"Creation error", MB_OK | MB_ICONERROR);
        game->destroy(game);
        exit(EXIT_FAILURE);
    }
    ShowWindow(game->window->hwnd, nShowCmd);
    UpdateWindow(game->window->hwnd);
    game->window->hdc = GetDC(game->window->hwnd);
    game->setupDoubleBuffering(game->window->hwnd, game->buffer, screen_width, screen_height);
}


LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Game* game = (Game*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch(uMsg)
    {
        case WM_NCCREATE:
        {
            LONG_PTR newLong = GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_THICKFRAME ;
            newLong &= ~WS_MAXIMIZEBOX; 
            UINT uFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED;
            SetWindowLongPtr(hwnd, GWL_STYLE, newLong);
            SetWindowPos(hwnd, NULL, 0, 0, 0, 0, uFlags);
        }break;

        case WM_CREATE:
        {
            CREATESTRUCT* cs = ((CREATESTRUCT*)lParam);
            game = (Game*)cs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)game);
            RECT ClientArea;
            if(GetClientRect(hwnd, &ClientArea))
            {
                screen_width = ClientArea.right - ClientArea.left;
                screen_height = ClientArea.bottom - ClientArea.top;
            }
            SetupSprite(&title, "resources/assets/sprites/snake_banner.bmp", 1, 1);
            SetupSprite(&keys, "resources/assets/sprites/keys.bmp", 1, 1);
            SetupSprite(&sound, "resources/assets/sprites/sound.bmp", 1, 2);
            SetupSprite(&trophee, "resources/assets/sprites/trophee.bmp", 1, 1);
            SetupSprite(&restart_sprite, "resources/assets/sprites/restart_button.bmp", 1, 1);
            sound.currentFrame = 1;
        }break;

        case WM_SETCURSOR:
        {
            game->window->cursor = LoadCursor(NULL, (LPSTR)IDC_ARROW);
            SetCursor(game->window->cursor);
        }break; 

        case WM_ERASEBKGND:
            return 1;

        case WM_LBUTTONUP:
        {
            hasClicked = TRUE;
        }break;

        case WM_SIZE:
        {
            screen_height = HIWORD(lParam);
            screen_width = LOWORD(lParam);
            if(game)
                game->resizeDoubleBuffer(hwnd, game->buffer, screen_width, screen_height);
        }break;

        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
        }break;


        case WM_DESTROY:
        {
            SpriteCleanup(&title);
            SpriteCleanup(&keys);
            SpriteCleanup(&sound);
            SpriteCleanup(&trophee);
            SpriteCleanup(&restart_sprite);
            SpriteCleanup(&game->pellet->sprite);
            PostQuitMessage(EXIT_SUCCESS);
        }break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
