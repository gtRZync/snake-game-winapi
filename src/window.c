#include "window.h"
#include "../resources/resources.h"
#include "sprite.h"
#include "game.h"

Input* self = NULL;
static const int buttonValues[KEY_COUNT];

static void handleInputCtxErr(Game* game) {
    if (!self) {
        const wchar_t* errCaption = L"Input Context Not Set";
        const wchar_t* errMsg = L"Input context is null. Call 'setInputContext(Input *ctx)' in main.c immediately after initializing the game, before recompiling.";
        MessageBoxW(NULL, errMsg, errCaption, MB_OK | MB_ICONERROR);
        game->destroy(game);
        exit(ERROR_SUCCESS);
    }
}

void CreateGameWindow(Game* game, HINSTANCE hInstance, int nShowCmd)
{
    handleInputCtxErr(game);
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
        game//! this makes it crashes
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
    setupDoubleBuffering(window->handle, game->buffer, window->width, window->height);
}

static void setInputState(Button* button, bool newState) {
    if(newState)
    {
        if(!button->held) {
            button->pressed = true;
            button->held = true;
            button->released = false;
        }
    }
    else
    {
        button->pressed = false;
        button->held = false;
        button->released = true;
    }
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
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            if (!cs || !cs->lpCreateParams) {
                MessageBoxA(NULL, "lpCreateParams is NULL!", "Fatal Error", MB_OK | MB_ICONERROR);
                return -1;
            }

            Game* game = (Game*)cs->lpCreateParams;
            if (!game) {
                MessageBoxA(NULL, "Game ptr is NULL!", "Fatal Error", MB_OK | MB_ICONERROR);
                return -1;
            }

            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)game);
            Window* window = game->window;
            RECT ClientArea;
            if(GetClientRect(window->handle, &ClientArea))
            {
                window->width = ClientArea.right - ClientArea.left;
                window->height = ClientArea.bottom - ClientArea.top;
            }
            SetupSprite(&title, "resources/assets/sprites/snake_banner.bmp", (const Frame){.totalRows=1, .totalCols=1});
            SetupSprite(&keys, "resources/assets/sprites/keys.bmp", (const Frame){.totalRows=1, .totalCols=1});
            SetupSprite(&sound, "resources/assets/sprites/sound.bmp", (const Frame){.totalRows=1, .totalCols=2});
            SetupSprite(&trophy, "resources/assets/sprites/trophy.bmp", (const Frame){.totalRows=1, .totalCols=1});
            SetupSprite(&restart_sprite, "resources/assets/sprites/restart_button.bmp", (const Frame){.totalRows=1, .totalCols=3});
            SetupSprite(&home_sprite, "resources/assets/sprites/home_button.bmp", (const Frame){.totalRows=1, .totalCols=3});
        }return 0;

        case WM_KILLFOCUS:
        case WM_SETFOCUS:
        {
            bool focused = (uMsg == WM_SETFOCUS);
            self->focused = focused;
        }return 0;

        case WM_SETCURSOR:
        {
            game->window->cursor = LoadCursor(NULL, (LPSTR)IDC_ARROW);
            SetCursor(game->window->cursor);
        }return 0; 

        case WM_ERASEBKGND:
            return 1;
        case WM_LBUTTONDOWN:
            setInputState(&self->mouse[MOUSE_LEFT], true);
            return 0;
        case WM_LBUTTONUP:
            setInputState(&self->mouse[MOUSE_LEFT], false);
            return 0;
        case WM_RBUTTONDOWN:
            setInputState(&self->mouse[MOUSE_RIGHT], true);
            return 0;
        case WM_RBUTTONUP:
            setInputState(&self->mouse[MOUSE_RIGHT], false);
            return 0;

        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            bool pressed = (uMsg == WM_KEYDOWN);
            for(KeyCode key = ARROW_UP; key < KEY_COUNT; key++) {
                if(wParam == buttonValues[key]) {
                    setInputState(&self->keyBoard[key], pressed);
                }
            }
        }return 0;

        case WM_SIZE:
        {
            if(game){
                game->window->height = HIWORD(lParam);
                game->window->width = LOWORD(lParam);
                game->resizeDoubleBuffer(hwnd, game->buffer, game->window->width, game->window->height);
            }
        }return 0;

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
        }return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static const int buttonValues[KEY_COUNT] = 
{
    VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_ESCAPE, 
    VK_F3,'Z', 'Q', 'S', 'D'
};

static void resetInput(Button *buttons) {
    ZeroMemory(buttons, sizeof(Button));
}

void setInputStateAfter() {
    if(!self->focused) {
        resetInput(self->keyBoard);
        return;
    }
    for(KeyCode key = ARROW_UP ; key < KEY_COUNT ; key++) {
        self->keyBoard[key].pressed = false;
        self->keyBoard[key].released = false;
    }
    for(MouseButton button = MOUSE_LEFT ; button < MOUSE_BUTTON_COUNT ; button++) {
        self->mouse[button].pressed = false;
        self->mouse[button].released = false;
    }
}

void setInputContext(Input *ctx)
{
    self = ctx;
}