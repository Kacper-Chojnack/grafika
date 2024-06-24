/****************************************************************************
** Copyright (C) 2014, 2017 Aleksander Denisiuk
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "input.h"

int readSize(FILE* in, int* Nx, int* Ny){
  size_t line_size = 32;
  char* line;
  line = malloc(line_size * sizeof (char));
  
  if( line == NULL)    {
      fprintf(stderr, "Unable to allocate buffer\n");
      return 4;
  }
  
  getline(&line, &line_size, in);
  if(strncmp(line, "%Nx: ", 4)==0){
    *Nx = atoi(line+5);
  }
  else {
     fprintf(stderr, "Bad file format?? (first line)\n");
     free(line);
     return 5;
  }
  
  getline(&line, &line_size, in);
  if(strncmp(line, "%Ny: ", 4)==0){
    *Ny = atoi(line+5);
  }
  else {
     free(line);
     fprintf(stderr, "Bad file format?? (second line)\n");
     return 6;
  }
  
  free(line);
  
  return 0;
}

