#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)	//Open event table

EVT_SIZE(MainWindow::OnSizeChange)
EVT_MENU(10001, MainWindow::OnPlayButtonClick)
EVT_MENU(10002, MainWindow::OnPauseButtonClick)
EVT_MENU(10003, MainWindow::OnNextButtonClick)
EVT_MENU(10004, MainWindow::OnTrashButtonClick)
EVT_MENU(10005, MainWindow::OnOptionsClick)
EVT_MENU(10006, MainWindow::OnShowNeighbor)
EVT_TIMER(10007, MainWindow::OnTimerCount)
EVT_MENU(10008, MainWindow::OnRandomize)
EVT_MENU(10009, MainWindow::OnRandomize)
EVT_MENU(10010, MainWindow::OnShowGrid)
EVT_MENU(10011, MainWindow::OnShowDividers)
EVT_MENU(10012, MainWindow::OnToroidal)
EVT_MENU(10013, MainWindow::OnFinite)
EVT_MENU(10014, MainWindow::OnResetSettings)
EVT_MENU(10015, MainWindow::OnImport)
EVT_MENU(10016, MainWindow::OnShowHud)
EVT_MENU(wxID_NEW, MainWindow::OnNewClick)
EVT_MENU(wxID_SAVE, MainWindow::OnSaveClick)
EVT_MENU(wxID_SAVEAS, MainWindow::OnSaveAsClick)
EVT_MENU(wxID_OPEN, MainWindow::OnOpenClick)
EVT_MENU(wxID_EXIT, MainWindow::OnExitClick)


wxEND_EVENT_TABLE()		//Close event table


MainWindow::MainWindow()
	:wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(0, 0), wxSize(750, 750))
{
	settings.LoadData();	//Loads settings

	//Creates the icons for tool bar:
	wxBitmap playIcon(play_xpm);
	wxBitmap pauseIcon(pause_xpm);
	wxBitmap nextIcon(next_xpm);
	wxBitmap trashIcon(trash_xpm);

	//Creates menu bar:
	menuBar = new wxMenuBar();
	SetMenuBar(menuBar);

	//Creates tool bar
	mainToolBar = CreateToolBar();
	mainToolBar->AddTool(10001, "Play", playIcon);
	mainToolBar->AddTool(10002, "Pause", pauseIcon);
	mainToolBar->AddTool(10003, "Next", nextIcon);
	mainToolBar->AddTool(10004, "Trash", trashIcon);
	mainToolBar->Realize();

	//Creates menu objects:

	fileMenu = new wxMenu();
	fileMenu->Append(wxID_NEW);
	fileMenu->Append(wxID_SAVE);
	fileMenu->Append(wxID_SAVEAS);
	fileMenu->Append(wxID_OPEN);
	fileMenu->Append(10015, "Import");
	fileMenu->Append(wxID_EXIT);

	optionsMenu = new wxMenu();
	optionsMenu->Append(10005, "Options");
	optionsMenu->Append(10014, "Reset Default Settings");


	viewMenu = new wxMenu();
	viewMenu->AppendCheckItem(10006, "Show Neighbor Count");
	viewMenu->Check(10006, settings.GetShowNeighbors());

	viewMenu->AppendCheckItem(10010, "Show Grid");
	viewMenu->Check(10010, settings.GetShowGrid());

	viewMenu->AppendCheckItem(10011, "Show 10x10 Dividers");
	viewMenu->Check(10011, settings.GetShowDividers());

	viewMenu->AppendCheckItem(10012, "Toroidal");
	viewMenu->Check(10012, settings.GetToroidal());

	viewMenu->AppendCheckItem(10013, "Finite");
	viewMenu->Check(10013, settings.GetFinite());

	viewMenu->AppendCheckItem(10016, "Show HUD");
	viewMenu->Check(10016, settings.GetShowHud());


	randomizeMenu = new wxMenu();
	randomizeMenu->Append(10008, "Randomize by Seed");
	randomizeMenu->Append(10009, "Randomize by Time");


	//Adds menus to menu bar:
	menuBar->Append(fileMenu, "File");
	menuBar->Append(viewMenu, "View");
	menuBar->Append(randomizeMenu, "Randomize");
	menuBar->Append(optionsMenu, "Settings");

	timer = new wxTimer(this, 10007);	//Creates timer object


	//Creates status bar
	mainStatusBar = CreateStatusBar();
	UpdateStatusText();

	CountAllNeighbors();	//Counts all neighbors

	panel = new DrawingPanel(this, gameBoard, &settings, neighborCount, &mGenerations,&mLivingCells);	//Creates drawing panel

	//Creates window box sizer
	boxSizer = new wxBoxSizer(wxVERTICAL);
	boxSizer->Add(panel, 1, wxEXPAND | wxALL);
	this->SetSizer(boxSizer);

	InitializeGrid();

	this->Layout();
}
MainWindow::~MainWindow()
{
	delete timer;
}


