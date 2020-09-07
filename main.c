#include <unistd.h>
#include "string.h"
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h> 
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include "visualizer.c"

//structs
typedef struct valgrind_info {
    char * str; 
    int line_num; 
} valgrind_info; 

// typedef struct datastructure {
//     char* std_structure; 
//     char* listed_structure; 
//     char* name; 
//     struct datastructure** containments; 
//     size_t capacity; 
//     int num_containments; 
//     bool mem_being_used; 
// } datastructure; 

//methods
valgrind_info* extract_valgrind_info(FILE* fp);
void create_full_visualization(char* leak_file);
char * extract_listed_structure(char* word);
char* get_arg(char* word);
char* get_name_from_line(char* line);
datastructure* get_datastructure_from_name(char* name);
char* get_first_arg(char* word);
char* get_second_arg(char* word);
void print_test_visualization();

//globals
const char* MEM_ARGUMENT = "--leak-check=full"; 
const char* MEM_PRGRM = "valgrind"; 
const char* RUN_PRGRM_STR = "./"; 
const char NULL_CHAR = '\0'; 

const char* make_queue = "make_queue"; 
const char* make_stack = "make_stack";
const char* make_vector = "make_vector"; 
const char* make_array = "make_array"; 

const char* add_queue = "make_queue"; 
const char* add_stack = "make_stack";
const char* add_vector = "make_vector"; 
const char* add_array = "make_array"; 

const char* destroy_queue = "make_queue"; 
const char* destroy_stack = "make_stack";
const char* destroy_vector = "make_vector"; 
const char* destroy_array = "make_array"; 

int num_queues; 
int num_stacks;
int num_vectors; 
int num_arrays;
int max_size = 100;  

int q_idx;
int s_idx;
int v_idx;
int a_idx; 

char** queue_names; 
char** stack_names; 
char** vector_names; 
char** array_names;

datastructure* structures_in_code; 
int num_structures_created; 

