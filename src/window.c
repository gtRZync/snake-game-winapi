#include "window.h"
#include "../resources/resources.h"

int32_t timer_intervalUID = 130;
int32_t screen_height, screen_width;
Snake  snake;
Pellet pellet;
DIRECTIONS current_direction = RIGHT;
MSG msg = { };
int32_t score = 0;

void CreateGameWindow(HINSTANCE hInstance, WNDPROC wndProc, int nShowCmd)
{
    const char CLASS_NAME[] = "Snake_Cls";
    WNDCLASS wc = { };
    wc.lpszClassName = CLASS_NAME;
    wc.lpfnWndProc = wndProc;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SNAKE_ICON));
    wc.hCursor = LoadCursor(hInstance, (LPSTR)IDC_ARROW);
    wc.hInstance = hInstance;
    
    if(!RegisterClass(&wc))
    {
        MessageBox(NULL, "Error registering class", "Registration error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }

    
    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "Snake Game",
        WS_OVERLAPPEDWINDOW,
        CW_CENTERED_X,
        CW_CENTERED_Y,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    
    if(!hwnd)
    {
        MessageBox(NULL, "Error creating window", "Creation error", MB_OK | MB_ICONERROR);
        exit(EXIT_FAILURE);
    }
    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);
}

int HandleGameMessages()
{
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


LRESULT CALLBACK GameWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
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
            init_pellet(&pellet);
            snake = create_snake(5, 5);
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
            render_to_back_buffer(double_buffer.BackBuffer, &pellet, &snake);
            game_over(double_buffer.BackBuffer, &snake);
            copy_to_front_buffer(double_buffer.BackBuffer, hdc, screen_width, screen_height);
            EndPaint(hwnd, &ps);
        }break;
        
        case WM_KEYDOWN:
        {
            change_direction(&current_direction, &snake, wParam);
            if(wParam == VK_ESCAPE) SendMessage(hwnd, WM_CLOSE, wParam, lParam);
        }break;

        case WM_TIMER:
        {
            move_player(current_direction, &snake);
            eat_pellet(&snake, &pellet);
            animate_pellet(&pellet);
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
            KillTimer(hwnd, TIMER_ID);
            double_buffer_cleanup(&double_buffer);
            _free_list(snake.head,"free_logs/free_logs.txt");
            PostQuitMessage(EXIT_SUCCESS);
        }break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}