void MainWindow::Randomize(int seed)
{
	srand(seed);
	for (int y = 0; y < settings.GetGridWidth(); y++)
	{
		for (int x = 0; x < settings.GetGridHeight(); x++)
		{
			int num = rand();
			if (num % 4 == 0)
			{
				gameBoard[y][x] = true;
			}
			else
			{
				gameBoard[y][x] = false;
			}
		}
	}
	CountAllNeighbors();
	UpdateStatusText();
	panel->Refresh();
}

//Overridden Refresh method for main window:
void MainWindow::Refresh(bool eraseBackground, const wxRect* rect)
{
	CountAllNeighbors();
	UpdateStatusText();
	wxFrame::Refresh();
}

void MainWindow::InitializeGrid()
{
	//Resizes the gameboard and the neighbor count vector

	gameBoard.resize(settings.GetGridWidth());

	for (size_t i = 0; i < settings.GetGridWidth(); i++)
	{
		gameBoard[i].resize(settings.GetGridHeight());
	}

	neighborCount.clear();
	neighborCount.resize(settings.GetGridWidth());

	for (int i = 0; i < neighborCount.size(); i++)
	{
		neighborCount[i].resize(settings.GetGridHeight());
	}

	panel->SetGridSize(settings.GetGridHeight(), settings.GetGridWidth());	//Resizes the panel the the gridsize stored in settings
}

void MainWindow::UpdateStatusText()
{
	CountLivingCells(gameBoard);
	mainStatusBar->SetStatusText("Generation: " + std::to_string(mGenerations) + "  Living Cells: " + std::to_string(mLivingCells), 0);
}

int MainWindow::CountLivingCells(vector<vector<bool>>& gameBoard)
{
	mLivingCells = 0;
	for (int y = 0; y < gameBoard.size(); y++)
	{
		for (int x = 0; x < gameBoard[y].size(); x++)
		{
			if (gameBoard[y][x])
			{
				mLivingCells++;
			}
		}
	}
	return mLivingCells;
}

void MainWindow::CountAllNeighbors()
{

	for (int j = 0; j < gameBoard.size(); j++)
	{
		for (int k = 0; k < gameBoard[j].size(); k++)
		{
			neighborCount[j][k] = GetNeighborCount(j, k);
		}
	}
}

int MainWindow::GetNeighborCount(int x, int y)
{
	int neighborCount = 0;
	int gridHeight = settings.GetGridHeight(), gridWidth = settings.GetGridWidth();

	for (int j = -1; j <= 1; j++)
	{
		for (int i = -1; i <= 1; i++)
		{
			int xCheck = x + i;
			int yCheck = y + j;
			if (settings.GetFinite())
			{
				if (j == 0 && i == 0) continue;
				if (xCheck < 0) continue;
				if (yCheck < 0) continue;
				if (xCheck >= gridWidth) continue;
				if (yCheck >= gridHeight) continue;
				if (gameBoard[xCheck][yCheck] == true) neighborCount++;
			}
			else
			{
				if (j == 0 && i == 0) continue;
				if (xCheck < 0) xCheck = gridWidth - 1;
				if (yCheck < 0) yCheck = gridWidth - 1;
				if (xCheck >= gridWidth) xCheck = 0;
				if (yCheck >= gridHeight) yCheck = 0;
				if (gameBoard[xCheck][yCheck] == true) neighborCount++;
			}
		}
	}
	return neighborCount;
}

