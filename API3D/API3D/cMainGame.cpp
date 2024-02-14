#include "framework.h"
#include "cMainGame.h"

cMainGame::cMainGame()
	:m_hBitmap(NULL)
	, m_vEye(0, 0, -5)
	, m_vLookAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_vPosition(0, 0, 0)
	, m_fBoxRotY(0.0f),

	m_vBoxDirection(0, 0, 1),
	m_fScale(1.0f), // 1 = 기본 크기
	m_fCameraDistance(5.0f),
	m_isLButtonDown(false),
	m_vCamRotAngle(0, 0, 0)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;
}


cMainGame::~cMainGame()
{
	SelectObject(m_MemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_MemDC);
}

void cMainGame::Setup()
{
	HDC hdc = GetDC(g_hWnd);
	m_MemDC = CreateCompatibleDC(hdc);

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	m_hBitmap = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	m_hOldBitmap = (HBITMAP)SelectObject(m_MemDC, m_hBitmap);
	ReleaseDC(g_hWnd, hdc);

	// >> : vertex
	{
		// : front
		m_vecVertex.push_back(cVector3(-1.0f, -1.0f, -1.0f));
		m_vecVertex.push_back(cVector3(-1.0f, 1.0f, -1.0f));
		m_vecVertex.push_back(cVector3(1.0f, 1.0f, -1.0f));
		m_vecVertex.push_back(cVector3(1.0f, -1.0f, -1.0f));
		// : far
		m_vecVertex.push_back(cVector3(-1.0f, -1.0f, 1.0f));
		m_vecVertex.push_back(cVector3(-1.0f, 1.0f, 1.0f));
		m_vecVertex.push_back(cVector3(1.0f, 1.0f, 1.0f));
		m_vecVertex.push_back(cVector3(1.0f, -1.0f, 1.0f));
		// << :
	}

	// >> : index
	{
		m_vecIndex.push_back(0);
		m_vecIndex.push_back(1);
		m_vecIndex.push_back(2);

		m_vecIndex.push_back(0);
		m_vecIndex.push_back(2);
		m_vecIndex.push_back(3);
		
		m_vecIndex.push_back(4);
		m_vecIndex.push_back(6);
		m_vecIndex.push_back(5);

		m_vecIndex.push_back(4);
		m_vecIndex.push_back(7);
		m_vecIndex.push_back(6);

		m_vecIndex.push_back(4);
		m_vecIndex.push_back(5);
		m_vecIndex.push_back(1);

		m_vecIndex.push_back(4);
		m_vecIndex.push_back(1);
		m_vecIndex.push_back(0);

		m_vecIndex.push_back(3);
		m_vecIndex.push_back(2);
		m_vecIndex.push_back(6);

		m_vecIndex.push_back(3);
		m_vecIndex.push_back(6);
		m_vecIndex.push_back(7);

		m_vecIndex.push_back(1);
		m_vecIndex.push_back(5);
		m_vecIndex.push_back(6);

		m_vecIndex.push_back(1);
		m_vecIndex.push_back(6);
		m_vecIndex.push_back(2);

		m_vecIndex.push_back(4);
		m_vecIndex.push_back(0);
		m_vecIndex.push_back(3);

		m_vecIndex.push_back(4);
		m_vecIndex.push_back(3);
		m_vecIndex.push_back(7);
	}
	// << :

	m_matWorld = cMatrix::Identity(4);
	m_matView = cMatrix::Identity(4);
	m_matProj = cMatrix::Identity(4);
	m_matViewport = cMatrix::Identity(4);

	SetGrid();
}

void cMainGame::Update()
{
	Update_Scale();
	Update_Rotation();
	Update_Move();

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	cMatrix matRX = cMatrix::RotationX(m_vCamRotAngle.x);
	cMatrix matRY = cMatrix::RotationY(m_vCamRotAngle.y);
	cMatrix matRC = matRX * matRY;

	m_vLookAt = m_vPosition;
	m_vEye = cVector3(0.0f, m_fCameraDistance, -m_fCameraDistance);
	m_vEye = cVector3::TransformCoord(m_vEye, matRC);
	m_vEye = m_vEye + m_vPosition;

	// W = S x R x T
	cMatrix matS = cMatrix::Scale(m_fScale);
	cMatrix matR = cMatrix::RotationY(m_fBoxRotY);
	cMatrix matT = cMatrix::Translation(m_vPosition);

	m_vBoxDirection = cVector3(0, 0, 1);
	m_vBoxDirection = cVector3::TransformNormal(m_vBoxDirection, matR);

	m_matWorld = matS * matR * matT;
	m_matView = cMatrix::View(m_vEye, m_vLookAt, m_vUp);
	m_matProj = cMatrix::Projection(PI / 4.0f, (float)rc.right / rc.bottom, 1.0f, 1000.f);
	m_matViewport = cMatrix::Viewport(0, 0, rc.right, rc.bottom, 0, 1);
}

void cMainGame::Render(HDC hdc)
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	PatBlt(m_MemDC, rc.left, rc.top, rc.right, rc.bottom, WHITENESS);

	cMatrix matWVP = m_matWorld * m_matView * m_matProj;
	for (size_t i = 0; i < m_vecIndex.size(); i += 3)
	{
		cVector3 v1 = m_vecVertex[m_vecIndex[i + 0]];
		cVector3 v2 = m_vecVertex[m_vecIndex[i + 1]];
		cVector3 v3 = m_vecVertex[m_vecIndex[i + 2]];

		v1 = cVector3::TransformCoord(v1, matWVP);
		v2 = cVector3::TransformCoord(v2, matWVP);
		v3 = cVector3::TransformCoord(v3, matWVP);

		// : to do something
		if (IsBackFace(v1, v2, v3))
			continue;

		v1 = cVector3::TransformCoord(v1, m_matViewport);
		v2 = cVector3::TransformCoord(v2, m_matViewport);
		v3 = cVector3::TransformCoord(v3, m_matViewport);

		// >> :
		MoveToEx(m_MemDC, v1.x, v1.y, NULL);
		LineTo(m_MemDC, v2.x, v2.y);
		LineTo(m_MemDC, v3.x, v3.y);
		LineTo(m_MemDC, v1.x, v1.y);
	}

	DrawGrid();

	BitBlt(hdc, 0, 0, rc.right, rc.bottom, m_MemDC, 0, 0, SRCCOPY);
}

