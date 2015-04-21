#include "stdafx.h"
#include "TestSinglePolygonWithObject.h"
#include "Defines.h"

CTestSinglePolygonWithObject::CTestSinglePolygonWithObject()
{
}


CTestSinglePolygonWithObject::~CTestSinglePolygonWithObject()
{
}

void CTestSinglePolygonWithObject::Initialize()
{
	// initialize a single polygon
	auto poly = &poly1[0];
	poly->state = POLY4DV1_STATE_ACTIVE;
	poly->attr = 0;
	poly->color = RGB(0, 255, 0);
	poly->worldPos = { 0, 0, 0, 1 };
	poly->vlist[0] = { 0, 50, 0, 1 };
	poly->vlist[1] = { 10, 0, 10, 1 };
	poly->vlist[2] = { 10, 0, -10, 1 };
	poly->prev = NULL;
	poly->next = &poly1[1];
	poly = &poly1[1];
	poly->state = POLY4DV1_STATE_ACTIVE;
	poly->attr = 0;
	poly->color = RGB(0, 255, 0);
	poly->worldPos = { 0, 0, 0, 1 };
	poly->vlist[0] = { 0, 50, 0, 1 };
	poly->vlist[1] = { 10, 0, 10, 1 };
	poly->vlist[2] = { -10, 0, 10, 1 };
	poly->prev = &poly1[0];
	poly->next = &poly1[2];
	poly = &poly1[2];
	poly->state = POLY4DV1_STATE_ACTIVE;
	poly->attr = 0;
	poly->color = RGB(0, 255, 0);
	poly->worldPos = { 0, 0, 0, 1 };
	poly->vlist[0] = { 0, 50, 0, 1 };
	poly->vlist[1] = { 10, 0, -10, 1 };
	poly->vlist[2] = { -10, 0, -10, 1 };
	poly->prev = &poly1[1];
	poly->next = &poly1[3];
	poly = &poly1[3];
	poly->state = POLY4DV1_STATE_ACTIVE;
	poly->attr = 0;
	poly->color = RGB(0, 255, 0);
	poly->worldPos = { 0, 0, 0, 1 };
	poly->vlist[0] = { 0, 50, 0, 1 };
	poly->vlist[1] = { -10, 0, -10, 1 };
	poly->vlist[2] = { -10, 0, 10, 1 };
	poly->prev = &poly1[2];
	poly->next = NULL;


	POINT4D  cam_pos = { 0, 0, -200, 1 };
	POINT4D  cam_target = { 0, 0, 5, 1 };
	VECTOR4D cam_dir = { 0, 0, 0, 1 };
	VECTOR4D cv = { 0, 0, 1, 1 };

	m_pCam = new CCamera();
	m_pCam->Initialize(CAMERA_TYPE::CAMERA_ELUER, cam_pos, cam_dir, &cam_target, &cv, false, 5, 50, 90, WINDOW_WIDTH, WINDOW_HEIGHT);
	m_pCam->UpdateCameraMatrix();

	// ����͸�Ӻ���Ļ�任����
	// ͸�ӱ任����
	m_pCam->UpdateProjectMatrix();
	// ��Ļ�任����
	m_pCam->UpdateScreenMatrix();
}

void CTestSinglePolygonWithObject::Render()
{
	if (KEY_DOWN(VK_DOWN))
	{
		m_pCam->WorldPos.z -= 1;
	}
	if (KEY_DOWN(VK_UP))
	{
		m_pCam->WorldPos.z += 1;
	}
	if (KEY_DOWN(VK_LEFT))
	{
		m_pCam->WorldPos.x -= 1;
	}
	if (KEY_DOWN(VK_RIGHT))
	{
		m_pCam->WorldPos.x += 1;
	}
	if (KEY_DOWN(VK_SPACE))
	{
		m_pCam->WorldPos.y += 1;
	}
	if (KEY_DOWN(VK_NUMPAD0))
	{
		m_pCam->WorldPos.y -= 1;
	}
	m_pCam->UpdateCameraMatrix();

	for (int i = 0; i < ARRAYSIZE(poly1); i++)
	{
		CPLGLoader::Insert_POLYF4DV1_RENDERLIST4DV1(&rend_list, &poly1[i]);
	}

	/*****************************
	3D��ˮ��
	*****************************/
	// ����任
	CPLGLoader::Model_To_World_RENDERLIST4DV1(&rend_list, 2);


	// ÿ����תһ������

	// ��������
	//ObjectDeleteBackface(&g_Obj, &g_Camera);

	// ����任
	CPLGLoader::World_To_Camera_RENDERLIST4DV1(&rend_list, &m_pCam->MatrixCamera);

	// ͶӰ�任
	CPLGLoader::Camera_To_Perspective_RENDERLIST4DV1(&rend_list, m_pCam);

	// �ӿڱ任
	CPLGLoader::Perspective_To_Screen_RENDERLIST4DV1(&rend_list, m_pCam);

	// ��������
	LPBYTE lpBack = m_pDraw->DDraw_Lock_Back_Surface();

	// render the polygon list
	CPLGLoader::Draw_RENDERLIST4DV1_Wire16(&rend_list, lpBack, m_pDraw->GetBackPitch(), m_rcWindow);

	// unlock the back buffer
	m_pDraw->DDraw_Unlock_Back_Surface();

	m_pDraw->Wait_Clock(30);

}
