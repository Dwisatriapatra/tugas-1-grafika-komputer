#include "Demo.h"
Demo::Demo() {
}
Demo::~Demo() {
}
void Demo::Init() {
	// build dan complie shader
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);
	BuildColoredCube();
	BuildColoredPlane();
}
void Demo::DeInit() {
	// Alokasi kembali resource
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}
// Memproses inputan GLFW
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
void Demo::Update(double deltaTime) {
}
void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	// Pass perspective
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	// Kamera untuk melihat objek
	glm::mat4 view = glm::lookAt(glm::vec3(0, 3, 8), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	DrawColoredCube();
	DrawColoredPlane();
	glDisable(GL_DEPTH_TEST);
}
void Demo::BuildColoredCube() {
	// Membuat texture dan meload gambar ke texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("kotak.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Set up dan konfugurasi vertex
	float vertices[] = {
		// Pengaturan posisi
		// depan
		-0.5, -0.5, 0.5, 0, 0,  
		0.5, -0.5, 0.5, 1, 0,   
		0.5,  0.5, 0.5, 1, 1,   
		-0.5,  0.5, 0.5, 0, 1,  
		// kanan
		0.5,  0.5,  0.5, 0, 0,  
		0.5,  0.5, -0.5, 1, 0,  
		0.5, -0.5, -0.5, 1, 1,  
		0.5, -0.5,  0.5, 0, 1,  
		// belakang
		-0.5, -0.5, -0.5, 0, 0,  
		0.5,  -0.5, -0.5, 1, 0, 
		0.5,   0.5, -0.5, 1, 1, 
		-0.5,  0.5, -0.5, 0, 1, 
		// kiri
		-0.5, -0.5, -0.5, 0, 0, 
		-0.5, -0.5,  0.5, 1, 0, 
		-0.5,  0.5,  0.5, 1, 1, 
		-0.5,  0.5, -0.5, 0, 1, 
		// atas
		0.5, 0.5,  0.5, 0, 0,   
		-0.5, 0.5,  0.5, 1, 0,  
		-0.5, 0.5, -0.5, 1, 1,  
		0.5, 0.5, -0.5, 0, 1,   
		// bawah
		-0.5, -0.5, -0.5, 0, 0, 
		0.5, -0.5, -0.5, 1, 0,  
		0.5, -0.5,  0.5, 1, 1,  
		-0.5, -0.5,  0.5, 0, 1, 
	};
	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // depan
		4,  5,  6,  4,  6,  7,   // kanan
		8,  9,  10, 8,  10, 11,  // belakang
		12, 14, 13, 12, 15, 14,  // kiri
		16, 18, 17, 16, 19, 18,  // atas
		20, 22, 21, 20, 23, 22   // bawah
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// binding vertex array
	glBindVertexArray(VAO);
	// Proses binding serta mengkonfigurasi vertex buffer beserta atributnya
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Mendefinisika vertex pointer ke layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	// Mendefinisikan texcoord pointer lyout 1
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Demo::DrawColoredCube()
{
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// load dan membuat texture
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image = SOIL_load_image("lantai.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// membuild geometri
	GLfloat vertices[] = {
		// pengaturan posisi
		// bawah
		-30.0, -0.5, -30.0,  0,  0,
		 30.0, -0.5, -30.0, 30,  0,
		 30.0, -0.5,  30.0, 30, 30,
		-30.0, -0.5,  30.0,  0, 30,
	};
	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Atribut Posisi
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Atribut texCoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}
void Demo::DrawColoredPlane()
{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 1);
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}
int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Dwi Satria Patra", 800, 600, false, false);
}