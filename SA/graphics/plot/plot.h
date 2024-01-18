#ifndef SA_GRAPHICS_PLOT
    #define SA_GRAPHICS_PLOT

    #include "SA/global/global.h"

    void SA_graphics_plot_continuous_graph(SA_GraphicsWindow* window, double* x_array, double* y_array, unsigned int array_size, SA_GraphicsRectangle* draw_area, uint32_t axis_color, uint32_t graph_color, uint32_t background_color);

#endif