void cMainGame::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		m_isLButtonDown = true;
		break;
	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		if (m_isLButtonDown)
		{
			POINT ptCurMouse;
			ptCurMouse.x = LOWORD(lParam);
			ptCurMouse.y = HIWORD(lParam);
			float fDeltaX = (float)ptCurMouse.x - m_ptPrevMouse.x;
			float fDeltaY = (float)ptCurMouse.y - m_ptPrevMouse.y;

			m_vCamRotAngle.y += (fDeltaX / 100.0f);
			m_vCamRotAngle.x += (fDeltaY / 100.0f);

			if (m_vCamRotAngle.x < -PI / 2.0f + 0.0001f)
				m_vCamRotAngle.x = -PI / 2.0f + 0.0001f;
			if (m_vCamRotAngle.x > PI / 2.0f + 0.0001f)
				m_vCamRotAngle.x = PI / 2.0f + 0.0001f;

			m_ptPrevMouse = ptCurMouse;
		}
		break;
	case WM_MOUSEWHEEL:
		m_fCameraDistance -= (GET_WHEEL_DELTA_WPARAM(wParam) / 30.0f);
		if (m_fCameraDistance < 0.0001f)
			m_fCameraDistance = 0.0001f;
		break;
	}
}

bool cMainGame::IsBackFace(cVector3& v1, cVector3& v2, cVector3& v3)
{
	cVector3 v12 = v2 - v1;
	cVector3 v13 = v3 - v1;
	cVector3 n = cVector3::Cross(v12, v13); //Normal vector
	cVector3 l = cVector3(0, 0, 1);
	if (cVector3::Dot(n, l) > 0)
		return true;

	return false;

}

void cMainGame::SetGrid()
{
	int nNumHalfTile = 5;
	float fInterval = 1.0f;
	float fMax = nNumHalfTile * fInterval;
	float fMin = -nNumHalfTile * fInterval;

	for (int i = 0; i <= nNumHalfTile; ++i)
	{
		m_vecLineVertex.push_back(cVector3(fMin, 0, i * fInterval));
		m_vecLineVertex.push_back(cVector3(fMax, 0, i * fInterval));

		m_vecLineVertex.push_back(cVector3(fMin, 0, -i * fInterval));
		m_vecLineVertex.push_back(cVector3(fMax, 0, -i * fInterval));

		m_vecLineVertex.push_back(cVector3(i * fInterval, 0, fMin));
		m_vecLineVertex.push_back(cVector3(i * fInterval, 0, fMax));

		m_vecLineVertex.push_back(cVector3(-i * fInterval, 0, fMin));
		m_vecLineVertex.push_back(cVector3(-i * fInterval, 0, fMax));
	}

	m_vecLineVertex.push_back(cVector3(0, 0, fMin));
	m_vecLineVertex.push_back(cVector3(0, 0, fMax));

	m_vecLineVertex.push_back(cVector3(fMin, 0, 0));
	m_vecLineVertex.push_back(cVector3(fMax, 0, 0));

	m_vAxisXTextPosition = cVector3(fMax, 0, 0);
	m_vAxisZTextPosition = cVector3(0, 0, fMax);
}

void cMainGame::DrawGrid()
{
	cMatrix mat = m_matView * m_matProj * m_matViewport;

	for (size_t i = 0; i < m_vecLineVertex.size(); i += 2)
	{
		cVector3 v1 = m_vecLineVertex[i + 0];
		cVector3 v2 = m_vecLineVertex[i + 1];

		v1 = cVector3::TransformCoord(v1, mat);
		v2 = cVector3::TransformCoord(v2, mat);

		MoveToEx(m_MemDC, v1.x, v1.y, NULL);
		LineTo(m_MemDC, v2.x, v2.y);
	}

	cVector3 v = m_vAxisXTextPosition;
	v = cVector3::TransformCoord(v, mat);
	TextOut(m_MemDC, v.x, v.y, (LPCWSTR)"X", 1);

	v = m_vAxisZTextPosition;
	v = cVector3::TransformCoord(v, mat);
	TextOut(m_MemDC, v.x, v.y, (LPCWSTR)"Z", 1);

}

void cMainGame::Update_Rotation()
{
	if (GetKeyState('A') & 0x8000)
	{
		m_fBoxRotY -= 0.1f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_fBoxRotY += 0.1f;
	}
}

void cMainGame::Update_Move()
{
	if (GetKeyState('W') & 0x8000)
	{
		cVector3 dir = (m_vBoxDirection * 0.1f);
		m_vPosition = m_vPosition + dir;
	}
	if (GetKeyState('S') & 0x8000)
	{
		cVector3 dir = (m_vBoxDirection * 0.1f);
		m_vPosition = m_vPosition - dir;
	}
}

void cMainGame::Update_Scale()
{
	if (GetKeyState('Z') & 0x8000)
	{
		m_fScale += 0.1f;
		if (m_fScale > 3.0f)
			m_fScale = 3.0f;
	}
	if (GetKeyState('X') & 0x8000)
	{
		m_fScale -= 0.1f;
		if (m_fScale < 0.1f)
			m_fScale = 0.1f;
	}
}
