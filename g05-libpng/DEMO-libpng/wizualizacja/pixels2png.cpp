/****************************************************************************
** Copyright (C) 2014 Aleksander Denisiuk
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
****************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <opt.h>
#include <png.h>

#include "input.h"

using namespace std;

double Max=2.5;
double Min=-1.5;
char * inPixelFile=(char*)"test.pixels";
char * outPNGFile=(char*)"test.png";

int main(int argc, char **argv){
  // Options
  optrega(&Max, OPT_DOUBLE, 'M', (char*) "max", (char*) "Maximum data value");
  optrega(&Min, OPT_DOUBLE, 'm', (char*) "min", (char*) "Minimum data value");
  optrega(&inPixelFile, OPT_STRING, 'i', (char*) "in-pixel-file", (char*) "Input Pixels File");
  optrega(&outPNGFile, OPT_STRING, 'o', (char*) "out-png-file", (char*) "Output PNG File");
  opt(&argc,&argv);
  opt_free();
  // Reading size
  ifstream in(inPixelFile, ifstream::binary);
  int Nx, Ny;
  readSize(in, Nx, Ny);
  cout<<"Max: "<<Max<<endl<<"Min: "<<Min<<endl;
  cout<<"Nx: "<<Nx<<endl<<"Ny: "<<Ny<<endl<<"in pixels file: "<<inPixelFile<<endl<<"out PNG file: "<<outPNGFile<<endl;
  cout<<"reading pixels data from "<<inPixelFile<<"..."<<endl;
  int Size=Nx*Ny;
  double* R=new double[Size];
  in.read((char*)R,Size*sizeof(double));
  in.close();
  cout<<"outputting PNG data to "<<outPNGFile<<"..."<<endl;
  
  
  FILE *fp = fopen(outPNGFile, "wb");
  if (!fp){
    cerr<<"Error opening file "<<outPNGFile<<endl;
    return 1;
  }
  
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) return (2);
		   png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr){
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    return (3);
  }
  
  png_init_io(png_ptr, fp);
  
  png_set_IHDR(png_ptr, info_ptr, Nx, Ny, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  cout<<"\twriting header ..."<<endl;
  png_write_info(png_ptr, info_ptr);
  
  cout<<"\twriting image data ..."<<endl;
  png_bytep row_pointer=new png_byte[Nx];
  for (int j=0;j<Ny;j++){
    for(int i=0;i<Nx;i++){
      int val=(R[Nx*j+i]-Min)/(Max-Min)*256+0.5;
      if (val>255) val=255;
      else if (val<0) val=0;
      row_pointer[i]=(png_byte)val;
    }
    png_write_row(png_ptr, row_pointer);
  }
  delete row_pointer;
  
  png_write_end(png_ptr, NULL);
  fclose(fp);
  png_destroy_write_struct(&png_ptr, &info_ptr);

  return 0;
}

