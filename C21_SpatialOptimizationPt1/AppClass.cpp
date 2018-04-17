#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(
		vector3(0.0f, 0.0f, 100.0f), //Position
		vector3(0.0f, 0.0f, 99.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)
	
	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();

	uint uInstances = 500;
	int nSquare = static_cast<int>(std::sqrt(uInstances));
	uInstances = nSquare * nSquare;
	uint uIndex = 0;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj");
			vector3 v3Position = vector3(glm::sphericalRand(34.0f));
			matrix4 m4Position = glm::translate(v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position);

			/* Add each entity to separate dimension
			 * removes all potential collisions
			 */
			//m_pEntityMngr->AddDimension(-1, uIndex);
			//++uIndex;
			
			#pragma region Hard Code Dimensions

			// dimension 1
			if (v3Position.x < 0.0f && v3Position.y < 0.0f && v3Position.z > 0.0f)
				m_pEntityMngr->AddDimension(-1, 1);
			// dimension 2
			if (v3Position.x < 0.0f && v3Position.y > 0.0f && v3Position.z > 0.0f)
				m_pEntityMngr->AddDimension(-1, 2);
			// dimension 3
			if (v3Position.x > 0.0f && v3Position.y > 0.0f && v3Position.z > 0.0f)
				m_pEntityMngr->AddDimension(-1, 3);
			// dimension 4
			if (v3Position.x > 0.0f && v3Position.y < 0.0f && v3Position.z > 0.0f)
				m_pEntityMngr->AddDimension(-1, 4);
			// dimension 5
			if (v3Position.x < 0.0f && v3Position.y < 0.0f && v3Position.z < 0.0f)
				m_pEntityMngr->AddDimension(-1, 5);
			// dimension 6
			if (v3Position.x < 0.0f && v3Position.y > 0.0f && v3Position.z < 0.0f)
				m_pEntityMngr->AddDimension(-1, 6);
			// dimension 7
			if (v3Position.x > 0.0f && v3Position.y > 0.0f && v3Position.z < 0.0f)
				m_pEntityMngr->AddDimension(-1, 7);
			// dimension 8
			if (v3Position.x > 0.0f && v3Position.y < 0.0f && v3Position.z < 0.0f)
				m_pEntityMngr->AddDimension(-1, 8);

			#pragma endregion
			
		}
	}
	m_pEntityMngr->Update();
	//steve
	//m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
	
	//Update Entity Manager
	m_pEntityMngr->Update();

	m_pMeshMngr->AddGridToRenderList(glm::rotate(IDENTITY_M4, 90.0f, AXIS_Y));
	m_pMeshMngr->AddGridToRenderList(glm::translate(vector3(-17.0f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, 90.0f, AXIS_Y));
	m_pMeshMngr->AddGridToRenderList(glm::translate(vector3(17.0f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, 90.0f, AXIS_Y));
		
	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
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
}
void Application::Release(void)
{
	//release GUI
	ShutdownGUI();
}