cmake_minimum_required(VERSION 2.8)
project( ImageFilter C )

find_package( OpenCV REQUIRED)
#FIND_PACKAGE( OpenMP REQUIRED)

if(OPENMP_FOUND)
	message("OPENMP FOUND")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable( ${PROJECT_NAME} main.c sampling.c TrainingNN.c ImageIO.c svlm.c GA/GAGene.c)

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )


