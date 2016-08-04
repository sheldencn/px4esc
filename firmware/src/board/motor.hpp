/****************************************************************************
*
*   Copyright (C) 2016  Zubax Robotics  <info@zubax.com>
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name PX4 nor the names of its contributors may be
*    used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#pragma once

#include <cstdint>
#include <functional>
#include <math.hpp>
#include <zubax_chibios/util/heapless.hpp>


namespace board
{
/**
 * This namespace contains API to the motor control hardware - PWM, ADC, Driver IC, etc.
 */
namespace motor
{
/**
 * After initialization the driver will enter the inactive mode.
 */
void init();

/**
 * Activates/deactivates the power stage hardware.
 * Must be activated before the motor can be started.
 * Must be deactivated after the motor is stopped.
 * The default state is deactivated.
 */
void setActive(bool active);

/**
 * @ref setActive().
 */
bool isActive();

/**
 * Meaningful results guaranteed only after initialization.
 * @return PWM carrier frequency in Hertz.
 */
float getPWMFrequency();

/**
 * Meaningful results guaranteed only after initialization.
 * @return PWM dead time in seconds.
 */
float getPWMDeadTime();

/**
 * This function should only be called after @ref activate() and before @ref deactivate().
 * @param abc           PWM values per channel in the range [0, 1].
 */
void setPWM(const math::Vector<3>& abc);

/**
 * Immediately deactivates the PWM outputs (shuts down the carrier).
 * Further use of the driver may not be possible.
 * This function can be called from ANY context, e.g. from Hard Fault handler.
 */
void emergency();

/**
 * @ref getStatus().
 */
struct Status
{
    float inverter_temperature = 0.0F;          ///< Kelvin
    float inverter_voltage = 0.0F;              ///< Volt

    bool bad_power = false;                     ///< PWRGD
    bool overload = false;                      ///< OCTW
    bool fault = false;                         ///< FAULT

    bool isOkay() const
    {
        return !(bad_power || overload || fault);
    }

    auto toString() const
    {
        return os::heapless::concatenate<60>("BadPower: ", bad_power, ", ",
                                             "Overload: ", overload, ", ",
                                             "Fault: ", fault, ", ",
                                             "InverterVolt: ", inverter_voltage, ", ",
                                             "InverterTemp: ", inverter_temperature);
    }
};

/**
 * Returns immediate status information. @ref Status.
 */
Status getStatus();

/**
 * This function is invoked from the IRQ context when new data arrives. It must be defined elsewhere.
 * All units are SI units.
 */
extern void handleSampleIRQ(const math::Vector<2>& phase_currents_ab,
                            const float inverter_voltage);

}
}
