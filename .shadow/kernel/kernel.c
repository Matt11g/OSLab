#include <am.h>
#include <amdev.h>
#include <klib.h>
#include <klib-macros.h>
#include "img.h"

#define SIDE 1

static int w, h;  // Screen size

/*#define KEYNAME(key) \
  [AM_KEY_##key] = #key,
static const char *key_names[] = { AM_KEYS(KEYNAME) };

static inline void puts(const char *s) {
  for (; *s; s++) putch(*s);
}

void print_key() {
  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
  ioe_read(AM_INPUT_KEYBRD, &event);
  if (event.keycode != AM_KEY_NONE && event.keydown) {
    puts("Key pressed: ");
    puts(key_names[event.keycode]);
    puts("\n");
  }
}*/

void get_key() {
  AM_INPUT_KEYBRD_T event = { .keycode = AM_KEY_NONE };
  ioe_read(AM_INPUT_KEYBRD, &event);
  if (event.keycode != AM_KEY_NONE && event.keydown) {
    if (event.keycode == AM_KEY_ESCAPE) halt(1);
  }
}

static void draw_tile(int x, int y, int w, int h, uint32_t color) {
  uint32_t pixels[w * h]; // WARNING: large stack-allocated memory
  AM_GPU_FBDRAW_T event = {
    .x = x, .y = y, .w = w, .h = h, .sync = 1,
    .pixels = pixels,
  };
  for (int i = 0; i < w * h; i++) {
    pixels[i] = color;
  }
  ioe_write(AM_GPU_FBDRAW, &event);
}

//int cnt = 0;
int ww = 400, hh = 300;
//img_rgb_len = 1192500
uint32_t get_rgb(int num) {
  return (img_rgb[3 * num] << 16) + (img_rgb[3 * num + 1] << 8) + (img_rgb[3 * num + 2]);
}

#define MIN(x, y) (x > y) ? y : x
void splash() {
  AM_GPU_CONFIG_T info = {0};
  ioe_read(AM_GPU_CONFIG, &info);
  w = info.width;
  h = info.height;  
  /*for (int x = 0; x * SIDE <= w; x ++) {
    for (int y = 0; y * SIDE <= h; y++) {
      //if ((x & 1) ^ (y & 1)) {
        //draw_tile(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
      //}
			draw_tile(x * SIDE, y * SIDE, SIDE, SIDE, get_rgb(x * ww / w + y * hh * h / h));
    }
  }*/
  for (int x = 0; x * SIDE <= MIN(ww, w); x ++) {
    for (int y = 0; y * SIDE <= MIN(hh, h); y++) {
      /*if ((x & 1) ^ (y & 1)) {
        draw_tile(x * SIDE, y * SIDE, SIDE, SIDE, 0xffffff); // white
      }*/
			draw_tile(x * SIDE, y * SIDE, SIDE, SIDE, get_rgb(x * ww + y));
    }
  }
}

// Operating system is a C program!
int main(const char *args) {
  ioe_init();

  /*puts("mainargs = \"");
  puts(args);  // make run mainargs=xxx
  puts("\"\n");*/

  splash();

  /*puts("Press any key to see its key code...\n");
  while (1) {
    print_key();
  }*/
  while (1) {
    get_key();
	}
  return 0;
}
