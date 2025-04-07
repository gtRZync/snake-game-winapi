#include "snake.h"

Snake create_snake(int start_x, int start_y)
{
    Snake snk;
    snk.head = NULL;
    snk.cx = start_x;
    snk.cy = start_y;
    snk.head = create_node(start_x, start_y);
    snk.rect = (RECT){
        snk.cx * TILE_SIZE,
        snk.cy * TILE_SIZE,
        (snk.cx + 1) * TILE_SIZE,
        (snk.cy + 1) * TILE_SIZE,
    };
    snk.should_grow = FALSE;
    return snk;
}

SnakeNode* create_node(int cx, int cy)
{
    SnakeNode* new_node = malloc(sizeof(SnakeNode));
    if(new_node == NULL)
    {
        MessageBox(NULL, "Memory Allocation for new_node failed.", "malloc failed", MB_OK | MB_ICONERROR); //! can get specific error with GetLastError()
        exit(EXIT_FAILURE);
    }
    new_node->x = cx;
    new_node->y = cy;
    new_node->next = NULL;
    return new_node;
}

void free_list(SnakeNode* head)
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

void _free_list(SnakeNode* head, const char* filename) //! time display can be formatted but not my priority 
{
    time_t tm;
    time(&tm);
    int i = 0;
    FILE* fp = fopen(filename, "a");
    if (!fp)
    {
        MessageBoxW(NULL, L"Failed to open the file. The nodes will still be freed, but no log will be created.", L"Error", MB_ICONERROR | MB_OK);
        free_list(head);
        exit(EXIT_FAILURE);
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

void add_head(SnakeNode** head, int cx, int cy)
{
    SnakeNode* new_node = create_node(cx, cy);
    new_node->next = *head;
    (*head) = new_node;
}

void remove_tail(SnakeNode** head_ptr_ptr)
{
    if(*head_ptr_ptr == NULL)
        return;

    if((*head_ptr_ptr)->next == NULL)
    {
        free(*head_ptr_ptr);
        *head_ptr_ptr = NULL;
        return;
    }

    SnakeNode* current = *head_ptr_ptr;
    while(current->next->next != NULL)
    {
        current = current->next;
    }
    free(current->next);
    current->next = NULL;
}


