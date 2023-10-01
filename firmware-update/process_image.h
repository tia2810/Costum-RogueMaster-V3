#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

#include <esp_camera.h>
#include <FS.h>

#include "camera_model.h"
#include "dithering.h"

/** Process and send grayscale images back to the Flipper Zero. */
void process_image(camera_fb_t *frame_buffer);

#endif
