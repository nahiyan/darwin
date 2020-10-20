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

    auto extensionsListBox = new wxListBox(extensionsPanel, 1);
    std::vector<wxString> extensions;
    for (int i = 0; i < 2; i++)
    {
        extensions.push_back("Hey " + std::to_string(i));
    }
    extensionsListBox->Append(extensions);
    extensionsStaticBoxSizer->Add(extensionsListBox, 0, wxEXPAND | wxALL, 5);

    // Sessions
    auto sessionsPanel = new wxPanel(firstRow);
    firstRowSizer->Add(sessionsPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    auto sessionsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, sessionsPanel, "Sessions");
    sessionsPanel->SetSizer(sessionsStaticBoxSizer);

    auto sessionsListBox = new wxListBox(sessionsPanel, 2);
    std::vector<wxString> sessions;
    for (int i = 0; i < 5; i++)
    {
        sessions.push_back(std::to_string(i));
    }
    sessionsListBox->Append(sessions);
    sessionsStaticBoxSizer->Add(sessionsListBox, 0, wxEXPAND | wxALL, 5);

    // Generations
    auto generationsPanel = new wxPanel(firstRow);
    firstRowSizer->Add(generationsPanel, 0, wxEXPAND | wxALL, 10);
    auto generationsStaticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, generationsPanel, "Generations");
    generationsPanel->SetSizer(generationsStaticBoxSizer);

    auto generationsListBox = new wxListBox(generationsPanel, 3);
    std::vector<wxString> generations;
    for (int i = 0; i < 20; i++)
    {
        generations.push_back(std::to_string(i));
    }
    generationsListBox->Append(generations);
    generationsStaticBoxSizer->Add(generationsListBox, 0, wxEXPAND | wxALL, 5);

    // Second row
    auto secondRow = new wxPanel(this);
    frameSizer->Add(secondRow);
    auto secondRowSizer = new wxBoxSizer(wxHORIZONTAL);
    secondRow->SetSizer(secondRowSizer);

    auto startButton = new wxButton(secondRow, wxID_ANY, wxT("Start"));
    startButton->Enable(false);
    secondRowSizer->Add(startButton, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 10);

    frameSizer->Fit(this);

    // Events
    Bind(wxEVT_LISTBOX, &ControlPanelFrame::SelectExtension, this, 1);
}

void ControlPanelFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}

void ControlPanelFrame::SelectExtension(wxCommandEvent &event)
{
    printf("%d ", event.GetSelection());
    printf("%s ", event.GetString().ToStdString().c_str());
}
