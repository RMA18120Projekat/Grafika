
// IKolokvijum2019View.cpp : implementation of the CIKolokvijum2019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IKolokvijum2019.h"
#endif

#include "IKolokvijum2019Doc.h"
#include "IKolokvijum2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"DImage.h"

// CIKolokvijum2019View

IMPLEMENT_DYNCREATE(CIKolokvijum2019View, CView)

BEGIN_MESSAGE_MAP(CIKolokvijum2019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIKolokvijum2019View construction/destruction

CIKolokvijum2019View::CIKolokvijum2019View() noexcept
{
	// TODO: add construction code here
	arm1 = new DImage();
	arm2 = new DImage();
	background = new DImage();
	leg1 = new DImage();
	leg2 = new DImage();
	body1 = new DImage();
	body1->Load(CString("body1.png"));
	arm1->Load(CString("arm1.png"));
	arm2->Load(CString("arm2.png"));
	background->Load(CString("background.jpg"));
	leg1->Load(CString("leg1.png"));
	leg2->Load(CString("leg2.png"));
	alfa1 = 0;
	alfa2 = 0;
	alfa3 = 0;
	alfa4 = 0;
	alfa5 = 0;



}

CIKolokvijum2019View::~CIKolokvijum2019View()
{
	if (arm1)
		delete arm1;
	if (arm2)
		delete arm2;
	if (leg1)
		delete leg1;
	if (leg2)
		delete leg2;
	if (background)
		delete background;
	if (body1)
		delete body1;




}

BOOL CIKolokvijum2019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKolokvijum2019View drawing
void Koordinatni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);
	pDC->MoveTo(0, 0);
	pDC->LineTo(300, 0);
}
/*void CIKolokvijum2019View::Mem2Pdc(CDC* pDC, CBitmap* source, CBitmap* mask, int w, int h)
{
	CDC* zaPdc = new CDC();
	zaPdc->CreateCompatibleDC(pDC);
	CBitmap*stari=zaPdc->SelectObject(mask);
	pDC->BitBlt((-w/2), (-h/2), w, h, zaPdc, 0, 0, SRCAND);
	zaPdc->SelectObject(source);
	pDC->BitBlt(0, 0, w, h, zaPdc, 0, 0, SRCPAINT);
	zaPdc->SelectObject(&stari);
	delete zaPdc;


}*/
void CIKolokvijum2019View::DrawBackground(CDC* pDC, CRect rect)
{
	background->Draw(pDC, CRect(0, 0, background->Width(), background->Height()), rect);
}
void  CIKolokvijum2019View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	auto b = pImage->GetDIBBits();
	pImage->DrawTransparent(pDC, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()),RGB(b[2],b[1],b[0]));
	
}
void  CIKolokvijum2019View::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
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
void CIKolokvijum2019View::Rotate(CDC* pDC, float ugao, bool rightMultiply)
{
	float u = (ugao * 3.14) / 180;
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = cos(u);
	a.eM12 = sin(u);
	a.eM21 = -sin(u);
	a.eM22 = cos(u);
	if (rightMultiply)
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);

}

void CIKolokvijum2019View::DrawArm1(CDC* pDC)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	Translate(pDC, 34, 31, false);
	Rotate(pDC, alfa1, false);
	DrawImgTransparent(pDC,arm1);
	pDC->SetWorldTransform(&stara);
}
void CIKolokvijum2019View::DrawArm2(CDC* pDC)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	Translate(pDC, 210 , 102 , false);
	Translate(pDC, 10, -30, false);
	Rotate(pDC, alfa2, false);
	//Translate(pDC, -8, -87,false);
	//Koordinatni(pDC);
	DrawImgTransparent(pDC, arm2);
	pDC->SetWorldTransform(&stara);
}
void CIKolokvijum2019View::DrawLeg1(CDC* pDC)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	Translate(pDC, 30, 125, false);
	Translate(pDC, 40, 120, false);
	Rotate(pDC, alfa3-90, false);
	Translate(pDC, -250, -70, false);
	DrawImgTransparent(pDC, leg1);
	//Koordinatni(pDC);
	pDC->SetWorldTransform(&a);
}

void CIKolokvijum2019View::DrawLeg2(CDC* pDC)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	Translate(pDC,35, 60,false);
	Rotate(pDC, 90 + alfa4, false);
	Translate(pDC, 360, -150, false);
	DrawImgTransparent(pDC, leg2);
	//Koordinatni(pDC);
	pDC->SetWorldTransform(&a);
}
void CIKolokvijum2019View::DrawBody(CDC* pDC)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	Translate(pDC, 26, 133, false);
	Rotate(pDC, -90 + alfa5, false);
	Translate(pDC, -180,-50 , false);
	DrawImgTransparent(pDC, body1);
	Koordinatni(pDC);
	pDC->SetWorldTransform(&a);
}
void CIKolokvijum2019View::DrawTransformenr(CDC* pDC)
{
	CRect a;
	GetClientRect(a);
	DrawBackground(pDC, a);
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawLeg1(pDC);
	DrawLeg2(pDC);

}

void CIKolokvijum2019View::OnDraw(CDC* pDC)
{
	CIKolokvijum2019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	DrawTransformenr(pDC);
	
	// TODO: add draw code for native data here
}
void CIKolokvijum2019View::OnKeyDown(UINT a, UINT b, UINT c)
{
	if (a == 65)
	{
		alfa1 += 15;
		alfa3 += 15;
	}
	else if (a == 81)
	{
		alfa1 -= 15;
		alfa3 -= 15;
	}
	else if (a == 84)
	{
		alfa2 -= 15;

	}
	else if (a == 71)
	{
		alfa2 += 15;
	}
	else if (a == 82)
	{
		alfa4 += 15;

	}
	else if (a == 102)
	{
		alfa4 -= 15;
	}
	else if (a == 87)
	{
		alfa5 -= 15;
	}
	else if (a == 83)
	{
		alfa5 += 15;
	}
	Invalidate();
}


// CIKolokvijum2019View printing

BOOL CIKolokvijum2019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKolokvijum2019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKolokvijum2019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIKolokvijum2019View diagnostics

#ifdef _DEBUG
void CIKolokvijum2019View::AssertValid() const
{
	CView::AssertValid();
}

void CIKolokvijum2019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKolokvijum2019Doc* CIKolokvijum2019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKolokvijum2019Doc)));
	return (CIKolokvijum2019Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKolokvijum2019View message handlers
