#pragma once
#include "wx/wx.h"
#include <vector>
#include "Settings.h"

using std::vector;

class DrawingPanel : public wxPanel
{
private:
	vector<vector<bool>>& dGameBoard;
	vector<vector<int>>& rNeighborCount;
	wxFrame* frame = nullptr;
	Settings* pSettings = nullptr;
	int* pGenerations = nullptr;
	int* pLivingCells = nullptr;

public:

	DrawingPanel(wxFrame* parent, vector<vector<bool>>& mainGameBoard,Settings* parentSettings, vector<vector<int>>& neighborCount, int* generations, int* livingCells);
	~DrawingPanel();
	void OnPaint(wxPaintEvent& paintEvent);
	void SetSize(wxSize& size);
	void SetGridSize(int newGridHeight, int newGridWidth);
	void OnMouseUp(wxMouseEvent& evt);

	wxDECLARE_EVENT_TABLE();
};

