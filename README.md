# Image-Filters

<img width="627" height="419" alt="Screenshot 2025-10-15 180020" src="https://github.com/user-attachments/assets/9f5cb87f-73aa-47a6-83c5-6e572bb0543f" />

## Problem to Solve

Perhaps the simplest way to represent an image is with a grid of pixels (i.e., dots), each of which can be of a different color. For black-and-white images, we thus need 1 bit per pixel, as 0 could represent black and 1 could represent white, as in the below.

<img width="1021" height="369" alt="Screenshot 2025-10-15 180625" src="https://github.com/user-attachments/assets/5954c7e7-7675-479f-bf3d-3071ad1f80dc" />

In this sense, then, is an image just a bitmap (i.e., a map of bits). For more colorful images, you simply need more bits per pixel. A file format (like BMP, JPEG, or PNG) that supports “24-bit color” uses 24 bits per pixel. (BMP actually supports 1-, 4-, 8-, 16-, 24-, and 32-bit color.)

A 24-bit BMP uses 8 bits to signify the amount of red in a pixel’s color, 8 bits to signify the amount of green in a pixel’s color, and 8 bits to signify the amount of blue in a pixel’s color. If you’ve ever heard of RGB color, well, there you have it: red, green, blue.

If the R, G, and B values of some pixel in a BMP are, say, 0xff, 0x00, and 0x00 in hexadecimal, that pixel is purely red, as 0xff (otherwise known as 255 in decimal) implies “a lot of red,” while 0x00 and 0x00 imply “no green” and “no blue,” respectively. In this problem, you’ll manipulate these R, G, and B values of individual pixels, ultimately creating your very own image filters.

In a file called helpers.c in a folder called filter-less, write a program to apply filters to BMPs.

## A Bit(map) More Technical

Recall that a file is just a sequence of bits, arranged in some fashion. A 24-bit BMP file, then, is essentially just a sequence of bits, (almost) every 24 of which happen to represent some pixel’s color. But a BMP file also contains some “metadata,” information like an image’s height and width. That metadata is stored at the beginning of the file in the form of two data structures generally referred to as “headers,” not to be confused with C’s header files. (Incidentally, these headers have evolved over time. This problem uses the latest version of Microsoft’s BMP format, 4.0, which debuted with Windows 95.)

The first of these headers, called BITMAPFILEHEADER, is 14 bytes long. (Recall that 1 byte equals 8 bits.) The second of these headers, called BITMAPINFOHEADER, is 40 bytes long. Immediately following these headers is the actual bitmap: an array of bytes, triples of which represent a pixel’s color. However, BMP stores these triples backwards (i.e., as BGR), with 8 bits for blue, followed by 8 bits for green, followed by 8 bits for red. (Some BMPs also store the entire bitmap backwards, with an image’s top row at the end of the BMP file. But we’ve stored this problem set’s BMPs as described herein, with each bitmap’s top row first and bottom row last.) In other words, were we to convert the 1-bit smiley above to a 24-bit smiley, substituting red for black, a 24-bit BMP would store this bitmap as follows, where 0000ff signifies red and ffffff signifies white; we’ve highlighted in red all instances of 0000ff.

<img width="775" height="406" alt="Screenshot 2025-10-15 180902" src="https://github.com/user-attachments/assets/05dadaf9-ec3f-4cb7-955c-02c0c280f15c" />

## Image Filtering

What does it even mean to filter an image? You can think of filtering an image as taking the pixels of some original image, and modifying each pixel in such a way that a particular effect is apparent in the resulting image.

## Understanding Files

### bmp.h

Open up bmp.h (as by double-clicking on it in the file browser) and have a look.

You’ll see definitions of the headers we’ve mentioned (BITMAPINFOHEADER and BITMAPFILEHEADER). In addition, that file defines BYTE, DWORD, LONG, and WORD, data types normally found in the world of Windows programming. Notice how they’re just aliases for primitives with which you are (hopefully) already familiar. It appears that BITMAPFILEHEADER and BITMAPINFOHEADER make use of these types.

