#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <Windows.h>

#include "dirent.h"

#define MAX_LINE_LENGTH 256
#define MAX_THREADS 1024

typedef struct {
    char filename[MAX_LINE_LENGTH];
    const char* target_string;
} ThreadData;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* search_file(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    char line[MAX_LINE_LENGTH];

    FILE* file = fopen(data->filename, "r");
    if (file == NULL)
    {
        perror("Error Opening File");
        pthread_exit(NULL);
    }

    int line_number = 1;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, data->target_string) != NULL)
        {
            pthread_mutex_lock(&mutex);
            printf("Match found in file %s, line %d: %s\n", data->filename, line_number, line);
            pthread_mutex_unlock(&mutex);
        }
        line_number++;
    }

    fclose(file);
    pthread_exit(NULL);
}

void search_files(const char* folder_path, const char* target_string)
{
    DIR* directory;
    struct dirent* entry;

    directory = opendir(folder_path);
    if (directory == NULL)
    {
        perror("Error Opening Directory");
        return;
    }

    pthread_t threads[MAX_THREADS];
    ThreadData thread_data[MAX_THREADS];
    int num_thread = 0;

    while ((entry = readdir(directory)) != NULL)
    {
        // 过滤 "." 和 ".."
        if (entry->d_type == DT_REG)
        {
            snprintf(thread_data[num_thread].filename, sizeof(thread_data[num_thread].filename), "%s/%s", folder_path, entry->d_name);
            thread_data[num_thread].target_string = target_string;

            if (pthread_create(&threads[num_thread], NULL, search_file, &thread_data[num_thread]) != 0)
            {
                fprintf(stderr, "Error Creating Thread\n");
                break;
            }

            num_thread++;
        }
    }

    closedir(directory);

    for (int i = 0; i < num_thread; i++)
    {
        pthread_join(threads[i], NULL);
    }
}

void search_in_file(const char* filename, const char* target_string)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 1;
    int found = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, target_string) != NULL)
        {
            printf("In %s: line %d\n", filename, line_number);
            found = 1;
        }
        line_number++;
    }

    //if (!found)
    //{
    //    printf("String not found in %s\n", filename);
    //}

    fclose(file);
}

void search_in_directory(const char* directory_path, const char* target_string)
{
    DIR* directory;
    struct dirent* entry;

    directory = opendir(directory_path);
    if (directory == NULL)
    {
        printf("Error opening directory: %s\n", directory_path);
        return;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            char filename[MAX_LINE_LENGTH];
            snprintf(filename, sizeof(filename), "%s/%s", directory_path, entry->d_name);
            search_in_file(filename, target_string);
        }
    }

    closedir(directory);
}

int main()
{
    char folder_path[256] = "C:/Users/86136/Desktop/PThreadTest/test1000";
    char target_string[256];

    printf("Enter target string: ");
    scanf("%s", target_string);

    LARGE_INTEGER  num_p;
    long long start_p, end_p, freq_p;
    QueryPerformanceFrequency(&num_p);
    freq_p = num_p.QuadPart;
    QueryPerformanceCounter(&num_p);
    start_p = num_p.QuadPart;

    //search in files
    search_files(folder_path, target_string);

    QueryPerformanceCounter(&num_p);
    end_p = num_p.QuadPart;

    printf("parallel time = %.2f ms\n\n\n", (double)((end_p - start_p) / 10));


    LARGE_INTEGER  num_s;
    long long start_s, end_s, freq_s;
    QueryPerformanceFrequency(&num_s);
    freq_s = num_s.QuadPart;
    QueryPerformanceCounter(&num_s);
    start_s = num_s.QuadPart;

    //calculating
    search_in_directory(folder_path, target_string);

    QueryPerformanceCounter(&num_s);
    end_s = num_s.QuadPart;

    printf("serial time = %.2f ms\n", (double)((end_s - start_s) / 10));

    return 0;
}