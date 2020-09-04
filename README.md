# SpeedyTiltShift2018

Analyze the performance implications of mobile image processing implementations.
Fancy image transformations -- colloquially, “filters” -- have dramatically captured the attention of the mobile market, to the point that operating systems and devices take special measures to promote efficient image processing. This project assignment is intended for you to learn about system bottlenecks to image processing and tricks to optimize performance.
For this project assignment, you will be building “Tilt-Shift” functionality, simulating macro photography for full-size scenes. You will design and benchmark your implementation using Java, C++ and ARM NEON intrinsics.
Things you will need to self-teach
	Gaussian blur: https://en.wikipedia.org/wiki/Gaussian_blur 
	How to use Android native functions
	How to work with the ARGB8888 representation of an image
Github
Use Github to maintain a private repository of your code. Github education pack allows unlimited private repositories. Name your git repository “eee598-tiltshift-[lastname1]-[lastname2]-[lastname3]”, e.g., “eee598-tiltshift-likamwa-hujinhan12”. Add “roblkw-asu” and “hujinhan12” as collaborators.
You should commit to the git at least once a week.
Application hooks
For convenience and standardization, this assignment will provide an application template that includes application infrastructure to process an image file through transformations. 
You will be provided with three dummy functions: 
	Bitmap tiltshift_java(Bitmap input, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3)
	Bitmap tiltshift_cpp(Bitmap input, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3)
	Bitmap tiltshift_neon(Bitmap input, float sigma_far, float sigma_near, int a0, int a1, int a2, int a3)
It is your job to populate the three functions.
All three functions take in two sigma float values: sigma_near (σ1) and sigma_far (σ2), representing the blur of near and far objects, and four integers, a0, a1, a2, a3 representing the location and gradient of transition between blurry and non-blurry regions. The functions take in an input bitmap to create and return an output bitmap.
Tilt-shift algorithm
The tilt-shift effect simulates the focal blur effects created when photographing miniature scenes. A simple way to create the tilt-shift effect is to apply a Gaussian blur kernel of different sigma value (blurriness) depending on the assumed physical depth of the subject. For this assignment, we will use the y-coordinate to assume the depth of the image contents.
The tilt shift filter should take in 4 integer arguments representing the image rows for the progression of blur. The filter should also take in two float arguments, σ1 and σ2 representing the blur of the “near” and “far” regions respectively. See Figure 1 for specification on how to assign blur operations.
 
Figure 1: Tilt shift blur algorithm Gaussian blur kernels
Gaussian blur algorithm	
In this section, we provide the details of the Gaussian blur algorithm. The algorithm takes an image region and kernel matrices as inputs and outputs a blurred image region. Each input pixel has an R,G, and B color component value. Each output pixel is a Gaussian-weighted combination of nearby input pixel values. Gaussian blur should be performed on each color channel (R, G, B) independently. There are two approaches to computing Gaussian blur: (i) a weight matrix approach; and (ii) a weight vector approach. You may choose to implement either.
Parameter	Symbol
(y, x)	Pixel indices
Standard deviation	σ
Radius of the kernel	r
Input image region	P
Gaussian weight kernel matrix	G
Gaussian weight vector	G
Output image region	P

NOTE: Input pixels with out-of-bound indices should be treated as black pixels. e.g., p(-1, -1) = 0.
Gaussian blur through a weight matrix approach [slow method]	
The size of kernel matrix, G, is (2r+1) by (2r+1), where r ≥ ceil (2 σ)	
Then, each entry G(y,x), where x∈ [-r,r] and y∈ [-r,r] can be generated as follows:
G(y,x) = 1/(2πσ^2 ) exp(-x^2+ y22σ2)
Important note: If σ<0.6, you should not apply the Gaussian blur operation at all.
Then, each pixel in the output image P is obtained by applying G as a weighted sum on a patch of pixels of the input image p. Mathematically, for each (y, x) in P:
P(y,x) = 	G(-r, -r)*p(y-r, x-r), + … + G(0, -r)*p(y, x-r),+ … + G(r, -r)*p(y+r, x-r) +	
G(-r, 0)*p(y-r, x),     + … + G(0, 0)*p(y, x),   + … + G(r, 0)*p(y+r, x) +	
G(-r, r)*p(y-r, x+r), + … + G(0, r)*p(y, x+r),+ … + G(r, r)*p(y+r, x+r)
Gaussian blur through a weight vector approach [fast method]	
It is often faster to perform the Gaussian blur as two independent transforms across each x-y dimension. To do this, generate Gaussian kernel vector G, where k∈ [-r,r] as follows: 
G(k) = 1/√(2πσ^2 ) exp(-k^2/(2σ^2 ))
Then, a first transformation generates the intermediate matrix, q(y, x):
q(y, x) = G(-r)*p(y-r, x), + … + G(0)*p(y, x),+ … + G(r)*p(y+r, x)
In a second transformation, this intermediate matrix is used to generate the final output:
P(y, x) = G(-r)*q(y, x-r), + … + G(0)*q(y, x),+ … + G(r)*q(y, x+r)
Benchmarks
Provide comparisons against your 3 tilt-shift implementations through the use of benchmark operations. The results of these benchmarks should be charted and clearly discussed in your documentation.
For each implementation, you should:
	Use timestamps to measure elapsed time of operations.
	Vary image sizes to see relationships between elapsed time.
	Set the tablet into performance mode, optimized mode.
