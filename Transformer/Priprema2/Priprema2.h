
// Priprema2.h : main header file for the Priprema2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CPriprema2App:
// See Priprema2.cpp for the implementation of this class
//

class CPriprema2App : public CWinApp
{
public:
	CPriprema2App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPriprema2App theApp;
