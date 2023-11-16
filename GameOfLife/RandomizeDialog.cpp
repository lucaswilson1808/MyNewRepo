#include "RandomizeDialog.h"


RandomizeDialog::RandomizeDialog(wxWindow* parent, int* seed, bool* cancelBool)
	:wxDialog(parent, wxID_ANY, "Randomize", wxPoint(100, 100), wxSize(300, 150)), pSeed(seed), cancel(cancelBool)
{
	randomSeed = wxGetNumberFromUser("", "  Seed number: ", "Randomize by seed",0,0,4000000);
	if (randomSeed == -1) 
	{
		*cancel = true;
	}
	else
	{
		*pSeed = randomSeed;
	}
	if (pSeed != nullptr)
	{
		pSeed = nullptr;
	}
}