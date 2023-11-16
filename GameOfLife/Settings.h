#pragma once
#include "wx/wx.h"
#include <fstream>

struct Settings
{ 

private:
	unsigned int aliveRed = 128, aliveGreen = 128, aliveBlue = 128;
	unsigned int deadRed = 255, deadGreen = 255, deadBlue = 255;
	unsigned int aliveAlpha = 255, deadAlpha = 255;
	int gridHeight = 15;
	int gridWidth = 15;
	int interval = 50;
	bool ShowNeighborCount = false;
	bool showGrid = true;
	bool showDividers = true;
	bool toroidal = false;
	bool finite = true;
	bool showHud = true;
	
public:
	void SaveData()
	{
		std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
		file.write((char*)this, sizeof(Settings));
		file.close();
	}
	void LoadData()
	{
		std::ifstream file("settings.bin", std::ios::binary | std::ios::in);
		file.read((char*)this, sizeof(Settings));
		file.close();
	}

 
	//Getters: 
	bool GetShowHud()
	{
		return showHud;
	}

	bool GetToroidal()
	{
		return toroidal;
	}

	bool GetFinite()
	{
		return finite;
	}

	bool GetShowGrid()
	{
		return showGrid;
	}

	bool GetShowDividers()
	{
		return showDividers;
	}

	int GetGridHeight()
	{
		return gridHeight;
	}

	int GetGridWidth()
	{
		return gridWidth;
	}
	int GetInterval()
	{
		return interval;
	}

	bool GetShowNeighbors()
	{
		return ShowNeighborCount;
	}

	wxColor GetAliveColor()
	{
		return wxColor(aliveRed, aliveGreen,aliveBlue, aliveAlpha);
	}
	wxColor GetDeadColor() 
	{
		return wxColor(deadRed, deadGreen, deadBlue,deadAlpha);
	}

	
	//Setters:

	void SetShowHud(bool trueOrFalse)
	{
		showHud = trueOrFalse;
	}

	void SetToroidal(bool trueOrFalse)
	{
		toroidal = trueOrFalse;
	}

	void SetFinite(bool trueOrFalse)
	{
		finite = trueOrFalse;
	}

	void SetShowGrid(bool trueOrFalse)
	{
		showGrid = trueOrFalse;
	}
	
	void SetShowDividers(bool trueOrFalse)
	{
		showDividers = trueOrFalse;
	}

	void SetGridHeight(int newGridHeight)
	{
		gridHeight = newGridHeight;
	}

	void SetGridWidth(int newGridWidth)
	{
		gridWidth = newGridWidth;
	}

	void SetInterval(int newInterval)
	{
		interval = newInterval;
	}

	void SetShowNeighbors(bool trueOrFalse)
	{
		ShowNeighborCount = trueOrFalse;
	}

	void SetAliveColor(wxColor color)
	{
		aliveRed = color.GetRed();
		aliveGreen = color.GetGreen();
		aliveBlue = color.GetBlue();
		aliveAlpha = color.GetAlpha();
	}
	void SetDeadColor(wxColor color)
	{
		deadRed = color.GetRed();
		deadGreen = color.GetGreen();
		deadBlue = color.GetBlue();
		deadAlpha = color.GetAlpha();
	}

};