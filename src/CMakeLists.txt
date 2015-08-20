cmake_minimum_required(VERSION 2.8)
project( ImageFilter C )
find_package( OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable( ${PROJECT_NAME} main.c sampling.c TrainingNN.c ImageIO.c svlm.c GA/GAGene.c)
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )


