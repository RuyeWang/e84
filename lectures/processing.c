/*********************************************************************************************** 

   This is a template image processing program to be used in E186. This program contains the basic
   input, processing and output parts. You can copy this program to your own directory and modify it
   according to your needs. In particular, you can replace the sample processing function called
   negative included in this program by any other processing functions write for the projects 
   assigned during the semester. Also included in this program are two histogram functions (one for
   color images, one for black/white images). These are gifts for you to be used in the class.

   To compile this program on the HP stations, enter:
        cc -lm -Ae processing.c -o processing
   or
        gcc -lm  processing.c -o processing
   To run the program, enter:
        processing filename_of_input_image filename_of_output_image
   or simply enter
        processing
   and the program will prompt you to enter the filenames for both input and output images.

***************************************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define glevel 256
#define Pi  3.1415926536
#define Pi2 6.28
#define Sqrt2 1.4142136
#define SWAP(a,b) tempr=(a); (a)=(b); (b)=tempr

// These are global variables used by all functions 

char *strstr();

char magic[4];
char in_filename[50], out_filename[50];
int M_in,N_in, M_out,N_out,Max,color_in, color_out;         
float **temp2_in, **temp2_out, ***temp3_in, ***temp3_out;

float *fl_alloc1d(), **fl_alloc2d(), ***fl_alloc3d();
float **read_bw_image(), ***read_color_image();

int lookup[glevel];

main(argc, argv)
int argc;
char **argv;
{
  int i,j,k,l,m,n;
  short w;
  FILE *in_file, *out_file;
  
  // Check number of command line paramenters 
  if (argc!=3)
    {
      fprintf(stderr,"\nUsage: processing in_filename out_filename \n\n");
      printf("\nInput image filename: ");  scanf("%s",&in_filename);
      printf("\nOutput image filename: ");  scanf("%s",&out_filename);
    }
  else  
    { strcpy(in_filename,argv[1]);  strcpy(out_filename,argv[2]); }

  if ( (in_file = fopen(in_filename, "r")) == NULL) 
    { fprintf(stderr, "Unable to open file: %s for reading, exiting.\n", in_filename);
      exit(0);
    }
  if ( (out_file = fopen(out_filename, "w")) == NULL) 
    { fprintf(stderr, "Unable to open file: %s for writing, exiting.\n", out_filename);
      exit(0);
    }

  // Read header information of input image, m and n (size of image), 
  // the magic number (P5 for black/white, P6 for color RGB), etc. 
  get_header(in_file);

  /* Here we assume the output image is of the same size as the input image. If this is not 
     the case, modify M_out and N_out in your processing functions according to your algorithm. */
  M_out=M_in; N_out=N_in;

  if (strcmp(magic, "P5")==0) 
    { color_in=color_out=0;
      temp2_in=read_bw_image(in_file);            /* read input b/w image into temp2_in */
      temp2_out=fl_alloc2d(M_out,N_out);          /* allocate memory for temp2_out */
      bw_histogram(temp2_in,M_in,N_in);           /* calculating histogram of input */
      printf("Processing black/white image of size (%d x %d)\n",M_in,N_in);    
    }
  else
    { color_in=1;
      temp3_in=read_color_image(in_file);         /* read input color image into temp3_in */
      color_histogram(temp3_in,M_in,N_in);        /* calculating histogram of input */
      printf("use only one component of Red (0), Green (1), Blue (2) or all three (3) ");
      scanf("%d",&k);
      if (k<3) 
	{ 
	  color_out=0;
	  temp2_out=fl_alloc2d(M_out,N_out);       /* allocate memory for temp2_out */
	  temp2_in=temp3_in[k];
	  if (k==0)
	    printf("Processing R-component of input image of size (%d x %d)\n",M_in,N_in);    
	  else if (k==1)
	    printf("Processing G-component of input image of size (%d x %d)\n",M_in,N_in);    
	  else if (k==2)
	    printf("Processing B-component of input image of size (%d x %d)\n",M_in,N_in);    
	}
      else
	{ color_out=1;
	  temp3_out=fl_alloc3d(3,M_out,N_out);                /* allocate memory for temp3_out */
	  printf("Processing color image of size (%d x %d)\n",M_in,N_in);
	}
    }

  printf("Choose one of the processing functions:\n");
  printf(" (0) No further processing\n");
  printf(" (1) Linear stretchn\n");
  printf(" (2) Histogram equalization\n");
  printf(" (3) Histogram specification\n");
  printf(" (4) Negative\n");
  printf(" (5) Threshold\n");
  printf(" (6) Fourier filters\n");
  printf(" (7) Walsh_Hadamard filter\n");
  printf(" (8) Discrete cosin filter\n");
  printf(" (9) Convolution\n");
  printf("(10) Edge detection\n");
  printf("(11) Median filter\n");
  printf("(12) piecewise linear mapping\n");
  printf("(13) Color image enhancement\n");
  printf("(14) Blur\n");
  printf("(20) Temp (B/W)\n");
  printf("(21) Temp (color)\n");

  scanf("%d",&k);
  printf("choice=%d\n",k);
  switch (k) {
  case  0:  break;                    /* histogram is always calculated */
  case  1:  stretch();  break;
  case  2:  hist_equal();  break;
  case  3:  hist_specify();  break;
  case  4:  negative();  break;
  case  5:  threshold();  break;
  case  6:  DFT_filtering(); break;
  case  7:  WHT_filtering(); break;
  case  8:  DCT_filtering(); break;
  case  9:  convolution(); break;
  case 10:  edge_detect(); break;
  case 11:  median(); break;
  case 12:  linearmap(); break;
  case 13:  color_proc(); break;
  case 14:  blur(); break;
  case 20:  temp_bw(); break;
  case 21:  temp_color(); break;
  }
  if (k) write_image(out_file);     /* write output image */

}


float ***read_color_image(in_file)
FILE *in_file;
{
  int i,j,k;
  float ***temp;
/*
  printf("Color input image.\n");  
*/
  temp=fl_alloc3d(3,M_in,N_in);

  for (i=0; i<M_in; i++)
    for (j=0; j<N_in; j++)
      for (k=0; k<3; k++)
	temp[k][i][j] = read_byte(in_file); 
  
  fclose(in_file); 
  return temp;
}



float **read_bw_image(in_file)
FILE *in_file;
{
  int i,j,k;
  float **temp;

/* 
   printf("Black/white input image.\n");  
*/
  temp=fl_alloc2d(M_in,N_in);

  for (i=0; i<M_in; i++)
    for (j=0; j<N_in; j++)
      temp[i][j] = read_byte(in_file); 
  
  fclose(in_file); 
  return temp;
}


write_image(out_file)
FILE *out_file;
{
  int i,j,k;

  /* write magic number and m, n as the size of the output image. If output image has different size than the
     input image, make sure m, n have the right value before writing */

  printf("N_out=%d M_out=%d\n",N_out,M_out);

  if (color_out)
    { printf("Writing color image %s on disk...\n",out_filename);
      fprintf(out_file,"%s\n%d %d\n%d\n","P6",N_out,M_out,Max);
      for (i=0; i<M_out; i++)
	for (j=0; j<N_out; j++)
	  for (k=0; k<3; k++)
	    putc( (short)temp3_out[k][i][j], out_file);
    }
  else
    { printf("Writing B/W image %s on disk...\n",out_filename);
      fprintf(out_file,"%s\n%d %d\n%d\n","P5",N_out,M_out,Max);
      for (i=0; i<M_out; i++)
	for (j=0; j<N_out; j++)
	  putc( (short)temp2_out[i][j], out_file);
      }
  fclose(out_file);
}


color_histogram(array,nr,nc)
float ***array;
int nr,nc;
{
  int i,j,k,l,m,n,npixel, scale=4, nrow;
  int max_hist_ds=200, max_hist_ac=246;
  float u,v,w1,w2,max,sum,***hist;
  float hist_ds[2048],hist_ac[2048];
  char *sp, *filename;

  printf("Calculating histogram of color image %s...\n",in_filename);
  nrow=max_hist_ac+scale; 

  npixel=nr*nc; 
/*
  printf("row=%d, column=%d, npixel=%d, glevel=%d, nrow=%d\n",
	 nr,nc,npixel, glevel, nrow);
*/
  hist=fl_alloc3d(nrow+1,glevel,3);

  for (k=0; k<3; k++)
    { 
      for (j=0; j<glevel; j++)
	{ hist_ds[j]=hist_ac[j]=0;
	  for (i=0; i<nrow; i++)  hist[i][j][k]=255;
	}
      for (j=0; j<nc; j++) 
	for (i=0; i<nr; i++) 
	  {
	    l=array[k][i][j]+0.5;
	    hist_ds[l]+=1.0;
	  }
      max=0; sum=0;
      w2=(float)max_hist_ac/npixel;
      for (j=0; j<glevel; j++)
	{
	  sum+=hist_ds[j];
	  hist_ac[j]+=sum;
	  hist_ac[j]*=w2;
/*
          u=hist_ds[j]=pow(hist_ds[j],0.4);
*/
	  u=hist_ds[j];
	  if (max < u) max=u;
	}
      if (max>0)
	{
	  w1=(float)max_hist_ds/max;   

	  for (j=0; j<glevel; j++) 
	    { hist_ds[j]*=w1;
	      if (!(j%32)) 
		for (i=nrow; i>nrow-scale; i--)  hist[i][j][k]=hist[i][j+1][k]=0.0;
	      for (i=nrow-scale; i>=0; i--) 
		if (i>nrow-scale-(int)hist_ds[j] && i>nrow-scale-(int)hist_ac[j])
		  hist[i][j][k]=0;
		else if (i>nrow-scale-(int)hist_ds[j] || i>nrow-scale-(int)hist_ac[j])
		  hist[i][j][k]=160;
	    }
	}
    }

  filename=in_filename;
  sp=strstr(in_filename,".p");
  if (sp) strcpy(sp,".hist");
  else strncat(filename,".hist",5);
  printf("Histogram filename: %s\n",filename);

/*
  sprintf(fname, "%s_hist",in_filename);
*/
  display_color_image(filename,hist,nrow,glevel);

  printf("The histogram filename is:   %s\n",filename);
}


