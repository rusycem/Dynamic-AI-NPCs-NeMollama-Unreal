# DYNAMIC AI-DRIVEN NPCS WITH REAL-TIME ADAPTIVE CONVERSATIONS IN UNREAL ENGINE 5

> **Offline / Hybrid AI Pipeline for Fully Reactive NPC Dialogue, TTS, and Facial Animation in UE5**  
> Powered by **Ollama (LLaMA 3.2)**, **NVIDIA NeMo FastPitch**, and **NVIDIA ACE Audio2Face**.

<div align="center">

![UE5](https://img.shields.io/badge/Engine-Unreal%20Engine%205.4-blue?logo=unrealengine)
![Python](https://img.shields.io/badge/Python-3.13-yellow?logo=python)
![NVIDIA](https://img.shields.io/badge/NVIDIA-ACE%20%7C%20NeMo-green?logo=nvidia)
![License](https://img.shields.io/badge/Status-Prototype-orange)

</div>

---

## 📘 Abstract

This project presents a **semi-offline AI pipeline** for dynamic, conversational Non-Playable Characters (NPCs) in **Unreal Engine 5**.  
Instead of relying entirely on cloud APIs, the system focuses on **local text generation**, **local text-to-speech synthesis**, and **real-time facial animation**.

The architecture integrates:

- 🧠 **Ollama-hosted LLaMA 3.2** → Contextual NPC dialogue  
- 🔊 **NVIDIA NeMo FastPitch (WSL2)** → High-quality, low-latency TTS  
- 😀 **NVIDIA ACE Audio2Face** → Facial animation & blendshape generation  
- 🧍 **MetaHuman** → Final in-engine expressive performance  

The final prototype supports **real-time player–NPC conversations** with near-instant speech playback and dynamic facial animation.  
While LLM and TTS components run fully offline, **Audio2Face remains the main vendor-dependent limitation**, requiring NVIDIA tools and licensing.

This work demonstrates a **feasible hybrid workflow** balancing immersion, privacy, and creative control—while highlighting future opportunities for optimization and localized facial animation.

---

## 🛠️ Software Requirements

| Component | Purpose | Notes |
|----------|---------|-------|
| **Unreal Engine 5.4.4** | Core game engine | Hosts MetaHumans + A2F integration |
| **MetaHuman (UE5)** | High-fidelity character rig | Default UE5 plugin |
| **Ollama / LLaMA 3.2** | Local LLM for dialogue | Called via Python subprocess (cmd) |
| **NVIDIA NeMo FastPitch** | TTS engine | Runs in **WSL2**, requires Conda + GPU |
| **NVIDIA ACE / Audio2Face (Kairos Template)** | Audio→Blendshape | Requires NVIDIA developer access |
| **Python 3.13** | Pipeline orchestration | Calls Ollama & WSL2 NeMo |
| **cmd / PowerShell / WSL** | Shell automation | Required for cross-environment workflow |

---

## 📐 Architecture Overview

<div align="center">

<img width="505" height="396" alt="Pipeline Flowchart" src="https://github.com/user-attachments/assets/b44bf4ca-2dcc-4e9d-9c30-003b39319603" />

**Figure 1. Overall Pipeline Architecture**

</div>

### Pipeline Breakdown
1. **Player Input → UE Blueprint/Python Handler**  
2. **Prompt sent to Ollama (local LLaMA 3.2)**  
3. **LLM returns structured JSON response**  
4. **Python script sends text to NeMo FastPitch (WSL2)**  
5. **Generated WAV file sent to NVIDIA ACE Audio2Face**  
6. **Blendshapes streamed to UE5 via Live Link**  
7. **MetaHuman animates + audio plays in sync**

---

## 🎮 Demo

<div align="center">

<img width="856" height="479" alt="Gameplay Screenshot" src="https://github.com/user-attachments/assets/098168e8-1553-4411-a050-cb9ec62fa5fc" />

**Figure 2. In-game demonstration (UE5 MetaHuman NPC)**

</div>

---

## 📦 Installation & Setup

### 1. Clone the Repository
```bash
git clone https://github.com/<your-repo>/dynamic-ai-npc-ue5.git
cd dynamic-ai-npc-ue5
```

### 2. Install Ollama + LLaMA 3.2
For detailed character setup, prompts, and MetaHuman/Audio2Face integration, see [CHARACTER_SETUP.md](CHARACTER_SETUP.md)
```bash
curl -fsSL https://ollama.com/install.sh | sh
ollama pull llama3.2
```

Test:
```bash
ollama run llama3.2
```

### 3. Install NeMo FastPitch inside WSL2
For detailed NVIDIA NeMo, WSL, conda, and  text-to-speech integration, see [TTS_SETUP.md](TTS_SETUP.md)
```bash
wsl --install
```

Inside WSL:
```bash
conda create -n nemo python=3.10
conda activate nemo
pip install nemo_toolkit['all']
```

Check GPU support:
```bash
nvidia-smi
```

### 4. Install NVIDIA ACE (Audio2Face Kairos Template)
Download from:  
https://docs.nvidia.com/ace/latest/

### 5. Open the UE5 Project
Enable required plugins:

- Python Editor Script Plugin  
- MetaHuman  
- Live Link  
- Audio2Face / Kairos  

---

## 📁 Project Structure (GitHub Repo)
```bash
/Dynamic-AI-NPCs-NeMollama-Unreal
    Config/
    Content/
    ExternalDependencies/
        modelfile.7z
        tts_en_multispeaker_fastpitchhifigan.7z
    Plugins/VisualStudioTools/
    Source/
README.md
CHARACTER_SETUP.md
LOCAL_SETUP.md
TTS_SETUP.md
KairosSample.uproject
...
```

## ⚙️ Local Dependencies (Not Included in Repo)

This repository contains the UE5 project only. To fully enable dynamic NPC dialogues and TTS:

1. **Ollama LLaMA 3.2** must be installed locally, see [TTS_SETUP.md](TTS_SETUP.md)
2. **Custom character models** (Damien Cross, Clarissa Vane, Emily Langford) are required to setup manually, see [CHARACTER_SETUP.md](CHARACTER_SETUP.md)
3. **NVIDIA NeMo FastPitch + HiFi-GAN** models for TTS must be set up in WSL2, see [TTS_SETUP.md](TTS_SETUP.md)
4. Python helper scripts (`tts_inference.py`, `tts_runner.py`, etc.) must be placed in your local workspace, see [LOCAL_SETUP.md](LOCAL_SETUP.md)

> These files are not included in the repo due to size/licensing constraints.
> For full local directory structure, see [LOCAL_SETUP.md](LOCAL_SETUP.md)


---

## 🚀 Features
- ✔ Local LLM dialogue (no cloud)
- ✔ WSL2-accelerated TTS synthesis
- ✔ Real-time facial animation via Audio2Face
- ✔ Fully compatible with MetaHumans
- ✔ Modular Python pipeline

---

## 📈 Limitations
- ⚠ Audio2Face requires NVIDIA ecosystem + license
- ⚠ Real-time performance depends on GPU
- ⚠ Sync accuracy tied to Live Link

---

## 🔭 Future Work
- Fully local open-source alternative to Audio2Face
- Real-time phoneme-based animation (OpenFace / DeepSpeech)
- UX latency benchmarking
- Emotion tagging + expressive NPC mood models
- GPU optimization

---

## 📚 Related Documentation
**NVIDIA ACE:**  
https://docs.nvidia.com/ace/latest/index.html  

**Kairos Unreal Sample Project:**  
https://docs.nvidia.com/ace/latest/workflows/kairos/kairos-unreal-sample-project.html  

---

## 📄 License
Distributed as a research prototype.  
NVIDIA ACE components follow their respective licenses.
