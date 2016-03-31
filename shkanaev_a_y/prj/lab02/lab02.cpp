/*
argv: argc == 5
1) path_to_image
2) path_to_output_image
3) size_of_sobel_kernel (1<= <=7, odd) 
4) size_of_median_kernel (greater than 1, odd)
====================
argv: argv == 2
1) path_to_image
*/

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;



class SomeFilterApp
{
public:
	SomeFilterApp() {}
	~SomeFilterApp() {}
	void show(Mat img);
	void noGui(Mat img, const String& outPath, const int& sob, const int& med);
private:
	SomeFilterApp(const SomeFilterApp&);
	SomeFilterApp& operator=(const SomeFilterApp&);
private:
	static void on_kernSob(int size, void* param);
	static void on_kernMed(int size, void* param);
private:
	Mat         imgSrc_;
	static String wndTitle_;
};

String SomeFilterApp::wndTitle_("SomeFilterApp");

void SomeFilterApp::show(Mat img)
{
	Mat box, gaus;
	imgSrc_ = img;
	namedWindow(wndTitle_, 1);
	createTrackbar("Krnl.Sob", wndTitle_, 0, 7, on_kernSob, this);
	createTrackbar("Krnl.Med", wndTitle_, 0, 20, on_kernMed, this);
	imshow(wndTitle_, imgSrc_);
}

void SomeFilterApp::noGui(Mat img, const String & outPath, const int & sob, const int & med)
{
	if (img.data && !outPath.empty())
	{
		Mat sobImg, medImg;
		if (sob >= 1 && sob <= 7 && sob % 2 == 1) {
			Sobel(img, sobImg, img.depth(), 0, 1, sob);
		}
		else {
			sobImg = img;
		}
		if (med >= 1 && med % 2 == 1)
		{
			medianBlur(sobImg, medImg, med);
		}
		else {
			medImg = sobImg;
		}
		imwrite(outPath, medImg);
	}
}

void SomeFilterApp::on_kernSob(int size, void * param)
{
	int kerSiz = cvGetTrackbarPos("Krnl.Med", wndTitle_.c_str());
	if (size >= 1 && size <= 7 && size % 2 == 1)
	{
		SomeFilterApp& app(*reinterpret_cast<SomeFilterApp*>(param));
		Mat sobImg, medImg;
		Sobel(app.imgSrc_, sobImg, app.imgSrc_.depth(), 0, 1, size);
		if (kerSiz % 2 == 1 && kerSiz >= 3) {
			medianBlur(sobImg, medImg, kerSiz);
		}
		else
		{
			medImg = sobImg;
		}
		imshow(wndTitle_, medImg);
	}

}

void SomeFilterApp::on_kernMed(int size, void * param)
{
	if (size % 2 == 1 && size >= 3)
	{
		int kerSiz = cvGetTrackbarPos("Krnl.Sob", wndTitle_.c_str());
		SomeFilterApp& app(*reinterpret_cast<SomeFilterApp*>(param));
		Mat sobImg, gauImg;
		if (kerSiz >= 1 && kerSiz <=7 && kerSiz % 2 == 1) {
			Sobel(app.imgSrc_, sobImg, app.imgSrc_.depth(), 0, 1, kerSiz);
		}
		else {
			sobImg = app.imgSrc_;
		}
		medianBlur(sobImg, gauImg, size);
		imshow(wndTitle_, gauImg);
	}
}

int main(int argc, const char* argv[])
{
	SomeFilterApp app;
	Mat imgSrc;
	if (argc == 2)
	{
		imgSrc = imread(argv[1]);
		if (imgSrc.data) {
			app.show(imgSrc);
			for (int key(0); 27 != key; key = waitKey(1))
			{
			}
		}
		waitKey(0);
	}
	else if (argc == 5)
	{
		imgSrc = imread(argv[1]);
		app.noGui(imgSrc, argv[2], atoi(argv[3]), atoi(argv[4]));
	}
	else
	{
		cout << "argv: argc == 5\n 1) path_to_image\n2) path_to_output_image\n3)";
		cout << "size_of_sobel_kernel (1<= <=7, odd)\n4) size_of_median_kernel (greater than 1, odd)";
		cout << "\n == == == == == == == == == == \nargv: argv == 2\n1) path_to_image" << endl;
	}
}