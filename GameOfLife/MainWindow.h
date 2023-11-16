#pragma once
#include "wx/wx.h"
#include "wx/filedlg.h"
#include <vector>
#include "DrawingPanel.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include "RandomizeDialog.h"

using std::vector;

class MainWindow : public wxFrame
{
private:

	int mGenerations = 0;
	int mLivingCells = 0;
	std::string fileName;
	DrawingPanel* panel = nullptr;
	wxBoxSizer* boxSizer = nullptr;
	wxStatusBar* mainStatusBar = nullptr;
	wxMenuBar* menuBar = nullptr;
	wxToolBar* mainToolBar = nullptr;
	wxTimer* timer = nullptr;
	vector<vector<bool>> gameBoard; 
	vector<vector<int>> neighborCount;
	wxMenu* fileMenu = nullptr;
	wxMenu* optionsMenu = nullptr;
	wxMenu* viewMenu = nullptr;
	wxMenu* randomizeMenu = nullptr;
	Settings settings;

public:
	MainWindow();
	~MainWindow();

	//Member functions:
	void Randomize(int seed = time(NULL));
	void Refresh(bool eraseBackground, const wxRect* rect);
	void InitializeGrid();
	void UpdateStatusText();
	int CountLivingCells(vector<vector<bool>>& gameBoard);
	void CountAllNeighbors();
	int GetNeighborCount(int x, int y);
	void CreateNextGen();
	void ClearGameBoard();
	void ResetGame();
	void ClearNeighborCount();

	wxDECLARE_EVENT_TABLE();

	//Event handler functions:
	void OnSizeChange(wxSizeEvent& evt);
	void OnShowNeighbor(wxCommandEvent& evt);
	void OnOptionsClick(wxCommandEvent& evt);
	void OnRandomize(wxCommandEvent& evt);
	void OnPlayButtonClick(wxCommandEvent& evt);
	void OnPauseButtonClick(wxCommandEvent& evt);
	void OnNextButtonClick(wxCommandEvent& evt);
	void OnTrashButtonClick(wxCommandEvent& evt);
	void OnTimerCount(wxTimerEvent& evt);
	void OnNewClick(wxCommandEvent& evt);
	void OnOpenClick(wxCommandEvent& evt);
	void OnSaveClick(wxCommandEvent& evt);
	void OnSaveAsClick(wxCommandEvent& evt);
	void OnExitClick(wxCommandEvent& evt);
	void OnShowGrid(wxCommandEvent& evt);
	void OnShowDividers(wxCommandEvent& evt);
	void OnFinite(wxCommandEvent& evt);
	void OnToroidal(wxCommandEvent& evt);
	void OnResetSettings(wxCommandEvent& evt);
	void OnImport(wxCommandEvent& evt);
	void OnShowHud(wxCommandEvent& evt);
};

