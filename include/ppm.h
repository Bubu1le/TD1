#include<stdio.h>
#include<stdlib.h>

typedef struct rgb {
	unsigned char r, g, b;
} rgb_t;

struct ppm {
	int height, width, max_value;
	rbg_t ** pixels;
};

struct ppm* ppm_alloc(int h, int w, int max);

void ppm_free(struct ppm P5);

struct ppm* ppm_read_asc(char *fname);

int ppm_write_asc(char *fname, struct ppm *P5);

struct ppm* ppm_read_bin(char *fname);

int ppm_write_bin(char *fname, struct ppm *P5);

void ppm_negative(struct ppm *scr, struct ppm **dst);

void ppm_extract(char *fname, struct ppm *ppm_t, int dx, int dy, int height, int width);

short** ppm_get_histogram(struct ppm *P3);

void ppm_write_histogram(struct ppm *P3, char *fname);

void ppm_to_pgm(struct ppm *P3, struct pgm *P2);

