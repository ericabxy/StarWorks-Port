/* Copyright 2026 Eric Abides
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "libretro.h"

#define LIBRARY_NAME "StarWorks Port"
#define LIBRARY_VERSION "v1"

#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240
#define VIDEO_PIXELS VIDEO_WIDTH * VIDEO_HEIGHT
#define VIDEO_ASPECT_RATIO 4.0f / 3.0f

#define NUMBER_OF_CONTROLS 2

/* From audio.h */
#define SAMPLE_RATE 30000.0f

/* From video.h */
#define FRAME_RATE 60.0f

/* --------------------------------------------------------------------
 *                    DECLARE AND INITIALIZE GLOBALS
 * --------------------------------------------------------------------
 */
static uint32_t *frame_buf;
static struct retro_log_callback logging;
static retro_log_printf_t log_cb;
//static bool analog_mouse = true;
//static bool analog_mouse_relative = false;
static bool enable_audio = true;

static void fallback_log(enum retro_log_level level, const char *fmt, ...)
{
   (void)level;
   va_list va;
   va_start(va, fmt);
   vfprintf(stderr, fmt, va);
   va_end(va);
}

void retro_init(void)
{
   frame_buf = calloc(VIDEO_PIXELS, sizeof(uint32_t));

   /* Initialize state machine variables here. */
}

void retro_deinit(void)
{
   /* Free all dynamically allocated memory here. */
   free(frame_buf);
   frame_buf = NULL;
}

unsigned retro_api_version(void)
{
   return RETRO_API_VERSION;
}

void retro_set_controller_port_device(unsigned port, unsigned device)
{
   log_cb(RETRO_LOG_INFO, "Plugging device %u into port %u.\n", device, port);
}

void retro_get_system_info(struct retro_system_info *info)
{
   memset(info, 0, sizeof(*info));
   info->library_name     = LIBRARY_NAME;
   info->library_version  = LIBRARY_VERSION;
   info->need_fullpath    = false;
   info->valid_extensions = NULL;
}

/* --------------------------------------------------------------------
 *                 CALLBACKS FOR THE LIBRETRO FRONTEND
 * --------------------------------------------------------------------
 */
static retro_video_refresh_t video_cb;
static retro_audio_sample_t audio_cb;
static retro_audio_sample_batch_t audio_batch_cb;
static retro_environment_t environ_cb;
static retro_input_poll_t input_poll_cb;
static retro_input_state_t input_state_cb;

void retro_get_system_av_info(struct retro_system_av_info *info)
{
   float aspect = VIDEO_ASPECT_RATIO;

   info->timing = (struct retro_system_timing) {
      .fps = FRAME_RATE,
      .sample_rate = SAMPLE_RATE,
   };

   info->geometry = (struct retro_game_geometry) {
      .base_width   = VIDEO_WIDTH,
      .base_height  = VIDEO_HEIGHT,
      .max_width    = VIDEO_WIDTH,
      .max_height   = VIDEO_HEIGHT,
      .aspect_ratio = aspect,
   };
}

void retro_set_environment(retro_environment_t cb)
{
   environ_cb = cb;

   bool no_content = true;
   cb(RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME, &no_content);

   if (cb(RETRO_ENVIRONMENT_GET_LOG_INTERFACE, &logging))
      log_cb = logging.log;
   else
      log_cb = fallback_log;
}

void retro_set_audio_sample(retro_audio_sample_t cb)
{
   audio_cb = cb;
}

void retro_set_audio_sample_batch(retro_audio_sample_batch_t cb)
{
   audio_batch_cb = cb;
}

void retro_set_input_poll(retro_input_poll_t cb)
{
   input_poll_cb = cb;
}

void retro_set_input_state(retro_input_state_t cb)
{
   input_state_cb = cb;
}

void retro_set_video_refresh(retro_video_refresh_t cb)
{
   video_cb = cb;
}