bw_histogram(array,nr,nc)
float **array;
int nr,nc;
{
  int i,j,k,l,m,n,npixel, scale=4, nrow;
  int max_hist_ds=200, max_hist_ac=246;
  float u,v,w1,w2,max,sum,**hist,hist_ds[2048],hist_ac[2048];
  char *sp, *filename;

  printf("Calculating histogram of B/W image %s...\n",in_filename);
  nrow=max_hist_ac+scale; 

  npixel=nr*nc; 
/*
  printf("npixel=%d, glevel=%d, nrow=%d\n",npixel, glevel, nrow);
*/
  hist=fl_alloc2d(nrow+1,glevel);

  for (j=0; j<glevel; j++)
    { hist_ds[j]=hist_ac[j]=0;
      for (i=0; i<nrow; i++)  hist[i][j]=255;
    }

  for (j=0; j<nc; j++) 
    for (i=0; i<nr; i++) 
      {
	l=array[i][j]+0.5;
	hist_ds[l]+=1.0;
      }
  max=0; sum=0;
  w2=(float)max_hist_ac/npixel;
  for (j=0; j<glevel; j++)
    {
      sum+=hist_ds[j];
      hist_ac[j]+=sum;
      hist_ac[j]*=w2;
/*
      u=hist_ds[j]=pow(hist_ds[j],0.4);
*/
      u=hist_ds[j];
      if (max < u) max=u;
    }
  w1=(float)max_hist_ds/max;   

  for (j=0; j<glevel; j++) 
    { hist_ds[j]*=w1;
      if (!(j%32)) 
	for (i=nrow; i>nrow-scale; i--)  hist[i][j]=hist[i][j+1]=0;
      for (i=nrow-scale; i>=0; i--) 
	if (i>nrow-scale-(int)hist_ds[j] && i>nrow-scale-(int)hist_ac[j])
	  hist[i][j]=0;
	else if (i>nrow-scale-(int)hist_ds[j] || i>nrow-scale-(int)hist_ac[j])
	  hist[i][j]=160;
    }

  filename=in_filename;
  sp=strstr(in_filename,".p");
  if (sp) strcpy(sp,".hist");
  else strncat(filename,".hist",5);
  printf("Histogram filename: %s\n",filename);
  
/*
  sprintf(fname, "hist_%s",in_filename);
*/
  display_bw_image(filename,hist,nrow,glevel);
  printf("The histogram filename is:  %s\n",filename);
  
}      




get_header(in_f)
FILE *in_f;
{
  char word[10];
/*
  printf("get header...\n");
*/
  if (getword(in_f,word)!=0) eof_err();
  strcpy(magic,word);

  if ( (strcmp(magic,"P5")!=0) && (strcmp(magic,"P6")!=0) ) 
    {
      fprintf(stderr,"\nBad file type, use raw grayscale (pgm) or raw color (ppm) file.\n");
      exit(0);
    }

  if (getword(in_f,word)!=0) eof_err();  N_in=atoi(word);
  if (getword(in_f,word)!=0) eof_err();  M_in=atoi(word);
  if (getword(in_f,word)!=0) eof_err();  Max=atoi(word);
}


/**********
 *
 * read_byte() - read a byte of data from a file and check for EOF
 *
 *  arguments - FILE * of file to read from
 *
 *  returns - int value of the byte read
 *
 **********/

int read_byte(in_f)
FILE *in_f;
{
int cha;

cha=fgetc(in_f);
if (cha==EOF) eof_err();

return cha;
}


/**********
 *
 * getword() - eliminates whitspace and comments and gets a single word.
 *
 *  arguments: file pointer to read from, initialized char * to place word into
 *
 *  returns: 0 if no end of file
 *           -1 if no word and eof
 *           1 if word then eof
 *
 **********/

int getword(in_f, word)  
FILE *in_f;
char *word;
{              
int cha=' ';
int x=0;
short end=0;

/* Eliminate White Space and comments (from # to end of line) */
do  
  {
  cha=fgetc(in_f);   
  if (cha=='#') 
    {
    do cha=fgetc(in_f);
    while (!(cha=='\n'));
    }
  }
while (isspace(cha) && (cha!=EOF));

if (cha==EOF) return -1;
/* get chars until a whitespace or End of File*/
else
  do
    { word[x++]=cha; cha=fgetc(in_f); }			/* Get Single Word */
  while ( !(isspace(cha)) && (cha!=EOF) );

/* if end of the file then we are done reading chars */
if (cha==EOF) end=1;

word[x]='\0';
return end;  
}

  
/***********
 * 
 * malloc_err() - print mem allocation error message and exit 
 *
 ***********/

malloc_err()
{
fprintf(stderr, "Error allocating memory, exiting. \n");
exit(0);
}


/**********
 * 
 * eof_err() - print end of file error message and exit 
 *
 **********/

eof_err()
{
fprintf(stderr, "Unexpected end of file, exiting. \n");
exit(0);
}

pause()
{
  int i;
  printf("paused, enter anything to continue...\n");
  scanf("%d",&i);
}


float ***fl_alloc3d(nrow, ncol, npla)
int nrow, ncol, npla;
{
int i,j,k;
float ***array;

if ( (array = (float ***) malloc(nrow * sizeof(float**)) ) == NULL)
  malloc_err();
for (i=0; i<nrow; i++)
  if ( (array[i] = (float **)malloc(ncol * sizeof(float*)) ) == NULL)
    malloc_err();
for (i=0; i<nrow; i++)
  for (j=0; j<ncol; j++)
    if ( (array[i][j] = (float *)malloc(npla * sizeof(float)) )== NULL)
      malloc_err();
    else
      for (k=0; k<npla; k++) array[i][j][k]=0.0;
return array;
}


float **fl_alloc2d(m,n)
int m,n;
{ int r,c;
  float **array;
  if ( (array = (float **) malloc(m * sizeof(float*)) )==NULL) 
    printf("Unable to allocate memory for 2D float array...\n");
  for (r=0; r<m; r++)
    if ( (array[r] = (float*)malloc(n * sizeof(float)) ) == NULL)
      printf("Unable to allocate memory for 2D float array...\n");
    else 
      for (c=0; c<n; c++) array[r][c]=0;
  return array;
}

float *fl_alloc1d(m)
int m;
{ int i;
  float *array;
  if ( (array = (float *) malloc(m * sizeof(float)) )==NULL) 
    printf("Unable to allocate memory for 1D float array...\n");
  else
    for (i=0; i<m; i++) array[i]=0.0;
  return array;
}

display_bw_image(filename,tmp,m,n)
float **tmp;
char filename[];
int m,n;
{ int i,j,k,l,s;
  float v, min, max;
  unsigned char w;
  FILE *fp;

  k=m; if (k<n) k=n;
  s=256/k;
  s=1;
/*
  printf("Display black/white image %s of size (%d, %d).\n",filename,m,n);
*/
  min=9e9; max=-min;
  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      { if (min>tmp[i][j]) min=tmp[i][j];  
	if (max<tmp[i][j]) max=tmp[i][j]; 
      }
  v=255.0/(max-min);

/*  printf("   min=%f, max=%f, v=%f, s=%d\n", min,max,v,s); */

  if ( (fp = fopen(filename, "w")) == NULL) 
    { fprintf(stderr, "Unable to open image %s for writing, exiting.\n",filename);
      exit(0);
    }
  fprintf(fp,"%s\n%d %d\n%d\n", "P5", s*n,s*m, 255);
  for (i=0; i<m; i++)
    for (l=0; l<s; l++)
      for (j=0; j<n; j++)
	for (k=0; k<s; k++)
	  putc(w=(tmp[i][j]-min)*v,fp);
  fclose(fp);      
}


