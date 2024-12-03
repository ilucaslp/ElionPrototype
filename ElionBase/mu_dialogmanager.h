#ifndef __MU_DIALOGMANAGER_H__
#define __MU_DIALOGMANAGER_H__

#pragma once

class MUDialogAnswerData
{
public:
	mu_string Answer;
	mu_int32 LinkForAnswer;
	mu_int32 ReturnForAnswer;
};

class MUDialogData
{
public:
	mu_string Text;
	std::vector<MUDialogAnswerData> Answers;
};

class MUDialogManager
{
public:
	MUDialogManager();

	mu_boolean Load(const mu_text *filename);

	const MUDialogData *GetDialog(mu_int32 dialogIndex);

public:
	std::vector<MUDialogData> _Dialogs;
	std::map<mu_int32, MUDialogData*> _DialogsMap;
};

extern MUDialogManager g_DialogManager;

#endif