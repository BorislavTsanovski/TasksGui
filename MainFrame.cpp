#include "MainFrame.h"
#include "Task.h"

MainFrame::MainFrame(const wxString& title) 
	: wxFrame(nullptr, wxID_ANY, title)
{
	CreateControls();
	BindEventHandlers();
	AddSavedTasks();
}

void MainFrame::CreateControls()
{
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));
	wxFont headLineFont(wxFontInfo(wxSize(0, 36)).Bold());

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	headlineText = new wxStaticText(panel, wxID_ANY, "To-DO List", wxPoint(0,22), wxSize(800, -1), wxALIGN_CENTER_HORIZONTAL);
	headlineText->SetFont(headLineFont);

	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 80), wxSize(495, 35), wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add", wxPoint(600, 80), wxSize(100, 35));
	checkListBox = new wxCheckListBox(panel, wxID_ANY, wxPoint(100, 120), wxSize(600, 400));
	clearButton = new wxButton(panel, wxID_ANY, "Clear", wxPoint(100, 525), wxSize(100, 35));
}

void MainFrame::BindEventHandlers()
{
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checkListBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeysDown, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
}

void MainFrame::AddSavedTasks()
{
	std::vector<Task> tasks = LoadTasksFromFile("tasks.txt");
	for (int i = 0; i < tasks.size(); i++) {
		checkListBox->Insert(tasks[i].description, i);
		checkListBox->Check(i, tasks[i].done);
	}
}

void MainFrame::OnAddButtonClicked(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnInputEnter(wxCommandEvent& evt)
{
	AddTaskFromInput();
}

void MainFrame::OnListKeysDown(wxKeyEvent& evt)
{
	switch (evt.GetKeyCode()) {
		case WXK_DELETE: DeleteSelectedTask(); break;
		case WXK_UP: MoveSelectedTask(-1); break;
		case WXK_DOWN: MoveSelectedTask(1); break;
	}
}

void MainFrame::OnClearButtonClicked(wxCommandEvent& evt)
{
	if (checkListBox->IsEmpty()) return;
	wxMessageDialog dialog(this, "Are you sure you want to clear all tasks?", "Clear", wxYES_NO | wxCANCEL);
	int result = dialog.ShowModal();
	if (result == wxID_YES)
	{
		checkListBox->Clear();
		inputField->SetFocus();
	}
}

void MainFrame::OnWindowClosed(wxCloseEvent& evt)
{
	int count = checkListBox->GetCount();
	std::vector<Task> tasks(count);
	for (int i = 0; i < count; i++) {
		tasks[i].description = checkListBox->GetString(i).ToStdString();
		tasks[i].done = checkListBox->IsChecked(i);
	}
	SaveTaskToFile(tasks, "tasks.txt");
	evt.Skip();
}

void MainFrame::AddTaskFromInput()
{
	wxString description = inputField->GetValue();
	if (!description.IsEmpty()) {
		checkListBox->Insert(description, checkListBox->GetCount());
		inputField->Clear();
	}

	inputField->SetFocus();
}

void MainFrame::DeleteSelectedTask()
{
	int selectedIndex = checkListBox->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		checkListBox->Delete(selectedIndex);
		int count = checkListBox->GetCount();
		if (!count) {
			inputField->SetFocus();
			return;
		}
		if (selectedIndex == count) selectedIndex--;
		checkListBox->SetSelection(selectedIndex);
	}
}

void MainFrame::MoveSelectedTask(int offset)
{
	int selectedIndex = checkListBox->GetSelection();
	if (selectedIndex == wxNOT_FOUND) return;
	int nextIndex = selectedIndex + offset;
	if (selectedIndex >= 0 && selectedIndex < checkListBox->GetCount()) {
		SwapTasks(selectedIndex, nextIndex);
	}
	checkListBox->SetSelection(nextIndex);
}

void MainFrame::SwapTasks(int i, int j)
{
	std::string tempDescr = checkListBox->GetString(i).ToStdString();
	bool done = checkListBox->IsChecked(i);

	checkListBox->SetString(i, checkListBox->GetString(j));
	checkListBox->Check(i, checkListBox->IsChecked(j));

	checkListBox->SetString(j, tempDescr);
	checkListBox->Check(j, done);
}