int main(int argc, char** argv) {
    
    //check for sufficient arguments
    if (argc < 2) {
        //print_usage(); 
        printf("not enough arugments");
        exit(1); 
    }

    char* exe_name = argv[1]; 

    //initalizations and count 
    int num_queues = 0; 
    int num_arrays = 0; 
    int num_vectors = 0; 
    int num_stacks = 0; 
  
    int option = 0; 
    while ((option = getopt(argc, argv, "q:s:v:a:n:")) != -1) {
        switch (option) {
            case '?':
                //print_usage(); 
                printf("invalid argument");
                exit(1); 
            case 'q': 
                num_queues++; 
                break;
            case 's':
                num_stacks++; 
                break; 
            case 'v':
                num_vectors++; 
                break;
            case 'a':
                num_arrays++;
                break;
            case 'n':
                max_size = atoi(optarg);
                break;
        }
    }

    //check valid input 
    if (max_size == 0) {
        printf("Must specify n\n");
        exit(1); 
    }

    //malloc space for the names arrays 
    queue_names = (char**) malloc(num_queues*sizeof(char*)); 
    stack_names = (char**) malloc(num_stacks*sizeof(char*)); 
    array_names = (char**) malloc(num_arrays*sizeof(char*)); 
    vector_names = (char**) malloc(num_vectors*sizeof(char*)); 

    //adding to name arrays
    q_idx = 0; 
    s_idx = 0; 
    v_idx = 0; 
    a_idx = 0; 

    optind = 1; //reset so that we can parse the command line arguments again 
    option = 0; 

     while ((option = getopt(argc, argv, "q:s:v:a:n:")) != -1) {
        switch (option) {
            case 'q': 
                queue_names[q_idx] = optarg; 
                q_idx++; 
                break;
            case 's':
               stack_names[s_idx] = optarg; 
                s_idx++; 
                break;
            case 'v':
                vector_names[v_idx] = optarg; 
                v_idx++; 
                break;
            case 'a':
               array_names[a_idx] = optarg; 
                a_idx++; 
                break;
            case 'n':
                break;
        }
    }

    //print arrays
    printf("Queues:\n"); 
    for (int i = 0; i < num_queues; i++) {
        printf("%s ", queue_names[i]); 
    }
    printf("\n");
    printf("Arrays:\n"); 
    for (int i = 0; i < num_arrays; i++) {
        printf("%s ", array_names[i]); 
    }
    printf("\n");
    printf("Stacks:\n"); 
    for (int i = 0; i < num_stacks; i++) {
        printf("%s ", stack_names[i]); 
    }
    printf("\n");
    printf("Vectors:\n"); 
    for (int i = 0; i < num_vectors; i++) {
        printf("%s ", vector_names[i]); 
    }
    printf("\n");

    //add ./ to the executable name --> ./exeName
    int size_buff = strlen(RUN_PRGRM_STR) + strlen(exe_name) + 1; 
    char buff[size_buff]; 
    strcpy(buff, RUN_PRGRM_STR);
    strcpy(buff + strlen(RUN_PRGRM_STR), exe_name);
    buff[size_buff - 1] = NULL_CHAR; 

    //save stderr file descriptor 
    int saved_stderr = dup(2);

    //create pipe
    int pipefd[2];
    pipe(pipefd);

    //run valgrind
    printf("Running Valgrind...\n");  
    pid_t child_pid = fork();
    if (child_pid == 0) {
        dup2(pipefd[1], 2);
        //close read because child process is only writting
        close(pipefd[0]); 
        execlp(MEM_PRGRM, MEM_PRGRM, MEM_ARGUMENT, buff, (char*) NULL);
    } else {
        int status; 
        waitpid(child_pid, &status, 0); 
    }

    //close write, becasue parent process is only reading
    close(pipefd[1]); 
    //get file pointer from file descriptor
    FILE* fp = fdopen(pipefd[0], "r"); 

    //extract line number and file name from valgrind output (from pipe)
    printf("Parsing Valgrind...\n");
    valgrind_info* v = extract_valgrind_info(fp);
    //safety check to avoid segfaults 
    if (v == NULL) {
        printf("There is no memory leak...\nexiting...\n"); 
        exit(1); 
    }

    printf("Memory Leak in File: %s on line (int): %d \n", v->str, v->line_num); 

    printf("Opening %s...\n", v->str); 
    //open the file, 
    int leak_file_fd = open(v->str, O_RDONLY, S_IRUSR);
    //get file pointer from file descriptor
    FILE* leak_file_fp = fdopen(leak_file_fd, "r"); 
    
    //go to the line, perform analysis of the line by adding every data structure in the line to a queue and parallelizing the task
    size_t val = 0;
    char* lineptr = NULL; 
    for (int i = 0; i < v->line_num; i++) {
        getline(&lineptr, &val, leak_file_fp);
        if (lineptr[strlen(lineptr) - 1] == '\n') {
            lineptr[strlen(lineptr) - 1] = NULL_CHAR;
        }
    }
    printf("Code With Leaks: %s\n", lineptr); 
    free(lineptr); 
    close(leak_file_fd); 

    //testing
    printf("Generating Text For Visualizations...\n"); 
    create_full_visualization(v->str);

    	FILE * output = fopen("output.html", "w");
 	if (!output) {
 		return 0;
 	}

    //Generate Graphics 
 	writeHead(output);
 	writeSVG(output, structures_in_code, NULL, num_structures_created);
 	writeSummary(output);
 	fclose(output); 

    //clean up
    dup2(saved_stderr, 2);
    close(pipefd[0]); 
    close(leak_file_fd); 
}

//TODO: NEED TO CHANGE TO RETURN NULL IN THE CASE THAT THERE ARE NO MEMORY LEAKS
valgrind_info* extract_valgrind_info(FILE* fp) {
    size_t val = 0;
    char* lineptr = NULL; 
    char* filename = (char*) malloc(1); 
    char* line_num = (char*) malloc(1); 
    bool mem_err = false; 
    while (getline(&lineptr, &val, fp) != -1) {
        if (lineptr[strlen(lineptr) - 1] == '\n') {
            lineptr[strlen(lineptr) - 1] = NULL_CHAR;
        }
        char* main_ptr; 
        if ((strstr(lineptr, "by ") != NULL) && ((main_ptr = strstr(lineptr, "main")) != NULL) ) {
            mem_err = true; 
            filename = (char*) realloc(filename, strlen(lineptr)); 
            line_num = (char*) realloc(line_num, strlen(lineptr)); 
            main_ptr += 6; 
            int idx = 0; 
            while (*main_ptr != ':') {
                filename[idx] = *main_ptr; 
                main_ptr++; 
                idx++;
            }
            filename[idx] = NULL_CHAR;
            main_ptr++; 
            idx = 0;
            while (*main_ptr != ')') {
                line_num[idx] = *main_ptr; 
                main_ptr++; 
                idx++;
            }
            line_num[idx] = NULL_CHAR;
        }
    }
    if (mem_err == false) {
        return NULL; 
    }
    free(lineptr); 
    valgrind_info* v = (valgrind_info*) malloc(sizeof(valgrind_info)); 
    v->str = filename; 
    v->line_num = atoi(line_num);
    free(line_num); 
    return v; 
}

