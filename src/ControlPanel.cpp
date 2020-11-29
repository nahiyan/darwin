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
#include <extensions/wheels/AppDelegate.h>
#include <extensions/flappers/AppDelegate.h>
#include <core/CoreSession.h>
#include "cocos2d.h"

#define EXTENSIONS_LIST_BOX_ID 1
#define SESSIONS_LIST_BOX_ID 2
#define GENERATIONS_LIST_BOX_ID 3
#define START_BUTTON_ID 4
#define CLEAR_SELECTION_BUTTON_ID 5
#define LIST_BOX_MIN_WIDTH 170
#define LIST_BOX_MIN_HEIGHT 300

IMPLEMENT_APP_NO_MAIN(ControlPanel);
IMPLEMENT_WX_THEME_SUPPORT;

std::string ordinal(int n)
{
    static const char suffixes[][3] = {"th", "st", "nd", "rd"};
    auto ord = n % 100;
    if (ord / 10 == 1)
        ord = 0;
    ord = ord % 10;
    if (ord > 3)
        ord = 0;
    return std::to_string(n) + suffixes[ord];
}

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
    std::ifstream dbPathFile;
    dbPathFile.open(cocos2d::FileUtils::getInstance()->fullPathForFilename("db_path.txt"));
    std::string dbPath;
    dbPathFile >> dbPath;
    dbPathFile.close();

    Database::open(dbPath);

    // Initialization
    this->sessionIds = std::vector<int>{};
    this->generationIds = std::vector<int>{};

    // Auto-start an extension
    // this->Destroy();
    // this->Close(true);
    // Wheels::AppDelegate app;
    // cocos2d::Application::getInstance()->run();

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

    auto extensionNames_ = Database::getExtensionNames();
    for (auto extensionName : extensionNames_)
        this->extensionNames.push_back(extensionName);

    this->extensionsListBox->Append(this->extensionNames);
    this->extensionsListBox->SetMinSize(wxSize(LIST_BOX_MIN_WIDTH, LIST_BOX_MIN_HEIGHT));
    extensionsStaticBoxSizer->Add(this->extensionsListBox, 0, wxEXPAND | wxALL, 5);

    // Sessions
    auto sessionsPanel = new wxPanel(selectionRow);
    selectionRowSizer->Add(sessionsPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    auto sessionsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, sessionsPanel, "Sessions");
    sessionsPanel->SetSizer(sessionsStaticBoxSizer);

    this->sessionsListBox = new wxListBox(sessionsPanel, SESSIONS_LIST_BOX_ID);
    this->sessionsListBox->SetMinSize(wxSize(LIST_BOX_MIN_WIDTH, LIST_BOX_MIN_HEIGHT));
    sessionsListBox->Enable(false);
    sessionsStaticBoxSizer->Add(this->sessionsListBox, 0, wxEXPAND | wxALL, 5);

    // Generations
    auto generationsPanel = new wxPanel(selectionRow);
    selectionRowSizer->Add(generationsPanel, 0, wxEXPAND | wxALL, 10);
    auto generationsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, generationsPanel, "Generations");
    generationsPanel->SetSizer(generationsStaticBoxSizer);

    this->generationsListBox = new wxListBox(generationsPanel, GENERATIONS_LIST_BOX_ID);
    this->generationsListBox->SetMinSize(wxSize(LIST_BOX_MIN_WIDTH, LIST_BOX_MIN_HEIGHT));
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
    this->sessionsListBox->Clear();
    this->generationsListBox->Clear();
    this->generationsListBox->Enable(false);

    auto extensionName = event.GetString().ToStdString();
    auto ids = Database::getSessionIds(extensionName);
    std::copy(ids.begin(), ids.end(), std::back_inserter(this->sessionIds));

    std::vector<wxString> items;
    for (int i = ids.size(); i > 0; i--)
        items.push_back(wxString(ordinal(i)));

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
    this->generationsListBox->Clear();

    auto extensionName = this->extensionsListBox->GetStringSelection().ToStdString();
    auto sessionId = this->sessionIds[event.GetSelection()];
    auto ids = Database::getGenerationIds(sessionId);
    std::copy(ids.begin(), ids.end(), std::back_inserter(this->generationIds));

    std::vector<wxString> items;
    for (int i = ids.size(); i > 0; i--)
        items.push_back(wxString(ordinal(i)));

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
        sessionId = this->sessionIds[this->sessionsListBox->GetSelection()];
    if (this->generationsListBox->GetStringSelection().ToStdString().size() != 0)
        generationId = this->generationIds[this->generationsListBox->GetSelection()];

    CoreSession::sessionId = sessionId;
    CoreSession::generationId = generationId;

    this->Destroy();
    this->Close(true);

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
    else if (extensionName == "Wheels")
    {
        Wheels::AppDelegate app;
        cocos2d::Application::getInstance()->run();
    }
    else if (extensionName == "Flappers")
    {
        Flappers::AppDelegate app;
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
            text = "Evolution of " + extensionName + " will continue from " + sessionsListBox->GetStringSelection() + " session.\nWarning: This will remove all existing generations in the session.";
        else
            text = "Evolution of " + extensionName + " will continue from " + generationsListBox->GetStringSelection() + " generation of  " + sessionsListBox->GetStringSelection() + " session.";
    }

    this->summary->SetLabel(wxString(text));
    this->GetSizer()->Fit(this);
}
