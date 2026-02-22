#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint VAO, VBO;

GLuint CompileShader(const char *path, GLenum shaderType)
{
    std::ifstream shaderFile(path);
    if (!shaderFile.is_open())
    {
        std::cerr << "ERROR: Shader file not found: " << path << std::endl;
        return 0;
    }

    std::string shaderCode((std::istreambuf_iterator<char>(shaderFile)),
                           std::istreambuf_iterator<char>());
    const char *shaderSource = shaderCode.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader compilation failed: " << path << "\n"
                  << infoLog << std::endl;
        return 0;
    }

    return shader;
}

GLuint CreateShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath)
{
    GLuint vertexShader = CompileShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    if (vertexShader == 0 || fragmentShader == 0)
    {
        std::cerr << "ERROR: Shader compilation failed. Exiting." << std::endl;
        return 0;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR: Program linking failed\n"
                  << infoLog << std::endl;
        return 0;
    }

    return shaderProgram;
}

void SetupCube(float *vertices)
{
    float cubeVertices[] = {
        // Posições dos vértices e cores para o cubo
        -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 0.5f
    };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main()
{
    // Inicializa SFML com OpenGL moderno
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    settings.depthBits = 24;

    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL Modern Test", sf::Style::Default, settings);

    if (!window.isOpen())
    {
        std::cerr << "Failed to create SFML window!" << std::endl;
        return -1;
    }

    // Inicializando GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    GLuint shaderProgram = CreateShaderProgram("vertex_shader.glsl", "fragment_shader.glsl");

    float vertices[72];
    SetupCube(vertices);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

    glm::mat4 model = glm::mat4(1.0f); // Inicializa a matriz de modelo com a identidade

    float angle = 0.0f; // Ângulo de rotação

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Atualiza o ângulo de rotação
        angle += 0.01f; // Aumenta o ângulo a cada quadro

        // Cria a rotação em torno do eixo Y
        model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

        // Limpa o buffer de cor e profundidade
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Passa as matrizes para os shaders
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // Desenha o cubo
        glBindVertexArray(VAO);
        glDrawArrays(GL_QUADS, 0, 24);
        glBindVertexArray(0);

        window.display();
    }

    return 0;
}