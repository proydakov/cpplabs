/*
 *  Copyright (c) 2011 Dmitry Suvorov <D.Suvorov90@gmail.com>
 */

#include <cv_viewer.h>

#include <landscape_generator.h>

// map configure
static const int map_width = 640;
static const int map_height = 480;

static const int pixel_size = 4;

static const int map_lowland = 0;
static const int map_hill = 160;

int main(int argc, char** argv)
{ 
    (void)argc;
    (void)argv;
    
    cv_viewer::task_set task;
    task.insert(&generator_mount);
    task.insert(&two_dimensional_random_generator);
    
    cv_viewer::cv_viewer viewer(map_height, map_width, map_lowland, map_hill, pixel_size);
    viewer.set_task(task);
    viewer.start("landscape");
    viewer.stop();
    
    return 0;
}