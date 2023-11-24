
// IKol2015View.cpp : implementation of the CIKol2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IKol2015.h"
#endif

#include "IKol2015Doc.h"
#include "IKol2015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"DImage.h"


// CIKol2015View

IMPLEMENT_DYNCREATE(CIKol2015View, CView)

BEGIN_MESSAGE_MAP(CIKol2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIKol2015View construction/destruction

CIKol2015View::CIKol2015View() noexcept
{
	// TODO: add construction code here
	pozadina = new DImage();
	pozadina->Load(CString("blue.png"));
	susret = false;
	otvorena = true;
	offsetX = 0;
	offsetY = 0;
	rotirajX = false;
	rotirajY = 0;
}

CIKol2015View::~CIKol2015View()
{
	if (pozadina)
		delete pozadina;
}

BOOL CIKol2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKol2015View drawing
void CIKol2015View::Translate(CDC* pDC, float dx, float dy)
{
	XFORM matrica;
	matrica.eDx = dx;
	matrica.eDy = dy;
	matrica.eM11 = 1;
	matrica.eM12 = 0;
	matrica.eM21 = 0;
	matrica.eM22 = 1;
	pDC->ModifyWorldTransform(&matrica, MWT_LEFTMULTIPLY);
}
void CIKol2015View::Rotate(CDC* pDC, float angle)
{
	float r = (angle * 3.14) / 180;
	XFORM matrica;
	matrica.eDx = 0;
	matrica.eDy = 0;
	matrica.eM11 = cos(r);
	matrica.eM12 = sin(r);
	matrica.eM21 = -sin(r);
	matrica.eM22 = cos(r);
	pDC->ModifyWorldTransform(&matrica, MWT_LEFTMULTIPLY);
}
void CIKol2015View::Mirror(CDC* pDC,float mx,float my)
{
	XFORM matrica;
	matrica.eDx = 0;
	matrica.eDy = 0;
	matrica.eM11 = mx ? -1 : 1;
	matrica.eM12 = 0;
	matrica.eM21 = 0;
	matrica.eM22 = my ? -1 : 1;
	pDC->ModifyWorldTransform(&matrica, MWT_LEFTMULTIPLY);
}
void Koordinatni(CDC* pDC)
{
	CPen cetka1(PS_SOLID,1,RGB(0, 0, 0));
	CPen* staraC = pDC->SelectObject(&cetka1);

	pDC->MoveTo(0, 0);
	pDC->LineTo(300, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);
	pDC->SelectObject(&staraC);
}

void CIKol2015View::DrawPacman(CDC* pDC, CRect rect, float angle)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	CBrush cetka1(RGB(255, 255, 0));
	CBrush* staraC = pDC->SelectObject(&cetka1);
	Translate(pDC, 400, 400);
	//Koordinatni(pDC);
	if (rotirajX)
		Mirror(pDC, true, false);
	else
		Mirror(pDC, false, false);
	Rotate(pDC, rotirajY);
	Translate(pDC, offsetX, offsetY);
	
	int dY = otvorena ? sin(angle * 3.14 / 180) * rect.Height() : 0;
	pDC->Pie(rect,
		POINT{ rect.right, rect.bottom - rect.Height() / 2 - dY },
		POINT{ rect.right, rect.bottom - rect.Height() / 2 + dY }
	);
	Translate(pDC, 43, 15);
	//Koordinatni(pDC);
	CBrush crna(RGB(0, 0, 0));
	pDC->SelectObject(&crna);
	pDC->Ellipse(-rect.Width() / 16, -rect.Width() / 16, rect.Width() / 8, rect.Width() / 8);
	pDC->SelectObject(&staraC);
	pDC->SetWorldTransform(&stara);

}
void CIKol2015View::DrawGhost(CDC* pDC, CRect rect)
{
	
	CBrush cetka1(RGB(255, 0, 0));
	CBrush* staraC = pDC->SelectObject(&cetka1);
	Translate(pDC, 200, 200);
	int d = rect.Width();
	int dSmall = d / 6;
	int rSmall = dSmall / 2;
	//pDC->Rectangle(rect);
	pDC->BeginPath();
	{
		pDC->MoveTo(rect.left, rect.top+d/2);
		pDC->SetArcDirection(AD_CLOCKWISE);
		POINT a = { rect.left,rect.top+rect.Width() / 2 };
		POINT b = { rect.right, rect.top+rect.Width() / 2};
		pDC->ArcTo(rect, a, b);
		pDC->LineTo(rect.right, rect.bottom);
		pDC->ArcTo(
			CRect(POINT{ rect.right - rSmall, rect.bottom - dSmall }, SIZE{ dSmall, dSmall }),
			POINT{ rect.right, rect.bottom },
			POINT{ rect.right - rSmall, rect.bottom - rSmall }
		);
		for (int i = 0; i < 5; i++) {
			pDC->SetArcDirection(i % 2 == 0 ? AD_COUNTERCLOCKWISE : AD_CLOCKWISE);

			pDC->ArcTo(
				CRect(
					POINT{ rect.right - rSmall - (i + 1) * dSmall, rect.bottom - dSmall },
					SIZE{ dSmall, dSmall }
				),
				POINT{ rect.right - rSmall - i * dSmall, rect.bottom - rSmall },
				POINT{ rect.right - rSmall - (i + 1) * dSmall, rect.bottom - rSmall }
			);
		}
		pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->ArcTo(
			CRect(POINT{ rect.left - rSmall, rect.bottom - dSmall }, SIZE{ dSmall, dSmall }),
			POINT{ rect.left + rSmall, rect.bottom - rSmall },
			POINT{ rect.left, rect.bottom }
		);

	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();
	Translate(pDC, 120, 150);
	//Koordinatni(pDC);
	CBrush cetka2(RGB(255, 255, 255));
	pDC->SelectObject(&cetka2);
	pDC->Ellipse(-rSmall, -rSmall, 2 * rSmall, 2 * rSmall);
	CBrush cetka3(RGB(0, 0, 255));
	pDC->SelectObject(&cetka3);
	Translate(pDC, 5, 3);
	pDC->Ellipse(-rSmall/2, -rSmall/2,  rSmall,  rSmall);
	Translate(pDC, 30, -3);
	//Koordinatni(pDC);
	pDC->SelectObject(&cetka2);
	pDC->Ellipse(-rSmall, -rSmall, 2 * rSmall, 2 * rSmall);
	pDC->SelectObject(&cetka3);
	Translate(pDC, 5, 3);
	pDC->Ellipse(-rSmall / 2, -rSmall / 2, rSmall, rSmall);
	pDC->SelectObject(&staraC);

}
void CIKol2015View::BackGround(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);
	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), client);

}

