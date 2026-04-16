#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
	wxPanel* panel;
	wxStaticText* headlineText;
	wxTextCtrl* inputField;
	wxButton* addButton;
	wxCheckListBox* checkListBox;
	wxButton* clearButton;

public:
	MainFrame(const wxString& title);
	void CreateControls();
};