Perhaps most importantly for you, this file also defines a struct called RGBTRIPLE that, quite simply, “encapsulates” three bytes: one blue, one green, and one red (the order, recall, in which we expect to find RGB triples actually on disk).

Why are these structs useful? Well, recall that a file is just a sequence of bytes (or, ultimately, bits) on disk. But those bytes are generally ordered in such a way that the first few represent something, the next few represent something else, and so on. “File formats” exist because the world has standardized what bytes mean what. Now, we could just read a file from disk into RAM as one big array of bytes. And we could just remember that the byte at array[i] represents one thing, while the byte at array[j] represents another. But why not give some of those bytes names so that we can retrieve them from memory more easily? That’s precisely what the structs in bmp.h allow us to do. Rather than think of some file as one long sequence of bytes, we can instead think of it as a sequence of structs.


### filter.c

Now, let’s open up filter.c. This file has been written already for you, but there are a couple important points worth noting here.

First, notice the definition of filters on line 10. That string tells the program what the allowable command-line arguments to the program are: b, g, r, and s. Each of them specifies a different filter that we might apply to our images: blur, grayscale, reflection, and sepia.

Then, lines 11 to 32 run a check for flags and validates them while storing them up in an array for sequentially applying them.

The next several lines open up an image file, make sure it’s indeed a BMP file, and read all of the pixel information into a 2D array called image.

Scroll down to the for loop on switch statement that begins on line 101. Notice that, depending on what filter flags you've passed in sequence , a different function is called: if the user chooses filter b, the program calls the blur function; if g, then grayscale is called; if r, then reflect is called; and if s, then sepia is called. Notice, too, that each of these functions take as arguments the height of the image, the width of the image, and the 2D array of pixels. After each execution of filter function, loop moves onto next filter in array to apply, if present.

These are the functions you’ll (soon!) implement. As you might imagine, the goal is for each of these functions to edit the 2D array of pixels in such a way that the desired filter is applied to the image.

The remaining lines of the program take the resulting image and write them out to a new image file.

### Helpers.h

Next, take a look at helpers.h. This file is quite short, and just provides the function prototypes for the functions you saw earlier.

Here, take note of the fact that each function takes a 2D array called image as an argument, where image is an array of height many rows, and each row is itself another array of width many RGBTRIPLEs. So if image represents the whole picture, then image[0] represents the first row, and image[0][0] represents the pixel in the upper-left corner of the image


### Helpers.c

Now, open up helpers.c. Here’s where the implementation of the functions declared in helpers.h belong. But note that, right now, the implementations are missing! This part is up to you.


# Algorithms:

Now.  let us scal different algorithms to form different filters.

### 1.) The grayscale algorithm

The “grayscale” filter is a mainstay of the image filtering world: it takes (potentially quite colorful) images and converts them to “black-and-white”, as though the image were taken before the popularization of color photography. How does something like that work?

Recall that if the red, green, and blue values are all set to 0x00 (hexadecimal for 0), then the pixel is black. And if all values are set to 0xff (hexadecimal for 255), then the pixel is white. So long as the red, green, and blue values are all equal, the result will be varying shades of gray along the black-white spectrum, with higher values meaning lighter shades (closer to white) and lower values meaning darker shades (closer to black).
So to convert a pixel to grayscale, you just need to make sure the red, green, and blue values are all the same value. But how do you know what value to make them? Well, it’s probably reasonable to expect that if the original red, green, and blue values were all pretty high, then the new value should also be pretty high. And if the original values were all low, then the new value should also be low.
In fact, to ensure each pixel of the new image still has the same general brightness or darkness as the old image, you can take the average of the red, green, and blue values to determine what shade of grey to make the new pixel.
If you apply the above algorithm to each pixel in the image, the result will be an image converted to grayscale.

### 2.) The Reflect Algorithm

The reflect function should take an image and reflect it horizontally.
Some more adventurous filters might go further than merely editing the pixels in place; they might move the pixels around the image. Reflecting an image, for example, is a filter where the resulting image is what you would get by placing the original image in front of a mirror.

