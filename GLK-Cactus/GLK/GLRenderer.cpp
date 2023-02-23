#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glLoadIdentity();
	glClearColor(0.53, 0.75, 1, 1);
	//glClearColor(0, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	double alphaRad = alpha * pi / 180;
	double betaRad = beta * pi / 180;
	double eyeX = distance * cos(alphaRad) * cos(betaRad);
	double eyeY = distance * sin(alphaRad);
	double eyeZ = distance * cos(alphaRad) * sin(betaRad);
	gluLookAt(eyeX,eyeY, eyeZ, 0, 0, 0, 0, 1, 0);
	glTranslatef(0, -17, 0);
	DrawGrid(100, 100, 10, 10);
	DrawAxis(500);
	DrawFigure(angleRot);
	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);


	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawGrid(double width, double height, int nSegW, int nSegH)
{
	glBegin(GL_LINES);
	glLineWidth(1.0);
	glColor3f(1, 0.6, 0.0);
	for (int i = 0; i <= nSegW; i++)
	{
		glVertex3f(-width / 2 + i * width / nSegW, 0, -height / 2);
		glVertex3f(-width / 2 + i * width / nSegW, 0, height / 2);
	}

	for (int i = 0; i <= nSegH; i++) {
		glVertex3f(-width / 2, 0, -height / 2 + i * height / nSegH);
		glVertex3f(width / 2, 0, -height / 2 + i * height / nSegH);
	}

	glEnd();
}

void CGLRenderer::DrawAxis(double width)
{
	glBegin(GL_LINES);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, width, 0);

	glColor3f(1.0, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(width,0,0);

	glColor3f(0.0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0,0,width);

	glEnd();

}

void CGLRenderer::DrawSphere(double r, int nSegAlpha, int nSegBeta)
{
	double x, y, z;
	for (int i = 0; i < nSegAlpha; i++) {
		glBegin(GL_QUAD_STRIP);
		glColor3f(0.0, 0.8, 0.0);
		double alpha1 = -90 + i * 180.0 / nSegAlpha, alpha2 = -90 + (i + 1) * 180.0 / nSegAlpha;
		for (int j = 0; j <= nSegBeta; j++) {
			double beta = j * 360 / nSegBeta;
			double alpha1Rad = alpha1 * pi / 180, alpha2Rad = alpha2 * pi / 180, betaRad = pi * beta / 180;
			x = r * cos(alpha1Rad) * cos(betaRad);
			y = r * sin(alpha1Rad);
			z = r * cos(alpha1Rad) * sin(betaRad);
			glVertex3f(x, y, z);
			x = r * cos(alpha2Rad) * cos(betaRad);
			y = r * sin(alpha2Rad);
			z = r * cos(alpha2Rad) * sin(betaRad);
			glVertex3f(x, y, z);

		}
		glEnd();
	}
}

void CGLRenderer::DrawCylinder(double h, double r1, double r2, int nSeg,double c1, double c2, double c3)
{
	glColor3f(c1,c2,c3);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(r1 * cos(i * 2 * pi / nSeg), 0, r1 * sin(i * 2 * pi / nSeg));
	}
	glEnd();
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(r1 * cos(i * 2 * pi / nSeg), 0, r1 * sin(i * 2 * pi / nSeg));
		glVertex3f(r2 * cos(i * 2 * pi / nSeg), h, r2 * sin(i * 2 * pi / nSeg));
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, h, 0);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(r2 * cos(i * 2 * pi / nSeg), h, r2 * sin(i * 2 * pi / nSeg));
	}
	glEnd();


}

void CGLRenderer::DrawCone(double h, double r, int nSeg,float yellow)
{
	if (yellow) glColor3f(1,1,0);
		else glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, 0);
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(r * cos(i * 2 * pi / nSeg), 0, r * sin(i * 2 * pi / nSeg));
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, h, 0);
	
	for (int i = 0; i <= nSeg; i++) {
		glVertex3f(r * cos(i * pi * 2 / nSeg), 0, r * sin(i * pi * 2 / nSeg));
	}
	glEnd();

	
}

void CGLRenderer::DrawPot(double r1, double r2, double h)
{
	DrawCylinder(h,r1, r2,8, 0.47, 0.0, 0.0);
	

}

void CGLRenderer::DrawFigure(double angle)
{
	float  potH = 15;
	float H = 9, R = 2.5, coneR = 3;
	DrawPot(10, 13, potH);
	

	glTranslatef(0, 15, 0);
	DrawCylinder(H, coneR, coneR, 8,1,1,1);

	glTranslatef(0, H+R, 0);
	DrawSphere(R, 20, 20);
	glTranslatef(0, R, 0);
	DrawCone(H,coneR,8,false);
	glTranslatef(0, H+R, 0);
	DrawSphere(R, 20, 20);
	//glPushMatrix();
		glPushMatrix();

			glRotatef(45, 1, 0, 0);
			glTranslatef(0, R, 0);
			DrawCone(H,coneR,8,false);
			glTranslatef(0, H+R, 0);
			DrawSphere(R, 20, 20);
		glPopMatrix();
		glPushMatrix();
			glRotatef(-45, 1, 0, 0);
			glTranslatef(0, R, 0);
			DrawCylinder(H, R, R, 8, 1, 1, 1);
			glTranslatef(0, H + R, 0);
			DrawSphere(R, 20, 20);

			//prva podgrana
			glRotatef(45, 1, 0, 0);
			glTranslatef(0, R, 0);
			DrawCone(H,coneR,8,false);
			glTranslatef(0, H + R, 0);
			DrawSphere(R, 20, 20);

			//vracanje nazad
			glTranslatef(0, -(H + 2 * R),0);
			glRotatef(-45, 1, 0, 0);

			//druga podgrana
			glTranslatef(0, R, 0);
			DrawCylinder(H, coneR, coneR, 8, 1, 1, 1);
			glTranslatef(0, H + R, 0);
			DrawSphere(R, 20, 20);

			//vracanje nazad
			glTranslatef(0, -(H + 2 * R), 0);

			//treca podgrana
			glRotatef(-45, 1, 0, 0);
			glTranslatef(0, R, 0);
			DrawCone(H,coneR,8,false);
			glTranslatef(0, H + R, 0);
			DrawSphere(R, 20, 20);

		glPopMatrix();
			
		glRotatef(angleRot,1,0,0);
		glTranslatef(0, R, 0);
		DrawCone(H,coneR,8,true); //ova da je zuta
		glTranslatef(0, H + R, 0);
		DrawSphere(R, 20, 20);

		//prva podgrana
		glRotatef(45, 1, 0, 0);
		glTranslatef(0, R, 0);
		DrawCone(H,coneR,8,false);
		glTranslatef(0, H + R, 0);
		DrawSphere(R, 20, 20);

		//vracanje nazad
		glTranslatef(0, -(H + 2 * R), 0);
		glRotatef(-45, 1, 0, 0);

		//druga podgrana
		glTranslatef(0, R, 0);
		DrawCone(H,coneR,8,false);
		glTranslatef(0, H + R, 0);
		DrawSphere(R, 20, 20);

		//vracanje nazad
		glTranslatef(0, -(H + 2 * R), 0);

		//treca podgrana
		glRotatef(-45, 1, 0, 0);
		glTranslatef(0, R, 0);
		DrawCone(H,coneR,8,false);
		glTranslatef(0, H + R, 0);
		DrawSphere(R, 20, 20);

}
