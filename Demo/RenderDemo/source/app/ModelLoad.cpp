#include "app/ModelLoad.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

bool ModelLoad::setup()
{
    const char* vertexCode = R"##(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
	)##";

    const char* fragmentCode = R"##(
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    //FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(1.0, 0.0, 0.0, 1);
}
	)##";

    models.push_back(Model{"data/backpack/backpack.obj"});
    shader = std::make_unique<Shader>(vertexCode, fragmentCode);

    glClearColor(0.2f, 0.0f, 0.0f, 0.f);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void ModelLoad::render()
{
    // Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->setActive();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));  // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));      // it's a bit too big for our scene, so scale it down
    shader->set("model", model);

    glm::mat4 view = getCamera().getView();
    shader->set("view", view);

    glm::mat4 projection = getWindow().getProjection(45.f, 0.1f, 100.f);
    shader->set("projection", projection);
    
    // render the loaded model

    for (const auto& model : models)
    {
        model.draw(*shader);
    }
}