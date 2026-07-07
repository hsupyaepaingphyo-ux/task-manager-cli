#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 256

typedef struct
{
    int id;
    char title[MAX_TITLE];
    int done;
} Task;

Task *tasks = NULL;
int task_count = 0;    // how many tasks are currently in the list
int task_capacity = 0; // how much space your've reserved

void add_Task(const char *title)
{
    if (task_count == task_capacity)
    { // is the bag full? > make it bigger
        task_capacity = task_capacity == 0 ? 4 : task_capacity * 2;
        tasks = realloc(tasks, task_capacity * sizeof(Task));
    }

    Task t;
    t.id = task_count + 1;
    t.done = 0;
    strncpy(t.title, title, MAX_TITLE - 1);
    t.title[MAX_TITLE - 1] = '\0';

    tasks[task_count] = t;
    task_count++;

    printf("Added task #%d\n", t.id);
}

void list_Task()
{
    if (task_count == 0)
    {
        printf("No tasks yet.\n");
        return;
    }

    for (int i = 0; i < task_count; i++)
    {
        printf("%s %d %s\n",
               tasks[i].done ? "[x]" : "[ ]",
               tasks[i].id,
               tasks[i].title);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    { // argc = how many words you typed
        printf("Usage : taskman <command> [args]\n");
        return 1;
    }

    if (strcmp(argv[1], "add") == 0 && argc >= 3)
    {
        add_Task(argv[2]);
    }
    else if (strcmp(argv[1], "list") == 0)
    {
        list_Task();
    }
    else
    {
        printf("Unknown command: %s\n", argv[1]);
    }

    free(tasks);
    return 0;
}