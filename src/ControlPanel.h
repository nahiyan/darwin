#ifndef __MAIN_H__
#define __MAIN_H__

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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
    wxListBox *extensionsListBox;
    wxListBox *sessionsListBox;
    wxListBox *generationsListBox;
    wxButton *startButton;
    wxButton *clearSelectionButton;
    wxStaticText *summary;
    int initialWidth;

    void OnExit(wxCommandEvent &event);
    void SelectExtension(wxCommandEvent &event);
    void SelectSession(wxCommandEvent &event);
    void SelectGeneration(wxCommandEvent &event);
    void StartEvolution(wxCommandEvent &event);
    void ClearSelection(wxCommandEvent &event);
    void updateSummary();
};

#endif // __MAIN_H__
