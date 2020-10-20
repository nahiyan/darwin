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
#include "cocos2d.h"
#include <extensions/jumper/AppDelegate.h>

#define EXTENSIONS_LIST_BOX_ID 1
#define SESSIONS_LIST_BOX_ID 2
#define GENERATIONS_LIST_BOX_ID 3
#define START_BUTTON_ID 4

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
    // Database::open(cocos2d::FileUtils::getInstance()->fullPathForFilename("database.db"));
    Database::open("../../../../portal/database/database.db");
    auto extensionNames = Database::getExtensionNames();

    auto menuBar = new wxMenuBar();
    SetMenuBar(menuBar);

    // Frame sizer
    auto frameSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(frameSizer);

    // First row
    auto firstRow = new wxPanel(this);
    auto firstRowSizer = new wxBoxSizer(wxHORIZONTAL);
    firstRow->SetSizer(firstRowSizer);
    frameSizer->Add(firstRow);

    // Extensions
    auto extensionsPanel = new wxPanel(firstRow);
    firstRowSizer->Add(extensionsPanel, 0, wxEXPAND | wxALL, 10);
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
    auto sessionsPanel = new wxPanel(firstRow);
    firstRowSizer->Add(sessionsPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    auto sessionsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, sessionsPanel, "Sessions");
    sessionsPanel->SetSizer(sessionsStaticBoxSizer);

    this->sessionsListBox = new wxListBox(sessionsPanel, SESSIONS_LIST_BOX_ID);
    sessionsListBox->Enable(false);
    sessionsStaticBoxSizer->Add(this->sessionsListBox, 0, wxEXPAND | wxALL, 5);

    // Generations
    auto generationsPanel = new wxPanel(firstRow);
    firstRowSizer->Add(generationsPanel, 0, wxEXPAND | wxALL, 10);
    auto generationsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, generationsPanel, "Generations");
    generationsPanel->SetSizer(generationsStaticBoxSizer);

    this->generationsListBox = new wxListBox(generationsPanel, GENERATIONS_LIST_BOX_ID);
    generationsListBox->Enable(false);
    generationsStaticBoxSizer->Add(this->generationsListBox, 0, wxEXPAND | wxALL, 5);

    // Second row
    auto secondRow = new wxPanel(this);
    frameSizer->Add(secondRow);
    auto secondRowSizer = new wxBoxSizer(wxHORIZONTAL);
    secondRow->SetSizer(secondRowSizer);

    this->startButton = new wxButton(secondRow, START_BUTTON_ID, wxT("Start"));
    this->startButton->Enable(false);
    secondRowSizer->Add(this->startButton, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

    frameSizer->Fit(this);

    // Events
    Bind(wxEVT_LISTBOX, &ControlPanelFrame::SelectExtension, this, EXTENSIONS_LIST_BOX_ID);
    Bind(wxEVT_LISTBOX, &ControlPanelFrame::SelectSession, this, SESSIONS_LIST_BOX_ID);
    Bind(wxEVT_BUTTON, &ControlPanelFrame::StartEvolution, this, START_BUTTON_ID);
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
}

void ControlPanelFrame::SelectSession(wxCommandEvent &event)
{
    auto extensionName = this->extensionsListBox->GetStringSelection().ToStdString();

    auto sessionId = std::stoi(event.GetString().ToStdString());
    auto ids = Database::getGenerationIds(extensionName, sessionId);
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
}

void ControlPanelFrame::StartEvolution(wxCommandEvent &event)
{
    Jumper::AppDelegate app;
    cocos2d::Application::getInstance()->run();
}
