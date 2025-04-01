#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
void init_env(FoxScriptEnv *env) {
    env->var_count = 0;
    env->func_count = 0;
    env->var_capacity = 10;  // Initial capacity of 10
    env->variables = (Variable *)malloc(env->var_capacity * sizeof(Variable));
    env->functions = (Function *)malloc(10 * sizeof(Function));  // Fixed size for functions (can be dynamic too)
}

// Resize the variables array when needed
void resize_variables(FoxScriptEnv *env) {
    env->var_capacity *= 2;  // Double the capacity
    env->variables = (Variable *)realloc(env->variables, env->var_capacity * sizeof(Variable));
}

// Add variable to environment
int add_variable(FoxScriptEnv *env, const char *name, int value, int is_mutable) {
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

// Update an existing variable's value
int update_variable(FoxScriptEnv *env, const char *name, int value) {
    for (int i = 0; i < env->var_count; i++) {
        if (strcmp(env->variables[i].name, name) == 0) {
            if (env->variables[i].is_mutable) {
                env->variables[i].value = value;
                return 0;
            } else {
                printf("Error: Variable %s is immutable!\n", name);
                return -1;
            }
        }
    }
    printf("Error: Variable %s not found!\n", name);
    return -1;
}

// Add function to environment
int add_function(FoxScriptEnv *env, const char *name, const char *body, const char *params[], int param_count) {
    Function new_func;
    strncpy(new_func.name, name, MAX_NAME_LENGTH);
    new_func.param_count = param_count;

    for (int i = 0; i < param_count; i++) {
        strncpy(new_func.params[i], params[i], MAX_NAME_LENGTH);
    }

    strncpy(new_func.body, body, MAX_NAME_LENGTH);
    env->functions[env->func_count++] = new_func;
    return 0;
}

// Call a function (mocked behavior)
int call_function(FoxScriptEnv *env, const char *name, int args[], int arg_count) {
    for (int i = 0; i < env->func_count; i++) {
        if (strcmp(env->functions[i].name, name) == 0) {
            if (env->functions[i].param_count == arg_count) {
                printf("Calling function %s with correct arguments.\n", name);
                return 0;
            } else {
                printf("Error: Incorrect number of arguments for function %s\n", name);
                return -1;
            }
        }
    }
    printf("Error: Function %s not found!\n", name);
    return -1;
}

// Free environment memory
void free_env(FoxScriptEnv *env) {
    free(env->variables);
    free(env->functions);
}

int main() {
    FoxScriptEnv env;
    init_env(&env);

    // Add variables
    add_variable(&env, "x", 10, 1);  // Mutable variable
    add_variable(&env, "y", 20, 0);  // Immutable variable

    // Update variables
    update_variable(&env, "x", 15);
    update_variable(&env, "y", 25);  // Should show error

    // Add function
    const char *params[] = {"a", "b"};
    add_function(&env, "add", "return a + b;", params, 2);

    // Call function
    int args[] = {10, 20};
    call_function(&env, "add", args, 2);

    // Free allocated memory
    free_env(&env);

    return 0;
}
