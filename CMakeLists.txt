cmake_minimum_required(VERSION 3.10)
project(RayTracer13 VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(RayTracer13 
	main.cpp

	output/ppm/PPMColor.h

	output/ppm/PPMImage.h
	output/ppm/PPMImage.cpp

	output/ppm/PPMImageFileWriter.h
	output/ppm/PPMImageFileWriter.cpp

	output/ppm/PPMImageMeta.h
	output/ppm/PPMImageMeta.cpp

	output/ppm/PPMImageWriter.h
	output/ppm/PPMImageWriter.cpp
)

enable_testing()