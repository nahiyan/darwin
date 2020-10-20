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
    void OnExit(wxCommandEvent &event);
    void SelectExtension(wxCommandEvent &event);
};

#endif // __MAIN_H__
