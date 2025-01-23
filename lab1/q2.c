#include <stdio.h>
#include <stdlib.h> // For exit()
int main()
{
FILE *fptr1, *fptr2;
int offset=0;
char filename[100], c;
printf("Enter the filename to open for reading: \n");
scanf("%s", filename);
fptr1 = fopen(filename, "r");
// Open one file for reading
if (fptr1 == NULL)
{
printf("Cannot open file %s \n", filename);
exit(0);
}
printf("Enter the filename to open for writing: \n");
scanf("%s", filename);
fptr2 = fopen(filename, "w+"); // Open another file for writing
fseek(fptr1,0,SEEK_END);
int max=ftell(fptr1);
c = fgetc(fptr1);
// Read contents from file
while (offset <= max)
{
fputc(c,fptr2);
offset++;
fseek(fptr1,-(offset*sizeof(char)),SEEK_END);
c = fgetc(fptr1);
}
printf("\nContents copied to %s", filename);
fclose(fptr1);
fclose(fptr2);
return 0;
}