#include "SA/strings/strings.h"
#include "SA/graphics/graphics.h"
#include "SA/graphics/plot/plot.h"

#define MARGIN_X 40
#define MARGIN_Y 40
#define FONT_SIZE 12
#define FONT_PADDING 20

/*
This will display the maximum of digits in a given place.
Use the exponential notation if required
*/
static void display_maximum_digits(char* string, double number, int max_chars)
{
    int power = 0.0;
    int i = 0;
    while(number < 1.0)
    {
        number *= 10.0;
        power--;
    }
    while(number >= 10.0)
    {
        number /= 10.0;
        power++;
    }

    if(power >= max_chars || power < 2 - max_chars)
    {
        char str_pow[5];
        int len;
        SA_int64_to_str(str_pow, power);
        len = SA_strlen(str_pow);

        string[0] = '0' + (uint8_t)number;
        if(max_chars >= 4 + len)
        {
            string[1] = '.';
            for(i = 0; i < max_chars - 3 - len; i++)
            {
                number -= (uint8_t)number;
                number *= 10;
                string[2+i] = '0' + (uint8_t)number;
            }
        }
        else
        {
            i = -1;
        }
        string[2+i] = 'e';
        SA_strcpy(string + 3 + i, str_pow);

        return;
    }
    
    if(power >= 0)
    {
        while(power >= 0)
        {
            string[i] = '0' + (uint8_t)number;
            number -= (uint8_t)number;
            number *= 10.0;
            power--;
            max_chars--;
            i++;
        }
        if(max_chars-1 > 0)
        {
            string[i] = '.';
            i++;
            max_chars--;
        }
        else
        {
            max_chars = 0;
        }
    }
    else
    {
        string[i] = '0';
        i++;
        string[i] = '.';
        i++;
        power++;
        max_chars -= 2;
        while(power < 0)
        {
            string[i] = '0';
            i++;
            max_chars--;
            power++;
        }
    }
    while(max_chars > 0)
    {
        string[i] = '0' + (uint8_t)number;
        number -= (uint8_t)number;
        number *= 10.0;
        i++;
        max_chars--;
    }
    
    string[i] = '\0';
    
}

static double get_step(double min_bound, double max_bound, unsigned int n)
{
    double ideal_step = (max_bound - min_bound) / (double)(n);
    signed int i = 0;

    while(ideal_step < 1.0)
    {
        ideal_step *= 10.0;
        i--;
    }
    while(ideal_step >= 10.0)
    {
        ideal_step /= 10.0;
        i++;
    }

    ideal_step = (unsigned long long int)(ideal_step + 0.5);

    while(i > 0)
    {
        ideal_step *= 10.0;
        i--;
    }
    while(i < 0)
    {
        ideal_step /= 10.0;
        i++;
    }

    return ideal_step;
}

static double min_array(double* array, unsigned int array_size)
{
    double x = array[0];
    for(unsigned int i = 0; i < array_size; i++)
    {
        if(array[i] < x)
        {
            x = array[i];
        }
    }
    return x;
}

static double max_array(double* array, unsigned int array_size)
{
    double x = array[0];
    for(unsigned int i = 0; i < array_size; i++)
    {
        if(array[i] > x)
        {
            x = array[i];
        }
    }
    return x;
}

static double plot_axis_values(SA_GraphicsWindow* window, double* array, unsigned int array_size, SA_GraphicsRectangle* draw_area, uint32_t axis_color, SA_bool is_y_axis, double* min_el)
{
    char str[6];
    uint32_t xy_step;
    uint32_t xy;
    *min_el = min_array(array, array_size);
    double current = *min_el;
    double max = max_array(array, array_size);
    unsigned int points_number = (draw_area->height - 2*MARGIN_Y) / (FONT_SIZE + FONT_PADDING);
    double step = get_step(current, max, points_number-1);
    if(is_y_axis)
    {
        xy_step = (draw_area->height - 2*MARGIN_Y) / (points_number);
        xy = draw_area->height - MARGIN_Y;
    }
    else
    {
        xy_step = (draw_area->width - 2*MARGIN_X) / (points_number);
        xy = MARGIN_X;
    }

    if(is_y_axis)
    {
        while(xy > MARGIN_X+xy_step)
        {
            display_maximum_digits(str, current, 5);
            SA_graphics_vram_draw_text(window, 5, xy + 4, str, axis_color);
            SA_graphics_vram_draw_horizontal_line(window, MARGIN_X-3, MARGIN_X+3, xy, axis_color, 2);
            current += step;
            xy -= xy_step;
        }
        display_maximum_digits(str, current, 5);
        SA_graphics_vram_draw_text(window, 5, xy + 4, str, axis_color);
        SA_graphics_vram_draw_horizontal_line(window, MARGIN_X-3, MARGIN_X+3, xy, axis_color, 2);
    }
    else
    {
        while(xy < draw_area->width-MARGIN_X-xy_step)
        {
            display_maximum_digits(str, current, 4);
            SA_graphics_vram_draw_text(window, xy-10, draw_area->height - MARGIN_Y + 20, str, axis_color);
            SA_graphics_vram_draw_vertical_line(window, xy, draw_area->height-MARGIN_Y-3, draw_area->height-MARGIN_X+3, axis_color, 2);
            current += step;
            xy += xy_step;
        }
        display_maximum_digits(str, current, 4);
        SA_graphics_vram_draw_text(window, xy-10, draw_area->height - MARGIN_Y + 20, str, axis_color);
        SA_graphics_vram_draw_vertical_line(window, xy, draw_area->height-MARGIN_Y-3, draw_area->height-MARGIN_X+3, axis_color, 2);
    }

    return xy_step / step;
}

void SA_graphics_plot_continuous_graph(SA_GraphicsWindow* window, double* x_array, double* y_array, unsigned int array_size, SA_GraphicsRectangle* draw_area, uint32_t axis_color, uint32_t graph_color, uint32_t background_color)
{
    double min_x, min_y;
    double x_factor, y_factor;
    uint32_t y_bottom = draw_area->top_left_corner_y + draw_area->height - MARGIN_Y;
    SA_graphics_vram_draw_rectangle(window, draw_area->top_left_corner_x, draw_area->top_left_corner_y, draw_area->width, draw_area->height, background_color);
    SA_graphics_vram_draw_vertical_line(window, draw_area->top_left_corner_x + MARGIN_X, draw_area->top_left_corner_y + MARGIN_Y, y_bottom, axis_color, 2);
    SA_graphics_vram_draw_horizontal_line(window, draw_area->top_left_corner_x + MARGIN_X, draw_area->top_left_corner_x + draw_area->width - MARGIN_X, y_bottom, axis_color, 2);

    x_factor = plot_axis_values(window, x_array, array_size, draw_area, axis_color, SA_FALSE, &min_x);
    y_factor = plot_axis_values(window, y_array, array_size, draw_area, axis_color, SA_TRUE, &min_y);

    for(unsigned int i = 0; i < array_size-1; i++)
    {
        SA_graphics_vram_draw_line(window, MARGIN_X + (x_array[i]-min_x)*x_factor, y_bottom - (y_array[i] - min_y)*y_factor, MARGIN_X + (x_array[i+1]-min_x)*x_factor, y_bottom - (y_array[i+1] - min_y)*y_factor, graph_color, 2);
    }
}