display_color_image(filename,tmp,m,n)
float ***tmp;
char filename[];
int m,n;
{ int i,j,k,l,s;
  float v, min, max;
  unsigned char w;
  FILE *fp;

  k=m; if (k<n) k=n;
  s=256/k;
  s=1;

  printf("Display color image %s of size (%d, %d).\n",filename,m,n);

  min=9e9; max=-min;
  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      for (k=0; k<3; k++)
	{ if (min>tmp[i][j][k]) min=tmp[i][j][k];  
	  if (max<tmp[i][j][k]) max=tmp[i][j][k]; 
	}
  v=255.0/(max-min);

/*  printf("   min=%f, max=%f, v=%f, s=%d\n", min,max,v,s); */

  if ( (fp = fopen(filename, "w")) == NULL) 
    { fprintf(stderr, "Unable to open image %s for writing, exiting.\n",filename);
      exit(0);
    }
  fprintf(fp,"%s\n%d %d\n%d\n", "P6", s*n,s*m, 255);
  for (i=0; i<m; i++)
    for (l=0; l<s; l++)
      for (j=0; j<n; j++)
	for (k=0; k<s; k++)
	  { putc(w=(tmp[i][j][0]-min)*v,fp);
	    putc(w=(tmp[i][j][1]-min)*v,fp);
	    putc(w=(tmp[i][j][2]-min)*v,fp);
	  }
  fclose(fp);      
}




/************************************************************************************

  In the following, we list all the processing functions.

************************************************************************************/

threshold()
{
  int i,j,k,tl,th, w, valid;
  float Max;

  valid=0;
  while (!valid)
    {
      printf("Low threshold (0 ~ 255): ");
      scanf("%d",&tl);
      printf("High threshold (%d ~ 255): ",tl);
      scanf("%d",&th);
      if (th>=tl) valid=1;
    }

  Max=-999;
  if (color_out)
    for (i=0; i<M_out; i++)
      for (j=0; j<N_out; j++)
	for (k=0; k<3; k++)
	  {
	    w=temp3_in[k][i][j];
	    if (w<tl) w=0; if (w>th) w=255;
	    temp3_out[k][i][j]=w;
	    if (Max<w) Max=w;
	  } 
  else 
    for (i=0; i<M_out; i++)
      for (j=0; j<N_out; j++)
	{
	  w=temp2_in[i][j];
	  if (w<tl) w=0; if (w>th) w=255;
	  temp2_out[i][j]=w;
	  if (Max<w) Max=w;
	}
}

temp_color()
{
  int i,j,k,I1,J1,I2,J2,G;
  float r,g,b,Max;

  printf("gray level set to white: ");
  scanf("%d",&G);
  for (i=0; i<M_in; i++)
    for (j=0; j<N_in; j++)
      { r=temp3_in[0][i][j]; g=temp3_in[1][i][j]; b=temp3_in[2][i][j];
        if (r==G && g==G && b==G )  { r=g=b=255; }
	    //        if (r==0   && g==255 && b==255)   { g=b=155; }
	temp3_out[0][i][j]=r; temp3_out[1][i][j]=g; temp3_out[2][i][j]=b; 
      }
}

temp_bw()
{
  int i,j,k,I1,J1,I2,J2,G;
  float r,g,b,Max;

  printf("gray level set to white: ");
  scanf("%d",&G);

  for (i=0; i<M_out; i++)
    for (j=0; j<N_out; j++)
      { r=temp2_in[i][j]; 
        temp2_out[i][j]=r;
        if (r==G) temp2_out[i][j]=255;
      }
}


	


negative()
{
  int i,j,k;
  float Max, w;

  Max=-999;
  if (color_out)
    for (i=0; i<M_out; i++)
      for (j=0; j<N_out; j++)
	for (k=0; k<3; k++)
	  {
	    w=temp3_out[k][i][j]=255.0-temp3_in[k][i][j];
	    if (Max<w) Max=w;
	  } 
  else 
    for (i=0; i<M_out; i++)
      for (j=0; j<N_out; j++)
	{
	  w=temp2_out[i][j]=255.0-temp2_in[i][j];
	  if (Max<w) Max=w;
	}
}

/**********
 *
 * stretch() - stretch histogram linearly
 *
 **********/

stretch()
{
  int i,j,k,l,m,n,min,max,sum;
  float u,v,w,cut_low,cut_high,hist[glevel];

  printf("Given a B/W image, stretch its histogram linearly\n");

  if (color_out)  { printf("This program only stretchs B/W images, exit\n");  exit(0); }

  printf("cut-off percentage at low end (0.01, 0.03, etc.): ");
  scanf("%f",&cut_low);
  printf("cut-off percentage at high end (0.01, 0.03, etc.): ");
  scanf("%f",&cut_high); 

  w=1.0/M_in/N_in;

  for (i=0; i<glevel; i++)  hist[i]=0;
  for (i=0; i<M_in; i++) 
    for (j=0; j<N_in; j++) 
      hist[ (int)(temp2_in[i][j]+0.5) ]+=w;;

  w=0.0;   min=0;
  while (w < cut_low)   {  w+=hist[min]; min++; }
  w=0.0;   max=glevel-1;
  while (w < cut_high)  {  w+=hist[max]; max--; }
  w=(float)(glevel-1)/(max-min);

  printf("min=%d, max=%d, w=%f\n",min, max, w);

  for (l=0; l<glevel; l++)
    if (l<=min) lookup[l]=0;
    else if (l>=max) lookup[l]=glevel-1;
    else lookup[l]=w*(l-min);

  for (i=0; i<M_in; i++) 
    for (j=0; j<N_in; j++) 
      { l=temp2_in[i][j];
	temp2_out[i][j]=lookup[l];
      }
/*
  for (i=0; i<M_in; i++) 
    for (j=0; j<N_in; j++) 
      { v=temp2_out[i][j]=(temp2_in[i][j]-min)*w;
	if (v>=glevel) temp2_out[i][j]=glevel-1;
	if (v<0) temp2_out[i][j]=0;
      }
*/
}


/**********
 *
 * hist_equal() - histogram equalization
 *
 **********/

hist_equal()
{
  int i,j,k,l,m,n,min,max,npixel;
  float w,sum,cut,hist[glevel];

  printf("Given an input image, equalize its histogram\n");

  if (color_out) { printf("This program only processes B/W images, exit\n");    exit(0);  }

  w=1.0/M_in/N_in;

  for (i=0; i<glevel; i++)  hist[i]=0;
  for (i=0; i<M_in; i++) 
    for (j=0; j<N_in; j++) 
      hist[ (int)temp2_in[i][j] ]+=w;
  sum=0;
  for (i=0; i<glevel; i++)
    { sum+=hist[i];   hist[i]=(glevel-1)*sum;  }
  
  for (i=0; i<M_in; i++) 
    for (j=0; j<N_in; j++) 
      temp2_out[i][j]=hist[(int)temp2_in[i][j]];
}


/**********
 *
 * hist_specify() - histogram specification
 *
 **********/

hist_specify()
{
  int i,j,k,l,m,n,min,max,npixel;
  float d0,d1,w,total,sum,Sum,hist[glevel],Hist[glevel];

  printf("Histogram Specification\n");

  if (color_out) { printf("This program only processes B/W images, exit\n");    exit(0);  }

  npixel=M_in*N_in;

  /* specify desired histogram here */
  total=0;
  for (i=0; i<glevel; i++)
    { 
      /*
      if (i<64) Hist[i]=i;
      else if (i<128) Hist[i]=127-i;
      else if (i<192) Hist[i]=i-128;
      else Hist[i]=255-i;
      */

      if (i<128) Hist[i]=i;
      else Hist[i]=255-i;
      total+=Hist[i];  
    }

  for (i=0; i<glevel; i++)    hist[i]=0;
  for (i=0; i<M_in; i++) 
    for (j=0; j<N_in; j++) 
      hist[ (int)temp2_in[i][j] ]++;

  sum=0; Sum=0;
  for (i=0; i<glevel; i++)
    { sum+=hist[i]; Sum+=Hist[i];
      hist[i]=sum/npixel; Hist[i]=Sum/total;
    }

  j=0;
  for (i=0; i<glevel; i++)
    { 
      if ( hist[i]<=Hist[j] ) lookup[i]=j;
      else 
	{ while (hist[i]>Hist[j]) j++;
	  if (Hist[j]-hist[i] > hist[i]-Hist[j-1]) lookup[i]=(j--);
	  else lookup[i]=j;
	}
/*
	  d0=pow(hist[i]-Hist[j-1], 2);
	  d1=pow(hist[i]-Hist[j], 2);
	  if (d0 < d1) lookup[i]=(j--);
	  else lookup[i]=j;
	}
*/
    }
	   
  for (i=0; i<M_in; i++) 
    for (j=0; j<N_in; j++) 
      temp2_out[i][j]=lookup[ (int)temp2_in[i][j] ];
}


