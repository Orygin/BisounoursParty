#ifndef OPTIONPAGE_H
#define OPTIONPAGE_H
#pragma once

#include "vgui_controls/Frame.h"
#include <vgui/IVGui.h>
#include <vgui/ISurface.h>
#include <vgui\ISystem.h>
#include <vgui_controls/EditablePanel.h>

struct OptionLine
{
	char *Name;

};

class OptionsPage : public vgui::EditablePanel
{
public:
	OptionsPage();

	void AddItem(vgui::Panel *item);

private:
	CUtlVector<vgui::Panel> m_Componants;
};

#endif