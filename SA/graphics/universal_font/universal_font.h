/*
=========================================================================
============================= UF File Format ============================
=========================================================================


|00 -  07|08 -  15|16 -  23|24 -  31|32 -  39|40 -  47|48 -  55|56 -  63|
|--------+--------+--------+--------+--------+--------+--------+--------|
| height | width  | bytes_per_glyph |           glyphs_number           |
|--------+--------+--------+--------+--------+--------+--------+--------|
|    unicode_code_points_number     |      unicode_sequences_size       |
|--------+--------+--------+--------+--------+--------+--------+--------|
|                                 flags                                 |
|--------+--------+--------+--------+--------+--------+--------+--------|
|      unicode_code_point (0)       |         glyph_offset (0)          |
|--------+--------+--------+--------+--------+--------+--------+--------|
|      unicode_code_point (1)       |         glyph_offset (1)          |
|--------+--------+--------+--------+--------+--------+--------+--------|
|      unicode_code_point (2)       |         glyph_offset (2)          |
|--------+--------+--------+--------+--------+--------+--------+--------|
|      unicode_code_point (3)       |         glyph_offset (3)          |
|--------+--------+--------+--------+--------+--------+--------+--------|
|                                  ...                                  |
|--------+--------+--------+--------+--------+--------+--------+--------|
|unicode_code_point(glyphs_number-1)|     glyph_id (glyphs_number-1)    |
|--------+--------+--------+--------+--------+--------+--------+--------|
|11111111 11111111 11111111 11111111|         sequence_size (0)         |
|--------+--------+--------+--------+--------+--------+--------+--------|
|         glyph_offset (0)          |         unicode_seq1 (0)          |
|--------+--------+--------+--------+--------+--------+--------+--------|
|         unicode_seq2 (0)          |11111111 11111111 11111111 11111111|
|--------+--------+--------+--------+--------+--------+--------+--------|
|         sequence_size (1)         |         glyph_offset (1)          |
|--------+--------+--------+--------+--------+--------+--------+--------|
|         unicode_seq1 (1)          |         unicode_seq2 (1)          |
|--------+--------+--------+--------+--------+--------+--------+--------|
|         unicode_seq3 (1)          |11111111 11111111 11111111 11111111|
|--------+--------+--------+--------+--------+--------+--------+--------|
|                ...                |         sequence_size (N)         |
|--------+--------+--------+--------+--------+--------+--------+--------|
|         glyph_offset (N)          |         unicode_seq1 (N)          |
|--------+--------+--------+--------+--------+--------+--------+--------|
|         unicode_seq2 (N)          |         unicode_seq2 (N)          |
|--------+--------+--------+--------+--------+--------+--------+--------|
*/



#ifndef SA_UNIVERSAL_FONT
    #define SA_UNIVERSAL_FONT

    #include "SA/global/global.h"

    #define SEQUENCE_SEPARATOR (uint32_t)(-1)

    enum SA_UNIVERSAL_FONT_FLAGS {
        SA_UF_MONOSPACE         = 1 << 0,
        SA_UF_HAS_UNICODE_TABLE = 1 << 1,
    };

    typedef struct _unicode_locator {
        uint32_t unicode_id;
        uint32_t glyphe_offset;
    } UnicodeLocator;

    typedef struct _unicode_seq_locator {
        uint32_t seq_size;
        uint32_t glyph_offset;
        uint32_t sequence[];
    } UnicodeSeqLocator;

    // struct packed with 64 bits chunks
    typedef struct _sa_universal_font {
        /*uint32_t foreground_color;
        uint32_t background_color;
        uint16_t display_width;
        uint16_t display_height;*/

        uint8_t glyph_height;
        uint8_t glyph_width;
        uint16_t bytes_per_glyph;
        uint32_t flags;
        uint32_t glyphs_number;
        uint32_t unicode_ids_number;
        uint32_t unicode_seq_ids_size;
        uint32_t padding;
        UnicodeLocator* unicode_ids;
        uint32_t* unicode_seq_ids;
        SA_byte* glyphs;
    } SA_UniversalFont;

    #ifdef __cplusplus
    extern "C"{
    #endif

    /**
     * @brief Load a SA_UniversalFont structure from a PSF font given its file path
     * 
     * @param dest Pointer to the SA_UniversalFont structure to fill
     * @param filepath File path of the PSF font file
     * @return - SA_FALSE if loading the font failed
     * @return - SA_TRUE if loading the font was successful
     */
    SA_bool SA_UF_load_psf_font_from_file(SA_UniversalFont* dest, const char* filepath);

    #ifdef __cplusplus
    }
    #endif

#endif