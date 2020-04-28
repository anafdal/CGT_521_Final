#include<GL/glew.h>

#include <glfw3.h>
#include <iostream>//writing
#include <fstream>//for streaming
#include <string>
#include <vector>

#include <sstream>
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"
#include "Texture.h"
#include "camera.h"

#include "stb_image.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Our state
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
unsigned int FBO_ID = -1;

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 580;
static bool reverse = false;
static int cubemap = 1;//chose between panorama
static int scene = 1;//chose between effects
static float a = 0.2126f;//inversion
static float b = 0.7152f;
static float limit = 1.0f;//black & white
static float col[3] = { 1.0f, 1.0f, 1.0f };///colorEdit
static int choice = 1;//chose between color edits
static float offset_test = 0.003f;//post-effects
static float offset_kernel = 0.003f;
static float offset_edgeD = 0.003f;
static float offset_blurry = 0.003f;
static float k = 9.0f;
static float n = 1.0f;
static float e = 8.0f;
static float d = 1.0f;
static float bl = 16.0f;
static float o = 1.0f;
static float t = 2.0f;
static float m = 8.0f;

//camera set up
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT /2.0;


//macros for debug 
#define ASSERT(x) if(!(x))__debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))

static void GLClearError() {//modified from The Cherno

	while (glGetError() != GL_NO_ERROR);

}
static bool  GLLogCall(const char*function,const char*file,int line) {//check for errors
	
	while (GLenum error=glGetError()) {

		std::cout << "[OpenGL_Error] (" << error << "):"<<function<<" "<<file<<": "<<line << std::endl;
		return false;
	}
	return true;
}

void reset_settings()//reset everything back to original values
{
	choice = 1;
	scene = 1;
	a = 0.2126f;
	b = 0.7152f;
	limit = 1.0f;
	offset_test = 0.003f;
	offset_kernel = 0.003f;
	offset_edgeD = 0.003f;
	offset_blurry = 0.003f;
	k = 9.0f;
	n = 1.0f;
	e = 8.0f;
	d = 1.0f;
	bl = 16.0f;
	o = 1.0f;
	t = 2.0f;
	m = 8.0f;

	col[0] = 1.0f;///colorEdit
	col[1] = 1.0f;
	col[2] = 1.0f;
}

void draw_gui() {
	 // Start the Dear ImGui frame
	ImGui_ImplGlfwGL3_NewFrame();

	Shader shader("shaders/vshader.glsl", "shaders/fshader.glsl");
	shader.Bind();

	ImGui::Begin("Post-Effects");                          
	
	// Display some text 
	ImGui::Text("Choose image you want to work with.");
	

	ImGui::RadioButton("Cubemap 1", &cubemap, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Cubemap 2", &cubemap, 2);

	ImGui::Text("  ");
	if (ImGui::Button("Reset All Settings"))//go back as before
	{
		reset_settings();
	}
	ImGui::Text("  ");
	ImGui::Text("  ");

	// Display reverse perspective
	ImGui::Checkbox("Show Reverse Perspective.", &reverse);///////////////////////////////////////////////////
	ImGui::Text("  ");
	ImGui::Text("  ");

	ImGui::Text("Edit colors.");// Display some text 
	ImGui::RadioButton("Normal  ", &choice, 1);
	ImGui::RadioButton("Inverted View", &choice, 2);
	ImGui::SliderFloat("Light", &limit, 0.2f, 2.0f);


	ImGui::RadioButton("Black and White View", &choice, 3);
	ImGui::SliderFloat("Shade 1", &a, 0.0f, 1.0f);
	ImGui::SliderFloat("Shade 2", &b, 0.0f, 1.0f);

	ImGui::Text("Pick specific color here.");
	ImGui::ColorEdit3("Color", col);

	ImGui::Text("  ");
	ImGui::Text("  ");

	ImGui::Text("Choose post-effects below.");
	ImGui::Text("You can change the parameters for each effect as you want to.");

	ImGui::RadioButton("Normal View", &scene, 1);

	ImGui::RadioButton("Kernel Effect View 1", &scene, 2);
	ImGui::SliderFloat("Image offset 0", &offset_test, 0.003f, 1.0f);
	ImGui::SliderFloat("Brightness    ", &m, 1.0f, 10.0f);

	ImGui::RadioButton("Kernel Effect View 2", &scene, 3);
	ImGui::SliderFloat("Image offset 1", &offset_kernel, 0.003f, 1.0f);
	ImGui::SliderFloat("Brightness ", &k, 5.0f, 15.0f);
	ImGui::SliderFloat("Darken", &n, 1.0f,2.5f);

	ImGui::RadioButton("Edge Detection", &scene, 4);
	ImGui::SliderFloat("Image offset 2", &offset_edgeD, 0.003f, 1.0f);
	ImGui::SliderFloat("  Darken ", &e, 1.0f, 15.0f);
	ImGui::SliderFloat(" Brightness ", &d, 1.0f, 2.5f);

	ImGui::RadioButton("Blurry View", &scene, 5);
	ImGui::SliderFloat("Image offset 3", &offset_blurry, 0.003f, 1.0f);
	ImGui::SliderFloat(" Darken", &bl, 5.0f, 30.0f);
	ImGui::SliderFloat(" Brightness 1 ", &o, 1.0f, 5.0f);
	ImGui::SliderFloat(" Brightness 2 ", &t, 1.0f, 10.0f);


	ImGui::Text("  ");
	ImGui::Text("  ");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

}

void draw_pass_1(int skyboxVAO, int textureID)//cubemap
{
	const int pass = 1;

	Shader shader("shaders/vshader.glsl", "shaders/fshader.glsl");
	shader.Bind();


	//PMV
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

	///uniforms
	shader.SetUniform1i("pass", pass);
	shader.SetUniformMatrix4fv("view", view);
	shader.SetUniformMatrix4fv("projection", projection);
	shader.SetUniformMatrix4fv("model", model);

	shader.SetUniform1i("choice", choice);
	shader.SetUniform1f("a", a);
	shader.SetUniform1f("b", b);

	shader.SetUniform1f("sub", limit);
	shader.SetUniform4f("Color", col[0], col[1], col[2], 1.0f);
	

	//draw cubemap here
	glBindVertexArray(skyboxVAO);
	GLCall(glActiveTexture(GL_TEXTURE0));
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_BACK);

}

