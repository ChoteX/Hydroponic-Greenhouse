# Hydroponic-Greenhouse
Automated Greenhouse + AI disease detection

Team members: Dachi Tchotashvili, Levan Lolashvili, Dea Nozadze, Nika Gegenava



# 🥬 Lettuce Health Classifier

This project uses a modified EfficientNet-B0 model to classify grayscale images of lettuce leaves as either **Healthy** or **Diseased**.

---

## 🧠 Model Overview

- **Base Model**: EfficientNet-B0
- **Input**: Grayscale image (1-channel)
- **Output Classes**: 
  - `Healthy`
  - `Diseased`
- **Checkpoint**: `lettuce_classifier.pth`

---

## 🛠️ Requirements

Install dependencies using pip:

```bash
pip install torch torchvision pillow


