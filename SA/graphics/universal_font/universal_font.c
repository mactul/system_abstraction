#include <stdio.h>
#include <stdlib.h>
#include "SA/maths/maths.h"
#include "SA/strings/strings.h"
#include "SA/memory/mem_alloc.h"
#include "SA/memory/dynamic_array.h"
#include "SA/graphics/universal_font/universal_font.h"

#define PSF1_FONT_MAGIC 0x436
#define PSF2_FONT_MAGIC 0x864AB572

enum PSF1_FONT_MODES {
    PSF1_MODE512    = 1 << 0,
    PSF1_MODEHASTAB = 1 << 1,
    PSF1_MODESEQ    = 1 << 2,
};

enum PSF2_FLAGS {
    PSF2_HAS_UNICODE_TABLE = 1 << 0,
};

typedef struct _psf1_font {
    uint16_t magic;
    uint8_t font_mode;
    uint8_t character_size;
} __attribute__((packed)) PSF1_Font;

typedef struct __attribute__((packed)) _psf2_font {
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t flags;
    uint32_t glyph_num;
    uint32_t bpg;
    uint32_t height;
    uint32_t width;
} PSF2_Font;

typedef struct _glyph_id {
    uint32_t unicode_id;
    uint32_t glyph_index;
} GlyphId;

static void SA_UF_load_psf1_font_header(SA_UniversalFont* dest, const PSF1_Font* src)
{
    dest->background_color = 0x00000000;
    dest->foreground_color = 0x00FFFFFF;
    dest->display_width = 8;
    dest->display_height = src->character_size;
    dest->glyph_height = src->character_size;
    dest->glyph_width = 8;
    dest->bytes_per_glyph = dest->glyph_height * 1;
    dest->glyph_number = (src->font_mode & PSF1_MODE512) ? 512: 256;
    dest->flags = SA_UF_MONOSPACE | ((src->font_mode & (PSF1_MODEHASTAB | PSF1_MODESEQ)) ? SA_UF_HAS_UNICODE_TABLE: 0);
}

static void SA_UF_load_psf2_font_header(SA_UniversalFont* dest, const PSF2_Font* src)
{
    dest->background_color = 0x00000000;
    dest->foreground_color = 0x00FFFFFF;
    dest->display_width = src->height;
    dest->display_height = src->height;
    dest->flags = SA_UF_MONOSPACE | ((src->flags & PSF2_HAS_UNICODE_TABLE) ? SA_UF_HAS_UNICODE_TABLE: 0);
    dest->glyph_height = src->height;
    dest->glyph_width = src->width;
    dest->bytes_per_glyph = src->bpg;
    dest->glyph_number = src->glyph_num;
}

char SA_UF_load_psf_font_header(SA_UniversalFont* dest, FILE* file)
{
    uint32_t data;
    if(fread(&data, sizeof(uint32_t), 1, file) <= 0)
    {
        return 0;
    }

    if(((PSF1_Font*)(&data))->magic == PSF1_FONT_MAGIC)
    {
        SA_UF_load_psf1_font_header(dest, (PSF1_Font*)(&data));
        return 1;
    }
    if(data == PSF2_FONT_MAGIC)
    {
        PSF2_Font font;
        fread((uint32_t*)(&font) + 1, sizeof(PSF2_Font) - sizeof(uint32_t), 1, file);
        SA_UF_load_psf2_font_header(dest, &font);
        return 2;
    }

    return 0;
}

SA_DynamicArray* SA_UF_PSF1_get_ids(FILE* unicode_table_file, uint32_t glyph_number)
{
    uint32_t glyph_index = 0;
    SA_DynamicArray* ids = SA_dynarray_create_size_hint(GlyphId, glyph_number);
    for(uint64_t i = 0; i < glyph_number; i++)
    {
        uint16_t id = 0;
        do
        {
            fread(&id, sizeof(id), 1, unicode_table_file);
            if(id < 0xFFFE)
            {
                GlyphId g_id = {.glyph_index = glyph_index, .unicode_id = 0};
                if(i < SA_dynarray_size(ids))
                {
                    // already uncountered
                    g_id = SA_dynarray_get(GlyphId, ids, i);
                }
                g_id.unicode_id = (g_id.unicode_id << 16) | (uint32_t)id;
                SA_dynarray_set(GlyphId, ids, i, g_id);
            }
        } while(id < 0xFFFE);

        if(id == 0xFFFF)
            glyph_index++;
        else
            glyph_number++;
    }
    return ids;
}

static int cmp_glyphs(const void* e1, const void* e2)
{
    return *((int*)e1) - *((int*)e2);
}

static inline uint32_t SA_UF_get_unicode_at_index(const SA_UniversalFont* font, uint32_t index)
{
    return ((SA_UF_Glyph*)((SA_byte*)font->glyphs + index * (font->bytes_per_glyph + sizeof(uint32_t))))->unicode_id;
}

SA_byte* SA_UF_get_glyph(const SA_UniversalFont* font, uint32_t unicode_char)
{
    uint32_t min_index = 0;
    uint32_t max_index = 
}

SA_bool SA_UF_load_psf_font_from_file(SA_UniversalFont* dest, const char* filepath)
{
    FILE* file = fopen(filepath, "rb");

    if(file == NULL)
    {
        return SA_FALSE;
    }

    switch(SA_UF_load_psf_font_header(dest, file))
    {
        case 0:
            break;
        case 1:
            uint32_t glyphs_pos = ftell(file);
            fseek(file, dest->glyph_number * dest->bytes_per_glyph, SEEK_CUR);
            const SA_DynamicArray* ids = SA_UF_PSF1_get_ids(file, dest->glyph_number);
            dest->glyphs = SA_malloc(SA_dynarray_size(ids) * (dest->bytes_per_glyph + sizeof(uint32_t)));
            fseek(file, glyphs_pos, SEEK_SET);
            SA_byte* glyph_ptr = dest->glyphs;
            for(uint64_t i = 0; i < SA_dynarray_size(ids); i++)
            {
                GlyphId* g_id = _SA_dynarray_get_element_ptr(ids, i);

                fread(((SA_UF_Glyph*)glyph_ptr)->glyph, dest->bytes_per_glyph, 1, file);
                ((SA_UF_Glyph*)glyph_ptr)->unicode_id = g_id->unicode_id;

                while(g_id->glyph_index == (g_id+1)->glyph_index)
                {
                    SA_memcpy(((SA_UF_Glyph*)(glyph_ptr + dest->bytes_per_glyph + sizeof(uint32_t)))->glyph, ((SA_UF_Glyph*)glyph_ptr)->glyph, dest->bytes_per_glyph);
                    glyph_ptr += dest->bytes_per_glyph + sizeof(uint32_t);
                    g_id++;
                    i++;
                    ((SA_UF_Glyph*)glyph_ptr)->unicode_id = g_id->unicode_id;
                }

                glyph_ptr += dest->bytes_per_glyph + sizeof(uint32_t);
            }
            qsort(dest->glyphs, SA_dynarray_size(ids), dest->bytes_per_glyph + sizeof(uint32_t), cmp_glyphs);
            break;
        case 2:
            // not implemented
            fclose(file);
            return SA_FALSE;
    }
    

    fclose(file);

    return SA_TRUE;
}