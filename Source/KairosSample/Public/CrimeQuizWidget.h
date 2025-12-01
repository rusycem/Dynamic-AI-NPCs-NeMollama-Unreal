// /* * SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved. * SPDX-License-Identifier: MIT * * Permission is hereby granted, free of charge, to any person obtaining a * copy of this software and associated documentation files (the "Software"), * to deal in the Software without restriction, including without limitation * the rights to use, copy, modify, merge, publish, distribute, sublicense, * and/or sell copies of the Software, and to permit persons to whom the * Software is furnished to do so, subject to the following conditions: * * The above copyright notice and this permission notice shall be included in * all copies or substantial portions of the Software. * * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER * DEALINGS IN THE SOFTWARE. */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CrimeQuizWidget.generated.h"


UCLASS()
class KAIROSSAMPLE_API UCrimeQuizWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    // === Widgets ===
    UPROPERTY(meta = (BindWidget)) UButton* PersonAButton;
    UPROPERTY(meta = (BindWidget)) UButton* PersonBButton;
    UPROPERTY(meta = (BindWidget)) UButton* PersonCButton;

    UPROPERTY(meta = (BindWidget)) UComboBoxString* VehicleComboBox;
    UPROPERTY(meta = (BindWidget)) UComboBoxString* WeaponComboBox;
    UPROPERTY(meta = (BindWidget)) UComboBoxString* WoundedComboBox;

    UPROPERTY(meta = (BindWidget)) UButton* ConfirmButton;
    UPROPERTY(meta = (BindWidget)) UTextBlock* ResultText;

    UPROPERTY(meta = (BindWidget)) UImage* WinImage;
    UPROPERTY(meta = (BindWidget)) UImage* LoseImage;

    // === Correct Answers ===
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Correct Answers")
    FString CorrectPerson = TEXT("C");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Correct Answers")
    FString CorrectVehicle = TEXT("Car B");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Correct Answers")
    FString CorrectWeapon = TEXT("Gun");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Correct Answers")
    FString CorrectWounded = TEXT("Yes");

    // === Exposed to Blueprint when quiz ends ===
    UFUNCTION(BlueprintImplementableEvent, Category = "Quiz")
    void OnQuizCompleted(int32 FinalScore);

private:
    FString SelectedPerson;

    UFUNCTION() void OnPersonASelected();
    UFUNCTION() void OnPersonBSelected();
    UFUNCTION() void OnPersonCSelected();
    UFUNCTION() void OnConfirmPressed();
    UFUNCTION() void CheckIfAllAnswersSelected();

    // 🔄 Wrapper for ComboBox change delegate
    UFUNCTION() void OnComboBoxSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

    int32 GetScore() const;
    void UpdatePersonButtonStyles();

    const FLinearColor SelectedColor = FLinearColor(1.f, 1.f, 0.f, 1.f); // Yellow
    const FLinearColor NormalColor = FLinearColor(1.f, 1.f, 1.f, 1.f);   // White
};
