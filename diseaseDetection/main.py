import torch
import torch.nn as nn
import torchvision.models as models
from torchvision import transforms
from PIL import Image

import warnings

warnings.filterwarnings("ignore", category=UserWarning, message=".*deprecated.*")

model = models.efficientnet_b0(pretrained=False, num_classes=2)

model.features[0][0] = nn.Conv2d(
    in_channels=1,
    out_channels=model.features[0][0].out_channels,
    kernel_size=model.features[0][0].kernel_size,
    stride=model.features[0][0].stride,
    padding=model.features[0][0].padding,
    bias=False
)

num_classes = 2
in_features = model.classifier[1].in_features
model.classifier[1] = nn.Linear(in_features, num_classes)

model.load_state_dict(torch.load(r"D:\\Lettuce Classifier\\lettuce_classifier.pth", map_location=torch.device('cpu')))

model.eval()



classes = ["Healthy", "Diseased"]

transform = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize(mean=[0.5], std=[0.5]) 
])

image_path = r"C:\Users\Home\Desktop\test.jpeg" 
image = Image.open(image_path).convert('L')
input_tensor = transform(image).unsqueeze(0) 

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
input_tensor = input_tensor.to(device)
model = model.to(device)

with torch.no_grad():
    output = model(input_tensor)
    predicted_class = torch.argmax(output, dim=1).item()

print(f"Prediction: {classes[predicted_class]}")
