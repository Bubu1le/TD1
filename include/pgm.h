#include<stdio.h>
#include<stdlib.h>

struct pgm {
	int height, width, max_value;
	unsigned char **pixels;
};

struct pgm* pgm_alloc(int h, int w, int max);

void pgm_free(struct pgm * P2);

void supprimer_ligne(FILE* fichier);

struct pgm* pgm_read_asc(char *fname);

int pgm_write_asc(char *fname, struct pgm *P2);

struct pgm* pgm_read_bin(char *fname);

int pgm_write_bin(char *fanme, struct pgm *P2);

void pgm_negative(struct pgm *src, struct pgm **dst);

void pgm_extract(char *fname, struct pgm *pgm_t, int dx, int dy, int width, int height);

short* pgm_get_histogram(struct pgm *P2);

void pgm_write_histogram(char *fname, struct pgm * P2);

void lecture_image();

void ecriture_image();

void lecture_binaire();

void ecriture_binaire();
