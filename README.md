# ImageFilter

Create a debug project with eclipse:
	cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_MAKE_ARGUMENTS=-j4  ../src/.

Create a build project:
	cmake ../src/.

Building a project
	cmake --build . -- -j4
