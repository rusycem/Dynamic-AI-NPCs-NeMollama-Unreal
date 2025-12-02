# Integrating NeMo TTS (FastPitch)

This document details the integration of **NVIDIA NeMo TTS** using the **FastPitch** model with **HiFi-GAN** vocoder for NPC dialogue synthesis in Unreal Engine 5. The pipeline runs locally through **WSL2 (Ubuntu)** and **Conda**, enabling semi-offline operation without relying on cloud licenses.

---

## 1.0 Local NeMo Repository Setup

1. **Install WSL2 with Ubuntu**

```bash
wsl --install -d Ubuntu
wsl -d Ubuntu
```

Update system packages:

```bash
sudo apt update && sudo apt upgrade -y
```

2. **Install Conda (Miniconda recommended)**

```bash
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh -O ~/miniconda.sh
bash ~/miniconda.sh
source ~/.bashrc
conda --version
```

3. **Create NeMo environment**

```bash
conda create --name nemo python=3.10.12
conda activate nemo
```

4. **Install NeMo TTS dependencies**

```bash
pip install "nemo_toolkit[tts]"
sudo apt install ffmpeg -y
```

5. **Verify installation**

```bash
python -c "import nemo.collections.tts as tts; print(tts.__version__)"
```

---

## 2.0 TTS Inference Script

Inside WSL, create `tts_inference.py` to load FastPitch + HiFi-GAN models, tokenize input text, generate mel spectrograms, and synthesize `.wav` audio:

```python
from nemo.collections.tts.models import FastPitchModel, HifiGanModel
import soundfile as sf

# Load pretrained models from NGC
spec_generator = FastPitchModel.from_pretrained("tts_en_fastpitch_multispeaker")
vocoder = HifiGanModel.from_pretrained("tts_en_hifitts_hifigan_ft_fastpitch")

# Generate spectrogram + audio
tokens = spec_generator.parse("Hello! This is a test.")
spectrogram = spec_generator.generate_spectrogram(tokens=tokens, speaker=10)
audio = vocoder.convert_spectrogram_to_audio(spec=spectrogram)

# Save audio
sf.write("output.wav", audio.cpu().numpy(), 44100)
```

Run the script in WSL2:

```bash
conda activate nemo
python tts_inference.py
```

---

## 3.0 C++ Helper Class (TTSHelper) in UE5

To bridge **Unreal Engine 5** with **NeMo FastPitch** in WSL2, a custom **C++ helper class** `TTSHelper` was implemented. It handles:

- Process execution
- Command-line argument passing
- Linux-to-Windows path conversion

### Workflow

1. **Input:** NPC dialogue text + speaker ID → `RunTTSCommand`.  
2. **Execution:** Launch `wsl.exe` from Windows to run `run_nemo.sh` inside Ubuntu, which calls `tts_inference.py`.  
3. **Audio Output:** `.wav` file generated inside shared `/mnt/...` filesystem.  
4. **Path Conversion:** Linux path → Windows path for Unreal Engine access.  
5. **Integration:** `.wav` played in UE5 or sent to Audio2Face for facial animation.

### Example C++ Implementation

```cpp
void UTTSHelper::RunTTSCommand(const FString& TextToSpeak, const FString& Voice) {
    const FString WSLExe = TEXT("C:\\Windows\\System32\\wsl.exe");
    const FString RunScript = TEXT("/home/muhdi/run_nemo.sh");
    const FString OutLinux = TEXT("/mnt/d/.../audio.wav");
    FString EscapedText = TextToSpeak;
    EscapedText.ReplaceInline(TEXT("\""), TEXT("\\\""));

    FString Args = FString::Printf(TEXT("-d Ubuntu %s \"%s\" %s \"%s\""),
        *RunScript, *EscapedText, *Voice, *OutLinux);

    FProcHandle Proc = FPlatformProcess::CreateProc(
        *WSLExe, *Args, false, true, false, nullptr, 0, nullptr, nullptr
    );

    FString OutWin = LinuxToWin(OutLinux);
    if (FPaths::FileExists(OutWin)) {
        UE_LOG(LogTemp, Log, TEXT("NeMo audio ready: %s"), *OutWin);
    }
}
```

**Key Features:**

- Cross-platform bridging: `/mnt/...` → `D:\...`  
- Process control: `FPlatformProcess` monitors NeMo inference  
- Timeout handling: Prevents UE hang if TTS stalls  
- Extensible: Output `.wav` can feed MetaHuman audio or Audio2Face

---

## 4.0 Playing Audio in Unreal

Once `.wav` is generated:

```cpp
USoundWave* SW = USoundWaveProceduralLibrary::LoadFromFile(AudioPath);
UGameplayStatics::PlaySoundAtLocation(World, SW, NPCActor->GetActorLocation());
```

---

## 5.0 Enhancements & Considerations

- **Security:** Load tokens/credentials from environment variables or config files  
- **Voice Selection UI:** Supports `--list-voices` for dynamic selection  
- **Streaming Option:** `--stream` flag for faster, low-latency playback
