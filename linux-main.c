#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int convert(const char* f_in_name, const char* f_out_name);

int main(int argc, char* argv[]) {
	if(argc < 3) {
		fprintf(stderr, "Usage: %s in.obj out.ply\n", argv[0]);
		return 1;
	}

	return convert(argv[1], argv[2]);
}