fdct2d(x,m,n,inverse)    /* size has to be power of 2 */
float **x;
int m,n,inverse;
{
  int i,j,k;
  float *y;

  k=n; if (m>n) k=m;
  y = (float *) malloc(k*sizeof(float));

  printf("   row transform...\n");
  for (i=0; i<m; i++)
    { for (j=0; j<n; j++) y[j]=x[i][j]; 
      fdct(y,n,inverse);
      for (j=0; j<n; j++) x[i][j]=y[j];  
    }
  printf("   column transform...\n");
  for (j=0; j<n; j++)
    { for (i=0; i<m; i++) y[i]=x[i][j];
      fdct(y,m,inverse);
      for (i=0; i<m; i++) x[i][j]=y[i];
    }
  free(y);
}



dct2d(x,m,n,inverse)    /* for arbitrary size DCT2D */
float **x;
int m,n,inverse;
{
  int i,j,k;
  float **t,**y, u0,v0,u,v,w,wm,wn;

  k=m; if (n>m) k=n;
  wm=Pi/2/m, wn=Pi/2/n;
  u0=sqrt(1.0/m); u=sqrt(2.0/m);   
  v0=sqrt(1.0/n); v=sqrt(2.0/n); 
  t=fl_alloc2d(k,k); y=fl_alloc2d(m,n);

/* preparing for column transform matrix */  
  for (i=0; i<m; i++)
    for (j=0; j<m; j++)
      if (inverse) t[i][j]=cos( (2*i+1)*j*wm );
      else t[i][j]=cos( (2*j+1)*i*wm );
/* matrix multiplication for column transform */
  for (j=0; j<n; j++)                 /* for all columns */
    for (i=0; i<m; i++)               /* for each element in the column */
      { y[i][j]=0;
	for (k=0; k<m; k++)
	  {
	    if (inverse)
	      if (k) w=u;  else w=u0; 
	    else w=1;
	    y[i][j]+=w*t[i][k]*x[k][j];
	  }
      }
/* preparing for row transform matrix */  
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      if (inverse) t[i][j]=cos( (2*j+1)*i*wn );
      else t[i][j]=cos(  (2*i+1)*j*wn );
/* matrix multiplication for row transform */
  for (i=0; i<m; i++)                 /* for all rows */
    for (j=0; j<n; j++)               /* for each element in the row */
      { x[i][j]=0;
	for (k=0; k<n; k++)
	  { if (inverse)
	      if (k) w=v;  else w=v0; 
	    else w=1;
	    x[i][j]+=w*y[i][k]*t[k][j];
	  }
      }
  if (!inverse)
    { x[0][0]*=u0*v0;
      for (i=1; i<m; i++) x[i][0]*=u*v0;
      for (j=1; j<n; j++) x[0][j]*=u0*v;
      for (i=1; i<m; i++)
	for (j=1; j<n; j++)
	  x[i][j]*=u*v;
    }  
  
  fl_dealloc2d(t,k,k);  fl_dealloc2d(y,m,n);	

}



/**********
 *
 * Discrete Cosine Filtering() 
 *
 **********/
DCT_filtering()
{
  int i,j,k,m,n,p2;

  float c,w,min,max;

  if (color_out) { printf("This program only processes B/W images, exit\n");    exit(0);  }

  m=M_in; n=N_in;
  i=w=log2((float)m);  j=w=log2((float)n);
  if (pow(2.0,(float)i)==m && pow(2.0,(float)j)==n) p2=1; else p2=0;

  printf("Image size: %d by %d, log2: %d %d \n",m,n,i,j);
  printf("Image size: %d by %d, log2: %d %d \n",m,n,i,j);
  if (p2) printf("m, n are power of 2, use fast DCT.\n");
  else printf("m, n are not power of 2, use regular DCT.\n");

  printf("Filtering, low-pass (0) or high_pass (1): ");
  scanf("%d",&k);      

  printf("Forward 2D-DCT...\n");
  if (p2) fdct2d(temp2_in,m,n,0); else dct2d(temp2_in,m,n,0);

  printf("2D filtering...\n");
  spectral_filtering2d(temp2_in,m,n,k);

  printf("Inverse 2D-DCT...\n");
  if (p2) fdct2d(temp2_in,m,n,1); else dct2d(temp2_in,m,n,1);

  min=1.0e9; max=-min;
  printf("Writing to out_image...\n");
  for (i=0; i<M_out; i++)
    for (j=0; j<N_out; j++)
      { w=temp2_in[i][j]; if (min>w) min=w; if (max<w) max=w; }

  w=255/(max-min);
  printf("min=%f, max=%f, w=%f\n",min,max,w);
  for (i=0; i<M_out; i++) 
    for (j=0; j<N_out; j++) 
      temp2_out[i][j]=w*(temp2_in[i][j]-min);

}

/* dct based on fft, fast */

fdct(x,n,inverse)
float *x;
int n,inverse;
{ int i,j,k;
  float *yr,*yi;
  float a;

  yr=(float *) malloc(n*sizeof(float));
  yi=(float *) malloc(n*sizeof(float));

  if (inverse)
    for (i=0; i<n; i++)
      { if (i) x[i]*=Sqrt2;  a=-Pi*i/n/2;
	yr[i]=x[i]*cos(a); yi[i]=-x[i]*sin(a);
      }
  else
    for (i=0; i<n/2; i++)
      { yr[i]=x[2*i]; yr[n-1-i]=x[2*i+1]; yi[i]=yi[i+n/2]=0.0; }
  
  fft(yr,yi,n,inverse);     /* forward fft  */

  if (inverse)
    for (i=0; i<n/2; i++)
      { x[2*i]=yr[i]; x[2*i+1]=yr[n-1-i]; }
   else 
    { for (i=0; i<n; i++)
	{ a=-Pi*i/n/2;
	  x[i]=(cos(a)*yr[i]-sin(a)*yi[i])*Sqrt2;
	}
      x[0]/=Sqrt2;
    }

  free(yr); free(yi);
}


/* dct by definition. slow */
dct(x,n,inverse)
float x[];
int n,inverse;
{ int mmax,i,j;
  float *y,u,u0,w,t;

  w=0.5*Pi/n;  
  u0=sqrt(1.0/n); u=sqrt(2.0/n); 
  y=(float *)malloc(n*sizeof(float));
  
  if (inverse)
    for (i=0; i<n; i++)
      { y[i]=u0*x[0];
	for (j=1; j<n; j++)
	  y[i]+=u*x[j]*cos((2*i+1)*j*w);
      }
  else
    for (i=0; i<n; i++)
      { y[i]=0;
	for (j=0; j<n; j++)
	  y[i]+=x[j]*cos((2*j+1)*i*w);
	if (i) y[i]*=u; else y[0]*=u0;
      }
  for (i=0; i<n; i++) x[i]=y[i];  
  free(y);
}


/**********
 *
 * Walsh_Hadamard() - filtering in sequency domain
 *
 **********/

WHT_filtering()
{
  int i,j,k,m,n,lm,ln;
  float c,w,min,max, *x, **xx;

  if (color_out) { printf("This program only processes B/W images, exit\n");    exit(0);  }

  m=M_in; n=N_in;

/* m=n=8;  */

  lm=w=log2(m); ln=w=log2(n);
  k=n; if (m>n) k=m;
  printf("Image size: %d by %d, lm=%d, ln=%d, k=%d\n",m,n,lm,ln,k);

  xx = fl_alloc2d(m,n); 
  x = (float *) malloc(k*sizeof(float));

  printf("Filtering, low-pass (0) or high_pass (1): ");
  scanf("%d",&k);      

/*
  for (i=0; i<m; i++) 
    for (j=0; j<n; j++) 
      xx[i][j]=i+j;
  printf("Date in spatial domain...");
  for (i=0; i<m; i++)
    { printf("\n");
      for (j=0; j<n; j++)
	printf("%5.1f ",xx[i][j]);
    }
*/
  printf("Forward 2DWHT\n   row transform...\n");
  for (i=0; i<m; i++)
    { for (j=0; j<n; j++)
	{ x[j]=temp2_in[i][j]; 
/*	x[j]=xx[i][j]; */}
      wht(x,ln,1,0);
      for (j=0; j<n; j++)
	xx[i][j]=x[j];  
    }
  printf("   column transform...\n");
  for (j=0; j<n; j++)
    { for (i=0; i<m; i++)
	x[i]=xx[i][j];
      wht(x,lm,1,0);
      for (i=0; i<m; i++)
	xx[i][j]=x[i];
    }

/*
  printf("Data in sequency domain:\n");
  for (i=0; i<m; i++)
    { printf("\n");
      for (j=0; j<n; j++)
	printf("%5.1f ",xx[i][j]);
    }
  pause();
*/

  spectral_filtering2d(xx,m,n,k);

  printf("Inverse 2DWHT\n   column transform...\n");
  for (j=0; j<n; j++)
    { for (i=0; i<m; i++)
	x[i]=xx[i][j];
      wht(x,lm,1,1);
      for (i=0; i<m; i++)
	xx[i][j]=x[i]; 
    }
  printf("   row transform...\n");
  for (i=0; i<m; i++)
    { for (j=0; j<n; j++)
	x[j]=xx[i][j];
      wht(x,ln,1,1);
      for (j=0; j<n; j++)
	xx[i][j]=x[j];
    }

/*
  printf("Bact to spatial domain...\n");
  for (i=0; i<m; i++)
    { printf("\n");
      for (j=0; j<n; j++)
	printf("%5.1f ",xx[i][j]);
    }
*/
  min=1.0e9; max=-min;
  printf("Writing to out_image...\n");
  for (i=0; i<M_out; i++)
    for (j=0; j<N_out; j++)
      { if (min>xx[i][j]) min=xx[i][j]; 
	if (max<xx[i][j]) max=xx[i][j]; 
      }

  w=255/(max-min);

  printf("min=%f, max=%f, w=%f\n",min,max,w);
  for (i=0; i<M_out; i++) 
    for (j=0; j<N_out; j++) 
      temp2_out[i][j]=w*(xx[i][j]-min);

}


