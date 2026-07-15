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

void save_Tasks()
{
    FILE *f = fopen("tasks.txt", "w");

    if (f == NULL)
    {
        printf("Error: could not save tasks.\n");
        return;
    }

    for (int i = 0; i < task_count; i++)
    {
        fprintf(f, "%d|%d|%s\n", tasks[i].id, tasks[i].done, tasks[i].title);
    }

    fclose(f);
}

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

    save_Tasks();

    printf("Added task #%d\n", t.id);
    return;
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

void complete_Task(int id)
{
    for (int i = 0; i < task_count; i++)
    {
        if (tasks[i].id == id)
        {
            tasks[i].done = 1;
            save_Tasks();
            printf("Task #%d marked done.\n", id);
            return;
        }
    }
    printf("No task with #%d ID\n", id);
    return;
}

void delete_Task(int id)
{
    for (int i = 0; i < task_count; i++)
    {
        if (tasks[i].id == id)
        {
            for (int j = i; j < task_count - 1; j++)
            {
                tasks[j] = tasks[j + 1];
            }
            task_count--;
            save_Tasks();
            printf("Task #%d deleted\n", id);
            return;
        }
    }
    printf("No task with id %d\n", id);
    return;
}

void load_Tasks()
{
    FILE *f = fopen("tasks.txt", "r");

    if (f == NULL)
    {
        // printf("Error reading the file.");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), f))
    {
        int id, done;
        char title[MAX_TITLE];

        if (sscanf(line, "%d|%d|%[^\n]", &id, &done, title) == 3)
        {
            if (task_count == task_capacity)
            {
                task_capacity = task_count == 0 ? 4 : task_capacity * 2;
                tasks = realloc(tasks, task_capacity * sizeof(Task));
            }

            Task t;
            t.id = id;
            t.done = done;
            strncpy(t.title, title, MAX_TITLE - 1);
            t.title[MAX_TITLE - 1] = '\0';

            tasks[task_count] = t;
            task_count++;
        }
    }
    fclose(f);
}

void parse_and_run(char *input)
{
    // trim the new line char off the end
    input[strcspn(input, "\n")] = '\0';

    //"quit" command
    if (strcmp(input, "quit") == 0)
    {
        printf("Bye!\n");
        free(tasks);
        exit(0);
    }

    //"list" command
    if (strcmp(input, "list") == 0)
    {
        list_Task();
        return;
    }

    //"add" command
    if (strncmp(input, "add ", 4) == 0)
    {
        char *title = input + 4;

        // strip surrounding quotes if present
        if (title[0] == '"')
        {
            title++;
            int len = strlen(title);
            if (len > 0 && title[len - 1] == '"')
            {
                title[len - 1] = '\0';
            }
        }

        add_Task(title);
        return;
    }

    //"complete" command
    if (strncmp(input, "complete ", 9) == 0)
    {
        int id = atoi(input + 9); // atoi - "ascii to int" text to int
        complete_Task(id);
        return;
    }

    //"delete" command
    if (strncmp(input, "delete ", 7) == 0)
    {
        int id = atoi(input + 7);
        delete_Task(id);
        return;
    }

    printf("Unknown command: %s\n", input);
    return;
}

int main()
{
    char input[512];

    load_Tasks();

    printf("taskman - type 'add <task>', 'list', 'complete, 'delete' or 'quit'\n\n");

    while (1)
    {
        printf("> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        parse_and_run(input);
    }

    free(tasks);
    return 0;
}