/*
 *  Copyright 2013 David Farrell
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "ParticlesDrawable.h"

template <>
void BoundedValue<GLfloat>::Clamp(void)
{
    if(value < min) value = min;
    if(value > max) value = max;
}

template <>
void BoundedValue<glm::vec3>::Clamp(void)
{
    if(value.x < min.x) value.x = min.x;
    if(value.y < min.y) value.y = min.y;
    if(value.z < min.z) value.z = min.z;

    if(value.x > max.x) value.x = max.x;
    if(value.y > max.y) value.y = max.y;
    if(value.z > max.z) value.z = max.z;
}
