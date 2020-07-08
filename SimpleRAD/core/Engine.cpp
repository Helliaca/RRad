#include "Engine.h"

#include "assets/Texture.h"

#include "assets/Shader.h"

#include "assets/Data.h"

using namespace scene;

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
    GLFWwindow* window = glfwCreateWindow(WIN_SIZE_X, WIN_SIZE_Y, "RRAD", NULL, NULL);
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
    glViewport(0, 0, MAP_RES, MAP_RES);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
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
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    glViewport(0, 0, WIN_SIZE_X, WIN_SIZE_Y);
}

void make_nrmTex(GLFWwindow* window, unsigned int VAO, Texture* tex, int size) {
    tex->clear();

    Shader* shad = new Shader("shaders/store.vs", "shaders/store_nrm.fs");
    shad->use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Settings
    glViewport(0, 0, MAP_RES, MAP_RES);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
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
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    glViewport(0, 0, WIN_SIZE_X, WIN_SIZE_Y);
}

void make_arfTex(GLFWwindow* window, unsigned int VAO, Texture* tex, int size) {
    tex->clear();

    Shader* shad = new Shader("shaders/store.vs", "shaders/store_arf.fs");
    shad->use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Settings
    glViewport(0, 0, MAP_RES, MAP_RES);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
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
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    glViewport(0, 0, WIN_SIZE_X, WIN_SIZE_Y);
}

void make_ligTex(GLFWwindow* window, unsigned int VAO, Texture* tex, int size, Texture* posTex, Texture* nrmTex, Texture* arfTex, Texture* old_ligTex, int pass) {
    tex->clear();

    Shader* shad = new Shader("shaders/make_lightmap.vs", "shaders/make_lightmap.fs");
    shad->use();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Settings
    glViewport(0, 0, MAP_RES, MAP_RES);
    //glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    //glDisable(GL_BLEND);

    //Texture/Map
    shad->setInt("pass", pass);
    tex->use(shad->ID, "tex2D", 0);
    glBindImageTexture(0, tex->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
    posTex->use(shad->ID, "posTex", 1);
    nrmTex->use(shad->ID, "nrmTex", 2);
    arfTex->use(shad->ID, "arfTex", 3);
    old_ligTex->use(shad->ID, "ligTex", 4);

    //Render
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);

    //Revert Settings
    //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    glViewport(0, 0, WIN_SIZE_X, WIN_SIZE_Y);

    std::cout << "Pass " << pass << " completed." << std::endl;
}

bool renderMode_texture = false;
bool phong = false;
bool pass_onnextframe = false;
std::string currentTexture = "ligTex0";
int pass_c = 0;

void Engine::console()
{
    std::string in;
    while (true)
    {
        std::getline(std::cin, in);
        std::istringstream iss(in);
        std::vector<std::string> input(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());

        settingMutex.lock();

        if (input.size() == 1) {
            if (input[0] == "tex" || input[0] == "obj" || input[0] == "mode") renderMode_texture = !renderMode_texture;
            else if (input[0] == "pass") pass_onnextframe = true;
            else if (input[0] == "phong") phong = !phong;
        }
        else if (input.size() == 2) {
            if (input[0] == "tex") currentTexture = input[1];
        }

        settingMutex.unlock();
    }
}

void Engine::run() {
    loadGlfw();
    GLFWwindow* window = createWindow();
    loadGlad();

    ObjImporter* imp = new ObjImporter();
    imp->import_from_file("simpelRad.fbx");

    // build and compile our shader zprogram
    // ------------------------------------
    Shader* texShader = new Shader("shaders/showTexture.vs", "shaders/showTexture.fs");
    Shader* objShader = new Shader("shaders/emit.vs", "shaders/emit.fs");
    Shader* phgShader = new Shader("shaders/emit.vs", "shaders/emit_phong.fs");
    Shader* currentShader = objShader;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * imp->vertexData.size(), &imp->vertexData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * imp->indices.size(), &imp->indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // nrm attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // texture coord attribute
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
    // area factor attribute
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(4);

    int size = imp->indices.size();

    Texture* posTex = new Texture();
    make_posTex(window, VAO, posTex, size);

    Texture* nrmTex = new Texture();
    make_nrmTex(window, VAO, nrmTex, size);

    Texture* arfTex = new Texture();
    make_arfTex(window, VAO, arfTex, size);


    Texture* ligTex0 = new Texture();
    Texture* ligTex1 = new Texture();
    make_ligTex(window, VAO, ligTex0, size, posTex, nrmTex, arfTex, ligTex1, 0);
    //make_ligTex(window, VAO, ligTex1, size, posTex, nrmTex, ligTex0, 1);
    //make_ligTex(window, VAO, ligTex0, size, posTex, nrmTex, ligTex1, 2);

    consoleThread = std::thread(&Engine::console, this);
    consoleThread.detach();


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        settingMutex.lock();

        if (pass_onnextframe) {
            pass_c++;
            if (pass_c % 2 != 0) {
                make_ligTex(window, VAO, ligTex1, size, posTex, nrmTex, arfTex, ligTex0, pass_c);
                if (currentTexture == "ligTex0") currentTexture = "ligTex1";
            }
            else {
                make_ligTex(window, VAO, ligTex0, size, posTex, nrmTex, arfTex, ligTex1, pass_c);
                if (currentTexture == "ligTex1") currentTexture = "ligTex0";
            }
            pass_onnextframe = false;
        }

        // render
        // ------
        glClearColor(0.1f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        // render container
        if (phong) {
            currentShader = phgShader;
        }
        else if(renderMode_texture) {
            currentShader = texShader;
        }
        else {
            currentShader = objShader;
        }

        currentShader->use();

        if(currentTexture=="ligTex1") ligTex1->use(currentShader->ID, "tex2D", 0);
        else if (currentTexture == "posTex") posTex->use(currentShader->ID, "tex2D", 0);
        else if (currentTexture == "nrmTex") nrmTex->use(currentShader->ID, "tex2D", 0);
        else if (currentTexture == "arfTex") arfTex->use(currentShader->ID, "tex2D", 0);
        else ligTex0->use(currentShader->ID, "tex2D", 0);
        

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

        settingMutex.unlock();

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