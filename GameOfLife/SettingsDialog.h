#pragma once
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include "Settings.h"

class SettingsDialog : public wxDialog
{
private:
	Settings* pSettings = nullptr;
	wxSpinCtrl* gridSizeSpinner = nullptr;
	wxSpinCtrl* intervalSpinner = nullptr;
	wxColourPickerCtrl* liveColorPicker = nullptr;
	wxColourPickerCtrl* deadColorPicker = nullptr;



public:
	SettingsDialog(wxWindow* parent, Settings* settings);
	void OnOkButton(wxCommandEvent& evt);
	wxDECLARE_EVENT_TABLE();
};

