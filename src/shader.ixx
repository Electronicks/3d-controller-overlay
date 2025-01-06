module;
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>

export module shader;

import std;

export GLuint CompileShader(GLuint type, const char* shaderSource)
{
    GLuint shaderObject;

    if (type == GL_VERTEX_SHADER)
    {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else
    {
        return 1;
    }

    glShaderSource(shaderObject, 1, &shaderSource, NULL);
    glCompileShader(shaderObject);

    int result;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        std::string errorMessages(length, '\0');
        glGetShaderInfoLog(shaderObject, length, &length, errorMessages.data());

        if (type == GL_VERTEX_SHADER)
        {
            printf("ERROR: GL_VERTEX_SHADER compilation failed!\n%s", errorMessages.c_str());
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
            printf("ERROR: GL_FRAGMENT_SHADER compilation failed!\n%s", errorMessages.c_str());
        }

        glDeleteShader(shaderObject);

        return 0;
    }

    return shaderObject;
}

export GLuint CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    GLuint programObject = glCreateProgram();

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);
    glLinkProgram(programObject);

    int result = 0;
    glGetProgramiv(programObject, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
        std::string errorMessages(length, '\0');
        glGetProgramInfoLog(programObject, length, &length, errorMessages.data());

        printf("ERROR: Shader Program linking failed! : %s\n", errorMessages.c_str());
    }

    //glValidateProgram(programObject);

    //glDetachShader(programObject, vertexShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return programObject;
}

export std::string GetShaderSource(std::string path)
{
    char* base_path = SDL_GetBasePath();
    std::filesystem::path file_path;
    file_path = std::filesystem::path(base_path);
    std::filesystem::path sub_path(path);
    file_path /= sub_path;

    std::ifstream ifs;
    std::string shader_source;

    ifs = std::ifstream(file_path);
    if (!ifs)
    {
        printf("Uh oh, file could not be opened for reading!\n");
    }
    else
    {
        while (ifs)
        {
            std::string line;
            std::getline(ifs, line);
            shader_source.append(line);
            shader_source.append("\n");
        }
        //printf(shader_source.c_str());
    }

    return shader_source;
}

export void shaderUniformBool(GLuint ID, const char* name, bool value)
{
    glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

export void shaderUniformInt(GLuint ID, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name), value);
}

export void shaderUniformFloat(GLuint ID, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name), value);
}

export void shaderUniformMat4(GLuint ID, const char* name, glm::mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(mat));
}

export void shaderUniformMat3(GLuint ID, const char* name, glm::mat3 mat)
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, false, glm::value_ptr(mat));
}

export void shaderUniformVec3(GLuint ID, const char* name, glm::vec3 vec)
{

    glUniform3fv(glGetUniformLocation(ID, name), 1, &vec[0]);
}

export void shaderUniform2f(GLuint ID, const char* name, float value1, float value2)
{
    glUniform2f(glGetUniformLocation(ID, name), value1, value2);
}

export void shaderUniform3f(GLuint ID, const char* name, float value1, float value2, float value3)
{
    glUniform3f(glGetUniformLocation(ID, name), value1, value2, value3);
}

export void shaderUniform4f(GLuint ID, const char* name, float value1, float value2, float value3, float value4)
{
    glUniform4f(glGetUniformLocation(ID, name), value1, value2, value3, value4);
}

