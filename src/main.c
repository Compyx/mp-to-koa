/* vim: set et ts=4 sts=4 sw=4 fdm=marker syntax=c.doxygen: */

/** \file   main.c
 * \brief   Convert multipaint .bin to Koala
 * \author  Bas Wassink <b.wassink@ziggo.nl>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>


/** \brief  Program name */
#define PRG_NAME            "mp-to-koa"
/** \brief  Program version */
#define PRG_VERSION         "0.1"


/** \brief  Koala file size, excluding load address */
#define KOA_SIZE            10001
/** \brief  Koala bitmap size */
#define KOA_BITMAP_SIZE     8000
/** \brief  Koala bitmap offset */
#define KOA_BITMAP_OFFSET   0
/** \brief  Koala videoram size */
#define KOA_VIDRAM_SIZE     1000
/** \brief  Koala videoram offset */
#define KOA_VIDRAM_OFFSET   8000
/** \brief  Koala colorram size */
#define KOA_COLRRAM_SIZE    1000
/** \brief  Koala colorram offset */
#define KOA_COLRAM_OFFSET   9000
/** \brief  Koala background color offset */
#define KOA_BGCOLOR_OFFSET  10000


/** \brief  Multipaint file size */
#define MP_SIZE             88000
/** \brief  Multipaint bitmap offset */
#define MP_BITMAP_OFFSET    1024
/** \brief  Multipaint bitmap size: 1 byte per pixel */
#define MP_BITMAP_SIZE      (40 * 25 * 8 * 8)
/** \brief  Multipaint videoram-lo offset */
#define MP_VIDRAM_LO_OFFSET 65536
/** \brief  Multipaint videoram-hi offset */
#define MP_VIDRAM_HI_OFFSET (MP_VIDRAM_LO_OFFSET + 1000)
/** \brief  Multipaint colorram offset */
#define MP_COLRAM_OFFSET    (MP_VIDRAM_HI_OFFSET + 1000)



/** \brief  Koala file data
 */
static uint8_t koa_data[KOA_SIZE];

/** \brief  Multipaint .bin file data
 */
static uint8_t mp_data[MP_SIZE];


/** \brief  Display help
 */
static void show_help(void)
{
    printf("%s %s\n\n", PRG_NAME, PRG_VERSION);
    printf("Usage %s <multipaint-bin-file> <koala-file>\n", PRG_NAME);
}


/** \brief  Load multipaint .bin file \a path
 *
 * \param[in]   path    path to Multipaint .bin file
 *
 * \return  bool
 */
static bool load_mp(const char *path)
{
    FILE *fp;

    printf("Reading Multipaint image '%s' ... ", path);
    fp = fopen(path, "rb");
    if (fp == NULL) {
        perror(PRG_NAME);
        return false;
    }
    if (fread(mp_data, 1, MP_SIZE, fp) != MP_SIZE) {
        perror(PRG_NAME);
        fclose(fp);
        return false;
    }
    fclose(fp);
    printf("OK\n");
    return true;
}


/** \brief  Get state of bitmap bit (\a, \y)
 *
 * \param[in]   x   x pos
 * \param[in]   y   y pos
 *
 * \return  bit is on
 */
static bool get_bitmap_bit(unsigned int x, unsigned int y)
{
    size_t offset = MP_BITMAP_OFFSET + x + y * 320;

    return mp_data[offset] ? true : false;
}


/** \brief  Grab a byte from the MP bitmap
 *
 * \param[in]   r       row in screen (0-24)
 * \param[in]   c       column in screen (0-39)
 * \param[in]   b       byte in (r,c) (0-7)
 *
 * \return  byte at the above coordinates
 */
static uint8_t get_bitmap_byte(unsigned int r, unsigned int c, unsigned int b)
{
    unsigned int x;
    unsigned int y;
    uint8_t data = 0;

    x = c * 8;
    y = r * 8;

    for (unsigned int n = 0; n < 8; n++) {
        if (get_bitmap_bit(x + n, y + b)) {
            data |= (uint8_t)(1U << (7 - n));
        }
    }
    return data;
}



/** \brief  Convert MP bitmap data to normal koala bitmap data
 */
static void copy_bitmap(void)
{
    for (unsigned int row = 0; row < 25; row++) {
        for (unsigned int col = 0; col < 40; col++) {
            uint8_t bmp_char[8];
            for (unsigned int b = 0; b < 8; b++) {
                bmp_char[b] = get_bitmap_byte(row, col, b);
            }
            memcpy(koa_data + (row * 320 + col * 8), bmp_char, 8);
        }
    }
}


/** \brief  Combine vidram colors and copy into koala data
 */
static void copy_vidram(void)
{
    for (unsigned int i = 0; i < 1000; i++) {
        koa_data[KOA_VIDRAM_OFFSET + i] = (uint8_t)(
                mp_data[MP_VIDRAM_LO_OFFSET + i ] +
                (mp_data[MP_VIDRAM_HI_OFFSET + i] << 4));
    }
}


/** \brief  Copy MP colorram to Koala colorram
 */
static void copy_colram(void)
{
    memcpy(koa_data + KOA_COLRAM_OFFSET, mp_data + MP_COLRAM_OFFSET, 1001);
}



/** \brief  Write Koala file
 *
 * \param[in]   path    path to save Koala file to
 *
 * \return  bool
 */
static bool save_koa(const char *path)
{
    FILE *fp;

    printf("Writing Koala image '%s' ... ", path);

    fp = fopen(path, "wb");
    if (fp == NULL) {
        perror(PRG_NAME);
        return false;
    }

    /* write load address
     *
     * Yes, I should check if these fail.
     */
    fputc(0x00, fp);
    fputc(0x40, fp);

    if (fwrite(koa_data, 1, KOA_SIZE, fp) != KOA_SIZE) {
        perror(PRG_NAME);
        fclose(fp);
        return false;
    }

    fclose(fp);
    printf("OK\n");
    return true;
}



/** \brief  Program driver
 *
 * \param[in]   argc    argument count
 * \param[in]   argv    argument vector
 *
 * \return  0 on success
 */
int main(int argc, char *argv[])
{
    if (argc < 3) {
        show_help();
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        show_help();
        return EXIT_SUCCESS;
    }

    memset(koa_data, 0, KOA_SIZE);
    memset(mp_data, 0, MP_SIZE);

    if (!load_mp(argv[1])) {
        return EXIT_FAILURE;
    }

    copy_bitmap();
    copy_vidram();
    copy_colram();

    if (!save_koa(argv[2])) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
