
// Januarski1KolokvijumView.cpp : implementation of the CJanuarski1KolokvijumView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Januarski1Kolokvijum.h"
#endif

#include "Januarski1KolokvijumDoc.h"
#include "Januarski1KolokvijumView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>


// CJanuarski1KolokvijumView

IMPLEMENT_DYNCREATE(CJanuarski1KolokvijumView, CView)

BEGIN_MESSAGE_MAP(CJanuarski1KolokvijumView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CJanuarski1KolokvijumView construction/destruction

CJanuarski1KolokvijumView::CJanuarski1KolokvijumView() noexcept
{
	// TODO: add construction code here

}

CJanuarski1KolokvijumView::~CJanuarski1KolokvijumView()
{
}

BOOL CJanuarski1KolokvijumView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CJanuarski1KolokvijumView drawing
void CJanuarski1KolokvijumView::Translate(CDC* pDC, float dx, float dy)
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
void CJanuarski1KolokvijumView::Rotate(CDC* pDC, float angle)
{
	float alfa = (angle * 3.14) / 180;
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = cos(alfa);
	a.eM12 = sin(alfa);
	a.eM21 = -sin(alfa);
	a.eM22 = cos(alfa);
	pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);
}
void CJanuarski1KolokvijumView::Scale(CDC* pDC, float sx,float sy)
{
	XFORM a;
	a.eDx = 0;
	a.eDy = 0;
	a.eM11 = sx;
	a.eM12 = 0;
	a.eM21 = 0;
	a.eM22 = sy;
	pDC->ModifyWorldTransform(&a, MWT_LEFTMULTIPLY);
}

void Koordinatni(CDC* pDC)
{
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, 40);
	pDC->MoveTo(0, 0);
	pDC->LineTo(40, 0);
}
void CJanuarski1KolokvijumView::DrawArcSpot(CDC* pDC, int size, COLORREF colFill, int widthLine, COLORREF colLine)
{
	XFORM a;
	pDC->GetWorldTransform(&a);
	//Translate(pDC,400, 200);
	//pDC->Rectangle(-size / 2, -size / 2, size / 2, size / 2);
	pDC->BeginPath();
	pDC->MoveTo(-size/3, -size/2);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(-size / 4, -size / 2 - size / 4, size / 4, -size / 2 + size / 4, -size / 4, -size / 2,  size / 4, -size / 2);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(size / 4, -size / 2, size / 2, -size / 2 + size / 4, size / 4, -size / 2, size / 2, -size / 2 + size / 4);
	Rotate(pDC, 90);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(-size / 4, -size / 2 - size / 4, size / 4, -size / 2 + size / 4, -size / 4, -size / 2, size / 4, -size / 2);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(size / 4, -size / 2, size / 2, -size / 2 + size / 4, size / 4, -size / 2, size / 2, -size / 2 + size / 4);
	Rotate(pDC, 90);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(-size / 4, -size / 2 - size / 4, size / 4, -size / 2 + size / 4, -size / 4, -size / 2, size / 4, -size / 2);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(size / 4, -size / 2, size / 2, -size / 2 + size / 4, size / 4, -size / 2, size / 2, -size / 2 + size / 4);
	Rotate(pDC, 90);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(-size / 4, -size / 2 - size / 4, size / 4, -size / 2 + size / 4, -size / 4, -size / 2, size / 4, -size / 2);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(size / 4, -size / 2, size / 2, -size / 2 + size / 4, size / 4, -size / 2, size / 2, -size / 2 + size / 4);

	
	//Koordinatni(pDC);
	pDC->EndPath();
	CPen olovka(PS_SOLID, widthLine, colLine);
	CBrush cetka(colFill);
	CPen* staraO = pDC->SelectObject(&olovka);
	CBrush* cetkaO = pDC->SelectObject(&cetka);
	pDC->StrokeAndFillPath();
	pDC->SelectObject(&staraO);
	pDC->SelectObject(&cetkaO);

	pDC->SetWorldTransform(&a);

}

