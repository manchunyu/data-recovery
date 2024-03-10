#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int BLOCKSIZE = 512;
typedef uint8_t BYTE;

bool is_jpeg(BYTE *arr);
void generate_name(char *name, int *c);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "rb");
    if (card == NULL)
    {
        return 1;
    }

    BYTE *block = malloc(BLOCKSIZE);
    if (block == NULL)
    {
        fclose(card);
        return 1;
    }

    int counter = 0;
    bool found = false;
    char filename[8];
    FILE *f = NULL;

    while (fread(block, BLOCKSIZE, 1, card) != 0)
    {
        if (is_jpeg(block))
        {
            if (found)
            {
                fclose(f);
            }

            generate_name(filename, &counter);
            f = fopen(filename, "wb");
            fwrite(block, BLOCKSIZE, 1, f);
            found = true;
        }
        else if (found)
        {
            fwrite(block, BLOCKSIZE, 1, f);
        }
    }

    free(block);
    fclose(f);
    fclose(card);
    return 0;
}

bool is_jpeg(BYTE *arr)
{
    if (arr[0] == 0xff && arr[1] == 0xd8 && arr[2] == 0xff && arr[3] >= 0xe0 && arr[3] <= 0xef)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void generate_name(char *name, int *c)
{
    sprintf(name, "%03i.jpg", *c);
    *c += 1;
}