void draw_pass2(int quadVAO,int textureColorbuffer)//quadrilateral
{
	const int pass = 2;
	Shader shader2("shaders/vshader2.glsl", "shaders/fshader2.glsl");
	shader2.Bind();

	///uniforms
	shader2.SetUniform1i("pass", pass);
	//shader2.SetUniform4f("Color", col[0], col[1], col[2], 1.0f);

	shader2.SetUniform1i("scene", scene);//effects

	shader2.SetUniform1f("offset0", offset_test);//test
	shader2.SetUniform1f("m", m);

	shader2.SetUniform1f("offset1", offset_kernel);//kernel
	shader2.SetUniform1f("k", k);
	shader2.SetUniform1f("n", n);

	shader2.SetUniform1f("offset2", offset_edgeD);//edge detection
	shader2.SetUniform1f("e", e);
	shader2.SetUniform1f("d", d);

	shader2.SetUniform1f("offset3", offset_blurry);//blurry
	shader2.SetUniform1f("bl", bl);
	shader2.SetUniform1f("o", o);
	shader2.SetUniform1f("t", t);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
	// clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);

}
unsigned int create_Cubemap(std::vector<std::string> faces1) {///load in cubemap +Framebuffer 2
	

		unsigned int textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		int width, height, nrChannels;
		unsigned char* data;

		for (GLuint i = 0; i < faces1.size(); i++)
		{
			data = stbi_load(faces1[i].c_str(), &width, &height, &nrChannels, 0);
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		//Create renderbuffer for depth
		unsigned int Depthbuffer_ID;
		glGenRenderbuffers(1, &Depthbuffer_ID);
		glBindRenderbuffer(GL_RENDERBUFFER, Depthbuffer_ID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);

		//framebuffer
		glGenFramebuffers(1, &FBO_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
		//attach cube faces to the FBO color attachments
		for (int i = 0; i < 6; i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureID, 0);
		}
		//attach depth renderbuffer to FBO
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, Depthbuffer_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
		for (int i = 0; i < 6; i++)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);
		}


		return textureID;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	

	 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//opengl version; need vao
	 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	 glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);



	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Window", NULL, NULL);//create with this in the title
	if (!window)//if you can't terminate 
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);//needs a valid context to get glew
	glfwSwapInterval(1);//get smoother animation

	if (glewInit() != GLEW_OK) {///if glew works
		std::cout << "Error!" << std::endl;
	}
	else
		std::cout << glGetString(GL_VERSION) << std::endl;//tells g l version

    /////////////////////////////////////////////////////////////////////////////////////////////SHADERS
	  glEnable(GL_DEPTH_TEST);
	  Shader shader("shaders/vshader.glsl", "shaders/fshader.glsl");
	  shader.Bind();

	  Shader shader2("shaders/vshader2.glsl", "shaders/fshader2.glsl");
	  shader2.Bind();
	
	  Shader shader3("shaders/cubevshader.glsl", "shaders/cubefshader.glsl");
	  shader3.Bind();
	  { //////////////////////////////////////////////////////////////////////////////////////////////skybox+Framebyffer 2
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		unsigned int skyboxVAO, vbo;
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &vbo);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// load textures
		std::vector<std::string> faces1
			{
				"Cubemap1/right.jpg",
				"Cubemap1/left.jpg",
				"Cubemap1/top.jpg",
				"Cubemap1/bottom.jpg",
				"Cubemap1/front.jpg",
				"Cubemap1/back.jpg",
			};
			std::vector<std::string> faces2
			{
				"Cubemap2/px.jpg",
				"Cubemap2/nx.jpg",
				"Cubemap2/py.jpg",
				"Cubemap2/ny.jpg",
				"Cubemap2/pz.jpg",
				"Cubemap2/nz.jpg",
			};
		
	
	
	   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////cubemap	
				unsigned int textureID1 = create_Cubemap(faces1);	
				unsigned int textureID2 = create_Cubemap(faces2);
				unsigned int textureID= create_Cubemap(faces1);

       ////////////////////////////////////////////////////////////////////////////////////////////////inverted cube
				float cubeVertices[] = {
		// position          // normal
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,//back
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,//front
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,//right
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,


		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,//left
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,


		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,//bottom
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,//top
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
				};

				// cube
				unsigned int cubeVAO, cubeVBO;
				glGenVertexArrays(1, &cubeVAO);
				glGenBuffers(1, &cubeVBO);
				glBindVertexArray(cubeVAO);
				glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));


				glBindVertexArray(0);
				
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////Framebuffer 1
	  float quadVertices[] = { // vertex attributes 
				  // positions   // texCoords
				  -1.0f,  1.0f,  0.0f, 1.0f,
				  -1.0f, -1.0f,  0.0f, 0.0f,
				   1.0f, -1.0f,  1.0f, 0.0f,

				  -1.0f,  1.0f,  0.0f, 1.0f,
				   1.0f, -1.0f,  1.0f, 0.0f,
				   1.0f,  1.0f,  1.0f, 1.0f
				};
	  // screen quad VAO
	  unsigned int quadVAO, quadVBO;
	  glGenVertexArrays(1, &quadVAO);
	  glGenBuffers(1, &quadVBO);
	  glBindVertexArray(quadVAO);
	  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	  glEnableVertexAttribArray(0);
	  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	  glEnableVertexAttribArray(1);
	  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	  //framebuffer configuration
      unsigned int framebuffer;
	  glGenFramebuffers(1, &framebuffer);
	  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	  // create a color attachment texture
	  unsigned int textureColorbuffer;
	  glGenTextures(1, &textureColorbuffer);
	  glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	  // create a renderbuffer object 
	  unsigned int rbo;
	  glGenRenderbuffers(1, &rbo);
	  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); 
	  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		  std::cout << "Error!" << std::endl;
	  glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
		/////////////////////////////////////////////////////////////////////////////////////////////////////////create imgui context
		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		/////////////////////////////////////////////////////////////////////////////////////////////////////////unbind everything

		glBindVertexArray(0);
		shader.Unbind();
		shader2.Unbind();
		shader3.Unbind();

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))//////////////////////////////////////////////////////////////////////////Drawing
		{
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			if (cubemap == 1) {
				draw_pass_1(skyboxVAO, textureID1);//draw cubemap 1

				textureID = textureID1;
			}
			else if (cubemap == 2) {
				draw_pass_1(skyboxVAO, textureID2);//draw cubemap 2

				textureID = textureID2;
			}
		
			////////////////////////////////////////////////////////



			draw_pass2(quadVAO, textureColorbuffer);//draw quad

			if (reverse == true) {
				glDepthMask(GL_FALSE);
				shader3.Bind();

				//reversed cube
				glm::mat4 model = glm::mat4(1.0f);
				glm::mat4 view = camera.GetViewMatrix();
				glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


				shader3.SetUniformMatrix4fv("m", model);
				shader3.SetUniformMatrix4fv("v", view);
				shader3.SetUniformMatrix4fv("p", projection);
				shader3.SetUniform3fv("cameraPos", camera.Position);

				glBindVertexArray(cubeVAO);
				//glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindVertexArray(0);
				glDepthMask(GL_TRUE);
			}
		
         

			draw_gui();
			
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		shader.Unbind();//clean up
	}//make scope here to make it easier to close out of

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}


