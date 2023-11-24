
// HodajuciTransformerView.cpp : implementation of the CHodajuciTransformerView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "HodajuciTransformer.h"
#endif

#include "HodajuciTransformerDoc.h"
#include "HodajuciTransformerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"DImage.h"

// CHodajuciTransformerView

IMPLEMENT_DYNCREATE(CHodajuciTransformerView, CView)

BEGIN_MESSAGE_MAP(CHodajuciTransformerView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CHodajuciTransformerView construction/destruction

CHodajuciTransformerView::CHodajuciTransformerView() noexcept
{
	// TODO: add construction code here

	body = new DImage();
	leg1 = new DImage();
	leg2 = new DImage();
	leg3= new DImage();
	back2 = new DImage();
	body->Load(CString("body.png"));
	leg1->Load(CString("leg1.png"));
	leg2->Load(CString("leg2.png"));
	leg3->Load(CString("leg3.png"));
	back2->Load(CString("background.jpeg"));


}

CHodajuciTransformerView::~CHodajuciTransformerView()
{
	if(body)
		delete body;
	if(leg1)
		delete leg1;
	if(leg2)
		delete leg2;
	if(leg3)
		delete leg3;
	if(back2)
		delete back2;
}

BOOL CHodajuciTransformerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CHodajuciTransformerView drawing
void LoadIdentity(CDC* pDC)
{
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = 1;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = 1;
	pDC->SetWorldTransform(&a);
}
void Translate(CDC* pDC, float dx, float dy,bool rightMultiply)
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
void Rotate(CDC* pDC, float angle, bool rightMultiply)
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
void Scale(CDC* pDC, float sx,float sy, bool rightMultiply)
{
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = sx;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = sy;
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

void CHodajuciTransformerView::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	Translate(pDC, dx+28, dy+28,false);
	Rotate(pDC, alpha, false);
	leg1->Draw(pDC,CRect(0, 0, leg1->Width()/3, leg1->Height()), CRect(0, 0, leg1->Width()/3, leg1->Height()));
	Translate(pDC, dx + 29-200, dy + 29, false);
	Rotate(pDC, -alpha, false);
	leg2->Draw(pDC, CRect( 0,0,leg2->Width(), leg2->Height()), CRect(-leg2->Width() / 2, -leg2->Height() / 2, leg2->Width(), leg2->Height()));
	Translate(pDC, 148, 40, false);
	//Koordinatni(pDC);
	leg3->Draw(pDC,CRect(0, 0, leg3->Width(), leg3->Height()), CRect(-leg2->Width() / 2, -leg3->Height() / 2, leg3->Width(), leg3->Height()));
	pDC->SetWorldTransform(&stara);
}
void CHodajuciTransformerView::DrawScene(CDC* pDC, CRect rect)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	back2->Draw(pDC, CRect(0, 0, back2->Width(), back2->Height()), rect);
	Translate(pDC,100, 100,false);
	body->Draw(pDC, CRect(0, 0, body->Width(), body->Height()), CRect(0, 0, body->Width(), body->Height()));
	DrawLeg(pDC, 10, -10, 155);
	DrawLeg(pDC, 0, -10, 155);
	DrawLeg(pDC,-20,-40,155);
	DrawLeg(pDC, 20, -40,155);
	Koordinatni(pDC);
	pDC->SetWorldTransform(&stara);
}


void CHodajuciTransformerView::OnDraw(CDC* pDC)
{
	CHodajuciTransformerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	CRect a;
	GetClientRect(a);

	DrawScene(pDC, a);
	
	// TODO: add draw code for native data here
}


// CHodajuciTransformerView printing

BOOL CHodajuciTransformerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHodajuciTransformerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHodajuciTransformerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CHodajuciTransformerView diagnostics

#ifdef _DEBUG
void CHodajuciTransformerView::AssertValid() const
{
	CView::AssertValid();
}

void CHodajuciTransformerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHodajuciTransformerDoc* CHodajuciTransformerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHodajuciTransformerDoc)));
	return (CHodajuciTransformerDoc*)m_pDocument;
}
#endif //_DEBUG


// CHodajuciTransformerView message handlers
