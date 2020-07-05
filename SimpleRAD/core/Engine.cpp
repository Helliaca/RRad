#include "Engine.h"

#include "assets/Texture.h"

#include "assets/Shader.h"

Engine::Engine() {

}

Engine::~Engine() {

}

void loadGlfw() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void loadGlad() { //only needs to be called once.
	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

GLFWwindow* createWindow() {
    GLFWwindow* window = glfwCreateWindow(800, 600, "RRAD", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

void make_posTex(GLFWwindow* window, unsigned int VAO, Texture* tex, int size) {
    tex->clear();

    Shader* shad = new Shader("shaders/store.vs", "shaders/store_worldpos.fs");
    shad->use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Settings
    glViewport(0, 0, 128, 128);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glDisable(GL_CULL_FACE);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_BLEND);

    //Texture/Map
    tex->use(shad->ID, "tex2D", 0);
    glBindImageTexture(0, tex->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

    //Render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);

    //Revert Settings
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    glViewport(0, 0, 800, 600);
}

void make_nrmTex(GLFWwindow* window, unsigned int VAO, Texture* tex, int size) {
    tex->clear();

    Shader* shad = new Shader("shaders/store.vs", "shaders/store_nrm.fs");
    shad->use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Settings
    glViewport(0, 0, 128, 128);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glDisable(GL_CULL_FACE);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_BLEND);

    //Texture/Map
    tex->use(shad->ID, "tex2D", 0);
    glBindImageTexture(0, tex->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

    //Render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);

    //Revert Settings
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    glViewport(0, 0, 800, 600);
}

void make_ligTex(GLFWwindow* window, unsigned int VAO, Texture* tex, int size, Texture* posTex, Texture* nrmTex, Texture* old_ligTex, bool init) {
    tex->clear();

    Shader* shad = new Shader("shaders/make_lightmap.vs", "shaders/make_lightmap.fs");
    shad->use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Settings
    glViewport(0, 0, 128, 128);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glDisable(GL_CULL_FACE);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_BLEND);

    //Texture/Map
    shad->setBool("init", init);
    tex->use(shad->ID, "tex2D", 0);
    glBindImageTexture(0, tex->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
    posTex->use(shad->ID, "posTex", 1);
    nrmTex->use(shad->ID, "nrmTex", 2);
    old_ligTex->use(shad->ID, "ligTex", 3);

    //Render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);

    //Revert Settings
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    glViewport(0, 0, 800, 600);
}

void Engine::run() {
    loadGlfw();
    GLFWwindow* window = createWindow();
    loadGlad();

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("shaders/showTexture.vs", "shaders/showTexture.fs");
    //Shader ourShader("shaders/emit.vs", "shaders/emit.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // normal vector     // texture coords
        // Left Wall
        -1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f, // 0   i:0
        -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,   0.1f, 0.0f, // 4   i:1
        -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,   0.1f, 0.1f, // 7   i:2
        -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f,  0.0f,  0.0f,   0.0f, 0.1f, // 3   i:3
        // Right Wall
         1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.1f, // 1   i:4
         1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   0.1f, 0.1f, // 5   i:5
         1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   0.1f, 0.2f, // 6   i:6
         1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.2f, // 2   i:7
         // Bottom Wall
        -1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   0.0f, 0.2f, // 0   i:8
         1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   0.1f, 0.2f, // 1   i:9
         1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   0.1f, 0.3f, // 5   i:10
        -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f,  1.0f,  0.0f,   0.0f, 0.3f, // 4   i:11
        // Top Wall
        -1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.3f, // 3   i:12
         1.0f,  1.0f, -1.0f,   1.0f, 1.0f, 1.0f,   0.0f, -1.0f,  0.0f,   0.1f, 0.3f, // 2   i:13
         1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f, -1.0f,  0.0f,   0.1f, 0.4f, // 6   i:14
        -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f, -1.0f,  0.0f,   0.0f, 0.4f, // 7   i:15
        // Back Wall
        -1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.4f, // 4   i:16
         1.0f, -1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f, -1.0f,   0.1f, 0.4f, // 5   i:17
         1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f, -1.0f,   0.1f, 0.5f, // 6   i:18
        -1.0f,  1.0f,  1.0f,   1.0f, 1.0f, 1.0f,   0.0f,  0.0f, -1.0f,   0.0f, 0.5f, // 7   i:19
    };
    unsigned int indices[] = {
        // Left Wall
        0, 1, 2,
        0, 2, 3,
        // Right Wall
        4, 5, 6,
        4, 6, 7,
        // Bottom
        8, 9, 10,
        8, 10, 11,
        // Top
        12, 13, 14,
        12, 14, 15,
        // Back
        16, 17, 18,
        16, 18, 19,
    };

    glDisable(GL_CULL_FACE);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // nrm attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);

    Texture* posTex = new Texture();
    make_posTex(window, VAO, posTex, (sizeof(indices) / sizeof(*indices)));

    Texture* nrmTex = new Texture();
    make_nrmTex(window, VAO, nrmTex, (sizeof(indices) / sizeof(*indices)));

    Texture* ligTex0 = new Texture();
    Texture* ligTex1 = new Texture();
    make_ligTex(window, VAO, ligTex0, (sizeof(indices) / sizeof(*indices)), posTex, nrmTex, ligTex1, true);
    make_ligTex(window, VAO, ligTex1, (sizeof(indices) / sizeof(*indices)), posTex, nrmTex, ligTex0, false);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render container
        ourShader.use();

        //tex->use(ourShader.ID, "tex2D", 0);
        //glBindImageTexture(0, tex->textureID, 0, GL_TRUE, 0, GL_READ_ONLY, GL_RGBA8);
        glBindTexture(GL_TEXTURE_2D, ligTex1->textureID);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (sizeof(indices) / sizeof(*indices)), GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return;
}