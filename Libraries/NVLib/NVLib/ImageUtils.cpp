//--------------------------------------------------
// Implementation code for ImageUtils
//
// @author: Wild Boar
//--------------------------------------------------

#include "ImageUtils.h"
using namespace NVLib;

//--------------------------------------------------
// GetHistogram
//--------------------------------------------------

/**
 * Calculates a histogram for the given image
 * @param grayImage The gray image that we are getting the histogram for
 * @return Return a Mat
 */
Mat ImageUtils::GetHistogram(Mat& grayImage)
{
	Mat histogram = Mat_<int>::zeros(256, 1);
	auto pixelCount = grayImage.rows * grayImage.cols;
	auto histogramData = (int*)histogram.data;
	
	for (auto i = 0; i < pixelCount; i++)
	{
		histogramData[grayImage.data[i]]++;
	}

	return histogram;
}

//--------------------------------------------------
// GetCumulativeHistogram
//--------------------------------------------------

/**
 * Defines the functionality to calculate a cumulative histogram
 * @param histogram The histogram that we are converting from
 */
Mat ImageUtils::GetCumulativeHistogram(Mat& histogram) 
{
	Mat cumulativeHistogram = Mat_<int>::zeros(256, 1);
	auto histData = (int*)histogram.data;
	auto chistData = (int*)cumulativeHistogram.data;

	chistData[0] = histData[0];

	for (auto i = 1; i < 256; i++)
	{
		chistData[i] = histData[i] + chistData[i - 1];
	}
	
	return cumulativeHistogram;
}

//--------------------------------------------------
// GetUpperPercentileIntensity
//--------------------------------------------------

/**
 * Get the upper percentile intensity
 * @param chistogram The cumulative histogram that we are working from
 * @param percentile The percentile (note that is a percentile divided by 100, so 10% = 0.1)
 * @return The color intensity of the image at that percentile
 */
int ImageUtils::GetUpperPercentileIntensity(Mat& chistogram, double percentile) 
{
	if (percentile >= 1) return 255; if (percentile <= 0) return 0;

	auto chistData = (int*)chistogram.data;
	auto totalPixels = chistData[255];

	for (auto i = 254; i >= 0; i--) 
	{
		auto offset = totalPixels - chistData[i];
		auto ratio = (double)offset / (double)totalPixels;
		if (ratio >= percentile) return i;
	}

	return 0;
}

//--------------------------------------------------
// GetOrderedLabelCounts
//--------------------------------------------------

/**
 * Retrieve the counts of pixels for each label and order the results from biggest to smallest
 * @param labelMap The map of labels
 * @param labelCount The number of labels there are
 * @param result The result that we are adding
 */
void ImageUtils::GetOrderedLabelCounts(Mat& labelMap, int labelCount, vector<Pair>& result) 
{
	result.clear();

	for (auto i = 0; i < labelCount; i++) result.push_back(Pair(i, 0));
	auto labelData = (int*)labelMap.data; auto pixelCount = labelMap.rows * labelMap.cols;

	for (auto i = 0; i < pixelCount; i++) 
	{
		auto label = labelData[i];
		result[label].second++;
	}

	sort(result.begin(), result.end(), [](auto& left, auto& right) { return left.second > right.second; });
}

//--------------------------------------------------
// GetBinaryLabelMap
//--------------------------------------------------

/**
 * Retrieve a binary map with a foreground label as white and background as black
 * @param fullMap The full map that we are creating th binary image for
 * @param foregroundLabel The foreground label that we are using
 * @return A binary map
 */
Mat ImageUtils::GetBinaryLabelMap(Mat& fullMap, int foregroundLabel) 
{
	Mat result = Mat_<uchar>(fullMap.size());
	auto labelData = (char*)fullMap.data;
	auto pixelCount = fullMap.rows * fullMap.cols;

	for (auto i = 0; i < pixelCount; i++) 
	{
		auto label = labelData[i];
		result.data[i] = label == foregroundLabel ? 255 : 0;
	}

	return result;
}

//--------------------------------------------------
// GetBiggestContour
//--------------------------------------------------

/**
 * Defines the functionary to get the biggest contour
 * @param binaryImage The binary image that we are working with
 * @param contour The contour that we are working with
 */