spectral_filtering2d(x,m,n,k)
float **x;
int m,n,k;
{ int i,j,l,N;
  float d0,d1,min,max,w,**spectrum;;
  spectrum=fl_alloc2d(m,n);

  if (k) { d0=n/8; printf("High-pass filtering...\n   d0=%f",d0); }
  else { d0=n/8; printf("Low-pass filtering...\n   d0=%f",d0); }

  printf("\nIdeal (0), Butterworth (1), allpass (2) ");
  scanf("%d",&l);
  N=4;
/*  if (l) { N=4; if (k) { N=8; d0=n/2.3; }}  */

  min=1.0e9; max=-min;
  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      { w=spectrum[i][j]=log2(1.0+abs(x[i][j]));
	if (min>w) min=w;  if (max<w) max=w;
	d1=sqrt( w=i*i+j*j ); 
	if (l==1) 
	  { w=1./(1.+pow(d1/d0,N)); 
	    if (k) w=1-w;
	    x[i][j]*=w; 
	  }
	else if (l==0) 
	  if (k)           /* high-pass */
	    { if (d1<d0) { x[i][j]=0; }}
	  else             /* low-pass  */
	    if (d1>d0) { x[i][j]=0; }
      }
  w=255.0/(max-min);
  printf("Min=%f, Max=%f, w=%f\n",min,max,w);
  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      spectrum[i][j]=w*(spectrum[i][j]-min); 
  write_spectrum(spectrum,m,n);
}


int howo(i,m)    /*  converts a sequency index i to Haramard index j  */
int i,m;
{ int j,k;
/*  j=i>>1;  i=i^j;  /* converting binary to Gray code */
  i=i^(i>>1);
  j=0;
  for (k=0; k<m; ++k)
    j=(j << 1) | (1 & (i >> k));    /* bit-reversal */
  return j;
}

wht(x,m,sequency,inverse)
float x[];
int m,inverse,sequency;
{ int mmax,step,i,j,k,j1,n,nu,N;
  float *y,t;

  N=pow(2,m);
  y=(float *)malloc(N*sizeof(float));

  for (i=0; i<m; i++) {         /* for log N stages */
    n=pow(2,m-1-i);             /* length of section */
    k=0;
    while (k<N-1) {             /* before reaching end */
      for (j=0; j<n; j++) {     /* for each section */
	j1=k+j;
	t=x[j1]+x[j1+n];
	x[j1+n]=x[j1]-x[j1+n];
	x[j1]=t;
      }
      k+=2*n;
    }
  }
  if (inverse)        /* only for inverse transform */
    for (i=0; i<N; i++) x[i]=x[i]/N;

  if (sequency) {     /* converting to sequency (Walsh) order */
/*
    printf("\n before reordering...\n");
    for (i=0; i<N; i++)  printf("%3.0f ",x[i]);
    printf("\n");
*/ 
    for (i=0; i<N; i++) { j=howo(i,m);  y[i]=x[j]; }
    for (i=0; i<N; i++) x[i]=y[i];
/*
    printf("\n after reordering...\n");
    for (i=0; i<N; i++)  printf("%3.0f ",x[i]);
    printf("\n");
*/
    free(y);
  }
}


/**********
 *
 * convolution() - convolution with specified kernel
 *
 **********/

convolution()
{
  int i,j,k,m,n,ni,ni1,ni2,nj,nj1,nj2;
  float c,w,min,max, kn[15][15];

  if (color_out) { printf("This program only processes B/W images, exit\n");    exit(0);  }

  printf("Convolution: size of window (3, 5, 7, ...)\n");
  printf("   # of rows: "); scanf("%d",&ni);
  printf("   # of columns: "); scanf("%d",&nj); 
  printf("size of the window: %d by %d\n",ni,nj);

  printf("Specify kernel:\n");
  printf("   overall coefficient: ");
  scanf("%f",&c);
  printf("   %d by %d elements:\n",ni,nj);
  for (m=0; m<ni; m++)
    for (n=0; n<nj; n++)
      { printf("w(%d,%d)=",m,n);
	scanf("%f",&kn[m][n]);
	kn[m][n]*=c;
      }

  ni=nj=3;
  ni1=ni-1; ni2=ni1/2;
  nj1=nj-1; nj2=nj1/2;
/*
  kn[0][0]=-1; kn[0][1]=-1; kn[0][2]=-1;
  kn[1][0]=-1; kn[1][1]=10; kn[1][2]=-1;
  kn[2][0]=-1; kn[2][1]=-1; kn[2][2]=-1;

  kn[0][0]=-1; kn[0][1]=0; kn[0][2]=1;
  kn[1][0]=-1; kn[1][1]=0; kn[1][2]=1;
  kn[2][0]=-1; kn[2][1]=0; kn[2][2]=1;

  kn[0][0]=-1; kn[0][1]=-1; kn[0][2]=-1;
  kn[1][0]=0; kn[1][1]=0; kn[1][2]=0;
  kn[2][0]=1; kn[2][1]=1; kn[2][2]=1;
*/
  
  M_out=M_in-ni1;
  N_out=N_in-nj1;

  printf("M_in=%d, N_in=%d\n",M_in,N_in);
  printf("M_out=%d, N_out=%d\n",M_out,N_out);

  min=1.0e9; max=-min;
  for (i=ni2; i<M_in-ni2; i++) 
    { for (j=nj2; j<N_in-nj2; j++) 
	{ w=0;
	  for (m=-ni2; m<=ni2; m++)
	    for (n=-nj2; n<=nj2; n++)
	      w+=temp2_in[i+m][j+n]*kn[m+ni2][n+nj2];
	  temp2_out[i-ni2][j-ni2]=w;
	  if (w > max) max=w;
	  if (w < min) min=w;
	}
      if (!i%10) printf("i=%d ",i-ni2);
    }

  w=255/(max-min);
  printf("min=%f, max=%f, w=%f\n",min,max,w);
  for (i=0; i<M_out; i++) 
    for (j=0; j<N_out; j++) 
      temp2_out[i][j]=w*(temp2_out[i][j]-min);
}



blur()
{}


/**********
 *
 * color_proc() - color image processing to increase I and S. 
 *
 **********/

