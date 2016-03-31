/*
argv: argc == 5
1) path_to_image
2) path_to_output_image
3) chanal_to_process
4) Threshold
====================
argv: argv == 2
1) path_to_image
*/

#include <opencv2/opencv.hpp>
#include <typeinfo>
using namespace std;
using namespace cv;

class ChannelBinarApp
{
public:
	ChannelBinarApp() {} 
	~ChannelBinarApp() {} 
	void show(Mat img);
	void imgBin(Mat img);
	void noGui(Mat img, const String& outPath, const int& cnl, const int& thr);
private:
	ChannelBinarApp(const ChannelBinarApp&);
	ChannelBinarApp& operator=(const ChannelBinarApp&);
private:
	static void on_chanels(int iChannel, void* param);
	static void on_threshold(int iThreshold, void* param);
private:
	static String wndTitle_;
	Mat         imgSrc_;
	vector<Mat> imgCh_;
	int			slcChanal{0};
	bool		gui{ true };
	String pathOut;
};

int main(int argc, const char* argv[])
{
	ChannelBinarApp app;
	Mat imgSrc;
	if ((argc == 5) && (0 <= atoi(argv[3])) && (atoi(argv[3]) <= 3) && (0 <= atoi(argv[4])) && (atoi(argv[4]) <= 255) && imgSrc.data)
	{
		imgSrc = imread(argv[1]);
		app.noGui(imgSrc, argv[2], atoi(argv[3]), atoi(argv[4]));
	}
	else if(argc == 2)
	{
		imgSrc = imread(argv[1]);
		if (imgSrc.data)
		{
			app.show(imgSrc);
			for (int key(0); 27 != key; key = waitKey(1))
			{
			}
			waitKey(0);
		}
		else
		{
			cout << "Check input image" << endl;
		}
	}
	else
	{
		cout << "argv: argc == 5\n 1) path_to_image\n2) path_to_output_image\n3)";
		cout << "chanal_to_process\n4) Threshold\n====================\nargv: argv == 2\n1) path_to_image" << endl;
	}
	
}

String ChannelBinarApp::wndTitle_("ChannelBinarApp");

void ChannelBinarApp::show(Mat img)
{
	imgSrc_ = img;
	namedWindow(wndTitle_, 1);
	createTrackbar("chanels", wndTitle_, 0, img.channels(), on_chanels, this);
	createTrackbar("threshold", wndTitle_, 0, 256, on_threshold, this);
	imshow(wndTitle_, imgSrc_);
}

void ChannelBinarApp::imgBin(Mat img)
{
	imgSrc_ = img;
}

void ChannelBinarApp::noGui(Mat img, const String& outPath, const int& cnl, const int& thr)
{
	pathOut = outPath;
	imgSrc_ = img;
	gui = false;
	on_chanels(cnl, this);
	on_threshold(thr, this);
}

void ChannelBinarApp::on_chanels(int iChannel, void* param)
{
	ChannelBinarApp& app(*reinterpret_cast<ChannelBinarApp*>(param));
	if (0 == iChannel && app.gui == true)
	{
		imshow(wndTitle_, app.imgSrc_);
	}
	else
	{
		app.slcChanal = iChannel;
	}
}

void ChannelBinarApp::on_threshold(int iThreshold, void* param)
{
	ChannelBinarApp& app(*reinterpret_cast<ChannelBinarApp*>(param));
	if (0 == app.slcChanal && app.gui == true)
	{
		imshow(wndTitle_, app.imgSrc_);
	}
	else
	{
		Mat _tmp, toShow;
		app.imgCh_.resize(app.imgSrc_.channels());
		split(app.imgSrc_, app.imgCh_);
		threshold(app.imgCh_[app.slcChanal - 1], _tmp, iThreshold, 255, THRESH_BINARY);
		app.imgCh_[app.slcChanal - 1] = _tmp;
		merge(app.imgCh_, toShow);
		if (app.gui == true)
		{
			imshow(wndTitle_, toShow);
		}
		else
		{
			imwrite(app.pathOut, toShow);
		}
	}
}
