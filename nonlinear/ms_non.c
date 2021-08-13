/*=======================
  M A N D E L B R O T
  =======================*/

// Implementation Based on Rosetta Code Example
// 1) Draws Mandelbrot set for Fc(z)=z*z +c using
//    Mandelbrot algorithm (boolean escape time).
// 2) Technique of creating ppm file is  based on
//    the code of Claudio Rocchini.  http://en.
//    wikipedia.org/wiki/Image:Color_complex_plot
//    .jpg.  Create 24 bit color graphic file,
//    portable pixmap file = PPM, see http://en.
//    wikipedia.org/wiki/Portable_pixmap to see
//    the file use external application (graphic
//    viewer).


// Inclusions
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Definitions
#define SHOW_TIMES 1

// Image Structure Definition
typedef struct {
	unsigned char color[3];
} image;


// Main
int main ( int argc, char *argv[ ] ) {

	const unsigned int     maxColorComponentValue =  255;   // Color Component - 24-bit RGB Coded from 0 to 255
	const unsigned int     iMax                   =  200;   // Maximum Number of Iterations
	unsigned int     i                      =  0;     // Iteration Number
	unsigned int     iX                     =  0;     // Screen (Integer) X Coordinate
	unsigned int     iY                     =  0;     // Screen (Integer) Y Coordinate
	unsigned int     iXMax                  =  2048;  // Generated Image Width
	unsigned int     iYMax                  =  2048;  // Generated Image Height
	unsigned int     cores                  =  0;     // Number of Cores Utilized in Parallel
	unsigned int     display                =  0;     // Argument to Display Debug Text
	unsigned int     size                   =  0;     // Size of Generated Image in Pixels
	unsigned int     thisPixelNum           =  0;     // Iterator for Tracking Pixel Number
	const          double  cXMin                  = -1.5;   // Generated View Window X Minimum
	const          double  cXMax                  =  0.5;   // Generated View Window X Maximum
	const          double  cYMin                  = -1.0;   // Generated View Window Y Minimum
	const          double  cYMax                  =  1.0;   // Generated View Window Y Maximum
	const          double  escapeRadius           =  2.0;   // Bail-Out Value - Radius of Circle
	double  cX                     =  0.0;   // World (Double) X Coordinate
	double  cY                     =  0.0;   // World (Double) Y Coordinate
	double  escapeRadius2          =  4.0;   // Square of EscapeRadius
	double  pixelWidth             =  0.0;   // Determination of Relative Pixel Width from Window and Size Parameters
	double  pixelHeight            =  0.0;   // Determination of Relative Pixel Height from Window and Size Parameters
	double  zX                     =  0.0;   // Z = Zx + Zy * i; Z0 = 0
	double  zY                     =  0.0;   // (see just above)
	double  zX2                    =  0.0;   // Square of Zx
	double  zY2                    =  0.0;   // Square of Zy
	char   *filename               =  NULL;  // Filename to Save Mandelbrot Image
	char   *comment                =  "# ";  // Dynamic File Header Comment
	image  *fractal;                         // Image Being Built
	FILE   *fp                     =  NULL;  // A File

	double  jobStart               =  0.0;   // Timer for Program Start
	double  mandelbrotStart        =  0.0;   // Timer for Mandelbrot Fractal Generation Start
	double  mandelbrotEnd          =  0.0;   // Timer for Mandelbrot Fractal Generation End
	double  writeStart             =  0.0;   // Timer for Image Write Out Start
	double  writeEnd               =  0.0;   // Timer for Image Write Out End
	double  jobEnd                 =  0.0;   // Timer for Program Completion

	double  mandelbrotTime         =  0.0;   // Program Time in Color Search Operation Phase
	double  writeTime              =  0.0;   // Program Time in Image Write Phase
	double  addTime                =  0.0;   // Additional Overhead Time
	double  totalTime              =  0.0;   // Total Program Time

	// Set Job Start Timer
	jobStart = omp_get_wtime( );

	// Display Usage Instructions If Argument Count Incorrect
	if( argc != 5 ) {
		printf( "\nUsage: %s <output> <x/y> <cores> <display>\n", argv[0] );
		printf( "  Output  - a .ppm image to output with the fractal.\n" );
		printf( "  X/Y     - width and height of image in pixels.\n" );
		printf( "  Cores   - number of cores to utilize for parallel operation.\n" );
		printf( "  Display - 1 displays debug text, 0 just displays time values for raw data tables.\n\n" );
		return 1;
	}

	// Parse Input Arguments
	filename      = argv[1];
	iXMax         = atoi( argv[2] );
	iYMax         = iXMax;
	cores         = atoi( argv[3] );
	display       = atoi( argv[4] );

	// Intro Text
	if( display ) {
		printf( "\n   = = =  Mandelbrot Set Generator  = = =   \n\n" );
	}

	// Early Calculations
	size          = iXMax * iYMax;                // Determination of Size from
	pixelWidth    = ( cXMax - cXMin ) / iXMax;    // Determination of Relative Pixel Width from Window and Size Parameters
	pixelHeight   = ( cYMax - cYMin ) / iYMax;    // Determination of Relative Pixel Height from Window and Size Parameters
	escapeRadius2 = escapeRadius * escapeRadius;  // Calculate Square of EscapeRadius.

	// OpenMP Initial Tasks and Tests
	omp_set_num_threads( cores );
	if( display ) {
		printf( "Using %d Cores of Maximum %d Cores Available\nTesting - Report\n", cores, omp_get_max_threads( ) );
		#pragma omp parallel for
		for( size_t i = 0; i < (size_t)omp_get_num_threads( ); i++ ) {
			printf( "  Core %d of %d Reporting!\n", omp_get_thread_num( ), omp_get_num_threads( ) );
		}
	}

	// Allocate Storage for Image Colors
	fractal = malloc( size * sizeof( *fractal ) );

	// Compute Fractal Image
	if( display ) {
		printf( "\nGenerating Mandelbrot Set...\n" );
	}
	mandelbrotStart = omp_get_wtime( );
	#pragma omp parallel for shared( fractal, iXMax, iYMax, pixelHeight, pixelWidth, escapeRadius2 ) \
		private( i, iX, iY, cX, cY, zX, zY, zX2, zY2, thisPixelNum ) schedule(guided,100) 
		//default( none ) \
		schedule( dynamic )
		for( iY = 0; iY < iYMax; iY++ ) 
		{
			cY      = cYMin + iY * pixelHeight;
			if( fabs( cY ) < ( pixelHeight / 2 ) ) 
			{
				cY    = 0.0;  // Main Antenna
			}
			for( iX = 0; iX < iXMax; iX++ ) 
			{
				cX    = cXMin + iX * pixelWidth;

				// Initial Value of Orbit - Critical Point Z = 0
				zX    = 0.0;
				zY    = 0.0;
				zX2   = zX * zX;
				zY2   = zY * zY;

				for( i = 0; ( i < iMax ) && ( ( zX2 + zY2 ) < escapeRadius2 ); i++ ) 
				{
					zY  = 2 * zX * zY + cY;
					zX  = zX2 - zY2 + cX;
					zX2 = zX * zX;
					zY2 = zY * zY;
				};

				// Save Pixel Color
				thisPixelNum  = iY * iYMax + iX;  // Where is this pixel in the image?
				if( i == iMax ) 
				{                                    			   // Color for Interior of Mandelbrot Set - Dark Gray
					fractal[thisPixelNum].color[0] = 37;               //   Red Component
					fractal[thisPixelNum].color[1] = 37;               //   Green Component
					fractal[thisPixelNum].color[2] = 37;               //   Blue Component
				} else {                                                   // Color for Exterior of Mandelbrot Set - Blue
					fractal[thisPixelNum].color[0] = 0;                //   Red Component
					fractal[thisPixelNum].color[1] = 0;                //   Green Component
					fractal[thisPixelNum].color[2] = 255;              //   Blue Component
				}  // End If
			}    // End iX For
		}      // End iY For
	mandelbrotEnd = omp_get_wtime( );

	// Image File Write Phase
	if( display ) {
		printf( "Writing File Out...\n" );
	}
	writeStart    = omp_get_wtime( );
	// Create New File - give it a name and open it in binary mode.
	fp            = fopen( filename, "wb" );  // b - Binary Mode
	// Write ASCII Header to the File
	fprintf( fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXMax, iYMax, maxColorComponentValue );
	// Image File Write Out - must be done serially.
	for( iY = 0; iY < iYMax; iY++ ) {
		for( iX = 0; iX < iXMax; iX++ ) {
			thisPixelNum = iY * iYMax + iX;
			// Write Color to the File
			fwrite( fractal[thisPixelNum].color, 1, 3, fp );
		}
	}
	fclose( fp );
	writeEnd = omp_get_wtime( );

	// Final Tasks
	free( fractal );
	if( display ) {
		printf( "Operation Complete!\n\n" );
	}

	// Timing Calculations
	jobEnd         = omp_get_wtime( );
	mandelbrotTime = mandelbrotEnd - mandelbrotStart;
	writeTime      = writeEnd      - writeStart;
	totalTime      = jobEnd        - jobStart;
	addTime        = totalTime     - mandelbrotTime - writeTime;

	// Timing Display
	if( SHOW_TIMES ) {
		if( display ) {
			printf( "  === Timing Data ===\n  Mandelbrot:\t\t" );
		}
		printf( "%0.9lf ", mandelbrotTime );
		if( display ) {
			printf( "\n  Image Write:\t\t" );
			printf( "%0.9lf\n", writeTime );
			printf( "  Ending Overhead:\t" );
			printf( "%0.9lf\n", addTime );
			printf( "  Total Job Time:\t" );
			printf( "%0.9lf\n\n", totalTime );
		}
	}
	return 0;

}

