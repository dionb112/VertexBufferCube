#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <MyMatrix3.h>

static const float VALUE = 0.5f;

using namespace std;
using namespace sf;

class Game
{
public:
	Game();
	~Game();
	void run();
private:
	MyVector3D m_vertices[8];
	MyVector3D m_colors[8];
	MyVector3D m_centre;
	unsigned int m_vertexIndex[36] = {
		0,1,2,  0,2,3,
		4,5,6,  4,6,7,
		4,0,3,  4,3,7,
		1,0,4,  1,4,5,
		1,5,6,  1,6,2,
		2,3,7,  2,7,6
	};

	Window window;
	bool isRunning = false;
	void initialize();
	void update();
	void render();
	void unload();

	Clock clock;
	Time elapsed;

	float rotationAngle = 0.0f;
};