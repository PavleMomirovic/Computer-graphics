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

	void DrawAxes();

	UINT LoadTexture(char* fileName);

	void DrawEnvCube(double a);

	void DrawSphere(double r, int nSeg, double texU, double texV, double texR);

	void DrawCone(double r, double h, int nSeg, double texU, double texV, double texR);

	void DrawCilinder(double r1, double r2, double h, int nSeg, double texU, double texV, double texR);

	void DrawSpiderBody();

	void DrawLegSegment(double r, double h, int nSeg);

	void DrawLeg();

	void DrawSpider();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 
	UINT spider;
	UINT env[6];

public:
	float alpha=10, beta=10, dist=28;

};
