#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	double angleRot = 0;
	double distance = 150;
	double alpha = 30;
	double beta = 0;

protected:
	double pi = 3.1415926535897932384626433832795;
	
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	void DrawGrid(double width, double height, int nSegW, int nSegH);
	void DrawAxis(double width);
	void DrawSphere(double r, int nSegAlpha, int nSegBeta);
	void DrawCylinder(double h, double r1, double r2, int nSeg, double c1, double c2, double c3);
	void DrawCone(double h, double r, int nSeg, float yellow);
	void DrawPot(double r1, double r2, double h);
	void DrawFigure(double angle);
};
