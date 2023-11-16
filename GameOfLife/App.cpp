#include "App.h"

wxIMPLEMENT_APP(App);

App::App() {

}

App::~App() {

}

bool App::OnInit() {

	windowPtr = new MainWindow();
	windowPtr->Show();

	return true;
}