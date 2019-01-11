// Copies a BMP file

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize fraction infile outfile\n");
        return 1;
    }

    // remember filenames
    float scale   = atof( argv[1] );
    char *infile  = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // remember how long the picture used to be.
    int org_biWidth = bi.biWidth;
    int org_biHeight = bi.biHeight;

    bi.biWidth  = (LONG) floor(scale * bi.biWidth);
    bi.biHeight = (LONG) floor(scale * bi.biHeight);

    // determine padding for scanlines
    int padding = (4 - (org_biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_new = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding_new) * abs(bi.biHeight);
    bf.bfSize = 14 + 40 + ((sizeof(RGBTRIPLE) * bi.biWidth) + padding_new) * abs(bi.biHeight);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // create a imageHolder (iH) to store the image in memory
    RGBTRIPLE iH[abs(org_biHeight)][org_biWidth];

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(org_biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < org_biWidth; j++)
        {
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            iH[i][j] = triple;
        }
        fseek(inptr, padding, SEEK_CUR);
    }

    // for every pixel in the new image
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        int ii = (int) floor(i/scale);
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            int jj = (int) floor(j/scale);

            triple.rgbtBlue  = iH[ ii ][ jj ].rgbtBlue;
            triple.rgbtGreen = iH[ ii ][ jj ].rgbtGreen;
            triple.rgbtRed   = iH[ ii ][ jj ].rgbtRed;

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        //

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding_new; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
