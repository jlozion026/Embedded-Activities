.PHONY: default compile flash

default: compile flash

compile:
	arduino-cli compile -b esp8266:esp8266:nodemcu

flash:
	arduino-cli upload -v -p /dev/ttyUSB0 -b esp8266:esp8266:nodemcu