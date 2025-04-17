#include "window.h"
#include "../resources/resources.h"
#include "sprite.h"
#include "game.h"

int32_t timer_intervalUID = 130;
int32_t screen_height, screen_width;
MSG msg = { };
int32_t score = 0;

void CreateGameWindow(Game* game, HINSTANCE hInstance, int nShowCmd)
{
    game->window->CLASS_NAME = "SnakeGameClass";
    game->window->wc = (WNDCLASS){ };
    game->window->wc.lpszClassName = game->window->CLASS_NAME;
    game->window->wc.lpfnWndProc = game->window->gameProc;
    game->window->wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE_ICON));
    game->window->wc.hCursor = LoadCursor(hInstance, (LPSTR)IDC_ARROW);
    game->window->wc.hInstance = hInstance;
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
}

int HandleGameMessages(Game* game)
{
    while(game->isRunning)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_CLOSE)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}


LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Game* game = (Game*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    static DOUBLE_BUFFER double_buffer;
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

            
            SetTimer(hwnd, TIMER_ID, timer_intervalUID, NULL);
            RECT ClientArea;
            if(GetClientRect(hwnd, &ClientArea))
            {
                screen_width = ClientArea.right - ClientArea.left;
                screen_height = ClientArea.bottom - ClientArea.top;
            }
            setup_double_buffer(hwnd, &double_buffer, screen_width, screen_height);

        }break;

        case WM_ERASEBKGND:
            return 1;

        case WM_PAINT:
        {   
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            renderToBackBuffer(double_buffer.BackBuffer, game->pellet, game->snake);
            copyToFrontBuffer(double_buffer.BackBuffer, hdc, screen_width, screen_height);
            EndPaint(hwnd, &ps);
        }break;
        
        case WM_KEYDOWN:
        {
            changeDirection(&game->snake_direction, game->snake, wParam);
            if(wParam == VK_ESCAPE) SendMessage(hwnd, WM_CLOSE, 0, 0);
        }break;

        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
        }

        case WM_TIMER:
        {
            updateSnakePosition(game->snake_direction, game->snake);
            eatPellet(game->snake, game->pellet);
            animatePellet(game->pellet);
            InvalidateRect(hwnd, NULL, TRUE);
        }break;

        case WM_SIZE:
        {
            screen_height = HIWORD(lParam);
            screen_width = LOWORD(lParam);
            setup_double_buffer(hwnd, &double_buffer, screen_width, screen_height);
        }break;

        case WM_DESTROY:
        {
            double_buffer_cleanup(&double_buffer);
            game->isRunning = FALSE;
            PostQuitMessage(EXIT_SUCCESS);
        }break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
