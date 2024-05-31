#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 256
#define BUFFER_SIZE 1024

void listFilesAndFolders(const char *path)
{
    DIR *directory;
    struct dirent *entry;

    directory = opendir(path);
    
    if (directory == NULL)
    {
        perror("Error opening directory");
        return;
    }

    printf("Files and folders in directory '%s':\n", path);
    
    while ((entry = readdir(directory)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }

    closedir(directory);
}

void createFiles(const char *path)
{
    int fileCount;
    printf("Enter the number of files to create: ");
    scanf("%d", &fileCount);

    for (int i = 0; i < fileCount; ++i)
    {
        char filename[MAX_PATH_LENGTH];
        printf("Enter the filename to create (%d/%d): ", i + 1, fileCount);
        scanf("%s", filename);
        char filePath[MAX_PATH_LENGTH];
        snprintf(filePath, sizeof(filePath), "%s/%s", path, filename);

        FILE *file = fopen(filePath, "w");
    
        if (file == NULL)
        {
            perror("Error creating file");
        }
    
        else
        {
            printf("File '%s' created successfully.\n", filename);
            fclose(file);
        }
    }
}

void deleteFiles(const char *path)
{
    int fileCount;
    printf("Enter the number of files to delete: ");
    scanf("%d", &fileCount);

    for (int i = 0; i < fileCount; ++i)
    {
        char filename[MAX_PATH_LENGTH];
        printf("Enter the filename to delete (%d/%d): ", i + 1, fileCount);
        scanf("%s", filename);
        char filePath[MAX_PATH_LENGTH];
        snprintf(filePath, sizeof(filePath), "%s/%s", path, filename);

        if (remove(filePath) == 0)
        {
            printf("File '%s' deleted successfully.\n", filename);
        }
    
        else
        {
            perror("Error deleting file");
        }
    }
}

void copyFile(const char *sourcePath)
{
    char sourceFilename[MAX_PATH_LENGTH];
    printf("Enter the filename to copy: ");
    scanf("%s", sourceFilename);
    char sourceFilePath[MAX_PATH_LENGTH];
    snprintf(sourceFilePath, sizeof(sourceFilePath), "%s/%s", sourcePath, sourceFilename);

    char destinationPath[MAX_PATH_LENGTH];
    printf("Enter the destination directory: ");
    scanf("%s", destinationPath);
    char destinationFilePath[MAX_PATH_LENGTH];
    snprintf(destinationFilePath, sizeof(destinationFilePath), "%s/%s", destinationPath, sourceFilename);

    FILE *sourceFile = fopen(sourceFilePath, "r");
    
    if (sourceFile == NULL)
    {
        perror("Error opening source file");
        return;
    }
    
    FILE *destinationFile = fopen(destinationFilePath, "w");
    
    if (destinationFile == NULL)
    {
        perror("Error opening destination file");
        fclose(sourceFile);
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    
    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, sourceFile)) > 0)
    {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }

    printf("File '%s' copied successfully.\n", sourceFilename);
    fclose(sourceFile);
    fclose(destinationFile);
}

void modifyFile(const char *path)
{
    char filename[MAX_PATH_LENGTH];
    printf("Enter the filename to modify: ");
    scanf("%s", filename);
    char filePath[MAX_PATH_LENGTH];
    snprintf(filePath, sizeof(filePath), "%s/%s", path, filename);

    FILE *file = fopen(filePath, "a");
    
    if (file == NULL)
    {
        perror("Error opening file for modification");
        return;
    }

    printf("Enter text to append to the file (Ctrl+D to end input):\n");
    char buffer[BUFFER_SIZE];
    
    while (fgets(buffer, sizeof(buffer), stdin))
    {
        fputs(buffer, file);
    }

    printf("File '%s' modified successfully.\n", filename);
    fclose(file);
}

int main()
{
    char currentPath[MAX_PATH_LENGTH];
    char newPath[MAX_PATH_LENGTH];

    if (getcwd(currentPath, sizeof(currentPath)) == NULL)
    {
        perror("Error getting current directory");
        return 1;
    }

    printf("Welcome to the File Manager\n");

    while (1)
    {
        int choice;
        printf("\nCurrent Directory: %s\n", currentPath);
        printf("1. List files and folders\n");
        printf("2. Create files\n");
        printf("3. Delete files\n");
        printf("4. Copy a file\n");
        printf("5. Modify a file\n");
        printf("6. Change directory\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            listFilesAndFolders(currentPath);
            break;
        
        case 2:
            createFiles(currentPath);
            break;
        
        case 3:
            deleteFiles(currentPath);
            break;
        
        case 4:
            copyFile(currentPath);
            break;
        
        case 5:
            modifyFile(currentPath);
            break;
        
        case 6:
            printf("Enter the name of the directory to navigate to: ");
            scanf("%s", newPath);
            
            if (chdir(newPath) == 0)
            {
                if (getcwd(currentPath, sizeof(currentPath)) == NULL)
                {
                    perror("Error getting current directory");
                    return 1;
                }
            }

            else
            {
                perror("Error changing directory");
            }
            
            break;

        case 7:
            printf("Exiting...\n");
            return 0;

        default:
            printf("Invalid choice. Please enter a number between 1 and 7.\n");
        }
    }

    return 0;
}