void MainWindow::CreateNextGen()
{
	vector<vector<bool>> sandbox;
	sandbox.resize(settings.GetGridWidth());

	for (int i = 0; i < sandbox.size(); i++)
	{
		sandbox[i].resize(settings.GetGridHeight());
	}

	for (int j = 0; j < gameBoard.size(); j++)
	{
		for (int k = 0; k < gameBoard[j].size(); k++)
		{
			if (gameBoard[j][k] && neighborCount[j][k] < 2)
			{
				sandbox[j][k] = false;
			}
			else if (gameBoard[j][k] && neighborCount[j][k] > 3)
			{
				sandbox[j][k] = false;
			}
			else if (gameBoard[j][k] && (neighborCount[j][k] == 2 || neighborCount[j][k] == 3))
			{
				sandbox[j][k] = true;
			}
			else if (gameBoard[j][k] == false && neighborCount[j][k] == 3)
			{
				sandbox[j][k] = true;
			}
		}
	}
	swap(gameBoard, sandbox);
	CountAllNeighbors();
	mGenerations++;
	UpdateStatusText();
	panel->Refresh();
}

void MainWindow::ClearGameBoard()
{
	for (size_t x = 0; x < settings.GetGridWidth(); x++)
	{
		for (size_t y = 0; y < settings.GetGridHeight(); y++)
		{
			gameBoard[x][y] = false;
		}
	}
}

void MainWindow::ResetGame()
{
	mLivingCells = 0;
	mGenerations = 0;
	ClearGameBoard();
	InitializeGrid();
	SetStatusText("Generation: " + std::to_string(mGenerations) + "  Living Cells: " + std::to_string(mLivingCells), 0);
}

void MainWindow::ClearNeighborCount()
{

	for (size_t x = 0; x < settings.GetGridWidth(); x++)
	{
		for (size_t y = 0; y < settings.GetGridHeight(); y++)
		{
			neighborCount[x][y] = 0;
		}
	}
}



//Event Handler functions:
void MainWindow::OnSizeChange(wxSizeEvent& evt)
{

	wxSize caughtSize = evt.GetSize();	//Sotres the new size of the window

	if (panel != nullptr)
	{
		panel->SetSize(caughtSize);		//Sets panel size to the new window size
		evt.Skip();
	}

}

void MainWindow::OnShowNeighbor(wxCommandEvent& evt)
{

	//Checks wether or not the option to show neighbor count is on or off

		//If on, turns it off
	if (settings.GetShowNeighbors())
	{
		viewMenu->Check(10006, false);
		settings.SetShowNeighbors(false);
	}
	//If off turns it on
	else
	{
		viewMenu->Check(10006, true);
		settings.SetShowNeighbors(true);
	}

	settings.SaveData();	//Saves data to the settings file
	panel->Refresh();		//Repaints the panel
	evt.Skip();
}

void MainWindow::OnOptionsClick(wxCommandEvent& evt)
{
	if (timer->IsRunning())
	{
		timer->Stop();
	}
	SettingsDialog* dialog = new SettingsDialog(this, &settings);
	if (dialog->ShowModal() == wxID_OK)
	{
		InitializeGrid();
		panel->Refresh();
	}
	evt.Skip();
}

void MainWindow::OnPlayButtonClick(wxCommandEvent& evt)
{
	timer->Start(settings.GetInterval());
	evt.Skip();
}

void MainWindow::OnPauseButtonClick(wxCommandEvent& evt)
{
	if (timer->IsRunning())
	{
		timer->Stop();
	}
	evt.Skip();
}

void MainWindow::OnNextButtonClick(wxCommandEvent& evt)
{
	CreateNextGen();
	evt.Skip();
}

void MainWindow::OnTrashButtonClick(wxCommandEvent& evt)
{
	//Resets all the member variables gameboard size and initializes the grid before refreshing the panel

	timer->Stop();
	ResetGame();
	panel->Refresh();
	evt.Skip();
}

void MainWindow::OnTimerCount(wxTimerEvent& evt)
{
	CreateNextGen();
	evt.Skip();
}

void MainWindow::OnRandomize(wxCommandEvent& evt)
{
	if (timer->IsRunning()) {
		timer->Stop();
	}

	if (evt.GetId() == 10008)
	{
		int seed;
		bool cancel = false;
		RandomizeDialog* dialog = new RandomizeDialog(this, &seed, &cancel);

		if (cancel == false)
		{
			Randomize(seed);
		}
		else
		{

		}

	}
	else if (evt.GetId() == 10009)
	{
		Randomize();
	}
	else
	{

	}
	evt.Skip();
}

void MainWindow::OnNewClick(wxCommandEvent& evt)
{
	if (timer->IsRunning())
	{
		timer->Stop();
	}
	fileName.clear();
	ResetGame();
	panel->Refresh();
	evt.Skip();
}

