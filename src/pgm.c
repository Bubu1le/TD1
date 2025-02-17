#include"pgm.h"

struct pgm* pgm_alloc(int h, int w, int max) {
	struct pgm *P2 = (struct pgm*)malloc(sizeof(struct pgm));
	
	P2->height = h;
	P2->width = w;
	P2->max_value = max;
	
	P2->pixels = (unsigned char**)malloc(h*sizeof(unsigned char*));
	for(int i = 0; i < h; i++)
		P2->pixels[i] = (unsigned char*)malloc(w*sizeof(unsigned char));
		
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
			P2->pixels[i][j] = max;
			
	return P2;
}

void pgm_free(struct pgm * P2) {
	for(int i = 0; i < P2->height; i++)
		free(P2->pixels[i]);
	free(P2->pixels);
	free(P2);
}

void supprimer_ligne(FILE* fichier) {
	char car;
	for(int i = 0; i < 2; i++) {
		fscanf(fichier, "%c", &car);
		while(car != '\n')
			fscanf(fichier, "%c", &car);
	}
}

struct pgm* pgm_read_asc(char *fname) {
	FILE* fichier;
	fichier = fopen(fname, "r");
	int h, w, max;
	
	supprimer_ligne(fichier);
	
	fscanf(fichier, "%d", &w);
	fscanf(fichier, "%d", &h);
	fscanf(fichier, "%d", &max);
	struct pgm *P2 = pgm_alloc(h, w, max);
	
	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
			fscanf(fichier, "%hhu", &P2->pixels[i][j]);
			
	fclose(fichier);
	return P2;
}

int pgm_write_asc(char *fname, struct pgm *P2) {
	FILE* fichier = fopen(fname, "w");
	if(fichier != NULL) { 
		fprintf(fichier, "%s", "P2 \n");
		fprintf(fichier, "%d %d \n", P2->width, P2->height);
		fprintf(fichier, "%d \n", P2->max_value);
		for(int i = 0; i < P2->height; i++)
			for(int j = 0; j < P2->width; j++)
				fprintf(fichier, "%hhu \n", P2->pixels[i][j]);
		fclose(fichier);
		return 0;
	}	
	return 1;
}

struct pgm* pgm_read_bin(char *fname) {
	FILE* fichier = fopen(fname, "rb");
	int h, w, max;
	
	supprimer_ligne(fichier);
	
	fscanf(fichier, "%d", &w);
	fscanf(fichier, "%d", &h);
	fscanf(fichier, "%d", &max);
	struct pgm *P5 = pgm_alloc(h, w, max);
	
	if(fichier != NULL) {
		fread(&P5->pixels[0][0], sizeof(unsigned char), 1, fichier);
		for(int i = 0; i < P5->height; i++)
			for(int j = 0; j < P5->width; j++)
				fread(&P5->pixels[i][j], sizeof(unsigned char), 1, fichier);
		fclose(fichier);
	}
	return P5;
}

int pgm_write_bin(char *fname, struct pgm *P6) {
	FILE* fichier = fopen(fname, "wb");
	if(fichier != NULL) {
		fprintf(fichier, "%s", "P6 \n");
		fprintf(fichier, "%d %d \n", P6->width, P6->height);
		fprintf(fichier, "%d \n", P6->max_value);
		for(int i = 0; i < P6->height; i++)
			for(int j = 0; j < P6->width; j++)
				fwrite(&P6->pixels[i][j], sizeof(unsigned char), 1, fichier);
		fclose(fichier);
		return 0;
	}
	return 1;
}

void pgm_negative(struct pgm *src, struct pgm **dst) {
	for(int i = 0; i < src->height; i++)
		for(int j = 0; i < src->width; j++)
			(*dst)->pixels[i][j] = src->max_value - src->pixels[i][j];
}

void pgm_extract(char *fname, struct pgm *pgm_t, int dx, int dy, int width, int height) {
	struct pgm *dst = pgm_alloc(height, width, pgm_t->max_value);
	
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			dst->pixels[i][j] = pgm_t->pixels[i + dx][j + dy];
	int res = pgm_write_asc(fname, dst);
	if(res == 0)
		printf("Aucun problème !");
}

short* pgm_get_histogram(struct pgm *P2) {
	short *histogram = (short*)malloc((P2->max_value+1)*sizeof(short));
	for(int i = 0; i < P2->max_value + 1; i++)
		histogram[i] = 0;
	
	for(int i = 0; i < P2->height; i++)
		for(int j = 0; j < P2->width; j++)
			histogram[P2->pixels[i][j]] += 1;
	return histogram;
}

void pgm_write_histogram(char *fname, struct pgm * P2) {
	FILE *fichier = fopen(fname, "r");
	short *Histogram = NULL;
	Histogram = pgm_get_histogram(P2);
	
	if(fichier != NULL)
		for(int i = 0; i < P2->max_value + 1; i++)
			fprintf(fichier, "%d %d \n", i, Histogram[i]);
	fclose(fichier);
	free(Histogram);
}

// ---------------------------------------------- Execution ------------------------------------------------------
void lecture_image() {
	struct pgm *P2 = pgm_read_asc("./src/eye_s_asc.pgm");
	printf("H : %d, W : %d, Max = %d, val = %hhu \n", P2->height, P2->width, P2->max_value, P2->pixels[0][0]);
	pgm_free(P2);
}

void ecriture_image() {
	struct pgm *P2 = pgm_alloc(4, 4, 12);
	int res = pgm_write_asc("./bin/Test.pgm", P2);
	printf("Res : %d \n", res);
	pgm_free(P2);
}

void lecture_binaire() {
	struct pgm *P2 = pgm_read_bin("./src/eye_s_bin.pgm");
	printf("H : %d, W : %d, Max = %d, val = %hhu \n", P2->height, P2->width, P2->max_value, P2->pixels[0][0]);
	for(int i = 0; i < 5; i++)
		printf("Pixels[0][%d] = %hhu \n", i, P2->pixels[0][i]);
	printf("Pixels[%d][%d] = %hhu \n", P2->height, P2->width, P2->pixels[P2->height-1][P2->width-1]);
}

void ecriture_binaire() {
	struct pgm *P2 = pgm_alloc(4, 4, 12);
	int res2 = pgm_write_bin("./bin/Test.bin", P2);
	printf("Res 2 : %d \n", res2);
	free(P2);
}

int main() {
	//lecture_image();
	//ecriture_image();
	lecture_binaire();
	//ecriture_binaire();
	return 0;
}
