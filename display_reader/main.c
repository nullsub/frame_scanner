/* ImLib2 Header */
#include <X11/Xlib.h>       /* needed by Imlib2.h */
#include <Imlib2.h>

/* standard things */
#include <stdio.h>          /* puts, printf, BUFSIZ, perror, FILE */
#include <stdlib.h>         /* exit */

/* string manipulation */
#include <string.h>         /* strcpy */

#define HORIZ_UP 1
#define VERT_LEFT_UP 2
#define VERT_RIGHT_UP 4
#define HORIZ_MID 8
#define VERT_LEFT_DOWN 16
#define VERT_RIGHT_DOWN 32
#define HORIZ_DOWN 64
#define ALL_SEGS 127
#define DECIMAL 128

/* digits */
#define D_ZERO (ALL_SEGS & ~HORIZ_MID)
#define D_ONE (VERT_RIGHT_UP | VERT_RIGHT_DOWN)
#define D_TWO (ALL_SEGS & ~(VERT_LEFT_UP | VERT_RIGHT_DOWN))
#define D_THREE (ALL_SEGS & ~(VERT_LEFT_UP | VERT_LEFT_DOWN))
#define D_FOUR (ALL_SEGS & ~(HORIZ_UP | VERT_LEFT_DOWN | HORIZ_DOWN))
#define D_FIVE (ALL_SEGS & ~(VERT_RIGHT_UP | VERT_LEFT_DOWN))
#define D_SIX (ALL_SEGS & ~VERT_RIGHT_UP)
#define D_SEVEN (HORIZ_UP | VERT_RIGHT_UP | VERT_RIGHT_DOWN)
#define D_ALTSEVEN (VERT_LEFT_UP | D_SEVEN)
#define D_EIGHT ALL_SEGS
#define D_NINE (ALL_SEGS & ~VERT_LEFT_DOWN)
#define D_DECIMAL DECIMAL
#define D_HEX_A (ALL_SEGS & ~HORIZ_DOWN)
#define D_HEX_b (ALL_SEGS & ~(HORIZ_UP | VERT_RIGHT_UP))
#define D_HEX_C (ALL_SEGS & ~(VERT_RIGHT_UP | HORIZ_MID | VERT_RIGHT_DOWN))
#define D_HEX_c (HORIZ_MID | VERT_LEFT_DOWN | HORIZ_DOWN)
#define D_HEX_d (ALL_SEGS & ~(HORIZ_UP | VERT_LEFT_UP))
#define D_HEX_E (ALL_SEGS & ~(VERT_RIGHT_UP | VERT_RIGHT_DOWN))
#define D_HEX_F (ALL_SEGS & ~(VERT_RIGHT_UP | VERT_RIGHT_DOWN | HORIZ_DOWN))
#define D_UNKNOWN 0

/* colors used by ssocr */
#define	BLACK 0
#define WHITE 255

/* compute luminance Y_709 from linear RGB values */
int get_lum_709(Imlib_Color *color)
{
	return 0.2125*color->red + 0.7154*color->green + 0.0721*color->blue;
}

/* compute luminance Y_601 from gamma corrected (non-linear) RGB values */
int get_lum_601(Imlib_Color *color)
{
	return 0.299*color->red + 0.587*color->green + 0.114*color->blue;
}

/* compute luminance Y = (R+G+B)/3 */
int get_lum_lin(Imlib_Color *color)
{
	return (color->red + color->green + color->blue) / 3;
}

/* compute luminance Y = min(R,G,B) as used in GNU Ocrad 0.14 */
int get_lum_min(Imlib_Color *color)
{
	return (color->red < color->green) ?
		((color->red < color->blue) ? color->red : color->blue) :
		((color->green < color->blue) ? color->green : color->blue);
}

/* compute luminance Y = max(R,G,B) */
int get_lum_max(Imlib_Color *color)
{
	return (color->red > color->green) ?
		((color->red > color->blue) ? color->red : color->blue) :
		((color->green > color->blue) ? color->green : color->blue);
}

/* compute luminance from RGB values */
int get_lum(Imlib_Color *color)
{
	return get_lum_709(color);
	/*
	   switch(lt) {
	   case REC709:  return get_lum_709(color);
	   case REC601:  return get_lum_601(color);
	   case LINEAR:  return get_lum_lin(color);
	   case MINIMUM: return get_lum_min(color);
	   case MAXIMUM: return get_lum_max(color);
	   case RED:     return get_lum_red(color);
	   case GREEN:   return get_lum_green(color);
	   case BLUE:    return get_lum_blue(color);
	   default:
	   fprintf(stderr, "error: get_lum(): unknown transfer function no. %d\n",
	   lt);
	   exit(99);
	   }*/
}


/* draw a fore- or background pixel */
void draw_pixel(Imlib_Image *image, int x, int y, int color)
{
	Imlib_Image *current_image; /* save current image */

	current_image = imlib_context_get_image();
	imlib_context_set_image(image);
	imlib_context_set_color(color, color, color, 255);
	imlib_image_draw_pixel(x,y,0);
	imlib_context_set_image(current_image);
}

/* check if a pixel is set regarding current foreground/background colors */
int is_pixel_set(int value, double threshold)
{
	if(value < threshold/100.0*255)
		return 1;
	return 0;
}


/* set pixels with (brightness) value lower than threshold that have more than
 * mask pixels around it set (including the examined pixel itself) to black
 * (foreground), set pixels with (brightness) value lower than threshold that
 * less or equal pixels around it set to white (background) */
