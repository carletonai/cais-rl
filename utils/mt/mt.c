/****************************************************
    mt - Multi-Tool
    a project-aware shell tool for cais-rl.
    written by: @Tarek-Sallam
****************************************************/

#include <dirent.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_PROJECTS 50
#define MAX_PATH_LEN 512
#define MAX_NAME_LEN 64

// a struct for each project
typedef struct {
    char name[MAX_NAME_LEN];
    char path[MAX_PATH_LEN];
} Project;

// the multi-tool struct
typedef struct {
    char repo_root[MAX_PATH_LEN];
    Project projects[MAX_PROJECTS];
    int project_count;
} MT;

// finds the root of the git repository
int find_repo_root(char *repo_root) {
    char cwd[MAX_PATH_LEN];
    char git_path[MAX_PATH_LEN];
    struct stat st;

    if (!getcwd(cwd, sizeof(cwd))) {
        return -1;
    }

    strcpy(repo_root, cwd);

    while (strlen(repo_root) > 1) {
        snprintf(git_path, sizeof(git_path), "%s/.git", repo_root);
        if (stat(git_path, &st) == 0) {
            return 0;
        }

        char *last_slash = strrchr(repo_root, '/');
        if (last_slash && last_slash != repo_root) {
            *last_slash = '\0';
        } else {
            break;
        }
    }

    return 0;
}

int load_projects_config(MT *mt);

int which_project(MT *mt, char *project_name) {
    char cwd[MAX_PATH_LEN];
    if (!getcwd(cwd, sizeof(cwd))) {
        return -1;
    }

    for (int i = 0; i < mt->project_count; i++) {
        if (strstr(cwd, mt->projects[i].path) != NULL) {
            strcpy(project_name, mt->projects[i].name);
            return 0;
        }
    }

    return -1;
}

int find_script_in_project(MT *mt, const char *script_name, char *script_path) {
}

int find_script_in_repo(MT *mt, const char *script_name, char *script_path) {}

int find_script(MT *mt, const char *script_name, char *script_path) {}

int list_project_scripts(MT *mt) {}

int run_script(MT *mt, const char *script_name) {}

int run_script(MT *mt, const char *script_name) {}

int get_project_path(MT *mt, const char *project_name, char *project_path) {}

int main(int argc, char *argv[]) {
    MT mt;

    if (!mt_init(&mt)) {
        return 1;
    }

    // show scripts if no arguments
    if (argc < 2) {
        list_scripts(&mt);
        return 0;
    }

    // list command to list scripts
    if (strcmp(argv[1], "list") == 0 || strcmp(argv[1], "ls") == 0) {
        list_scripts(&mt);
        return 0;
    }

    // help command
    if (strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0) {
        printf("mt - Multi-Tool for cais-rl\n\n");
        printf("Usage: \n");
        printf("  mt                    List available scripts\n");
        printf("  mt <script>           Run a script\n");
        printf("  mt <script> [args]    Run a script with arguments\n");
        printf("  mt list               List available scripts\n");
        printf("  mt help               Show this help\n\n");
        return 0;
    }

    char *script_name = argv[1];
    char **script_args = (argc > 2) ? &argv[2] : NULL;
    int arg_count = (argc > 2) ? argc - 2 : 0;

    int result = find_and_run_script(&mt, script_name, script_args, arg_count);
    if (result != 0) {
        fprintf(stderr, "Failed to run script '%s'\n", script_name);
    }
    return result;
}
