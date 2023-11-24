
// IKolokvijum2018View.cpp : implementation of the CIKolokvijum2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IKolokvijum2018.h"
#endif

#include "IKolokvijum2018Doc.h"
#include "IKolokvijum2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"DImage.h"


// CIKolokvijum2018View

IMPLEMENT_DYNCREATE(CIKolokvijum2018View, CView)

BEGIN_MESSAGE_MAP(CIKolokvijum2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIKolokvijum2018View construction/destruction

CIKolokvijum2018View::CIKolokvijum2018View() noexcept
{
	// TODO: add construction code here
	bager = new DImage();
	arm1 = new DImage();
	arm2 = new DImage();
	pozadina = new DImage();
	viljuska = GetEnhMetaFileW(CString("viljuska.emf"));
	bager->Load(CString("bager.png"));
	arm1->Load(CString("arm1.png"));
	arm2->Load(CString("arm2.png"));
	pozadina->Load(CString("pozadina.png"));
	alfa1 = 0;
	alfa2 = 0;
	alfa3 = 0;
	offset = 0;



}

CIKolokvijum2018View::~CIKolokvijum2018View()
{
	if (bager)
		delete bager;
	if (arm1)
		delete arm1;

	if (arm2)
		delete arm2;
	if (pozadina)
		delete pozadina;
	DeleteEnhMetaFile(viljuska);
	
}

BOOL CIKolokvijum2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIKolokvijum2018View drawing
void Translate(CDC* pDC,float dx,float dy,bool rightMultiply)
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
void CIKolokvijum2018View::DrawBackground(CDC* pDC)
{
	CRect client;
	GetClientRect(client);
	int w = pozadina->Width() - client.Width();
	int h = pozadina->Height() - client.Height();
	pozadina->Draw(pDC, CRect(w/2,h, w/2+client.Width(), pozadina->Height()), CRect(0, 0, client.Width(),client.Height()));



}
void CIKolokvijum2018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CBitmap source;
	CBitmap mask;
	CDC* sC = new CDC();
	CDC* mC = new CDC();
	sC->CreateCompatibleDC(pDC);
	mC->CreateCompatibleDC(pDC);
	source.CreateCompatibleBitmap(pDC, pImage->Width(), pImage->Height());
	mask.CreateBitmap(pImage->Width(), pImage->Height(), 1, 1, NULL);
	CBitmap* a1 = sC->SelectObject(&source);
	CBitmap* a2 = mC->SelectObject(&mask);
	pImage->Draw(sC, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()));
	COLORREF topLeft = sC->GetPixel(0, 0);
	COLORREF pozadina = sC->SetBkColor(topLeft);
	COLORREF text = sC->SetTextColor(RGB(255, 255, 255));
	mC->BitBlt(0, 0, pImage->Width(), pImage->Height(), sC, 0, 0, SRCCOPY);
	sC->SetBkColor(RGB(0, 0, 0));
	sC->BitBlt(0, 0, pImage->Width(), pImage->Height(), mC, 0, 0, SRCAND);
	mC->SetTextColor(pozadina);
	sC->SetBkColor(text);
	
	sC->SelectObject(&a1);
	mC->SelectObject(&a2);
	delete sC;
	delete mC;
	CDC* m = new CDC();
	m->CreateCompatibleDC(pDC);
	CBitmap*a3=m->SelectObject(&mask);
	pDC->BitBlt(0, 0, pImage->Width(), pImage->Height(), m, 0, 0, SRCAND);
	m->SelectObject(&source);
	pDC->BitBlt(0, 0, pImage->Width(), pImage->Height(), m, 0, 0, SRCPAINT);
	m->SelectObject(&a3);
	delete m;

}
void Koordinatni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(100, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 30);
}
void CIKolokvijum2018View::DrawBody(CDC* pDC)
{
	CRect a;
	GetClientRect(a);
	Translate(pDC, a.right-bager->Width(), a.bottom-bager->Height(), false);
	DrawImgTransparent(pDC, bager);
	Translate(pDC, 0, 250, false);
	//Koordinatni(pDC);

}
void CIKolokvijum2018View::DrawArm1(CDC* pDC)
{
	Rotate(pDC, -90, false);
	Translate(pDC, 58, 61, false);
	Rotate(pDC, alfa1, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, arm1);
	Translate(pDC, 309,61 ,false);
	//Koordinatni(pDC);
	
}
void CIKolokvijum2018View::DrawArm2(CDC* pDC)
{
	Rotate(pDC, -90, false);
	Translate(pDC, -35, -35, false);
	Translate(pDC, 36, 40, false);
	Rotate(pDC, alfa2, false);
	Translate(pDC, -36, -40, false);
	//Translate(pDC, 35, 35, false);
	DrawImgTransparent(pDC, arm2);
	Translate(pDC, 200, 80, false);
	//Koordinatni(pDC);
	
}
void CIKolokvijum2018View::DrawFork(CDC* pDC)
{
	Scale(pDC, 2.5, 2.5, false);
	Rotate(pDC, 90, false);
	Translate(pDC, -15, -35, false);
	ENHMETAHEADER a;
	GetEnhMetaFileHeader(viljuska, sizeof(ENHMETAHEADER), &a);
	Translate(pDC, 0, 7, false);
	Rotate(pDC, alfa3, false);
	Translate(pDC, 0, -7, false);
	//Koordinatni(pDC);
	pDC->PlayMetaFile(viljuska, CRect(a.rclBounds.left,a.rclBounds.top,a.rclBounds.right,a.rclBounds.bottom));
	Scale(pDC, 1 / 2.5, 1 / 2.5, false);

	//Koordinatni(pDC);


}

