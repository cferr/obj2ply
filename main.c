#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if(argc < 3) {
		fprintf(stderr, "Usage: %s in.obj out.ply\n", argv[0]);
	}

	FILE* f_in = fopen(argv[1], "r");
	if(!f_in)
		return 1;

	FILE* f_out = fopen(argv[2], "w");
	if(!f_out)
		return 1;

	char* line = NULL;
	size_t n_line = 0;

	// We assume the file is: all vertices, all normals, then all faces
	int is_exp;

	unsigned long long nb_vertices = 0;
	unsigned long long nb_faces = 0;

	long faces_start_pos = 0;

	// Count vertices
	is_exp = 1;
	do {
		getline(&line, &n_line, f_in);
		if(line[0] != 'v' || line[1] != ' ')
			is_exp = 0;
		else
			nb_vertices += 1;
	} while(is_exp != 0 && !feof(f_in));

	if(feof(f_in))
		return 1;

	// Go to faces
	is_exp = 1;
	do {
		getline(&line, &n_line, f_in);
		if(!(line[0] != 'f' || line[1] != ' '))
			is_exp = 0;
	} while(is_exp != 0 && !feof(f_in));

	if(feof(f_in))
		return 1;

	// Rewind until previous line
	do {
		// -2 seek because fgetc will consume one character
		// Start: | ?? | PP | CC |
		//                    ^^
		// After seek: | ?? | PP | CC |
		//               ^^
		// After fgetc: | ?? | PP | CC |
		//                     ^^
		// After this loop, the seek is at the first character of the
		// current line
		fseek(f_in, -2, SEEK_CUR);
	} while(fgetc(f_in) != '\n');

	faces_start_pos = ftell(f_in);

	// Count faces
	is_exp = 1;
	do {
		getline(&line, &n_line, f_in);
		if(feof(f_in) || (line[0] != 'f' || line[1] != ' '))
			is_exp = 0;
		else
			nb_faces += 1;
	} while(is_exp != 0);

	// Reset
	fseek(f_in, 0, SEEK_SET);

	// Echo header
	fprintf(f_out, "ply\n\
format ascii 1.0\n\
comment Automatically gnerated by model-converter\n\
element vertex %lld\n\
property float x\n\
property float y\n\
property float z\n\
element face %lld\n\
property list uchar int vertex_indices\n\
end_header\n", nb_vertices, nb_faces);


	// Process vertex lines -> literal translation
	for(unsigned long long i = 0; i < nb_vertices; ++i) {
		// Get one line...
		ssize_t line_len = getline(&line, &n_line, f_in);
		// Cut the first two characters...
		fwrite(line + 2, line_len - 2, 1, f_out);
	}

	// Process face lines -> subtract 1 to everyone
	// Go to start of faces
	fseek(f_in, faces_start_pos, SEEK_SET);
	for(unsigned long long i = 0; i < nb_faces; ++i) {
		// Get one line...
		getline(&line, &n_line, f_in);

		// Process the line...
		unsigned long long p1, p2, p3;
		sscanf(line, "f %lld %lld %lld", &p1, &p2, &p3);
		fprintf(f_out, "3 %lld %lld %lld\n", p1-1, p2-1, p3-1);
	}

	fclose(f_in);
	fclose(f_out);

	return 0;
}
