#include "ppm.h"
#include "pgm.h"

struct ppm* ppm_alloc(int h, int w, int max) {
	struct ppm *P3 = (struct ppm*)malloc(sizeof(struct ppm));
	
	P3->height = h;
	P3->width = w;
	P3->max_value = max;
	P3->pixels = (rgb_t**)malloc(h*sizeof(rgb_t*));
	for(int i = 0; i < h; i++)
		P3->pixels[i] = (rgb_t*)malloc(w*sizeof(rgb_t));
	
	for(int i = 0; i < h; i++) {
		for(int j = 0; j < w; j++) {
			P3->pixels[i][j].r = max;
			P3->pixels[i][j].g = max;
			P3->pixels[i][j].b = max;
		}
	}
	return P3;
}

void ppm_free(struct ppm *P3) {
	for(int i = 0; i < P3->height; i++)
		free(P3->pixels[i]);
	free(P3->pixels);
	free(P3);
}

void supprimer_ligne(FILE* fichier) {
	char car;
	for(int i = 0; i < 2; i++) {
		fscanf(fichier, "%c", &car);
		while(car != '\n')
			fscanf(fichier, "%c", &car);
	}
}

struct ppm* ppm_read_asc(char *fname) {
	FILE *fichier = fopen(fname, "r");
	int h, w, max;
	
	supprimer_ligne(fichier);
	
	fscanf(fichier, "%d", &w);
	fscanf(fichier, "%d", &h);
	fscanf(fichier, "%d", &max);
	struct pgm *P3 = ppm_alloc(h, w, max);
	
	for(int i = 0; i < h; i++) {
		for(int j = 0; j < w; j++) {
			fscanf(fichier, "%hhu", &P3->pixels[i][j].r);
			fscanf(fichier, "%hhu", &P3->pixels[i][j].g);
			fscanf(fichier, "%hhu", &P3->pixels[i][j].b);
		}
	}
	fclose(fichier);
	return P3;
}

int ppm_write_asc(char *fname, struct ppm *P3) {
	FILE *fichier = fopen(fname, "w");
	
	if(fichier != NULL) { 
		fprintf(fichier, "%s", "P3 \n");
		fprintf(fichier, "%d %d \n", P3->width, P3->height);
		fprintf(fichier, "%d \n", P3->max_value);
		for(int i = 0; i < P3->height; i++) {
			for(int j = 0; j < P3->width; j++) {
				fprintf(fichier, "%hhu \n", P3->pixels[i][j].r);
				fprintf(fichier, "%hhu \n", P3->pixels[i][j].g);
				fprintf(fichier, "%hhu \n", P3->pixels[i][j].b);
			}
		}
		fclose(fichier);
		return 0;
		
	}
	return 1;
}

struct ppm* ppm_read_bin(char *fname) {
	FILE* fichier = fopen(fname, "rb");
	int h, w, max;
	
	supprimer_ligne(fichier);
	
	fscanf(fichier, "%d", &w);
	fscanf(fichier, "%d", &h);
	fscanf(fichier, "%d", &max);
	struct pgm *P6 = pgm_alloc(h, w, max);
	
	if(fichier != NULL) {
		fread(&P6->pixels[0][0], sizeof(unsigned char), 1, fichier);
		for(int i = 0; i < P6->height; i++) {
			for(int j = 0; j < P6->width; j++) {
				fread(&P6->pixels[i][j].r, sizeof(unsigned char), 1, fichier);
				fread(&P6->pixels[i][j].g, sizeof(unsigned char), 1, fichier);
				fread(&P6->pixels[i][j].b, sizeof(unsigned char), 1, fichier);
			}
		}
		fclose(fichier);
	
	}
	return P6;
}

int ppm_write_bin(char *fname, struct ppm *P3) {
	FILE *fichier = fopen(fname, "wb");
	
	if(fichier != NULL) {
		fprintf(fichier, "%s", "P6 \n");
		fprintf(fichier, "%d %d \n", P3->width, P3->height);
		fprintf(fichier, "%d \n", P3->max_value);
		for(int i = 0; i < P3->height; i++) {
			for(int j = 0; j < P3->width; j++) {
				fwrite(&P3->pixels[i][j].r, sizeof(unsigned char), 1, fichier);
				fwrite(&P3->pixels[i][j].g, sizeof(unsigned char), 1, fichier);
				fwrite(&P3->pixels[i][j].b, sizeof(unsigned char), 1, fichier);
			}
		}
		fclose(fichier);
		return 0;
	}
	return 1;
}

void ppm_negative(struct ppm *scr, struct ppm **dst) {
	for(int i = 0; i < scr->height; i++) {
		for(int j = 0; j < scr->width; j++) {
			dst->pixels[i][j].r = scr->max_value - scr->pixels[i][j].r;
			dst->pixels[i][j].g = scr->max_value - scr->pixels[i][j].g;
			dst->pixels[i][j].b = scr->max_value - scr->pixels[i][j].b;
		}
	}
}

void ppm_extract(char *fname, struct ppm *ppm_t, int dx, int dy, int height, int width) {
	struct ppm *dst = ppm_alloc(height, width, ppm_t->max_value);
	
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			dst->pixels[i][j].r = ppm_t->pixels[i + dx][j + dy].r;
			dst->pixels[i][j].g = ppm_t->pixels[i + dx][j + dy].g;
			dst->pixels[i][j].b = ppm_t->pixels[i + dx][j + dy].b;
		}
	}
	int res = ppm_write_asc(fname, dst);
	if(res == 0)
		printf("Aucun problème !");
}

short** ppm_get_histogram(struct ppm *P3) {
	short **histogram = (short**)malloc(3*sizeof(short*));
	for(int i = 0; i < 3; i++) {
		histogram[i] = (short*)malloc((P3->max_value + 1)*sizeof(short));
		for(int j = 0; j < P3->max_value + 1; j++)
			histogram[i][j] = 0;
	}
	
	for(int i = 0; i < P3->height; i++) {
		for(int j = 0; j < P3->width; j++) {
			histogram[0][P3->pixels[i][j].r] += 1;
			histogram[1][P3->pixels[i][j].g] += 1;
			histogram[2][P3->pixels[i][j].b] += 1;
		}
	}
	return histogram;
}

void ppm_write_histogram(struct ppm *P3, char *fname) {
	FILE *fichier = fopen(fname, "w");
	short **Histogram = ppm_get_histogram(P3);
	
	if(fichier != NULL) {
		for(int i = 0; i < P3->max_value + 1; i++)	
			fprintf(fichier, "%d %d %d %d\n", i, Histogram[0][i], Histogram[1][i], Histogram[2][i]);
		fclose(fichier);
	}
	for(int i = 0; i < 3; i++)
		free(Histogram[i]);
	free(Histogram);
}

void ppm_to_pgm(struct ppm *P3, struct pgm *P2) {
	// 0.299 * r + 0.587 * g + 0.114 * b
	for(int i = 0; i < P3->height; i++)
		for(int j = 0; j < P3->height; j++)
			P2->pixels[i][j] = 0.299 * P3->pixels[i][j].r + 0.587 * P3->pixels[i][j].g + 0.114 * P3->pixels[i][j].b;
}
