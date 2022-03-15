#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

// inc matrix graph structure
typedef struct{
	bool** inc_matrix; 
	uint64_t cw;
	uint64_t ch;
} graph;

//initialization graph structure (memory allocating for graph structure)
graph *init_graph(uint64_t cw, uint64_t ch) {
	graph *new_graph = (graph*) malloc(sizeof(graph));
	new_graph->inc_matrix = (bool**) malloc(ch * sizeof(bool*));
	for (uint64_t i = 0; i < ch; ++i){
		new_graph->inc_matrix[i] = (bool*) calloc(cw, sizeof(bool));
	}
	new_graph->cw = cw;
	new_graph->ch = ch;
	return new_graph;
}

//Clean allocated RAM for graph structure
void del_graph(graph* cur_graph){
	for (uint64_t i = 0; i < cur_graph->ch; ++i) {
		free(cur_graph->inc_matrix[i]);
	}
	free(cur_graph->inc_matrix);
	free(cur_graph);
}

//
uint64_t count_width(FILE* file){
	uint64_t count = 0;
	char ch;
	do {
		fscanf(file, "%c", &ch);
		if (ch == ' ') count++;
	} while (ch != '\n');
	fclose(file);
	return (count);
}

//
uint64_t count_height(FILE* file){
	uint64_t count = 0;
	char ch;
	while (! feof(file))
    {
        if (fgetc(file) == '\n')
            count++;
    }
	fclose(file);
	return (count);
}

//Reading inc matrix from file to graph structure
void fread_graph(graph* cur_graph, FILE* file) {
	uint8_t buffer;
	for (uint64_t i = 0; i < cur_graph->ch; ++i) {
		for (uint64_t j = 0; j < cur_graph->cw; ++j) {
			fscanf(file, "%" SCNu8 "", &buffer);
            if (buffer) cur_graph->inc_matrix[i][j] = true;
		}
	}
	fclose(file);
}

//Visualization graph in DOT(Graphviz) format from graph struct
void visual_graph(graph* cur_graph, FILE* file) {
	fputs("graph graphname{\n", file);
	
	for(uint64_t i = 0; i < cur_graph->ch; ++i) {
		fprintf(file, "\t%c;\n", 'a' + i);
	}
	
	int i1,i2;
	for (uint64_t i = 0; i < cur_graph->cw; ++i) {
		i1=i2=-1;
		for (uint64_t j = 0; j < cur_graph->ch; ++j){
			if ((cur_graph->inc_matrix[j][i]==1)) {
				if (i1 == -1) i1=i2=j;
				else i2=j;
			}
		}
		fprintf(file, "\t%c -- %c;\n", 'a' + i1, 'a' + i2);
	}
	fputs("}", file);
	fclose(file);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		puts("Error: wrong argunets!\n");
		return -1;
	}  
	
	graph* cur_graph = init_graph(count_width(fopen(argv[1], "r")), count_height(fopen(argv[1], "r")));
	fread_graph(cur_graph, fopen(argv[1], "r"));
	visual_graph(cur_graph, fopen(argv[2], "w"));
	
	del_graph(cur_graph);
	return 0;
}