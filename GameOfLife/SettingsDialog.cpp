#include "SettingsDialog.h"

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)

EVT_BUTTON(wxID_OK, SettingsDialog::OnOkButton)

wxEND_EVENT_TABLE()


SettingsDialog::SettingsDialog(wxWindow* parent, Settings* settings)
	:wxDialog(parent, wxID_ANY, "Settings", wxPoint(100,100), wxSize(250,300)), pSettings(settings)
{

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainSizer);

	wxBoxSizer* gridSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* intervalSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* liveColorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* deadColorSizer = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* okOrCancel = CreateButtonSizer(wxOK | wxCANCEL);

	gridSizeSpinner = new wxSpinCtrl(this, 20001);
	gridSizeSpinner->SetValue(pSettings->GetGridHeight());
	gridSizeSpinner->SetMax(500);
	intervalSpinner = new wxSpinCtrl(this, 20002);
	intervalSpinner->SetValue(pSettings->GetInterval());
	intervalSpinner->SetMax(1000);
	liveColorPicker = new wxColourPickerCtrl(this, 20003);
	liveColorPicker->SetColour(settings->GetAliveColor());
	deadColorPicker = new wxColourPickerCtrl(this, 20004);
	deadColorPicker->SetColour(settings->GetDeadColor());
	


	wxStaticText* gridLabel = new wxStaticText(this, wxID_ANY, "  Grid Size");
	gridLabel->SetMinSize(wxSize(100, 50));

	wxStaticText* intervalLabel = new wxStaticText(this, wxID_ANY, "  Time Interval");
	intervalLabel->SetMinSize(wxSize(100, 50));

	wxStaticText* liveColorLabel = new wxStaticText(this, wxID_ANY, "  Live Cell Color");
	liveColorLabel->SetMinSize(wxSize(100, 50));

	wxStaticText* deadColorLabel = new wxStaticText(this, wxID_ANY, "  Dead Cell Color");
	deadColorLabel->SetMinSize(wxSize(100, 50));

	gridSizer->Add(gridLabel);
	gridSizer->Add(gridSizeSpinner);
	intervalSizer->Add(intervalLabel);
	intervalSizer->Add(intervalSpinner);
	liveColorSizer->Add(liveColorLabel);
	liveColorSizer->Add(liveColorPicker);
	deadColorSizer->Add(deadColorLabel);
	deadColorSizer->Add(deadColorPicker);

	mainSizer->Add(gridSizer);
	mainSizer->Add(intervalSizer);
	mainSizer->Add(liveColorSizer);
	mainSizer->Add(deadColorSizer);
	mainSizer->Add(okOrCancel);
	
}

void SettingsDialog::OnOkButton(wxCommandEvent& evt)
{
	pSettings->SetGridHeight(gridSizeSpinner->GetValue());
	pSettings->SetGridWidth(gridSizeSpinner->GetValue());
	pSettings->SetInterval(intervalSpinner->GetValue());
	pSettings->SetAliveColor(liveColorPicker->GetColour());
	pSettings->SetDeadColor(deadColorPicker->GetColour());
	EndModal(wxID_OK);
	pSettings->SaveData();
}