Any pixels on the left side of the image should end up on the right, and vice versa.
Note that all of the original pixels of the original image will still be present in the reflected image, it’s just that those pixels may have been rearranged to be in a different place in the image.


### 3.) The Sepia Algorithm

 Most image editing programs support a “sepia” filter, which gives images an old-timey feel by making the whole image look a bit reddish-brown.

An image can be converted to sepia by taking each pixel, and computing new red, green, and blue values based on the original values of the three.
There are a number of algorithms for converting an image to sepia, but for this problem, we’ll ask you to use the following algorithm. For each pixel, the sepia color values should be calculated based on the original color values per the below.
sepiaRed = .393 * originalRed + .769 * originalGreen + .189 * originalBlue
sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue
sepiaBlue = .272 * originalRed + .534 * originalGreen + .131 * originalBlue
Of course, the result of each of these formulas may not be an integer, but each value could be rounded to the nearest integer. It’s also possible that the result of the formula is a number greater than 255, the maximum value for an 8-bit color value. In that case, the red, green, and blue values should be capped at 255. As a result, we can guarantee that the resulting red, green, and blue values will be whole numbers between 0 and 255, inclusive.
If you apply the above algorithm to each pixel in the image, the result will be an image converted to sepia.


### 4.) The Blur Algorithm

There are a number of ways to create the effect of blurring or softening an image, giving the resulting picture a hazy, dreamlike feeling (or perhaps just the feeling of having lost your glasses). For this problem, we’ll use the “box blur,” which works by taking each pixel and, for each color value, giving it a new value by averaging the color values of neighboring pixels.
The new value of each pixel would be the average of the values of all of the pixels that are within 1 row and column of the original pixel (forming a 3x3 box). For example, each of the color values for pixel 6 would be obtained by averaging the original color values of pixels 1, 2, 3, 5, 6, 7, 9, 10, and 11 (note that pixel 6 itself is included in the average). Likewise, the color values for pixel 11 would be be obtained by averaging the color values of pixels 6, 7, 8, 10, 11, 12, 14, 15 and 16.
For a pixel along the edge or corner, like pixel 15, we would still look for all pixels within 1 row and column, but only those that are actually within the image: in this case, we’d take the average of pixels 10, 11, 12, 14, 15, and 16.


If you apply the above algorithm to each pixel in the image, the result should look like a blurry, out-of-focus version of the original.

### 5.) The Vignette Algorithm

The vignette filter gives an image a cinematic look by gradually darkening its corners. 

**Algorithm:**
1. Compute the center of the image (cx, cy).
2. Calculate the maximum distance from the center to any corner (max_dis).
3. For each pixel at (i, j):
   - Calculate the Euclidean distance from this pixel to the center: dist = sqrt((j - cx)^2 + (i - cy)^2)
   - Compute a vignette factor: vig = 1.0 - (dist / max_dis)
   - Clamp vig between 0.0 (fully dark) and 1.0 (original color).
   - Multiply the pixel's R, G, B values by vig to darken farther-away pixels more thoroughly.

If you apply this algorithm, your resulting image will have gently darkened corners and an undisturbed/sunnier center.

---

### Usage

To apply a filter via command-line:
- `g`: grayscale
- `s`: sepia
- `r`: reflect
- `b`: blur
- `i`: invert
- `v`: vignette

For vignette:
```
./filter v input.bmp output.bmp
```
You can also chain multiple filters by supplying multiple tags (e.g., `./filter vg input.bmp output.bmp` for vignette then grayscale).

You should not modify any of the function signatures, nor should you modify any other files other than helpers.c.

Consider the following grid of pixels, where we’ve numbered each pixel.

<img width="528" height="492" alt="Screenshot 2025-10-16 041628" src="https://github.com/user-attachments/assets/5695cd6f-c535-42fc-8f8f-87bfbb362364" />

## Team and Credits

### Project Manager : Harsh Sankhla
### Built With: ❤️ by PyC

