#include "AppClass.h"
using namespace Simplex;
void Application::Display(void)
{
#pragma region DOES NOT NEED CHANGES
	// Clear the screen
	ClearScreen();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer
#pragma endregion
	//Your code goes here ---------------------------------------------------------
	
	m_sProgrammer = "Tim Ascencio - ta3755@rit.edu"; //Replace with your name and email

	vector3 v3Start; //start point
	vector3 v3End; //end point
	static uint route = 0; //current route
	v3Start = m_v3StopList[route]; //start at the current route
	v3End = m_v3StopList[(route + 1) % m_v3StopList.size()]; //end at route +1 (if overboard will restart from 0)

	//get the percentage
	float fTimeBetweenStops = 2.0;//in seconds
	
	//map the value to be between 0.0 and 1.0
	float fPercentage = MapValue(fTimer, 0.0f, fTimeBetweenStops, 0.0f, 1.0f);

	//calculate the current position
	vector3 v3CurrentPos = glm::lerp(v3Start, v3End, fPercentage);
	matrix4 m4Model = glm::translate(IDENTITY_M4, v3CurrentPos) * ToMatrix4(m_qArcBall);

	//if we are done with this route
	if (fPercentage >= 1.0f)
	{
		route++; //go to the next route
		fTimer = m_pSystem->GetDeltaTime(uClock);//restart the clock
		route %= m_v3StopList.size();//make sure we are within boundaries
	}
	
	//---------------------------------------------------------
#pragma region DOES NOT NEED CHANGES
	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix

	// render the object
	m_pMesh->Render(m4Projection, m4View, m4Model);

	// draw stops to know we are within stops
	for (uint i = 0; i < m_v3StopList.size(); ++i)
	{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_v3StopList[i]) * glm::scale(vector3(0.10f)), C_RED);
	}
		
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
#pragma endregion
}
#pragma region DOES NOT NEED CHANGES
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 15.0f, ZERO_V3, AXIS_Y);

	//initializing the model
	m_pMesh = new Simplex::MyMesh();

	//Generate a primitive
	m_pMesh->GenerateCone(1.5f, 2.0f, 15, C_GREEN);

	//Check if stops is empty if it is add some points to it
	if (m_v3StopList.size() == 0)
	{
		m_v3StopList.push_back(vector3(-2.5f,-2.5f,-2.5f));
		m_v3StopList.push_back(vector3( 2.5f,-2.5f,-2.5f));
		m_v3StopList.push_back(vector3( 2.5f, 2.5f, 2.5f));
		m_v3StopList.push_back(vector3(-2.5f, 2.5f, 2.5f));
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
void Application::Release(void)
{
	//release model
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}
#pragma endregion