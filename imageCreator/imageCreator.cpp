// copies a BMP file

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <complex>

#include "bmp.h"

using namespace std;

RGBTRIPLE mandel(int i, int j, int maxH, int maxW);
RGBTRIPLE redcircle(int i, int j, int maxH, int maxW);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4 && argc != 5)
    {
      cout << argc;
        fprintf(stderr, "Usage: image outfile width height [motif]\n");
        return 1;
    }
    string motif = "redcircle";
    if (argc == 5) {
      motif = argv[4];
      //cout<<"unknown motif";
    }
    // remember filenames
    char *outfile = argv[1];

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        //fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // create BITMAPFILEHEADER and BITMAPINFOHEADER
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    
    bf.bfType = 0x4d42;
    bf.bfSize;
    bf.bfOffBits = 54;
    
    bi.biSize = 40;
    bi.biWidth  = 4*atoi(argv[2]);
    bi.biHeight = 4*atoi(argv[3]);
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 4;
    bi.biSizeImage; // defined below
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed       = 0;
    bi.biClrImportant  = 0;


    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    bf.bfSize = 14 + 40 + ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over outfile's scanlines
    for (int i = 0; i < abs(bi.biHeight); i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
	  RGBTRIPLE triple;
            // temporary storage
	  if (motif == "redcircle") {
	    triple = redcircle(i,j,bi.biHeight,bi.biWidth);
	  } else if (motif == "mandel") {
	    triple =    mandel(i,j,bi.biHeight,bi.biWidth);
	  }
	  else {
	    cout<<"unknown motif";
	    return 1;
	  }
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // add padding, if any.
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
RGBTRIPLE mandel(int i, int j, int maxH, int maxW) {
  using namespace std::complex_literals;
  int MAXK = 200;
  RGBTRIPLE triple;
  
  if ( std::pow( std::pow(i,2) + std::pow(j,2) ,(1/2.0)) > 2) {
    triple.rgbtBlue  = 0x00;
    triple.rgbtGreen = 0x00;
    triple.rgbtRed   = 0x00;
  }
  if( std::pow( std::pow(i+1,2) + std::pow(j,2) ,(1/2.0) ) <0.2 ){
    triple.rgbtBlue  = 0x00;
    triple.rgbtGreen = 0x00;
    triple.rgbtRed   = 0x00;
  }
  
  if (i<0 && i>-0.5 && j>-0.4 && j<0.4){
    triple.rgbtBlue  = 0x00;
    triple.rgbtGreen = 0x00;
    triple.rgbtRed   = 0x00;
  }
  /*
  float XT = 0.5;
  float YT = 1.5;
  float XB = -2;
  float YB = -1.5;
  */
  float XT = 0.28165;
  float YT = 0.01085;
  float XB = 0.27707;
  float YB = 0.00627;

  float j_m = ( (XT-XB) / maxW) *j + XB;
  float i_m = ( (YT-YB) / maxH) *i + YB;
  
  std::complex<double> z = 0;
  std::complex<double> t;
  t.real(j_m);
  t.imag(i_m);
  //std::cout<<maxW<<" "<<i<<" "<<t<<endl;
  triple.rgbtBlue  = 0x00;
  triple.rgbtGreen = 0x00;
  triple.rgbtRed   = 0x00;
  for ( int ii = 0; ii < MAXK; ii++) {
    z = pow(z,2) + t;

    if (std::abs(z) > 2) {
      triple.rgbtBlue  = min( (ii<MAXK/2) ? ii*512/MAXK : 0 ,0xff);
      triple.rgbtGreen = min( (ii<MAXK/2 && ii < MAXK/4) ? ii*512/MAXK : 0 ,0xff);
      triple.rgbtRed   = min( (ii<MAXK/2) ? ii*512/MAXK : 0 ,0xff);
      return triple;
    }
    
  }
  if (i == maxW/2 || j == maxH/2){
  triple.rgbtBlue  = 0xFF;
  triple.rgbtGreen = 0xFF;
  triple.rgbtRed   = 0xFF;
  }
  return triple;
}

RGBTRIPLE redcircle(int i, int j, int maxH, int maxW) {
  // temporary storage
  RGBTRIPLE triple;

  int R = min(maxH,maxW);
  
  // write RGB triple to outfile
  triple.rgbtBlue  = 0xff;
  triple.rgbtGreen = 0xff;
  triple.rgbtRed   = 0xff;

  if ( pow((i-maxH /2),2) + pow((j-maxW /2),2) < pow(R/4,2)+R/2 &&
       pow((i-maxH /2),2) + pow((j-maxW /2),2) > pow(R/4,2)-R/2) {
    triple.rgbtBlue  = 0x00;
    triple.rgbtGreen = 0x00;
    triple.rgbtRed   = 0xFF;	
  }
  return triple;
}