color_proc()
{
  int i, j, k, L, min, max, valid;
  float cut_low, cut_high,p;
  float u,v,w,r,g,b,h,s,l, dh,ds,dl,sum;
  float ***tmp, hist[glevel];

  printf("Enhancsing color image...\n");
  if (!color_in) { printf("Not a color image, not processed. Exit\n");   exit(1);  }

  printf("Modifying Hue, enter rotation angle (-180 thru +180 degrees): ");
  scanf("%f",&dh);
  dh=dh*Pi/180.0;
  printf("Modifying Saturation, enter scaling factor (0 and up...): ");
  scanf("%f",&ds);

  printf("Linear stretch (0), Nonlinear map & stretch (1), or Histogram equalization (2) ? ");
  scanf("%d",&L);

  if (L==1)
    { printf("out=pow(in, p), p<1 to stretch low range, p>1 to stretch high range, p=");
      scanf("%f",&p); 
    }
  if (L<2)
    { valid=0;
      if (!valid)
	{ printf("cut-off intensity percentage at low end (e.g., 0.01, 0.03, etc.): ");
	  scanf("%f",&cut_low); 
	  if (cut_low<1.0) valid=1;
	}
      valid=0;
      if (!valid)
	{ printf("cut-off intensity percentage at high end (e.g., 0.01, 0.03, etc.): ");
	  scanf("%f",&cut_high); 
	  if (cut_high<1.0) valid=1;
	}
    }

  tmp=fl_alloc3d(3,M_in,N_in);

  //printf("Size of input image: %d by %d\n",M_in, N_in); pause();
  w=1.0/M_in/N_in;
  for (i=0; i<glevel; i++)  hist[i]=0;

  u=glevel-1;  v=1.0/u;
  for (i=0; i<M_in; i++)
    for (j=0; j<N_in; j++)
      { 
	r=temp3_in[0][i][j]*v;	g=temp3_in[1][i][j]*v;	b=temp3_in[2][i][j]*v;
	rgb_hsi(r,g,b,&h,&s,&l);
	//	l=0.5; h=1.5; s=0.5;
	if (L==1) l=pow(l, p);  // nonlinear mapping
	k=l*(glevel-1);
	//printf("(%d %d) [%.2f %.2f %.2f], [%.2f %.2f %.2f], k=%d\n",i,j,r,g,b,h,s,l,k);
	h=h+dh;  if (h>2*Pi) h=h-2*Pi;  if (h<0) h=h+2*Pi;	
	tmp[0][i][j]=h;  tmp[1][i][j]=s*ds;  tmp[2][i][j]=k;
	hist[k]+=w;
      }

  // for (k=0; k<glevel; k++) printf("%f  ",hist[k]);  printf("\n");  pause();

  if (L<2)
    { 
      w=0.0;   min=0;
      while (w < cut_low)   {  w+=hist[min]; min++; }
      w=0.0;   max=glevel-1;
      while (w < cut_high)  {  w+=hist[max]; max--; }
      w=(float)(glevel-1)/(max-min);
      printf("min=%d, max=%d, w=%f\n",min,max,w);

      for (k=0; k<glevel; k++)
	{
	  if (k<=min) lookup[k]=0;
	  else if (k>=max) lookup[k]=glevel-1;
	  else lookup[k]=w*(k-min);
	  // printf("%4d  ",lookup[k]); 
	}
    }
  else
    { sum=0;
      for (k=0; k<glevel; k++)
	{ sum+=hist[k];   lookup[k]=(int)((glevel-1)*sum);
	  printf("%4d  ",lookup[k]); 
	}
    }

  for (i=0; i<M_in; i++)
    for (j=0; j<N_in; j++)
      {
	h=tmp[0][i][j];  s=tmp[1][i][j];
	k=tmp[2][i][j];  l=v*lookup[k];

	hsi_rgb(h,s,l,&r,&g,&b);

	temp3_out[0][i][j]=r*u;
	temp3_out[1][i][j]=g*u;
	temp3_out[2][i][j]=b*u;
      }
}



rgb_hsi(R,G,B,H,S,I)
     float R, G, B, *H, *S, *I;
{ float r, g, b, h, s, i, w;

 w=R+G+B; i=w/3; 
 r=R/w; g=G/w; b=B/w;
 // printf("r=%.2f, g=%.2f, b=%.2f\n",r,g,b); 

 if (R==G && G==B)  s=h=0;
 else 
   { w=0.5*(R-G+R-B)/sqrtf((R-G)*(R-G)+(R-B)*(G-B));
     if (w>1) w=1;
     else if (w<-1) w=-1;
     h=acosf(w);
     if (B > G) h=Pi2-h;
     if (r <= g && r <= b) s=1-3*r;
     if (g <= r && g <= b) s=1-3*g;
     if (b <= r && b <= g) s=1-3*b;
   }
 // printf("H=%.2f, S=%.2f, I=%.2f\n", h*180/Pi, s, i); 
 *I=i; *S=s; *H=h;
}


hsi_rgb(H,S,I,R,G,B)
float *R, *G, *B, H, S, I;
{ float r, g, b;
/* 
  printf("I=%f, H=%f, S=%f\n\n", I, H*180/Pi, S);
*/
  if (S>1) S=1;
  if (I>1) I=1;
  if (S==0)
     *R=*G=*B=I;
  else
    { 
      if ( (H >= 0) && (H <2*Pi/3) )
	{ b=(1-S)/3;
	  r=(1+S*cos(H)/cos(Pi/3-H))/3;
	  g=1-r-b;
	}
      else if ( (H>=2*Pi/3) && (H<4*Pi/3) )
	{ H=H-2*Pi/3;
	  r=(1-S)/3;
	  g=(1+S*cos(H)/cos(Pi/3-H))/3;
	  b=1-r-g;
	} 
      else if ( (H>=4*Pi/3) && (H<2*Pi) )
	{ H=H-4*Pi/3;
	  g=(1-S)/3;
	  b=(1+S*cos(H)/cos(Pi/3-H))/3;
	  r=1-b-g;
	}
      else
	{ printf("\nH out of range: %f\n",H*180/Pi); pause();}
      if (r<0 || g<0 || b<0) 
	{ printf("\n\nr,g,b: %f, %f, %f",r,g,b);
	  printf("  h,s,i: %f, %f, %f\n",H,S,I);
	  pause();
	}
      if (r < 0) r=0;   if (g < 0) g=0;   if (b < 0) b=0;
      *R=3*I*r; *G=3*I*g; *B=3*I*b;
      if (*R > 1) *R=1;   if (*G > 1) *G=1;   if (*B > 1) *B=1;
    }

/*  printf("R=%6.2f, G=%6.2f, B=%6.2f\n\n",*R,*G,*B);  */
}

edge_detect()
{
  int i,j,k,l,m,n,kn[5][5];
  float u,v,w,min,max;

  if (color_out) { printf("This program only processes B/W images, exit\n");    exit(0);  }

  printf("Choose one mehtod: (1) Roberts, (2) Sobel, (3) Prewitt, (4) Laplas. \n");
  scanf("%d",&l);

  switch (l) {
  case 1:
    printf("Roberts operators, Size of mask is %d.\n",k=2);
    kn[0][0]=-1;   kn[1][0]=1;  kn[0][1]=kn[1][1]=0;
    break;
  case 2:
    printf("Sobel operators, Size of mask is %d.\n",k=3);
    for (i=0; i<3; i++)
      { kn[0][i]=-1;   kn[1][i]=0;  kn[2][i]=1; }
    kn[0][1]=-2;  kn[2][1]=2;
    break;
  case 3: 
    printf("Prewitt operators, Size of mask: 3, 4 or 5? ");
    scanf("%d",&k);
    if (k<=3 || k>5) 
      { if (k!=3) { printf("Size of mask k can only be 3 <= k <= 5. Assuming 3.\n"); k=3; }
	for (i=0; i<k; i++)
	  { kn[0][i]=-1; kn[1][i]=0; kn[2][i]=1; }
      }
    else if (k==4)
      for (i=0; i<k; i++)
	{ kn[0][i]=-3; kn[1][i]=-1; kn[2][i]=1; kn[3][i]=3; }
    else if (k==5)
      for (i=0; i<k; i++)
	{ kn[0][i]=-3; kn[1][i]=-1; kn[2][i]=0; kn[3][i]=1; kn[4][i]=3; }
    break;
  case 4:
    printf("Laplas operators, Size of mask is %d.\n   4-neighbor or 8-neighbor? ",k=3);
    scanf("%d",&j);
    if (j!=8)
      { if (j!=4) printf("either 4-neighbor or 8-neighbor. Assuming 4.\n");
	kn[0][0]=kn[0][2]=kn[2][0]=kn[2][2]=0;
	kn[0][1]=kn[1][0]=kn[2][1]=kn[1][2]=-1;
	kn[1][1]=4;
      }
    else 
      { kn[0][0]=kn[0][2]=kn[2][0]=kn[2][2]=kn[0][1]=kn[1][0]=kn[2][1]=kn[1][2]=-1;
	kn[1][1]=8;
      }
    break;
  }

  M_out=M_in-k+1;   N_out=N_in-k+1;
  for (i=0; i<M_out; i++) 
    for (j=0; j<N_out; j++) 
      { u=v=0;
	for (m=0; m<k; m++)
	  for (n=0; n<k; n++)
	    { u+=temp2_in[i+m][j+n]*kn[m][n];
	      v+=temp2_in[i+m][j+n]*kn[m][n];
	    }
	if (l==4) v=0;                  /* Laplas operator does not need this. */
	w=temp2_out[i][j]=sqrt(u*u+v*v);
	if (w > max) max=w;
	if (w < min) min=w;
      }
  w=(float)(glevel-1)/(max-min);
  printf("min=%f, max=%f, w=%f\n",min,max,w);
  for (i=0; i<M_out; i++) 
    for (j=0; j<N_out; j++) 
      temp2_out[i][j]=w*(temp2_out[i][j]-min);
}


linearmap()
{
  int i,j,k,l,m,n,K,x,y;
  int bp[2][64];
  float slope;

  if (color_out) { printf("This program only processes B/W images, exit\n");    exit(0);  }

  printf("Piecewise linear mapping, number of break points: ");
  scanf("%d",&K);

  for (k=0; k<K; k++)
    { printf("Break point %d, x y: ",k);
      scanf("%d %d",&x,&y);
      bp[0][k]=x;  bp[1][k]=y;       printf("(%d, %d)\n",x,y);
      lookup[x]=y;
      if (k>0)
	{ slope=(float)(bp[1][k]-bp[1][k-1])/(bp[0][k]-bp[0][k-1]);
	  printf("slope: %f\n",slope);
	  for (i=bp[0][k-1]+1; i<bp[0][k]; i++)
	    lookup[i]=bp[1][k-1]+slope*(i-bp[0][k-1]);
	}
    }
/*
  for (i=0; i<glevel; i++)
    printf("%3d  ",lookup[i]);
*/
  for (i=0; i<M_out; i++) 
    for (j=0; j<N_out; j++) 
      {
	k=temp2_in[i][j];
	temp2_out[i][j]=lookup[k];
      }

}
      

