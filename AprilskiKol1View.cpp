
// AprilskiKol1View.cpp : implementation of the CAprilskiKol1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "AprilskiKol1.h"
#endif

#include "AprilskiKol1Doc.h"
#include "AprilskiKol1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include"DImage.h"


// CAprilskiKol1View

IMPLEMENT_DYNCREATE(CAprilskiKol1View, CView)

BEGIN_MESSAGE_MAP(CAprilskiKol1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAprilskiKol1View construction/destruction

CAprilskiKol1View::CAprilskiKol1View() noexcept
{
	// TODO: add construction code here

}

CAprilskiKol1View::~CAprilskiKol1View()
{
}

BOOL CAprilskiKol1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CAprilskiKol1View drawing
void CAprilskiKol1View::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
{
	XFORM a;
	a.eDx = dx;
	a.eDy = dy;
	a.eM11 = 1;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = 1;
	if (rightMultiply)
	{
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	}
	else
	{
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);
	}

}
void CAprilskiKol1View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float radiani = (angle * 3.14) / 180;
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = cos(radiani);
	a.eM12 = sin(radiani);
	a.eM21 = -sin(radiani);
	a.eM22 = cos(radiani);
	if (rightMultiply)
	{
		pDC->ModifyWorldTransform(&a, MWT_RIGHTMULTIPLY);
	}
	else
	{
		pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);
	}


}
void Koordinatni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 90);
	pDC->MoveTo(0, 0);
	pDC->LineTo(90, 0);
}
void CAprilskiKol1View::DrawBlade(CDC* pDC,int size)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	//Translate(pDC, 700, 200, false);
	//Koordinatni(pDC);
	CPen olovka(PS_SOLID,3, RGB(128, 128, 128));
	CPen* staraO = pDC->SelectObject(&olovka);
	CBrush svetla(RGB(211, 211, 211));
	CBrush* staraC = pDC->SelectObject(&svetla);
	POINT tacke1[] = {{0,0},{4 * size,0},{(size), -(size)}};
	pDC->Polygon(tacke1, 3);
	CBrush tamna(RGB(169, 169, 169));
	pDC->SelectObject(&tamna);
	POINT tacke2[] = { {0,0},{(4 * size),0},{size,size} };
	pDC->SetBkMode(TRANSPARENT);
	CFont font;
	font.CreateFontW(0.7 * size, 0, 0, 0, FW_NORMAL,                  // Debljina fonta (FW_BOLD za podebljano)
		FALSE,                      // Isključivanje kurziva
		FALSE,                      // Isključivanje podebljavanja
		0,                          // Podebljavanje (ignorirat će se)
		ANSI_CHARSET,               // Skup znakova (ANSI_CHARSET, DEFAULT_CHARSET, ...)
		OUT_DEFAULT_PRECIS,         // Preciznost izlaznog uređaja
		CLIP_DEFAULT_PRECIS,        // Preciznost obrezivanja
		DEFAULT_QUALITY,            // Kvaliteta izlaznog uređaja
		DEFAULT_PITCH | FF_DONTCARE, // Razmak između znakova i oblik fonta
		_T("Monotype Cursiva"));
	pDC->Polygon(tacke2, 3);
	CFont* stari = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(255, 255, 255));
	Translate(pDC,0.7*size,-0.3*size,false);
	//Koordinatni(pDC);
	pDC->TextOutW(-2, -2, CString("Shuricane"));
	pDC->SetTextColor(RGB(200, 200, 200));
	pDC->TextOutW(0, 0, CString("Shuricane"));
	pDC->SelectObject(&staraC);
	pDC->SelectObject(&staraO);
	pDC->SetWorldTransform(&stara);
	pDC->SelectObject(&stari);
}
void CAprilskiKol1View::DrawStar(CDC* pDC, int size,float angle,float dx)
{
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	Translate(pDC, 200, 200, false);
	Translate(pDC, dx, 0, false);
	Rotate(pDC, angle, false);
	//Koordinatni(pDC);
	for (int i = 0;i < 4;i++)
	{
		DrawBlade(pDC, size);
		Rotate(pDC, 90, false);
	

	}
	pDC->SetWorldTransform(&stara);
}
void CAprilskiKol1View::DrawScene(CDC* pDC)
{
	// Dobivanje dimenzija prozora
	CRect client;
	GetClientRect(client);
	CBitmap mapa;
	mapa.CreateCompatibleBitmap(pDC,client.Width(),client.Height());
	CDC* cdc = new CDC();
	cdc->CreateCompatibleDC(pDC);
	CBitmap* stara = cdc->SelectObject(&mapa);
	DImage nebo;
	nebo.Load(CString("CieloBBC6.bmp"));

	
	// Iscrtaj nebo koje zauzima cijeli prozor
	nebo.Draw(cdc, CRect(0, 0, nebo.Width(), nebo.Height()), CRect(client.left, client.top, client.right, client.bottom));

	// Učitaj teksturu
	DImage textura;
	textura.Load(CString("DoorsMedieval0466_1_L.jpg"));

	// Iscrtaj teksturu koja zauzima posljednju trećinu širine ekrana i cijelu visinu ekrana
	CRect texturaRect(client.left + 2 * client.right / 3, client.top, client.right, client.bottom);
	textura.Draw(cdc,  CRect(0, 0, textura.Width(), textura.Height()),texturaRect);
	pDC->BitBlt(0, 0, client.right, client.bottom, cdc, 0, 0, SRCCOPY);
	cdc->SelectObject(&stara);
	DImage* snimi = new DImage(mapa);
	snimi->Save(CString("bitmapa"));
	delete cdc;
	delete snimi;
}

void CAprilskiKol1View::OnDraw(CDC* pDC)
{
	CAprilskiKol1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	//DrawBlade(pDC, 40);
	DrawScene(pDC);
	DrawStar(pDC, 35,-80,700);

	// TODO: add draw code for native data here
}


// CAprilskiKol1View printing

BOOL CAprilskiKol1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAprilskiKol1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAprilskiKol1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CAprilskiKol1View diagnostics

#ifdef _DEBUG
void CAprilskiKol1View::AssertValid() const
{
	CView::AssertValid();
}

void CAprilskiKol1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAprilskiKol1Doc* CAprilskiKol1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAprilskiKol1Doc)));
	return (CAprilskiKol1Doc*)m_pDocument;
}
#endif //_DEBUG


// CAprilskiKol1View message handlers
