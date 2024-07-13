#include <iostream>
#include <cstdlib>
#include "RtMidi.h"

void midiCallback(double deltatime, std::vector<unsigned char> *message, void *userData) {
    unsigned int nBytes = message->size();
    for (unsigned int i = 0; i < nBytes; i++) {
        std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    }
    if (nBytes > 0) {
        std::cout << "stamp = " << deltatime << std::endl;
    }
}

int main() {
    RtMidiIn midiIn;
    unsigned int portNumber = 0;

    unsigned int nPorts = midiIn.getPortCount();
    if (nPorts == 0) {
        std::cout << "No MIDI input ports available." << std::endl;
        return 1;
    }

    std::cout << "Available MIDI input ports:" << std::endl;
    for (unsigned int i = 0; i < nPorts; i++) {
        std::string portName = midiIn.getPortName(i);
        std::cout << "  Port " << i << ": " << portName << std::endl;
    }

    std::string inputStr;
    std::cout << "Select a port: ";
    std::getline(std::cin, inputStr); // Read entire line

    try {
        portNumber = std::stoi(inputStr); // Convert string to integer
    } catch (const std::exception& e) {
        std::cerr << "Invalid input: " << e.what() << std::endl;
        return 1;
    }

    if (portNumber >= nPorts) {
        std::cerr << "Invalid port number selected." << std::endl;
        return 1;
    }

    midiIn.openPort(portNumber);
    std::cout << "Opened MIDI input port: " << midiIn.getPortName(portNumber) << std::endl;

    midiIn.setCallback(&midiCallback);
    midiIn.ignoreTypes(false, false, false);

    std::cout << "Listening for MIDI input... Press Enter to quit." << std::endl;
    char input;
    std::cin.get(input); // Wait for Enter key press

    midiIn.closePort();
    return 0;
}
