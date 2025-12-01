// /* * SPDX-FileCopyrightText: Copyright (c) 2024 NVIDIA CORPORATION & AFFILIATES. All rights reserved. * SPDX-License-Identifier: MIT * * Permission is hereby granted, free of charge, to any person obtaining a * copy of this software and associated documentation files (the "Software"), * to deal in the Software without restriction, including without limitation * the rights to use, copy, modify, merge, publish, distribute, sublicense, * and/or sell copies of the Software, and to permit persons to whom the * Software is furnished to do so, subject to the following conditions: * * The above copyright notice and this permission notice shall be included in * all copies or substantial portions of the Software. * * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER * DEALINGS IN THE SOFTWARE. */


#include "TTSHelper.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/ScopeExit.h"
#include "HAL/PlatformProcess.h"
#include "HAL/PlatformFilemanager.h"


static FString LinuxToWin(const FString& LinuxPath) {
    // Very basic converter for /mnt/d/... -> D:\...
    if (LinuxPath.StartsWith(TEXT("/mnt/"))) {
        TCHAR Drive = FChar::ToUpper(LinuxPath[5]);        // e.g. 'd'
        FString Rest = LinuxPath.Mid(7);                   // skip "/mnt/X/"
        Rest.ReplaceInline(TEXT("/"), TEXT("\\"));
        return FString::Printf(TEXT("%c:\\%s"), Drive, *Rest);
    }
    return LinuxPath;
}

void UTTSHelper::RunTTSCommand(const FString& TextToSpeak, const FString& Voice)
{
    // wsl exe
    const FString WSLExe = TEXT("C:\\Windows\\System32\\wsl.exe");

    // WSL wrapper and arguments
    const FString WSL_D = TEXT("-d Ubuntu"); // include -d flag as part of args
    const FString RunScript = TEXT("/home/muhdi/run_nemo.sh");

    // Output path (WSL style)
    const FString OutLinux = TEXT("/mnt/d/Users/muhdi/Documents/Visual Studio Code Projects/tts_en_multispeaker_fastpitchhifigan/audio.wav");

    // Escape double-quotes in text to avoid breaking the shell quoting
    FString EscapedText = TextToSpeak;
    EscapedText.ReplaceInline(TEXT("\""), TEXT("\\\""));

    // Build args: the entire trailing part passed to wsl.exe
    FString Args = FString::Printf(TEXT("%s %s \"%s\" %s \"%s\""),
        *WSL_D,                // -d Ubuntu
        *RunScript,            // /home/muhdi/run_nemo.sh
        *EscapedText,          // "text"
        *Voice,                // speaker id or voice label
        *OutLinux              // output file path
    );

    UE_LOG(LogTemp, Warning, TEXT("Launching: %s %s"), *WSLExe, *Args);

    // Launch and wait (you can adapt to non-blocking or capture stdout if desired)
    void* ReadPipe = nullptr;
    void* WritePipe = nullptr;
    FPlatformProcess::CreatePipe(ReadPipe, WritePipe);

    FProcHandle Proc = FPlatformProcess::CreateProc(
        *WSLExe,
        *Args,
        /*bLaunchDetached=*/ false,
        /*bLaunchHidden=*/ true,
        /*bLaunchReallyHidden=*/ false,
        /*OutProcessID=*/ nullptr,
        /*Priority=*/ 0,
        /*OptionalWorkingDirectory=*/ nullptr,
        /*PipeWrite=*/ WritePipe
    );

    if (!Proc.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to start NeMo TTS process."));
        FPlatformProcess::ClosePipe(ReadPipe, WritePipe);
        return;
    }

    // Wait loop: collect output, timeout if needed
    const double Start = FPlatformTime::Seconds();
    const double Timeout = 180.0; // adjust if you want longer
    while (FPlatformProcess::IsProcRunning(Proc))
    {
        FString Out = FPlatformProcess::ReadPipe(ReadPipe);
        if (!Out.IsEmpty()) UE_LOG(LogTemp, Log, TEXT("%s"), *Out);
        if (FPlatformTime::Seconds() - Start > Timeout)
        {
            UE_LOG(LogTemp, Error, TEXT("NeMo TTS timed out; terminating"));
            FPlatformProcess::TerminateProc(Proc, true);
            break;
        }
        FPlatformProcess::Sleep(0.05f);
    }

    // Drain
    FString Remaining = FPlatformProcess::ReadPipe(ReadPipe);
    if (!Remaining.IsEmpty()) UE_LOG(LogTemp, Log, TEXT("%s"), *Remaining);

    int32 ReturnCode = 0;
    if (FPlatformProcess::GetProcReturnCode(Proc, &ReturnCode))
    {
        UE_LOG(LogTemp, Log, TEXT("NeMo TTS exited with code %d"), ReturnCode);
    }

    FPlatformProcess::ClosePipe(ReadPipe, WritePipe);

    // Convert Linux path to Windows path to load/play file
    auto LinuxToWindows = [](const FString& LinuxPath)->FString {
        if (LinuxPath.StartsWith(TEXT("/mnt/"))) {
            TCHAR Drive = FChar::ToUpper(LinuxPath[5]);
            FString Rest = LinuxPath.Mid(7);
            Rest.ReplaceInline(TEXT("/"), TEXT("\\"));
            return FString::Printf(TEXT("%c:\\%s"), Drive, *Rest);
        }
        return LinuxPath;
        };

    FString OutWin = LinuxToWindows(OutLinux);
    if (!FPaths::FileExists(OutWin)) {
        UE_LOG(LogTemp, Error, TEXT("Expected output wav not found: %s"), *OutWin);
        return;
    }

    // At this point you can load/play the WAV or send to A2F.
    UE_LOG(LogTemp, Log, TEXT("NeMo audio ready: %s"), *OutWin);
}
