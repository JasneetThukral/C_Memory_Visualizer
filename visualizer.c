#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//typedef int bool;
//Move this struct from main.c into a new file. Then include that new file into both main.c and the visualization file so that you can use the struct in both files. 
typedef struct datastructure {
    char* std_structure; 
    char* listed_structure; 
    char* name; 
    struct datastructure** containments; //array of pointers to data structures
    size_t capacity; 
    int mem_being_used;
	int num_containments;
} datastructure;

void writeHead(FILE * output) {
	fprintf(output, "<!DOCTYPE HTML>\n<html>\n<head>\n<meta charset='utf-8'/>\n<title>Datastructure result</title>\n</head>\n");
}

void colormap(char * input, char * output) {
	if (input == NULL) {
		strcpy(output, "black");
		return;
	}
	
	if (strcmp(input, "stack")) {
		strcpy(output, "magenta");
	} else if (strcmp(input, "queue")) {
		strcpy(output, "green");
	} else if (strcmp(input, "vector")) {
		strcpy(output, "yellow");
	} else if (strcmp(input, "array")) {
		strcpy(output, "pink");
	} else {
		strcpy(output, "black");
	}
}

void edgecolormap(int input, char * output) {
	if (!input) {
		strcpy(output, "blue");
	} else {
		strcpy(output, "red");
	}
}

void writeSVG(FILE * , datastructure*, char *, int);

void visualizer(FILE * out, char * stroutput, datastructure * ds, char * parent, int count) {
	size_t offset = 0;
	for (int i = 0; i < count; i++) {
		char * fillcolor = malloc(20);
		char * edgecolor = malloc(20);
		colormap(ds[i].std_structure, fillcolor);
		edgecolormap(ds[i].mem_being_used, edgecolor);
		
		sprintf(stroutput + offset, "<rect x = %d width = 100 height = 100 style='fill:%s;stroke-width:4;stroke:%s;'></rect>", i * 100, fillcolor, edgecolor);
		
		free(edgecolor);
		free(fillcolor);
		offset = strlen(stroutput);
		sprintf(stroutput + offset, "<text x = %d y = 10 fill = 'white'>%s</text>", 100*i+10, ds[i].name);
		offset = strlen(stroutput);
		sprintf(stroutput + offset, "<text x = %d y = 30 fill = 'white'>Size: %zu</text>", 100*i+10, ds[i].capacity);
		offset = strlen(stroutput);
		if (parent != NULL) {
			sprintf(stroutput + offset, "<text x = %d y = 50 fill = 'white'>Parent: %s</text>", 100*i+10, parent);
			offset = strlen(stroutput);
		}
		
		if (ds[i].num_containments > 0) {
			writeSVG(out, *(ds[i].containments), ds[i].name, ds[i].num_containments);
		}
	}
}

void writeSVG(FILE * output, datastructure * ds, char * parent, int count) {
	char * info = malloc(4096); // String buffer max size, increase if needed
	visualizer(output, info, ds, parent, count);
	fprintf(output, "<svg width = 1980 height = 100>%s</svg>", info);
	free(info);
}

void writeSummary(FILE * output) {
	fprintf(output, "<p>Red border means unfreed memory, Blue border means freed memory</p>");
	fprintf(output, "<p><ul><li>Stack - Magenta</li><li>Queue - Green</li><li>Vector - Yellow</li><li>Array - Pink</li><li>Other - Black</li></ul></p>");
}