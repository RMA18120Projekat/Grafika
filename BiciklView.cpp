
// BiciklView.cpp : implementation of the CBiciklView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Bicikl.h"
#endif

#include "BiciklDoc.h"
#include "BiciklView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include"DImage.h"
// CBiciklView

IMPLEMENT_DYNCREATE(CBiciklView, CView)

BEGIN_MESSAGE_MAP(CBiciklView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CBiciklView construction/destruction

CBiciklView::CBiciklView() noexcept
{
	// TODO: add construction code here
	offset= 0;
	start = 0.0;

}

CBiciklView::~CBiciklView()
{
}

BOOL CBiciklView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
void CBiciklView::Translate(CDC* pDC, float dx, float dy, bool rightMultiply)
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
void CBiciklView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	double r = (angle * 3.14) / 180;
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

// CBiciklView drawing
void Koordinatne(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 100);
	pDC->MoveTo(0, 0);
	pDC->LineTo(300, 0);
}
void CBiciklView::DrawCycle(CDC* pDC, int r, int n, int d, COLORREF clr)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	CPen olovka(PS_SOLID, d, clr);
	CBrush cetka(clr);
	CPen* staraO = pDC->SelectObject(&olovka);
	CRgn osovina;
	int rOsovine = 2 * d;
	int Gx = 0 - rOsovine;
	int Gy = 0 - rOsovine;
	int Vx = 0 - r;
	int Vy = 0 - r;
	CBrush* staraC = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->Ellipse(Vx, Vy, Vx + 2 * r, Vy + 2 * r);
	 pDC->SelectObject(&cetka);
	osovina.CreateEllipticRgn(Gx, Gy, Gx + 2 * rOsovine, Gy + 2 * rOsovine);
	pDC->FillRgn(&osovina, &cetka);
	CPen sitnija(PS_SOLID, 1, clr);
	pDC->SelectObject(&sitnija);
	//Koordinatne(pDC);
	float alfa = 360 / n;
	for (int i = 0;i < n;i++)
	{
		
		pDC->GetWorldTransform(&a);
		Rotate(pDC, alfa * i, false);
		pDC->MoveTo(0, 0);
		pDC->LineTo(0, r);
		pDC->SetWorldTransform(&a);

	}
	pDC->SelectObject(&staraO);
	pDC->SelectObject(&staraC);
	pDC->SetWorldTransform(&a);


}
void CBiciklView::DrawFrame(CDC* pDC, int r, int d, COLORREF clr)
{

	CPen olovka(PS_SOLID, d, clr);
	CBrush cetka(clr);
	CPen* staraO = pDC->SelectObject(&olovka);
	CBrush* staraC = pDC->SelectObject(&cetka);

	//Koordinatne(pDC);
	pDC->BeginPath();
	{
		pDC->MoveTo(0, 0);
		pDC->LineTo(0, -1.4 * r);
		pDC->MoveTo(0, -1.4 * r);
		pDC->LineTo(0.2 * r, -1.4*r);
		//pDC->MoveTo(0, -1.2*r);
		
		//pDC->MoveTo(0, -0.2 * r);


	}
	pDC->EndPath();
	pDC->StrokeAndFillPath();
	CBrush* stara = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	pDC->BeginPath();
	{
		pDC->MoveTo(0, -1.2 * r);
		//pDC->Rectangle(-1.2*r, -1.2 * r, 1.2 * r, 1.2*r);
		pDC->SetArcDirection(AD_CLOCKWISE);
		POINT a1 = { 0,-1.2* r };
		POINT a2 = { 1.2 * r,0 };
		pDC->ArcTo(CRect(-1.2*r, -1.2 * r, 1.2 * r, 1.2*r), a1, a2);
		float x = 1.2 * r;
		float y = 0;
		pDC->LineTo(x + 0.2 * r, 0);
		pDC->MoveTo(x, y);
		//pDC->Rectangle(x, y-0.75*r, x + 1.5 * r, y + 0.75 * r);
		pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
		POINT a3 = { x,y };
		POINT a4 = { x + 0.75 * r ,y + 0.75 * r };
		pDC->Arc(CRect(x, y - 0.75 * r, x + 1.5 * r, y + 0.75 * r), a3, a4);
	
	}
	pDC->EndPath();
	pDC->StrokePath();
	pDC->SelectObject(&staraO);
	pDC->SelectObject(&staraC);

}
void CBiciklView::DrawSeat(CDC* pDC, int r, int d, float beta, COLORREF clr)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	CPen olovka(PS_SOLID, d, clr);
	CPen* staraO = pDC->SelectObject(&olovka);
	Translate(pDC,  0.2*r, -1.2*r,false);
	//Koordinatne(pDC);
	Rotate(pDC, beta, false);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0.3 * r, 0);
	Translate(pDC, 0.3 * r, 0, false);
	Rotate(pDC, -beta, false);
	pDC->BeginPath();
	{
		pDC->MoveTo(0, 0);
		pDC->LineTo(-0.2 * r, 0);
		pDC->MoveTo(0, 0);
		pDC->LineTo(0.1 * r, 0);
	}
	pDC->EndPath();
	pDC->StrokePath();
	pDC->SelectObject(&staraO);
	
	
	pDC->SetWorldTransform(&a);

}
void CBiciklView::DrawPedals(CDC* pDC, int r, int d, float alpha, COLORREF clr)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	CPen olovka(PS_SOLID, d, clr);
	CPen* staraO = pDC->SelectObject(&olovka);
	Rotate(pDC, alpha, false);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, -0.3 * r);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 0.3 * r);
	pDC->SelectObject(&staraO);
	Translate(pDC,0, -0.3 * r,false);
	Rotate(pDC, -alpha, false);
	pDC->MoveTo(0, 0);
	pDC->LineTo(-0.1 * r, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0.1 * r, 0);
	Rotate(pDC, alpha, false);
	Translate(pDC, 0, 2*0.3 * r, false);
	Rotate(pDC, -alpha, false);
	pDC->MoveTo(0, 0);
	pDC->LineTo(-0.1 * r, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0.1 * r, 0);

	pDC->SetWorldTransform(&a);



}
void CBiciklView::DrawBicycle(CDC* pDC, int r, int n, int d, float beta, COLORREF clr, float omega, float time)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	float alpha = -omega * time;
	Rotate(pDC, alpha, false);
	DrawCycle(pDC, r, n, d,clr);
	Rotate(pDC, -alpha, false);
	DrawSeat(pDC, r, d, beta, clr);
	DrawPedals(pDC, r, d, alpha, clr);
	DrawFrame(pDC, r, d,clr);
	Translate(pDC, (1.4 + 0.75 - 0.25) * r, 0.75 * r, false);
	Rotate(pDC, 4*alpha, false);
	DrawCycle(pDC, 0.25*r, n, d, clr);

	pDC->SetWorldTransform(&a);
}
void CBiciklView::DrawTrack(CDC* pDC, CRect rcTruck, int lineWidth, COLORREF clrSky, COLORREF clrGrass, COLORREF clrTrack)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	CBrush cetka1(clrSky);
	CBrush* staraC = pDC->SelectObject(&cetka1);
	pDC->Rectangle(0,0,rcTruck.Width(), 0.4*rcTruck.Height());
	CBrush cetka2(clrGrass);
	pDC->SelectObject(&cetka2);
	pDC->Rectangle(0, 0.4*rcTruck.Height(), rcTruck.Width(), 0.4 * rcTruck.Height()+ 0.2 * rcTruck.Height());
	CBrush cetka3(clrTrack);
	pDC->SelectObject(&cetka3);
	pDC->Rectangle(0, 0.4 * rcTruck.Height() + 0.2 * rcTruck.Height(), rcTruck.Width(), 0.4 * rcTruck.Height() + 0.2 * rcTruck.Height()+0.4*rcTruck.Height());
	pDC->SelectStockObject(NULL_BRUSH);
	CPen olovka(PS_SOLID, lineWidth, RGB(255, 255, 255));
	CPen* staraO = pDC->SelectObject(&olovka);
	pDC->MoveTo(0, 0.4 * rcTruck.Height() + 0.2 * rcTruck.Height());
	pDC->LineTo(rcTruck.Width(), 0.4 * rcTruck.Height() + 0.2 * rcTruck.Height());
	pDC->MoveTo(0, 0.4 * rcTruck.Height() + 0.2 * rcTruck.Height()+ 0.4 * rcTruck.Height());
	pDC->LineTo(rcTruck.Width(), 0.4 * rcTruck.Height() + 0.2 * rcTruck.Height()+ 0.4 * rcTruck.Height());
	pDC->MoveTo(0, 0.4 * rcTruck.Height() + 0.2 * rcTruck.Height() + 0.2 * rcTruck.Height());
	for (int i = 0;i < rcTruck.Width();i+=rcTruck.Width()/10)
	{
		pDC->MoveTo(i, 0.4 * rcTruck.Height() + 0.2 * rcTruck.Height() + 0.2 * rcTruck.Height());
		pDC->LineTo(i + rcTruck.Width() / 10, 0.4 * rcTruck.Height() + 0.2 * rcTruck.Height() + 0.2 * rcTruck.Height());
		i+=rcTruck.Width() / 11;
	}
	pDC->SelectObject(&staraC);
	pDC->SelectObject(&staraO);
	pDC->SetWorldTransform(&a);

}
void CBiciklView::DrawSignature(CDC* pDC, CRect rcTrack, int fontHeight, CString strText, COLORREF clrText)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	CFont font;
	font.CreateFontW(fontHeight, 0, 900, 900, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(clrText);
	Translate(pDC, rcTrack.right-fontHeight/8, rcTrack.bottom-2*fontHeight,false);
	Rotate(pDC, -90, false);
	int align = TA_RIGHT|TA_BOTTOM ;
	int old=pDC->SetTextAlign(align);
	pDC->TextOut(0,0,strText);
	pDC->SetTextAlign(old);
	pDC->SetWorldTransform(&a);
	CFont*stari=pDC->SelectObject(&font);
	pDC->SelectObject(&stari);
}
void CBiciklView::DrawBicycleOnTrack(CDC* pDC, CRect rcTrack, int r, int n, int d, float beta,COLORREF clr,COLORREF clrSky, COLORREF clrGrass,COLORREF clrTrack, CString strText, float omega, float time)
{
	DrawTrack(pDC, rcTrack, rcTrack.Height()/30, RGB(173, 216, 230), RGB(0, 255, 0), RGB(211, 211, 211));
	DrawSignature(pDC, rcTrack, rcTrack.Height() / 15, strText, clr);
	Translate(pDC, rcTrack.right-200+offset, rcTrack.bottom-(rcTrack.Height()/ 12+0.25*r), false);
	//Koordinatne(pDC);
	DrawBicycle(pDC, r, n, d, beta, clr, omega, time);

}
void CBiciklView::SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp, CString strFIleName)
{
	CBitmap mapa;
	mapa.CreateCompatibleBitmap(pDC, rcBmp.Width(), rcBmp.Height());
	CDC* dc = new CDC();
	dc->CreateCompatibleDC(pDC);
	CBitmap* stara = dc->SelectObject(&mapa);
	dc->StretchBlt(0, 0,rcBmp.Width(),rcBmp.Height(),pDC,rcDC.left,rcDC.top,rcDC.Width(),rcDC.Height(),SRCCOPY);
	DImage slika(mapa);
	slika.Save(strFIleName);
	dc->SelectObject(&stara);
	delete dc;

}
void CBiciklView::OnDraw(CDC* pDC)
{
	CBiciklDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	XFORM stara;
	pDC->GetWorldTransform(&stara);
	CRect c;
	GetClientRect(&c);
	clock_t timeEnd = clock();
	float time = timeEnd - start;
	time = (float)(time / CLOCKS_PER_SEC);
	
	DrawBicycleOnTrack(pDC, c, 100, 20, 5, -30, RGB(0, 0, 255), RGB(25, 25, 112), RGB(0, 255, 0), RGB(40, 40, 40), CString("Alpina"), 60, time);
	SaveDC(pDC, c, c, CString("Bicikla.bmp"));
	pDC->SetWorldTransform(&stara);
	// TODO: add draw code for native data here
}


// CBiciklView printing

BOOL CBiciklView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBiciklView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBiciklView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBiciklView diagnostics

#ifdef _DEBUG
void CBiciklView::AssertValid() const
{
	CView::AssertValid();
}

void CBiciklView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBiciklDoc* CBiciklView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBiciklDoc)));
	return (CBiciklDoc*)m_pDocument;
}
#endif //_DEBUG


// CBiciklView message handlers


BOOL CBiciklView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CBiciklView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case VK_LEFT:
		offset -= 10;
		Invalidate();
		break;
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


int CBiciklView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (start== 0.0)
	{
		start = clock();
		id = SetTimer(1, 10, NULL);
	}

	return 0;
}


void CBiciklView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == id)
	{
		Invalidate();
	}

	CView::OnTimer(nIDEvent);
}
