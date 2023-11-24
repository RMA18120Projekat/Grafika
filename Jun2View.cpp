
// Jun2View.cpp : implementation of the CJun2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Jun2.h"
#endif

#include "Jun2Doc.h"
#include "Jun2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include<cmath>

// CJun2View

IMPLEMENT_DYNCREATE(CJun2View, CView)

BEGIN_MESSAGE_MAP(CJun2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CJun2View construction/destruction

CJun2View::CJun2View() noexcept
{
	// TODO: add construction code here

}

CJun2View::~CJun2View()
{
}

BOOL CJun2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
void Translate(CDC* pDC, float dx, float dy)
{
	XFORM a;
	a.eDx = dx;
	a.eDy = dy;
	a.eM11 = 1;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = 1;
	pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);
}
void Rotate(CDC* pDC, float angle)
{
	float radiani = (angle * 3.14) / 180;
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = cos(radiani);
	a.eM12 = sin(radiani);
	a.eM21 = -sin(radiani);
	a.eM22 = cos(radiani);
	pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);
}
// CJun2View drawing
void CJun2View::DrawWheel(CDC* pDC, double r1, double r2, double w)
{
	CBrush cetka(RGB(128, 128, 128));
	CBrush*staraC=pDC->SelectObject(&cetka);
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	//Translate(pDC, 200, 200);
	CRgn region;
	region.CreateEllipticRgn(-r1-r2, -r1-r2, r1+r2, r1+r2);
	pDC->FillRgn(&region, &cetka);
	CRgn odsecanje;
	odsecanje.CreateEllipticRgn(-r1, -r1, r1, r1);
	CBrush cetka2(RGB(255,255,255));
	pDC->SelectObject(&cetka2);
	pDC->FillRgn(&odsecanje, &cetka2);
	pDC->SelectObject(&cetka);
	pDC->Rectangle(-w, -r1, w, r1);
	Rotate(pDC, -45);
	pDC->Rectangle(-w, -r1, w, r1);
	Rotate(pDC, 90);
	pDC->Rectangle(-w, -r1, w, r1);

	pDC->SetWorldTransform(&stara);
	pDC->SelectObject(&staraC);
}
void Koordinatni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);
	pDC->MoveTo(0, 0);
	pDC->LineTo(500,0);
}
void CJun2View::DrawWP(CDC* pDC, double r1, double r2, double w, double L, double alpha, double d)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	Translate(pDC, 200, 400);
	float angle = (alpha * 3.14) / 180;
	POINT tacke[] = { {0,-L * tan(angle)},{0,0},{L,0 } };
	CRgn trougao;
	trougao.CreatePolygonRgn(tacke, 3, WINDING);
	CBrush cetka(RGB(128, 128, 128));
	CBrush* c = pDC->SelectObject(&cetka);
	pDC->FillRgn(&trougao, &cetka);
	Koordinatni(pDC);
	Translate(pDC, d, 0);
	Rotate(pDC, (r1 + r2) / d);
	DrawWheel(pDC, 30, 20, 10);
	pDC->SelectObject(&c);
	pDC->SetWorldTransform(&stara);
}


void CJun2View::OnDraw(CDC* pDC)
{
	CJun2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	//DrawWheel(pDC, 80, 30,10);
	DrawWP(pDC, 80, 30, 10, 340, 30, 180);
	// TODO: add draw code for native data here
}


// CJun2View printing

BOOL CJun2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CJun2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CJun2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CJun2View diagnostics

#ifdef _DEBUG
void CJun2View::AssertValid() const
{
	CView::AssertValid();
}

void CJun2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CJun2Doc* CJun2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJun2Doc)));
	return (CJun2Doc*)m_pDocument;
}
#endif //_DEBUG


// CJun2View message handlers
