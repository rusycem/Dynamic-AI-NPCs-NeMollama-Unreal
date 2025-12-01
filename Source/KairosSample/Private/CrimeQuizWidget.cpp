// /* * SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved. * SPDX-License-Identifier: MIT * * Permission is hereby granted, free of charge, to any person obtaining a * copy of this software and associated documentation files (the "Software"), * to deal in the Software without restriction, including without limitation * the rights to use, copy, modify, merge, publish, distribute, sublicense, * and/or sell copies of the Software, and to permit persons to whom the * Software is furnished to do so, subject to the following conditions: * * The above copyright notice and this permission notice shall be included in * all copies or substantial portions of the Software. * * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER * DEALINGS IN THE SOFTWARE. */

#include "CrimeQuizWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UCrimeQuizWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (PersonAButton) PersonAButton->OnClicked.AddDynamic(this, &UCrimeQuizWidget::OnPersonASelected);
    if (PersonBButton) PersonBButton->OnClicked.AddDynamic(this, &UCrimeQuizWidget::OnPersonBSelected);
    if (PersonCButton) PersonCButton->OnClicked.AddDynamic(this, &UCrimeQuizWidget::OnPersonCSelected);
    if (ConfirmButton) ConfirmButton->OnClicked.AddDynamic(this, &UCrimeQuizWidget::OnConfirmPressed);

    // ✅ Use wrapper for binding
    if (VehicleComboBox)
        VehicleComboBox->OnSelectionChanged.AddDynamic(this, &UCrimeQuizWidget::OnComboBoxSelectionChanged);
    if (WeaponComboBox)
        WeaponComboBox->OnSelectionChanged.AddDynamic(this, &UCrimeQuizWidget::OnComboBoxSelectionChanged);
    if (WoundedComboBox)
        WoundedComboBox->OnSelectionChanged.AddDynamic(this, &UCrimeQuizWidget::OnComboBoxSelectionChanged);

    // Hide confirm at start
    if (ConfirmButton)
        ConfirmButton->SetVisibility(ESlateVisibility::Hidden);

    // Also hide result
    if (ResultText)
        ResultText->SetVisibility(ESlateVisibility::Hidden);
}

void UCrimeQuizWidget::OnPersonASelected()
{
    SelectedPerson = "A";
    UpdatePersonButtonStyles();
    CheckIfAllAnswersSelected();
}

void UCrimeQuizWidget::OnPersonBSelected()
{
    SelectedPerson = "B";
    UpdatePersonButtonStyles();
    CheckIfAllAnswersSelected();
}

void UCrimeQuizWidget::OnPersonCSelected()
{
    SelectedPerson = "C";
    UpdatePersonButtonStyles();
    CheckIfAllAnswersSelected();
}

void UCrimeQuizWidget::UpdatePersonButtonStyles()
{
    if (PersonAButton)
        PersonAButton->SetBackgroundColor(SelectedPerson == "A" ? SelectedColor : NormalColor);
    if (PersonBButton)
        PersonBButton->SetBackgroundColor(SelectedPerson == "B" ? SelectedColor : NormalColor);
    if (PersonCButton)
        PersonCButton->SetBackgroundColor(SelectedPerson == "C" ? SelectedColor : NormalColor);
}

void UCrimeQuizWidget::CheckIfAllAnswersSelected()
{
    if (!SelectedPerson.IsEmpty() &&
        VehicleComboBox && !VehicleComboBox->GetSelectedOption().IsEmpty() &&
        WeaponComboBox && !WeaponComboBox->GetSelectedOption().IsEmpty() &&
        WoundedComboBox && !WoundedComboBox->GetSelectedOption().IsEmpty())
    {
        if (ConfirmButton)
            ConfirmButton->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        if (ConfirmButton)
            ConfirmButton->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UCrimeQuizWidget::OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    CheckIfAllAnswersSelected();
}

void UCrimeQuizWidget::OnConfirmPressed()
{
    FString Feedback;

    // Check which answers are wrong — just state that they're wrong
    if (SelectedPerson != CorrectPerson)
        Feedback += TEXT("Suspect choice is incorrect.\n");

    FString SelectedVehicle = VehicleComboBox ? VehicleComboBox->GetSelectedOption() : TEXT("");
    if (SelectedVehicle != CorrectVehicle)
        Feedback += TEXT("Vehicle choice is incorrect.\n");

    FString SelectedWeapon = WeaponComboBox ? WeaponComboBox->GetSelectedOption() : TEXT("");
    if (SelectedWeapon != CorrectWeapon)
        Feedback += TEXT("Weapon choice is incorrect.\n");

    FString SelectedWounded = WoundedComboBox ? WoundedComboBox->GetSelectedOption() : TEXT("");
    if (SelectedWounded != CorrectWounded)
        Feedback += TEXT("Wounded status is incorrect.\n");

    int32 Score = GetScore();

    // Show either win or lose visual cue
    if (Score == 4)
    {
        Feedback += TEXT("\n✅ You solved the case!");
        if (WinImage) WinImage->SetVisibility(ESlateVisibility::Visible);
        if (LoseImage) LoseImage->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        Feedback += TEXT("\n❌ You failed to solve the case.");
        if (WinImage) WinImage->SetVisibility(ESlateVisibility::Hidden);
        if (LoseImage) LoseImage->SetVisibility(ESlateVisibility::Visible);
    }

    Feedback += FString::Printf(TEXT("\nYour Score: %d / 4"), Score);

    if (ResultText)
    {
        ResultText->SetText(FText::FromString(Feedback));
        ResultText->SetVisibility(ESlateVisibility::Visible);
    }

    OnQuizCompleted(Score); // Let Blueprint handle any win/lose logic or animations too
}


int32 UCrimeQuizWidget::GetScore() const
{
    int32 Score = 0;
    if (SelectedPerson == CorrectPerson) Score++;
    if (VehicleComboBox->GetSelectedOption() == CorrectVehicle) Score++;
    if (WeaponComboBox->GetSelectedOption() == CorrectWeapon) Score++;
    if (WoundedComboBox->GetSelectedOption() == CorrectWounded) Score++;
    return Score;
}
