#include "AppClass.h"
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//init the mesh
	m_pMesh = new MyMesh();
	m_pMesh->GenerateDiamond(3, 0.5f, 0.5f, C_GREEN);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	matrix4 m4Model;

	//Get a timer
	static float fTimer = 0;
	static uint uClock = m_pSystem->GenClock();
	fTimer += m_pSystem->GetDeltaTime(uClock);

#pragma region Calculate Orbit Stops

	float fRadius = shapeCount * 0.3f;
	
	if (fTimer > 1) {
		shapeCount++; // increment shapes on screen
		m_pMesh->GenerateDiamond(shapeCount, 0.5f, 0.5f, C_GREEN);
		fTimer = m_pSystem->GetDeltaTime(uClock); // restart clock
	}

	static float currentX = (fRadius); // fSize is approximately orbit radius
	static float currentY = 0;

	float step = ((float)TWOPI / shapeCount); 
	float currentAngle = step;

	std::vector<vector3> currentOrbitList; // array containing a set of coordinates for a specific orbit

	// Within the current orbit being generated, prepare to fill vector<vector<vector3>> stops_list
	for (int j = 0; j < shapeCount; j++) {

		float newX = (float)cos(currentAngle); // cosine of angle
		float newY = (float)sin(currentAngle); // sin of angle

		newX = newX * (fRadius);
		newY = newY * (fRadius);

		currentX = newX;
		currentY = newY;

		currentAngle += step;

		vector3 currentPoint = vector3(currentX, currentY, 0); // generate x y z coord
		currentOrbitList.push_back(currentPoint); // push back x y z coord into array
	}

	stops_list.push_back(currentOrbitList); // array containing arrays of coordinates for different orbits

	fRadius += 0.5f;

#pragma endregion 
	
	// shape position
	matrix4 m4Rotation = glm::rotate(IDENTITY_M4, fTimer, vector3(0.0f, 0.0f, 1.0f));
	m4Model = m4Rotation * glm::translate(IDENTITY_M4, vector3(0.0f, 0.0f, 0.0f)) * glm::transpose(m4Rotation);

	// rotate shape
	matrix4 m4Translation = glm::translate(IDENTITY_M4, vector3(0.0f, 0.0f, 0.0f));
	matrix4 m4TransInverse = glm::translate(IDENTITY_M4, vector3(0.0f, 0.0f, 0.0f));
	m4Model = m4TransInverse * m4Rotation * m4Translation;

	// render the shapes in different positions
	for (int i = 0; i < shapeCount; i++)
		m_pMesh->Render(m4Projection, m4View, m4Model * glm::translate(currentOrbitList[i]));

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//draw the center of the world
	//m_pMeshMngr->AddSphereToRenderList(glm::scale(vector3(0.1f)), C_RED, RENDER_WIRE);

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the mesh
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}
