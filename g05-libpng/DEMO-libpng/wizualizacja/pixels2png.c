/****************************************************************************
** Copyright (C) 2014, 2017, 2021 Aleksander Denisiuk
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

#include <stdio.h>
#include <stdlib.h>
#include <argtable2.h>
#include <png.h>

#include "input.h"

void terminate(void* *argtable, int exitcode){
   if(argtable == NULL) printf("NULL\n");
   arg_freetable(argtable, sizeof(argtable)/sizeof(argtable[0]));
   exit(exitcode);
}

int main(int argc, char **argv){
  double Max;
  double Min;
  const char * in_pixel_file;
  const char * out_png_file;
  
  FILE * in;
  int i, j;
  int Nx, Ny;
  int size;
  double* R;
  
  // Options
  struct arg_file *in_pixel_file_name = arg_file0("i", "in-pixel-file", "<input>", "Input Pixels File");
  struct arg_file *out_png_file_name = arg_file0("o", "out-png-file", "<output>", "Output PNG File");
  struct arg_dbl *min = arg_dbl0("m", "min", "<min>", "Minimum data value");
  struct arg_dbl *max = arg_dbl0("M", "max", "<max>", "Maximum data value");
  struct arg_lit *help = arg_lit0("h","help", "print this help and exit");
  struct arg_end *end = arg_end(10); // maksymalna liczba błędów 10
  
  int nerrors;
  
  void *argtable[] = {in_pixel_file_name, out_png_file_name, min, max, help, end};
  
  if (arg_nullcheck(argtable) != 0) {
     printf("error: insufficient memory\n");
     terminate(argtable, 1);
  }
  
  min->dval[0] = -1.5;
  max->dval[0] = 2.5;
  in_pixel_file_name->filename[0] = "test.pixels";
  out_png_file_name->filename[0] = "test.png";
  
  nerrors = arg_parse(argc, argv, argtable);
  
  if (nerrors==0){
     in_pixel_file = in_pixel_file_name->filename[0];
     out_png_file = out_png_file_name->filename[0];
     Min = min->dval[0];
     Max = max->dval[0];
  }
  else{
     arg_print_errors(stderr, end, "argv[0]");
     arg_print_glossary(stderr, argtable, " %-25s %s\n");
     terminate(argtable, 1);
  }

  if (help->count > 0){
     printf("Usage: pixels2png");
     arg_print_syntax(stdout, argtable,"\n");
     arg_print_glossary(stdout, argtable,"  %-25s %s\n");
     terminate(argtable, 0);
  }

  
  printf("Max: %f\nMin: %f\n", Max, Min);
  printf("in pixels file: %s\nout PNG file: %s\n", in_pixel_file, out_png_file);

  // Reading size
  in = fopen(in_pixel_file, "rb");
  if (!in){
    perror(in_pixel_file);
    terminate(argtable, 2);
  }
  
  int err_code = readSize(in, &Nx, &Ny);
  if(err_code>0) terminate(argtable, err_code);
  
  printf("Nx: %i\nNy: %i\n", Nx, Ny);
  printf("reading pixels data from %s...\n", in_pixel_file);
  size=Nx*Ny;
  R= (double *) malloc(size*sizeof(double));
  fread(R, size*sizeof(double), 1, in);
  fclose(in);

////  
  printf("outputting PNG data to %s\n", out_png_file);
  FILE *fp = fopen(out_png_file, "wb");
  if (!fp){
    perror(out_png_file);
    terminate(argtable, 8);
  }
  
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    terminate(argtable, 9);
  }
  
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr){
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    terminate(argtable, 10);
  }
  
  png_init_io(png_ptr, fp);
  
  png_set_IHDR(png_ptr, info_ptr, Nx, Ny, 8, PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  printf("\twriting header ...\n");
  png_write_info(png_ptr, info_ptr);
  
  printf("\twriting image data ...\n");
  png_bytep row_pointer = (png_bytep) malloc(sizeof(png_byte)*Nx);
//   png_bytep row_pointer=new png_byte[Nx];
  for (j=0;j<Ny;j++){
    for(i=0;i<Nx;i++){
      int val=(R[Nx*j+i]-Min)/(Max-Min)*256+0.5;
      if (val>255) val=255;
      else if (val<0) val=0;
      row_pointer[i]=(png_byte)val;
    }
    png_write_row(png_ptr, row_pointer);
  }
  free (row_pointer);
  
  png_write_end(png_ptr, NULL);
  fclose(fp);
  png_destroy_write_struct(&png_ptr, &info_ptr);
 
 arg_freetable(argtable, sizeof(argtable)/sizeof(argtable[0]) );

  return 0;
}

