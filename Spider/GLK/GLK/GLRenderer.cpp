#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
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
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	spider = LoadTexture("spider.png");

	env[0] = LoadTexture("front.jpg");
	env[1] = LoadTexture("back.jpg");
	env[2] = LoadTexture("left.jpg");
	env[3] = LoadTexture("right.jpg");
	env[4] = LoadTexture("top.jpg");
	env[5] = LoadTexture("bot.jpg");

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D); //ovo mora i ovde i u prepare scene


	//kamera
	double alphaRad = alpha * 3.14 / 180;
	double betaRad = beta * 3.14 / 180;
	double eyex = dist * cos(alphaRad) * cos(betaRad);
	double eyey = dist * sin(alphaRad);
	double eyez = dist * cos(alphaRad) * sin(betaRad);
	gluLookAt(eyex, eyey, eyez, 0, 0, 0, 0, 10, 0);

	glTranslatef(0, -10, 0);
	glPushMatrix();
		glTranslatef(0,50, 0);
		DrawEnvCube(100);
	glPopMatrix();
	//crtanje
	DrawAxes();

	DrawSpider();

	//dete
	glPushMatrix();
		glTranslatef(25, 0, 25);
		glScalef(0.2, 0.2, 0.2);
		DrawSpider();
	glPopMatrix();


	glTranslatef(-20, 0, -20);
	//DrawCilinder(0, 6, 7, 8, 0, 0, 0);
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
	gluPerspective(50, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
	glDeleteTextures(1, &spider);
	glDeleteTextures(6, env);
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes()
{
	glLineWidth(2.0);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(50, 0, 0);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 50, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 50);

	glEnd();

}

