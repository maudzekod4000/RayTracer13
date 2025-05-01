cmake_minimum_required(VERSION 3.10)
project(RayTracer13 VERSION 0.1)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

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
	sampling/Ray.h
	sampling/Camera.h
	sampling/Camera.cpp
	sampling/Vertex.h
	sampling/Triangle.h
	sampling/Triangle.cpp
	sampling/Ray.h
	sampling/IntersectionData.h
	sampling/Object.h
	sampling/Object.cpp
	input/RenderConfigDecoder.h
	input/CameraSettings.h
	input/CameraSettings.cpp
	input/ImageSettings.h
	input/ImageSettings.cpp
	input/Settings.h
	input/Settings.cpp
	input/Scene.h
	input/Scene.cpp
	input/RenderConfigDecoderJSON.h
	input/RenderConfigDecoderJSON.cpp
	input/FileReader.h
	input/FileReader.cpp
	input/RenderConfig.h
	calc/TypeDefs.h
)

include("${DIR}/lib/rapidjson/rapidjson.cmake")
add_subdirectory(${DIR}/lib/glm) # Folder where glm's CMakeLists.txt lives
add_dependencies(${PROJECT_NAME} rapidjson)
target_include_directories(${PROJECT_NAME} PRIVATE
${DIR}/sampling
${DIR}/out 
${DIR}/lib 
${DIR}/calc 
${RAPIDJSON_INCLUDE_DIR})