#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCKSIZE 512 /* Defining Constant Blocksize of 512 Bytes */

typedef uint8_t BYTE; /* Defining a byte type as uint8 representing unsigned 8 bit integer */

int main(int argc, char *argv[])
{
    /* checking the count of arguments (Should be equal to 2) */
    /* If not, print usage */

    if (argc == 2)
    {
        /* If the argument count is valid, then starts with opening the input file in read mode */

        FILE *inputfile = fopen(argv[1], "r");

        /* If its an invalid input file, then returns anything except 0 indicating the program did not execute normally */

        if (!inputfile)
        {
            return 2;
        }

        /* Define all the necessary variables */

        FILE *outputfile = NULL; /* Output file where the output is written to */
        char filename[8]; /* File name that holds 8 characters */
        BYTE buffer[512]; /* Buffer to hold the input file stream */
        int counter = 0; /* Counter to count the number of files written */

        /* Endless loop until end of input file is reached */

        while (!feof(inputfile))
        {
            /* Read the input file as per defined Blocksize and stream it into the defined buffer */
            /* Store the bytes in bytesread variable declared as size_t so as to get the number of elements to read/write */

            size_t bytesread = fread(buffer, BLOCKSIZE, 1, inputfile);

            /* A boolean function to check if it is a valid JPEG File based on first 4 bytes and what they represent */

            bool isValidJPEG = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;

            /* If it is a valid JPEG File and the output isn't NULL (Thus isn't the first file) */
            /* Close that first file, so we can move on to the next file */

            if (isValidJPEG && outputfile != NULL)
            {
                fclose(outputfile);
            }

            /* if it is a valid JPEG, then we create the file name using sprintf and open the outfile in the write mode */
            /* The counter is incremented so as to keep track of the files and name the files as well */

            if (isValidJPEG)
            {
                sprintf(filename, "%03i.jpg", counter);
                outputfile = fopen(filename, "w");
                counter++;
            }

            /* This condition handles the case where an output file is already open and this means it is a valid JPEG File */
            /* Because an output file is opened if and only the file is a valid JPEG file */

            if (outputfile != NULL)
            {
                fwrite(buffer, BLOCKSIZE, bytesread, outputfile);
            }
        }

        /* Finally, close all the files that were opened */

        fclose(inputfile);
        fclose(outputfile);
        return 0;
    }
    else
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
}
