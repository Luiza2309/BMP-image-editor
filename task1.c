#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define magic 100

#pragma pack(1)

typedef struct {
    unsigned char B;
    unsigned char G;
    unsigned char R;
} RGB;

typedef struct {
    unsigned char  fileMarker1;
    unsigned char  fileMarker2;
    unsigned int   bfSize;
    unsigned short unused1;
    unsigned short unused2;
    unsigned int   imageDataOffset;
} bmp_fileheader;

typedef struct {
    unsigned int   biSize;
    signed int     width;
    signed int     height;
    unsigned short planes;
    unsigned short bitPix;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} bmp_infoheader;

#pragma pack()

void edit(RGB*** matrix, char* input_filename,
 bmp_fileheader *header, bmp_infoheader *info, int *p) {
    // Deschide fisierul
    FILE *f = fopen(input_filename, "rb");
    if (f == NULL)
        fprintf(stderr, "Can't open %s", input_filename);

    // citeste header si info si muta cursorul peste offset
    fread(&(*header), sizeof(bmp_fileheader), 1, f);
    fread(&(*info), sizeof(bmp_infoheader), 1, f);
    fseek(f, (*header).imageDataOffset, SEEK_SET);

    // aloca spatiu matricei
    *matrix = malloc((*info).height * sizeof(RGB*));
    for (int i = 0; i < (*info).height; i++)
        (*matrix)[i] = malloc((*info).width * sizeof(RGB));

    // calculeaza paddingul
    if ( ((*info).width * 3) % 4 !=0 ) {
        *p = 4 - (3 * (*info).width) % 4;
    }

    // citeste matricea
    for (int i = 0; i <= (*info).height - 1; i++) {
        for (int j = 0; j < (*info).width; j++) {
            fread(&(*matrix)[i][j].B, 1, 1, f);
            fread(&(*matrix)[i][j].G, 1, 1, f);
            fread(&(*matrix)[i][j].R, 1, 1, f);
        }
        // muta cursorul peste padding
        fseek(f, (ftell(f) + (*p)), SEEK_SET);
    }
    fclose(f);
}

void save(RGB** matrix, char* output_filename,
 bmp_fileheader header, bmp_infoheader info, int p) {
    // deschide fisierul
    FILE *f = fopen(output_filename, "wb");

    // scrie headerul si info
    fwrite(&header, sizeof(bmp_fileheader), 1, f);
    fwrite(&info, sizeof(bmp_infoheader), 1, f);

    // scrie matricea
    for (int i = 0; i <= info.height - 1; i++) {
        for (int j = 0; j < info.width; j++) {
            fwrite(&matrix[i][j].B, 1, 1, f);
            fwrite(&matrix[i][j].G, 1, 1, f);
            fwrite(&matrix[i][j].R, 1, 1, f);
        }
        // adauga padding
        for (int n = 0; n < p; n++) {
            fputc(0x00, f);
        }
    }
}

void quit(RGB*** matrix, bmp_infoheader info) {
    for (int i = 0; i < info.height; i++)
        free((*matrix)[i]);
    free(*matrix);
}

void insert(RGB*** matrix, RGB** matrix1, bmp_infoheader info,
 int y, int x, char* insert_filename) {
    bmp_fileheader header1;
    bmp_infoheader info1;
    int p1 = 0;
    if (x > info.height || y > info.width) {
        return;
    } else {
        // citeste matricea de inserat
        edit(&matrix1, insert_filename, &header1, &info1, &p1);
        // parcurge matricea de inserat si o adauga in matricea initiala
        for (int i = 0; i < info1.height; i++) {
            for (int j = 0; j < info1.width; j++) {
                if (x + i < info.height && y + j < info.width) {
                    (*matrix)[x + i][y + j] = matrix1[i][j];
                }
            }
        }
    }
    // elibereaza memoria alocata pentru matricea de inserat
    quit(&matrix1, info1);
}

int max(int x1, int x2) {
    if (x1 > x2)
        return x1;
    else
        return x2;
}

int min(int x1, int x2) {
    if (x1 > x2)
        return x2;
    else
        return x1;
}

