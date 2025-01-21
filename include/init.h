
#ifndef INIT_H
#define INIT_H



#include <list>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <iomanip>

#include <limits.h>
#include "Configuration.h"

//#include <highgui.h>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/video/background_segm.hpp>
#include <opencv2/video/tracking.hpp>
#include <limits>
#include <cstddef>
#include "tools.h"
#include "Color.h"
#include "Frequency.h"
#include "Stats.h"
#include "Similarity.h"
#include "Clustering.h"
#include "Segmentation.h"
#include "Morpho.h"
#include "Shape.h"
//#include "Texture.h"
#include "Filtering.h"
//#include "BGS.h"
#include "Motion.h"
//#include "Similarity.h"
//#include "stereo.h"

#define PI   (3.1415926535897932384626433832795)
#define testMax(x,y)  ( (x) > (y) ? (x) : (y) )
#define testMin(x,y)  ( (x) < (y) ? (x) : (y) )
#define DE2(x1, y1, x2, y2) sqrt((x1-x2)*(x1-x2)+ (y1-y2)*(y1-y2))

// --------------------------------
// CONSTANTS for display
// --------------------------------
#define WIDTH_WINDOW 100
#define HEIGHT_WINDOW 100

#endif
