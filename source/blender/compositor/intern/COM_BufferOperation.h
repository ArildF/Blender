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
 * Copyright 2021, Blender Foundation.
 */

#pragma once

#include "COM_NodeOperation.h"

namespace blender::compositor {

class BufferOperation : public NodeOperation {
 private:
  MemoryBuffer *buffer_;
  MemoryBuffer *inflated_buffer_;

 public:
  BufferOperation(MemoryBuffer *buffer, DataType data_type);

  void *initializeTileData(rcti *rect) override;
  void deinitExecution() override;
  void executePixelSampled(float output[4], float x, float y, PixelSampler sampler) override;
  void executePixelFiltered(float output[4], float x, float y, float dx[2], float dy[2]) override;
};

}  // namespace blender::compositor
