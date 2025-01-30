	sudo apt-get install libhidapi-dev
	sudo apt-get install libusb-1.0-0-dev
g++ -o game main.cpp controller.cpp view.cpp -lhidapi-hidraw
sudo ./game