void MainWindow::OnSaveClick(wxCommandEvent& evt)
{
	std::ofstream fileStream;

	if (fileName.empty())
	{
		wxFileDialog fileDialog(this, "Save Game of Life file", wxEmptyString, wxEmptyString, "Game of Life files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (fileDialog.ShowModal() == wxID_CANCEL)
		{
			return;
		}
		fileName = fileDialog.GetPath();
		fileStream.open(fileName);
		if (fileStream.is_open())
		{
			for (int x = 0; x < gameBoard.size(); x++)
			{
				for (int y = 0; y < gameBoard[x].size(); y++)
				{
					if (gameBoard[y][x])
					{
						fileStream << '*';
					}
					else
					{
						fileStream << '.';
					}
				}
				fileStream << '\n';
			}
			fileStream.close();
		}
		settings.SaveData();
	}
	else
	{
		fileStream.open(fileName);
		if (fileStream.is_open())
		{
			for (int x = 0; x < gameBoard.size(); x++)
			{
				for (int y = 0; y < gameBoard[x].size(); y++)
				{
					if (gameBoard[y][x])
					{
						fileStream << '*';
					}
					else
					{
						fileStream << '.';
					}
				}
				fileStream << '\n';
			}
			fileStream.close();
		}
		settings.SaveData();
	}

}

void MainWindow::OnSaveAsClick(wxCommandEvent& evt)
{
	wxFileDialog fileDialog(this, "Save Game of Life file", wxEmptyString, wxEmptyString, "Game of Life files (*.cells) | *.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	fileName = fileDialog.GetPath();
	std::ofstream fileStream;
	fileStream.open(fileName);
	if (fileStream.is_open())
	{
		for (int x = 0; x < gameBoard.size(); x++)
		{
			for (int y = 0; y < gameBoard[x].size(); y++)
			{
				if (gameBoard[y][x])
				{
					fileStream << '*';
				}
				else
				{
					fileStream << '.';
				}
			}
			fileStream << '\n';
		}
		fileStream.close();
	}
	settings.SaveData();
	evt.Skip();
}

void MainWindow::OnOpenClick(wxCommandEvent& evt)
{

	if (timer->IsRunning())
	{
		timer->Stop();
	}

	wxFileDialog fileDialog(this, "Open Game of Life file", wxEmptyString, wxEmptyString, "Game of Life files (*.cells) | *.cells", wxFD_OPEN);

	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	ClearGameBoard();

	std::ifstream fileStream;
	fileName = fileDialog.GetPath();
	fileStream.open(fileName);
	if (fileStream.is_open())
	{

		std::string line;
		int index = 0;
		int temp = 0;
		int width = 0;
		do {
			std::getline(fileStream, line);
			temp = line.size();
			if (line.empty())
			{
				break;
			}
			width = temp;
			++index;


		} while (!fileStream.eof());


		gameBoard.resize(width);
		for (int i = 0; i < gameBoard.size(); i++)
		{
			gameBoard[i].resize(index);
		}
		settings.SetGridWidth(width);
		settings.SetGridHeight(index);

		neighborCount.clear();
		neighborCount.resize(settings.GetGridWidth());

		for (int i = 0; i < neighborCount.size(); i++)
		{
			neighborCount[i].resize(settings.GetGridHeight());
		}

		ResetGame();
		fileStream.close();
	}
	fileStream.open(fileName);
	if (fileStream.is_open()) {
		int index = 0;
		while (!fileStream.eof())
		{
			std::string buffer;
			std::getline(fileStream, buffer);
			if (buffer[0] == '!')
			{
				continue;
			}
			for (int i = 0; i < buffer.size(); i++)
			{
				if (buffer[i] == '*')
				{
					gameBoard[i][index] = true;
				}
				else if (buffer[i] == '.')
				{
					gameBoard[i][index] = false;
				}

			}
			index++;
		}
		fileStream.close();
	}

	CountAllNeighbors();
	UpdateStatusText();
	panel->Refresh();
	evt.Skip();
}

void MainWindow::OnExitClick(wxCommandEvent& evt)
{
	Close();
}

void MainWindow::OnShowGrid(wxCommandEvent& evt)
{
	if (settings.GetShowGrid())
	{
		viewMenu->Check(10010, false);
		settings.SetShowGrid(false);
	}
	else
	{
		viewMenu->Check(10010, true);
		settings.SetShowGrid(true);
	}

	settings.SaveData();	//Saves data to the settings file
	panel->Refresh();		//Repaints the panel
	evt.Skip();
}

void MainWindow::OnShowDividers(wxCommandEvent& evt)
{
	if (settings.GetShowDividers())
	{
		viewMenu->Check(10011, false);
		settings.SetShowDividers(false);
	}
	else
	{
		viewMenu->Check(10011, true);
		settings.SetShowDividers(true);
	}

	settings.SaveData();	//Saves data to the settings file
	panel->Refresh();		//Repaints the panel
	evt.Skip();
}

void MainWindow::OnFinite(wxCommandEvent& evt)
{
	if (settings.GetFinite())
	{
		viewMenu->Check(10012, false);
		viewMenu->Check(10013, true);
	}
	else
	{
		viewMenu->Check(10012, false);
		viewMenu->Check(10013, true);
		settings.SetToroidal(false);
		settings.SetFinite(true);
	}
	settings.SaveData();	//Saves data to the settings file
	panel->Refresh();		//Repaints the panel
	evt.Skip();
}

void MainWindow::OnToroidal(wxCommandEvent& evt)
{
	if (settings.GetToroidal())
	{
		viewMenu->Check(10012, true);
		viewMenu->Check(10013, false);
	}
	else
	{
		viewMenu->Check(10012, true);
		viewMenu->Check(10013, false);
		settings.SetToroidal(true);
		settings.SetFinite(false);
	}
	settings.SaveData();	//Saves data to the settings file
	panel->Refresh();		//Repaints the panel
	evt.Skip();
}

void MainWindow::OnResetSettings(wxCommandEvent& evt)
{

	//Threw an exception that I cannot recreate...

	if (timer->IsRunning())
	{
		timer->Stop();
	}
	ResetGame();
	ClearNeighborCount();
	settings.SetAliveColor(wxColor(128, 128, 128, 255));
	settings.SetDeadColor(wxColor(255, 255, 255, 255));
	settings.SetGridHeight(15);
	settings.SetGridWidth(15);
	settings.SetInterval(50);
	settings.SetShowNeighbors(false);
	viewMenu->Check(10006, false);
	settings.SetShowGrid(true);
	viewMenu->Check(10010, true);
	settings.SetShowDividers(true);
	viewMenu->Check(10011, true);
	settings.SetToroidal(false);
	viewMenu->Check(10012, false);
	settings.SetFinite(true);
	viewMenu->Check(10013, true);
	settings.SetShowHud(true);
	viewMenu->Check(10016, true);

	settings.SaveData();
	panel->Refresh();
	evt.Skip();
}

void MainWindow::OnImport(wxCommandEvent& evt)
{
	if (timer->IsRunning())
	{
		timer->Stop();
	}

	wxFileDialog fileDialog(this, "Open Game of Life file", wxEmptyString, wxEmptyString, "Game of Life files (*.cells) | *.cells", wxFD_OPEN);

	if (fileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	std::ifstream fileStream;
	fileName = fileDialog.GetPath();
	fileStream.open(fileName);
	if (fileStream.is_open()) {
		ResetGame();
		int index = 0;
		while (!fileStream.eof() && index < gameBoard.size())
		{
			std::string buffer;
			std::getline(fileStream, buffer);

			if (buffer[0] == '!')
			{
				continue;
			}

			for (int i = 0; i < gameBoard.size(); i++)
			{
				if (buffer.size() >= i)
				{
					if (buffer[i] == '*')
					{
						gameBoard[i][index] = true;
					}
					else if (buffer[i] == '.')
					{
						gameBoard[i][index] = false;
					}
				}
				else
				{
					break;
				}
			}
			index++;
		}
		fileStream.close();
	}

	CountAllNeighbors();
	UpdateStatusText();
	panel->Refresh();
	evt.Skip();
}

void MainWindow::OnShowHud(wxCommandEvent& evt)
{
	if (settings.GetShowHud())
	{
		viewMenu->Check(10016, false);
		settings.SetShowHud(false);
	}
	else
	{
		viewMenu->Check(10016, true);
		settings.SetShowHud(true);
	}

	settings.SaveData();	//Saves data to the settings file
	panel->Refresh();		//Repaints the panel
	evt.Skip();
}
