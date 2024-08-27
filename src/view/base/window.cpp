
#include "window.h"

std::pair<FrameDimentions, ViewportUpdateParams> getNormalizedDimentions(int newWidth, int newHeight) {
	 FrameDimentions dimentions{};
	 ViewportUpdateParams viewportParams{};

	 float widthGCD{ newWidth / widthRatioFloat };
	 float heightGCD{ newHeight / heightRatioFloat };

	 if (static_cast<int>(widthGCD) != static_cast<int>(heightGCD)) {
		 //std::cout << "gcd not equal" << std::endl << std::endl;
		 if (heightGCD > widthGCD) {
			 //std::cout << "Height Too Big" << std::endl;
			 float correctedHeight{ heightRatioFloat * widthGCD };

			 dimentions.frameWidth = newWidth;
			 dimentions.frameHeight = correctedHeight;

			 dimentions.scale = correctedHeight / standardWindowHeight;

			 viewportParams = { 0, static_cast<int>((newHeight - correctedHeight) / 2), newWidth, static_cast<int>(correctedHeight) };
		 }
		 else {
			 //std::cout << "Width Too Big" << std::endl;
			 float correctedWidth{ widthRatioFloat * heightGCD };

			 dimentions.frameWidth = correctedWidth;
			 dimentions.frameHeight = newHeight;
			 dimentions.scale = correctedWidth / standardWindowWidth;

			 viewportParams = { static_cast<int>((newWidth - correctedWidth) / 2), 0, static_cast<int>(correctedWidth), newHeight };
		 }
	 }
	 else {
		 //std::cout << "gcd equal" << std::endl << std::endl;

		 dimentions.frameWidth = newWidth;
		 dimentions.frameHeight = newHeight;
		 dimentions.scale = newWidth / standardWindowWidth;
		 
		 viewportParams = { 0, 0, newWidth, newHeight };
	 }

	 return std::pair(dimentions, viewportParams);
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
