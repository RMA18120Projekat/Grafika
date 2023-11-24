
// 2017IKolokvijumView.cpp : implementation of the CMy2017IKolokvijumView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "2017IKolokvijum.h"
#endif

#include "2017IKolokvijumDoc.h"
#include "2017IKolokvijumView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"DImage.h"



// CMy2017IKolokvijumView

IMPLEMENT_DYNCREATE(CMy2017IKolokvijumView, CView)

BEGIN_MESSAGE_MAP(CMy2017IKolokvijumView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMy2017IKolokvijumView construction/destruction

CMy2017IKolokvijumView::CMy2017IKolokvijumView() noexcept
{
	// TODO: add construction code here
	leg1 = new DImage();
	leg2 = new DImage();
	leg3 = new DImage();
	back = new DImage();
	body = new DImage();
	leg1->Load(CString("Leg1.png"));
	leg2->Load(CString("Leg2.png"));
	leg3->Load(CString("Leg3.png"));
	back->Load(CString("Back2.jpg"));
	body->Load(CString("Body.png"));
	offset = 0;
	ratio = 1;
	pomerajX = 0;
	pomerajY = 0;
	imgW = back->Width();
	imgH = back->Height();
	

}

CMy2017IKolokvijumView::~CMy2017IKolokvijumView()
{
	if (leg1)
		delete leg1;

	if (leg2)
		delete leg2;

	if (leg3)
		delete leg3;

	if (body)
		delete body;

	if (back)
		delete back;
}

BOOL CMy2017IKolokvijumView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMy2017IKolokvijumView drawing
void CMy2017IKolokvijumView::LoadIdentity(CDC* pDC)
{
	XFORM a = {
		1,0,
		0,0,
		0,1,
		
	};
	

}
void CMy2017IKolokvijumView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM a;
	a.eDx = dX;
	a.eDy = dY;
	a.eM11 = 1;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = 1;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);

}
void CMy2017IKolokvijumView::Rotate(CDC* pDC, float angle, bool rightMultiply)
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
void CMy2017IKolokvijumView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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
void Koordinatni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(300, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);
}
void CMy2017IKolokvijumView::DrawTransparent(CDC* pDC, DImage*i)
{
	CBitmap izvor;
	CBitmap maska;
	CDC* ic = new CDC();
	CDC* mc = new CDC();
	izvor.CreateCompatibleBitmap(pDC,i->Width(),i->Height());
	maska.CreateBitmap(i->Width(), i->Height(), 1, 1, NULL);
	ic->CreateCompatibleDC(pDC);
	mc->CreateCompatibleDC(pDC);
	CBitmap* a1 = ic->SelectObject(&izvor);
	CBitmap* a2 = mc->SelectObject(&maska);
	i->Draw(ic, CRect(0, 0, body->Width(), body->Height()), CRect(0, 0, body->Width(), body->Height()));
	COLORREF topLeft = ic->GetPixel(0, 0);
	COLORREF pozadina = ic->SetBkColor(topLeft);
	COLORREF text = ic->SetTextColor(RGB(255, 255, 255));
	mc->BitBlt(0, 0, i->Width(), i->Height(), ic, 0, 0, SRCCOPY);
	ic->SetBkColor(RGB(0, 0, 0));
	ic->BitBlt(0, 0, i->Width(), i->Height(), mc, 0, 0, SRCAND);
	mc->SetTextColor(text);
	ic->SetBkColor(pozadina);
	mc->SelectObject(&a2);
	ic->SelectObject(&a1);
	delete mc;
	delete ic;
	CDC* l = new CDC();
	l->CreateCompatibleDC(pDC);
	CBitmap* a3 = l->SelectObject(&maska);
	pDC->BitBlt(0, 0, i->Width(), i->Height(), l, 0, 0, SRCAND);
	l->SelectObject(&izvor);
	pDC->BitBlt(0, 0, i->Width(), i->Height(), l, 0, 0, SRCPAINT);
	l->SelectObject(&a3);
	delete l;
}
void CMy2017IKolokvijumView::DrawScene(CDC* pDC, CRect rect)
{
	GetClientRect(&rectC);
	back->Draw(pDC, CRect(pomerajX, pomerajY, back->Width(), back->Height()), rect);
	XFORM a;
	pDC->GetWorldTransform(&a);
	Translate(pDC,100+offset, 100,false);
	Scale(pDC, ratio, ratio, false);
	//auto b = body->GetDIBBits();
	DrawTransparent(pDC, body);
	DrawLeg(pDC,20*seq[ind[0]], 50, 178);
	DrawLeg(pDC, 20*seq[ind[1]], 50, 178);
	DrawLeg(pDC, 20*seq[ind[2]], 265, 178);
	DrawLeg(pDC, 20*seq[ind[3]], 265, 178);

}
void CMy2017IKolokvijumView::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{

	XFORM a;
	pDC->GetWorldTransform(&a);
	Translate(pDC, dx, dy, false);
	auto b = body->GetDIBBits();
	Translate(pDC, 28, 28, false);
	Rotate(pDC, alpha, false);
	Translate(pDC, -28, -28, false);
	DrawTransparent(pDC, leg1);
	Translate(pDC, 0, 140, false);
	Translate(pDC, 29, 29, false);
	Rotate(pDC, -alpha, false);
	Translate(pDC, -29, -29, false);
	DrawTransparent(pDC, leg2);
	Translate(pDC, -30, 100, false);
	leg3->DrawTransparent(pDC, CRect(0, 0, leg3->Width(), leg3->Height()), CRect(0, 0, leg3->Width(), leg3->Height()), RGB(b[2], b[1], b[0]));
	pDC->SetWorldTransform(&a);
	//Koordinatni(pDC);


}
void CMy2017IKolokvijumView::OnDraw(CDC* pDC)
{
	CMy2017IKolokvijumDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	CRect a;
	GetClientRect(&a);
	CBitmap mapa;
	mapa.CreateCompatibleBitmap(pDC, a.Width(), a.Height());
	CDC* m = new CDC();
	m->CreateCompatibleDC(pDC);
	m->SetGraphicsMode(GM_ADVANCED);
	CBitmap* stara = m->SelectObject(&mapa);
	DrawScene(pDC, a);
	pDC->BitBlt(0, 0, a.Width(), a.Height(), m, 0, 0, SRCCOPY);
	m->SelectObject(&stara);
	delete m;

	// TODO: add draw code for native data here
}
void CMy2017IKolokvijumView::OnKeyDown(UINT a, UINT b, UINT c)
{
	switch (a)
	{
	case VK_RIGHT:
		for (int i = 0;i < 4;i++)
			ind[i]=(ind[i+1])%8;
		offset += 10;
		Invalidate();
		break;
	case VK_UP:
		ratio += 0.1;
		Invalidate();
		break;
	case VK_DOWN:
		ratio -= 0.1;
		Invalidate();
		break;
	case 'A':
		if (pomerajX >= 200)
			pomerajX -= 200;
		Invalidate();
		break;
	case 'D':
		if (rectC.Width() + pomerajX + 200 <= imgW)
			pomerajX += 200;
		Invalidate();
		break;
	case'W':
		if (pomerajY >= 200)
			pomerajY -= 200;
		Invalidate();
		break;
	case'S':
		if (rectC.Height() + pomerajY + 200 <= imgH)
			pomerajY += 200;
		Invalidate();
		break;
	default:
		break;
	}

}


// CMy2017IKolokvijumView printing

BOOL CMy2017IKolokvijumView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy2017IKolokvijumView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy2017IKolokvijumView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMy2017IKolokvijumView diagnostics

#ifdef _DEBUG
void CMy2017IKolokvijumView::AssertValid() const
{
	CView::AssertValid();
}

void CMy2017IKolokvijumView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2017IKolokvijumDoc* CMy2017IKolokvijumView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2017IKolokvijumDoc)));
	return (CMy2017IKolokvijumDoc*)m_pDocument;
}
#endif //_DEBUG


// CMy2017IKolokvijumView message handlers


BOOL CMy2017IKolokvijumView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}
