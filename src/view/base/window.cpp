
#include "window.h"

FrameDimensions getNormalizedDimentions(int newWidth, int newHeight) {
	FrameDimensions frame{};

	float widthGCD{ newWidth / widthRatioFloat };
	float heightGCD{ newHeight / heightRatioFloat };

	if (static_cast<int>(widthGCD) != static_cast<int>(heightGCD)) {
		//std::cout << "gcd not equal" << std::endl << std::endl;
		if (heightGCD > widthGCD) {
			//std::cout << "Height Too Big" << std::endl;
			float correctedHeight{ heightRatioFloat * widthGCD };

			frame.width = newWidth;
			frame.height = correctedHeight;
			frame.scale = correctedHeight / standardWindowHeight;

			frame.x = 0.0f; 
			frame.y = (newHeight - correctedHeight) / 2.0f;
		}
		else {
			//std::cout << "Width Too Big" << std::endl;
			float correctedWidth{ widthRatioFloat * heightGCD };

			frame.width = correctedWidth;
			frame.height = newHeight;
			frame.scale = correctedWidth / standardWindowWidth;

			frame.x = (newWidth - correctedWidth) / 2.0f;
			frame.y = 0.0f;
		}
	}
	else {
		//std::cout << "gcd equal" << std::endl << std::endl;

		frame.width = newWidth;
		frame.height = newHeight;
		frame.scale = newWidth / standardWindowWidth;
		 
		frame.x = 0;
		frame.y = 0;
	}

	return frame;
}

//float widthGCD{ newWidth / widthRatioFloat };
//float heightGCD{ newHeight / heightRatioFloat };
//
//if (static_cast<int>(widthGCD) != static_cast<int>(heightGCD)) {
//	//std::cout << "gcd not equal" << std::endl << std::endl;
//	if (heightGCD > widthGCD) {
//		std::cout << "Height Too Big" << std::endl;
//		float correctedHeight{ heightRatioFloat * widthGCD };
//
//
//		sg_frameWidth = newWidth;
//		sg_frameHeight = correctedHeight;
//
//		sg_scale = sg_frameHeight / standardWindowHeight;
//
//
//		glViewport(0, (newHeight - correctedHeight) / 2, newWidth, correctedHeight);
//	}
//	else {
//		std::cout << "Width Too Big" << std::endl;
//		float correctedWidth{ widthRatioFloat * heightGCD };
//
//
//		sg_frameWidth = correctedWidth;
//		sg_frameHeight = newHeight;
//
//		sg_scale = sg_frameWidth / standardWindowWidth;
//
//		glViewport((newWidth - correctedWidth) / 2, 0, correctedWidth, newHeight);
//	}
//}
//else {
//	std::cout << "gcd equal" << std::endl << std::endl;
//	glViewport(0, 0, newWidth, newHeight);
//
//	sg_scale = newWidth / standardWindowWidth;
//
//	sg_frameWidth = newWidth;
//	sg_frameHeight = newHeight;
//}
//
//glfwSetWindowAspectRatio(window, widthRatio, heightRatio);
