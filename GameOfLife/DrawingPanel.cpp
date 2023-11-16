#include "DrawingPanel.h"
#include "MainWindow.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)	//Open event table


EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)


wxEND_EVENT_TABLE()		//Close event table


DrawingPanel::DrawingPanel(wxFrame* parent, vector<vector<bool>>& mainGameBoard, Settings* parentSettings, vector<vector<int>>& neighborCount, int* generations, int* livingCells)
	:wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(750, 750)), dGameBoard(mainGameBoard), pSettings(parentSettings), rNeighborCount(neighborCount),pGenerations(generations), pLivingCells(livingCells)
{
	frame = parent;
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}


DrawingPanel::~DrawingPanel()
{
}


void DrawingPanel::OnPaint(wxPaintEvent& paintEvent)
{
	wxAutoBufferedPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* context = wxGraphicsContext::Create(dc);
	if (!context) { return; }
	context->SetPen(*wxBLACK);
	context->SetBrush(wxTransparentColor);
	context->SetFont(wxFontInfo(14), *wxRED);

	//Math for cell size:
	wxSize panelSize = GetSize();
	float height = panelSize.y / (float)pSettings->GetGridHeight();
	float width = panelSize.x / (float)pSettings->GetGridWidth();


	//Drawing each cell:


	for (int i = 0; i < pSettings->GetGridWidth(); i++)
	{
		for (int j = 0; j < pSettings->GetGridHeight(); j++)
		{
			double X;
			double Y;
			wxString singleNeighborCount;
			singleNeighborCount << rNeighborCount[i][j];
			if (dGameBoard[i][j] == false)
			{
				context->SetBrush(pSettings->GetDeadColor());
				if (pSettings->GetShowGrid() == false)
				{
					context->SetPen(wxPen(pSettings->GetDeadColor(), 0));
				}
			}
			else
			{
				context->SetBrush(pSettings->GetAliveColor());
				if (pSettings->GetShowGrid() == false)
				{
					context->SetPen(wxPen(pSettings->GetAliveColor(), 0));
				}
			}
			context->DrawRectangle(width * i, height * j, width, height);
			if (pSettings->GetShowNeighbors() && rNeighborCount[i][j] > 0) {
				context->GetTextExtent(singleNeighborCount, &X, &Y);
				double xPos = width * i + width / 2 - X / 2;
				double yPos = height * j + height / 2 - Y / 2;
				context->DrawText(singleNeighborCount, xPos, yPos);
			}
		}
	}

	if (pSettings->GetShowDividers())
	{
		int solidLines = pSettings->GetGridWidth() / 10;
		dc.SetPen(wxPen(*wxBLACK, 5));

		for (int i = 0; i < solidLines; i++)
		{

			wxPoint vStart(width * (i + 1) * 10, 0);
			wxPoint vEnd(width * (i + 1) * 10, this->GetSize().y);
			dc.DrawLine(vStart, vEnd);

			wxPoint hStart(0, height * (i + 1) * 10);
			wxPoint hEnd(this->GetSize().x, height * (i + 1) * 10);
			dc.DrawLine(hStart, hEnd);
		}
	}
	if (pSettings->GetShowHud())
	{
		wxString displayText;
		displayText << "Generations: ";
		displayText << *pGenerations;
		displayText << "\nLiving Cells: ";
		displayText << *pLivingCells;
		displayText << "\nUniverse Type: ";
		if (pSettings->GetToroidal())
		{
			displayText << "Toroidal";
		}
		else
		{
			displayText << "Finite";
		}
		displayText << "\nUniverse Size: ";
		displayText << pSettings->GetGridWidth();
		displayText << "x";
		displayText << pSettings->GetGridHeight();

		context->SetFont(wxFontInfo(15), *wxRED);
		double X;
		double Y;
		context->GetTextExtent(displayText, &X, &Y);
		double xPos = GetSize().x / X * 2;
		double yPos = GetSize().y - (Y + 10);
		context->DrawText(displayText, xPos, yPos);
	}

	//Deleting pointers:
	delete context;
}


void DrawingPanel::SetGridSize(int newGridHeight, int newGridWidth)
{
	pSettings->SetGridHeight(newGridHeight);
	pSettings->SetGridWidth(newGridWidth);
}


void DrawingPanel::OnMouseUp(wxMouseEvent& evt)
{
	int xPos = evt.GetX();
	int yPos = evt.GetY();

	wxSize panelSize = GetSize();
	float height = panelSize.y / (float)pSettings->GetGridHeight();
	float width = panelSize.x / (float)pSettings->GetGridWidth();

	int yClickPos = (yPos / height);
	int xClickPos = (xPos / width);

	dGameBoard[xClickPos][yClickPos] = !dGameBoard[xClickPos][yClickPos];
	frame->Refresh();
	Refresh();
}


void DrawingPanel::SetSize(wxSize& size)
{
	wxPanel::SetSize(size);
	Refresh();
}

