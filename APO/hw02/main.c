#define _POSIX_C_SOURCE 202001L

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char* argv[]){ 
    char *fname = argc > 1 ? argv[1] : NULL;

    int width = 0, height = 0, maxval;

    FILE *f = fopen(fname, "rb");
    char type[3];
    fscanf(f, "%s", type);
    fscanf(f, "%d %d", &width, &height);
    fscanf(f, "%d\n", &maxval);


    uint8_t buffer[3][width*3];
    uint8_t line_out[width*3];

    FILE *fout = fopen("output.ppm", "wb");
    fprintf(fout, "%s\n%d %d\n%d\n",type, width, height, maxval);

    int16_t pix[3][width];
    for (uint16_t i = 0; i < 2; i++){
        fread(buffer[i], sizeof(uint8_t), width*3, f);
    }
    int16_t k;
    int16_t res, res2, res3;
    int bin_size = 256/5;
    int bin_counts[5] = {0};
    for (uint16_t i = 0; i < height; i++){
        //Image Kernels
        for (uint16_t j = 0; j < width; j++){
            if (i>=1 && i<=height-2 && j>=1 && j<=width-2){
                res = ((buffer[1][j*3]) * 5) + 
                ((buffer[1][j*3-3]) * (-1)) +
                ((buffer[1][j*3+3]) * (-1)) +
                ((buffer[2][j*3]) * (-1)) +
                ((buffer[0][j*3]) * (-1));

                res2 = ((buffer[1][j*3+1]) * 5) +                            
                ((buffer[1][j*3-2]) * (-1)) +                              
                ((buffer[1][j*3+4]) * (-1)) +                              
                ((buffer[2][j*3+1]) * (-1)) +
                ((buffer[0][j*3+1]) * (-1));

                res3 = ((buffer[1][j*3+2]) * 5) + 
                ((buffer[1][j*3-1]) * (-1)) +
                ((buffer[1][j*3+5]) * (-1)) +
                ((buffer[2][j*3+2]) * (-1)) +
                ((buffer[0][j*3+2]) * (-1));
                
                res = res > maxval ? maxval : res;
                res = res < 0 ? 0 : res;
                res2 = res2 > maxval ? maxval : res2;
                res2 = res2 < 0 ? 0 : res2;
                res3 = res3 > maxval ? maxval : res3;
                res3 = res3 < 0 ? 0 : res3;

                k = 1;
                pix[1][j] = round((res*0.2126) + (res2*0.7152) + (res3*0.0722));
                line_out[j*3] = res;
                line_out[j*3+1] = res2;
                line_out[j*3+2] = res3;
            }
            else{
                k = i == 0 ? 0 : 1;
                pix[k][j] = round((buffer[k][j*3]*0.2126) + (buffer[k][j*3+1]*0.7152) + (buffer[k][j*3+2]*0.0722));
                line_out[j*3] = buffer[k][j*3];
                line_out[j*3+1] = buffer[k][j*3+1];
                line_out[j*3+2] = buffer[k][j*3+2];
            }
            int bin = pix[k][j] / bin_size;
            if (bin == 5){
                bin--;
            }
            bin_counts[bin]++;
        }
        fwrite(line_out, sizeof(unsigned char), width*3, fout);
        if (i <= height - 2){
            if (i >= 1) {
                memcpy(buffer[0], buffer[1], width*3);
                memcpy(buffer[1], buffer[2], width*3);
            }
            fread(buffer[2], sizeof(uint8_t), width*3, f);
        }
    }

    FILE *t = fopen("output.txt", "w");
    fprintf(t, "%d %d %d %d %d", bin_counts[0],bin_counts[1],bin_counts[2],bin_counts[3],bin_counts[4]);
    fclose(f);
    fclose(fout);
    fclose(t);
    return 0;
}

