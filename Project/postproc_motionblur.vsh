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

#version 150
#extension GL_ARB_explicit_attrib_location: enable
precision highp float;

// hacky way of avoiding enabling and disabling attribs every frame
layout(location = 10) in vec2 a_vCoord;

smooth out vec2 v_vCoord;

void main(void)
{
    gl_Position = vec4(a_vCoord, 0.0, 1.0);
    v_vCoord = (a_vCoord + 1.0) / 2.0;
}
