#pragma once
#include "wx/wx.h"
#include "wx/numdlg.h"


class RandomizeDialog : public wxDialog
{
private:
	int* pSeed = nullptr;
	bool* cancel = nullptr;
	int randomSeed;
public:
	RandomizeDialog(wxWindow* parent, int* seed, bool* cancelBool);

};