void ImageUtils::GetBiggestContour(Mat& binaryImage, vector<Point>& contour) 
{
	contour.clear();

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat cleanBinary; morphologyEx(binaryImage, cleanBinary, MORPH_OPEN, kernel);

	Mat edgeMap; Canny(cleanBinary, edgeMap, 10, 20);

	vector<vector<Point> > contours;
	findContours(edgeMap, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	if (contours.size() == 0) return;

	int longestContour = 0; auto currentMaxLength = contours[0].size();
	for (auto i = 0; i < contours.size(); i++) 
	{
		auto length = contours[i].size();
		if (length > currentMaxLength) { longestContour = i; currentMaxLength = length; }	
	}

	for (auto& point : contours[longestContour]) contour.push_back(point);
}

//--------------------------------------------------
// Retrieve a set of contours from the binary image
//--------------------------------------------------

/**
 * @brief Retrieve a set of contours from the given binary image
 * @param binaryImage The binary image that we are finding contours in
 * @param contours The resultant set of contours
*/
void ImageUtils::GetContours(Mat& binaryImage, vector<vector<Point>>& contours) 
{
	contours.clear();
	Mat edgeMap; Canny(binaryImage, edgeMap, 10, 20);
	findContours(edgeMap, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
}

//--------------------------------------------------
// GetColorBinary
//--------------------------------------------------

/**
 * Retrieve a binary image based on the presents of a given color within the image
 * @param image The image that we are getting the binary for
 * @param color The color that we are marking as black
 * @return The resultant binary mask
 */
Mat ImageUtils::GetColorBinary(Mat& image, const Vec3i& color) 
{
	Mat mask = Mat_<uchar>::zeros(image.size());

	for (auto row = 0; row < image.rows; row++) 
	{
		for (auto column = 0; column < image.cols; column++) 
		{
			auto index = column + row * image.cols;
			auto c1 = image.data[index * 3 + 0];
			auto c2 = image.data[index * 3 + 1];
			auto c3 = image.data[index * 3 + 2];
		
			if (c1 != color[0] || c2 != color[1] || c3 != color[2]) mask.data[index] = 255;			
			else mask.data[index] = 0;
		}	
	}

	return mask;
}

//--------------------------------------------------
// GetMaskImage
//--------------------------------------------------

/**
 * Apply a mask to an image and get an overlay
 * @param image The image that we are applying to
 * @param mask The binary image that we are rendering
 * @param color The render color
 */
Mat ImageUtils::GetMaskImage(Mat& image, Mat& mask, const Vec3i& color) 
{
	Mat result = image.clone();

	for (auto row = 0; row < image.rows; row++) 
	{
		for (auto column = 0; column < image.cols; column++) 
		{
			auto index = column + row * image.cols;
			if (mask.data[index] == 0) continue;
			
			auto c1 = (double)image.data[index * 3 + 0];
			auto c2 = (double)image.data[index * 3 + 1];
			auto c3 = (double)image.data[index * 3 + 2];

			result.data[index * 3 + 0] = saturate_cast<uchar>((c1 + color[0]) / 2.0);
			result.data[index * 3 + 1] = saturate_cast<uchar>((c2 + color[1]) / 2.0);
			result.data[index * 3 + 2] = saturate_cast<uchar>((c3 + color[2]) / 2.0);
		}	
	}

	return result;
}

//--------------------------------------------------
// Calculate the gradient map (magnitude and direction)
//--------------------------------------------------

/**
 * @brief Retrieve the gradient in the x-direction 
 * @param image The image that we are getting the gradients for
 * @return The resultant image (a signed matrix of 'int')
 */
Mat ImageUtils::GetGradientX(Mat& image) 
{
	Mat result; 
	//Sobel(image, result, CV_32F, 1, 0, 1);
	Scharr(image, result, CV_32F, 1, 0);
	return result;
}

/**
 * @brief Retrieve the gradient in the y-direction
 * @param image The image that we are getting the gradients for
 * @return The resultant image (a signed matrix of 'float')
 */
Mat ImageUtils::GetGradientY(Mat& image) 
{
	Mat result; 
	//Sobel(image, result, CV_32F, 0, 1, 1);
	Scharr(image, result, CV_32F, 0, 1);
	return result;
}

/**
 * @brief Calculate the gradient map
 * @param image The image we want the gradient map of
 * @return Mat The gradient we are calculating for
 */
Mat ImageUtils::GetGradientMap(Mat& image) 
{
	Mat gx = GetGradientX(image);
	Mat gy = GetGradientY(image);

	Mat magnitude; Mat direction; 
	cartToPolar(gx, gy, magnitude, direction, true);

	if (magnitude.channels() == 3) return GetColorGradient(magnitude, direction);

	auto parts = vector<Mat> { magnitude, direction };
	Mat result; merge(parts, result);

	return result;
}

/**
 * @brief Retrieve gradients if the images are a color image
 * @param magnitude The list of "color" magnitudes
 * @param direction The list of "color" directions
 * @return Mat The reusltant matrix
 */
Mat ImageUtils::GetColorGradient(Mat& magnitude, Mat& direction) 
{
	assert(magnitude.rows == direction.rows && magnitude.cols == direction.cols);

	auto mdata = (float *) magnitude.data; auto ddata = (float *) direction.data;

	Mat result = Mat_<Vec2f>(magnitude.size());
	auto output = (float *) result.data;

	for (auto row = 0; row < result.rows; row++) 
	{
		for (auto column = 0; column < result.cols; column++) 
		{
			auto bestMagnitude = 0; auto bestDirection = 0;	
			auto baseIndex = column + row * result.cols;

			for (auto channel = 0; channel < 3; channel++) 
			{
				auto index = baseIndex * 3 + channel;
				auto currentM = mdata[index];
				auto currentD = ddata[index];
				if (currentM > bestMagnitude) { bestMagnitude = currentM; bestDirection = currentD; }
			}

			output[baseIndex * 2 + 0] = bestMagnitude;
			output[baseIndex * 2 + 1] = bestDirection;		
		}
	}

	return result;
}


//--------------------------------------------------
// GetNormalizedGrayImage
//--------------------------------------------------

/**
 * @brief Convert an image into a normalized gray image
 * @param image The image that we are converting
 * @return a two channel float image (channel 0 = magnitude, channel 1 = theta)
*/
Mat ImageUtils::GetNormalizedGrayImage(Mat& image) 
{
	double min, max; minMaxLoc(image, &min, &max);
	Mat converted; image.convertTo(converted, CV_8U, 255.0 / (max - min), -min);
	return converted;
}

//--------------------------------------------------
// InvertMask
//--------------------------------------------------

/**
 * @brief Add the functionality to invert a mask image
 * @param maskImage The mask image that we are inverting
 * @return Mat The resultant mask image
 */
Mat ImageUtils::InvertMask(Mat& maskImage) 
{
	if (maskImage.channels() != 1) throw runtime_error("A mask image can only have 1 channel");

	Mat result = Mat_<uchar>(maskImage.size());

	for (auto row = 0; row < maskImage.rows; row++) 
	{
		for (auto column = 0; column < maskImage.cols; column++) 
		{
			auto index = column + row * maskImage.cols;
			result.data[index] = maskImage.data[index] == 0 ? 255 : 0;
		}
	}

	return result;
}

//--------------------------------------------------
// GetInOutHistograms
//--------------------------------------------------

/**
 * @brief Calculate The histograms for the inner region and outer region given a mask
 * @param grayImage The grayscale image that we are getting the histogram for
 * @param mask The mask we are using the label inner (white) and outer (black)
 * @param innerHist The inner histogram output
 * @param outerHist The outer histogram output
 */
void ImageUtils::GetInOutHistograms(Mat& grayImage, Mat& mask, Mat& innerHist, Mat& outerHist) 
{
	// Verify that we have a gray image
	if (grayImage.channels() != 1) throw runtime_error("Input needs to be a grayscale image");

	// Initialize the histograms with zeros
	innerHist = Mat_<int>::zeros(256, 1); outerHist = Mat_<int>::zeros(256, 1);
	auto innerData = (int *)innerHist.data; auto outerData = (int *)outerHist.data;;

	// loop through the data and update the histograms
	for (auto row = 0; row < grayImage.rows; row++) 
	{
		for (auto column = 0; column < grayImage.cols; column++) 
		{
			auto index = column + row * grayImage.cols;
			auto maskValue = mask.data[index];
			auto color = grayImage.data[index];

			if (maskValue == 0) outerData[color]++;
			else innerData[color]++; // Note the assumption says ANY value that is not zero is foreground!!!
		}
	}
}

//--------------------------------------------------
// GetPixelCount
//--------------------------------------------------

/**
 * @brief Retrieve the pixel count associated with a given histogram
 * @param histogram The histogram that we are getting the pixel count for
 * @return int The given pixel count
 */
int ImageUtils::GetPixelCount(Mat& histogram) 
{
	// Verify that the histogram has the expected "shape"
	if (histogram.rows != 256 || histogram.cols != 1) throw runtime_error("It is expected that the incomming histogram is 1 column with 256 rows!");

	// Setup variables
	auto accumulator = 0; auto input = (int *)histogram.data;

	// Loop through and count the counts
	for (auto i = 0; i < 256; i++) accumulator += input[i];

	// Return the value
	return accumulator;
}

/**
 * @brief Retrieves the number of pixels in an image of a given color
 * @param image The image that we are counting pixels for
 * @param color The color that determines a "hit"
 * @return int The number of the count
 */
int ImageUtils::GetPixelCount(Mat& image, const Scalar& color) 
{
	// Setup values
	auto accumulator = 0; auto channels = image.channels();

	// Loop through the values and update the accumulator
	for (auto row = 0; row < image.rows; row++) 
	{
		for (auto column = 0; column < image.cols; column++) 
		{
			auto index = column + row * image.cols;

			bool match = true;
			for (auto channel = 0; channel < channels; channel++) 
			{
				auto imageColor = image.data[index * channels + channel];		
				auto expectedColor = color[channel];
				match &= expectedColor == imageColor;
			}

			if (match) accumulator++;	
		}
	}

	// Return the result
	return accumulator;
}

//--------------------------------------------------
// Hist2PD
//--------------------------------------------------

/**
 * @brief Convert a histogram into a probability distribution
 * @param histogram The histogram that we are converting
 * @param distribution The probability distribution output
 * @return int The number of pixels in the histogram
 */
int ImageUtils::Hist2PD(Mat& histogram, Mat& distribution) 
{
	// Get the pixel count
	auto pixelCount = GetPixelCount(histogram);

	// Setup the result
	distribution = Mat_<double>(256, 1);

	// Setup the input and output handles
	auto input = (int *) histogram.data;
	auto output = (double *) distribution.data;

	// Loop through the values and update the distribution
	for (auto i = 0; i < 256; i++) 
	{
		auto probability = (double)input[i] / (double)pixelCount;
		output[i] = probability;
	}

	// return the result;
	return pixelCount;
}

//--------------------------------------------------
// AutoCanny
//--------------------------------------------------

/**
 * @brief Perform canny edge detection without parameters
 * @param image The image that we are processing
 * @param sigma The sigma that we are using
 * @return Mat the resultant edge map
 */
Mat ImageUtils::AutoCanny(Mat& image, float sigma) 
{
   	// Convert to grey colour space
    Mat gray; cvtColor( image, gray, COLOR_BGR2GRAY );
    
	// Apply small amount of Gaussian blurring
	GaussianBlur(gray, gray, cv::Size( 3, 3), 0, 0);
    
	// Get the median value of the matrix
    double median = GetMedian(gray);
    
	// Generate the thresholds
    int lower = (int)std::max(0.0, (1.0f - sigma) * median);
    int upper = (int)std::min(255.0, (1.0f + sigma) * median);
    
	// Apply canny operator
	Mat result; cv::Canny(gray, result, lower, upper);
    
	// Return the result
	return result;
}

/**
 * @brief Given a "grayscale" matrix - find the median value
 * @param matrix The matrix that we are calculating
 * @return double The resultant median value
 */
double ImageUtils::GetMedian(Mat& matrix) 
{
 	Mat input = matrix.reshape(0,1);// spread Input Mat to single row
    std::vector<double> vecFromMat;
    input.copyTo(vecFromMat); // Copy Input Mat to vector vecFromMat
    std::nth_element(vecFromMat.begin(), vecFromMat.begin() + vecFromMat.size() / 2, vecFromMat.end());
    return vecFromMat[vecFromMat.size() / 2];
}
