#include "mcpl.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int hits_box(const mcpl_particle_t* p, const double distance, const double width, const double height){
	/*
	 * A massless particle (or a massive particle in a gravity-free environment) hits a 2D box a distance
	 * L away if and only if it is inside the box after being projected into the plane of the box
	 * */
	double t = (distance - p->position[2]) / p->direction[2];
	double a = 2 * fabs(p->position[0] + t * p->direction[0]);
	double b = 2 * fabs(p->position[1] + t * p->direction[1]);
	int ans = (a < width && b < height) ? 1 : 0;
	return ans;
}

int main(int argc, char** argv){
	if (argc < 5){
		printf("usage: %s [input MCPL file] [target distance] [target width] [target height] {[output MCPL file]}", argv[0]);
		return 1;
	}

	const char * infilename = argv[1];
	const double distance = atof(argv[2]);
	const double width = atof(argv[3]);
	const double height = atof(argv[4]);

	char * outfilename;
	if (argc < 6){
		outfilename = (char *)calloc(10+ strlen(infilename), sizeof(char));
		sprintf(outfilename, "filtered_%s", infilename);
	} else {
		outfilename = argv[5];
	}

	// check whether the output filename ends in .mcpl or .mcpl.gz:
	int want_gzip = 0;
	char * lastdot = strrchr(outfilename, '.');
	if (strcmp(lastdot, ".gz") == 0){
		want_gzip = 1;
		*lastdot = '\0'; // terminate the filename here
	}

	printf("Write filtered file to %s\n", outfilename);

	mcpl_file_t fi = mcpl_open_file(infilename);
	mcpl_outfile_t fo = mcpl_create_outfile(outfilename);
	mcpl_transfer_metadata(fi, fo);
	char * comment = (char *) calloc(1024, sizeof(char));
	sprintf(comment, "Applied custom filter to select neutron which hit a (%f by %f) box %f along Z", width, height, distance);
	mcpl_hdr_add_comment(fo, comment);

	const mcpl_particle_t* particle;
	while ((particle = mcpl_read(fi))) {
		if (particle->pdgcode == 2112 && hits_box(particle, distance, width, height)) {
			mcpl_add_particle(fo, particle);
		}
	}

	want_gzip ? mcpl_closeandgzip_outfile(fo) : mcpl_close_outfile(fo);
	mcpl_close_file(fi);
}

