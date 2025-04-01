#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h> // For web requests

#define MAX_NAME_LENGTH 50

// Variable structure
typedef struct {
    char name[MAX_NAME_LENGTH];
    int value;
    int is_mutable;  // 1 for mutable, 0 for immutable
} Variable;

// Function structure
typedef struct {
    char name[MAX_NAME_LENGTH];
    char params[MAX_NAME_LENGTH][MAX_NAME_LENGTH];
    int param_count;
    char body[MAX_NAME_LENGTH];
} Function;

// Environment structure to store variables and functions
typedef struct {
    Variable *variables;    // Dynamic array of variables
    Function *functions;    // Array of functions
    int var_count;          // Current number of variables
    int func_count;         // Current number of functions
    int var_capacity;       // Capacity of the variable array
} FoxScriptEnv;

// Initialize environment
void envinit(FoxScriptEnv *env) {
    env->var_count = 0;
    env->func_count = 0;
    env->var_capacity = 10;
    env->variables = (Variable *)malloc(env->var_capacity * sizeof(Variable));
    env->functions = (Function *)malloc(10 * sizeof(Function));  // Fixed size for functions (can be dynamic too)
}

// Resize the variables array when needed
void resize_variables(FoxScriptEnv *env) {
    env->var_capacity *= 2;
    env->variables = (Variable *)realloc(env->variables, env->var_capacity * sizeof(Variable));
}

// Add variable to environment
int av(FoxScriptEnv *env, const char *name, int value, int is_mutable) {
    if (env->var_count >= env->var_capacity) {
        resize_variables(env);  // Resize array if full
    }

    Variable new_var;
    strncpy(new_var.name, name, MAX_NAME_LENGTH);
    new_var.value = value;
    new_var.is_mutable = is_mutable;
    env->variables[env->var_count++] = new_var;
    return 0;
}

// Delete variable from environment (user-defined command)
int dv(FoxScriptEnv *env, const char *name) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->variables[i].name, name) == 0) {
            for (int j = i; j < env->var_count - 1; j++) {
                env->variables[j] = env->variables[j + 1];
            }
            env->var_count--;
            return 0;
        }
    }
    return -1;  // Variable not found
}

// Perform a user-defined web request (using CURL)
void fetch(FoxScriptEnv *env, const char *url) {
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
}

// Free environment memory
void freemem(FoxScriptEnv *env) {
    free(env->variables);
    free(env->functions);
}

int main() {
    FoxScriptEnv env;
    envinit(&env);
}
