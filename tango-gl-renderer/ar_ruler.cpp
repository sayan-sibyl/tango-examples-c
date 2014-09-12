/*
 * Copyright 2014 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ar_ruler.h"

static const char kVertexShader[] = "attribute vec4 vertex;\n"
    "attribute vec4 color;\n"
    "uniform mat4 mvp;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_Position = mvp*vertex;\n"
    "  v_color = color;\n"
    "}\n";

static const char kFragmentShader[] = "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_FragColor = v_color;\n"
    "}\n";

static const float vertices[] = { -0.05f, 0.0f, 0.0f,
    0.05f, 0.0f, 0.0f,
    0.05f, 0.0f, -50.0f,
    0.05f, 0.0f, -50.0f,
    -0.05f,0.0f,-50.0f,
    -0.05f, 0.0f, 0.0f
};

static const float colors[] = {0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f
};

ArRuler::ArRuler() {
  shader_program_ = GlUtil::CreateProgram(kVertexShader, kFragmentShader);
  if (!shader_program_) {
    LOGE("Could not create program.");
  }
  uniform_mvp_mat_ = glGetUniformLocation(shader_program_, "mvp");
  attrib_colors_ = glGetAttribLocation(shader_program_, "color");
  attrib_vertices_ = glGetAttribLocation(shader_program_, "vertex");

  glGenBuffers(1, &vertex_buffer_);
  glGenBuffers(1, &color_buffer_);
}

void ArRuler::Render(glm::mat4 projection_mat, glm::mat4 view_mat) {
  glUseProgram(shader_program_);

  // Calculate model view projection matrix of this object.
  glm::mat4 model_mat = GetCurrentModelMatrix();
  glm::mat4 mvp_mat = projection_mat * view_mat * model_mat;
  glUniformMatrix4fv(uniform_mvp_mat_, 1, GL_FALSE, glm::value_ptr(mvp_mat));

  // Binding vertex buffer.
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3*6, vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(attrib_vertices_);
  glVertexAttribPointer(attrib_vertices_, 3, GL_FLOAT, GL_FALSE, 0,
                        (const void*) 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Binding color buffer.
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4*6, colors, GL_STATIC_DRAW);
  glEnableVertexAttribArray(attrib_colors_);
  glVertexAttribPointer(attrib_colors_, 4, GL_FLOAT, GL_FALSE, 0,
                        (const void*) 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDrawArrays(GL_TRIANGLES, 0, 3*6);
  glUseProgram(0);
}
