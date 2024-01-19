#include "SA/strings/strings.h"
#include "SA/memory/mem_alloc.h"
#include "SA/graphics/graphics.h"

#define FONT_SIZE_X 6
#define FONT_SIZE_Y 12


struct _SA_graphics_text_input {
    SA_GraphicsWindow* window;
    char* text;
    uint32_t max_text_size;
    uint32_t min_x;
    uint32_t min_y;
    uint32_t max_x;
    uint32_t max_y;
    uint32_t padding_x;
    uint32_t padding_y;
    uint32_t background_color;
    uint32_t text_color;
    SA_bool has_focus;
};


SA_GraphicsTextInput* SA_graphics_create_text_input(SA_GraphicsWindow* window, uint32_t x, uint32_t y, uint32_t background_color, uint32_t text_color, uint32_t max_text_size, uint32_t padding_x, uint32_t padding_y)
{
    SA_GraphicsTextInput* text_input = (SA_GraphicsTextInput*) SA_malloc(sizeof(SA_GraphicsTextInput));
    if(text_input == NULL)
    {
        return NULL;
    }
    text_input->text = (char*) SA_malloc(max_text_size+1);
    if(text_input->text == NULL)
    {
        SA_free(&text_input);
        return NULL;
    }
    text_input->text[0] = '\0';
    text_input->max_text_size = max_text_size;
    text_input->window = window;
    text_input->background_color = background_color;
    text_input->text_color = text_color;
    text_input->min_x = x;
    text_input->min_y = y;
    text_input->max_x = x + FONT_SIZE_X * max_text_size + 2 * padding_x;
    text_input->max_y = y + FONT_SIZE_Y + 2 * padding_y;
    text_input->padding_x = padding_x;
    text_input->padding_y = padding_y;
    text_input->has_focus = SA_FALSE;

    SA_graphics_vram_draw_rectangle(window, x, y, FONT_SIZE_X * max_text_size + 2 * padding_x, FONT_SIZE_Y + 2 * padding_y, background_color);

    return text_input;
}


SA_bool SA_graphics_handle_text_input_events(SA_GraphicsTextInput* text_input, const SA_GraphicsEvent* event)
{
    if(event->event_type == SA_GRAPHICS_EVENT_MOUSE_LEFT_CLICK_DOWN)
    {
        text_input->has_focus = (text_input->min_x <= event->events.mouse.x && event->events.mouse.x <= text_input->max_x) &&
                                (text_input->min_y <= event->events.mouse.y && event->events.mouse.y <= text_input->max_y);
        
        return text_input->has_focus;
    }
    if(!text_input->has_focus)
    {
        return SA_FALSE;
    }

    if(event->event_type == SA_GRAPHICS_EVENT_KEY_DOWN)
    {
        uint32_t actual_size = SA_strlen(text_input->text);
        if(event->events.key.keycode == SA_GRAPHICS_KEYCODE_BACKSPACE)
        {
            if(text_input->text[0] != '\0')
            {
                text_input->text[actual_size-1] = '\0';
            }
        }
        else if(actual_size < text_input->max_text_size)
        {
            char c = event->events.key.str[0];
            if(c >= ' ')  // printable chars
                SA_strcpy(text_input->text + actual_size, event->events.key.str);
        }
        SA_graphics_vram_draw_rectangle(text_input->window, text_input->min_x, text_input->min_y, text_input->max_x - text_input->min_x, text_input->max_y - text_input->min_y, text_input->background_color);
        SA_graphics_vram_draw_text(text_input->window, text_input->min_x + text_input->padding_x, text_input->min_y + 10 + text_input->padding_y, text_input->text, text_input->text_color);
        SA_graphics_vram_flush(text_input->window);
    }

    return SA_TRUE;
}

const char* SA_graphics_get_text_input_value(SA_GraphicsTextInput* text_input)
{
    return text_input->text;
}

void SA_graphics_get_text_input_rectangle(SA_GraphicsTextInput* text_input, SA_GraphicsRectangle* rectangle)
{
    rectangle->top_left_corner_x = text_input->min_x;
    rectangle->top_left_corner_y = text_input->min_y;
    rectangle->width = text_input->max_x - text_input->min_x;
    rectangle->height = text_input->max_y - text_input->min_y;
}

void SA_graphics_free_text_input(SA_GraphicsTextInput** text_input)
{
    if(*text_input == NULL)
    {
        return;
    }
    SA_free(&((*text_input)->text));
    SA_free(text_input);
}