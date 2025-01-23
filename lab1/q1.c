#include <stdio.h>
#include <stdlib.h> // For exit()
int main()
{
FILE *fptr1, *fptr2;
int characters=0,lines=0;
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
c = fgetc(fptr1);
// Read contents from file
while (c != EOF)
{
characters++;
if(c =='\n'){
    lines++;
}
c = fgetc(fptr1);
}
printf("\nNumber of characters %d", characters);
printf("\nNumber of lines %d", lines+1);
fclose(fptr1);
return 0;
}