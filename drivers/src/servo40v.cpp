#include "../include/servo40v.hpp"


#include <array>
#include <libhal/timeout.hpp>
#include <libhal-util/i2c.hpp>
#include <cstring> 
#include <libhal-util/bit_bang_i2c.hpp> 

using namespace std::chrono_literals;

sjsu::drivers::Servo40V::Servo40V(hal::i2c& p_i2c, hal::steady_clock& p_clock, hal::serial& p_terminal)
    : m_i2c(p_i2c), m_clock(p_clock), m_terminal(p_terminal) {}

void sjsu::drivers::Servo40V::writeRegister(char reg, float value) {
    std::array<hal::byte, 5> data;
    data[0] = static_cast<hal::byte>(reg);
    std::memcpy(&data[1], &value, sizeof(float));
    hal::write(m_i2c, deviceAddress, data);
    
    //hal::write(m_i2c, deviceAddress, std::array<hal::byte, 2> {reg, value});
}

// Helper function to read a float value from a register
float sjsu::drivers::Servo40V::readRegister(char reg) {
     std::array<hal::byte, 4> data;
    hal::write(m_i2c, deviceAddress, std::array<hal::byte, 1>{static_cast<hal::byte>(reg)});
    hal::read(m_i2c, deviceAddress, data);

    float value;
    std::memcpy(&value, data.data(), sizeof(float));
    //float* floatPointer = reinterpret_cast<float*>(&data);
    //value = *floatPointer;

    return value;
}

// PID Constants
void sjsu::drivers::Servo40V::setProportional(float p) {
    writeRegister('p', p);
}

void sjsu::drivers::Servo40V::setIntegral(float i) {
    writeRegister('i', i);
}

void sjsu::drivers::Servo40V::setDerivative(float d) {
    writeRegister('d', d);
}

void sjsu::drivers::Servo40V::setFeedForward(float f) {
    writeRegister('f', f);
}

// Setpoint
void sjsu::drivers::Servo40V::setSetpoint(float setpoint) {
    writeRegister('s', setpoint);
}

// Mode Register
void sjsu::drivers::Servo40V::setMode(hal::byte mode, float newSetpoint) {
    std::array<hal::byte, 6> data; // Updated size

    // First byte is the register ('m')
    data[0] = static_cast<hal::byte>('m');

    // Second byte is the mode
    data[1] = mode;

    // Pack the float into the remaining bytes
    std::memcpy(&data[2], &newSetpoint, sizeof(float));

    // Send the data over I2C
    hal::write(m_i2c, deviceAddress, data);
}


    //hal::write(m_i2c, deviceAddress, std::array<hal::byte, 3> {'m', mode, newSetpoint});


// Read Functions
float sjsu::drivers::Servo40V::getAngle() {
    return readRegister('x');
}

float sjsu::drivers::Servo40V::getVelocityChange() {
    return readRegister('v');
}

float sjsu::drivers::Servo40V::getCurrent() {
    return readRegister('a');
}