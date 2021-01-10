#ifndef __MAIN_H__
#define __MAIN_H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/spinctrl.h>

class ControlPanel : public wxApp
{
public:
    virtual bool OnInit();
};

class ControlPanelFrame : public wxFrame
{
public:
    ControlPanelFrame();

private:
    std::string dbPath;

    // widgets
    wxListBox *extensionsListBox;
    wxListBox *sessionsListBox;
    wxListBox *generationsListBox;
    wxButton *startButton;
    wxButton *clearSelectionButton;
    wxButton *openPortal;
    wxStaticText *summary;
    wxSpinCtrlDouble *mutationRate, *speed, *populationSize, *eliteFraction, *fertileFraction, *randomFraction;

    // lists
    std::vector<wxString> extensionNames;
    std::vector<int> sessionIds;
    std::vector<int> generationIds;

    void OnExit(wxCommandEvent &event);
    void SelectExtension(wxCommandEvent &event);
    void SelectSession(wxCommandEvent &event);
    void SelectGeneration(wxCommandEvent &event);
    void StartEvolution(wxCommandEvent &event);
    void ClearSelection(wxCommandEvent &event);
    void OpenPortal(wxCommandEvent &event);
    void UpdatePopulationSize(wxCommandEvent &event);
    void UpdateEliteFraction(wxCommandEvent &event);
    void UpdateFertileFraction(wxCommandEvent &event);
    void UpdateRandomFraction(wxCommandEvent &event);
    void updateSummary();
};

#endif // __MAIN_H__
