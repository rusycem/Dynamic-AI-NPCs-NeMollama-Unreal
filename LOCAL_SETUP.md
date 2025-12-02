# 🔙 Local Development Directory Structure

This shows how the project is organized on my local machine:

1. Ollama
   `C:/Users/username/AppData/Local/Programs/Ollama`

2. Linux/Ubuntu (WSL2)
   `/home/username/run_nemo.sh`
   `/home/username/miniconda3`
   `/home/username/ollama_bridge.py`

3. Custom model files
   `C:/Users/username/OneDrive/Desktop/modelfile`
   - ask_ollama.bat
   - Modelfile
   - my.txt
   - ollama.json
   - python-clientsscriptsttstalk

4. NeMo TTS scripts & models
   `D:/Users/username/Documents/Visual Studio Code Projects/tts_en_multispeaker_fastpitchhifigan`
   - audio.wav
   - tts_en_fastpitch_multispeaker.nemo
   - tts_en_hifitts_hifigan_ft_fastpitch.nemo
   - tts_inference.py
   - tts_runner.py

5. Unreal Engine 5 Project
   - Content/
   - Scripts/

## ⚙️ External Dependencies

Some components are not included in the main repo due to size or licensing restrictions.  
You can download it here at [ExternalDependencies](ExternalDependencies) and extract them into the `path` folder.

1. **NeMo TTS Models**  
   - File: `tts_en_multispeaker_fastpitchhifigan.7z`  
   - Path after extraction: `D:/Users/username/Documents/Visual Studio Code Projects/tts_en_multispeaker_fastpitchhifigan`  

2. **Custom Ollama Character Models**  
   - File: `modelfile.7z`  
   - Path after extraction: `C:/Users/username/OneDrive/Desktop/modelfile`
  
## ⚙️ Downloading NVIDIA NeMo TTS Models

This project uses **NVIDIA NeMo TTS** with the **FastPitch** model and **HiFi-GAN vocoder** for NPC speech synthesis.  
Due to licensing and size restrictions, the model files are **not included in this repository**. You must download them manually.

### Required Models

Inside the folder `ExternalDependencies/tts_en_multispeaker_fastpitchhifigan/`, place the following files:

1. `tts_en_fastpitch_multispeaker.nemo`  
2. `tts_en_hifitts_hifigan_ft_fastpitch.nemo`

### Download Instructions

1. Go to the official NVIDIA NGC catalog page: [NeMo TTS FastPitch + HiFi-GAN](https://catalog.ngc.nvidia.com/orgs/nvidia/teams/nemo/models/tts_en_multispeaker_fastpitchhifigan)  
2. Download the `.nemo` files listed above.  
3. Place them inside:

```
ExternalDependencies/tts_en_multispeaker_fastpitchhifigan/
```

> After this step, the Python inference script (`tts_inference.py`) will be able to locate the models for speech synthesis in Unreal Engine.