/* --------------------------------------------------------------------
 *                         CORE STATE MANAGEMENT
 * --------------------------------------------------------------------
 */
static int x_coord;
static int y_coord;
static unsigned phase;
//static int mouse_rel_x;
//static int mouse_rel_y;

void retro_reset(void)
{
   x_coord = 0;
   y_coord = 0;
}

static void update_input(void)
{
   int port = 0;

   int dir_x = 0;
   int dir_y = 0;

   input_poll_cb();

   for (port = 0; port < NUMBER_OF_CONTROLS; port ++)
   {
      if (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP))
         dir_y--;
      if (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN))
         dir_y++;
      if (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT))
         dir_x--;
      if (input_state_cb(port, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT))
         dir_x++;

      /* Respond to controller input here. */

      x_coord = (x_coord + dir_x) & 31;
      y_coord = (y_coord + dir_y) & 31;
   }
}

static void video_callback(void)
{
   /* Try rendering straight into VRAM if we can. */
   uint32_t *buf = NULL;
   unsigned stride = 0;
   struct retro_framebuffer fb = {0};
   fb.width = VIDEO_WIDTH;
   fb.height = VIDEO_HEIGHT;
   fb.access_flags = RETRO_MEMORY_ACCESS_WRITE;
   if (environ_cb(RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER, &fb) && fb.format == RETRO_PIXEL_FORMAT_XRGB8888)
   {
      buf = fb.data;
      stride = fb.pitch >> 2;
   }
   else
   {
      buf = frame_buf;
      stride = VIDEO_WIDTH;
   }

   /* Update video buffer here. */

   video_cb(buf, VIDEO_WIDTH, VIDEO_HEIGHT, stride << 2);
}

static void check_variables(void)
{
}

static void audio_callback(void)
{
   if (!enable_audio)
      return;

   for (unsigned i = 0; i < SAMPLE_RATE / FRAME_RATE; i++, phase++)
   {
      int16_t val = 0x800 * sinf(2.0f * M_PI * phase * 300.0f / SAMPLE_RATE);
      audio_cb(val, val);
   }

   phase %= 100;
}

void retro_run(void)
{
   update_input();
   video_callback();
   audio_callback();

   /* Update game machine state here. */

   bool updated = false;
   if (environ_cb(RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE, &updated) && updated)
      check_variables();
}

bool retro_load_game(const struct retro_game_info *info)
{
   enum retro_pixel_format fmt = RETRO_PIXEL_FORMAT_XRGB8888;
   if (!environ_cb(RETRO_ENVIRONMENT_SET_PIXEL_FORMAT, &fmt))
   {
      log_cb(RETRO_LOG_INFO, "XRGB8888 is not supported.\n");
      return false;
   }

   check_variables();

   (void)info;
   return true;
}

void retro_unload_game(void)
{
}

unsigned retro_get_region(void)
{
   return RETRO_REGION_NTSC;
}

bool retro_load_game_special(unsigned type, const struct retro_game_info *info, size_t num)
{
   if (type != 0x200)
      return false;
   if (num != 2)
      return false;
   return retro_load_game(NULL);
}

size_t retro_serialize_size(void)
{
   return 2;
}

bool retro_serialize(void *data_, size_t size)
{
   if (size < 2)
      return false;

   uint8_t *data = data_;
   data[0] = x_coord;
   data[1] = y_coord;
   return true;
}

bool retro_unserialize(const void *data_, size_t size)
{
   if (size < 2)
      return false;

   const uint8_t *data = data_;
   x_coord = data[0] & 31;
   y_coord = data[1] & 31;
   return true;
}

void *retro_get_memory_data(unsigned id)
{
   (void)id;
   return NULL;
}

size_t retro_get_memory_size(unsigned id)
{
   (void)id;
   return 0;
}

void retro_cheat_reset(void)
{}

void retro_cheat_set(unsigned index, bool enabled, const char *code)
{
   (void)index;
   (void)enabled;
   (void)code;
}

