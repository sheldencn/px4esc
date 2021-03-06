/**
 * Copyright (c) 2016  Zubax Robotics OU  <info@zubax.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *    following disclaimer in the documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *    products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <math/math.hpp>
#include <zubax_chibios/util/heapless.hpp>


namespace foc
{
namespace motor_id
{

using math::Scalar;

/**
 * Greater modes (listed later) allow to identify more parameters,
 * but impose more restrictions on the connected load.
 * Read the comments for details.
 */
enum class Mode
{
    /**
     * In this mode, the motor will not rotate, therefore it doesn't matter what load it is connected to.
     * Estimated parameters: Rs, L.
     */
    Static,

    /**
     * In this mode, the motor WILL SPIN.
     * In order to achieve correct results, the motor MUST NOT BE CONNECTED TO ANY MECHANICAL LOAD.
     * Estimated parameters: Rs, L, Phi.
     */
    RotationWithoutMechanicalLoad
};

/**
 * Parameters used by the motor parameters estimator logic.
 * Provided defaults should be valid for most, if not all, use cases.
 */
struct Parameters
{
    /// Fraction of the maximum motor current used for identification
    Scalar fraction_of_max_current = 0.3F;

    /// Current frequency used for inductance identification, Hertz
    Scalar current_injection_frequency = 1000.0F;

    /// Electrical angular velocity used for magnetic flux linkage identification, radian/second
    Scalar phi_estimation_electrical_angular_velocity = 150.0F;


    bool isValid() const
    {
        return math::Range<>(0.01F, 1.0F).contains(fraction_of_max_current) &&
               math::Range<>(10.0F, 100000.0F).contains(current_injection_frequency) &&
               math::Range<>(10.0F, 10000.0F).contains(phi_estimation_electrical_angular_velocity);
    }

    auto toString() const
    {
        return os::heapless::format("FracI: %.0f %%\n"
                                    "Finj : %.1f Hz\n"
                                    "Wphi : %.1f rad/s",
                                    double(fraction_of_max_current * 100.0F),
                                    double(current_injection_frequency),
                                    double(phi_estimation_electrical_angular_velocity));
    }
};

}
}