void CJanuarski1KolokvijumView::DrawTreeArcSpots(CDC* pDC, int size, COLORREF aColFill[], int widthLine, COLORREF colLine, float ratio)
{
	XFORM matrica;
	pDC->GetWorldTransform(&matrica);
	//Translate(pDC, 700, 200);
	Scale(pDC,  ratio,  ratio);
	DrawArcSpot(pDC, size, aColFill[2], widthLine, colLine);
	Scale(pDC, 1 / ratio, 1 / ratio);
	DrawArcSpot(pDC, size, aColFill[0], widthLine, colLine);
	Scale(pDC, 1 / ratio, 1 / ratio);
	DrawArcSpot(pDC, size, aColFill[1], widthLine, colLine);
	Scale(pDC, ratio, ratio);
	pDC->SetWorldTransform(&matrica);

}
void CJanuarski1KolokvijumView::DrawLabel(CDC* pDC, int fontHeight, COLORREF colText, CString strText)
{
	XFORM matrica;
	pDC->GetWorldTransform(&matrica);
	//Translate(pDC, 700, 200);
	CFont font;
	font.CreateFontW(fontHeight, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, _T("Arial"));
	CFont* stari = pDC->SelectObject(&font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOutW(-10, -10, strText);
	pDC->SelectObject(&stari);
	pDC->SetWorldTransform(&matrica);
}
void CJanuarski1KolokvijumView::DrawArcSpotsPattern(CDC* pDC, int size, COLORREF aCollFill[], int widthLine, COLORREF colLine, float ratio, COLORREF colText, int nRows, int nCols, float deltaX, float deltaY, float alpha)
{
	XFORM matrica;
	pDC->GetWorldTransform(&matrica);
	CRect client;
	GetClientRect(client);
	Translate(pDC, 200, 200);
	Koordinatni(pDC);
	for (int i = 1;i <= nRows;i++)
	{
		int kolko = 0;
		if (i % 2 != 0)
		{
			for (int j = 0;j < nCols;j++)
			{
				if (j == 0)
				{
					Translate(pDC, deltaX / 2, deltaY / 2);
					kolko += deltaX/2;
					DrawTreeArcSpots(pDC, size, aCollFill, widthLine, colLine, ratio);
					CString rez;
					rez.Format(_T("%d"), i);
					CString kolona;
					kolona.Format(_T("%d"), j+1);
					rez += kolona;
					DrawLabel(pDC, size / 2, colText, rez);

				}
				else
				{
					Translate(pDC, deltaX, 0);
					kolko += deltaX;
					DrawTreeArcSpots(pDC, size, aCollFill, widthLine, colLine, ratio);
					CString rez;
					rez.Format(_T("%d"), i);
					CString kolona;
					kolona.Format(_T("%d"), j);
					rez += kolona;
					DrawLabel(pDC, size / 2, colText, rez);


				}

			}
		}
		else
		{
			for (int z = 0;z < 3;z++)
			{
				
				Translate(pDC, deltaX, 0);
				kolko += deltaX;
				DrawTreeArcSpots(pDC, size, aCollFill, widthLine, colLine, ratio);
				CString rez;
				rez.Format(_T("%d"), i);
				CString kolona;
				kolona.Format(_T("%d"), z+1);
				rez += kolona;
				DrawLabel(pDC, size / 2, colText, rez);

			}
		}	
		Translate(pDC, -kolko, deltaY);
		
		
	}
	pDC->SetWorldTransform(&matrica);
}


void CJanuarski1KolokvijumView::OnDraw(CDC* pDC)
{
	CJanuarski1KolokvijumDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDC->SetGraphicsMode(GM_ADVANCED);
	COLORREF niz[] = {RGB(101,201,251),RGB(117,218,109),RGB(101,164,178)};
	//DrawTreeArcSpots(pDC, 100, niz, 10, RGB(312, 17, 101),3);
	//DrawLabel(pDC, 25, RGB(0, 0, 0), CString("23"));
	// TODO: add draw code for native data here
	DrawArcSpotsPattern(pDC, 40, niz, 5, RGB(312, 17, 101), 2, RGB(115, 250, 20), 3, 4, 200, 200, 45);
}



// CJanuarski1KolokvijumView printing

BOOL CJanuarski1KolokvijumView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CJanuarski1KolokvijumView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CJanuarski1KolokvijumView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CJanuarski1KolokvijumView diagnostics

#ifdef _DEBUG
void CJanuarski1KolokvijumView::AssertValid() const
{
	CView::AssertValid();
}

void CJanuarski1KolokvijumView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CJanuarski1KolokvijumDoc* CJanuarski1KolokvijumView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJanuarski1KolokvijumDoc)));
	return (CJanuarski1KolokvijumDoc*)m_pDocument;
}
#endif //_DEBUG


// CJanuarski1KolokvijumView message handlers
