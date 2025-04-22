#include "snake.h"

Snake* createSnake()
{
    int start_coord[] = {7, 10, 6, 10, 5, 10};
    size_t size = sizeof(start_coord) / sizeof(start_coord[0]);
    Snake* snake = (Snake*)malloc(sizeof(Snake));
    if(!snake)
    {
        MessageBoxW(NULL, L"Memory Allocation for snake failed.", L"malloc failed", MB_OK | MB_ICONERROR);
        return NULL;
    }
    snake->head = NULL;
    snake->head = createSnakeList(start_coord, size);
    if(!snake->head)
    {
        MessageBoxW(NULL, L"Memory Allocation for snake->head failed.", L"malloc failed", MB_OK | MB_ICONERROR);
        SAFE_FREE(snake);
        return NULL;
    }
    snake->previousX = 0;
    snake->previousY = 0;
    snake->cx = start_coord[0];
    snake->cy = start_coord[1];
    snake->headRect = SETUP_RECT(snake->cx, snake->cy, 1);
    snake->body = SETUP_RECT(0, 0, 0); //! Null rect
    snake->shouldGrow = FALSE;
    snake->isMoving = TRUE;
    snake->isDead = FALSE;
    return snake;
}

SnakeNode* createSnakeList(int *coords, size_t array_size)
{
    if(array_size < 1)
    {
        MessageBoxW(NULL, L"Can't have a null-sized array.", L"Array-size Error", MB_OK | MB_ICONWARNING);
        return NULL;
    }
    SnakeNode* head = createNode(coords[0], coords[1]);
    SnakeNode* current = head;
    for(int i = 2 ; i < array_size ; i += 2)
    {
        current->next = createNode(coords[i], coords[i+1]);
        current = current->next;
    }
    return head;
}

SnakeNode* createNode(int cx, int cy)
{
    SnakeNode* new_node = malloc(sizeof(SnakeNode));
    if(!new_node)
    {
        MessageBoxW(NULL, L"Memory Allocation for new_node failed.", L"malloc failed", MB_OK | MB_ICONERROR); 
        return NULL;
    }
    new_node->x = cx;
    new_node->y = cy;
    new_node->next = NULL;
    return new_node;
}

void freeSnakeMemory(SnakeNode* head)
{
    if(head)
    {
        SnakeNode* current = head;
        while(current != NULL)
        {
            SnakeNode* temp = current; 
            current = current->next;
            free(temp);
        }
    }
}

void logAndFreeSnakeMemory(SnakeNode* head, const char* filename) //! time display can be formatted but not my priority 
{
    time_t tm;
    time(&tm);
    int i = 0;
    FILE* fp = fopen(filename, "a");
    if (!fp)
    {
        MessageBoxW(NULL, L"Failed to open the file. The nodes will still be freed, but no log will be created.", L"Error", MB_ICONERROR | MB_OK);
        freeSnakeMemory(head);
        return;
    }
    if (head)
    {
        SnakeNode* current = head;
        fprintf(fp, "\nLog date and time : [%s]\n\n", strtok(ctime(&tm), "\n"));
        while(current != NULL)
        {
            SnakeNode* temp = current; 
            current = current->next;
            ++i;
            fprintf(fp, "freeing node with coord (%d, %d) (node: %d)\n", temp->x, temp->y, i);
            free(temp);
        }
    }
    fprintf(fp, "\nlog :\n%d nodes was freed.\n", i);
    fclose(fp);
}

void addHead(SnakeNode** head, int cx, int cy)
{
    SnakeNode* new_node = createNode(cx, cy);
    new_node->next = *head;
    (*head) = new_node;
}

void removeTail(SnakeNode** head_ptr_ptr)
{
    if(!*head_ptr_ptr)
        return;

    if((*head_ptr_ptr)->next == NULL)
    {
        SAFE_FREE(*head_ptr_ptr);
        return;
    }

    SnakeNode* current = *head_ptr_ptr;
    while(current->next->next != NULL)
    {
        current = current->next;
    }
    SAFE_FREE(current->next);
}


