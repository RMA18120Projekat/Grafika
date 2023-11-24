
// IKol2014View.cpp : implementation of the CIKol2014View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IKol2014.h"
#endif

#include "IKol2014Doc.h"
#include "IKol2014View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"DImage.h"


// CIKol2014View

IMPLEMENT_DYNCREATE(CIKol2014View, CView)

BEGIN_MESSAGE_MAP(CIKol2014View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIKol2014View construction/destruction

CIKol2014View::CIKol2014View() noexcept
{
	// TODO: add construction code here
	clio=GetEnhMetaFile(CString("clio.emf"));
	Wheel = new DImage();
	Wheel->Load(CString("Wheel.png"));
	desno = 0;
	nagib = 0;

}

CIKol2014View::~CIKol2014View()
{
	if (Wheel)
		delete Wheel;
	DeleteEnhMetaFile(clio);
}

BOOL CIKol2014View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKol2014View drawing
void CIKol2014View::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
{
	XFORM a;
	a.eDx = dx;
	a.eDy = dy;
	a.eM11 = 1;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = 1;
	if(rightMultiply)
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);

}
void CIKol2014View::Rotate(CDC* pDC, float angle,bool rightMultiply)
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
void CIKol2014View::Scale(CDC* pDC, float sX,float sY, bool rightMultiply)
{
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = sX;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = sY;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);


}
void CIKol2014View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = mx ? -1 : 1;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = my ? -1 : 1;;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);

}

void Koordinatni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(300, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);
}
void CIKol2014View::DrawGround(CDC* pDC, float angle)
{
	CRect c;
	GetClientRect(&c);
	CBrush cetka1(RGB(100, 100, 255));
	CBrush* staraC = pDC->SelectObject(&cetka1);
	pDC->Rectangle(c);
	CPen olovka(PS_SOLID, 3, RGB(150, 75, 0));
	CPen* staraO = pDC->SelectObject(&olovka);
	CBrush cetka2;
	cetka2.CreateHatchBrush(HS_FDIAGONAL, RGB(150, 75, 0));
	pDC->SelectObject(&cetka2);
	COLORREF pozadina=pDC->SetBkColor(RGB(144, 238, 144));
	
	POINT trougao[] = { {c.left,c.bottom},{c.right,c.bottom},{c.right,c.bottom-c.Width()*tan(angle * 3.14 / 180)} };
	pDC->Polygon(trougao, 3);
	pDC->SelectObject(&staraC);
	pDC->SelectObject(&staraO);
	pDC->SetBkColor(pozadina);
}
void CIKol2014View::DrawCar(CDC* pDC, int x, int y, int w, int h)
{
	
	//Koordinatni(pDC);
	Mirror(pDC, true, false, false);
	Translate(pDC, -300, 430, false);
	//Koordinatni(pDC);
	pDC->PlayMetaFile(clio, CRect(w/2 - x, h/2 - y, w/2 + x, h/2 + y));
	Mirror(pDC, true, false, false);


}
void DrawTransparent(CDC* pDC, DImage* i,int x,int y,int r)
{
	CBitmap source;
	CBitmap mask;
	CDC* s = new CDC();
	CDC* m = new CDC();
	source.CreateCompatibleBitmap(pDC,i->Width(),i->Height());
	mask.CreateBitmap(i->Width(), i->Height(), 1, 1, NULL);
	s->CreateCompatibleDC(pDC);
	m->CreateCompatibleDC(pDC);
	CBitmap* as = s->SelectObject(&source);
	CBitmap* am = m->SelectObject(&mask);
	i->Draw(s, CRect(0, 0, i->Width(), i->Height()), CRect(0, 0, i->Width(), i->Height()));
	COLORREF topLeft = s->GetPixel(0, 0);
	COLORREF pozadina = s->SetBkColor(topLeft);
	COLORREF text = s->SetTextColor(RGB(255, 255, 255));
	m->BitBlt(0, 0, i->Width(), i->Height(), s, 0, 0, SRCCOPY);
	s->SetBkColor(RGB(0, 0, 0));
	s->BitBlt(0, 0, i->Width(), i->Height(), m, 0, 0, SRCAND);
	m->SetBkColor(pozadina);
	s->SetTextColor(text);
	s->SelectObject(&as);
	m->SelectObject(&am);
	delete s;
	delete m;
	CDC* pom = new CDC();
	pom->CreateCompatibleDC(pDC);
	CBitmap* apom = pom->SelectObject(&mask);
	pDC->BitBlt(x-r, y-r, r+x, r+y, pom, 0, 0, SRCAND);
	pom->SelectObject(&source);
	pDC->BitBlt(0, 0, i->Width(), i->Height(), pom, 0, 0, SRCPAINT);
	pom->SelectObject(&apom);
	delete pom;

}
void CIKol2014View::DrawWheel(CDC* pDC, int x, int y, int r, float angle)
{
	
	Rotate(pDC, angle, false);
	Scale(pDC, 1.2, 1.2, false);
	//DrawTransparent(pDC, Wheel,200,200,38);
	auto b = Wheel->GetDIBBits();
	Wheel->DrawTransparent(pDC, CRect(0 ,0, Wheel->Width(),Wheel->Height()), CRect(x - r, y - r, x + r, y + r), RGB(b[2], b[1], b[0]));
	Scale(pDC, 1/1.2, 1/1.2, false);
	Rotate(pDC, -angle, false);

}
void CIKol2014View::Crtaj(CDC* pDC)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	DrawGround(pDC, nagib);
	Rotate(pDC, -nagib, false);
	Translate(pDC, desno, 0, false);
	DrawCar(pDC, 225, 90, 450, 180);
	Translate(pDC, -1.2 * 225, 90, false);
	Translate(pDC, -115, 70, false);
	DrawWheel(pDC, 0, 0, 38, nagib);
	Translate(pDC, 290, 0, false);
	Koordinatni(pDC);
	DrawWheel(pDC, 0, 0, 38, 45);

	pDC->SetWorldTransform(&stara);

}
void CIKol2014View::OnDraw(CDC* pDC)
{
	CIKol2014Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect c;
	GetClientRect(&c);
	CBitmap memorija;
	CDC* m = new CDC();
	m->CreateCompatibleDC(pDC);
	memorija.CreateCompatibleBitmap(pDC, c.Width(), c.Height());
	CBitmap* a = m->SelectObject(&memorija);
	int gm=m->SetGraphicsMode(GM_ADVANCED);
	Crtaj(m);
	
	m->SetGraphicsMode(gm);
	pDC->BitBlt(0, 0, c.Width(), c.Height(), m, 0, 0, SRCCOPY);
	m->SelectObject(&a);
	delete m;
	
	// TODO: add draw code for native data here
}


// CIKol2014View printing

BOOL CIKol2014View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKol2014View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKol2014View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIKol2014View diagnostics

#ifdef _DEBUG
void CIKol2014View::AssertValid() const
{
	CView::AssertValid();
}

void CIKol2014View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKol2014Doc* CIKol2014View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKol2014Doc)));
	return (CIKol2014Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKol2014View message handlers


BOOL CIKol2014View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CIKol2014View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	switch (nChar)
	{
	case VK_UP:
		if(nagib+10<=80)
			nagib += 10;
		desno = 0;
		Invalidate();
		break;
	case VK_DOWN:
		if(nagib-10>=-10)
			nagib -= 10;
		desno = 0;
		Invalidate();
		break;
	case VK_LEFT:
		desno -= 10;
		Invalidate();
		break;
	case VK_RIGHT:
		desno += 10;
		Invalidate();
		break;
	default:
		break;
	}
}