void CIKol2015View::TheEnd(CDC* pDC)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	CFont font;
	font.CreateFont(72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
	CFont* stariF = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	Translate(pDC, 500, 200);
	pDC->TextOutW(0, 0, CString("The end"));
	pDC->SelectObject(&stariF);
	pDC->SetWorldTransform(&stara);

}
bool CIKol2015View::Susret(CRect rect1, CRect rect2)
{
	float r1 = rect1.Width() / 2;
	float r2 = rect2.Width() / 2;
	POINT c1 = { rect1.top + r1 + rect1.left + r1 };
	POINT c2 = { rect2.top + r2 + rect2.left + r2 };
	return (pow(r1 - r2, 2) >= pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2) && pow(r1 - r2, 2) <= pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
}
void CIKol2015View::OnDraw(CDC* pDC)
{
	CIKol2015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM stara;
	//pDC->GetWorldTransform(&stara);
	CRect client;
	GetClientRect(&client);
	CBitmap memory;
	memory.CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	CDC* pom = new CDC();
	pom->CreateCompatibleDC(pDC);
	CBitmap* a = pom->SelectObject(&memory);
	BackGround(pom);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), pom, 0, 0, SRCCOPY);
	DrawPacman(pDC, CRect(0, 0, 100, 100),30);
	DrawGhost(pDC, CRect(100, 120, 196, 216));
	if( Susret(CRect(0, 0, 100, 100), CRect(100, 120, 196, 216)))
		TheEnd(pDC);
	//pDC->SetWorldTransform(&stara);
	pom->SelectObject(&a);
	delete pom;
	// TODO: add draw code for native data here
}


// CIKol2015View printing

BOOL CIKol2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKol2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKol2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIKol2015View diagnostics

#ifdef _DEBUG
void CIKol2015View::AssertValid() const
{
	CView::AssertValid();
}

void CIKol2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKol2015Doc* CIKol2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKol2015Doc)));
	return (CIKol2015Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKol2015View message handlers


BOOL CIKol2015View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CIKol2015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case VK_UP:
		offsetY -= 10;
		rotirajX = false;
		rotirajY = -90;
		otvorena = !otvorena;
		Invalidate();
		break;
	case VK_DOWN:
		offsetY += 10;
		rotirajY = -90;
		rotirajX = false;
		otvorena = !otvorena;
		Invalidate();
		break;
	case VK_LEFT:
		offsetX -= 10;
		rotirajX = true;
		rotirajY = 0;
		otvorena = !otvorena;
		Invalidate();
		break;
	case VK_RIGHT:
		offsetX += 10;
		rotirajX = false;
		rotirajY = 0;
		otvorena = !otvorena;
		Invalidate();
		break;
	default:
		break;


	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
