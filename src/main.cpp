#include <main.h>

void checkGlError() {
    GLenum glError = glGetError();

    switch (glError) {
        case GL_NO_ERROR:
            std::cout << "GL_NO_ERROR\n";
            break;
        case GL_INVALID_ENUM:
            std::cerr << "GL_INVALID_ENUM\n";
            break;
        case GL_INVALID_VALUE:
            std::cerr << "GL_INVALID_VALUE\n";
            break;
        case GL_INVALID_OPERATION:
            std::cerr << "GL_INVALID_OPERATION\n";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION\n";
            break;
        case GL_OUT_OF_MEMORY:
            std::cerr << "GL_OUT_OF_MEMORY\n";
            break;
        case GL_STACK_UNDERFLOW:
            std::cerr << "GL_STACK_UNDERFLOW\n";
            break;
        case GL_STACK_OVERFLOW:
            std::cerr << "GL_STACK_OVERFLOW\n";
            break;
        default:
            std::cerr << "Unknown OpenGL error!\n";
            break;
    }
}

int main(int argc, char* argv[]) {

    Window window("Octo Avenger", 1024, 786, false);
    Context context(window.getWindow());

    float vertices[] = {
        //Position    Texcoords
        -0.5f,  0.8f, 0.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.8f, 1.0f, 0.0f, 0.0f, // Top-right
         0.5f,  0.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
        -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // Bottom-left
            //Mirror
        -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, // Top-left
         0.5f,  0.0f, 1.0f, 1.0f, 1.0f, // Top-right
         0.5f, -0.8f, 1.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.8f, 0.0f, 0.0f, 1.0f // Bottom-left
    };

    GLuint vertexArrayObject;
    
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint elements[] = {
        0, 1, 2,
        0, 3, 2,
        4, 5, 6,
        4, 7, 6
    };

    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


    Shader vertexShader(GL_VERTEX_SHADER, "shaders/simple.vert");
    Shader fragmentShader(GL_FRAGMENT_SHADER, "shaders/simple.frag");

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader.getReference());
    glAttachShader(shaderProgram, fragmentShader.getReference());

    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));


    GLint markerAttrib = glGetAttribLocation(shaderProgram, "marker");
    glEnableVertexAttribArray(markerAttrib);
    glVertexAttribPointer(markerAttrib, 1, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(4*sizeof(float)));

    GLuint textures[2];
    glGenTextures(2, textures);

    int width, height;
    unsigned char* image;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    image = SOIL_load_image("assets/textures/sample.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkGlError();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    image = SOIL_load_image("assets/textures/sample2.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    auto timer = glGetUniformLocation(shaderProgram, "time");
    GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    checkGlError();

    SDL_Event windowEvent;
    while(true) {
        glClear(GL_COLOR_BUFFER_BIT);

        float time = (float)clock()/(float)CLOCKS_PER_SEC;

        glm::mat4 trans;
        trans = glm::rotate(trans, time*10.0f, glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

        glUniform1f(timer, (sin(time * 50.0f)+1.0f)/2.0f);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        if (SDL_PollEvent(&windowEvent)) {
            if (windowEvent.type == SDL_QUIT ||
                (windowEvent.type == SDL_KEYUP &&
                windowEvent.key.keysym.sym == SDLK_ESCAPE))
                    break;
        }
        SDL_GL_SwapWindow(window.getWindow());
    }
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArrayObject);
    return 0;
}