UINT CGLRenderer::LoadTexture(char* fileName)
{
	UINT texID;
	DImage img;
	img.Load((CString)fileName);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

	return texID;
}
void CGLRenderer::DrawEnvCube(double a)
{
	//Front
	glBindTexture(GL_TEXTURE_2D, env[0]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(1, 1);
	glVertex3f( a / 2, -a / 2, -a / 2);
	glTexCoord2f(1, 0);
	glVertex3f( a / 2, a / 2, -a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, -a / 2);
	glEnd();
	//Back
	glBindTexture(GL_TEXTURE_2D, env[1]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);
	glTexCoord2f(0, 1);
	glVertex3f(a / 2, -a / 2, a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(a / 2, a / 2, a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-a / 2, a / 2, a / 2);
	glEnd();
	//Left
	glBindTexture(GL_TEXTURE_2D, env[2]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-a / 2, a / 2, -a / 2);
	glEnd();
	//Right
	glBindTexture(GL_TEXTURE_2D, env[3]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(a / 2, -a / 2, -a / 2);
	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(a / 2, a / 2, -a / 2);
	glEnd();
	//Top
	glBindTexture(GL_TEXTURE_2D, env[4]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, a / 2, -a / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-a / 2, a / 2, a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(a / 2, a / 2, -a / 2);
	glEnd();	
	//bottom
	glBindTexture(GL_TEXTURE_2D, env[5]);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, -a / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);
	glTexCoord2f(1, 0);
	glVertex3f(a / 2, -a / 2, a / 2);
	glTexCoord2f(0, 0);
	glVertex3f(a / 2, -a / 2, -a / 2);
	glEnd();
}
void CGLRenderer::DrawSphere(double r, int nSeg, double texU, double texV, double texR)
{
	int i, j;
	double ang1, ang2;
	double dAng1, dAng2;
	dAng1 = 3.14 / (double)nSeg;
	dAng2 = 2 * 3.14 / (double)nSeg;
	ang1 = -3.14 / 2;

	for (i = 0; i < nSeg; i++) {
		ang2 = 0;
		glBegin(GL_QUAD_STRIP);
		for (j = 0; j < nSeg+1; j++) {
			double x1 = r * cos(ang1) * cos(ang2);
			double y1 = r * sin(ang1);
			double z1 = r * cos(ang1) * sin(ang2);
			double x2 = r * cos(ang1 + dAng1) * cos(ang2);
			double y2 = r * sin(ang1 + dAng1);
			double z2 = r * cos(ang1 + dAng1) * sin(ang2);
			double tx1 = texR * x1 / r + texU;
			double ty1 = texR * z1 / r + texV;
			double tx2 = texR * x2 / r + texU;
			double ty2 = texR * z2 / r + texV;
			glTexCoord2f(tx1, ty1);
			glVertex3d(x1, y1, z1);
			glTexCoord2f(tx2, ty2);
			glVertex3d(x2, y2, z2);
			ang2 += dAng2;
		}
		glEnd();
		ang1 += dAng1;
	}
}
void CGLRenderer::DrawCone(double r, double h, int nSeg, double texU, double texV, double texR)
{
	int i;
	double ang1, dAng1;
	double;
	dAng1 = 2 * 3.14 / (double)nSeg;
	ang1 = 0;
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(texU, texV);
		glVertex3d(0.0, h, 0.0);
		for (i = 0; i < nSeg + 1; i++) {
			double x = r * cos(ang1);
			double y = 0;
			double z = r * sin(ang1);
			double tx = texR *cos(ang1) + texU;
			double ty = texR * sin(ang1) / r + texV;
			glTexCoord2f(tx, ty);
			glVertex3d(x, y, z);
			ang1 += dAng1;
		}
	glEnd();
}
void CGLRenderer::DrawCilinder(double r1, double r2, double h, int nSeg, double texU, double texV, double texR)
{
	int i;
	double ang1, dAng1;
	double;
	dAng1 = 2 * 3.14 / (double)nSeg;
	ang1 = 0;
	glColor3f(0.5, 0.5, 0.5);

	glBegin(GL_POLYGON);
	for (int j = 0; j < nSeg; j++) {
		double x1 = r1 * cos(ang1);
		double z1 = r1 * sin(ang1);
		glVertex3f(x1, 0, z1);
		ang1 += dAng1;

	}
	glEnd();
	ang1 = 0;
	glBegin(GL_POLYGON);
	for (int j = 0; j < nSeg; j++) {
		double x1 = r2 * cos(ang1);
		double z1 = r2 * sin(ang1);
		glVertex3f(x1, h, z1);
		ang1 += dAng1;
	}
	glEnd();
	ang1 = 0;
	//omotac
	glBegin(GL_QUAD_STRIP);
	glColor3f(0.7, 0.7, 0.7);
	for (i = 0; i < nSeg + 1; i++) {
		double x1 = r1 * cos(ang1);
		double y1 = 0;
		double z1 = r1 * sin(ang1);
		double x2 = r2 * cos(ang1);
		double y2 = h;
		double z2 = r2 * sin(ang1);
		double tx = texR * x1 / h + texU;
		double ty = texR * z1 / h + texV;
		glTexCoord2f(texU, texV);
		glVertex3d(x1, y1, z1);
		glTexCoord2f(tx, ty);
		glVertex3d(x2, y2, z2);
		ang1 += dAng1;
	}
	glEnd();

}
void CGLRenderer::DrawSpiderBody()
{
	glPushMatrix();
		glPushMatrix();
			//telo
			glScalef(1, 0.5, 1);
			DrawSphere(3, 10, 0.25, 0.25, 0.24);
			//glava
			glTranslatef(-4, 0, 0);
			DrawSphere(2, 10, 0.75, 0.25, 0.24);
		glPopMatrix();
		//dupe
		glTranslatef(6, 0, 0);
		glScalef(1, 0.8, 1);
		DrawSphere(5, 10, 0.25, 0.25, 0.24);

	glPopMatrix();

}
void CGLRenderer::DrawLegSegment(double r, double h, int nSeg)
{
	glPushMatrix();
		glTranslatef(0, r, 0);
		DrawSphere(r, 2*nSeg, 0.25, 0.25, 0.24);
		DrawCone(r, h, nSeg, 0.75, 0.75, 0.24);
	glPopMatrix();
}
void CGLRenderer::DrawLeg()
{
	glPushMatrix();
		DrawLegSegment(1, 10, 5);
		glTranslatef(0, 11, 0);
		glRotatef(85,1, 0, 0);
		DrawLegSegment(1, 15, 5);
	glPopMatrix();
}
void CGLRenderer::DrawSpider()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spider);
	glColor3f(1, 1, 1);

	glPushMatrix();

	glTranslatef(0, 5, 0);
	DrawSpiderBody();

	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glRotatef(i*30-45, 0, 1, 0);
		glRotatef(50, 1, 0, 0);

		DrawLeg();
		glPopMatrix();
	}
	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glRotatef(i*30-45+180, 0, 1, 0);
		glRotatef(50, 1, 0, 0);

		DrawLeg();
		glPopMatrix();
	}

	glPopMatrix();
}