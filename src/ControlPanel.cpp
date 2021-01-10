#include "ControlPanel.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/listbase.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <string>
#include <functional>
#include <wx/event.h>
#include <core/Database.h>
#include <fstream>
#include <extensions/jumper/AppDelegate.h>
#include <extensions/wheels/AppDelegate.h>
#include <extensions/flappers/AppDelegate.h>
#include <core/Session.h>
#include "cocos2d.h"

#define EXTENSIONS_LIST_BOX_ID 1
#define SESSIONS_LIST_BOX_ID 2
#define GENERATIONS_LIST_BOX_ID 3
#define START_BUTTON_ID 4
#define CLEAR_SELECTION_BUTTON_ID 5
#define OPEN_PORTAL_BUTTON_ID 6
#define POPULATION_SIZE_SPIN_BUTTON_ID 7
#define ELITE_FRACTION_SPIN_BUTTON_ID 8
#define FERTILE_FRACTION_SPIN_BUTTON_ID 9
#define RANDOM_FRACTION_SPIN_BUTTON_ID 10
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
    // Open the database
    std::ifstream dbPathFile;
    dbPathFile.open(cocos2d::FileUtils::getInstance()->fullPathForFilename("db_path.txt"));
    dbPathFile >> this->dbPath;
    dbPathFile.close();

    Core::Database::open(this->dbPath);

    // Initialization
    this->sessionIds = std::vector<int>{};
    this->generationIds = std::vector<int>{};

    // Auto-start an extension
    // this->Destroy();
    // this->Close(true);
    // Flappers::AppDelegate app;
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

    auto extensionNames_ = Core::Database::getExtensionNames();
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

    // Settings row
    auto settingsPanel = new wxPanel(masterPanel);
    masterPanelSizer->Add(settingsPanel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    auto settingsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, settingsPanel, "Settings");
    settingsPanel->SetSizer(settingsStaticBoxSizer);

    // Grid panel
    auto gridPanel = new wxPanel(settingsPanel);
    settingsStaticBoxSizer->Add(gridPanel, 0, wxEXPAND);
    auto gridPanelSizer = new wxFlexGridSizer(4, wxSize(10, 10));
    gridPanel->SetSizer(gridPanelSizer);

    // Mutation Rate
    auto mutationRateText = new wxStaticText(gridPanel, wxID_ANY, wxT("Mutation Rate (%): "));
    gridPanelSizer->Add(mutationRateText);

    this->mutationRate = new wxSpinCtrlDouble(gridPanel, wxID_ANY);
    this->mutationRate->SetIncrement(1);
    this->mutationRate->SetDigits(1);
    this->mutationRate->SetRange(0, 100);
    this->mutationRate->SetValue(1);
    gridPanelSizer->Add(this->mutationRate);

    // Elite Fraction
    auto eliteFractionText = new wxStaticText(gridPanel, wxID_ANY, wxT("Elite Fraction (%): "));
    gridPanelSizer->Add(eliteFractionText, 0, wxLEFT | wxEXPAND, 0);

    this->eliteFraction = new wxSpinCtrlDouble(gridPanel, ELITE_FRACTION_SPIN_BUTTON_ID);
    this->eliteFraction->SetIncrement(1);
    this->eliteFraction->SetDigits(0);
    this->eliteFraction->SetRange(0, 100);
    this->eliteFraction->SetValue(5);
    gridPanelSizer->Add(this->eliteFraction);

    // Speed
    auto speedText = new wxStaticText(gridPanel, wxID_ANY, wxT("Speed: "));
    gridPanelSizer->Add(speedText, 0, wxLEFT | wxEXPAND, 0);

    this->speed = new wxSpinCtrlDouble(gridPanel, wxID_ANY);
    this->speed->SetIncrement(.1);
    this->speed->SetDigits(1);
    this->speed->SetRange(0, 100);
    this->speed->SetValue(1);
    gridPanelSizer->Add(this->speed);

    // Fertile Fraction
    auto fertileFractionText = new wxStaticText(gridPanel, wxID_ANY, wxT("Fertile Fraction (%): "));
    gridPanelSizer->Add(fertileFractionText, 0, wxLEFT | wxEXPAND, 0);

    this->fertileFraction = new wxSpinCtrlDouble(gridPanel, FERTILE_FRACTION_SPIN_BUTTON_ID);
    this->fertileFraction->SetIncrement(1);
    this->fertileFraction->SetDigits(0);
    this->fertileFraction->SetRange(0, 100);
    this->fertileFraction->SetValue(30);
    gridPanelSizer->Add(this->fertileFraction);

    // Population size
    auto populationSizeText = new wxStaticText(gridPanel, wxID_ANY, wxT("Population Size: "));
    gridPanelSizer->Add(populationSizeText, 0, wxLEFT | wxEXPAND, 0);

    this->populationSize = new wxSpinCtrlDouble(gridPanel, POPULATION_SIZE_SPIN_BUTTON_ID);
    this->populationSize->SetIncrement(1);
    this->populationSize->SetDigits(0);
    this->populationSize->SetRange(0, 10000);
    this->populationSize->SetValue(50);
    gridPanelSizer->Add(this->populationSize);

    // Random Fraction
    auto randomFractionText = new wxStaticText(gridPanel, wxID_ANY, wxT("Random Fraction (%): "));
    gridPanelSizer->Add(randomFractionText, 0, wxLEFT | wxEXPAND, 0);

    this->randomFraction = new wxSpinCtrlDouble(gridPanel, RANDOM_FRACTION_SPIN_BUTTON_ID);
    this->randomFraction->SetIncrement(1);
    this->randomFraction->SetDigits(0);
    this->randomFraction->SetRange(0, 100);
    this->randomFraction->SetValue(15);
    gridPanelSizer->Add(this->randomFraction);

    // Summary row
    auto summaryRow = new wxPanel(masterPanel);
    masterPanelSizer->Add(summaryRow, 0, wxEXPAND | wxRIGHT, 10);
    auto summaryRowSizer = new wxBoxSizer(wxHORIZONTAL);
    summaryRow->SetSizer(summaryRowSizer);

    this->summary = new wxStaticText(summaryRow, wxID_ANY, wxT(""));
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

    // Open Portal
    this->openPortal = new wxButton(actionsRow, OPEN_PORTAL_BUTTON_ID, wxT("Open Portal"));
    actionsRowSizer->Add(this->openPortal, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

    this->updateSummary();
    frameSizer->Fit(this);

    // Events
    Bind(wxEVT_LISTBOX, &ControlPanelFrame::SelectExtension, this, EXTENSIONS_LIST_BOX_ID);
    Bind(wxEVT_LISTBOX, &ControlPanelFrame::SelectSession, this, SESSIONS_LIST_BOX_ID);
    Bind(wxEVT_LISTBOX, &ControlPanelFrame::SelectGeneration, this, GENERATIONS_LIST_BOX_ID);
    Bind(wxEVT_BUTTON, &ControlPanelFrame::StartEvolution, this, START_BUTTON_ID);
    Bind(wxEVT_BUTTON, &ControlPanelFrame::ClearSelection, this, CLEAR_SELECTION_BUTTON_ID);
    Bind(wxEVT_BUTTON, &ControlPanelFrame::OpenPortal, this, OPEN_PORTAL_BUTTON_ID);
    Bind(wxEVT_SPINCTRLDOUBLE, &ControlPanelFrame::UpdatePopulationSize, this, POPULATION_SIZE_SPIN_BUTTON_ID);
    Bind(wxEVT_SPINCTRLDOUBLE, &ControlPanelFrame::UpdateEliteFraction, this, ELITE_FRACTION_SPIN_BUTTON_ID);
    Bind(wxEVT_SPINCTRLDOUBLE, &ControlPanelFrame::UpdateFertileFraction, this, FERTILE_FRACTION_SPIN_BUTTON_ID);
    Bind(wxEVT_SPINCTRLDOUBLE, &ControlPanelFrame::UpdateRandomFraction, this, RANDOM_FRACTION_SPIN_BUTTON_ID);
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
    auto ids = Core::Database::getSessionIds(extensionName);
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
    auto ids = Core::Database::getGenerationIds(sessionId);
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

    Core::Session::sessionId = sessionId;
    Core::Session::generationId = generationId;

    try
    {
        Core::Session::generationIndex = std::stoi(this->generationsListBox->GetStringSelection().ToStdString()) + 1;
    }
    catch (...)
    {
        Core::Session::generationIndex = 1;
    }

    Core::Session::speed = this->speed->GetValue();
    Core::Session::populationSize = this->populationSize->GetValue();
    Core::Session::mutationRate = this->mutationRate->GetValue() / 100;
    Core::Session::eliteFraction = this->eliteFraction->GetValue() / 100;
    Core::Session::fertileFraction = this->fertileFraction->GetValue() / 100;
    Core::Session::randomFraction = this->randomFraction->GetValue() / 100;

    int eliteCount = floor(this->populationSize->GetValue() * (this->eliteFraction->GetValue() / 100));
    int randomCount = floor(this->populationSize->GetValue() * (this->randomFraction->GetValue() / 100));
    int offspringCount = this->populationSize->GetValue() - randomCount - eliteCount;

    if (this->eliteFraction->GetValue() + this->randomFraction->GetValue() > 100)
    {
        wxMessageBox("Elite Fraction + Random Fraction exceeds 100%.", "Invalid Fractions", wxICON_ERROR | wxOK | wxCENTRE, this);
        return;
    }
    else
    {
        if (this->fertileFraction->GetValue() > 0 && offspringCount <= 0)
        {
            wxMessageBox("You've set a non-zero fertile fraction, yet there is no room for reproduction since the elite and random members comprise the entirety of the population.", "No Room for Reproduction", wxICON_EXCLAMATION | wxOK | wxCENTRE, this);
        }

        this->Destroy();
        this->Close(true);

        if (extensionName == "Jumper")
        {
            Jumper::AppDelegate app;
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

void ControlPanelFrame::OpenPortal(wxCommandEvent &event)
{
    system(("cd " + dbPath.substr(0, dbPath.find("portal") + 6) + "; php artisan serve &").c_str());
    wxLaunchDefaultBrowser("http://127.0.0.1:8000");
}

void ControlPanelFrame::UpdatePopulationSize(wxCommandEvent &event)
{
    this->updateSummary();
}

void ControlPanelFrame::UpdateEliteFraction(wxCommandEvent &event)
{
    this->updateSummary();
}

void ControlPanelFrame::UpdateFertileFraction(wxCommandEvent &event)
{
    this->updateSummary();
}

void ControlPanelFrame::UpdateRandomFraction(wxCommandEvent &event)
{
    this->updateSummary();
}

void ControlPanelFrame::updateSummary()
{
    std::string text;

    int eliteCount = floor(this->populationSize->GetValue() * (this->eliteFraction->GetValue() / 100));
    int randomCount = floor(this->populationSize->GetValue() * (this->randomFraction->GetValue() / 100));
    int offspringCount = this->populationSize->GetValue() - randomCount - eliteCount;

    std::ostringstream out;
    out << "Elite Members: " << eliteCount << ", Offsprings: " << offspringCount << ", Random Members: " << randomCount << ".\n\n";
    text = out.str();

    if (offspringCount == 0 && this->fertileFraction->GetValue() > 0)
        text += "Warning: No offspring will be produced across the generations despite the fertile fraction being non-zero.\n\n";

    if (extensionsListBox->GetSelection() == wxNOT_FOUND)
    {
        text += "No Selection.";
    }
    else
    {
        auto extensionName = extensionsListBox->GetStringSelection().ToStdString();

        if (sessionsListBox->GetSelection() == wxNOT_FOUND)
            text += "Evolution of " + extensionName + " will begin with a new session.";
        else if (generationsListBox->GetSelection() == wxNOT_FOUND)
            text += "Evolution of " + extensionName + " will continue from " + sessionsListBox->GetStringSelection() + " session.\n\nWarning: This will remove all existing generations in the session.";
        else
            text += "Evolution of " + extensionName + " will continue from " + generationsListBox->GetStringSelection() + " generation of  " + sessionsListBox->GetStringSelection() + " session.";
    }

    this->summary->SetLabel(wxString(text));
    this->GetSizer()->Fit(this);
}
