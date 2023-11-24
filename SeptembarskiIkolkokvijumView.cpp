
// SeptembarskiIkolkokvijumView.cpp : implementation of the CSeptembarskiIkolkokvijumView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "SeptembarskiIkolkokvijum.h"
#endif

#include "SeptembarskiIkolkokvijumDoc.h"
#include "SeptembarskiIkolkokvijumView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSeptembarskiIkolkokvijumView

IMPLEMENT_DYNCREATE(CSeptembarskiIkolkokvijumView, CView)

BEGIN_MESSAGE_MAP(CSeptembarskiIkolkokvijumView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CSeptembarskiIkolkokvijumView construction/destruction

CSeptembarskiIkolkokvijumView::CSeptembarskiIkolkokvijumView() noexcept
{
	// TODO: add construction code here

}

CSeptembarskiIkolkokvijumView::~CSeptembarskiIkolkokvijumView()
{
}

BOOL CSeptembarskiIkolkokvijumView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
void Transform(CDC* pDC, XFORM* a)
{
	pDC->ModifyWorldTransform(a, MWT_LEFTMULTIPLY);
}
void CSeptembarskiIkolkokvijumView::Translate(CDC* pDC, float dx, float dy)
{
	XFORM matrica;
	matrica.eDx = dx;
	matrica.eDy = dy;
	matrica.eM11 = 1;
	matrica.eM12 = 0;
	matrica.eM21 = 0;
	matrica.eM22 = 1;
	Transform(pDC, &matrica);
}
void CSeptembarskiIkolkokvijumView::Rotate(CDC* pDC, float angle)
{
	float radian = (angle * 3.14) / 180;
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = cos(radian);
	a.eM12 = sin(radian);
	a.eM21 = -sin(radian);
	a.eM22 = cos(radian);
	Transform(pDC, &a);
}
void Koordinantni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);
	pDC->MoveTo(0, 0);
	pDC->LineTo(100, 0);
}
void CSeptembarskiIkolkokvijumView::Mirror(CDC* pDC, bool mx, bool my)
{
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = mx ? 1 : 1;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = my ? 1 : -1;
	Transform(pDC, &a);
}

// CSeptembarskiIkolkokvijumView drawing
void CSeptembarskiIkolkokvijumView::DrawFilmReel(CDC* pDC, int r, int n, int dBig, int rBig, int dSmall, int rSmall, COLORREF colorFill, int wLine, COLORREF colLine,double angle,int nText, CString strText[],int nSpacing[],int hFont, COLORREF colText)
{
	XFORM matrica;
	pDC->GetWorldTransform(&matrica);
	CPen olovka( PS_SOLID,wLine,colLine);
	CPen* staraO = pDC->SelectObject(&olovka);
	CBrush cetka(colorFill);
	CBrush* staraC = pDC->SelectObject(&cetka);
	Translate(pDC, 3.5*r, 1.5*r);
	Rotate(pDC, angle);
	//Koordinantni(pDC);
	float gx = 0 - r;
	float gy = 0 - r;
	pDC->Ellipse(gx, gy, 2 * r + gx, 2 * r + gy);
	for (int i = 0;i < n;i++)
	{
		Rotate(pDC,60);
		Translate(pDC, 0, dBig);
		//Koordinantni(pDC);
		float g1x = 0 - rBig;
		float g1y = 0 - rBig;
		CBrush cetkaB(RGB(255, 255, 255));
		pDC->SelectObject(&cetkaB);
		pDC->Ellipse(g1x, g1y, 2 * rBig + g1x, 2 * rBig + g1y);
	
		Translate(pDC, 0, -dBig);

	}
	for (int i = 0;i < 4;i++)
	{
		Rotate(pDC, 90);
		Translate(pDC, 0, dSmall);
		//Koordinantni(pDC);
		float g1x = 0 - rSmall;
		float g1y = 0 - rSmall;
		CBrush cetkaB(RGB(255, 255, 255));
		pDC->SelectObject(&cetkaB);
		pDC->Ellipse(g1x, g1y, 2 * rSmall + g1x, 2 * rSmall + g1y);

		Translate(pDC, 0, -dSmall);

	}
	//Koordinantni(pDC);
	pDC->SetWorldTransform(&matrica);
	Translate(pDC, 3.5 * r, 1.5 * r);
	//Koordinantni(pDC);
	CFont font;
	font.CreateFontW(hFont, 0, 0, 0,900, 1, 0, 0, 0, 0, 0, 0, 0, _T("Tahoma"));
	CFont* stari = pDC->SelectObject(&font);
	int textAlign = TA_RIGHT | TA_TOP;
	pDC->SetTextAlign(textAlign);
	Translate(pDC, -1.2 * r, gy);
	pDC->MoveTo(0, 0);
	pDC->SetTextColor(colText);
	pDC->TextOutW(0, 0, strText[0]);
	int j = 0;
	for (int i = 1;i < nText;i++)
	{
		Translate(pDC, 0, nSpacing[j]);
		j++;
		pDC->MoveTo(0, 0);
		pDC->TextOutW(0, 0, strText[i]);



	}
	pDC->SetWorldTransform(&matrica);
	pDC->SelectObject(&stari);
	pDC->SelectObject(&staraC);
	pDC->SelectObject(&staraO);

}

void CSeptembarskiIkolkokvijumView:: SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp, CString sfile)
{
	CBitmap destination;
	destination.CreateCompatibleBitmap(pDC, rcBmp.Width(), rcBmp.Height());
	CDC* destinationC = new CDC();
	destinationC->CreateCompatibleDC(pDC);
	CBitmap* stara = destinationC->SelectObject(&destination);
	destinationC->BitBlt(0, 0, rcDC.Width(), rcDC.Height(), pDC, 0, 0, SRCCOPY);
	//destination.Save(sfile);
	destinationC->SelectObject(&stara);
	delete destinationC;
}

void CSeptembarskiIkolkokvijumView::OnDraw(CDC* pDC)
{
	CSeptembarskiIkolkokvijumDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	CString niz[3];
	niz[0] = CString("FILMSKI");
	niz[1] = CString("SUSRETI");
	niz[2] = CString("NIS");
	int razmak[] = { 60,160 };
	SaveDC(pDC, CRect(0, 0, 300, 300), CRect(0, 0, 300, 300), CString("Mapa"));
	DrawFilmReel(pDC, 140, 6, 60, 20, 20, 10, RGB(255, 0, 0), 1, RGB(0, 255, 0), 30 ,3, niz, razmak, 40, RGB(120, 120, 120));
	// TODO: add draw code for native data here
}


// CSeptembarskiIkolkokvijumView printing

BOOL CSeptembarskiIkolkokvijumView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSeptembarskiIkolkokvijumView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSeptembarskiIkolkokvijumView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CSeptembarskiIkolkokvijumView diagnostics

#ifdef _DEBUG
void CSeptembarskiIkolkokvijumView::AssertValid() const
{
	CView::AssertValid();
}

void CSeptembarskiIkolkokvijumView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSeptembarskiIkolkokvijumDoc* CSeptembarskiIkolkokvijumView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSeptembarskiIkolkokvijumDoc)));
	return (CSeptembarskiIkolkokvijumDoc*)m_pDocument;
}
#endif //_DEBUG


// CSeptembarskiIkolkokvijumView message handlers