Imlib_Image keep_pixels_filter(Imlib_Image *source_image, double thresh, int mask)
{
	Imlib_Image new_image; /* construct filtered image here */
	int height, width; /* image dimensions */
	int x,y,i,j; /* iteration variables */
	int set_pixel; /* should  pixel be set or not? */
	Imlib_Color color;
	int lum; /* luminance value of pixel */

	/* create a new image */
	imlib_context_set_image(*source_image);
	height = imlib_image_get_height();
	width = imlib_image_get_width();
	new_image = imlib_clone_image();

	/* draw white (background) rectangle to clear new image */
	imlib_context_set_image(new_image);
	imlib_context_set_color(0, 0, 0, 255);
	imlib_image_draw_rectangle(0, 0, width, height);
	imlib_context_set_image(*source_image);

	/* check for every pixel if it should be set in filtered image */
	for(x=0; x<width; x++) {
		for(y=0; y<height; y++) {
			set_pixel = 0;
			imlib_image_query_pixel(x, y, &color);
			lum = get_lum(&color);
			if(is_pixel_set(lum, thresh)) { /* only test neighbors of set pixels */
				for(i=x-1; i<=x+1; i++) {
					for(j=y-1; j<=y+1; j++) {
						if(i>=0 && i<width && j>=0 && j<height) { /* i,j inside image? */
							imlib_image_query_pixel(i, j, &color);
							lum = get_lum(&color);
							if(is_pixel_set(lum, thresh)) {
								set_pixel++;
							}
						}
					}
				}
			}
			/* set pixel if at least mask pixels around it are set */
			/* mask = 1 keeps all pixels */
			if(set_pixel > mask) {
				draw_pixel(new_image, x, y, BLACK);
			} else {
				draw_pixel(new_image, x, y, WHITE);
			}
		}
	}

	/* return filtered image */
	return new_image;
}

float get_brightness(int x, int y, int size)
{
	Imlib_Color color;
	int crrnt_lum = 0;
	for(int x_i = 0; x_i < size; x_i++) {
		for(int y_i = 0; y_i < size; y_i++) {
			imlib_image_query_pixel(x_i + x, y_i + y, &color);
			crrnt_lum += get_lum(&color);
		}
	}
	return ((crrnt_lum*100/(size*size))/255);
}
//#define DEBUG 1
int segment_exists(int x, int y)
{
	float brt = get_brightness(x, y, 32);
	#ifdef DEBUG
	printf("brt is %f\n", brt);
	#endif
	if(brt <= 50)
		return 1;
	return 0;
}

int x_offset = 0;
int y_offset = 0;

void find_offset()
{
	for(int x = 0; x< 450; x++) {
		if(get_brightness(x, 100, 5) <= 10) {
			x_offset = x;
			for(int y = 100; y > 0; y--) {
				if(get_brightness(x_offset+20, y, 5) > 10) {
					y_offset = y;
					return;
				}
				return;
			}
		}
	}
}

int main(int argc, char **argv) {
	Imlib_Image image = NULL; /* an image handle */
	char *imgfile = NULL; /* filename of image file */
	imgfile = argv[argc-1];
	image = imlib_load_image(imgfile);
	int target_val[4] = {0,0,0,0};
	if(!image) {
		fprintf(stderr, "could not load image %s\n", imgfile);
		exit(99);
	}

	/* set the image we loaded as the current context image to work on */
	imlib_context_set_image(image);
	/* get image parameters */
	image = keep_pixels_filter(&image,  51.2, 7); //(image, threshold, mask)
	imlib_context_set_image(image);

	imlib_image_set_format("jpeg");
	imlib_save_image("debug.jpg");

	find_offset();
	if(x_offset > 160)
		x_offset -= 120;
	x_offset += 20;
	y_offset += 20;

	#ifdef DEBUG
	printf("x_offset is %i\n", x_offset);
	printf("y_offset is %i\n", y_offset);
	#endif

	int point = 99;
	for(int i = 0; i < 4; i++) {
	#ifdef DEBUG
		printf("\n");
	#endif
		if(segment_exists(x_offset + 62 + i*219, y_offset)) {
			target_val[i] |= HORIZ_UP;
		}
		if(segment_exists(x_offset + i*210, 80 + y_offset)) {
			target_val[i] |= VERT_LEFT_UP;
		}
		if(segment_exists(x_offset + 120 + i*210, 80 + y_offset)) {
			target_val[i] |= VERT_RIGHT_UP;
		}
		if(segment_exists(x_offset + 62 + i*219, 168 + y_offset)) {
			target_val[i] |= HORIZ_MID;
		}
		if(segment_exists(x_offset + i*210, 245 + y_offset)) {
			target_val[i] |= VERT_LEFT_DOWN;
		}
		if(segment_exists(x_offset + 120 + i*210, 245 + y_offset)) {
			target_val[i] |= VERT_RIGHT_DOWN;
		}
		if(segment_exists(x_offset + 62 + i*210, 327 + y_offset)) {
			target_val[i] |= HORIZ_DOWN;
		}
		if(segment_exists(x_offset + 162 + i*210, 327 + y_offset) && point == 99)
			point = i;
		switch(target_val[i]) {
			case D_ZERO:
				printf("0");
				break;
			case D_ONE:
				printf("1");
				break;
			case D_TWO:
				printf("2");
				break;
			case D_THREE:
				printf("3");
				break;
			case D_FOUR:
				printf("4");
				break;
			case D_FIVE:
				printf("5");
				break;
			case D_SIX:
				printf("6");
				break;
			case D_SEVEN:
				printf("7");
				break;
			case D_EIGHT:
				printf("8");
				break;
			case D_NINE:
				printf("9");
				break;
			default:
				printf("x");
		}
		if(point == i)
			printf(".");
	}
	printf("\n");
}
