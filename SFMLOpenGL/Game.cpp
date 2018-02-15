#include <Game.h>

static bool flip;

Game::Game() : window(VideoMode(800, 600), "OpenGL Cube VBO")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning) {

		cout << "Game running..." << endl;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[3];
} VertexBufferTime;


VertexBufferTime vertex[36];
GLubyte triangles[36];

/* Variable to hold the VBO identifier */
GLuint vbo[1];
GLuint index;

void Game::initialize()
{
	isRunning = true;

	glewInit();

	/* Vertices counter-clockwise winding */
	m_vertices[0] = { 0.5f, 0.5f, -0.5f };
	m_vertices[1] = { -0.5f, 0.5f, -0.5f };
	m_vertices[2] = { -0.5f, -0.5f, -0.5f };
	m_vertices[3] = { 0.5f, -0.5f, -0.5f };
	m_vertices[4] = { 0.5f, 0.5f, 0.5f };
	m_vertices[5] = { -0.5f, 0.5f, 0.5f };
	m_vertices[6] = { -0.5f, -0.5f, 0.5f };
	m_vertices[7] = { 0.5f, -0.5f, 0.5f };

	m_colors[0] = { 0.0f, 0.1f, 0.0f };
	m_colors[1] = { 0.1f, 0.0f, 0.0f };
	m_colors[2] = { 0.1f, 0.1f, 0.0f };
	m_colors[3] = { 0.0f, 0.0f, 0.0f };
	m_colors[4] = { 0.0f, 0.1f, 0.0f };
	m_colors[5] = { 0.1f, 0.0f, 0.0f };
	m_colors[6] = { 0.1f, 0.1f, 0.0f };
	m_colors[7] = { 0.0f, 0.1f, 0.0f };

	// get centre of whole cube at once !
	for (int i = 0; i < 8; i++)
	{
		m_centre += m_vertices[i];
	}
	m_centre = m_centre / 8;

	for (int j = 0; j < 36; j++)
	{
		vertex[j].coordinate[0] = m_vertices[m_vertexIndex[j]].X();
		vertex[j].coordinate[1] = m_vertices[m_vertexIndex[j]].Y();
		vertex[j].coordinate[2] = m_vertices[m_vertexIndex[j]].Z();

		vertex[j].color[0] = 0.4f;
		vertex[j].color[1] = 0.2f;
		vertex[j].color[2] = 0.0f;


		triangles[j] = j;
	}

	//Front Face
	/*vertex[0].coordinate[0] = VALUE;
	vertex[0].coordinate[1] = VALUE;
	vertex[0].coordinate[2] = -VALUE;

	vertex[1].coordinate[0] = -VALUE;
	vertex[1].coordinate[1] = VALUE;
	vertex[1].coordinate[2] = -VALUE;

	vertex[2].coordinate[0] = -VALUE;
	vertex[2].coordinate[1] = -VALUE;
	vertex[2].coordinate[2] = -VALUE;

	vertex[3].coordinate[0] = VALUE;
	vertex[3].coordinate[1] = VALUE;
	vertex[3].coordinate[2] = -VALUE;

	vertex[4].coordinate[0] = -VALUE;
	vertex[4].coordinate[1] = -VALUE;
	vertex[4].coordinate[2] = -VALUE;

	vertex[5].coordinate[0] = VALUE;
	vertex[5].coordinate[1] = -VALUE;
	vertex[5].coordinate[2] = -VALUE;*/

	//vertex[0].color[0] = 0.1f;
	//vertex[0].color[1] = 1.0f;
	//vertex[0].color[2] = 0.0f;

	//vertex[1].color[0] = 0.2f;
	//vertex[1].color[1] = 1.0f;
	//vertex[1].color[2] = 0.0f;

	//vertex[2].color[0] = 0.3f;
	//vertex[2].color[1] = 1.0f;
	//vertex[2].color[2] = 0.0f;

	//vertex[3].color[0] = 0.4f;
	//vertex[3].color[1] = 1.0f;
	//vertex[3].color[2] = 0.0f;

	//vertex[4].color[0] = 0.5f;
	//vertex[4].color[1] = 1.0f;
	//vertex[4].color[2] = 0.0f;

	//vertex[5].color[0] = 0.6f;
	//vertex[5].color[1] = 1.0f;
	//vertex[5].color[2] = 0.0f;




	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferTime) * 36, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Game::update()
{
	elapsed = clock.getElapsedTime();

	if (elapsed.asSeconds() >= 1.0f)
	{
		clock.restart();

		if (!flip)
		{
			flip = true;
		}
		else
			flip = false;
	}

	if (flip)
	{
		rotationAngle += 0.005f;

		if (rotationAngle > 360.0f)
		{
			rotationAngle -= 360.0f;
		}
	}

	for (int j = 0; j < 36; j++)
	{
		MyVector3D vector{ vertex[j].coordinate[0], vertex[j].coordinate[1], vertex[j].coordinate[2] };

		// move to origin of shape
		vector = MyMatrix3::translation(-m_centre) * vector;

		// rotate
		vector = MyMatrix3::rotationZ(0.001) * vector;
		// vector = MyMatrix3::rotationX(0.001) * vector;
		vector = MyMatrix3::rotationY(0.001) * vector;

		// move back
		vector = MyMatrix3::translation(m_centre) * vector;

		// scale
		double scalingFactor(1);
		MyMatrix3 standardScale = MyMatrix3::scale(scalingFactor);
		vector = standardScale * vector;

		vertex[j].coordinate[0] = vector.X();
		vertex[j].coordinate[1] = vector.Y();
		vertex[j].coordinate[2] = vector.Z();
	}
	cout << "Update up" << endl;
}

void Game::render()
{
	cout << "Drawing" << endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferTime) * 36, vertex, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);


	glColorPointer(3, GL_FLOAT, sizeof(VertexBufferTime), (char*)NULL + 12);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain 
		model compoents that are and are not to be drawn )	*/
	glVertexPointer(3, GL_FLOAT, sizeof(VertexBufferTime), (char*)NULL + 0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	window.display();

}

void Game::unload()
{
	cout << "Cleaning up" << endl;

	glDeleteBuffers(1, vbo);
}

