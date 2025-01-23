#include <stdio.h>
#include <stdlib.h> // For exit()
int main()
{
FILE *fptr1, *fptr2,*fptr3;
char filename[100], c,d;
int flip=0;
printf("Enter the filename to open for reading: \n");
scanf("%s", filename);
fptr1 = fopen(filename, "r");
// Open one file for reading
if (fptr1 == NULL)
{
printf("Cannot open file %s \n", filename);
exit(0);
}
printf("Enter the 2nd filename to open for reading: \n");
scanf("%s", filename);
fptr2 = fopen(filename, "r");
// Open one file for reading
if (fptr2 == NULL)
{
printf("Cannot open file %s \n", filename);
exit(0);
}
printf("Enter the filename to open for writing: \n");
scanf("%s", filename);
fptr3 = fopen(filename, "w+"); // Open another file for writing
c = fgetc(fptr1);
d=fgetc(fptr2);
// Read contents from file
while (d != EOF && c != EOF)
{
while(flip%2==0 && c!=EOF ){
    fputc(c, fptr3);
    c = fgetc(fptr1);
    if(c=='\n')
    {break;}
}
fputc('\n', fptr3);
// fseek(fptr1,1,SEEK_CUR);
// c=fgetc(fptr1);
flip++;
while(flip%2!=0 && d!=EOF){
  fputc(d, fptr3);
  d = fgetc(fptr2);
  if(d =='\n'){
    break;
  }
}
// fputc(c,fptr3);
// fputc('w',fptr3);
// fseek(fptr2,1,SEEK_CUR);
// d=fgetc(fptr2);
flip++;
printf("%d",flip);
}

printf("\nContents copied to %s", filename);
fclose(fptr1);
fclose(fptr2);
fclose(fptr3);
return 0;
}