void draw_line(RGB*** matrix, bmp_infoheader info, int y1, int x1, int y2,
 int x2, int r, int g, int b, int w) {
    // calculeaza intervalul mai mare
    int x_max = max(x1, x2);
    int x_min = min(x1, x2);
    int y_max = max(y1, y2);
    int y_min = min(y1, y2);
    int i1 = 0;
    int i2 = 0;
    int interval_mare = 0;
    if (x_max - x_min > y_max - y_min) {
        i1 = x_min;
        i2 = x_max;
        interval_mare = 0;
    } else {
        i1 = y_min;
        i2 = y_max;
        interval_mare = 1;
    }

    // linie dreapta verticala
    if (x1 == x2) {
        for (int i = y_min; i <= y_max; i++) {
            for (int j = -w/2; j <= w/2; j++) {
                if (x1 + j >= 0 && x1 + j < info.height) {
                    for (int k = -w/2; k <= w/2; k++) {
                        if (i + k >= 0 && i + k < info.width) {
                            (*matrix)[x1 + j][i + k].R = r;
                            (*matrix)[x1 + j][i + k].G = g;
                            (*matrix)[x1 + j][i + k].B = b;
                        }
                    }
                }
            }
        }
    }

    // linie dreapta orizontala
    if (y1 == y2) {
        for (int i = x_min; i <= x_max; i++) {
            for (int j = -w/2; j <= w/2; j++) {
                if (j + y1 >= 0 && j + y1 < info.width) {
                    for (int k = -w/2; k <= w/2; k++) {
                        if (k + i >= 0 && k + i < info.height) {
                            (*matrix)[i + k][y1 + j].R = r;
                            (*matrix)[i + k][y1 + j].G = g;
                            (*matrix)[i + k][y1 + j].B = b;
                        }
                    }
                }
            }
        }
    }

    // linie oblica
    int aux = 0;
    for (int i = i1; i <= i2; i++) {
        if (interval_mare == 0) {
            aux = (i*(y2-y1)-x1*y2+y1*x2)/(x2-x1);
            for (int j = -w/2; j <= w/2; j++) {
                if (j + aux >= 0 && j + aux< info.width) {
                    for (int k = -w/2; k <= w/2; k++) {
                        if (k + i >= 0 && k + i < info.height) {
                            (*matrix)[i + k][aux + j].R = r;
                            (*matrix)[i + k][aux + j].G = g;
                            (*matrix)[i + k][aux + j].B = b;
                        }
                    }
                }
            }
        } else {
            aux = (i*(x2-x1)-y1*x2+x1*y2)/(y2-y1);
            for (int j = -w/2; j <= w/2; j++) {
                if (j + i >= 0 && j + i < info.width) {
                    for (int k = -w/2; k <= w/2; k++) {
                        if (k + aux >= 0 && k + aux < info.height) {
                            (*matrix)[aux + k][i + j].R = r;
                            (*matrix)[aux + k][i + j].G = g;
                            (*matrix)[aux + k][i + j].B = b;
                        }
                    }
                }
            }
        }
    }
}

void draw_rectangle(RGB*** matrix, bmp_infoheader info, int xd, int yd,
 int hd, int wd, int r, int g, int b, int w) {
    draw_line(&(*matrix), info, yd, xd, yd + hd, xd, r, g, b, w);
    draw_line(&(*matrix), info, yd, xd, yd, xd + wd, r, g, b, w);
    draw_line(&(*matrix), info, yd + hd, xd, yd + hd, xd + wd, r, g, b, w);
    draw_line(&(*matrix), info, yd, xd + wd, yd + hd, xd + wd, r, g, b, w);
}

void draw_triangle(RGB*** matrix, bmp_infoheader info, int yt1, int xt1,
 int yt2, int xt2, int yt3, int xt3, int r, int g, int b, int w) {
    draw_line(&(*matrix), info, yt1, xt1, yt2, xt2, r, g, b, w);
    draw_line(&(*matrix), info, yt1, xt1, yt3, xt3, r, g, b, w);
    draw_line(&(*matrix), info, yt3, xt3, yt2, xt2, r, g, b, w);
}