/**********
 *
 * median() - edge preservation using median filter
 *
 **********/

median()
{
  int i,j,k,l1,l2,m,n,sz,sz1,sz2,sp;
  short w, bin[625];
  float av, vr, vrmin;

  printf("Median filter: size of window (3, 5, 7, ...): ");
  scanf("%d",&sz); sz1=sz-1; sz2=sz1/2;
  printf("size of the window: %d\n",sz);

  printf("Shape of window: (0) cross, (1) diagonal, or (2) square? ");
  scanf("%d",&sp); 

  M_out=M_in-sz1;  N_out=N_in-sz1;
  printf("M_in=%d, N_in=%d\n",M_in,N_in);
  printf("M_out=%d, N_out=%d\n",M_out,N_out);

  if (color_out)
    { printf("This program does not process color images, Exit\n"); exit(0); }
  else
    {
      for (i=sz2; i<M_in-sz2; i++) {
	if (!(i%10)) printf("\ni=%d ",i);
	for (j=sz2; j<(N_in-sz2); j++) {
/*	  
	  for (m=i-sz2; m<=i+sz2; m++)
	    { for (n=j-sz2; n<=j+sz2; n++)
		printf("%d ",temp2_in[m][n]);
	      printf("\n");
	    }
	  printf("\n");
*/
	  if (sp==0)
	    { k=0;
	      for (m=i-sz2; m<=i+sz2; m++)
		{ bin[k]=temp2_in[m][j]; k++;}
	      for (n=j-sz2; n<=j+sz2; n++)
		if (n!=j) {bin[k]=temp2_in[i][n]; k++;}
	    }
	  else if (sp==1)
	    { k=0; l1=i-j; l2=i+j;
	      for (m=i-sz2; m<=i+sz2; m++)
		{ bin[k]=temp2_in[m][m-l1]; k++;
/*		  printf("(%d,%d) ",m,m-l1); */
		}
	      for (m=i-sz2; m<=i+sz2; m++)
		if (m!=i) 
		  { bin[k]=temp2_in[m][l2-m]; k++;
/*		    printf("(%d,%d) ",m,l2-m);  */
		  }
	    }
	  else
	    { k=0;
	      for (m=i-sz2; m<=i+sz2; m++)
		for (n=j-sz2; n<=j+sz2; n++)
		  { bin[k]=temp2_in[m][n]; k++; }
	    }
	  sort(bin,k); 
	  temp2_out[i-sz2][j-sz2]=bin[k/2];
/*	  printf("mid: %d ",bin[k/2]); pause();   */
	}
      }
    }
}

sort(bin,k)
short *bin; int k;
{
  int i,j;
  short w;
/*
  printf("\nk=%d\n",k);
  for (i=0; i<k; i++)
    printf("%d ",bin[i]);
*/
  for (i=0; i<k-1; i++)
    for (j=i+1; j<k; j++)
      if ( bin[i]>bin[j] )
	{ w=bin[i]; bin[i]=bin[j]; bin[j]=w; }
/*
  printf("\n");
  for (i=0; i<k; i++)
    printf("%d ",bin[i]);
*/
}

fl_dealloc2d(array,m,n)
float **array;
int m,n;
{ int i;
  for (i=0; i<m; i++)
      free(array[i]);
}

fl_dealloc3d(array,m,n,k)
float ***array;
int m,n,k;
{ int i,j;
  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      free(array[i][j]);
}


testfft1d()
{
  float xr[8], xi[8];
  int i,j,k,m=8;

  for (i=0; i<m; i++)
    { xr[i]=i; xi[i]=0.0; }
  for (i=0; i<m; i++)
    printf("%f, ",xr[i]);
  printf("\n");

  fft(xr,xi,m,0);

  for (i=0; i<m; i++)
    printf("%f, ",xr[i]);
  printf("\n");
  for (i=0; i<m; i++)
    printf("%f, ",xi[i]);
  printf("\n");

  fft(xr,xi,m,1);

  for (i=0; i<m; i++)
    printf("%f, ",xr[i]);
  printf("\n");
  for (i=0; i<m; i++)
    printf("%f, ",xi[i]);
  printf("\n");

}

/**********
 *
 * DFT_filter() - filtering in frequency domain
 *
 **********/

DFT_filtering()
{
  int i,j,k,m,n,p2;
  float c,w,min,max, *xr, *xi, **xxr, **xxi; 

/*  testfft1d(); pause(); */

  if (color_out) { printf("This program only processes B/W images, exit\n");    exit(0);  }

/*  M_in=8; N_in=8;  */

  m=M_in; n=N_in;
  i=w=log2((float)m);  j=w=log2((float)n);
  if ((k=pow(2.0,(float)i))==m && (k=pow(2.0,(float)j))==n) p2=1; else p2=0;

  printf("Image size: %d by %d, log2: %d %d \n",m,n,i,j);
  printf("Image size: %d by %d, log2: %d %d \n",m,n,i,j);
  if (p2) printf("m, n are power of 2, use fast DFT.\n");
  else printf("m, n are not power of 2, use regular DFT.\n");

  xxr = fl_alloc2d(m,n);
  xxi = fl_alloc2d(m,n);

/*
  for (i=0; i<m; i++) 
    for (j=0; j<n; j++) 
      temp2_in[i][j]=0;
  for (i=2; i<4; i++) 
    for (j=3; j<5; j++) 
      temp2_in[i][j]=4;

  printf("Date in spatial domain...");
  for (i=0; i<m; i++)
    { printf("\n");
      for (j=0; j<n; j++)
	printf("%5.1f ",temp2_in[i][j]);
    }
*/

  printf("Filtering, high-pass (0), low_pass (1), or directional (2): ");
  scanf("%d",&k);      

  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      { xxr[i][j]=temp2_in[i][j]; 
	if ((k) && ((i+j)%2)) xxr[i][j]=-xxr[i][j]; 
      }

  printf("Forward 2D-DFT...\n");
  if (p2) fft2d(xxr,xxi,m,n,0); else dft2d(xxr,xxi,m,n,0);

/*
  printf("Date in frequency domain...");
  for (i=0; i<m; i++)
    { printf("\n");
      for (j=0; j<n; j++)
	printf("%5.1f ",xxr[i][j]);
    }
  printf("\n");
  for (i=0; i<m; i++)
    { printf("\n");
      for (j=0; j<n; j++)
	printf("%5.1f ",xxi[i][j]);
    }
*/
  printf("2D Filtering...\n");
  Fourier_filter(xxr,xxi,m,n,k);

  printf("Inverse 2D-DFT...\n");
  if (p2) fft2d(xxr,xxi,m,n,1); else dft2d(xxr,xxi,m,n,1);

  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      if ((k) && ((i+j)%2)) xxr[i][j]=-xxr[i][j]; 
/*
  printf("Date in spatial domain again...");
  for (i=0; i<m; i++)
    { printf("\n");
      for (j=0; j<n; j++)
	printf("%5.1f ",xxr[i][j]);
    }
*/

  min=1.0e9; max=-min;
  printf("Writing to out_image...\n");
  for (i=0; i<M_out; i++)
    for (j=0; j<N_out; j++)
      { w=xxr[i][j]; if (min>w) min=w; if (max<w) max=w; }

  w=255/(max-min);

  printf("min=%f, max=%f, w=%f\n",min,max,w);
  for (i=0; i<M_out; i++) 
    for (j=0; j<N_out; j++) 
      temp2_out[i][j]=w*(xxr[i][j]-min);
  
  fl_dealloc2d(xxr,m,n);   fl_dealloc2d(xxi,m,n); 
}


fdft2d(xxr,xxi,m,n,inverse)
float **xxr,**xxi;
int m,n,inverse;
{
  int i,j,k;
  float *xr,*xi,u,v,w;
  
  k=m; if (n>m) k=n;
  xr = (float *) malloc(k*sizeof(float));
  xi = (float *) malloc(k*sizeof(float));

  printf("Forward 2DFFT\n   row transform...\n");
  for (i=0; i<m; i++)
    { for (j=0; j<n; j++)
	{ xr[j]=xxr[i][j];  xi[j]=0; }
      fft(xr,xi,n,inverse);
      for (j=0; j<n; j++)
	{ xxr[i][j]=xr[j]; xxi[i][j]=xi[j]; }
    }
  printf("   column transform...\n");
  for (j=0; j<n; j++)
    { for (i=0; i<m; i++)
	{ xr[i]=xxr[i][j]; xi[i]=xxi[i][j]; }
      fft(xr,xi,m,inverse);
      for (i=0; i<m; i++)
	{ xxr[i][j]=xr[i]; xxi[i][j]=xi[i]; }
    }
  free(xr);  free(xi);
}

