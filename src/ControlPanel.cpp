#include "ControlPanel.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/listbase.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <string>
#include <functional>
#include <wx/event.h>
#include <core/Database.h>
#include <fstream>
#include <extensions/jumper/AppDelegate.h>
#include <extensions/predation/AppDelegate.h>
#include <core/CoreSession.h>

#define EXTENSIONS_LIST_BOX_ID 1
#define SESSIONS_LIST_BOX_ID 2
#define GENERATIONS_LIST_BOX_ID 3
#define START_BUTTON_ID 4
#define CLEAR_SELECTION_BUTTON_ID 5

IMPLEMENT_APP_NO_MAIN(ControlPanel);
IMPLEMENT_WX_THEME_SUPPORT;

bool ControlPanel::OnInit()
{
    auto frame = new ControlPanelFrame();
    frame->Show(true);
    frame->Centre(wxBOTH);

    return true;
}

ControlPanelFrame::ControlPanelFrame()
    : wxFrame(NULL, wxID_ANY, wxT("Darwin"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
    Database::open("../../../../portal/database/database.db");
    auto extensionNames = Database::getExtensionNames();

    auto menuBar = new wxMenuBar();
    SetMenuBar(menuBar);

    // Frame sizer
    auto frameSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(frameSizer);

    // Master panel
    auto masterPanel = new wxPanel(this);
    auto masterPanelSizer = new wxBoxSizer(wxVERTICAL);
    masterPanel->SetSizer(masterPanelSizer);
    frameSizer->Add(masterPanel);

    // Selection row
    auto selectionRow = new wxPanel(masterPanel);
    auto selectionRowSizer = new wxBoxSizer(wxHORIZONTAL);
    selectionRow->SetSizer(selectionRowSizer);
    masterPanelSizer->Add(selectionRow);

    // Extensions
    auto extensionsPanel = new wxPanel(selectionRow);
    selectionRowSizer->Add(extensionsPanel, 0, wxEXPAND | wxALL, 10);
    auto extensionsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, extensionsPanel, "Extensions");
    extensionsPanel->SetSizer(extensionsStaticBoxSizer);

    this->extensionsListBox = new wxListBox(extensionsPanel, EXTENSIONS_LIST_BOX_ID);
    std::vector<wxString> extensions;
    for (auto extensionName : extensionNames)
    {
        extensions.push_back(extensionName);
    }
    this->extensionsListBox->Append(extensions);
    extensionsStaticBoxSizer->Add(this->extensionsListBox, 0, wxEXPAND | wxALL, 5);

    // Sessions
    auto sessionsPanel = new wxPanel(selectionRow);
    selectionRowSizer->Add(sessionsPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    auto sessionsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, sessionsPanel, "Sessions");
    sessionsPanel->SetSizer(sessionsStaticBoxSizer);

    this->sessionsListBox = new wxListBox(sessionsPanel, SESSIONS_LIST_BOX_ID);
    sessionsListBox->Enable(false);
    sessionsStaticBoxSizer->Add(this->sessionsListBox, 0, wxEXPAND | wxALL, 5);

    // Generations
    auto generationsPanel = new wxPanel(selectionRow);
    selectionRowSizer->Add(generationsPanel, 0, wxEXPAND | wxALL, 10);
    auto generationsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, generationsPanel, "Generations");
    generationsPanel->SetSizer(generationsStaticBoxSizer);

    this->generationsListBox = new wxListBox(generationsPanel, GENERATIONS_LIST_BOX_ID);
    generationsListBox->Enable(false);
    generationsStaticBoxSizer->Add(this->generationsListBox, 0, wxEXPAND | wxALL, 5);

    // Summary row
    auto summaryRow = new wxPanel(masterPanel);
    masterPanelSizer->Add(summaryRow, 0, wxEXPAND | wxRIGHT, 10);
    auto summaryRowSizer = new wxBoxSizer(wxHORIZONTAL);
    summaryRow->SetSizer(summaryRowSizer);

    this->summary = new wxStaticText(summaryRow, wxID_ANY, wxT("No selection."));
    summaryRowSizer->Add(this->summary, 0, wxLEFT, 10);

    // Actions row
    auto actionsRow = new wxPanel(masterPanel);
    masterPanelSizer->Add(actionsRow);
    auto actionsRowSizer = new wxBoxSizer(wxHORIZONTAL);
    actionsRow->SetSizer(actionsRowSizer);

    // Start button
    this->startButton = new wxButton(actionsRow, START_BUTTON_ID, wxT("Start"));
    this->startButton->Enable(false);
    actionsRowSizer->Add(this->startButton, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

    // Clear selection button
    this->clearSelectionButton = new wxButton(actionsRow, CLEAR_SELECTION_BUTTON_ID, wxT("Clear Selection"));
    this->clearSelectionButton->Enable(false);
    actionsRowSizer->Add(this->clearSelectionButton, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

    frameSizer->Fit(this);

    // Events
    Bind(wxEVT_LISTBOX, &ControlPanelFrame::SelectExtension, this, EXTENSIONS_LIST_BOX_ID);
    Bind(wxEVT_LISTBOX, &ControlPanelFrame::SelectSession, this, SESSIONS_LIST_BOX_ID);
    Bind(wxEVT_LISTBOX, &ControlPanelFrame::SelectGeneration, this, GENERATIONS_LIST_BOX_ID);
    Bind(wxEVT_BUTTON, &ControlPanelFrame::StartEvolution, this, START_BUTTON_ID);
    Bind(wxEVT_BUTTON, &ControlPanelFrame::ClearSelection, this, CLEAR_SELECTION_BUTTON_ID);
}

void ControlPanelFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void ControlPanelFrame::SelectExtension(wxCommandEvent &event)
{
    auto extensionName = event.GetString().ToStdString();
    auto ids = Database::getSessionIds(extensionName);
    this->sessionsListBox->Clear();
    this->generationsListBox->Clear();
    this->generationsListBox->Enable(false);

    std::vector<wxString> items;
    for (auto id : ids)
    {
        items.push_back(wxString(std::to_string(id)));
    }

    if (items.size() > 0)
        this->sessionsListBox->Enable(true);
    else
        this->sessionsListBox->Enable(false);

    this->sessionsListBox->Append(items);
    this->startButton->Enable(true);

    this->clearSelectionButton->Enable(true);

    this->updateSummary();
}

void ControlPanelFrame::SelectSession(wxCommandEvent &event)
{
    auto extensionName = this->extensionsListBox->GetStringSelection().ToStdString();

    auto sessionId = std::stoi(event.GetString().ToStdString());
    auto ids = Database::getGenerationIds(sessionId);
    this->generationsListBox->Clear();

    std::vector<wxString> items;
    for (auto id : ids)
    {
        items.push_back(wxString(std::to_string(id)));
    }

    if (items.size() > 0)
        this->generationsListBox->Enable(true);
    else
        this->generationsListBox->Enable(false);

    this->generationsListBox->Append(items);

    this->updateSummary();
}

void ControlPanelFrame::SelectGeneration(wxCommandEvent &event)
{
    this->updateSummary();
}

void ControlPanelFrame::StartEvolution(wxCommandEvent &event)
{
    auto extensionName = this->extensionsListBox->GetStringSelection().ToStdString();
    auto sessionId = 0;
    auto generationId = 0;

    if (this->sessionsListBox->GetStringSelection().ToStdString().size() != 0)
        sessionId = std::stoi(this->sessionsListBox->GetStringSelection().ToStdString());
    if (this->generationsListBox->GetStringSelection().ToStdString().size() != 0)
        generationId = std::stoi(this->generationsListBox->GetStringSelection().ToStdString());

    CoreSession::sessionId = sessionId;
    CoreSession::generationId = generationId;

    this->Destroy();

    if (extensionName == "Jumper")
    {
        Jumper::AppDelegate app;
        cocos2d::Application::getInstance()->run();
    }
    else if (extensionName == "Predation")
    {
        Predation::AppDelegate app;
        cocos2d::Application::getInstance()->run();
    }
}

void ControlPanelFrame::ClearSelection(wxCommandEvent &event)
{
    extensionsListBox->SetSelection(-1);

    sessionsListBox->SetSelection(-1);
    sessionsListBox->Clear();
    sessionsListBox->Enable(false);

    generationsListBox->SetSelection(-1);
    generationsListBox->Clear();
    generationsListBox->Enable(false);

    this->startButton->Enable(false);

    this->updateSummary();
}

void ControlPanelFrame::updateSummary()
{
    std::string text;

    if (extensionsListBox->GetSelection() == wxNOT_FOUND)
    {
        text = "No Selection.";
    }
    else
    {
        auto extensionName = extensionsListBox->GetStringSelection().ToStdString();

        if (sessionsListBox->GetSelection() == wxNOT_FOUND)
            text = "Evolution of " + extensionName + " will begin with a new session.";
        else if (generationsListBox->GetSelection() == wxNOT_FOUND)
            text = "Evolution of " + extensionName + " will continue from session " + sessionsListBox->GetStringSelection() + ".\nWarning: This will remove all existing generations in the session.";
        else
            text = "Evolution of " + extensionName + " will continue from generation " + generationsListBox->GetStringSelection() + " of session " + sessionsListBox->GetStringSelection() + ".";
    }

    this->summary->SetLabel(wxString(text));
    this->GetSizer()->Fit(this);
}