void create_full_visualization(char* leak_file) {

    int leak_file_fd = open(leak_file, O_RDONLY, S_IRUSR);
    FILE* leak_file_fp = fdopen(leak_file_fd, "r"); 

    num_structures_created = 0; 
    char word[1024];
    while (fscanf(leak_file_fp, " %1023s", word) == 1) {
        //printf("%s\n", word); 
        if (strstr(word, "make_queue") != NULL || strstr(word, "make_stack") != NULL 
        || strstr(word, "make_vector") != NULL || strstr(word, "make_array") != NULL) {
            num_structures_created++; 
        }
    }
    close(leak_file_fd); 

    printf("Number of std datastructures detected: %d\n", num_structures_created); 
    printf("Max containment of any datastructure: %d\n", max_size); 

    //allocate space num_structures_created many data structure structs 
    structures_in_code = (datastructure*) malloc(num_structures_created * sizeof(datastructure)); 
    //initalize all the arrays for the datastructures in the array and set their arrays to max_size
    for (int i = 0; i < num_structures_created; i++) {
        structures_in_code[i].containments = (datastructure**) malloc(max_size*sizeof(max_size * sizeof(datastructure*))); 
        structures_in_code[i].num_containments = 0; //nothing has been added yet
    }

     //populate data structures array
    leak_file_fd = open(leak_file, O_RDONLY, S_IRUSR);
    leak_file_fp = fdopen(leak_file_fd, "r"); 
    size_t val = 0;
    char* lineptr = NULL;
    int datastructure_idx = 0; 
    while (getline(&lineptr, &val, leak_file_fp) != -1) {
        if (lineptr[strlen(lineptr) - 1] == '\n') {
            lineptr[strlen(lineptr) - 1] = NULL_CHAR;
        }
        char* lineptr_cpy = strdup(lineptr);
        char* word = strtok(lineptr_cpy," \n");
        while(word != NULL) {
            if (strstr(word, "make_queue") != NULL) {
                structures_in_code[datastructure_idx].std_structure = "queue"; 
            }
            else if (strstr(word, "make_stack") != NULL) {
                structures_in_code[datastructure_idx].std_structure = "stack"; 
            }
            else if (strstr(word, "make_vector") != NULL ) {
                structures_in_code[datastructure_idx].std_structure = "vector"; 
            }
            else if (strstr(word, "make_array") != NULL) {
                structures_in_code[datastructure_idx].std_structure = "array"; 
            }
            if (strstr(word, "make_queue") != NULL || strstr(word, "make_stack") != NULL 
            || strstr(word, "make_vector") != NULL || strstr(word, "make_array") != NULL) {
                structures_in_code[datastructure_idx].name = get_name_from_line(lineptr); 
                structures_in_code[datastructure_idx].capacity = atoi(get_arg(word)); 
                structures_in_code[datastructure_idx].mem_being_used = false;  
                structures_in_code[datastructure_idx].listed_structure = extract_listed_structure(word);  
                datastructure_idx++;
            }
            word = strtok(NULL, " \n");
        } 
        free(lineptr_cpy); 
    }
    free(lineptr); 
    close(leak_file_fd); 

    //link data structures in data structure array 
    leak_file_fd = open(leak_file, O_RDONLY, S_IRUSR);
    leak_file_fp = fdopen(leak_file_fd, "r"); 
    char word2[1024];
    while (fscanf(leak_file_fp, " %1023s", word2) == 1) {
        if (strstr(word2, "add_queue") != NULL || strstr(word2, "add_stack") != NULL
            || strstr(word2, "add_vector") != NULL || strstr(word2, "add_arrayr") != NULL) { 
           datastructure* ds = get_datastructure_from_name(get_first_arg(word2)); 
           datastructure* sub =  get_datastructure_from_name(get_second_arg(word2));
           ds -> containments[ds->num_containments] = sub; 
           ds->num_containments++; 
        }
     
    }
    close(leak_file_fd); 

    //mark datastructures as free or not
    leak_file_fd = open(leak_file, O_RDONLY, S_IRUSR);
    leak_file_fp = fdopen(leak_file_fd, "r"); 
    char word3[1024];
    while (fscanf(leak_file_fp, " %1023s", word3) == 1) {
        if (strstr(word3, "destroy_queue") != NULL || strstr(word3, "destroy_stack") != NULL
        || strstr(word3, "destroy_vector") != NULL || strstr(word3, "destroy_array") != NULL) {
            datastructure* ds =  get_datastructure_from_name(get_arg(word3)); 
            ds -> mem_being_used = false; 
        }
    }
    close(leak_file_fd); 

    //Print Summary
    print_test_visualization(); 
}

