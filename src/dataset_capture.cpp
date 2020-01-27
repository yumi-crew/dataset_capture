#include <Zivid/Zivid.h>
#include <Zivid/CloudVisualizer.h>
#include <iostream>
#include <fstream>
#include <dataset_capture/pose_grabber.h>


Zivid::Frame capture_frame(Zivid::Camera* cam) {
	std::vector<Zivid::Frame> frames;
	for (const size_t iris : { 20U, 25U, 30U }) {
		*cam << Zivid::Settings::Iris{ iris };
		frames.emplace_back(cam->capture());
	}
	auto hdrFrame = Zivid::HDR::combineFrames(begin(frames), end(frames));
	return hdrFrame;
}


int main(int argc, char* argv[]) {
	
	//default values
	int dataset_size{20};
	std::string dest_dir{"default_dir"};
	std::string robot_ip_addr{"192.168.125.1"};

	if (argc == 3) {
		dataset_size = std::stoi(argv[1]);
		dest_dir = argv[2];
	}

	std::cout << "Number of images: " << dataset_size << std::endl;
	std::cout << "Destination directory: " << dest_dir << std::endl;
	std::cout << "Robot ip address: " << robot_ip_addr << std::endl;

	Zivid::Application zivid;
	std::cout << "Connecting to camera...\n";
	Zivid::Camera camera = zivid.connectCamera();
	// Zivid::Camera camera = zivid.createFileCamera("ZividOnePlusMedium.zdf");//("img.zdf");
	std::cout << "Connected\n";

	//init point cloud visualizer
	Zivid::CloudVisualizer vis;
	//init pose grabber
	auto pose_grabber = std::make_shared<Pose_grabber>(robot_ip_addr);
	pose_grabber->init();

	std::string save_query{ "" };
	std::vector<double> pose;
	std::string continue_query;
	int counter{ 0 };
	while (counter < dataset_size) {
		std::cout << "Press enter for next capture\n";
		std::cin >> continue_query;
		try {
			Zivid::Frame frame = capture_frame(&camera);
			// display the image for manual verification
			vis.showMaximized();
			vis.show(frame);
			vis.resetToFit();
			vis.run(); //blocking until closed

			std::cout << "Save frame [y/n]?  >>> \n";
			std::cin >> save_query;

			if (!save_query.compare("y")) {
				
				pose = pose_grabber->grab_pose("ROB_L");
				std::ofstream pose_file(dest_dir + "/pose" + std::to_string(counter) + ".txt");
				for (auto p : pose) { //write pose to terminal and file
					std::cout << p << " ";
					pose_file << p << " ";
				}
				pose_file.close();

				frame.save(dest_dir + "/img" + std::to_string(counter) + ".zdf");
				counter++;
			}
		}
		catch (const std::exception & e) {
			std::cerr << "Error: " << Zivid::toString(e) << std::endl;
			return EXIT_FAILURE;
		}
	}
	return 0;
}