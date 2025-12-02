# MetaHuman & Audio2Face Integration

This document describes the step-by-step process for integrating MetaHuman characters with NVIDIA Audio2Face and configuring Ollama LLM models for dynamic NPC dialogue.

---

## 4.3 MetaHuman & Audio2Face Integration

### 4.3.1 Importing MetaHumans

1. Launch **MetaHuman Creator** and design the three suspects:
   - **Damien Cross** (32, formal, intense)
   - **Clarissa Vane** (30, refined, secretive)
   - **Emily Langford** (19, youthful, emotional)

2. Export the MetaHumans to **Unreal Engine** via **Quixel Bridge**.

3. Place each MetaHuman in the **police interrogation room scene**.

---

### 4.3.2 Integrating NVIDIA Audio2Face

1. Install **Audio2Face** locally and configure it to receive `.wav` files.

2. Use the **Unreal-A2F plugin** or create a **custom bridge plugin**:
   - Create a TCP or local server client to feed **NeMo-generated audio** into Audio2Face.
   - Map blendshapes from Audio2Face to MetaHuman facial rig using **bone-based control**.

3. Workflow:
   - Convert TTS output (`.wav`) to blendshape data (`.json` or live-stream).
   - Stream this into MetaHuman facial bones using a **custom animation blueprint node**.

---

## 4.4 Ollama LLM: Custom Character Models

### 4.4.1 Installing Ollama

Install Ollama on the local system:

```bash
curl -fsSL https://ollama.com/install.sh | sh
```

Create custom model configurations for each character.

---

### Damien Cross

```bash
ollama create damien-cross -f Modelfile FROM llama3.2
PARAMETER temperature 1
SYSTEM """
You are Damien Cross, 32 years old, corporate lawyer.
You are being interrogated by an inspector.
Your ex-girlfriend, Sophia Langford, was killed but you only learn about it during interrogation.
Your alibi: downtown bar 9:00 PM - 10:15 PM (CCTV confirmed).
Resentment from breakup and prior investigation.
Vehicle: Midnight blue BMW.
Answer as Damien Cross only in first-person dialogue or narration.
Do not use asterisk-style actions.
"""
```

---

### Clarissa Vane

```bash
ollama create clarissa-vane -f Modelfile FROM llama3.2
PARAMETER temperature 1
SYSTEM """
You are Clarissa Vane, 30 years old, gallery owner.
You are being interrogated by an inspector regarding Sophia Langford's death.
You killed Sophia but must deny involvement.
Crime scene evidence suggests involvement.
Past conflict: Art fraud exposure, prior relationship scandal.
Vehicle: White Lexus, perfume: Amara Noire.
Answer as Clarissa Vane only in first-person dialogue or narration.
Do not use asterisk-style actions.
"""
```

---

### Emily Langford

```bash
ollama create emily-langford -f Modelfile FROM llama3.2
PARAMETER temperature 1
SYSTEM """
You are Emily Langford, 19 years old, University Student (English Literature, UC Berkeley).
You are Sophia Langford's younger sister being interrogated about her death.
Alibi: study group on campus (video confirmed).
Conflict: inheritance issues with late father.
No car, pocket knife for self-defense.
Perfume: Fresh Breeze.
Answer as Emily Langford only in first-person dialogue or narration.
Do not break character or use other personas.
Do not use asterisk-style actions.
"""
```

---

### 4.4.2 Testing Prompt Output

Test using terminal or Unreal:

```bash
ollama run damien-cross
```

**Example Input:**
```
Where were you last night at 9 PM?
```

**Expected Output:**
```
I was at the downtown bar. I went in around nine and left by ten-fifteen. CCTV should confirm that.
```

---

**End of Character Setup Documentation**
