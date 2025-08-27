#include "window.h"
#include "../resources/resources.h"
#include "sprite.h"
#include "game.h"


void CreateGameWindow(Game* game, HINSTANCE hInstance, int nShowCmd)
{
    Window* window = game->window;
    window->CLASS_NAME = "SnakeGameClass";
    window->class = (WNDCLASS){ };
    window->class.style = CS_OWNDC;
    window->class.lpszClassName = window->CLASS_NAME;
    window->class.lpfnWndProc = window->gameProc;
    window->class.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE_ICON));
    window->class.hCursor = LoadCursor(hInstance, (LPSTR)IDC_ARROW);
    window->class.hInstance = hInstance;
    window->class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
    if(!RegisterClass(&window->class))
    {
        MessageBoxW(NULL, L"Error registering class", L"Registration error", MB_OK | MB_ICONERROR);
        game->destroy(game);
        exit(EXIT_FAILURE);
    }

    
    window->handle = CreateWindowExA(
        0,
        window->CLASS_NAME,
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
    
    if(!window->handle)
    {
        MessageBoxW(NULL, L"Error creating window", L"Creation error", MB_OK | MB_ICONERROR);
        game->destroy(game); //! skeptical bout this, still need to figure it out
        exit(EXIT_FAILURE);
    }
    ShowWindow(window->handle, nShowCmd);
    UpdateWindow(window->handle);
    window->hdc = GetDC(window->handle);
    setupDoubleBuffering(window->handle, game->buffer, screen_width, screen_height);
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
            SetupSprite(&title, "resources/assets/sprites/snake_banner.bmp", (const Frame){.totalRows=1, .totalCols=1});
            SetupSprite(&keys, "resources/assets/sprites/keys.bmp", (const Frame){.totalRows=1, .totalCols=1});
            SetupSprite(&sound, "resources/assets/sprites/sound.bmp", (const Frame){.totalRows=1, .totalCols=2});
            SetupSprite(&trophy, "resources/assets/sprites/trophy.bmp", (const Frame){.totalRows=1, .totalCols=1});
            SetupSprite(&restart_sprite, "resources/assets/sprites/restart_button.bmp", (const Frame){.totalRows=1, .totalCols=3});
            SetupSprite(&home_sprite, "resources/assets/sprites/home_button.bmp", (const Frame){.totalRows=1, .totalCols=3});
        }break;

        case WM_SETCURSOR:
        {
            game->window->cursor = LoadCursor(NULL, (LPSTR)IDC_ARROW);
            SetCursor(game->window->cursor);
        }break; 

        case WM_ERASEBKGND:
            return 1;
        //TODO: better input handling
        case WM_LBUTTONUP:
        {
            hasClicked = true;
            hasPressed = false;
        }break;

        case WM_LBUTTONDOWN:
        {
            hasPressed = true;
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
            SpriteCleanup(&trophy);
            SpriteCleanup(&restart_sprite);
            SpriteCleanup(&home_sprite);
            if(game) 
            {
                if(game->pellet) 
                    SpriteCleanup(&game->pellet->sprite);
                if(game->snake)
                    SpriteCleanup(&game->snake->headSprite);
                if(game->buffer)
                    game->doubleBufferingCleanup(game->buffer);
            }
            PostQuitMessage(EXIT_SUCCESS);
        }break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
