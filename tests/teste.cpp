#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

struct Object3D {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::vec3 color;
    GLuint VAO, VBO, EBO;
    int indexCount;
    
    Object3D() : position(0.0f), rotation(0.0f), scale(1.0f), color(1.0f), VAO(0), VBO(0), EBO(0), indexCount(0) {}
};

class GameEngine {
private:
    sf::RenderWindow window;
    GLuint shaderProgram;
    GLuint shadowMapFBO;
    GLuint shadowMap;
    std::vector<Object3D> objects;
    
    glm::vec3 cameraPos;
    float cameraYaw;
    float cameraPitch;
    bool mouseCaptured;
    
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 lightSpaceMatrix;
    glm::vec3 lightPos;
    glm::vec3 lightColor;
    
public:
    GameEngine() : cameraPos(0.0f, 3.0f, 8.0f), cameraYaw(-90.0f), cameraPitch(0.0f), 
                   mouseCaptured(false), lightPos(5.0f, 8.0f, 5.0f), lightColor(1.0f, 1.0f, 0.9f) {
        createWindow();
        setupOpenGL();
        setupShadowMapping();
        setupShaders();
        setupScene();
    }
    
    ~GameEngine() {
        for(auto& obj : objects) {
            glDeleteVertexArrays(1, &obj.VAO);
            glDeleteBuffers(1, &obj.VBO);
            if(obj.EBO) glDeleteBuffers(1, &obj.EBO);
        }
        glDeleteFramebuffers(1, &shadowMapFBO);
        glDeleteTextures(1, &shadowMap);
        glDeleteProgram(shaderProgram);
    }
    