void fill(RGB*** matrix, int y, int x, int height, int width,
 RGB new_color, RGB old_color) {
    if (x >= 0 && x < height && y >= 0 && y < width) {
        if ((*matrix)[x][y].R == old_color.R &&
         (*matrix)[x][y].G == old_color.G &&
         (*matrix)[x][y].B == old_color.B) {
            (*matrix)[x][y].R = new_color.R;
            (*matrix)[x][y].G = new_color.G;
            (*matrix)[x][y].B = new_color.B;
            fill(&(*matrix), y - 1, x, height, width, new_color, old_color);
            fill(&(*matrix), y + 1, x, height, width, new_color, old_color);
            fill(&(*matrix), y, x + 1, height, width, new_color, old_color);
            fill(&(*matrix), y, x - 1, height, width, new_color, old_color);
        }
    }
}

int main() {
    RGB** matrix = NULL;
    RGB** matrix1 = NULL;
    bmp_infoheader info;
    bmp_fileheader header;
    int p = 0;  // padding
    char *v = malloc(magic * sizeof(char));  // cuvant
    char *path_input = malloc(magic * sizeof(char));
    char *path_output = malloc(magic * sizeof(char));
    char *insert_filename = malloc(magic * sizeof(char));
    int y = 0, x = 0;
    int r = 0, g = 0, b = 0;  // culorile
    int y1 = 0, x1 = 0, y2 = 0, x2 = 0;
    int w = 0;  // grosimea
    int xd = 0, yd = 0, hd = 0, wd = 0;  // coordonate dreptunghi
    int yt1 = 0, xt1 = 0, yt2 = 0, xt2 = 0, yt3 = 0, xt3 = 0;
    // coordonate triunghi
    int m = 0, n = 0;
    int height = 0, width = 0;
    RGB old_color;
    RGB new_color;

    while (scanf("%s", v)) {
        if (strcmp(v, "edit") == 0) {
            scanf("%s", path_input);
            edit(&matrix, path_input, &header, &info, &p);
            height = info.height;
            width = info.width;
        }
        if (strcmp(v, "save") == 0) {
            scanf("%s", path_output);
            save(matrix, path_output, header, info, p);
        }
        if (strcmp(v, "quit") == 0) {
            quit(&matrix, info);
            return 0;
        }
        if (strcmp(v, "insert") == 0) {
            scanf("%s", insert_filename);
            scanf("%d %d", &y, &x);
            insert(&matrix, matrix1, info, y, x, insert_filename);
        }
        if (strcmp(v, "draw_color") == 0) {
            scanf("%d %d %d", &r, &g, &b);
        }
        if (strcmp(v, "line_width") == 0) {
            scanf("%d", &w);
        }
        if (strcmp(v, "line") == 0) {
            scanf("%d %d %d %d", &y1, &x1, &y2, &x2);
            draw_line(&matrix, info, y1, x1, y2, x2, r, g, b, w);
        }
        if (strcmp(v, "rectangle") == 0) {
            scanf("%d %d %d %d", &yd, &xd, &hd, &wd);
            draw_rectangle(&matrix, info, xd, yd, hd, wd, r, g, b, w);
        }
        if (strcmp(v, "triangle") == 0) {
            scanf("%d %d %d %d %d %d", &yt1, &xt1, &yt2, &xt2, &yt3, &xt3);
            draw_triangle(&matrix, info, yt1, xt1, yt2, xt2, yt3, xt3,
             r, g, b, w);
        }
        if (strcmp(v, "fill") == 0) {
            scanf("%d %d", &n, &m);
            old_color.R = matrix[m][n].R;
            old_color.G = matrix[m][n].G;
            old_color.B = matrix[m][n].B;
            new_color.R = r;
            new_color.G = g;
            new_color.B = b;
            fill(&matrix, n, m, height, width, new_color, old_color);
        }
    }

    free(v);
    free(path_input);
    free(path_output);
    free(insert_filename);
    return 0;
}
