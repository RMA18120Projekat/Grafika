
// Kol12016View.cpp : implementation of the CKol12016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol12016.h"
#endif

#include "Kol12016Doc.h"
#include "Kol12016View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"DImage.h"

// CKol12016View

IMPLEMENT_DYNCREATE(CKol12016View, CView)

BEGIN_MESSAGE_MAP(CKol12016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol12016View construction/destruction

CKol12016View::CKol12016View() noexcept
{
	// TODO: add construction code here
	alfa = 0;
	yO = 0;
	kontakt = false;
	brojVKD = 0;
	brojVKU = 0;
	d = 0;
	pozadina = new DImage();
	pozadina->Load(CString("felt2.jpg"));
	okvir = new DImage();
	okvir->Load(CString("wood.jpg"));

}

CKol12016View::~CKol12016View()
{
	if (pozadina)
		delete pozadina;
}

BOOL CKol12016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol12016View drawing
void CKol12016View::Translate(CDC* pDC, float dx, float dy,bool rightMultiply)
{
	XFORM a;
	a.eDx = dx;
	a.eDy = dy;
	a.eM11 = 1;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = 1;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);

}
void CKol12016View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float r = (angle * 3.14) / 180;
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = cos(r);
	a.eM12 = sin(r);
	a.eM21 = -sin(r);
	a.eM22 = cos(r);
	if (rightMultiply)
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);

}
void Koordinatni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);
	pDC->MoveTo(0, 0);
	pDC->LineTo(300, 0);
}
void CKol12016View::DrawStick(CDC* pDC, int w)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	Translate(pDC, -10, -10, false);
	Rotate(pDC, alfa, false);
	Translate(pDC, 10, 10, false);
	if (!kontakt)
		Translate(pDC, 0, yO, false);
	CPen olovka(PS_SOLID,1, RGB(176, 113, 84));
	CPen* staraO = pDC->SelectObject(&olovka);
	CBrush cetka1( RGB(255, 190, 128));
	CBrush* staraC = pDC->SelectObject(&cetka1);
	int d = w / 100;
	POINT tacke[] = { {-d / 2,0},{d / 2,0},{d,w},{-d,w} };
	pDC->Polygon(tacke, 4);
	CBrush cetka2(RGB(176, 113, 84));
	pDC->SelectObject(&cetka2);
	tacke[1].x = d / 2 + 2 * w / 3 * (d / 2) / w;
	tacke[1].y = 2 * w / 3;
	tacke[0].x = -tacke[1].x;
	tacke[0].y = 2 * w / 3;
	pDC->Polygon(tacke, 4);
	pDC->Ellipse(-d, w - d, d, d + w);
	
	pDC->SetWorldTransform(&stara);


	pDC->SelectObject(&staraC);
	pDC->SelectObject(&staraO);

}
void CKol12016View::DrawBall(CDC* pDC, int w)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	if (!kontakt)
	{
		Rotate(pDC, alfa, false);
	
	}
	else
	{
		Translate(pDC, 0, yO, false);

	}
	
	CPen olovka(PS_SOLID, 1, RGB(255, 0,0));
	CPen* staraO = pDC->SelectObject(&olovka);
	CBrush cetka1(RGB(139, 0, 0));
	CBrush* staraC = pDC->SelectObject(&cetka1);

	
	pDC->Ellipse(-w / 2, -w / 2, w, w);
	CBrush cetka2(RGB(255, 255, 255));
	pDC->SelectObject(&cetka2);
	Translate(pDC, 2, -2, false);
	float r = w / 4;
	pDC->Ellipse(-r / 2, -r / 2, r, r);
	//Koordinatni(pDC);
	pDC->SelectObject(&staraC);
	pDC->SelectObject(&staraO);
	if (kontakt)
	{
		if (d - 1 == 0)
			Rotate(pDC, -2*alfa, false);
		Translate(pDC, 0, -yO, false);

	}
	pDC->SetWorldTransform(&stara);

}
void CKol12016View::DrawTable(CDC* pDC, CRect rect)
{
	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), rect);
}
void CKol12016View::DrawBorder(CDC* pDC, CRect rect, int w)
{
	CRect a;
	GetClientRect(&a);
	XFORM stara;
	okvir->Draw(pDC, CRect(0, 0, okvir->Width(), okvir->Height()), CRect(0, 0, okvir->Width(), okvir->Height()));
	CRect sto;
	sto.top = a.top + w;
	sto.left = a.left + w;
	sto.right = a.right - w;
	sto.bottom = a.bottom - w;
	DrawTable(pDC, sto);

}
void CKol12016View::DrawHoles(CDC* pDC, CRect rect, int size)
{
	XFORM matrica;
	pDC->GetWorldTransform(&matrica);
	CBrush cetka(RGB(0, 0, 0));
	CPen olovka(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* c = pDC->SelectObject(&cetka);
	CPen* p = pDC->SelectObject(&olovka);
	Translate(pDC, rect.top+  1.5*size,rect.left+  1.5*size, false);
	//Koordinatni(pDC);
	pDC->Ellipse(-size / 2, -size / 2, size, size);
	Translate(pDC, -(rect.top +  1.5*size), -(rect.left +  1.5*size), false);
	
	Translate(pDC, rect.right-2*size, rect.bottom- 2*size, false);
	pDC->Ellipse(-size / 2, -size / 2, size, size);
	Translate(pDC, -(rect.right - 2*size), -(rect.bottom -  2*size), false);
	
	Translate(pDC, rect.left+ 2 * size, rect.bottom- 2 * size, false);
	pDC->Ellipse(-size / 2, -size / 2, size, size);
	Translate(pDC, -(rect.left + 2 * size), -(rect.bottom - 2 * size), false);
	
	Translate(pDC, rect.right - 2 * size, rect.top + 1.5 * size,false);
	pDC->Ellipse(-size / 2, -size / 2, size, size);

	pDC->SetWorldTransform(&matrica);
	pDC->SelectObject(&c);
	pDC->SelectObject(&p);
}
void CKol12016View::OnDrawInMemory(CDC* pDC)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	CRect client;
	GetClientRect(&client);
	//DrawTable(pDC, CRect(client.left+50,client.top+50,client.right-50,client.bottom-50));
	DrawBorder(pDC, client, 50);
	
}
void CKol12016View::OnDraw(CDC* pDC)
{
	CKol12016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM stara;
	CBitmap memory;
	CRect cl;
	GetClientRect(&cl);
	memory.CreateCompatibleBitmap(pDC,cl.Width(),cl.Height());
	CDC* pom = new CDC();
	pom->CreateCompatibleDC(pDC);
	CBitmap*a=pom->SelectObject(&memory);
	OnDrawInMemory(pom);
	pDC->BitBlt(0, 0, cl.Width(), cl.Height(), pom, 0, 0, SRCCOPY);
	pDC->GetWorldTransform(&stara);
	Translate(pDC, 600, 450, false);
	DrawStick(pDC, 300);
	Translate(pDC, -10, -10, false);
	DrawBall(pDC, 30);
	Translate(pDC, 0, 30, false);
	pDC->SetWorldTransform(&stara);
	DrawHoles(pDC, cl, 30);

	pom->SelectObject(&a);
	delete pom;
	// TODO: add draw code for native data here
}


// CKol12016View printing

BOOL CKol12016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol12016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol12016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol12016View diagnostics

#ifdef _DEBUG
void CKol12016View::AssertValid() const
{
	CView::AssertValid();
}

void CKol12016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol12016Doc* CKol12016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol12016Doc)));
	return (CKol12016Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol12016View message handlers


void CKol12016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case VK_LEFT:
		alfa += 10;
		Invalidate();
		break;
	case VK_RIGHT:
		alfa -= 10;
		Invalidate();
		break;
	case VK_UP:
		if (yO<0)
			kontakt = true;
		else 
			kontakt = false;
		yO -= 10;
		brojVKU += 1;
		Invalidate();
		break;
	case VK_DOWN:
		yO += 10;
		brojVKD += 1;
		Invalidate();
		break;
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKol12016View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