void CIKolokvijum2018View::DrawExcavator(CDC* pDC)
{
	pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	Translate(pDC, -offset, 0, false);
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
	pDC->SetWorldTransform(&stara);

}
void CIKolokvijum2018View::OnDraw(CDC* pDC)
{
	CIKolokvijum2018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(&client);
	CDC* memory = new CDC();
	CBitmap map;
	map.CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	memory->CreateCompatibleDC(pDC);
	CBitmap* stara = memory->SelectObject(&map);
	DrawBackground(memory);

	DrawExcavator(memory);
	pDC->BitBlt(0, 0, client.Width(), client.Height(),memory,0,0,SRCCOPY);
	memory->SelectObject(&stara);
	delete memory;
	// TODO: add draw code for native data here
}
void  CIKolokvijum2018View::OnKeyDown(UINT a, UINT b, UINT c)
{
	switch (a)
	{
	case '1':
		alfa1 -= 10;
		Invalidate();
		break;
	case'2':
		alfa1 += 10;
		Invalidate();
		break;
	case '3':
		alfa2 -= 10;
		Invalidate();
		break;
	case'4':
		alfa2 += 10;
		Invalidate();
		break;
	case'5':
		alfa3 -= 10;
		Invalidate();
		break;
	case'7':
		alfa3 += 10;
		Invalidate();
		break;
	case VK_LEFT:
		offset += 10;
		Invalidate();
		break;
	case VK_RIGHT:
		offset -= 10;
		Invalidate();
		break;

	default:
		break;
	}

}

// CIKolokvijum2018View printing

BOOL CIKolokvijum2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIKolokvijum2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIKolokvijum2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIKolokvijum2018View diagnostics

#ifdef _DEBUG
void CIKolokvijum2018View::AssertValid() const
{
	CView::AssertValid();
}

void CIKolokvijum2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIKolokvijum2018Doc* CIKolokvijum2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIKolokvijum2018Doc)));
	return (CIKolokvijum2018Doc*)m_pDocument;
}
#endif //_DEBUG


// CIKolokvijum2018View message handlers
