
// zadatak4View.cpp : implementation of the Czadatak4View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "zadatak4.h"
#endif

#include "zadatak4Doc.h"
#include "zadatak4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"DImage.h"


// Czadatak4View

IMPLEMENT_DYNCREATE(Czadatak4View, CView)

BEGIN_MESSAGE_MAP(Czadatak4View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Czadatak4View construction/destruction

Czadatak4View::Czadatak4View() noexcept
{
	// TODO: add construction code here
	slika = new DImage();
	slika->Load(CString("Zad4.png"));

}

Czadatak4View::~Czadatak4View()
{
	if (slika)
		delete slika;
}

BOOL Czadatak4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Czadatak4View drawing
void Czadatak4View::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
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
void Czadatak4View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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
void Czadatak4View::Shear(CDC* pDC, float sx, float sy, bool rightMultiply)
{
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = 1;
	a.eM12 = sy;
	a.eM21 = sx;
	a.eM22 = 1;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);


}
void Czadatak4View::DrawFlagPart(CDC* pDC, DImage* img, double t1, double t2, int x1, int y1, int x2, int y2, int h)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	Translate(pDC, x1, y1, false);
	Shear(pDC, 0, ((float)(y2 - y1) /(x2 - x1)), false);
	Translate(pDC, -x1, -y1, false);
	img->Draw(pDC, CRect(t1*img->Width(), 0,t2* img->Width(), img->Height()), CRect(x1, y1, x1+(x2-x1), y1+h));
	pDC->SetWorldTransform(&a);

}
void Czadatak4View::DrawFlag(CDC* pDC, DImage* img, int x, int y, int w, int h, int nSeg, double a)
{
	double rad = (2* 3.14) ;
	double alfa = rad / nSeg;
	for (int i = 0;i < nSeg;i++)
	{
		double t1 = (i / nSeg);
		double t2 = (i + 1) / nSeg;
		int x1 = (int)(x+(i/nSeg) * w);
		int x2 = (int)(x+(i + 1)/nSeg * w);
		int y1 = (int)(y + a * sin(alfa * i));
		int y2 = (int)(y + a * sin(alfa * (i + 1)));
		DrawFlagPart(pDC, slika, t1, t2, x1, y1, x2, y2, h);
	}
}

void Czadatak4View::Draw(CDC* pDC)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	//Translate(pDC, 300, 100, false);
	//DrawFlag(pDC, slika, 30, 30, slika->Width() / 3, 50, 8, 40.0);
	Translate(pDC, 300, 200, false);
	DrawFlags(pDC, slika, 300, 300, 200, 80, 16, 20.0, 6);
	pDC->SetWorldTransform(&stara);

}
void Koordinatni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 300);
	pDC->MoveTo(0, 0);
	pDC->LineTo(100, 0);
}
void Czadatak4View::DrawFlags(CDC* pDC, DImage* img, int cx, int cy, int w, int h, int nSeg, double a,int nFlags)
{
	
	Koordinatni(pDC);
	float alfa = (2 * 3.14) / nFlags;
	for (int i = 0;i < nFlags;i++)
	{
		XFORM stara;
		pDC->GetWorldTransform(&stara);
		Translate(pDC, cx, cy, false);
		Rotate(pDC, alfa * i, false);

		DrawFlag(pDC, slika, (h / (2 * tan(alfa / 2))), -h / 2, w, h, nSeg, a);
		pDC->SetWorldTransform(&stara);
	}




}

void Czadatak4View::OnDraw(CDC* pDC)
{
	Czadatak4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	CRect c;
	GetClientRect(&c);
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	Translate(pDC, 300, 100, false);

	/*CDC* m = new CDC();
	CBitmap memorija;
	memorija.CreateCompatibleBitmap(pDC, c.Width(), c.Height());
	m->CreateCompatibleDC(pDC);
	CBitmap* a = m->SelectObject(&memorija);
	int gm=m->SetGraphicsMode(GM_ADVANCED);
	*/
	Draw(pDC);
	
	//m->SetGraphicsMode(gm);
	//pDC->BitBlt(0, 0, c.Width(), c.Height(), m, 0, 0, SRCCOPY);
	//m->SelectObject(&a);
	//delete m;
	pDC->SetWorldTransform(&stara);


	
	// TODO: add draw code for native data here
}


// Czadatak4View printing

BOOL Czadatak4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Czadatak4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Czadatak4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Czadatak4View diagnostics

#ifdef _DEBUG
void Czadatak4View::AssertValid() const
{
	CView::AssertValid();
}

void Czadatak4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Czadatak4Doc* Czadatak4View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Czadatak4Doc)));
	return (Czadatak4Doc*)m_pDocument;
}
#endif //_DEBUG


// Czadatak4View message handlers


BOOL Czadatak4View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
