#include <unistd.h>
#include <stdio.h>

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <sstream>
#include <memory>

#include "boost_util.h"
#include "arg_parse.h"
#include "display_sender.h"

using namespace std;
using namespace cv;

std::shared_ptr<Mat> process(Mat& m)
{
	std::shared_ptr<Mat> ret = make_shared<Mat>(m);
	return ret;
}

int main(int argc, const char* argv[])
{
	arg_parse args;
	string arg_error = args.parse(argc, argv);
	if(arg_error.length() > 0){
		cout << "arg parse error:" << arg_error << endl;
		return 1;
	}
	boost_util::set_log_level(args.log_level, 0);
	stringstream ss_args;
	args.print(ss_args);
	_li << ss_args.str();

	shared_ptr<display_sender> display_sender_;
	if(args.display_ip_address.size() > 0){
		display_sender_ = std::make_shared<display_sender>(0, args.display_ip_address, args.display_port);
	}

	Mat m_input = imread(args.input_image.c_str());
	if(m_input.cols == 0){
		_le << "invalid input_image path:" << args.input_image;
		return 1;
	}
	auto m_send = process(m_input);

	if(display_sender_ != nullptr && m_send != nullptr){
		display_sender_->send(*(m_send.get()));
		_li << "sended(" << m_send->cols << ", " << m_send->rows << ")";
	}

	getchar();

	return 0;
}
