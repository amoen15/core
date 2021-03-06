/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#include <protectiondlg.hxx>
#include <tabprotection.hxx>

#include <sal/macros.h>
#include <vector>

namespace {

// The order must match that of the list box.
const std::vector<ScTableProtection::Option> aOptions = {
    ScTableProtection::SELECT_LOCKED_CELLS,
    ScTableProtection::SELECT_UNLOCKED_CELLS,
    ScTableProtection::INSERT_COLUMNS,
    ScTableProtection::INSERT_ROWS,
    ScTableProtection::DELETE_COLUMNS,
    ScTableProtection::DELETE_ROWS,
};

}

ScTableProtectionDlg::ScTableProtectionDlg(vcl::Window* pParent)
    : ModalDialog( pParent, "ProtectSheetDialog", "modules/scalc/ui/protectsheetdlg.ui" )
{
    get(m_pPasswords, "passwords");
    get(m_pOptions, "options");
    get(m_pBtnProtect, "protect");
    get(m_pOptionsListBox, "checklist");
    get(m_pPassword1Edit, "password1");
    get(m_pPassword2Edit, "password2");
    get(m_pBtnOk, "ok");

    m_aSelectLockedCells = get<FixedText>("protected")->GetText();
    m_aSelectUnlockedCells = get<FixedText>("unprotected")->GetText();
    m_aInsertColumns = get<FixedText>("insert-columns")->GetText();
    m_aInsertRows = get<FixedText>("insert-rows")->GetText();
    m_aDeleteColumns = get<FixedText>("delete-columns")->GetText();
    m_aDeleteRows = get<FixedText>("delete-rows")->GetText();

    Init();
}

ScTableProtectionDlg::~ScTableProtectionDlg()
{
    disposeOnce();
}

void ScTableProtectionDlg::dispose()
{
    m_pBtnProtect.clear();
    m_pPasswords.clear();
    m_pOptions.clear();
    m_pPassword1Edit.clear();
    m_pPassword2Edit.clear();
    m_pOptionsListBox.clear();
    m_pBtnOk.clear();
    ModalDialog::dispose();
}

void ScTableProtectionDlg::SetDialogData(const ScTableProtection& rData)
{
    for (size_t i = 0; i < aOptions.size(); ++i)
        m_pOptionsListBox->CheckEntryPos(i, rData.isOptionEnabled(aOptions[i]));
}

void ScTableProtectionDlg::WriteData(ScTableProtection& rData) const
{
    rData.setProtected(m_pBtnProtect->IsChecked());

    // We assume that the two password texts match.
    rData.setPassword(m_pPassword1Edit->GetText());

    for (size_t i = 0; i < aOptions.size(); ++i)
        rData.setOption(aOptions[i], m_pOptionsListBox->IsChecked(i));
}

void ScTableProtectionDlg::Init()
{
    m_pBtnProtect->SetClickHdl(LINK( this, ScTableProtectionDlg, CheckBoxHdl ));

    m_pBtnOk->SetClickHdl(LINK( this, ScTableProtectionDlg, OKHdl ));

    Link<Edit&,void> aLink = LINK( this, ScTableProtectionDlg, PasswordModifyHdl );
    m_pPassword1Edit->SetModifyHdl(aLink);
    m_pPassword2Edit->SetModifyHdl(aLink);

    m_pOptionsListBox->SetUpdateMode(false);
    m_pOptionsListBox->Clear();

    m_pOptionsListBox->InsertEntry(m_aSelectLockedCells);
    m_pOptionsListBox->InsertEntry(m_aSelectUnlockedCells);
    m_pOptionsListBox->InsertEntry(m_aInsertColumns);
    m_pOptionsListBox->InsertEntry(m_aInsertRows);
    m_pOptionsListBox->InsertEntry(m_aDeleteColumns);
    m_pOptionsListBox->InsertEntry(m_aDeleteRows);

    m_pOptionsListBox->CheckEntryPos(0);
    m_pOptionsListBox->CheckEntryPos(1);

    m_pOptionsListBox->SetUpdateMode(true);

    // Set the default state of the dialog.
    m_pBtnProtect->Check();
    m_pPassword1Edit->GrabFocus();
}

void ScTableProtectionDlg::EnableOptionalWidgets(bool bEnable)
{
    m_pPasswords->Enable(bEnable);
    m_pOptions->Enable(bEnable);
    m_pOptionsListBox->Invalidate();
}

IMPL_LINK( ScTableProtectionDlg, CheckBoxHdl, Button*, pBtn, void )
{
    if (pBtn == m_pBtnProtect)
    {
        bool bChecked = m_pBtnProtect->IsChecked();
        EnableOptionalWidgets(bChecked);
        m_pBtnOk->Enable(bChecked);
    }
}

IMPL_LINK_NOARG(ScTableProtectionDlg, OKHdl, Button*, void)
{
    EndDialog(RET_OK);
}

IMPL_LINK_NOARG(ScTableProtectionDlg, PasswordModifyHdl, Edit&, void)
{
    OUString aPass1 = m_pPassword1Edit->GetText();
    OUString aPass2 = m_pPassword2Edit->GetText();
    m_pBtnOk->Enable(aPass1 == aPass2);
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
