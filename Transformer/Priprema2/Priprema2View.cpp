
// Priprema2View.cpp : implementation of the CPriprema2View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Priprema2.h"
#endif

#include "Priprema2Doc.h"
#include "Priprema2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPriprema2View

IMPLEMENT_DYNCREATE(CPriprema2View, CView)

BEGIN_MESSAGE_MAP(CPriprema2View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CPriprema2View construction/destruction

CPriprema2View::CPriprema2View() noexcept
{

	images[0].Load(CString("background.jpg"));
	images[1].Load(CString("body1.png"));
	images[2].Load(CString("arm1.png"));
	images[3].Load(CString("arm2.png"));
	images[4].Load(CString("leg1.png"));
	images[5].Load(CString("leg2.png"));
	// TODO: add construction code here

}

CPriprema2View::~CPriprema2View()
{
}

BOOL CPriprema2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPriprema2View drawing

void CPriprema2View::OnDraw(CDC* pDC)
{
	CPriprema2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	CBitmap img;
	img.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	
	memDC->SelectObject(img);
	memDC->SetGraphicsMode(GM_ADVANCED);

	DrawBackground(memDC,&rect);
	XFORM f;
	memDC->GetWorldTransform(&f);


	DrawTransformer(memDC);

	memDC->SetWorldTransform(&f);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	delete memDC;

	// TODO: add draw code for native data here
}

void CPriprema2View::DrawBackground(CDC* pDC, CRect rc)
{
	if (pomHor + rc.Width() > images[0].Width()) pomHor = images[0].Width()-rc.Width();
	images[0].DrawTransparent(pDC,CRect(0+pomHor, 0, rc.Width()+pomHor, rc.Height()),rc, RGB(255,255,255));
}
void CPriprema2View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	unsigned char* bits = pImage->GetDIBBits();
	pImage->DrawTransparent(pDC, CRect(0, 0, pImage->Width(), pImage->Height()), CRect(0, 0, pImage->Width(), pImage->Height()), RGB(bits[2], bits[1], bits[0]));
}
void CPriprema2View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM f;
	f.eM11 = 1;
	f.eM12 = 0;
	f.eM21 = 0;
	f.eM22 = 1;
	f.eDx = dX;
	f.eDy =dY;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&f, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&f, MWT_LEFTMULTIPLY);
}
void CPriprema2View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float pi = 3.142159;
	XFORM f;
	f.eM11 = cos(angle * pi / 180);
	f.eM12 = sin(angle * pi / 180);
	f.eM21 = -sin(angle * pi / 180);
	f.eM22 = cos(angle * pi / 180);
	f.eDx = 0;
	f.eDy = 0;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&f, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&f, MWT_LEFTMULTIPLY);

}
void CPriprema2View::DrawTransformer(CDC* pDC)
{
	DrawLeg2(pDC);
	DrawLeg1(pDC);
	DrawBody(pDC);
	DrawArm2(pDC);
	DrawArm1(pDC);

}
void CPriprema2View::DrawArm1(CDC* pDC)
{
	DrawImgTransparent(pDC, &images[2]);
}
void CPriprema2View::DrawArm2(CDC* pDC)
{
	Translate(pDC, 210, 85, false);
	Rotate(pDC, -u1, false);
	Translate(pDC, -34, -31, false);

	XFORM old;
	pDC->GetWorldTransform(&old);

	Translate(pDC, 210, 102, false);
	Rotate(pDC, u2-180, false);
	Translate(pDC, -23, -61, false);
	DrawImgTransparent(pDC, &images[3]);

	pDC->SetWorldTransform(&old);


}
void CPriprema2View::DrawLeg1(CDC* pDC)
{
	Translate(pDC, 200, 600, false);
	Rotate(pDC, u5, false);
	Translate(pDC, -30, -125, false);
	DrawImgTransparent(pDC, &images[4]);
}
void CPriprema2View::DrawLeg2(CDC* pDC)
{
	XFORM xform;
	pDC->GetWorldTransform(&xform);
	Translate(pDC, 200, 600, false);
	Rotate(pDC, u3, false);
	Translate(pDC, -35, -60, false);
	DrawImgTransparent(pDC, &images[5]);
	pDC->SetWorldTransform(&xform);
}
void CPriprema2View::DrawBody(CDC* pDC)
{
	Translate(pDC, 237, 125, false);
	Rotate(pDC, u4, false);
	Translate(pDC, -26, -133,false);

	DrawImgTransparent(pDC, &images[1]);
}

// CPriprema2View printing

BOOL CPriprema2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPriprema2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPriprema2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CPriprema2View diagnostics

#ifdef _DEBUG
void CPriprema2View::AssertValid() const
{
	CView::AssertValid();
}

void CPriprema2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPriprema2Doc* CPriprema2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPriprema2Doc)));
	return (CPriprema2Doc*)m_pDocument;
}
#endif //_DEBUG


// CPriprema2View message handlers


void CPriprema2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	//za pozadinu:
	if (nChar == 'Z') {
		pomHor += 20;
		Invalidate();
	}
	if (nChar == 'X') {
		pomHor -= 20;
		if (pomHor < 0) pomHor = 0;
		Invalidate();
	}

	//za sklapanje transformera:
	if (nChar == 'A') {
		u1 += 5;
		if (u1 > 0) u1 = 0;
		Invalidate();
	}
	if (nChar == 'Q') {
		u1 -= 5;
		if (u1 < -100) u1 = -100;
		Invalidate();
	}	
	if (nChar == 'E') {
		u5 += 5;
		if (u5 > 0) u5 = 0;
		Invalidate();
	}
	if (nChar == 'D') {
		u5 -= 5;
		if (u5 < -100) u5 = -100;
		Invalidate();
	}
	if (nChar == 'G') {
		u2 += 5;
		if (u2 > 90) u2 = 90;

		Invalidate();
	}
	if (nChar == 'T') {
		u2 -= 5;
		if (u2 < 0) u2 = 0;
		Invalidate();
	}
	if (nChar == 'F') {
		u3 += 5;
		if (u3 > 110) u3 = 110;
		Invalidate();
	}
	if (nChar == 'R') {
		u3 -= 5;
		if (u3 < 0) u3 = 0;
		Invalidate();
	}
	if (nChar == 'S') {
		u4 += 5;
		if (u4 > 15) u4 = 15;
		Invalidate();
	}
	if (nChar == 'W') {
		u4 -= 5;
		if (u4 < 0) u4 = 0;
		Invalidate();
	}

	if (nChar == VK_LEFT){

		u1 -= 5;
		if (u1 < -100) u1 = -100;
		u5 -= 5;
		if (u5 < -100) u5 = -100;
		u2 += 5;
		if (u2 > 90) u2 = 90;
		u3 += 5;
		if (u3 > 110) u3 = 110;
		u4 += 5;
		if (u4 > 15) u4 = 15;
		Invalidate();
	}

	if (nChar == VK_RIGHT) {
		u1 += 5;
		if (u1 > 0) u1 = 0;
		u5 += 5;
		if (u5 > 0) u5 = 0;
		u2 -= 5;
		if (u2 < 0) u2 = 0;
		u3 -= 5;
		if (u3 < 0) u3 = 0;
		u4 -= 5;
		if (u4 < 0) u4 = 0;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CPriprema2View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}