void print_test_visualization() {
    for(int i =0; i < 2; i++) {
        printf("\n");
    }
    printf("Visualization Summary:\n"); 
    char list[max_size * 1024];
    for (int i = 0; i < num_structures_created; i++) {
        printf("DataStructure{%s, %s, %s, ", structures_in_code[i].std_structure, structures_in_code[i].listed_structure, structures_in_code[i].name);
        printf("[");
        for (int j = 0; j < structures_in_code[i].num_containments; j++) {
            printf("%s", structures_in_code[i].containments[j]->name);
            if (j != structures_in_code[i].num_containments - 1) {
                printf(", ");
            }
        }
        printf("], ");
        printf("%zu, %d, %d}", structures_in_code[i].capacity, structures_in_code[i].num_containments, structures_in_code[i].mem_being_used);
        printf("\n");
    }
}

datastructure* get_datastructure_from_name(char* name) {
    for (int i = 0; i < num_structures_created; i++) {
        if (strcmp(structures_in_code[i].name, name) == 0) {
            return &structures_in_code[i]; 
        }
    }
    return NULL;
}

char * extract_listed_structure(char* word) {
    int idx = 0; 
    int num_dashes = 0; 
    char* buf = malloc(sizeof(word)); 
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == '(') {
            break; 
        }
        if (word[i] == '_') {
            num_dashes++; 
        }
        if (num_dashes == 2 && word[i] != '_') {
            buf[idx] = word[i]; 
            idx++; 
        }
    }
    buf[idx] = NULL_CHAR;
    // printf("extract_listed_structure: %s\n", buf); 
    return buf; 
}

//TODO: MIGHT BE ABLE TO ACCOMPLISH THIS USING AST
char* get_name_from_line(char* line) {  
    int idx = 0; 
    bool begin = false; 
    char* buf = malloc(sizeof(line)); 
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == '=') {
            break; 
        }
        if (begin == true && line[i] != ' ') {
            buf[idx] = line[i]; 
            idx++; 
        }
        if (line[i] == '*') {
            begin = true;
        }
    }
    buf[idx] = NULL_CHAR;
    // printf("get_name_from_line: %s\n", buf); 
    return buf; 
}

char* get_arg(char* word) {
    int idx = 0; 
    bool begin = false; 
    char* buf = malloc(sizeof(word)); 
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == ')') {
            break; 
        }
        if (begin == true) {
            buf[idx] = word[i]; 
            idx++; 
        }
        if (word[i] == '(') {
            begin = true; 
        }
    }
    buf[idx] = NULL_CHAR;
    // printf("ARG: %s\n", buf); 
    return buf; 
}

char* get_first_arg(char* word) {
    int idx = 0; 
    bool begin = false; 
    char* buf = malloc(sizeof(word)); 
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == ',') {
            break; 
        }
        if (begin == true && word[i] != ' ')  {
            buf[idx] = word[i]; 
            idx++; 
        }
        if (word[i] == '(') {
            begin = true; 
        }
    }
    buf[idx] = NULL_CHAR;
    // printf("FIRST ARG: %s\n", buf); 
    return buf; 
}

char* get_second_arg(char* word) {
    int idx = 0; 
    bool begin = false; 
    char* buf = malloc(sizeof(word)); 
    for (int i = 0; i < strlen(word); i++) {
        if (word[i] == ')') {
            break; 
        }
        if (begin == true && word[i] != ' ') {
            buf[idx] = word[i]; 
            idx++; 
        }
        if (word[i] == ',') {
            begin = true; 
        }
    }
    buf[idx] = NULL_CHAR;
    // printf("SECOND ARG: %s\n", buf); 
    return buf; 
}


