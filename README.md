# DYNAMIC AI-DRIVEN NPCS WITH REAL-TIME ADAPTIVE CONVERSATIONS IN UNREAL ENGINE 5
## Abstract

This project presents a practical, semi-offline 
pipeline for dynamic, AI-driven Non-Playable Characters (NPCs) 
implemented in Unreal Engine 5. Motivated by concerns over 
latency, cloud dependency, and user privacy inherent in fully 
cloud-hosted conversational platforms, the project integrates local 
and evaluation-licensed components to enable near real-time 
conversational interaction and expressive facial animation. The 
architecture pairs an on-premises Ollama-hosted LLaMA 3.2 
model for natural language generation, NVIDIA NeMo FastPitch 
for local text-to-speech synthesis, and NVIDIA ACE Audio2Face 
for facial animation. Development followed an iterative Game 
Development Life Cycle (GDLC) with prototype builds, asset 
integration using MetaHuman characters, and functionality 
testing. The resulting UE5 prototype processes player dialogue, 
synthesizes speech, and drives facial expressions in near real-time, 
demonstrating reduced reliance on external cloud services while 
highlighting remaining constraints—most notably Audio2Face’s 
dependence on NVIDIA’s evaluation ecosystem. Evaluation shows 
that text generation and speech synthesis can be achieved fully 
offline, whereas high-fidelity facial animation currently requires 
vendor-tied tooling. This work contributes a feasible hybrid 
workflow that balances immersion, privacy, and creative control, 
and it outlines future directions toward fully local facial 
animation, latency optimization, quantitative user-experience 
assessment, and broader adoption in interactive storytelling.

### Software Requirements 

1. Unreal Engine 5.4.4 – Core development platform (≈41.6 GB). Unreal hosts 
the MetaHuman characters and runs the Audio2Face integration (Live Link / 
plugin). 
2. MetaHuman (Default UE5) – Prebuilt high-fidelity character assets and facial 
rigs.
3. Ollama / LLaMA 3.2 – Local LLM for NPC dialogue. Ollama is executed via 
command-line (cmd / PowerShell) and invoked from Python helper scripts 
(e.g., ollama run <model>) to send prompts and receive JSON responses. 
4. NVIDIA NeMo FastPitch – TTS synthesis toolchain. NeMo is installed and 
executed inside WSL2 (Linux) using a Conda environment (required by 
NeMo), enabling compatibility with Linux-only tooling and model 
dependencies. The WSL2 instance must have GPU support enabled for 
accelerated inference. 
5. NVIDIA ACE / Audio2Face Kairos Template – Audio-to-blendshape service used to generate 
facial animation from TTS audio. Audio2Face Kairos can be accessed from official website and may require NVIDIA developer 
access and licensing for authoring microservices. 
6. Python 3.13 – Host scripts for orchestration, TTS runner wrappers, console 
logging, and CSV export. Python scripts call Ollama (via cmd) and interact 
with NeMo in WSL (via subprocess/SSH or by running scripts inside WSL). 
7. Shell / Command utilities – Windows cmd / PowerShell for launching Ollama 
and Unreal helper scripts; WSL bash for running Conda/NeMo command

### NVIDIA ACE

For details on NVIDIA ACE, please visit the main [ACE documentation website](https://docs.nvidia.com/ace/latest/index.html).

### Kairos Sample Project

The documentation for the Kairos Unreal Engine sample project can be found in the ACE documentation as well, please visit [the Kairos section here](https://docs.nvidia.com/ace/latest/workflows/kairos/kairos-unreal-sample-project.html).