ifdft2d(xxr,xxi,m,n,inverse)
float **xxr,**xxi;
int m,n,inverse;
{
  int i,j,k;
  float *xr,*xi,u,v,w;
  
  k=m; if (n>m) k=n;
  xr = (float *) malloc(k*sizeof(float));
  xi = (float *) malloc(k*sizeof(float));

  printf("   column transform...\n");
  for (j=0; j<n; j++)
    { for (i=0; i<m; i++)
	{ xr[i]=xxr[i][j]; xi[i]=xxi[i][j]; }
      fft(xr,xi,m,inverse);
      for (i=0; i<m; i++)
	{ xxr[i][j]=xr[i]; xxi[i][j]=xi[i]; }
    }
  printf("Forward 2DFFT\n   row transform...\n");
  for (i=0; i<m; i++)
    { for (j=0; j<n; j++)
	{ xr[j]=xxr[i][j];  xi[j]=0; }
      fft(xr,xi,n,inverse);
      for (j=0; j<n; j++)
	{ xxr[i][j]=xr[j]; xxi[i][j]=xi[j]; }
    }
  free(xr);  free(xi);
}


dft2d(xr,xi,m,n,inverse)    /* for arbitrary size DFT2D */ 
float **xr,**xi;
int m,n,inverse;
{
  int i,j,k;
  float **yr,**yi,**tr,**ti,af,u,v,w,wm,wn;

  k=m; if (n>m) k=n; 
  wm=Pi*2/m, wn=Pi*2/n;  w=1.0/sqrt((float)(m*n));
  tr=fl_alloc2d(k,k); ti=fl_alloc2d(k,k);    /* transfor matrices */
  yr=fl_alloc2d(m,n); yi=fl_alloc2d(m,n);    /* spectral matrics */

/* preparing for column transform matrix */  
  printf("Column transform...\n");
  for (i=0; i<m; i++)
    for (j=0; j<m; j++)
      { af=-i*j*wm;
	if (inverse) af=-af;
	tr[i][j]=w*cos(af); ti[i][j]=w*sin(af);
      }
/* matrix multiplication for column transform */
  for (j=0; j<n; j++)                 /* for all columns */
    for (i=0; i<m; i++)               /* for each element in the column */
      { yr[i][j]=yi[i][j]=0;
	for (k=0; k<m; k++)
	  { yr[i][j]+=tr[i][k]*xr[k][j]-ti[i][k]*xi[k][j];
	    yi[i][j]+=ti[i][k]*xr[k][j]+tr[i][k]*xi[k][j];
	  }
      }
/* preparing for row transform matrix */  
  printf("Row transform...\n");
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      { af=-i*j*wn;
	if (inverse) af=-af;
	tr[i][j]=w*cos(af); ti[i][j]=w*sin(af);
      }
/* matrix multiplication for row transform */
  for (i=0; i<m; i++)               /* for each element in the column */
    for (j=0; j<n; j++)                 /* for all columns */
      { xr[i][j]=xi[i][j]=0;
	for (k=0; k<n; k++)
	  { xr[i][j]+=yr[i][k]*tr[k][j]-yi[i][k]*ti[k][j];
	    xi[i][j]+=yi[i][k]*tr[k][j]+yr[i][k]*ti[k][j];
	  }
      }
  fl_dealloc2d(tr,k,k);  fl_dealloc2d(ti,k,k); 
  fl_dealloc2d(yr,m,n);	 fl_dealloc2d(yi,m,n);	
}


Fourier_filter(xxr,xxi,m,n,k)
float **xxr, **xxi;
int m,n,k;
{ 
  int D,i,j,l,M,N,m2=m/2,n2=n/2,flg;
  float af,c,s,cf,min,max,u,v,w,**spectrum;;

  spectrum=fl_alloc2d(m,n);

  if (k<2)              /* low or high pass filter */
    { printf("Type of filter, ideal (0), Butterworth (1) "); scanf("%d",&flg); }
  else                  /* directional filter */
    { flg=2; 
      printf("Directional filtering angle (0~180): ");
      scanf("%d",&D); af=Pi/180.0*D; c=cos(af); s=sin(af);
    }
  N=3;
  /* default normalized cutoff frequency */
  if (k) cf=0.05;               /* low-pass or directional filter */
  else cf=0.8;                  /* high-pass filter */
  printf("default normalized cutoff frequency: %.2f, use it (0) or change it (1)? ",cf);
  scanf("%d",&k);
  if (k==1) { printf("enter new cutoff frequency (0~1): "); scanf("%f",&cf); }

  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      { 
	u=1.0-(float)i/m2; v=1.0-(float)j/n2;  
	w=u*u+v*v;                             /* normalized distance to origin */
	switch(flg) {
	case 0:                                /* ideal filter */
	  v=1.0; if ( w > cf*cf) v=0.0; break;
	case 1:                                /* Butterworth filter */
	  v=1./(1.+pow( w/cf/cf, N) );  break;
	case 2:                                /* directional filter */
	  w=(u*c+v*s)/cf;
	  v=1.0/(1.0+0.414*pow(w*w, N)); 
	}
	xxr[i][j]*=v; xxi[i][j]*=v;
	spectrum[i][j]=pow(xxr[i][j]*xxr[i][j]+xxi[i][j]*xxi[i][j],0.02);
      }
  write_spectrum(spectrum,m,n);
}

write_spectrum(spectrum,m,n)
float **spectrum;
int m,n;
{ int i,j;
  float u,v,min,max; 
  short w;
  FILE *outfile;

  min=9e9; max=-min;
  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      { v=spectrum[i][j]; if (min>v) min=v;  if (max<v) max=v; }
  v=255.0/(max-min);
  printf("min=%f, max=%f, scale=%f\n",min,max,v);

  if ( (outfile = fopen("spectrum", "w")) == NULL) 
    { fprintf(stderr, "Unable to open 'spectrum' for writing, exiting.\n");
      exit(0);
    }
  fprintf(outfile,"%s\n%d %d\n%d\n", "P5",n,m,255);
  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      putc(w=v*(spectrum[i][j]-min) ,outfile);
  fclose(outfile);
}


fft2d(xxr,xxi,m,n,inverse)   /* 2D Fourier transform */
float **xxr, **xxi;
int m,n,inverse;
{ float *xr, *xi;
  int i,j,k;

  k=m; if (n>m) k=n;
  xr = (float *) malloc(k*sizeof(float));
  xi = (float *) malloc(k*sizeof(float));

  printf("\nRow xform...\n");
  for (j=0; j<n; j++)
    { 
      for (i=0; i<m; i++)
	{ xr[i]=xxr[i][j]; xi[i]=xxi[i][j]; }
      fft(xr,xi,m,inverse);
      for (i=0; i<m; i++)
	{ xxr[i][j]=xr[i]; xxi[i][j]=xi[i]; }
    }
  printf("\nColumn xform...\n");
  for (i=0; i<m; i++)
    { 
      for (j=0; j<n; j++)
	{ xr[j]=xxr[i][j]; xi[j]=xxi[i][j]; }
      fft(xr,xi,n,inverse);
      for (j=0; j<n; j++)
	{ xxr[i][j]=xr[j]; xxi[i][j]=xi[j]; }
    }
  free(xr);  free(xi);
}


fft(xr,xi,N,inverse)
float xr[],xi[];
int N,inverse;
{ int mmax,step,i,j,k,j1,m,n;
  float arg,s,c,w,tempr,tempi;
/*
  if (inverse) printf("IFFT of size %d \n",N);
  else printf("FFT of size %d \n",N);
*/
  m=w=log2((float)N);
  for (i=0; i<N; ++i) {       /* bit reversal */
    j=0;
    for (k=0; k<m; ++k)
      j=(j << 1) | (1 & (i >> k));
    if (j < i) 
      { SWAP(xr[i],xr[j]);  SWAP(xi[i],xi[j]); }
  }
  for (i=0; i<m; i++) {         /* for log N stages */
    n=w=pow(2.0,(float)i);  w=Pi/n; if (inverse) w=-w;
    k=0;
    while (k<N-1) {             /* for N components */
      for (j=0; j<n; j++) {     /* for each section */
	arg=-j*w; c=cos(arg); s=sin(arg); 
	j1=k+j;
	tempr=xr[j1+n]*c-xi[j1+n]*s;
	tempi=xi[j1+n]*c+xr[j1+n]*s;
	xr[j1+n]=xr[j1]-tempr;
	xi[j1+n]=xi[j1]-tempi;
	xr[j1]=xr[j1]+tempr;
	xi[j1]=xi[j1]+tempi;
      }
      k+=2*n;
    }
  }
  arg=1.0/sqrt((float)N);
  for (i=0; i<N; i++) 
    { xr[i]*=arg; xi[i]*=arg; }
}


