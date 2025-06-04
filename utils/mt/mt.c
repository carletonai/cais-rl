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

    return -1;
}

int load_projects_config(MT *mt) {
    char project_config_path[MAX_PATH_LEN];
    snprintf(project_config_path, sizeof(project_config_path),
             "%s/.mt/projects.yml", mt->repo_root);

    FILE *file = fopen(project_config_path, "r");
    if (!file) {
        fprintf(stderr, "Could not find %s\n", project_config_path);
        return 0;
    }

    char line[512];
    int in_projects = 0;
    mt->project_count = 0;

    while (fgets(line, sizeof(line), file) &&
           mt->project_count < MAX_PROJECTS) {
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0 || line[0] == '#') {
            continue;
        }

        if (strstr(line, "projects:")) {
            in_projects = 1;
            continue;
        }

        if (in_projects && line[0] == ' ') {
            char *colon = strchr(line, ':');
            if (!colon) continue;

            char *name_start = line;
            while (*name_start == ' ') name_start++;

            int name_len = colon - name_start;
            if (name_len >= MAX_NAME_LEN - 1 || name_len <= 0) continue;

            strncpy(mt->projects[mt->project_count].name, name_start, name_len);
            mt->projects[mt->project_count].name[name_len] = '\0';

            char *path_start = colon + 1;
            while (*path_start == ' ') path_start++;

            int path_len = strlen(path_start);
            if (path_len >= MAX_PATH_LEN - 1 || path_len <= 0) continue;

            strncpy(mt->projects[mt->project_count].path, path_start, path_len);
            mt->projects[mt->project_count].path[path_len] = '\0';

            mt->project_count++;
        } else if (in_projects && line[0] != ' ' && line[0] != '\t') {
            break;
        }
    }

    fclose(file);
    return mt->project_count > 0;
}

int mt_init(MT *mt) {
    if (!find_repo_root(mt->repo_root)) {
        fprintf(stderr, "Not in a git repository \n");
        return 1;
    }

    if (!load_projects_config(mt)) {
        fprintf(stderr, "Could not load projects config \n");
        return 1;
    }

    return 0;
}

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

char *find_script_in_dir(const char *dir_path, const char *script_name) {
    static char script_path[MAX_PATH_LEN];
    const char *extensions[] = {"", ".sh", NULL};

    for (int i = 0; extensions[i] != NULL; i++) {
        snprintf(script_path, sizeof(script_path), "%s/%s%s", dir_path,
                 script_name, extensions[i]);

        if (access(script_path, F_OK) == 0 && access(script_path, X_OK) == 0) {
            return script_path;
        }
    }

    return NULL;
}

int find_and_run_script(MT *mt, const char *script_name, char **script_args,
                        int arg_count) {
    char current_project[MAX_NAME_LEN];
    char scripts_dir[MAX_PATH_LEN];
    char script_path[MAX_PATH_LEN];
    char *found_script = NULL;

    // try to find the script in the current project
    if (which_project(mt, current_project) == 0) {
        char project_path[MAX_PATH_LEN];
        for (int i = 0; i < mt->project_count; i++) {
            if (strcmp(mt->projects[i].name, current_project) == 0) {
                snprintf(project_path, sizeof(project_path), "%s/%s",
                         mt->repo_root, mt->projects[i].path);

                snprintf(scripts_dir, sizeof(scripts_dir), "%s/.scripts",
                         project_path);
                found_script = find_script_in_dir(scripts_dir, script_name);
                if (found_script) {
                    break;
                }
            }
        }
    }

    // try the root .scripts folder
    if (!found_script) {
        snprintf(scripts_dir, sizeof(scripts_dir), "%s/.scripts",
                 mt->repo_root);
        found_script = find_script_in_dir(scripts_dir, script_name);
    }

    if (!found_script) {
        fprintf(stderr,
                "Script '%s' not found in project scripts or root scripts\n",
                script_name);
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) {
        char **exec_args = malloc((arg_count + 2) * sizeof(char *));
        exec_args[0] = found_script;
        for (int i = 0; i < arg_count; i++) {
            exec_args[i + 1] = script_args[i];
        }
        exec_args[arg_count + 1] = NULL;
        execv(found_script, exec_args);
        perror("execv failed");
        exit(1);
    } else if (pid > 0) {
        int status;
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    } else {
        perror("fork failed");
        return 1;
    }
}

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
