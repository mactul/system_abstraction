#ifndef SA_GRAPHICS_PLOT
    #define SA_GRAPHICS_PLOT

    #include "SA/global/global.h"

    /**
     * @brief Draw a continuous graph inside a graphical window
     * 
     * @param window Pointer to the SA_GraphicsWindow structure related to this window
     * @param x_array Array of doubles making the abscissa
     * @param y_array Array of doubles making the ordinates
     * @param array_size Length of both arrays
     * @param draw_area Pointer to a SA_GraphicsRectangle structure containing the horizontal and vertical position and size of the drawing area
     * @param axis_color Color for the axis scales
     * @param graph_color Color for the plotting color
     * @param background_color Background color of this plot
     */
    void SA_graphics_plot_continuous_graph(SA_GraphicsWindow* window, double* x_array, double* y_array, unsigned int array_size, SA_GraphicsRectangle* draw_area, uint32_t axis_color, uint32_t graph_color, uint32_t background_color);

#endif