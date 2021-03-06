/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Copyright 2011, Blender Foundation.
 */

#include "COM_ColorBalanceLGGOperation.h"
#include "BLI_math.h"

namespace blender::compositor {

inline float colorbalance_lgg(float in, float lift_lgg, float gamma_inv, float gain)
{
  /* 1:1 match with the sequencer with linear/srgb conversions, the conversion isn't pretty
   * but best keep it this way, since testing for durian shows a similar calculation
   * without lin/srgb conversions gives bad results (over-saturated shadows) with colors
   * slightly below 1.0. some correction can be done but it ends up looking bad for shadows or
   * lighter tones - campbell */
  float x = (((linearrgb_to_srgb(in) - 1.0f) * lift_lgg) + 1.0f) * gain;

  /* prevent NaN */
  if (x < 0.0f) {
    x = 0.0f;
  }

  return powf(srgb_to_linearrgb(x), gamma_inv);
}

ColorBalanceLGGOperation::ColorBalanceLGGOperation()
{
  this->addInputSocket(DataType::Value);
  this->addInputSocket(DataType::Color);
  this->addOutputSocket(DataType::Color);
  this->m_inputValueOperation = nullptr;
  this->m_inputColorOperation = nullptr;
  this->setResolutionInputSocketIndex(1);
}

void ColorBalanceLGGOperation::initExecution()
{
  this->m_inputValueOperation = this->getInputSocketReader(0);
  this->m_inputColorOperation = this->getInputSocketReader(1);
}

void ColorBalanceLGGOperation::executePixelSampled(float output[4],
                                                   float x,
                                                   float y,
                                                   PixelSampler sampler)
{
  float inputColor[4];
  float value[4];

  this->m_inputValueOperation->readSampled(value, x, y, sampler);
  this->m_inputColorOperation->readSampled(inputColor, x, y, sampler);

  float fac = value[0];
  fac = MIN2(1.0f, fac);
  const float mfac = 1.0f - fac;

  output[0] = mfac * inputColor[0] +
              fac * colorbalance_lgg(
                        inputColor[0], this->m_lift[0], this->m_gamma_inv[0], this->m_gain[0]);
  output[1] = mfac * inputColor[1] +
              fac * colorbalance_lgg(
                        inputColor[1], this->m_lift[1], this->m_gamma_inv[1], this->m_gain[1]);
  output[2] = mfac * inputColor[2] +
              fac * colorbalance_lgg(
                        inputColor[2], this->m_lift[2], this->m_gamma_inv[2], this->m_gain[2]);
  output[3] = inputColor[3];
}

void ColorBalanceLGGOperation::update_memory_buffer_row(PixelCursor &p)
{
  for (; p.out < p.row_end; p.next()) {
    const float *in_factor = p.ins[0];
    const float *in_color = p.ins[1];
    const float fac = MIN2(1.0f, in_factor[0]);
    const float fac_m = 1.0f - fac;
    p.out[0] = fac_m * in_color[0] +
               fac * colorbalance_lgg(in_color[0], m_lift[0], m_gamma_inv[0], m_gain[0]);
    p.out[1] = fac_m * in_color[1] +
               fac * colorbalance_lgg(in_color[1], m_lift[1], m_gamma_inv[1], m_gain[1]);
    p.out[2] = fac_m * in_color[2] +
               fac * colorbalance_lgg(in_color[2], m_lift[2], m_gamma_inv[2], m_gain[2]);
    p.out[3] = in_color[3];
  }
}

void ColorBalanceLGGOperation::deinitExecution()
{
  this->m_inputValueOperation = nullptr;
  this->m_inputColorOperation = nullptr;
}

}  // namespace blender::compositor
