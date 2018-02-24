#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Tim Ascencio - ta3755@g.rit.edu";
	
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}
	
	//if there are no segments create 7
	if(m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits
	// fRadius;

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
	//prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
	/*
		This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
	*/
	uint uSides = 3; //start with the minimal 3 sides
	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength

		/////////////////////////////////////////////////////////////////////////////////////

		static float currentX = fSize; // fSize = orbit radius
		static float currentY = 0;

		float step = ((float)TWOPI / i); // angle formula 2Pi divided by i = orbit subdivisions
		float currentAngle = step;

		float newX = (float)cos(currentAngle); // cosine of angle
		float newY = (float)sin(currentAngle); // sin of angle

		newX = newX * fSize;
		newY = newY * fSize;

		vector3 currentPoint = vector3(currentX, currentY, 0); // generate x y z coord
		stops_list.push_back(currentPoint); // push back x y z coord into array

		/////////////////////////////////////////////////////////////////////////////////////
	}
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
	matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
	/*
		The following offset will orient the orbits as in the demo, start without it to make your life easier.
	*/
	//m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);

	// draw a shapes
	for (uint i = 0; i < m_uOrbits; ++i)
	{
		// draw orbits
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));

		////////////////////////////////////////////////

		static uint positionCount = 0; // retains current point in array (starting point)

		uint startPoint = positionCount; // current point in array (starting point)
		uint endPoint;

		if (positionCount < stops_list.size() - 1)
			endPoint = positionCount + 1; // the following point in array (goal | end point)
		else
			endPoint = 0;

		vector3 startCoord = stops_list[startPoint]; // starting coordinates for lerp
		vector3 endCoord = stops_list[endPoint]; // goal and/or ending coordinates for lerp

		vector3 time = vector3(1.0f); // time percent - manipulate speed of object

		if (positionCount >= stops_list.size()) {
			positionCount = 0;
		}

		// calculate the current position
		vector3 v3CurrentPos = vector3(startCoord + time * (endCoord - startCoord)); // push movement into array

		////////////////////////////////////////////////

		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);
		
		// draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

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
	//release GUI
	ShutdownGUI();
}