    void createWindow() {
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.majorVersion = 3;
        settings.minorVersion = 3;
        
        window.create(sf::VideoMode(1024, 768), "Shadows Working!", sf::Style::Default, settings);
        window.setVerticalSyncEnabled(true);
        window.setMouseCursorVisible(false);
        mouseCaptured = true;
        
        sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);
    }
    
    void setupOpenGL() {
        glewExperimental = GL_TRUE;
        if(glewInit() != GLEW_OK) {
            throw std::runtime_error("Failed to initialize GLEW");
        }
        
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        
        std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    }
    
    void setupShadowMapping() {
        const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
        
        // Create framebuffer
        glGenFramebuffers(1, &shadowMapFBO);
        
        // Create depth texture
        glGenTextures(1, &shadowMap);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        
        // Attach depth texture to framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer not complete!" << std::endl;
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        std::cout << "Shadow map setup complete" << std::endl;
    }
    
    std::string readShaderFile(const std::string& path) {
        std::ifstream file(path);
        if(!file.is_open()) {
            std::cerr << "Failed to open shader: " << path << std::endl;
            return "";
        }
        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
    
    GLuint compileShader(const std::string& source, GLenum type) {
        GLuint shader = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "Shader compilation failed: " << infoLog << std::endl;
            return 0;
        }
        return shader;
    }
    
    void setupShaders() {
        std::string vertexSource = readShaderFile("vertex_shader.glsl");
        std::string fragmentSource = readShaderFile("fragment_shader.glsl");
        
        if(vertexSource.empty() || fragmentSource.empty()) {
            throw std::runtime_error("Failed to load shader files");
        }
        
        GLuint vertexShader = compileShader(vertexSource, GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
        
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "Program linking failed: " << infoLog << std::endl;
        }
        
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        
        std::cout << "Shaders loaded successfully!" << std::endl;
    }
    
    void createCube(Object3D& obj, float size = 1.0f) {
        float half = size / 2.0f;
        
        float vertices[] = {
            // Front face
            -half, -half,  half,  obj.color.r, obj.color.g, obj.color.b,
             half, -half,  half,  obj.color.r, obj.color.g, obj.color.b,
             half,  half,  half,  obj.color.r, obj.color.g, obj.color.b,
            -half,  half,  half,  obj.color.r, obj.color.g, obj.color.b,
            // Back face
            -half, -half, -half,  obj.color.r, obj.color.g, obj.color.b,
             half, -half, -half,  obj.color.r, obj.color.g, obj.color.b,
             half,  half, -half,  obj.color.r, obj.color.g, obj.color.b,
            -half,  half, -half,  obj.color.r, obj.color.g, obj.color.b,
        };
        
        unsigned int indices[] = {
            0,1,2, 2,3,0,      // front
            1,5,6, 6,2,1,      // right
            5,4,7, 7,6,5,      // back
            4,0,3, 3,7,4,      // left
            3,2,6, 6,7,3,      // top
            4,5,1, 1,0,4       // bottom
        };
        
        obj.indexCount = 36;
        
        glGenVertexArrays(1, &obj.VAO);
        glGenBuffers(1, &obj.VBO);
        glGenBuffers(1, &obj.EBO);
        
        glBindVertexArray(obj.VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindVertexArray(0);
    }
    
    void setupScene() {
        // Main red cube (raised to cast shadow on floor)
        Object3D cube1;
        cube1.position = glm::vec3(0.0f, 1.0f, 0.0f);
        cube1.color = glm::vec3(1.0f, 0.2f, 0.2f);
        createCube(cube1);
        objects.push_back(cube1);
        
        // Floor (light gray)
        Object3D floor;
        floor.position = glm::vec3(0.0f, -1.0f, 0.0f);
        floor.scale = glm::vec3(8.0f, 0.2f, 8.0f);
        floor.color = glm::vec3(0.7f, 0.7f, 0.7f);
        createCube(floor);
        objects.push_back(floor);
        
        // Left blue cube
        Object3D cube2;
        cube2.position = glm::vec3(-2.5f, 0.5f, 1.5f);
        cube2.color = glm::vec3(0.2f, 0.3f, 1.0f);
        createCube(cube2);
        objects.push_back(cube2);
        
        // Right green cube
        Object3D cube3;
        cube3.position = glm::vec3(2.5f, 0.5f, 1.5f);
        cube3.color = glm::vec3(0.2f, 1.0f, 0.2f);
        createCube(cube3);
        objects.push_back(cube3);
        
        // Back yellow cube
        Object3D cube4;
        cube4.position = glm::vec3(0.0f, 0.5f, 3.0f);
        cube4.color = glm::vec3(1.0f, 0.8f, 0.2f);
        createCube(cube4);
        objects.push_back(cube4);
        
        std::cout << "Created " << objects.size() << " objects" << std::endl;
    }
    
    void renderShadowMap() {
        // Setup light projection matrix
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 20.0f);
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;
        
        // Render to shadow map
        glViewport(0, 0, 1024, 1024);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        // Set uniforms for shadow pass
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(lightView));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        
        // Disable writing to color buffer
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        
        for(auto& obj : objects) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);
            model = glm::rotate(model, glm::radians(obj.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(obj.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(obj.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, obj.scale);
            
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            
            glBindVertexArray(obj.VAO);
            glDrawElements(GL_TRIANGLES, obj.indexCount, GL_UNSIGNED_INT, 0);
        }
        
        // Re-enable color writing
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, window.getSize().x, window.getSize().y);
    }
    
    void renderScene() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        
        // Set camera matrices
        projection = glm::perspective(glm::radians(45.0f), 
                                      (float)window.getSize().x / window.getSize().y, 
                                      0.1f, 100.0f);
        
        view = glm::lookAt(cameraPos, cameraPos + getCameraFront(), glm::vec3(0.0f, 1.0f, 0.0f));
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        
        // Light uniforms
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
        
        // Bind shadow map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, shadowMap);
        glUniform1i(glGetUniformLocation(shaderProgram, "shadowMap"), 0);
        
        for(auto& obj : objects) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.position);
            model = glm::rotate(model, glm::radians(obj.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(obj.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(obj.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, obj.scale);
            
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            
            glBindVertexArray(obj.VAO);
            glDrawElements(GL_TRIANGLES, obj.indexCount, GL_UNSIGNED_INT, 0);
        }
    }
    
    glm::vec3 getCameraFront() {
        glm::vec3 front;
        front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        front.y = sin(glm::radians(cameraPitch));
        front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        return glm::normalize(front);
    }
    
    void handleInput() {
        float speed = 0.1f;
        glm::vec3 forward = getCameraFront();
        forward.y = 0;
        forward = glm::normalize(forward);
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) cameraPos += forward * speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cameraPos -= forward * speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cameraPos += right * speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) cameraPos -= right * speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) cameraPos.y += speed;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) cameraPos.y -= speed;
        
        if(mouseCaptured && window.hasFocus()) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);
            
            if(mousePos != center) {
                cameraYaw += (mousePos.x - center.x) * 0.2f;
                cameraPitch -= (mousePos.y - center.y) * 0.2f;
                
                if(cameraPitch > 89.0f) cameraPitch = 89.0f;
                if(cameraPitch < -89.0f) cameraPitch = -89.0f;
                
                sf::Mouse::setPosition(center, window);
            }
        }
    }
    
    void update() {
        // Rotate main cube
        objects[0].rotation.y += 2.0f;
        objects[0].rotation.x += 1.0f;
        
        // Move light to show shadows changing
        static float time = 0;
        time += 0.01f;
        lightPos.x = 5.0f + sin(time) * 3.0f;
        lightPos.z = 4.0f + cos(time * 0.8f) * 3.0f;
        
        // Print light position occasionally
        static int frame = 0;
        if(frame++ % 300 == 0) {
            std::cout << "Light position: (" << lightPos.x << ", " << lightPos.y << ", " << lightPos.z << ")" << std::endl;
        }
    }
    
    void run() {
        sf::Clock clock;
        
        while(window.isOpen()) {
            sf::Event event;
            while(window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) window.close();
                else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
                else if(event.type == sf::Event::GainedFocus) {
                    window.setMouseCursorVisible(false);
                    mouseCaptured = true;
                }
                else if(event.type == sf::Event::LostFocus) {
                    window.setMouseCursorVisible(true);
                    mouseCaptured = false;
                }
            }
            
            handleInput();
            update();
            
            // First pass: render shadow map
            renderShadowMap();
            
            // Second pass: render scene with shadows
            renderScene();
            
            window.display();
        }
    }
};

int main() {
    try {
        std::cout << "=== SHADOW TEST ===" << std::endl;
        std::cout << "Controls: WASD + Mouse | Space/Shift | ESC to quit" << std::endl;
        std::cout << "Light moves automatically - watch the shadows change!" << std::endl;
        std::cout << std::endl;
        
        GameEngine engine;
        engine.run();
    }
    catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}