# Disparity Map Inpainting

<img src="results/sharp_linear_avg_median=5.png" width=100% />

This project aims to **develop** and implement **efficient** algorithms for single **disparity** map **inpainting** in plain C.

1. [Details](#details)
2. [Timeline](#timeline)
3. [Runtime](#runtime)
4. [Attributions] (#attributions)


## Details

#### Input
- linearly adressed 8bit grayscale image (unsigned char*)
- it is assumed, that the pixels to be inpainted have value 0

#### Algorithm
- all holes should be filled
- i.e. all masked pixels should be replaced by meaningful values
- e.g. according to their closest boundaries' values

#### Output
- no output (in-place processing)

## Timeline

1. I first tried **linear interpolation** in both horizontal and vertical scanlines as suggested by Daniel Gurdan.
Just the **horizontal** interpolation showed a major problem:
The **edges** of the input are **noisy**.
More concretely they show a 1-pixel anti-aliased border.
This falsifies the start and end point values of hole-line-segments, which in turn leads incorrectly interpolated to stripes all over the place.
<br><br>
![](results/linear_offset=0.png)
<br><br>
I also tried using different **offsets** beyond the initial start/end points and also different **thresholds** which decide when a pixel is overwritten and when not. 
No big breakthrough there.
<br><br>
2. I thought about a different approach and came up with **layered flood filling**.
The assumption was, that measurement errors occur at consistent depth planes.
I.e. a hole containing a foreground object (bright) probably could be this very object itself.
Also note, that obviously foreground overlays background (see implementation).
This resulted in the following.
<br><br>
![](results/layered_floodfill.png)
<br><br>
The problem was, that this approach yields - as had to be expected - **large constant areas**, in contrast to the smooth interpolated regions we want.
So, next try.
<br><br>
3. I took a closer look at the input image and only then discovered the constant width of 1-pixel for the anti-aliasing effect.
I thought, that probably the **vertical** and horizontal lines could complement each other - which of course is not the case but wasn't clear to me at the time. 
So, vertical interpolation implemented. 
One gets the final image by merging the horizontal and vertical result. 
I tried two versions, **averaging** (top) and **maxing** (bottom), the latter taking the brighter color for each pixel.
<br><br>
![](results/linearvh_avg_offset=1.png)
![](results/linearvh_max_offset=1.png)
<br><br>
Better, but still same problem.
<br><br>
4. Ok, so to the **hell** with those edges. 
Let's do something about them. <br>
I implemented an edge **sharpening** method specifically for my scenario. 
It looks for pixel adjacent to holes and assigns them the brightes value in their neighbourhood, effectively eliminating the anti-aliasing. 
Nice. 
While I first implemented only 4-way-neighbourhoods, **8-way** is necessary for the wanted results.
<br><br>
5. Back to the **linear interpolation**, but this time on the **preprocessed** input image.
<br><br>
![](results/sharp_linear_avg.png)
<br><br>
Nice, but could be better. 
Looks like a pretty good case for
<br><br>
6. **Median filters**. 
For now, I use the opencv implementation.
I thought this could be the most resource-consuming part, however, I found a paper introcuding an **O(1)** (in kernel size) median filter, which looks promissing. 
Will implement it in C when I have some time.
Here the currently final result:
<br><br>
![](results/sharp_linear_avg_median=5.png)


## Runtime
- All tests were performed on a _320x240_ greyscale image. <br>
- All O-Notations are given w.r.t. operations per pixel. <br>
- All measurements were taken as average on 10000 runs.

Method | Asymptotic Runtime
---|---
sharpenEdges | **O(1)**
fillLinear | **O(1)**
medianBlur | **O(1)**

Method(s) | Runtime
---|---
sharpenEdges -> fillLinear -> median | **4.168ms** (OpenCV)
sharpenEdges -> fillLinear -> median | **12.56ms** (ctmf)

Note that OpenCV makes use of hardware optimisations and/or OpenCL. <br>
While we do have a constant time inpainting algorithm, the involved constants must and can still be optimised.


## Attributions

Thanks to my bro for giving me something to think about ;).

<!-- All files in _paper/_ are there for convenience only. <br>
I take no credit whatsoever. <br>

- S. Perreault, P. Hébert - Median Filtering in Constant Time.pdf <br>
Published in the September 2007 issue of IEEE Transactions on Image Processing. <br>
https://nomis80.org/ctmf.html <br> -->
